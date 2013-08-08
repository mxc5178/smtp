#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>
#include <iostream>

//#include "util.h"

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    //
    // ss = string stream for streaming buffer into
    // s = string for streaming ss into for splitting
    //
    stringstream ss;
    string str;

    //
    // sockfd will store a handle to a socket
    // portno will store the port number
    //
    int sockfd, portno, n;

    //
    // serv_addr is a structure that will store the Ip information for the server
    // the client wants to connect to
    //
    struct sockaddr_in serv_addr;

    //
    // *server is a pointer of type hostent; the elements of hostent will be
    // populated by the gethostbyname() function
    //
    struct hostent *server;

    char buffer[256];

    //
    // make sure that there are 3 command line arguments
    // run the program like
    // ./client.exe localhost 31000
    //
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //
    //convert the port number from alpha to int
    //
    portno = atoi(argv[2]);

    //
    // create a socket by running the socket() function
    //sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //if (sockfd < 0)
    // error("ERROR opening socket");
    //
    // get the server Ip information(address)
    // gethostbyname() fills the hostent struct
    //
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //
    //clear the serv_addr struct
    //
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    //
    //copy the server address from the hostent struct into the serv_addr struct
    //
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);


    //
    //  loop forever ... until quit
    //
    while(1){

        //
        // create a socket by running the socket() function
        //
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");

        //
        //connect to the server
        //
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");

	

        //
        // [zero the buffer but don't] prompt the user for message
        //
        // printf("Please the enter message");
        //
        bzero(buffer,256);
        
        //
        //put the address into the buffer
        //
        fgets(buffer,255,stdin);
        
        //
        //write the message to the server
        //
        n = send(sockfd,buffer,strlen(buffer), 0);
        if (n < 0)
             error("ERROR writing to socket");
             
	
        //clear the buffer
        bzero(buffer,256);
        
        //get the message returned by the server
        recv(sockfd,buffer,255, 0);

        //
        // after receiving message from server
        // check for error/success codes
        //
//        std::string str;
//        str = buffer;

        //
        //  stream the buffer to a string
        //
        ss << buffer;
        str = ss.str();
        ss.str("");


        if( str.find("500") != std::string::npos && str.find("500") == 0){
            printf("%s", "500 Syntax error, command unrecognised\n");
        }else if( str.find("501") != std::string::npos && str.find("501") == 0){
            printf("%s", "501 Syntax error in parameters or arguments\n");
        }else if( str.find("502") != std::string::npos && str.find("502") == 0){
            printf("%s", "502 Command not implemented\n");
        }else if( str.find("550") != std::string::npos && str.find("550") == 0){
            printf("%s", "550 Requested action not taken: mailbox unavailable\n");
        }else if( str.find("551") != std::string::npos && str.find("551") == 0){
            printf("%s", "551 User not local; please try <forward-path>\n");
        }else if( str.find("552") != std::string::npos && str.find("552") == 0){
            printf("%s", "552 Requested mail action aborted: exceeded storage allocation\n");
        }else if( str.find("553") != std::string::npos && str.find("553") == 0){
            printf("%s", "553 Requested action not taken: mailbox name not allowed\n");
        }else if( str.find("554") != std::string::npos && str.find("554") == 0){
            error("554 Transaction failed\n");
        }else if( str.find("220") != std::string::npos && str.find("220") == 0){
            printf("%s", "220 <domain> Service ready\n");
        }else if( str.find("221") != std::string::npos && str.find("221") == 0){
            printf("%s", "221 <domain> Service closing transmission channel\n");
        }else if( str.find("250") != std::string::npos && str.find("250") == 0){
            printf("%s", "250 Requested mail action okay, completed\n");
        }else if( str.find("354") != std::string::npos && str.find("354") == 0){
            printf("%s", "354 Start main input; end with <CRLF>.<CRLF>\n");
        }else if (n < 0){
             error("ERROR reading from socket\n");
        }

        //        
        // display the message returned by the server
        //
        //printf("%s\n",buffer);
        
        close(sockfd);
    }

    //
    // close the forever loop
    //

    //close the socket and end the program
    close(sockfd);
    return 0;
}
