#ifndef _UART_H
#define _UART_H

#include "pi/pbase.h"
#include "types.h"

struct UartReg {
  _rw u32 dr;
  _rw u32 rsrecr;
  __r u32 spacer1[4];
  _rw u32 fr;
  __r u32 spacer2;
  __r u32 ilpr;
  _rw u32 ibrd;
  _rw u32 fbrd;
  _rw u32 lcrh;
  _rw u32 cr;
  _rw u32 ifls;
  _rw u32 imsc;
  _rw u32 ris;
  _rw u32 mis;
  _rw u32 icr;
  _rw u32 dmacr;
  __r u32 spacer3[13];
  _rw u32 itcr;
  _rw u32 itip;
  _rw u32 itop;
  _rw u32 tdr;
};

#define UART_AUX_REG ((struct UartReg *)(PBASE + 0x00201000))

void uart_init(void);
char uart_recv(void);
void uart_send_char(char c);
void uart_send_str(char *str);
u32 uart_read_int(void);
void uart_send_int(u32 number);

#endif