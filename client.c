#include  <sys/types.h>
#include  <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_utils.h"

int main()
{
	int sockfd, result;
	socklen_t len;
	struct sockaddr_in address;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr ("127.0.0.1");
	address.sin_port = htons (9734);
	len = sizeof (address);
    result = connect (sockfd, (struct sockaddr *) &address, len);
    signal(SIGPIPE, SIG_IGN);
    if (result == -1)
    {
        perror ("oops: netclient");
        exit (1);
    }
    struct dto_t dto;
    double number;
    int small_endianness = is_little_endian();
    long long request_number = 0, request_number_copy;
    pid_t pid = getpid();
    request_number++;
    request_number_copy = request_number;
    printf("%s\n", "What do you want to do: 0 - get square root, 1 - get current date, 2 - quit\n");
    fflush(stdout);
    int decision;
    scanf("%d", &decision);
    while(getchar() != '\n');
    if (decision == 0) {
        printf("%s\n", "Provide number:\n");
        fflush(stdout);
        scanf("%lf", &number);
        while(getchar() != '\n');
        dto.type = SQUARE_ROOT_REQUEST_ID;
        dto.data.number = number;
        dto.rq_id.pid = pid;
        dto.rq_id.request_number = request_number_copy;
        if (small_endianness) {
            switch_dto_endianness(&dto);
        }
        display_dto_from_big_endian(dto);
        my_write(sockfd, (void *) &dto, sizeof (struct dto_t));
        my_read(sockfd, (void *) &dto, sizeof(struct dto_t));
        display_dto_from_big_endian(dto);
        fflush(stdout);
    }
    else if (decision == 1) {
        dto.rq_id.request_number = request_number_copy;
        dto.rq_id.pid = pid;
        dto.type = TIME_REQUEST_ID;
        if (small_endianness) {
            switch_dto_endianness(&dto);
        }
        printf("Pre send\n");
        display_dto_from_big_endian(dto);
        my_write(sockfd, (void *) &dto, sizeof(struct dto_t));
        my_read(sockfd, (void *) &dto, sizeof(struct dto_t));
        printf("After read\n");
        display_dto_from_big_endian(dto);
    }
    else if (decision == 2) {
        close (sockfd);
        exit (0);
    }
}
