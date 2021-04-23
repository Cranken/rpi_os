#include "pi/mem.h"
#include "arch/sysregs.h"

.section ".text.boot"

.globl _start
_start:
  mrs	x0, mpidr_el1		
  and	x0, x0, #0x3		// Check processor core id
  cbz	x0, setup_el		// Go ahead if core 0, loop otherwise
  wfe
  b setup_stack_and_entry

setup_el:
  ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

  ldr x0, =CPACR_VALUE
  msr cpacr_el1, x0

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0

	adr	x0, init_bss_and_cores		
	msr	elr_el3, x0

	eret

init_bss_and_cores:
  adr	x0, bss_begin
  adr	x1, bss_end
  sub	x1, x1, x0
  bl 	memzero
  sev

  // See: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/arm64/booting.rst?h=v5.3#n255

	mov x0, #0
	adr x0, setup_stack_and_entry

	mov x1, #0xe0
	str x0, [x1]
	mov x1, #0xe8
	str x0, [x1]	
	mov x1, #0xf0
	str x0, [x1]

setup_stack_and_entry:
  mrs	x0, mpidr_el1		
  and	x0, x0, #0x3
  
  
  // Calculate offset for each kernel stack
  mov x1, #SECTION_SIZE
  mul x1, x1, x0 
  add x1, x1, #LOW_MEMORY

  // Set stackpointer
  mov	sp, x1

  // Enter kernel
  bl	kernel_main

proc_wait:
  wfe
  b proc_wait