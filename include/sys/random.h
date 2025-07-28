#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <sys/types.h>

#define GRND_NONBLOCK   0x0001
#define GRND_RANDOM     0x0002
#define GRND_INSECURE   0x0004

#ifdef __cplusplus
extern "C" {
#endif

ssize_t getrandom(void *buf, size_t buflen, unsigned int flags);

#ifdef __cplusplus
}
#endif

#endif