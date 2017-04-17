# UNIX-Semester-Project

Network server created by Keller Hood, Enoch Ng, Jorge Macias, Siyuan Sun, and Qi Gao for our semester project in CS 3376; builds on the client/server code provided at http://www.linuxhowtos.org/C_C++/socket.htm and adds the following features:

* User is prompted to choose between TCP and UDP in the client
* Server can handle both TCP and UDP requests
* Server loops continuously, creating child processes to handle new requests
* Server also handles any zombie processes created in this way correctly

