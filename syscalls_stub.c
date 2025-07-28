#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/eventfd.h>

/* Stub for eventfd */
int eventfd(unsigned int initval, int flags) {
    (void)initval;
    (void)flags;
    errno = ENOSYS;
    return -1;
}

/* Stub for eventfd_read */
int eventfd_read(int fd, eventfd_t *value) {
    (void)fd;
    (void)value;
    errno = ENOSYS;
    return -1;
}

/* Stub for eventfd_write */
int eventfd_write(int fd, eventfd_t value) {
    (void)fd;
    (void)value;
    errno = ENOSYS;
    return -1;
}

/* Stub for killpg */
int killpg(pid_t pgrp, int sig) {
    (void)pgrp;
    (void)sig;
    errno = ENOSYS;
    return -1;
}

/* Stub for lockf */
int lockf(int fd, int cmd, off_t len) {
    (void)fd;
    (void)cmd;
    (void)len;
    errno = ENOSYS;
    return -1;
}
