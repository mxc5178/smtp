/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

//function to print an error message and exit the program
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	//declare sockfd and newsockfd to use as "handles" to sockets
	//declare portno as an int to hold the prot number
     int sockfd, newsockfd, portno;
	 
	//clilen is a variable to hold the size(in bytes) of 
    //    a sockaddr_in struct	
     socklen_t clilen;
	 
	//buffer is an array of chars that will hold the message
    //    being received from a client or sent to a client	
     char buffer[256];
	 
	//serv_addr is a variable of type sockaddr_in
    //    serv_addr is now a struct which holds the Ip
    //    information for the server 
    //cli_addr is a variable of type sockaddr_in
    //    cli_addr is now a struct which holds the Ip
    //    information for the client once a client connection
    //    has been accepted by this server	
     struct sockaddr_in serv_addr, cli_addr;
	 
	// 
     int n;
	 
	//check that there are two command line arguments
    //    arg[1] is the port number
    //    for example, in a Linux/Mac/Unix terminal window
    //  	./server.exe 31000	
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
	 
	/*
	  There are four steps(functions) to creating a server 
	  using sockets
	    1. socket()
		2. bind()
		3. listen()
		4. accept()
    */	
	
	//create a socket using the socket function; the value
	//returned by the socket() function is a handle to the socket
	//(similar to the descriptor of a file)
	//save the socket handle; it will be needed by the bind(), listen(), and accept() functions
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 
	//if socket() returns a negative number, there has been an error 
     if (sockfd < 0) 
        error("ERROR opening socket");
		
	//bzero() is a function that fills a block of memory locations with binary zero
    //  the following call to bzero sets the location occupied by the
    //  serv_addr struct with all zeroes.
    //bzero just clears the memory location to removed any prior data that is unwanted 	
     bzero((char *) &serv_addr, sizeof(serv_addr));
	 
	//convert the alpha port number from the command line to a number 
     portno = atoi(argv[1]);
	 
	//fill the serv_addr struct members with appropriate information 
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	 
	//Bind the socket to a specific port using the socket handle and the serv_addr struct 
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
			  
	//Listen on the socket for a maximum number of connections; in this example
	//    max connections is 5 but the number is arbitrary and meaningless because
	//    this is a single threaded application
     listen(sockfd,5);
	 
	//get the size of the cii_addr struct 
     clilen = sizeof(cli_addr);
	 
	//accept connections from the socket; accept returns a new socket descriptor which
    //    is a new socket associated with the client requesting the connection;  
    //    the accept function fills the cli_addr struct with the address information
    //    about the client; communication with the client now occurs on the new socket
    //    see the read and write functions below	
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
	
	/*
	  The following read or write in this example is not looping, but it could be
	  To continue the communication between the client and server, you would include
	  the following code in a loop
	  
	  Note that this example uses read() and write(). Many examples use recv() and send()
	*/
	//clear the buffer
     bzero(buffer,256);
	 
	//get the message from the client
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
	 
	//send the message to the client 
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
	/*
	  This is where the loop would end
    */	
	
	//close both sockest and end the program
     close(newsockfd);
     close(sockfd);
     return 0; 
}