#ifndef CCTOOLS_COMPAT_BYTE_ORDER_H
#define CCTOOLS_COMPAT_BYTE_ORDER_H
#include <libkern/OSByteOrder.h>

enum NXByteOrder {
  NX_UnknownByteOrder,
  NX_LittleEndian,
  NX_BigEndian,
};

static inline enum NXByteOrder NXHostByteOrder(void) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return NX_LittleEndian;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  return NX_BigEndian;
#else
  return NX_UnknownByteOrder;
#endif
}
#endif
