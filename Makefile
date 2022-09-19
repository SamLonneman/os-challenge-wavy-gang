default: server.c messages.h
	gcc -o server server.c -I/openssl/include/ -L/openssl/lib/ -lcrypto

clean:
	rm -f server