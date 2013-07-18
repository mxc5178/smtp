##
## Makefile
##
##

CC = g++ -g -Wno-deprecated
CFLAGS = ''

client :
    $(CC) -o smtp-client client.cpp

server :
    $(CC) -o smtp-server server.cpp

example-server :
    $(CC) -o example-server example-server.cpp

example-client :
    $(CC) -o example-client example-client.cpp

clean :
    rm -f smtp-client && rm -f smtp-server && rm -f example-server rm -f example-client

