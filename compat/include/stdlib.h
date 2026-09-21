#ifndef CCTOOLS_COMPAT_STDLIB_H
#define CCTOOLS_COMPAT_STDLIB_H
#include <stddef.h>
#include_next <stdlib.h>
void *reallocf(void *, size_t);
#endif
