#include "helpers/utils.h"

#include "io/uart.h"

u32 readline(char *buf, u32 maxlen) {
  u32 num = 0;
  while (num < maxlen - 1) {
    char c = uart_recv();
    // It seems like screen sends \r when I press enter
    if (c == '\n' || c == '\0' || c == '\r') {
      break;
    }
    buf[num] = c;
    num++;
  }
  buf[num] = '\0';
  return num;
}

u32 strcmp(const char *str1, const char *str2) {
  while (1) {
    if (*str1 != *str2) {
      return *str1 - *str2;
    }

    if (*str1 == '\0') {
      return 0;
    }

    str1++;
    str2++;
  }
}

u32 strlen(const char *str) {
  if (!str) {
    return 0;
  }

  const char *start = str;
  while (*str) {
    str++;
  }
  return str - start;
}

void reverse(char *buf, u32 s_len) {
  u32 i, j;
  char cur;

  for (i = 0, j = s_len - 1; i < j; ++i, --j) {
    cur = buf[i];
    buf[i] = buf[j];
    buf[j] = cur;
  }
}

u32 uitoa(char *buf, u32 num, u32 base) {
  u32 size = 0;
  do {
    const char digit = (char)(num % base);
    buf[size++] = digit < 10 ? '0' + digit : 'a' + digit - 10;
  } while (num /= base);
  if (base == 16) {
    buf[size++] = 'x';
    buf[size++] = '0';
  }
  buf[size] = '\0';
  reverse(buf, size);

  return size;
}

u32 sitoa(char *buf, i32 num, u32 base) {
  u32 unum = abs(num);
  u32 size = uitoa(buf, unum, base);
  if (num < 0) {
    buf[size++] = '-';
    buf[size] = '\0';
  }
  return size;
}

u32 abs(i32 num) { return num < 0 ? -num : num; }