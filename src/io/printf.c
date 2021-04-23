#include "io/printf.h"

#include "helpers/utils.h"
#include "io/uart.h"

static inline void insert_char(char **buf, const char c);
static inline u32 insert_string(char **buf, const char *str);

// TODO: Remove this and use malloc when possible
#define PRINT_BUF_SIZE 128

u32 printf(const char *fmt, ...) {
  va_list args, args_copy;
  va_start(args, fmt);

  va_copy(args_copy, args);
  u32 size = vsnprintf(NULL, fmt, -1, args_copy);
  va_end(args_copy);

  char buf[PRINT_BUF_SIZE];
  size = vsnprintf(buf, fmt, -1, args);
  va_end(args);
  buf[size] = '\0';

  uart_send_str(buf);

  return size;
}

u32 nprintf(const char *fmt, i32 len, ...) {
  va_list args, args_copy;
  va_start(args, len);

  va_copy(args_copy, args);
  u32 size = vsnprintf(NULL, fmt, len, args_copy);
  va_end(args_copy);

  char buf[PRINT_BUF_SIZE];
  size = vsnprintf(buf, fmt, len, args);
  va_end(args);

  uart_send_str(buf);

  return size;
};

u32 sprintf(char *buf, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  u32 size = vsnprintf(buf, fmt, -1, args);
  va_end(args);

  return size;
};

u32 snprintf(char *buf, const char *fmt, i32 len, ...) {
  va_list args;
  va_start(args, len);
  u32 size = vsnprintf(buf, fmt, len, args);
  va_end(args);

  return size;
};

// print max len char into buf according to fmt
// negative len means unrestricted length
// returns length of string
u32 vsnprintf(char *buf, const char *fmt, i32 len, va_list args) {
  if (!fmt) return 0;
  u32 size = 0;

  for (; *fmt; fmt++) {
    if (len == 0) break;
    len--;

    if (*fmt != '%') {
      insert_char(&buf, *fmt);
      size++;
      continue;
    }

    Flags flags = 0;
    fmt++;
    switch (*fmt) {
      case 'u':
        flags |= f_u;
        break;
      case 'x':
        flags |= f_x;
        break;
      case 'c':
        flags |= f_c;
        break;
      case 's':
        flags |= f_s;
        break;
      case 'p':
        flags |= f_p;
        break;
      case '%':
        flags |= f_pp;
        break;
      default:
        fmt--;
        continue;
    }

    if (flags & f_u) {
      u32 num = va_arg(args, u32);
      char str[char_buf_size_32];

      uitoa(str, num, 10);
      u32 len = insert_string(&buf, str);
      size += len;
      continue;
    }

    if (flags & (f_x)) {
      continue;
    }

    if (flags & f_c) {
      char c = (char)va_arg(args, int);

      insert_char(&buf, c);
      size++;
      continue;
    }

    if (flags & f_s) {
      const char *str = (const char *)va_arg(args, char *);

      u32 len = insert_string(&buf, str);
      size += len;
      continue;
    }

    if (flags & f_p) {
      uintptr_t ptr = (uintptr_t)va_arg(args, void *);
      char str[char_buf_size_32];

      uitoa(str, ptr, 16);

      u32 len = insert_string(&buf, str);
      size += len;
      continue;
    }

    if (flags & f_pp) {
      insert_char(&buf, '%');
      size++;
      continue;
    }
  }

  return size;
}

// Insert character into buffer
static inline void insert_char(char **buf, const char c) {
  if (*buf) *(*buf)++ = c;
}

// Insert string into buffer, return length of inserted string
static inline u32 insert_string(char **buf, const char *str) {
  u32 num = 0;
  while (*str) {
    insert_char(buf, *str++);
    num++;
  }
  return num;
}
