default: all

all: server

Server: server.c
	gcc -g -o server server.c

clean veryclean:
	rm -f server