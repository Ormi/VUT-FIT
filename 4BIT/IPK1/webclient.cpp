/**
 * @file webclient.cpp
 *
 * Prvy projekt pre predmet IPK
 * Webovy klient
 *
 * @author xormos00, Michal Ormos
 * xormos00@stud.fit.vutbr.cz
 * @date 12.3.2016
 */

#include "webclient.h"

std::string originName;
/**
 * DEBUG function for debig print messages
 * @return void
 */
void testPrint(std::string testReport, std::string testFunction) {
    if(DEBUG){
        std::cout << __FILE__ << " (debug):" << testFunction << ": " << testReport << std::endl;
    }
}  

/**
 * Processed and printout error codes
 * @return void
 */ 
void returnExitCode(int code, std::string more = "") {

    std::cerr << __FILE__ << ": Error " << code << " : " << PRINT_ERROR[code] << std::endl;

    if(more.length() > 0){
        std::cerr << more << std::endl;
    }
}

/**
 * Replace bad ASCII characters
 * mainly "\ " as %20
 * @return void
 */
void replaceASCIIChars(std::string &s) {
    testPrint("Replacing ASCII char for HEXA value", __FUNCTION__);
    if(DEBUG) {
        std::cout << s << std::endl;
    }

    while(s.find(" ")!=string::npos){
        s.replace(s.find(" "), 1, "%20");
    }
    while(s.find("~")!=string::npos){
        s.replace(s.find("~"), 1, "%7E");
    }

    testPrint("Done", __FUNCTION__);
    testPrint(s, __FUNCTION__);
}

void returnASCIIChars(std::string &s) {
    testPrint("Replacing HEXA value for ASCII char", __FUNCTION__);
    if(DEBUG) {
        std::cout << s << std::endl;
    }

    while(s.find("%20")!=string::npos) {
        s.replace(s.find("%20"), 3, " ");
    }
    while(s.find("%7E")!=string::npos) {
        s.replace(s.find("%7E"), 3, "~");
    }

    testPrint("Done", __FUNCTION__);
    testPrint(s, __FUNCTION__);
}



/**
 * Parse url link given as argument into the structure s_url
 *
 * @param s_url url 
 * @param string link argument from input to parse/url link
 * @return structure of divided url link 
 */
void parseUrl(s_url &url, string link) {

	string link_copy = link;

	/* Scheme recognition */
	size_t pos = link_copy.find("://");
	if (pos == string::npos) {
		url.scheme = "http://";
		testPrint("Protocol was set: \"" + url.scheme + "\"", __FUNCTION__);		
	} else {
		url.scheme = link_copy.substr(0, pos + 3);
		link_copy.erase(0, pos + 3);
	}
		/* Scheme validation */
		if (url.scheme != "http://") {
			returnExitCode(E_SCHEME);
			return;
		}

	/* Path recognition */
	pos = link_copy.find("/");
	if (pos != string::npos) {
		url.path = link_copy.substr(pos);
		testPrint("Path was set: \"" + url.path + "\"", __FUNCTION__);		
		link_copy.erase(pos, url.path.length());
		if (url.path.find("#") != string::npos) {
			url.path.erase(url.path.find("#"));
		} 
	} else {
		url.path = "/";
	}

	/* Port recognition */
	pos = link_copy.find(":");
	if (pos != string::npos) {
		string address = link_copy.substr(pos + 1);
		link_copy.erase(pos, address.length() + 1);
		url.port = strtol(address.c_str(), NULL, 10);
		testPrint("Port was set: \"" + address + "\"", __FUNCTION__);			
	} else {
		url.port = 80;	
	}

	/* Rest is host */
	url.host = link_copy;
	replaceASCIIChars(url.path);


	testPrint("Host was set: \"" + url.host + "\"", __FUNCTION__);	
	testPrint("Protokol was set: \"" + url.scheme + "\"", __FUNCTION__);	
	testPrint("Path was set: \"" + url.path + "\"", __FUNCTION__);	
}

/**
 * Comunicate with server and get data
 *
 * @params_url &url input argument as divided strcture
 * @return responce s_responce structure of data
 */
