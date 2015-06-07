/* Add firmware image header and checksum to data */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint8_t check_hex(char * string) {
	uint8_t i, l = strlen(string);
	if (l > 8) {
		return 1;
	}
	for (i = 0; i < l; i++) {
		if ('0' <= string[i] && string[i] <= '9') {
			continue;
		}
		else if ('a' <= string[i] && string[i] <= 'f') {
			continue;
		} 
		else if ('A' <= string[i] && string[i] <= 'F') {
			// Convert to lower.
			string[i] -= 'A';
			string[i] += 'a';
			continue;
		} else {
			return 2;
		}
	}	
	return 0;
}

uint32_t read_hex(char * string) {
	uint8_t i;
	uint32_t result = 0;
	for (i = 0; i < 8; i++) {
		if (string[i] == '\0') {
			return result;
		} else {
			result <<= 4;
			if ('0' <= string[i] && string[i] <= '9') result |= ((string[i] - '0') & 0x0f);
			else result |= ((string[i] - 'a' + 10) & 0x0f);
		}
	} 
	return result;
}

// Assumes that len is even.
uint16_t gen_checksum(uint8_t * data, size_t len) {
	size_t i;
	uint16_t result = 0, t;
	for (i = 0; i < len; i += 2) {
		t = (data[i] << 8) | data[i + 1];
		result += t;
	}
	result = (~result) + 1;
	return result;
}

int main(int argc, char * argv[]) {
	FILE * fin, * fout;
	uint32_t start_addr;
	uint32_t burn_addr;
	size_t len;
	uint16_t checksum;
	uint8_t * data;

	uint8_t temp[4];

	if (argc != 6) {
		puts("Usage: mkimg <input> <output> <signature> <start address> <burn address>");
		return 0;
	}

	// Input file...
	if (!(fin = fopen(argv[1], "rb"))) {
		puts("Error, could not open input file.");
		return -1;
	}

	// Output file...
	if (!(fout = fopen(argv[2], "w+"))) {
		puts("Error, could not open output file.");
		fclose(fin);
		return -1;
	}

	// Read signature
	if (strlen(argv[3]) != 4) {
		puts("Error, signature must be 4 characters.");
		fclose(fin);
		fclose(fout);
		return -1;
	}
	fwrite(argv[3], sizeof(char), 4, fout);

	// Read start address
	if (check_hex(argv[4])) {
		puts("Error, bad start address.");
		fclose(fin);
		fclose(fout);
		return -1;
	}
	start_addr = read_hex(argv[4]);

	// Read burn address
	if (check_hex(argv[5])) {
		puts("Error, bad burn address.");
		fclose(fin);
		fclose(fout);
		return -1;
	}
	burn_addr = read_hex(argv[5]);

	printf("Start address: 0x%x\n", start_addr);
	// Write as big endian.
	temp[0] = start_addr >> 24;
	temp[1] = start_addr >> 16;
	temp[2] = start_addr >> 8;
	temp[3] = start_addr;
	fwrite(temp, sizeof(uint8_t), 4, fout);

	printf("Burn address: 0x%x\n", burn_addr);
	// Write as big endian.
	temp[0] = burn_addr >> 24;
	temp[1] = burn_addr >> 16;
	temp[2] = burn_addr >> 8;
	temp[3] = burn_addr;
	fwrite(temp, sizeof(uint8_t), 4, fout);

	// Get data length
	fseek(fin, 0, SEEK_END);
	len = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	// Align (make even)
	if (len & 0x01) len++;

	// Add length for checksum.
	len += 2;

	printf("Data length: 0x%lx\n", len);
	// Write as big endian.
	temp[0] = len >> 24;
	temp[1] = len >> 16;
	temp[2] = len >> 8;
	temp[3] = len;
	fwrite(temp, sizeof(uint8_t), 4, fout);

	// Roll back length
	len -= 2;

	// Read in from input.
	data = malloc(len);
	fread(data, sizeof(uint8_t), len, fin);
	// Write it out.
	fwrite(data, sizeof(uint8_t), len, fout);

	// Checksum the image.
	checksum = gen_checksum(data, len);

	// Write as big endian.
	temp[0] = checksum >> 8;
	temp[1] = checksum;
	fwrite(temp, sizeof(uint8_t), 2, fout);
	
	free(data);	
	fclose(fin);
	fclose(fout);
	return 0;
}