#ifndef CCTOOLS_COMPAT_OSBYTEORDER_H
#define CCTOOLS_COMPAT_OSBYTEORDER_H

#include <stdint.h>

#define OSSwapInt16(x) __builtin_bswap16((uint16_t)(x))
#define OSSwapInt32(x) __builtin_bswap32((uint32_t)(x))
#define OSSwapInt64(x) __builtin_bswap64((uint64_t)(x))
#define _OSSwapInt16(x) OSSwapInt16(x)
#define _OSSwapInt32(x) OSSwapInt32(x)
#define _OSSwapInt64(x) OSSwapInt64(x)
#define OSSwapHostToBigInt16(x) OSSwapInt16(x)
#define OSSwapHostToBigInt32(x) OSSwapInt32(x)
#define OSSwapHostToBigInt64(x) OSSwapInt64(x)
#define OSSwapBigToHostInt16(x) OSSwapInt16(x)
#define OSSwapBigToHostInt32(x) OSSwapInt32(x)
#define OSSwapBigToHostInt64(x) OSSwapInt64(x)

#endif
