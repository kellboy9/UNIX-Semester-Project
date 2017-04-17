CC=gcc

all: server client_function

server: server.o server_functions.o
	$(CC) -o server server.o server_functions.o
client_function: client_function.o
	$(CC) -o client_function client_function.o

server.o: server.c
	$(CC) -o server.o -c server.c
client_function.o: client_function.c
	$(CC) -o client_function.o -c client_function.c

server_functions.o: server_functions.c
	$(CC) -o server_functions.o -c server_functions.c

clean:
	rm client_function server *.o
