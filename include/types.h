#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

#define NULL ((void *)0)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define __r volatile const
#define __w volatile
#define _rw volatile

#define char_buf_size_32 12
#define char_buf_size_64 21

#endif