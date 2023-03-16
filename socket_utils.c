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

