client: client.c socket_utils.h socket_utils.c
	gcc client.c socket_utils.h socket_utils.c -o client
server: server.c socket_utils.h socket_utils.c
	gcc server.c socket_utils.h socket_utils.c -o server -lm