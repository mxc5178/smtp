smtp
====

smtp project for client server class

1. Build examples: 

    make example-client
    make example-server

2. Run examples:

    ./example-client <connect_port>
    ./example-server <url> <listen_port>

3. Build client/server:

    make smtp-client
    make smtp-server

4. Run client/server:

    ./smtp-client <server-addr>
    ./smtp-server <url> <port>

3. Clean the build:

    make clean-server
    make clean-client
    make clean-example
