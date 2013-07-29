#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
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
//connect to the server
//
    //if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    // error("ERROR connecting");

/*
This is where you would put a loop to
continuously communicate with the server
*/
    while(1){

        // create a socket by running the socket() function
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");

        //
        //connect to the server
        //
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");

	

	//
        //prompt the user for message
        //
        printf("Please the enter message");
        bzero(buffer,256);
        
        //
        //put the address into the buffer
        //
        fgets(buffer,255,stdin);
        
        //
        //write the message to the server
        //
        //n = write(sockfd,buffer,strlen(buffer));
        send(sockfd,buffer,strlen(buffer), 0);
        if (n < 0)
             error("ERROR writing to socket");
             
	
        //clear the buffer
        bzero(buffer,256);
        
        //get the message returned by the server
        //n = read(sockfd,buffer,255);
        recv(sockfd,buffer,255, 0);

        //
        // after receiving message from server
        // check for error/success codes
        //
	std::string str;
	str = buffer;

	if( str.find("500"))
		error("command error syntax");

        if (n < 0)
             error("ERROR reading from socket");
        
        //display the message returned by the server
        printf("%s\n",buffer);
        
        close(sockfd);
    }
/*
If you had a loop, this is where it would end
*/

//close the socket and end the program
    close(sockfd);
    return 0;
}
