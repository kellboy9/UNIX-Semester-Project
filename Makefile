CC=gcc

server: server.o server_functions.o
	$(CC) -o server server.o server_functions.o
client: client.o
	$(CC) -o client client.o

server.o: server.c
	$(CC) -o server.o -c server.c
client.o: client.c
	$(CC) -o client.o -c client.c

server_functions.o: server_functions.c
	$(CC) -o server_functions.o -c server_functions.c

clean:
	rm client server *.o
