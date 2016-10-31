/**
 * @file server.h
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

pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
using namespace std;

typedef struct {
  int port;
  int status;
} s_proc;

enum e_errors {
  E_OK = 0,
  E_ARGS,
  E_SOCKET,
  E_SOCKET_NAME,
  E_QUEUE,
  E_CLIENT,
  E_THREAD,
  E_SERVE_CLIENT,
  E_MESSAGE,
  E_SEND
};

enum e_nextAction {
  NONE,
  UPLOAD,
  DOWNLOAD
};

const char *PRINT_ERROR[] =
{
  "",                                                           //E_OK
  "Arguments",                                                  //E_ARGS
  "Chyba pri vytvarani socketu",                                //E_SOCKET
  "Chyba pri vytvarani mena socketu",                           //E_SOCKET_NAME
  "Chyba pri vytvarani fronty",                                 //E_QUEUE
  "Chyba spojenie s klientom",                                  //E_CLIENT
  "Chyba pri vytvarani vlakna",                                 //E_THREAD
  "Chyba pri obsluhe klienta",                                  //E_SERVE_CLIENT
  "Chyba pri prijmani spravy",                                  //E_MESSAGE
  "Chyba pri odosielani suboru"                                 //E_SEND
};

int stoi(ios_base & (*f)(ios_base&),const string &s);
void testPrint(std::string testReport, std::string testFunction);
void returnExitCode(int code, std::string more);
int send(int client, const char *fileMessage, int fileLength);
int sendFile(int client, ifstream *input, int fileLength);
void receiveFile(int client, string fileName, int fileSize);
int receiveMessage(int client);
void *serveClient(void* params);
int createServer (int port);
s_proc getProcess(int argc, char *argv[]);

 #endif
