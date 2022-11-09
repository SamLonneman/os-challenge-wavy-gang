default: server.c messages.h
	gcc -o server server.c serverOrig.c -lcrypto -pthread

clean:
	rm -f server