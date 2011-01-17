#ifndef KOALA_STDINT_H
#define KOALA_STDINT_H

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__)
// Visual Studio
// and
// Borland C++ compiler
typedef unsigned char uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef char int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

const int8_t INT8_MIN = -128;
const int8_t INT8_MAX = 127;
const uint8_t UINT8_MIN = 0;
const uint8_t UINT8_MAX = 255;
const int16_t INT16_MIN = -32768;
const int16_t INT16_MAX = 32767;
const uint16_t UINT16_MIN = 0;
const uint16_t UINT16_MAX = 65535;
const int32_t INT32_MIN = 0x80000000;
const int32_t INT32_MAX = 0x7fffffff;
const uint32_t UINT32_MIN = 0;
const uint32_t UINT32_MAX = 0xffffffff;
const int64_t INT64_MIN = 0x8000000000000000;
const int64_t INT64_MAX = 0x7fffffffffffffff;
const uint64_t UINT64_MIN = 0;
const uint64_t UINT64_MAX = 0xffffffffffffffff;


#define atoll		_atoi64

#else
// assume all others know about stdint
#include<stdint.h>
#endif

#endif
