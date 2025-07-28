#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <stdio.h>

void flockfile(FILE *file);
void funlockfile(FILE *file);

#endif /* FILE_LOCK_H */