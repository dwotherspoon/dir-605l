/*
Parse firmware image and extract parts.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct img_header {
	unsigned char signature[5];
	uint32_t startAddr;
	uint32_t burnAddr;
	uint32_t len;
} IMG_HEADER_T;

#define SWAP_32(v) ( (((v&0xff)<<24)&0xff000000) | ((((v>>8)&0xff)<<16)&0xff0000) | \
				((((v>>16)&0xff)<<8)&0xff00) | (((v>>24)&0xff)&0xff) )

uint16_t checksum(uint8_t * data, unsigned long len) {
	unsigned long i;
	uint16_t sum = 0, tmp;

	for (i = 0; i < len; i += 2) {
		tmp = (data[i] << 8) | data[i+1];
		sum += tmp;
		//printf("SUM: %x\n", sum);
	}
	if ( len % 2 ) {
		puts("Warning: Non-even length!");
		//tmp = data[len-1];
		//sum += SWAP_32(tmp);
	} 
	return sum;
}

int main(int argc, char * argv[]) {
	FILE * fp, * fout;
	IMG_HEADER_T iheader;
	unsigned long size;
	uint8_t * buf = NULL;
	char outname[] = "1234.bin";

	if (argc != 2) {
		puts("Usage: fparse <file>");
		return 0;
	}
	if ((fp = fopen(argv[1], "rb"))) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		while (ftell(fp) < size) {
			puts("Reading image header.");
			fread(iheader.signature, 1, 4, fp);
			iheader.signature[4] = '\0';

			fread(&iheader.startAddr, sizeof(uint32_t), 1, fp);
			iheader.startAddr = SWAP_32(iheader.startAddr);

			fread(&iheader.burnAddr, sizeof(uint32_t), 1, fp);
			iheader.burnAddr = SWAP_32(iheader.burnAddr);

			fread(&iheader.len, sizeof(uint32_t), 1, fp);
			iheader.len = SWAP_32(iheader.len);

			printf("Signature: %s\n", iheader.signature);
			printf("Start address: 0x%x\n", iheader.startAddr);
			printf("Burn address: 0x%x\n", iheader.burnAddr);
			printf("Data length: %u bytes\n", iheader.len);
			// Dump to file.
			free(buf);
			buf = malloc(iheader.len);
			fread(buf, 1, iheader.len, fp);
			outname[0] = iheader.signature[0];
			outname[1] = iheader.signature[1];
			outname[2] = iheader.signature[2];
			outname[3] = iheader.signature[3];
			printf("Saving to %s\n", outname);
			fflush(0);
			fout = fopen(outname, "w+");
			fwrite(buf, 1, iheader.len, fout);
			fclose(fout);
			// Get the checksum...
			printf("Checksum: 0x%x\n", checksum(buf, iheader.len));
			printf("Checking for next block at 0x%lx\n", ftell(fp));
		}
		puts("EOF");
		fclose(fp);
		return 0;
	} else {
		puts("Could not open file.");
		return -1;
	}
}