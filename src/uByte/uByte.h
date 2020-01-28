#pragma once
#include <inttypes.h>

typedef union {
	struct {
		uint8_t bit0 : 1;
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
		uint8_t bit7 : 1;
	} bits;
	uint8_t byte;
} uByte;

uint8_t getBit(uByte byte, uint8_t index, uint8_t littleEndian);
void setBit(uByte *byte, uint8_t index, uint8_t newValue, uint8_t littleEndian);
uint8_t countBits(uByte byte);
