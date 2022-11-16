default:
	gcc -o server server.c -lcrypto -pthread

clean:
	rm -f server