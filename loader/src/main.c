#include <serial.h>
#include <LzmaDec.h>
#include <Types.h>

#define LZMA_DATA_OFFSET  LZMA_PROPS_SIZE + 8

extern char __vmlinux_start;
extern char __vmlinux_end;
extern char __bend;

unsigned long free_mem;

/* 
	memcpy from linux/string.c, should be sufficent for LzmaDec. 
*/
void * memcpy(void * dest,const void *src,size_t count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

/* 
	Quick and hacky memory allocation for LzmaDec.
	Just allocates memory above stack space.
	TODO: Check memory bounds.
*/
void * alloc(void * p, size_t size) {
	void * res;

	/* Align */
	free_mem = (free_mem  + 3) & ~3;

	res = (void *)free_mem;

	free_mem += size;

	return res;
}

void free(void * p, void * address) {
	/* Do nothing, we should be fine. */
	p = p;
}

void decompress_kernel(unsigned char * output) {
	unsigned char * input = &__vmlinux_start;

	size_t len_input = &__vmlinux_end - &__vmlinux_start;
	size_t len_compressed = len_input - (LZMA_PROPS_SIZE + 8);

	size_t len_uncompressed;

	SRes result;
	ELzmaStatus state;
	ISzAlloc memalloc;
	memalloc.Alloc = &alloc;
	memalloc.Free = &free;

	tty_print("Decompressing kernel at ");
	tty_print_hex((unsigned long)input);

	/* Size follows properties in input - LE*/
	len_uncompressed = input[LZMA_PROPS_SIZE];
	len_uncompressed |= input[LZMA_PROPS_SIZE + 1] << 8;
	len_uncompressed |= input[LZMA_PROPS_SIZE + 2] << 16;
	len_uncompressed |= input[LZMA_PROPS_SIZE + 3] << 24;

	/* 
		size is 64 bits, we want the upper 32 to be zero 
		If all 0xFF, then size is just unknown. Need to handle that.
	*/
	// if (input[LZMA_PROPS_SIZE + 4] | input[LZMA_PROPS_SIZE + 5] | input[LZMA_PROPS_SIZE + 6] | input[LZMA_PROPS_SIZE + 7]) {
	// 	tty_print("Error: Kernel stream is too large.\r\n");
	// 	while (1);
	// }

	result = LzmaDecode(
		output, &len_uncompressed, 
		input + LZMA_DATA_OFFSET, &len_compressed,
		input, LZMA_PROPS_SIZE,
		LZMA_FINISH_ANY, &state, &memalloc);
	/* Could check result here, if needed. */

	tty_print("Uncompressed kernel length is ");
	tty_print_hex(len_uncompressed);
}


/*
	TODO: Flush cache.
*/
void main(void) {
	/* Free memory is stack size above end of .bss */
	free_mem = (unsigned long)&__bend;
	free_mem += STACK_SIZE;

	tty_print("RTL8196C Kernel Loader (David Wotherspoon 2015)\r\n");

	decompress_kernel((unsigned char * )(UNCOMPRESS_OUT));

	tty_print("Jumping into kernel at ");
	tty_print_hex(UNCOMPRESS_OUT);

	start_kernel(UNCOMPRESS_OUT);
}
