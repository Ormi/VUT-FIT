/**
 * @file client.h
 *
 * Prvy projekt pre predmet IPK
 * Prenos suborov
 *
 * @author xormos00, Michal Ormos
 * xormos00@stud.fit.vutbr.cz
 * @date 9.4.2016
 */

 #ifndef SERVER_H
 #define SERVER_H

 #include <stdlib.h>		//EXIT_SUCCESS / EXIT_FAILURE
 #include <sstream>
 #include <iostream>
 #include <fstream>
 #include <string.h> // stoi() adn to_string()
 #include <unistd.h>
 #include <netdb.h>
 #include <netinet/in.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <queue>

 #define DEBUG 0
 #define BUFFER_SIZE 1024
 using namespace std;

 typedef struct {
   int port;
   int status;
   string hostName;
   string fileName;
   int nextAction;
 } s_proc;

 enum e_nextAction {
   NONE,
   UPLOAD,
   DOWNLOAD
 };

 enum e_errors {
   E_OK = 0,
   E_ARGS,
   E_ARGS_PORT,
   E_ARGS_HOSTNAME,
   E_ARGS_FILENAME,
   E_ARGS_NOFILE,
   E_SOCKET,
   E_SOCKET_NAME,
   E_QUEUE,
   E_CLIENT,
   E_THREAD,
   E_SERVE_CLIENT,
   E_MESSAGE,
   E_SEND,
   E_FILE,
   E_CONNECT,
   E_IP
 };
 const char *PRINT_ERROR[] =
 {
   "",                                                           //E_OK
   "Arguments",                                                  //E_ARGS
   "Chyba v porte",                                              //E_ARGS_PORT
   "Chyba v hostname",                                           //E_ARGS_HOSTNAME
   "Chyba v filename",                                           //E_ARGS_FILENAME
   "Chyba odosielania, subor neexistuje",                        //E_ARGS_NOFILE
   "Chyba pri vytvarani socketu",                                //E_SOCKET
   "Chyba pri vytvarani mena socketu",                           //E_SOCKET_NAME
   "Chyba pri vytvarani fronty",                                 //E_QUEUE
   "Chyba spojenie s klientom",                                  //E_CLIENT
   "Chyba pri vytvarani vlakna",                                 //E_THREAD
   "Chyba pri obsluhe klienta",                                  //E_SERVE_CLIENT
   "Chyba pri prijmani spravy",                                  //E_MESSAGE
   "Chyba pri posielani suboru",                                 //E_SEND
   "Chyba stahovania, pri odosielani suboru",                    //E_FILE
   "Chyba pri vytvarani spojenie pre clienta/Neznamy server",    //E_CONNECT
   "chyba pri ziskavani IP adresy"                               //E_IP
 };

 int stoi(ios_base & (*f)(ios_base&),const string &s);
 void testPrint(std::string testReport, std::string testFunction);
 void returnExitCode(int code, std::string more);
 int upload(int server, string fileName);
 int download(int server, string fileName);
 int downloadHeader(int server);
 int send(int server, const char *fileMessage, int fileLength);
 int uploadFile(int server, ifstream *inputFile, int fileLength);
 void downloadFile(int server, string fileName, int fileSize);
 int createClient(int server, string hostString, int port);
 s_proc getProcess(int argc, char *argv[]);

 #endif
