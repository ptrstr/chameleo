#pragma once

typedef union {
	struct {
		unsigned char bit0 : 1;
		unsigned char bit1 : 1;
		unsigned char bit2 : 1;
		unsigned char bit3 : 1;
		unsigned char bit4 : 1;
		unsigned char bit5 : 1;
		unsigned char bit6 : 1;
		unsigned char bit7 : 1;
	} bits;
	unsigned char byte;
} uByte;

unsigned char getBit(uByte byte, unsigned char index, unsigned char littleEndian);
void setBit(uByte *byte, unsigned char index, unsigned char newValue, unsigned char littleEndian);
unsigned char countBits(uByte byte);
