#include <loader.h>

#define READ_BYTE(offset) (*(volatile unsigned char *)(IO_BASE+offset))

#define WRITE_BYTE(offset, val) (*(volatile unsigned char *)(IO_BASE+offset) = val)

void putc(char c) {
	unsigned int i = 0;
	for (i = 0; i < 0x6000; i++) {
		if (READ_BYTE(UART_LSR) & 0x20) break;
	}
	WRITE_BYTE(UART_THR, c);
}

void tty_print(char * c) {
	while (*c != '\0') {
		putc(*c++);
	}
}

/* Debugging use: print a 32bit value */
void tty_print_hex(unsigned h) {
	unsigned char c, v;
	tty_print("0x");
	for (c = 0; c < 8; c++) {
		v = h >> (28 - (c << 2));
		v &= 0x0f;
		if (v > 9) putc(v + 'a' - 10);
		else putc(v + '0');
	}
	tty_print("\r\n");
}
