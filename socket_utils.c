//
// Created by micha on 15.03.2023.
//
#include <unistd.h>
#include "socket_utils.h"

uint8_t is_little_endian() {
    int x = 1;
    uint8_t *y = (uint8_t *) &x;
    return *y;
}

void switch_endianness(void *ptr, enum  type_t type) {
    int size;
    if (type == DOUBLE) {
        size = sizeof(double);
    }
    else if (type == INT) {
        size = sizeof(int);
    }
    else if (type == PID) {
        size = sizeof(pid_t);
    }
    else if (type == LONGLONG) {
        size = sizeof(long long);
    }
    else if (type == UINT16) {
        size = sizeof(uint16_t);
    }
    uint8_t *contents = (uint8_t *) ptr;
    for (int i = 0, j = size - 1; i < j; i++, j--) {
        uint8_t save = contents[i];
        contents[i] = contents[j];
        contents[j] = save;
    }
}

int my_write(int fd, const void * buf, size_t len) {
    int i = 0;
    uint8_t * buffer = (uint8_t *) buf;
    while(i < len) {
        int retval = write(fd, buffer + i, 1);
        if (retval == 0) {
            if (errno == EINTR) {
                continue;
            }
            else {
                printf("breaking my write");
                break;
            }
        }
        i++;
    }
    printf("Written %d bytes\n", i);
    return i;
}
int my_read (int fd, const void * buf, size_t len) {
    int i = 0;
    uint8_t * buffer = (uint8_t *) buf;
    while(i < len) {
        int retval = read(fd, buffer + i, 1);
        fflush(stdout);
        if (retval == 0) {
            if (errno == EINTR) {
                continue;
            }
            else {
                printf("breaking my read");
                break;
            }
        }
        i++;
    }
    printf("Read %d bytes\n", i);
    fflush(stdout);
    return i;
}

void display_dto_from_big_endian(struct dto_t dto) {
    unsigned char is_little_endianness = is_little_endian();
    if (is_little_endianness) {
        switch_dto_endianness(&dto);
    }
    printf("type: %d\n", dto.type);
    printf("pid: %d\n", dto.rq_id.pid);
    printf("request number: %lld\n", dto.rq_id.request_number);
    if (dto.type == SQUARE_ROOT_RESPONSE_ID || dto.type == SQUARE_ROOT_REQUEST_ID) {
        printf("number: %f\n", dto.data.number);
    }
    else if (dto.type == TIME_REQUEST_ID) {
        printf("current date and time: %s\n", dto.data.date_buf.buf);
        printf("buffer length: %d\n", dto.data.date_buf.len);
    }
    putchar('\n');
    fflush(stdout);
}

void switch_dto_endianness(struct dto_t *dto) {
    switch_endianness((void *) &dto->type, UINT16);
    switch_endianness((void *) &dto->rq_id.request_number, LONGLONG);
    switch_endianness((void *) &dto->rq_id.pid, PID);
    switch_endianness((void *) &dto->data.number, DOUBLE);
}

