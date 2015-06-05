# D-Link DIR-605L

The information here is very much WIP.

## Overview
The D-Link DIR-605L is a budget 802.11n router, using the RTL8196C SoC. My unit has 32MB of SDRAM, 4MB of flash and runs a basic Linux 2.4 system, the GPL source code for which includes the Realtek toolchain can be found on D-Link's website.

## TFTP
Before passing over the the bootloader (rtkload in sources) a small Monitor program runs, you can jump into this by pressing 'ESC' on the console (before the decompressing kernel message). From here, you can upload a firmware image (set your ip to 192.168.0.2, the gateway ip to 192.168.0.1, netmask to 255.255.255.0), ensure you use binary transfer mode. The monitor will then automatically flash the relavent sections of the image (name does not matter) and reboot.

## Serial
The RTL8196C contains a 16550 UART which by default runs at 38400 baud 8N1. This breaks out to JP2 on the board, I recommend you use a USB to TTL serial adapter.