/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * libnewlib glue code
 *
 * Authors: Salar Samani <soltanisamani@gmail.com>
 *
 * Copyright (c) 2017, NEC Europe Ltd., NEC Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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

