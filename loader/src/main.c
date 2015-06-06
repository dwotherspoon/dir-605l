#include <serial.h>

extern char __vmlinux_start[];
extern char __vmlinux_end[];

void decompress_kernel(unsigned char * output) {
	unsigned char * start_kernel = __vmlinux_start;
	unsigned int 	len_compressed = __vmlinux_end - __vmlinux_start;


	tty_print("Decompressing kernel...");



	tty_print(" done.\r\n");
}

void main(void) {

	tty_print("RTL8196C Kernel Loader (David Wotherspoon 2015)\r\n");

	decompress_kernel((unsigned char * )(UNCOMPRESS_OUT));

	start_kernel(UNCOMPRESS_OUT);
}
