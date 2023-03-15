#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>

int main ()
{
	int server_sockfd, client_sockfd;
	socklen_t server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket (AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl (INADDR_ANY);
	server_address.sin_port = htons (9734);
	server_len = sizeof (server_address);
	bind (server_sockfd, (struct sockaddr *) &server_address, server_len);


	listen (server_sockfd, 5);

	while (1)
	{
		char chars[100];

		printf ("server waiting\n");


		client_len = sizeof (client_address);
		client_sockfd = accept (server_sockfd,
				(struct sockaddr *) &client_address,
				&client_len);

		read (client_sockfd, chars, 100);
		printf("%s\n", chars);
		close (client_sockfd);
	}
}
