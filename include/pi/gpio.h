#ifndef _P_GPIO_H
#define _P_GPIO_H

#include "pi/pbase.h"
#include "types.h"

typedef struct GPIOPin {
  __r u32 res;
  _rw u32 data[2];
} GPIOPin;

struct GPIOReg {
  _rw u32 GPFSEL[6];
  GPIOPin GPSET;
  GPIOPin GPCLR;
  GPIOPin GPLEV;
  GPIOPin GPEDS;
  GPIOPin GPREN;
  GPIOPin GPFEN;
  GPIOPin GPHEN;
  GPIOPin GPLEN;
  GPIOPin GPAREN;
  GPIOPin GPAFEN;
  _rw u32 GPIO_PUP_PDN_CNTRL_REG[4];
};

#define GPIO_REG ((struct GPIOReg *)(PBASE + 0x00200000))

#endif