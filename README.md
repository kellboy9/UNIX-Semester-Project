# UNIX-Semester-Project

Network server created by Keller Hood, Enoch Ng, Jorge Macias, Siyuan Sun, and Qi Gao for our semester project in CS 3376; builds on the client/server code provided at http://www.linuxhowtos.org/C_C++/socket.htm and adds the following features:

* User is prompted to choose between TCP and UDP in the client
* Server can handle both TCP and UDP requests
* Server loops continuously, creating child processes to handle new requests
* Server also handles any zombie processes created in this way correctly

The echo_c program takes two arguments: IP address of the server to connect to, and port number. You will be prompted to choose between TCP and UDP.

The echo_s program takes 1-3 arguments, which are port numbers that the server can accept messages on.

The log_s program takes 1 argument, which is the port number to listen on for messages from echo_s. This should be the same as the value of LOG_PORT in server_functions.c, which is 9999 by default.
