#ifndef _UTILS_H
#define _UTILS_H

#include "types.h"

u32 readline(char *buf, u32 maxlen);
u32 strcmp(const char *str1, const char *str2);
u32 strlen(const char *str);

void reverse(char *buf, u32 s_len);
// 32bit int to ascii, returns length of string
u32 uitoa(char *buf, u32 num, u32 base);
u32 sitoa(char *buf, i32 num, u32 base);

u32 abs(i32 num);

extern void branch_to_address(void *);
extern void awaken(void);
extern u32 get_el(void);

#endif