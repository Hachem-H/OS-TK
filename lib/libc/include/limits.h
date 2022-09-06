#pragma once

#define CHAR_BIT 8
#define MB_LEN_MAX 16

#define SCHAR_MIN -128
#define SCHAR_MAX  128
#define UCHAR_MAX  255
#define CHAR_MIN -128
#define CHAR_MAX  128

#define	USHRT_MAX	((unsigned short)(~0))
#define	SHRT_MAX	((short)(USHRT_MAX >> 1))
#define	SHRT_MIN	((short)(~SHRT_MAX))

#define	UINT_MAX	((unsigned int)(~0))
#define	INT_MAX	    ((int)(UINT_MAX >> 1))
#define	INT_MIN	    ((int)(~INT_MAX))

#define	ULONG_MAX	((unsigned long)(~0L))
#define	LONG_MAX	((long)(ULONG_MAX >> 1))
#define	LONG_MIN	((long)(~LONG_MAX))