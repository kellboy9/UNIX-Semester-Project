CC=gcc

all: echo_s echo_c

echo_s: echo_s.o server_functions.o
	$(CC) -o echo_s echo_s.o server_functions.o
echo_c: echo_c.o
	$(CC) -o echo_c echo_c.o

echo_s.o: echo_s.c
	$(CC) -o echo_s.o -c echo_s.c
echo_c.o: echo_c.c
	$(CC) -o echo_c.o -c echo_c.c

server_functions.o: server_functions.c
	$(CC) -o server_functions.o -c server_functions.c

clean:
	rm echo_c echo_s *.o
