#ifndef CCTOOLS_COMPAT_LIBC_H
#define CCTOOLS_COMPAT_LIBC_H
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>
#ifndef O_FSYNC
#define O_FSYNC O_SYNC
#endif
#ifndef O_EXLOCK
#define O_EXLOCK 0
#endif
#ifndef MAP_FILE
#define MAP_FILE 0
#endif
#ifndef MAP_RESILIENT_CODESIGN
#define MAP_RESILIENT_CODESIGN 0
#endif
void *reallocf(void *, size_t);
size_t strlcpy(char *, const char *, size_t);
size_t strlcat(char *, const char *, size_t);
void strmode(mode_t, char *);
#endif
