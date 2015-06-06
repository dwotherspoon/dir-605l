#include <regs.h>
#include <loader.h>

/* 
	This is the entry point to the loader. 
	N.B. Top of stack is STACK_SIZE above .bss
	TODO: Zero BSS and maybe stack.
*/

.text

.globl __init

__init:
	la s0, __bstart
	la s1, __bend
	nop
	addi s1, STACK_SIZE 
	move sp, s1
	j main
	nop

/* Enter into kernel (address in a0), zero arg registers. */

.globl start_kernel

start_kernel:
	move t0, a0
	move a0, zero
	move a1, zero
	move a2, zero
	jr t0
	nop
