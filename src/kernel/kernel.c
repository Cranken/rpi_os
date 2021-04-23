#include "helpers/utils.h"
#include "io/printf.h"
#include "io/uart.h"
#include "pi/mem.h"

#define BUFF_SIZE   100
#define KERNEL_ADDR 0x80000
#define COPY_OFFSET LOW_MEMORY
#define COPY_SPACER 0x10000

extern char bss_end[];
extern char kernel_addr[];

void debug_loop();

void copy_and_jump_to_kernel(u32 kernel_size) {
  char *kernel = (char *)KERNEL_ADDR;
  int debug = uart_read_int();

  u32 checksum = 0;

  for (u32 i = 0; i < kernel_size; i++) {
    char c = uart_recv();
    if (debug) {
      uart_send_char(c);
    }
    checksum += c;
    kernel[i] = c;
  }

  uart_send_int(checksum);

  uart_send_char('D');
  uart_send_char('o');
  uart_send_char('n');
  uart_send_char('e');
  uart_send_char('\r');
  uart_send_char('\n');
  // uart_send_str("Done copying kernel\r\n");

  branch_to_address((void *)KERNEL_ADDR);
}

void copy_current_kernel_and_jump() {
  char *kernel = (char *)KERNEL_ADDR;
  char *end = bss_end;

  u32 kernel_size = uart_read_int();

  char *new_address = kernel + kernel_size + COPY_SPACER;
  // Confirm kernel size
  uart_send_int(kernel_size);

  char *copy = new_address;

  while (kernel < end) {
    *copy = *kernel;
    kernel++;
    copy++;
  }

  char *original_function_address = (char *)&copy_and_jump_to_kernel;

  // Subtract kernel_addr offset to account for layout at 0x80000
  char *copied_function_address = original_function_address + (long)new_address - (long)KERNEL_ADDR;

  void (*call_function)(u32) = (void (*)(u32))copied_function_address;
  call_function(kernel_size);
}

static u8 semaphore = 0;
void kernel_main(u8 proc_id) {
  if (proc_id == 0) {
    uart_init();
  }

  while (proc_id != semaphore) {
  };

  printf("-------------------------\r\n");
  printf("Processor %u ready\r\n", proc_id);
  printf("-------------------------\r\n");

  semaphore++;

  if (proc_id == 0) {
    while (1) {
      debug_loop();
    }
  }
}
void test() {
  uart_send_str("hello from a new function\r\n");
  uart_send_str("hello from a new 2 function\r\n");
}

void test2() {
  printf("asd aas%uadasd\r\n", 123);
  printf("aasdsad %% %s: %u lel\r\n", "test", 1337);
  printf("aasdsad %% %s: %u lel\r\n", "beeee", 1337);
  printf("Current EL: %u \r\n", get_el());
}

void debug_loop() {
  char buf[BUFF_SIZE];
  while (1) {
    readline(buf, BUFF_SIZE);
    if (strcmp(buf, "echo") == 0) {
      printf("[debug]: echo\r\n");
      test2();
    } else if (strcmp(buf, "kernel") == 0) {
      copy_current_kernel_and_jump();
    } else if (strcmp(buf, "awaken") == 0) {
      awaken();
    } else {
      uart_send_str(buf);
      uart_send_str("\r\n");
    }
  }
}
