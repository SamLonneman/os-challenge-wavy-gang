default: server.c messages.h
	gcc -o server server.c -lcrypto -lpthread

clean:
	rm -f server