UNIX-Semester-Project
https://github.com/KayDevs/UNIX-Semester-Project

Network server created by Keller Hood, Enoch Ng, Jorge Macias, Siyuan Sun, and Qi Gao for our semester project in CS 3376; builds on the client/server code provided at http://www.linuxhowtos.org/C_C++/socket.htm and adds the following features:

* User is prompted to choose between TCP and UDP in the client
* Server can handle both TCP and UDP requests
* Server loops continuously, creating child processes to handle new requests
* Server also handles any zombie processes created in this way correctly
* The echo_c program takes two arguments: IP address of the server to connect to, and port number. You will be prompted to choose between TCP and UDP.

To run the echo server, execute echo_s with up to 3 port numbers as arguments, followed by -logip <ip_address>, where <ip_address> is the IP address of the log server.

The log_s program takes 1 argument, which is the port number to listen on for messages from echo_s. This should be the same as the value of LOG_PORT in server_functions.c, which is 9999 by default.

The user 1, Qi Gao, modify the code of "echo_s.c" so that there will be an argument passed to echo_s indicating which ip address the log server is running on.

The user 2, Keller Hood , modify the code of "echo_s.c" so that there will be an argument passed to echo_s indicating which port number the log server is running on.

The user 3, Jorge Macias, modify the code of "log_s.c" so that there wil be an argument passed to log_s indicating which port number the log server should listen to.

The user 4, Enoch Ng, modify the code of both "echo_s.c" and "log_s.c" so that if the echo server stopped by the ctrl-c, the echo_server will send a message "echo_s is stopping" to the log server. And log_server can terminate after receiving the message.

The user 5, Siyuan Sun, modify the code of "log_s.c" so that if the log server receive the message that "echo_s is stopping", it will log the message and terminate the program.
