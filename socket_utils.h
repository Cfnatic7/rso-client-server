//
// Created by micha on 15.03.2023.
//

#ifndef LAB1_SOCKET_UTILS_H
#define LAB1_SOCKET_UTILS_H
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#define SQUARE_ROOT_REQUEST_ID (uint8_t)0x0001
#define SQUARE_ROOT_RESPONSE_ID ((uint8_t) 0x1001)
#define TIME_REQUEST_ID ((uint8_t) 0x0002)
#define TIME_RESPONSE_ID ((uint8_t) 0x1002)
#define MAX_LEN ((uint8_t) 255)

enum type_t {
    DOUBLE, INT, PID, LONGLONG
};

struct rq_id_t {
    pid_t pid;
    long long request_number;
}__attribute__ ((packed));

struct date_buf_t {
    uint8_t len;
    uint8_t buf[MAX_LEN];
}__attribute__ ((packed));

union data_t {
    double number;
    struct date_buf_t date_buf;
}__attribute__ ((packed));

struct dto_t {
    uint8_t type;
    struct rq_id_t rq_id;
    union data_t data;
}__attribute__ ((packed));

uint8_t is_little_endian();
void switch_endianness(void *ptr, enum  type_t type);
int my_write(int fd, const void * buf, size_t len);
int my_read (int fd, const void * buf, size_t len);
#endif //LAB1_SOCKET_UTILS_H