s_response openUrl(s_url &url) {

	testPrint("openUrl", __FUNCTION__);

	// string request(
	// 				"HEAD " +  url.path + " HTTP/1.1\r\n"
	// 				"Host: " + url.host + "\r\n"
	// 				"Connection: close\r\n\r\n"
	// 				);
	string request(
						"GET " +  url.path + " HTTP/1.1\r\n"
						"Host: " + url.host + "\r\n"
						"Connection: close\r\n\r\n"
						);

	hostent *host;
	sockaddr_in serverSocket;
	int mySocket;
	char buffer[BUFFER_SIZE];
	int req_length;
	s_response response;

	/* Create my socket */
	if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		returnExitCode(E_SOCKET_CREATE);
	  	return response;
	}

	/* Loading host info */
	if ((host = gethostbyname(url.host.c_str())) == NULL) {
		returnExitCode(E_HOST);
		return response;
	}

	/* Host socket loading (type, port, IP) */
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(url.port);
	memcpy(&(serverSocket.sin_addr), host->h_addr, host->h_length);

	/* Connect to socket */
	if (connect(mySocket, (sockaddr *)&serverSocket, sizeof(serverSocket)) == -1) {
		returnExitCode(E_SOCKET);
		return response;
	}

	/* Sending request */
	if ((req_length = send(mySocket, request.c_str(), request.size(), 0)) == -1) {
		returnExitCode(E_REQUEST);
		return response;
	}

	/* Determine filename */
	string fileName;		
	size_t found;
  	found=url.path.find_last_of('/');
	if (found == std::string::npos) {	
			fileName = "index.html";			
		} else {
			returnASCIIChars(url.path);	
			fileName = url.path.substr(url.path.find_last_of('/') + 1);				
		}		
	testPrint("FILE NAME: \"" + url.path + "\"", __FUNCTION__);	
	testPrint("FILE NAME: \"" + fileName + "\"", __FUNCTION__);		
	if (fileName == "") {
		fileName = "index.html";
	}
	if (originName == "") {
		originName = fileName;
	} else {
		fileName = originName;
	}				

	/* Determine output file stream */
	ofstream outFile(fileName.c_str(), ostream::out);
	ostreambuf_iterator<char> out_it (outFile);

	string str, storage;
	while((req_length = recv(mySocket, buffer, BUFFER_SIZE, 0)) != 0) {

		testPrint("openUrl - WHILE", __FUNCTION__);

		str.erase();
		str.append(buffer, req_length);

		/* Headers */
		if (response.headers.length() == 0 && str.find(END_REG) != string::npos) {
			response.headers = str.substr(0, str.find(END_REG));

			/* Header erase, for raw data*/
			str.erase(str.begin(), str.begin()+response.headers.length() + 4);

			/* Finally get response code*/
			size_t space = response.headers.find(" ");
			response.code = atol(response.headers.substr(space + 1, 3).c_str());					

			if (response.code == 301 || response.code == 302) {
				/* REDIRECTING*/
				break;
			}

			/* Chunked connection */
			if (response.headers.find("Transfer-Encoding: chunked") != string::npos) {
				response.chunked = true;
			} else {
				response.chunked = false;
			}
		}

		/* If chunked add data to storage, else send data right to file */
		if (response.chunked) {
			storage += str;
		} else {
			copy(str.begin(), str.end(), out_it);
		}
	}

	/* Closing connection */
	close(mySocket);

	/* Send data as unchunk */
	response.data = storage;

	/* If needed unchuck data */
	if (response.chunked == true)
		unchunkStorage(&response.data);

	if (response.code != 301 && response.code != 302) {
		copy(response.data.begin(), response.data.end(), out_it);
	} else {
		if(remove(fileName.c_str()) != 0)
			returnExitCode(E_DELETEFILE);
	}

	return response;

}

/**
 * Reading chunk data and unchunk data
 *
 * @param string* storage unchunk data
 * @return void, changing directly chunk data structure
 */
void unchunkStorage(string* storage) {    
/*	
	Format of chunk message
	-----------------------
	<head><CRLF>
	<CRLF>
	{<length of chunk in hexa><CRLF>
	<data><CRLF>}*
	0<CRLF> //last chunk
*/
	string data = *storage;
	
    size_t pos = 0;
    size_t secondPos;
    
    string chunkSign;
    size_t chunkLen;
    
    string tmpData;
    string unchunked;
        
    
    /* Looking at the firs chunk to get ckunk size */
    secondPos = data.find("\r\n");
    chunkSign = data.substr(pos, secondPos-pos);
	chunkLen = strtol(chunkSign.c_str(), NULL, 16);
    
    /* Raw data from pos 0 */
    pos += secondPos + 2;
        
    /* Reading until end, last chunk is '0' */
    while (chunkLen > 0) {
		
		unchunked += data.substr(pos, chunkLen);
		pos += chunkLen + 2;

		secondPos = data.substr(pos).find("\r\n") + pos;
		chunkSign = data.substr(pos, secondPos-pos);

		chunkLen = strtol(chunkSign.c_str(), NULL, 16);
		pos = secondPos + 2; // + \r\n
	}
    
    *storage = unchunked;
}

/**
 * Dealing with return response code 
 *
 * @param input url string
 */
int main(int argc, char const *argv[])
{
	testPrint("launched", __FUNCTION__);

	originName = "";

	/* Controlling arguments*/
	if(argc != 2) {
		returnExitCode(E_WRONG_PARAMS1);
		return 1;
	}

	string url_loc = argv[1];
	s_url url;
	s_response response;

	size_t pos, pos_next;
	int redirectCounter = MAX_REDIRECTS;

	do {

		/* Parse input link to url structure */
		parseUrl(url, url_loc);		

		/* Opening comunication with server and get data */
		response = openUrl(url);

		testPrint("Response", __FUNCTION__);

		if (response.code == 200) {
			testPrint("Response=200", __FUNCTION__);
			break;
		} else 
		if (response.code == 301 || response.code == 302) {
			testPrint("Response=301/302", __FUNCTION__);			
			pos = response.headers.find("\r\nLocation: ");
			if (pos == string::npos) {
				returnExitCode(E_OK);
			}
			pos_next = response.headers.substr(pos + 2).find("\r\n") + pos + 2;

			url_loc = response.headers.substr(pos + 12, pos_next - pos - 12);

			redirectCounter--;
			continue;
		} else if (response.code >= 400 && response.code <= 505) {
			testPrint("Response=400-505", __FUNCTION__);	
			cerr << "Server return error code " << response.code << endl;
			return 1;
		} else {
			returnExitCode(E_WRONG_RES_CODE);
			cerr << "Server return error code " << response.code << endl;
			return 1;
		}

		if (redirectCounter < 0) {
			returnExitCode(E_REDIRECT_LIMIT);
			return 1;
		}
	} while ((response.code == 301 || response.code == 302) && redirectCounter >= 0);

	return 0;
}
