default: server.c messages.h
	gcc -o server server.c

clean:
	rm -f server