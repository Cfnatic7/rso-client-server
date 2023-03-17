#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "socket_utils.h"

int
main ()
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

    signal (SIGCHLD, SIG_IGN);

    struct dto_t dto;
    int is_little_endianness = is_little_endian();
    uint8_t request_number = 0, request_number_copy;

    while (1)
    {

        printf ("server waiting\n");

        client_len = sizeof (client_address);
        client_sockfd = accept (server_sockfd,
                                (struct sockaddr *) &client_address,
                                &client_len);
        request_number++;
        request_number_copy = request_number;

//        if (fork () == 0)
//        {
            pid_t pid = getpid();
            read (client_sockfd, &dto, sizeof(struct dto_t));
            if (is_little_endianness) {
                switch_dto_endianness(&dto);
            }
            if (dto.type == SQUARE_ROOT_REQUEST_ID) {
                printf("Entered square root handling\n");
                dto.type = SQUARE_ROOT_RESPONSE_ID;
                dto.rq_id.pid = pid;
                dto.rq_id.request_number = request_number_copy;
                double number = dto.data.number;
                double square_root = sqrt(number);
                dto.data.number = square_root;
                printf("square root: %f\n", dto.data.number);
            }
            else if (dto.type == TIME_REQUEST_ID) {
                dto.type = TIME_RESPONSE_ID;
                dto.rq_id.pid = pid;
                dto.rq_id.request_number = request_number_copy;
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf((char *) dto.data.date_buf.buf,
                        "%d-%02d-%02d %02d:%02d:%02d",
                        tm.tm_year + 1900,
                        tm.tm_mon + 1,
                        tm.tm_mday,
                        tm.tm_hour,
                        tm.tm_min,
                        tm.tm_sec);
                dto.data.date_buf.len = strlen((char *) dto.data.date_buf.buf);
            }
            if (is_little_endianness) {
                switch_dto_endianness(&dto);
            }
            printf("pre send\n");
            display_dto_from_big_endian(dto);
            write (client_sockfd, &dto, sizeof(struct dto_t));
            close (client_sockfd);
            exit (0);
//        }
//        else
//        {
//            close (client_sockfd);
//        }
    }
}