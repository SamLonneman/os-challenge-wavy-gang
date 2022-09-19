default: server.c messages.h openssl/sha.h
	gcc -o server server.c

clean:
	rm -f server