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

#include "file_lock.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

// Limit: Max number of simultaneously locked FILE*
#define MAX_FILE_LOCKS 32

typedef struct {
    FILE *file;
    pthread_mutex_t lock;
    int in_use;
} file_lock_entry_t;

static file_lock_entry_t file_locks[MAX_FILE_LOCKS] = {0};

// Internal helper to find or create a lock entry
static pthread_mutex_t *get_file_lock(FILE *file) {
    for (int i = 0; i < MAX_FILE_LOCKS; i++) {
        if (file_locks[i].in_use && file_locks[i].file == file) {
            return &file_locks[i].lock;
        }
    }

    for (int i = 0; i < MAX_FILE_LOCKS; i++) {
        if (!file_locks[i].in_use) {
            file_locks[i].file = file;
            pthread_mutex_init(&file_locks[i].lock, NULL);
            file_locks[i].in_use = 1;
            return &file_locks[i].lock;
        }
    }

    return NULL;  // No space left
}

void flockfile(FILE *file) {
    pthread_mutex_t *lock = get_file_lock(file);
    if (lock) {
        pthread_mutex_lock(lock);
    }
}

void funlockfile(FILE *file) {
    pthread_mutex_t *lock = get_file_lock(file);
    if (lock) {
        pthread_mutex_unlock(lock);
    }
}

