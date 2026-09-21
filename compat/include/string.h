#ifndef CCTOOLS_COMPAT_STRING_H
#define CCTOOLS_COMPAT_STRING_H
#include <stddef.h>
#include_next <string.h>
size_t strlcpy(char *, const char *, size_t);
size_t strlcat(char *, const char *, size_t);
#endif
