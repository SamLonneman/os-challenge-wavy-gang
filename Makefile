default: server.c messages.h
	gcc -o server server.c -lcrypto

clean:
	rm -f server