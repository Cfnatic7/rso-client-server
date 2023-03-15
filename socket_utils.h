//
// Created by micha on 15.03.2023.
//

#ifndef LAB1_SOCKET_UTILS_H
#define LAB1_SOCKET_UTILS_H
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#define SQUARE_ROOT_REQUEST_ID 0x0001
#define SQUARE_ROOT_RESPONSE_ID 0x1001
#define TIME_REQUEST_ID 0x0002
#define TIME_RESPONSE_ID 0x1002
#define MAX_LEN 255

enum type_t {
    DOUBLE, INT, PID
};

struct rq_id_t {
    pid_t pid;
    uint8_t request_number;
}__attribute__ ((packed));

struct square_root_query_t {
    uint8_t type;
    rq_id_t rq_id;
    double number;
}__attribute__ ((packed));

struct date_time_response_t {
    uint8_t type;
    rq_id_t rq_id;
    uint8_t len;
    char buf[MAX_LEN]
}__attribute__ ((packed));

struct date_time_query_t {
    uint8_t type;
    rq_id_t rq_id;
};

uint8_t is_little_endian();
void switch_endianness(void *ptr, enum  type_t type);
int my_write(int fd, const void * buf, size_t len);
int my_read (int fd, const void * buf, size_t len);
#endif //LAB1_SOCKET_UTILS_H
