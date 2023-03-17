build: client server
client: client.c socket_utils.h socket_utils.c
	gcc client.c socket_utils.h socket_utils.c -o client -ggdb
server: server.c socket_utils.h socket_utils.c
	gcc server.c socket_utils.h socket_utils.c -o server -lm -ggdb
clean:
	rm -f ./client
	rm -f ./server