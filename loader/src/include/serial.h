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
