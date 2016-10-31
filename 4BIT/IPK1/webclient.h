/**
 * @file webclient
 *
 * Prvy projekt pre predmet IPK
 * Webovy klient
 *
 * @author xormos00, Michal Ormos
 * xormos00@stud.fit.vutbr.cz
 * @date 12.3.2016
 */

 #ifndef _WEBCLIENT_H
 	#define _WEBCLIENT_H

 #include <stdlib.h>		//EXIT_SUCCESS / EXIT_FAILURE
 #include <string> 			//std, memset
 #include <iostream>		//cout
 #include <stdio.h> 		//printf
 #include <cstring>			//memset cstring is c++ equivalent of string in c
 #include <sys/socket.h>	//sockaddr_in
 #include <regex.h>			//Regular Expresion library
 #include <netinet/in.h>	//sockaddr_in
 #include <sstream>			//std::stringstream
 #include <netdb.h>			//gethostbyname()
 #include <fstream>			//ofstream
 #include <unistd.h>		//close
 #include <cstdlib>
 #include <vector>
 #include <iterator>
 #include <sys/types.h>

 #define MAX_REDIRECTS 5
 #define BUFFER_SIZE 1024
 #define END_REG "\r\n\r\n"
 #define DEBUG 0

 using namespace std;

 typedef struct TUrl
 {
 	int port;
 	string host;
 	string path;
 	string scheme;
 } s_url;

 typedef struct TResponse
 {
 	int code;
 	bool chunked;
 	string data;
 	string headers;
 } s_response;

  enum TErrors 
 {
 	E_OK = 0,				//0
 	E_WRONG_PARAMS1,		//1
 	E_WRONG_ADDRESS,		//2
 	E_SOCKET_INIT,			//3
 	E_HOST_NAME,			//4
 	E_CONNECTION,			//5
 	E_SCHEME,				//6
 	E_HOST,					//7
 	E_SOCKET, 				//8
 	E_REQUEST,				//9
 	E_DELETEFILE,			//10
 	E_SOCKET_CREATE,		//11
 	E_WRONG_RES_CODE,		//12
 	E_REDIRECT_LIMIT		//13
 };

 const char *PRINT_ERROR[] =
 {
 	"",															//OK
 	"Error: Not enought paramteres, minumum is 2, program and URL.",	//WROND_PARAMS1
 	"Error: in regular expression  / WRONG_ADRRESS"			,	//WRONG_ADDRESS
 	"Error: Inicialization of socket"						,	//SOCKET_INIT
 	"Error: Can't get host name / IP adress"				,	//HOST_NAME
 	"Error: Connection icializing problem"					,	//CONNECTION
 	"Error: Invalid protocol, program support only HTTP"	,	//SCHEME
 	"Error: Invalid host name"								,	//HOST
 	"Error: Connect to socket"								,	//SOCKET
 	"Error: Seding socket"									,	//REQUEST
 	"Error: Erasing temporary file"							,	//DELETEFILE
 	"Error: Creating socket"								,	//SOCKET_CREATE
 	"Error: Invalid server return code"						,	//WRONG_RES_CODE
 	"Error: Max redirection used"								//REDIRECT_LIMIT
 };

 void testPrint(std::string testReport, std::string testFunction);
 void replaceASCIIChars(std::string &s);
 void parseUrl(s_url &url, string link);
 void unchunkStorage(string* storage);
 s_response openUrl(s_url &url);


 #endif 