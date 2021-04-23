#ifndef _PRINTF_H
#define _PRINTF_H

#include <stdarg.h>

#include "types.h"

typedef enum print_flags {
  f_u = (1 << 0),   // Unsigned int
  f_x = (1 << 1),   // Unsigned hex
  f_c = (1 << 2),   // Character
  f_s = (1 << 3),   // String
  f_p = (1 << 4),   // Pointer
  f_pp = (1 << 5),  // %-Sign
} Flags;

u32 printf(const char *fmt, ...);
u32 nprintf(const char *fmt, i32 len, ...);
u32 sprintf(char *buf, const char *fmt, ...);
u32 snprintf(char *buf, const char *fmt, i32 len, ...);

u32 vsnprintf(char *buf, const char *fmt, i32 len, va_list args);
#endif /* _PRINTF_H */
