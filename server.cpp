//
// simple SMTP server
//
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <sstream>

#include "util.h"

using namespace std;

//
// function to print an error message and exit the program
//
void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    //
    // check that there are two command line arguments ... first
    //    arg[1] is the port number
    //    for example, in a Linux/Mac/Unix terminal window
    //      ./smtp-server 31000
    //  
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    //
    // v = a vector for breaking down the buffer traded between client and server
    // d = delimiter to break the string upon
    // i = integer returned by split ... 1 if success, 0 if fail
    // ss = string stream for streaming buffer into
    // s = string for streaming ss into for splitting
    //
    //vector<string> v;
    string d = " ";
    int i;
    stringstream ss;
    std::string s = "";

    string message;

    //
    // sockfd and newsockfd -> sockets
    // portno -> port number to listen on
    //
    int sockfd, newsockfd, portno;

    //
    // client_len is a variable to hold the size(in bytes) of 
    // a sockaddr_in struct  
    // 
    socklen_t client_len;

    //
    // buffer is an array of chars that will hold the message
    // being received from a client or sent to a client    
    // 
    char buffer[256];

    //
    // serv_addr: variable of type sockaddr_in
    //    serv_addr is a struct which holds the Ip
    //    information for the server
    // 
    // cli_addr: variable of type sockaddr_in
    //    cli_addr is a struct which holds the Ip
    //    information for the client once a client connection
    //    has been accepted by this server 
    //
    // cli_addr will be compared to serv_addr to determine that
    //     the client and server are on the same network
    // 
    struct sockaddr_in serv_addr, cli_addr;

    //
    // use n to read from and write to the sockets
    //
    int n;

    //
    // create a socket using the socket function; the value
    // returned by the socket() function is a handle to the socket
    // (similar to the descriptor of a file)
    // save the socket handle; it will be needed by the bind(), listen(), and accept() functions
    //
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //
    // if socket() returns a negative number, there has been an error 
    //
    if (sockfd < 0) 
        error("ERROR opening socket");

    //
    // bzero() is a function that fills a block of memory locations with binary zero
    //  the following call to bzero sets the location occupied by the
    //  serv_addr struct with all zeroes.
    //
    // bzero just clears the memory location to remove any prior data that is unwanted  
    //
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //
    // convert the alpha port number from the command line to a number 
    //
    portno = atoi(argv[1]);

    //
    // fill the serv_addr struct members with appropriate information 
    //
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //
    // Bind the socket to a specific port using the socket handle and the serv_addr struct 
    //
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    //
    // Listen on the socket for a maximum number of connections; in this example
    //    max connections is 5 but the number is arbitrary and meaningless because
    //    this is a single threaded application
    //
    listen(sockfd,5);

    //
    // some boolean variables to keep track of where we
    // are in the conversation
    //
    bool hello = false;
    bool mailfrom = false;
    bool mailto = false;
    bool data = false;
    bool quit = false;

    //
    // loop forever ... it's a server
    //
    while(1){


        //
        // get the size of the cii_addr struct 
        //
        client_len = sizeof(cli_addr);
        
        // 
        // accept connections from the socket; accept returns a new socket descriptor which
        //    is a new socket associated with the client requesting the connection;  
        //    the accept function fills the cli_addr struct with the address information
        //    about the client; communication with the client now occurs on the new socket
        //    see the read and write functions below    
        //
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client_len);
        if (newsockfd < 0) 
            error("ERROR on accept");


        //
        // clear the buffer
        //
        bzero(buffer,256);

        //
        // get the message from the client
        //
        // consider changing read() to recv()
        //
        n = read(newsockfd,buffer,255);

        //
        //  stream the buffer to a string and split it
        //
        ss << buffer;
        s = ss.str();
        ss.str("");


        //
        // check for specific messages here:
        //  HELO
        //  MAIL FROM:
        //  RCPT TO:
        //  DATA
        //  QUIT
        //
        if(s.find("HELO") != std::string::npos && s.find("HELO") == 0){

            printf("%s", "HELO\n");

            //
            //  220 - Hello request
            //

            hello = true;
            message = "220 Hello,  Service Ready";

        }else if(s.find("MAIL FROM:") != std::string::npos && s.find("MAIL FROM:") == 0){

            //
            // check that hello has been recieved
            //

            if (hello){
                message = "250 Ok";
            }else{
                message = "503 Bad sequence of commands";
            }
            printf("%s", "MAIL FROM:\n");

        }else if(s.find("RCPT TO:") != std::string::npos && s.find("RCPT TO:") == 0){

            if (hello){

                message = "250 Ok";

            }else{
                message = "503 Bad sequence of commands";
            }

            printf("%s", "RCPT TO:\n");

        }else if((s.find("DATA") != std::string::npos && s.find("DATA") == 0) || data){

            //
            // recieving part of the data segment
            //

            if(!hello){

                message = "503 Bad sequence of commands";

            }else if (data){

                //
                // this is part of the data to be written to a file
                //

                util::writeData(string("test-mail-file"), s);

                if(s[0] == '.'){
                    //
                    // end of data stream
                    //

                    printf("%s", "making data false\n");
                    data = false;

                }
                
                message = "250 OK";

            }else{

                printf("%s", "making data true\n");
                data = true;
                message = "354 End data with <CR><LF>.<CR><LF>";


            }

            printf("%s", "DATA\n");

        }else if(s.find("QUIT") != std::string::npos && s.find("QUIT") == 0){

            printf("%s", "QUIT\n");
            message = "221 Service closing transmission channel";
            quit = true;
        }else{

            printf("%s", "ERROR\n");
            message = "503 Bad sequence of commands";

        }

        //if (n < 0) error("ERROR reading from socket");
        if (n < 0) printf("ERROR reading from socket");
        printf("buffer: %s\n",buffer);

        bzero(buffer,256);
        //
        // send response to the client:
        //  220 - Hello request
        //  221 - Goodbye
        //  250 - Ok
        //  354 - Send your message
        //  501 - Syntax error
        //  55x - Failure

        //
        // consider changing write() to send()
        //
        //n = write(newsockfd,"I got your message",18);
        n = write(newsockfd, message.c_str(), message.size());

        //if (n < 0) error("ERROR writing to socket");
        if (n < 0) printf("ERROR writing to socket");

        if(quit){
            close(newsockfd);
            close(sockfd);
        }

    }
    //
    // end forever loop
    //

    //
    // close both sockets and end the program
    //
    close(newsockfd);
    close(sockfd);
    return 0; 
}
