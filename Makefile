CC=gcc

server: server.o
	$(CC) -o server server.o
client: client.o
	$(CC) -o client client.o

server.o: server.c
	$(CC) -o server.o -c server.c
client.o: client.c
	$(CC) -o client.o -c client.c


clean:
	rm client server *.o
