/**
 * @file server.cpp
 *
 * Prvy projekt pre predmet IPK
 * Prenos suborov
 *
 * @author xormos00, Michal Ormos
 * xormos00@stud.fit.vutbr.cz
 * @date 9.4.2016
 */

 #include "server.h"

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
 * Odosiela spravu pomocou @package write
 * V pripade zlyhania ukonci program
 * @PARAM int client
 * @PARAM const char *fileMessage
 * @PARAM int fileLength
 * @RETURN action succesfful
 */
int send(int client, const char *fileMessage, int fileLength) {
  testPrint("Odosielam", __FUNCTION__);
  if(write(client, fileMessage, fileLength) < 0)
  {
    returnExitCode(E_SEND);
  }
  return EXIT_SUCCESS;
}

/**
 * Pripravuje subor na odoslanie pomocou jeho mena a dlzky
 * Samotne odoslanie vykonava funkcia send()
 * @package read
 * @PARAM int client
 * @PARAM ifstream *input
 * @PARAM int fileLength
 * @RETURN action successful
 */
int sendFile(int client, ifstream *input, int fileLength) {
  testPrint("Odosielam subor", __FUNCTION__);
  char buffer[BUFFER_SIZE];
  int sizeLoaded = 0;
  int status = E_OK;

  while (fileLength != 0 && status == 0) {
    if (fileLength < BUFFER_SIZE) {
      (*input).read(buffer, fileLength);
      sizeLoaded = fileLength;
      fileLength = 0;
    } else {
      (*input).read(buffer, BUFFER_SIZE);
      sizeLoaded = BUFFER_SIZE;
      fileLength -= BUFFER_SIZE;
    }
    status = send(client, buffer, sizeLoaded);
  }
  return EXIT_SUCCESS;
}

/**
 * Prijma subor
 * max velkost 1048 BUFFER_SIZE
 *
 * @PARAM int client
 * @PARAM string fileName
 * @PARAM int fileSize
 * @RETURN void
 */
 void receiveFile(int client, string fileName, int fileSize) {
  testPrint("Prijmam subor", __FUNCTION__);
   ofstream output(fileName.c_str(), ios::binary);
   char buffer[BUFFER_SIZE];
   int receivedStatus = 0;
   while(fileSize > receivedStatus) {
     int numBytes = recv(client, &buffer, BUFFER_SIZE, 0);
     if (numBytes > 0) {
       output.write(buffer, numBytes);
       receivedStatus += numBytes;
     }
   }
   output.close();
 }

 /**
  * Prijatie spravy od klienta
  * Vzhladom na predom dohodnute hlavicky komunikacie
  * sa rozhodne ci subor odosielame alebo prijmame
  * V pripade chyby sa program ukonci s chybovym hlasenim
  * @PARAM char client
  * @RETURN int status
  */
int receiveMessage(int client) {
  testPrint("Prijmam spravu", __FUNCTION__);
  bool flag = true;
  e_nextAction nextAction = NONE;
  char buffer;
  string line = "";
  string fileName = "";
  int fileSize = -1;
  // int status = E_OK;

  // Parsuj hlavicku
  while(flag and recv(client, &buffer, 1, 0) == 1) {
    line += buffer;
    if(buffer == '\n') {
      // Rozslis typ spravy podla klucovych slov
      if (int(line.find("IPKCONNECTION ")) == 0) {
        if (line.substr(14) == "UPLOAD\n") {
          nextAction = UPLOAD;
        }
        if (line.substr(14) == "DOWNLOAD\n") {
          nextAction = DOWNLOAD;
        }
      } else if (int(line.find("filename: ") == 0)) {
        fileName = line.substr(10, line.length()-10-1);
      } else if (int(line.find("size: ")) == 0) {
        fileSize = stoi(dec, line.substr(6, line.length()-6-1));
      }  else if (line == "\n") {
          flag = false;
      }
      line.clear();
    }
  }

  if (fileName == "" || nextAction == NONE) {
    returnExitCode(E_MESSAGE, fileName);
  } else if (nextAction == UPLOAD) {
    if (fileSize >= 0) {
      receiveFile(client, fileName, fileSize);
    }
  } else if (nextAction == DOWNLOAD) {
    ifstream input(fileName.c_str(), ios::binary);
    input.seekg(0, ios::end);
    int fileLength = input.tellg();
    input.seekg(0, ios::beg);
    // If file don't exist
    if (fileLength < 0) {
      string replyMessage = "IPKCONNECTION NONEXIST\n\n";
      // status = send(client, replyMessage.c_str(), (int)replyMessage.length());
      send(client, replyMessage.c_str(), (int)replyMessage.length());
    // If file really exist
    } else {
      ostringstream fileStream;
      fileStream << fileLength;
      string replyMessage = "IPKCONNECTION EXIST\n";
      replyMessage += "size: " + fileStream.str() + "\n\n";

      // status = send(client, replyMessage.c_str(), (int)replyMessage.length());
      send(client, replyMessage.c_str(), (int)replyMessage.length());

      // status = sendFile(client, &input, fileLength);
      sendFile(client, &input, fileLength);
    }
    input.close();
  }
  return EXIT_SUCCESS;
}

