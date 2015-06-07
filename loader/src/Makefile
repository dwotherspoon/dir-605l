RSDK = ../../rsdk-1.3.6-4181-EB-2.4.25-0.9.30

AS = $(RSDK)/bin/rsdk-linux-as
CC = $(RSDK)/bin/rsdk-linux-gcc
LD = $(RSDK)/bin/rsdk-linux-ld
STRIP = $(RSDK)/bin/rsdk-linux-strip
OBJCOPY = $(RSDK)/bin/rsdk-linux-objcopy

ASFLAGS = -march=4181 -g -fno-pic -fomit-frame-pointer -mno-abicalls -Iinclude/

CFLAGS = -march=4181 -fno-pic -fomit-frame-pointer -mno-abicalls -Iinclude/ -G 0 -Os

LDFLAGS = -static -nostdlib
LDSCRIPT = ld.script

all: init.o main.o LzmaDec.o vmlinux.o vmlinux_img
	$(OBJCOPY) --add-section .vmlinux=vmlinux.lzma vmlinux.o
	$(LD) $(LDFLAGS) -o loader -G 0 -T $(LDSCRIPT) init.o main.o LzmaDec.o vmlinux.o
	$(OBJCOPY) -Obinary loader rom.bin
	#truncate -s 568322 rom.bin

vmlinux_img:
	cp vmlinux vmlinux.img
	$(STRIP) vmlinux.img
	$(OBJCOPY) -Obinary vmlinux.img vmlinux.img
	lzma -z -e vmlinux.img
	mv vmlinux.img.lzma vmlinux.lzma

clean:
	rm -f *.o loader rom.bin vmlinux.img vmlinux.lzma