/**
 * @file client.cpp
 *
 * Prvy projekt pre predmet IPK
 * Prenos suborov
 *
 * @author xormos00, Michal Ormos
 * xormos00@stud.fit.vutbr.cz
 * @date 9.4.2016
 */

  #include "client.h"

 /**
  * C++98 nepodporuje @PACKage stio, musim predefinovat
  * Spracuje string na integer
  * @PARAM ios_base input stream
  * @PARAM strin &s string mean to change to int
  * @RETURN string pretypovany na integer
  */
 int stoi(ios_base & (*f)(ios_base&),const string &s) {
   istringstream is(s);
   int number;
   is >> f >> number;
   return number;
 }
  /**
   * DEBUG function for debig print messages
   * @return void
   */
  void testPrint(std::string testReport, std::string testFunction) {
      if(DEBUG){
          cout << __FILE__ << " (debug):" << testFunction << ": " << testReport << endl;
      }
  }

  /**
   * Processed and printout error codes
   * @return void
   */
  void returnExitCode(int code, std::string more = "") {

      cerr << __FILE__ << ": Error " << code << " : " << PRINT_ERROR[code] << endl;

      if(more.length() > 0){
          cerr << more << endl;
      }
      exit(1);
  }
/**
 * Pripravuje upload suboru
 *
 * @PARAM int sserver
 * @PARAM string fileName
 * @RETURN
 */
int upload(int server, string fileName) {
  testPrint("client: uploadujem", __FUNCTION__);
  ifstream input(fileName.c_str(), ios::binary);
  input.seekg(0, ios::end);
  int fileLength = input.tellg();
  input.seekg(0, ios::beg);
  if (fileLength >= 0) {
    ostringstream stream;
    stream << fileLength;
    //Mame len cestu bez suborov
    fileName = fileName.substr(fileName.rfind("/") + 1);
    string message = "IPKCONNECTION UPLOAD\n";
    message += "filename: " + fileName + "\n";
    message += "size: " + stream.str() + "\n\n";
    if (send(server, message.c_str(), (int)message.length()) == 0) {
      uploadFile(server, &input, fileLength);
    }
  } else {
    returnExitCode(E_FILE);
  }
  input.close();
  return EXIT_SUCCESS;
}

/**
 * Samotne odosielanie suboru
 *
 * @PARAM int server
 * @PARAM char fileMessage
 * @PARAM int fileLength
 * @RETURN
 */
 int send(int server, const char *fileMessage, int fileLength) {
   testPrint("Client: Odosielam", __FUNCTION__);
   if(write(server, fileMessage, fileLength) < 0)
   {
     returnExitCode(E_SEND);
   }
   return EXIT_SUCCESS;
 }

 /**
  * Pripravuje subor na upload
  * Nacita a ulozi ho do bufferu
  * Dalej predava riadenie funkcii send()
  * @PARAM int server
  * @PARAM ifstream *inputFile
  * @PARAM int fileLength
  * @RETURN
  */
  int uploadFile(int server, ifstream *inputFile, int fileLength) {
    testPrint("Client: uploadujem subor", __FUNCTION__);
    char buffer[BUFFER_SIZE];
    int sizeLoaded = 0;
    int status = E_OK;

    while (fileLength != 0 && status == 0) {
      if (fileLength < BUFFER_SIZE) {
        (*inputFile).read(buffer, fileLength);
        sizeLoaded = fileLength;
        fileLength = 0;
      } else {
        (*inputFile).read(buffer, BUFFER_SIZE);
        sizeLoaded = BUFFER_SIZE;
        fileLength -= BUFFER_SIZE;
      }
      status = send(server, buffer, sizeLoaded);
    }
    return EXIT_SUCCESS;
  }

/**
 * Priprava na stiahnutie suboru
 * Overenie spravnej hlavicky na stiahnutie
 * Dalej predava riadenie funkcii downloadFile()
 * @PARAM int sevrer
 * @PARAM string fileName
 * @RETURN
 */
