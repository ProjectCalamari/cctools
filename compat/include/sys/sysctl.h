#ifndef CCTOOLS_COMPAT_SYS_SYSCTL_H
#define CCTOOLS_COMPAT_SYS_SYSCTL_H
#include <stddef.h>
#define CTL_KERN 1
#define KERN_OSRELEASE 2
int sysctl(const int *, unsigned int, void *, size_t *, const void *, size_t);
int sysctlbyname(const char *, void *, size_t *, const void *, size_t);
#endif
