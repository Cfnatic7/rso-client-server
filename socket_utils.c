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
    uint8_t *contents = (uint8_t *) ptr;
    for (int i = 0, j = size - 1; i < j; i++, j--) {
        uint8_t save = contents[j];
        contents[j] = contents[i];
        contents[i] = save;
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
            else i = -1;
        }
        i++;
    }
    return i;
}
int my_read (int fd, const void * buf, size_t len) {
    int i = 0;
    uint8_t * buffer = (uint8_t *) buf;
    while(i < len) {
        int retval = read(fd, buffer + i, 1);
        if (retval == 0) {
            if (errno == EINTR) {
                continue;
            }
            else i = -1;
        }
        i++;
    }
    return i;
}

void display_dto(struct dto_t dto) {
    unsigned char is_little_endianness = is_little_endian();
    uint8_t type = dto.type;
    pid_t pid = dto.rq_id.pid;
    long long request_number = dto.rq_id.request_number;
    double number = dto.data.number;
    uint8_t len = dto.data.date_buf.len;
    uint8_t buffer[MAX_LEN];
    strncpy((char *) buffer, (const char *) dto.data.date_buf.buf, len);
    if (is_little_endianness) {
        switch_endianness((void *) &pid, PID);
        switch_endianness((void *) &request_number, LONGLONG);
        switch_endianness((void *) &number, DOUBLE);
    }
    printf("type: %d\n", type);
    printf("pid: %d\n", pid);
    printf("request number: %lld\n", request_number);
    if (type == SQUARE_ROOT_RESPONSE_ID || type == SQUARE_ROOT_REQUEST_ID) {
        printf("number: %f\n", number);
    }
    else if (type == TIME_REQUEST_ID) {
        printf("current date and time: %s\n", buffer);
        printf("buffer length: %d\n", len);
    }
    fflush(stdout);
}

