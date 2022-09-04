#pragma once

#define INT8_MAX  127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807

#define INT8_MIN   -INT8_MAX
#define INT16_MIN  -INT16_MAX
#define INT32_MIN  -INT32_MAX
#define INT64_MIN  -INT64_MAX

#define UINT8_MIN   2*INT8_MAX+1
#define UINT16_MIN  2*INT16_MAX+1
#define UINT32_MIN  2*INT32_MAX+1
#define UINT64_MIN  2*INT64_MAX+1

#define INT_LEAST8_MAX  INT8_MAX 
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define INT_LEAST8_MIN  INT8_MAIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define UINT_LEAST8_MAX  UINT8_MAX 
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define UINT_LEAST8_MIN  UINT8_MAIN
#define UINT_LEAST16_MIN UINT16_MIN
#define UINT_LEAST32_MIN UINT32_MIN
#define UINT_LEAST64_MIN UINT64_MIN

#define INTMAX_C(c)  c ## LL
#define UINTMAX_C(c) c ## ULL

#define INT8_C(c)   c
#define INT16_C(c)  c
#define INT32_C(c)  c
#define INT64_C(c)  c ## LL

#define UINT8_C(c)  c
#define UINT16_C(c) c
#define UINT32_C(c) c ## U
#define UINT64_C(c) c ## ULL

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long        int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long      uint64_t;

typedef signed char        int_least8_t;
typedef signed short       int_least16_t;
typedef signed int         int_least32_t;
typedef signed long        int_least64_t;

typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned int       uint_least32_t;
typedef unsigned long      uint_least64_t;

typedef signed char        int_fast8_t;
typedef signed short       int_fast16_t;
typedef signed int         int_fast32_t;
typedef signed long long   int_fast64_t;

typedef unsigned char       uint_fast8_t;
typedef unsigned short      uint_fast16_t;
typedef unsigned int        uint_fast32_t;
typedef unsigned long long  uint_fast64_t;

typedef int                 intptr_t;
typedef unsigned int        uintptr_t;
typedef long long           intmax_t;
typedef unsigned long long  uintmax_t;