int download(int server, string fileName) {
   testPrint("client: Stahujem", __FUNCTION__);
  int status;
  string message = "IPKCONNECTION DOWNLOAD\n";
  message += "filename: " + fileName + "\n\n";
  status = send(server, message.c_str(), (int)message.length());
  if (status == E_OK) {
    int fileSize = downloadHeader(server);
    if(fileSize != -1) {
      downloadFile(server, fileName, fileSize);
    } else {
     returnExitCode(E_FILE);
    }
  }
  return EXIT_SUCCESS;
}

 /**
  * Samotne stahovanie suboru do bufferu
  *
  * @PARAM int server
  * @PARAM string fileName
  * @PARAM int fileSize
  * @RETURN
  */
 void downloadFile(int server, string fileName, int fileSize) {
    testPrint("client: Prijmam subor", __FUNCTION__);
    fileName = fileName.substr(fileName.rfind("/") + 1);
    ofstream output(fileName.c_str(), ios::binary);
    char buffer[BUFFER_SIZE];
    int receivedStatus = 0;
    while(fileSize > receivedStatus) {
      int numBytes = recv(server, &buffer, BUFFER_SIZE, 0);
      if (numBytes > 0) {
        receivedStatus += numBytes;
        output.write(buffer, numBytes);
      }
    }
    output.close();
  }

  /**
   * Parsovanie a kontrola spravnej hlavicky pre prenos
   * Kontrola existensie suboru a zistenie jeho velkosti
   * @PARAM int server
   * @RETURN
   */
   int downloadHeader(int server) {
     testPrint("Client: stahujem hlavicku", __FUNCTION__);
     bool flag = true;
     bool fileExist = false;
     char buffer;
     string line = "";
     int fileSize = -1;
     // int status = E_OK;

     // Parsuj hlavicku
     while(flag and recv(server, &buffer, 1, 0) == 1) {
       line += buffer;
       if(buffer == '\n') {
         // Rozslis typ spravy podla klucovych slov
         if (int(line.find("IPKCONNECTION ")) == 0) {
           if (line.substr(14) == "EXIST\n") {
             fileExist = true;
           }
         } else if (int(line.find("size: ")) == 0) {
           fileSize = stoi(dec, line.substr(6, line.length()-6-1));
         }  else if (line == "\n") {
             flag = false;
         }
         line.clear();
       }
     }
     if(!fileExist) {
       fileSize = -1;
     }
     return fileSize;
   }


/**
 * Vytvara klienta a zistuje informacie o servery, ci existuje na danom porte
 * Vracia informacie o servery pre dalsie spracovanie
 * @PARAM string hostName meno servera
 * @PARAM int port port servera
 * @RETURN vracia int odkaz na server
 */
 int createClient(string hostName, int port) {
   testPrint("Starting Client", __FUNCTION__);
   int server;
   hostent *host;
   if((host = gethostbyname(hostName.c_str())) == NULL) {
     returnExitCode(E_CONNECT);
   } else {
     // vytvor socket POZOR MEMCPY!!
     if ((server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
       returnExitCode(E_SEND);
     } else {
       sockaddr_in socketObj;
       socketObj.sin_family = PF_INET;
       socketObj.sin_port = htons(port);
       memcpy(&(socketObj.sin_addr), host->h_addr, host->h_length);
       if (connect(server,(sockaddr *)&socketObj, sizeof(socketObj)) < 0) {
         returnExitCode(E_CONNECT);
       }
     }
   }
   return server;
 }

/**
 * Spracuje vstupne parametre a uklada ich do pripravenej struktury
 * V pripade nesupechu ukoncuje program s chybovym hlasenim
 * @PARAM vstupne parametre z terminalu
 * @RETURN s_proc spracovane argumenty v strukture
 */
 s_proc getProcess(int argc, char *argv[]) {
   testPrint("Starting Client Arguments", __FUNCTION__);
   s_proc retCode;
   retCode.port = -1;
   retCode.status = E_OK;
   retCode.hostName = "";
   retCode.fileName = "";
   retCode.nextAction = NONE;
   char argument;
   string str;
   while ((argument = getopt(argc,argv, "h:p:d:u:")) != -1) {
     testPrint("Client: Ziskavam argumenty", __FUNCTION__);
     switch (argument) {
       case 'h':
        retCode.hostName.assign(optarg);
        break;
       case 'p':
        str.assign(optarg);
        retCode.port = stoi(dec, str);
        break;
       case 'd':
        if (retCode.nextAction == NONE) {
          retCode.fileName.assign(optarg);
          retCode.nextAction = DOWNLOAD;
        } else {
          returnExitCode(E_ARGS);
        }
        break;
       case 'u':
         if (retCode.nextAction == NONE) {
           retCode.fileName.assign(optarg);
           retCode.nextAction = UPLOAD;
         } else {
           returnExitCode(E_ARGS);
         }
         break;
       default:
        returnExitCode(E_ARGS);
     }
    }
  if (retCode.port < 1) {
      returnExitCode(E_ARGS_PORT);
  }
  if (retCode.hostName == "") {
      returnExitCode(E_ARGS_HOSTNAME);
  }
  if (retCode.fileName == "") {
      returnExitCode(E_ARGS_FILENAME);
  }
  if (retCode.nextAction == UPLOAD) {
      //Test existencie suboru
      ifstream file(retCode.fileName.c_str(), ios::binary);
      if(file.tellg() == -1) {
        returnExitCode(E_ARGS_NOFILE);
      }
      file.close();
  }
  testPrint("Client konci", __FUNCTION__);
  return retCode;
}

/**
 * Vola funkciu na parsovanie vstupu a vytvara klienta
 * Rozlisuje ci sa bude subor stahovat alebo posielat
 * @PARAM terminal parameters
 * @RETURN function successful
 */
int main(int argc, char *argv[]) {
  s_proc retProcess = getProcess(argc, argv);
  //int status = retProcess.status;
  int server;

  // Vytvor spojenie so serverom
  server = createClient(retProcess.hostName, retProcess.port);

  if(retProcess.nextAction == UPLOAD) {
    upload(server, retProcess.fileName);
  } else if (retProcess.nextAction == DOWNLOAD) {
    download(server, retProcess.fileName);
  }

  close(server);
  return EXIT_SUCCESS;
}
