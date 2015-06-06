RSDK = ../../rsdk-1.3.6-4181-EB-2.4.25-0.9.30

AS = $(RSDK)/bin/rsdk-linux-as
CC = $(RSDK)/bin/rsdk-linux-gcc
LD = $(RSDK)/bin/rsdk-linux-ld
OBJCOPY = $(RSDK)/bin/rsdk-linux-objcopy

ASFLAGS = -march=4181 -g -fno-pic -mno-abicalls -Iinclude/

CFLAGS = -march=4181 -fno-pic -mno-abicalls -Iinclude/ -G 0 -Os

LDFLAGS = -static -nostdlib
LDSCRIPT = ld.script

all: init.o main.o LzmaDec.o vmlinux.o
	$(OBJCOPY) --add-section .vmlinux=vmlinux.lzma vmlinux.o
	$(LD) $(LDFLAGS) -o loader -G 0 -T $(LDSCRIPT) init.o main.o LzmaDec.o vmlinux.o
	$(OBJCOPY) -Obinary loader rom
	truncate -s 568322 rom
clean:
	rm -f *.o loader rom