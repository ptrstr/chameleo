#include "steganographer.h"
#include <stdlib.h>
#include <string.h>

// TODO: Add prevention for impossible scenario where offsets are bigger than the file's size
// TODO: Add endianess customization

uint8_t steganograph(uint8_t *targetBuffer, uint64_t targetSize, uint8_t *secretBuffer, uint64_t secretSize, uint8_t *outputBuffer, uint64_t **offsets, uint64_t offsetsSize, uByte *activeBits, uint64_t activeBitsSize) {
	// Check if secret fits in target
	uint64_t activeBitsCount = 0;
	for (uint64_t i = 0; i < activeBitsSize; i++)
		activeBitsCount += countBits(activeBits[i]);
	uint64_t totalBytes = 0;
	for (uint64_t i = 0; i < offsetsSize; i++)
		totalBytes += offsets[i][1] - offsets[i][0];
	if (activeBitsCount * (totalBytes / activeBitsSize) < 8 * (secretSize)) return 1;

	// Steganographer starts here
	uint8_t secretBitIndex = 0;
	uint64_t secretByteIndex = 0;

	uint64_t activeBitsIndex = 0;

	memcpy(outputBuffer, targetBuffer, targetSize);
	for (uint64_t offsetsIndex = 0; offsetsIndex < offsetsSize; offsetsIndex++) {
		for (uint64_t targetByteIndex = offsets[offsetsIndex][0]; targetByteIndex < offsets[offsetsIndex][1]; targetByteIndex++) {
			for (uint8_t targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
				if (getBit(activeBits[activeBitsIndex], targetBitIndex, 0) == 1) {
					uByte outputByte;
					outputByte.byte = outputBuffer[targetByteIndex];
					uByte secretByte;
					secretByte.byte = secretBuffer[secretByteIndex];
					setBit(&outputByte, targetBitIndex, getBit(secretByte, secretBitIndex, 0), 0);
					outputBuffer[targetByteIndex] = outputByte.byte;

					secretBitIndex++;
					if (secretBitIndex >= 8) {
						secretBitIndex = 0;
						secretByteIndex++;
						if (secretByteIndex >= secretSize) {
							return 0;
						}
					}
				}
			}
			activeBitsIndex++;
			if (activeBitsIndex >= activeBitsSize)
				activeBitsIndex = 0;
		}
	}
	return 2;
}

uint8_t desteganograph(uint8_t *targetBuffer, uint64_t targetSize, uint8_t *secretBuffer, uint64_t secretSize, uint64_t **offsets, uint64_t offsetsSize, uByte *activeBits, uint64_t activeBitsSize) {
	// Desteganographer starts here
	uint8_t secretBitIndex = 0;
	uint64_t secretByteIndex = 0;

	uint64_t activeBitsIndex = 0;

	for (uint64_t offsetsIndex = 0; offsetsIndex < offsetsSize; offsetsIndex++) {
		for (uint64_t targetByteIndex = offsets[offsetsIndex][0]; targetByteIndex < offsets[offsetsIndex][1]; targetByteIndex++) {
			for (uint8_t targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
				if (getBit(activeBits[activeBitsIndex], targetBitIndex, 0) == 1) {
					uByte targetByte;
					targetByte.byte = targetBuffer[targetByteIndex];
					uByte secretByte;
					secretByte.byte = secretBuffer[secretByteIndex];
					setBit(&secretByte, secretBitIndex, getBit(targetByte, targetBitIndex, 0), 0);
					secretBuffer[secretByteIndex] = secretByte.byte;

					secretBitIndex++;
					if (secretBitIndex >= 8) {
						secretBitIndex = 0;
						secretByteIndex++;
						if (secretByteIndex >= secretSize) {
							return 0;
						}
					}
				}
			}
			activeBitsIndex++;
			if (activeBitsIndex >= activeBitsSize)
				activeBitsIndex = 0;
		}
	}
	return 2;
}
