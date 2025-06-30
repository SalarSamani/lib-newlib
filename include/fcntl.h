#ifndef _NEWLIB_GLUE_FCNTL_H_
#define _NEWLIB_GLUE_FCNTL_H_

#include_next <fcntl.h>
#define loff_t off_t

#include <uk/config.h>

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#if (defined CONFIG_ARCH_X86_64)
#define O_NOFOLLOW  0400000
#define O_DIRECTORY 0200000
#define O_CLOEXEC  02000000
#define O_DSYNC      010000
#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_NOATIME  01000000
#elif ((defined CONFIG_ARCH_ARM_64) || (defined CONFIG_ARCH_ARM_32))
#define O_NOFOLLOW  0100000
#define O_DIRECTORY 0400000000
#define O_CLOEXEC  02000000
#define O_DSYNC      010000
#define O_ASYNC      020000
#define O_DIRECT    0200000
#define O_NOATIME  01000000
#endif

#define FIONBIO      0x5421
#define FIOASYNC     0x5452

#define POSIX_FADV_NORMAL     0
#define POSIX_FADV_RANDOM     1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED   3
#define POSIX_FADV_DONTNEED   4
#define POSIX_FADV_NOREUSE    5

/* Glibc does not provide KEEP_SIZE and PUNCH_HOLE anymore. Instead it
 * includes linux/falloc.h.
 *
 * Musl still does provide them. And Newlib just does not care.
 */
#define FALLOC_FL_KEEP_SIZE 1
#define FALLOC_FL_PUNCH_HOLE 2


#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifndef S_IRUSR
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXU 0700
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXG 0070
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001
#define S_IRWXO 0007
#endif

/////////////////////////////
// #define F_DUPFD  0
// #define F_GETFD  1
// #define F_SETFD  2
// #define F_GETFL  3
// #define F_SETFL  4

// #define F_SETOWN 8
// #define F_GETOWN 9
#define F_SETSIG 10
#define F_GETSIG 11

// #define F_GETLK 5
// #define F_SETLK 6
// #define F_SETLKW 7

#define F_SETOWN_EX 15
#define F_GETOWN_EX 16

#define F_OFD_GETLK 36
#define F_OFD_SETLK 37
#define F_OFD_SETLKW 38

#define F_SETLEASE	1024
#define F_GETLEASE	1025
#define F_NOTIFY	1026
#define F_CANCELLK	1029
#define F_SETPIPE_SZ	1031
#define F_GETPIPE_SZ	1032
#define F_ADD_SEALS	1033
#define F_GET_SEALS	1034

#define F_SEAL_SEAL	0x0001
#define F_SEAL_SHRINK	0x0002
#define F_SEAL_GROW	0x0004
#define F_SEAL_WRITE	0x0008

#define F_GET_RW_HINT		1035
#define F_SET_RW_HINT		1036
#define F_GET_FILE_RW_HINT	1037
#define F_SET_FILE_RW_HINT	1038

#define RWF_WRITE_LIFE_NOT_SET	0
#define RWH_WRITE_LIFE_NONE	1
#define RWH_WRITE_LIFE_SHORT	2
#define RWH_WRITE_LIFE_MEDIUM	3
#define RWH_WRITE_LIFE_LONG	4
#define RWH_WRITE_LIFE_EXTREME	5
/////////////////////////////

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define AT_EMPTY_PATH 0x1000
#endif

int posix_fadvise(int fd, off_t offset, off_t len, int advice);

#endif
