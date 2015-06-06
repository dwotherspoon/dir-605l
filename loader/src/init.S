#include <regs.h>

/* This is the entry point to the loader. TODO: move the stack, and clear bss before jumping into main.c */

.text

.globl __init

__init:
	j main
	nop

/* Enter into kernel (address in a0), zero all arg registers. */

.globl start_kernel

start_kernel:
	move t0, a0
	move a0, zero
	move a1, zero
	move a2, zero
	jr t0
	nop