/**
 * Obsluha klienta cez semafor
 *
 * @PARAM fronta sprav
 * @RETURN NULL
 */
 void *serveClient(void* params) {
   queue<int> * myQueue = (queue<int> *) params;

   //synchronizacia vlakien
   pthread_mutex_lock (&MUTEX);
   int client = (*myQueue).front();
   (*myQueue).pop();
   pthread_mutex_unlock(&MUTEX);

   receiveMessage(client);
  //  if(receiveMessage(client) != E_OK) {
  //    returnExitCode(E_SERVE_CLIENT);
  //  }
   return NULL;
 }

/**
 * Vytvara server
 * Vytvara socket a frontu
 * Naslende pomocou while(1) vyckava na pripojenie klienta a obsluhu
 * V pripade chyby sa program ukonci a vrati chybovy kod
 * @PARAM int port vstupny port
 * @RETURN action successful
 */
 int createServer (int port) {
   testPrint("Vytvaram server", __FUNCTION__);
   int socketNum = -1;
   //Vytvorime socket
   if ((socketNum = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
   {
     returnExitCode(E_ARGS);
   }

   sockaddr_in socketName;
   sockaddr_in clientInfo;
   socklen_t addressLen;

   socketName.sin_family = AF_INET;
   socketName.sin_port = htons(port);
   socketName.sin_addr.s_addr = INADDR_ANY;

   //Prirad socketu meno
   if (bind(socketNum, (sockaddr *)&socketName, sizeof(socketName)) == -1)
   {
     returnExitCode(E_ARGS);
   }

   //Vytvor frontu
   if (listen(socketNum, 10) == -1)
   {
    returnExitCode(E_ARGS);
   }

   addressLen = sizeof(clientInfo);
   queue<int> myQueue;

   // Zaciname sa pripajat ku klientovi
   while(1) {
     testPrint("While start", __FUNCTION__);
     int clientNumber = accept(socketNum, (sockaddr*)&clientInfo, &addressLen);
     if (clientNumber == -1) {
       returnExitCode(E_CLIENT);
     } else {
       myQueue.push(clientNumber);
       pthread_t thread;
       pthread_attr_t attribute;
       pthread_attr_init(&attribute);
       pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_DETACHED);
       if(pthread_create(&thread, &attribute, serveClient, (void*) &myQueue))
       {
         returnExitCode(E_THREAD);
       }
       pthread_attr_destroy(&attribute);
     }
     testPrint("While end", __FUNCTION__);
   }
   return EXIT_SUCCESS;
 }

 /**
  * Spravuje argument -p pre server
  * Nasledne ho osetri
  * @PARAM argumenty z prikazoveho riadku
  * @RETURN v pripade uspechu vrati port a status cez struct
  * inak konci program s chybou
  */
 s_proc getProcess(int argc, char *argv[]) {
   testPrint("Starting", __FUNCTION__);
   s_proc retCode;
   retCode.port = -1;
   retCode.status = E_OK;
   char argument;
   string str;
   while ((argument = getopt(argc,argv, "p:")) != -1) {
     if (argument == 'p') {
       str.assign(optarg);
       retCode.port = stoi(dec, str);
       break;
     } else {
 		  returnExitCode(E_ARGS);
     }
   }
   if (retCode.port < 1)
   {
     returnExitCode(E_ARGS);
   }
   return retCode;
 }

/**
 * Vola parsovanie argumentov a naslede vytvara server
 * @PARAM vstupne parametry
 * @RETURN action successful
 */
int main(int argc, char *argv[]) {
  s_proc retProcess = getProcess(argc, argv);
  // int status = retProcess.status;

  testPrint("Main return retCode", __FUNCTION__);

  //vytvor server
  createServer(retProcess.port);

  return EXIT_SUCCESS;
}
