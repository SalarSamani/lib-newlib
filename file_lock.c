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
