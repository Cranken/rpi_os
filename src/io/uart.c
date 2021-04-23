#include "io/uart.h"

#include "helpers/utils.h"
#include "pi/gpio.h"

// https://developer.arm.com/documentation/ddi0183/g/programmers-model/register-descriptions/fractional-baud-rate-register--uartfbrd
// Calculate with defines since -mgeneral-regs-only and float dont work together
#define UART_CLOCK_FREQ 48000000  // 48 Mhz = 48 * 10^6
#define BAUD_RATE       115200
#define UART_BAUD_DIV   (double)UART_CLOCK_FREQ / (16 * BAUD_RATE)
#define UART_IBRD       (u32)(UART_BAUD_DIV)
#define UART_FBRD       (unsigned int)((UART_BAUD_DIV - UART_IBRD) * 64 + .5)

void uart_init() {
  unsigned int selector;

  // Set gpio 14 and 15 to alt func 5(pl011 uart)
  selector = GPIO_REG->GPFSEL[1];
  selector &= ~(0b111 << 12);
  selector |= 0b100 << 12;
  selector &= ~(0b111 << 15);
  selector |= 0b100 << 15;
  GPIO_REG->GPFSEL[1] = selector;

  selector = GPIO_REG->GPIO_PUP_PDN_CNTRL_REG[0];
  selector &= ~(0xf0000000);  // Disable gpio14 and gpio15 pullup/down
  GPIO_REG->GPIO_PUP_PDN_CNTRL_REG[0] = selector;

  // Disable Uart
  UART_AUX_REG->cr = 0;
  UART_AUX_REG->imsc = 0;

  // Set baud rate to 115200
  UART_AUX_REG->ibrd = UART_IBRD;
  UART_AUX_REG->fbrd = UART_FBRD;

  // enable fifo & 8-bit mode
  UART_AUX_REG->lcrh = 0b111 << 4;
  // Enable tx, rx and uart
  UART_AUX_REG->cr = (1 << 9) | (1 << 8) | (1 << 0);
}

void uart_send_char(char c) {
  while (UART_AUX_REG->fr & (1 << 5)) {
  }
  UART_AUX_REG->dr = c;
}

char uart_recv() {
  while (UART_AUX_REG->fr & (1 << 4)) {
  }
  return (UART_AUX_REG->dr & 0xFF);
}

void uart_send_str(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    uart_send_char((char)str[i]);
  }
}

u32 uart_read_int() {
  u32 num = 0;
  for (u8 i = 0; i < 4; i++) {
    char c = uart_recv();
    num = num << 8;
    num += (u32)c;
  }
  return num;
}

void uart_send_int(u32 number) {
  uart_send_char((char)((number >> 24) & 0xFF));
  uart_send_char((char)((number >> 16) & 0xFF));
  uart_send_char((char)((number >> 8) & 0xFF));
  uart_send_char((char)(number & 0xFF));
}