##
## Makefile
##
## submitted by Matt Cooper mxc5178@rit.edu
##

CC = g++ -g -Wno-deprecated
CFLAGS = ''

smtp-server :
	$(CC) -o smtp-server server.cpp util.cpp

clean-server :
	rm -f smtp-server

smtp-client :
	$(CC) -o smtp-client client.cpp util.cpp

clean-client :
	rm -f smtp-client

example-server :
	$(CC) -o example-server example-server.cpp

example-client :
	$(CC) -o example-client example-client.cpp

clean-example :
	rm -f example-server && rm -f example-client
