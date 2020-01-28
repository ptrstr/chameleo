#include "uByte.h"

uint8_t getBit(uByte byte, uint8_t index, uint8_t littleEndian) {
	// Verify Host Endianess and Switch if Little
	uByte host;
	host.byte = 1;
	if (host.bits.bit7 != 1) { // Little-endian
		if (littleEndian == 0)
			littleEndian = 1;
		else
			littleEndian = 0;
	}

	if (littleEndian == 1) { // Endianess check
		switch (index) {
		case 0:
			return byte.bits.bit7;
			break;
		case 1:
			return byte.bits.bit6;
			break;
		case 2:
			return byte.bits.bit5;
			break;
		case 3:
			return byte.bits.bit4;
			break;
		case 4:
			return byte.bits.bit3;
			break;
		case 5:
			return byte.bits.bit2;
			break;
		case 6:
			return byte.bits.bit1;
			break;
		case 7:
			return byte.bits.bit0;
			break;
		default:
			return 2;
		}
	} else if (littleEndian == 0) { // Endianess check
		switch (index) {
		case 0:
			return byte.bits.bit0;
			break;
		case 1:
			return byte.bits.bit1;
			break;
		case 2:
			return byte.bits.bit2;
			break;
		case 3:
			return byte.bits.bit3;
			break;
		case 4:
			return byte.bits.bit4;
			break;
		case 5:
			return byte.bits.bit5;
			break;
		case 6:
			return byte.bits.bit6;
			break;
		case 7:
			return byte.bits.bit7;
			break;
		default:
			return 2;
		}
	}
	return 2;
}

void setBit(uByte *byte, uint8_t index, uint8_t newValue, uint8_t littleEndian) {
	// Verify Host Endianess and Switch if Little
	uByte host;
	host.byte = 1;
	if (host.bits.bit7 != 1) { // Little-endian
		if (littleEndian == 0)
			littleEndian = 1;
		else
			littleEndian = 0;
	}

	if (newValue == 0 || newValue == 1) {
		if (littleEndian == 1) { // Endianess check
			switch (index) {
			case 0:
				(*byte).bits.bit7 = newValue;
				break;
			case 1:
				(*byte).bits.bit6 = newValue;
				break;
			case 2:
				(*byte).bits.bit5 = newValue;
				break;
			case 3:
				(*byte).bits.bit4 = newValue;
				break;
			case 4:
				(*byte).bits.bit3 = newValue;
				break;
			case 5:
				(*byte).bits.bit2 = newValue;
				break;
			case 6:
				(*byte).bits.bit1 = newValue;
				break;
			case 7:
				(*byte).bits.bit0 = newValue;
				break;
			default:
				return;
			}
		} else if (littleEndian == 0) { // Endianess check
			switch (index) {
			case 0:
				(*byte).bits.bit0 = newValue;
				break;
			case 1:
				(*byte).bits.bit1 = newValue;
				break;
			case 2:
				(*byte).bits.bit2 = newValue;
				break;
			case 3:
				(*byte).bits.bit3 = newValue;
				break;
			case 4:
				(*byte).bits.bit4 = newValue;
				break;
			case 5:
				(*byte).bits.bit5 = newValue;
				break;
			case 6:
				(*byte).bits.bit6 = newValue;
				break;
			case 7:
				(*byte).bits.bit7 = newValue;
				break;
			default:
				return;
			}
		}
	}
}

uint8_t countBits(uByte byte) {
	uint8_t bitCounter = 0;
	for (uint8_t i = 0; i < 8; i++) {
		if (getBit(byte, i, 0) == 1) {
			bitCounter++;
		}
	}
	return bitCounter;
}
