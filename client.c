#include  <sys/types.h>
#include  <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
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
    struct square_root_query_t square_root_query;
    struct date_time_query_t date_time_query;
    struct date_time_response_t date_time_response;
    double number;
    int is_little_endian = is_little_endian();
    int request_number = 0, request_number_copy;
    pid_t pid = getpid();
    if (is_little_endian) {
        switch_endianness((void *) &number, PID);
    }

    result = connect (sockfd, (struct sockaddr *) &address, len);
    if (result == -1)
    {
        perror ("oops: netclient");
        exit (1);
    }
    while(1) {
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
            square_root_query.type = SQUARE_ROOT_REQUEST_ID;
            if (is_little_endian) {
                switch_endianness((void *) &number, DOUBLE);
            }
            square_root_query.number = number;
            square_root_query.rq_id.pid = pid;
            square_root_query.rq_id.request_number = request_number_copy;
            my_write(sockfd, (void *) &square_root_query, sizeof (struct square_root_query_t));
            my_read(sockfd, (void *) &square_root_query, sizeof(struct square_root_query_t));
            if (is_little_endian) {
                switch_endianness((void *) &square_root_query.number, DOUBLE);
            }
            printf("square root: %lf\n", square_root_query.number);
            fflush(stdout);
        }
        else if (decision == 1) {
            date_time_query.rq_id.request_number = request_number_copy;
            date_time_query.rq_id.pid = pid;
            date_time_query.type = TIME_REQUEST_ID;
            my_write(sockfd, (void *) &date_time_query, sizeof(struct date_time_query_t));
            my_read(sockfd, (void *) &date_time_response, sizeof(struct date_time_response_t));
            for (int i = 0; i < date_time_response.len; i++) {
                putchar(date_time_response.buf[i]);
            }
            putchar('\n');
        }
        else if (decision == 2) {
            close (sockfd);
            exit (0);
        }
        request_number++;
        request_number_copy = request_number;
        if (is_little_endian) {
            switch_endianness((void *) &request_number_copy, INT);
        }
    }
	return 0;
}
