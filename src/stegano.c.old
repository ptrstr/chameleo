#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

unsigned char getBit(uByte byte, unsigned char index) {
	unsigned char littleEndian = 1;
  // Assumes system is little-endian
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

void setBit(uByte *byte, unsigned char index, unsigned char newValue) {
	unsigned char littleEndian = 1;
  if (newValue == 0 || newValue == 1) {
    // Assumes system is little-endian
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

unsigned char countBits(uByte byte) {
	unsigned char bitCounter = 0;
	for (unsigned char i = 0; i < 8; i++) {
		if (getBit(byte, i) == 1) {
			bitCounter++;
		}
	}
	return bitCounter;
}

void showError(const char *error) {
	printf("Error: %s\n", error);
	exit(1);
}

void steganograph(unsigned char *targetBuffer, unsigned long targetSize, char *targetFileName, unsigned char *secretBuffer, unsigned long secretSize, unsigned char *outputBuffer, uByte activeBits) {
	// Handle file types
	if (targetFileName[strlen(targetFileName) - 4] == '.' &&
		targetFileName[strlen(targetFileName) - 3] == 'w' &&
		targetFileName[strlen(targetFileName) - 2] == 'a' &&
		targetFileName[strlen(targetFileName) - 1] == 'v') {
		
		/// HEADER
		// Check Magic Bytes
		if (targetBuffer[0] != 'R' || targetBuffer[1] != 'I' || targetBuffer[3] != 'F' || targetBuffer[3] != 'F') {
			showError("File header is not correct. Make sure your wave file is valid!");
		}
		
		if (targetBuffer[8] != 'W' || targetBuffer[9] != 'A' || targetBuffer[10] != 'V' || targetBuffer[11] != 'E') {
			showError("File header is not correct. Make sure your wave file is valid!");
		}
		
		unsigned long offset = targetSize;
		// Get offset (wrong way)
		for (unsigned long i = 36; i < targetSize; i++) {
			if (targetBuffer[i] == 'd' && targetBuffer[i+1] == 'a' && targetBuffer[i+2] == 't' && targetBuffer[i+3] == 'a') {
				offset = i + 8;
				break;
			}
		}
		
		// Check if arguments match file size
		if (countBits(activeBits) * (targetSize - offset) < 8 * secretSize) {
			showError("File size mismatch! Secret file is can't fit in target with this configuration!");
		}
		
		// Steganographer starts here
		unsigned char secretBitIndex = 0;
		unsigned long secretByteIndex = 0;
		memcpy(outputBuffer, targetBuffer, targetSize);
		for (unsigned long targetByteIndex = offset; targetByteIndex < targetSize; targetByteIndex++) {
			for (unsigned char targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
				if (getBit(activeBits, targetBitIndex) == 1) {
					uByte outputByte;
					outputByte.byte = outputBuffer[targetByteIndex];
					uByte secretByte;
					secretByte.byte = secretBuffer[secretByteIndex];
					setBit(&outputByte, targetBitIndex, getBit(secretByte, secretBitIndex));
					outputBuffer[targetByteIndex] = outputByte.byte;
					
					secretBitIndex++;
					if (secretBitIndex >= 8) {
						secretBitIndex = 0;
						secretByteIndex++;
						if (secretByteIndex >= secretSize) {
							return;
						}
					}
				}
			}
		}
	} else {
		showError("File extension unknown");
	}
}

/*void desteganograph(unsigned char *targetBuffer, unsigned long targetSize, char *targetFileName, unsigned char *secretBuffer, unsigned long secretSize, uByte activeBits) {
	// Handle file types
	if (targetFileName[strlen(targetFileName) - 4] == '.' &&
		targetFileName[strlen(targetFileName) - 3] == 'w' &&
		targetFileName[strlen(targetFileName) - 2] == 'a' &&
		targetFileName[strlen(targetFileName) - 1] == 'v') {
		
		/// HEADER
		// Check Magic Bytes
		if (targetBuffer[0] != 'R' || targetBuffer[1] != 'I' || targetBuffer[3] != 'F' || targetBuffer[3] != 'F') {
			showError("File header is not correct. Make sure your wave file is valid!");
		}
		
		if (targetBuffer[8] != 'W' || targetBuffer[9] != 'A' || targetBuffer[10] != 'V' || targetBuffer[11] != 'E') {
			showError("File header is not correct. Make sure your wave file is valid!");
		}
		
		unsigned long offset = targetSize;
		// Get offset (wrong way)
		for (unsigned long i = 36; i < targetSize; i++) {
			if (targetBuffer[i] == 'd' && targetBuffer[i+1] == 'a' && targetBuffer[i+2] == 't' && targetBuffer[i+3] == 'a') {
				offset = i + 8;
				break;
			}
		}
		
		// Check if arguments match file size
		if (countBits(activeBits) * (targetSize - offset) < 8 * secretSize) {
			showError("File size mismatch! Secret file is can't fit in target with this configuration!");
		}
		
		// Steganographer starts here
		unsigned char secretBitIndex = 0;
		unsigned long secretByteIndex = 0;
		memcpy(outputBuffer, targetBuffer, targetSize);
		for (unsigned long targetByteIndex = offset; targetByteIndex < targetSize; targetByteIndex++) {
			for (unsigned char targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
				if (getBit(activeBits, targetBitIndex) == 1) {
					uByte outputByte;
					outputByte.byte = outputBuffer[targetByteIndex];
					uByte secretByte;
					secretByte.byte = secretBuffer[secretByteIndex];
					setBit(&outputByte, targetBitIndex, getBit(secretByte, secretBitIndex));
					outputBuffer[targetByteIndex] = outputByte.byte;
					
					secretBitIndex++;
					if (secretBitIndex >= 8) {
						secretBitIndex = 0;
						secretByteIndex++;
						if (secretByteIndex >= secretSize) {
							return;
						}
					}
				}
			}
		}
	} else {
		showError("File extension unknown");
	}
}*/

void deleteme(unsigned char *targetBuffer, unsigned long targetSize, unsigned char *secretBuffer, unsigned long secretSize, uByte activeBits, unsigned long start, unsigned long end) {
	unsigned char secretBitIndex = 0;
	unsigned long secretByteIndex = 0;
	for (unsigned long targetByteIndex = start; targetByteIndex < end && targetByteIndex < targetSize; targetByteIndex++) {
		for (unsigned char targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
			if (getBit(activeBits, targetBitIndex) == 1) {
				uByte secretByte;
				secretByte.byte = secretBuffer[secretByteIndex];
				uByte targetByte;
				targetByte.byte = targetBuffer[targetByteIndex];
				setBit(&secretByte, secretBitIndex, getBit(targetByte, targetBitIndex));
				secretBuffer[secretByteIndex] = secretByte.byte;
				
				secretBitIndex++;
				if (secretBitIndex >= 8) {
					secretBitIndex = 0;
					secretByteIndex++;
					if (secretByteIndex >= secretSize) {
						return;
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[]) {

  // Variable Initialization
  uByte activeBits;
  unsigned char isEncoding;
  
  unsigned char *targetBuffer;
  unsigned long targetSize;
  FILE *targetFile;
  char *targetFileName;
  
  unsigned char *secretBuffer;
  unsigned long secretSize;
  FILE *secretFile;
  char *secretFileName;

  char *outputFileName;

  // Verify Parameters
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-b") == 0) {
      activeBits.byte = strtoul(argv[i + 1], NULL, 2);
    } else if (strcmp(argv[i], "-d") == 0) {
      isEncoding = 0;
    } else if (strcmp(argv[i], "-e") == 0) {
      isEncoding = 1;
    } else if (strcmp(argv[i], "-t") == 0) {
      targetFileName = argv[i + 1];
    } else if (strcmp(argv[i], "-s") == 0) {
      secretFileName = argv[i + 1];
    } else if (strcmp(argv[i], "-o") == 0) {
      outputFileName = argv[i + 1];
    }
  }

  // Open Files
  targetFile = fopen(targetFileName, "rb");
  if (!targetFile) {
    printf("%s\n", targetFileName);
    showError("targetFile does not exist!");
  }
  fseek(targetFile, 0, SEEK_END);
  targetSize = ftell(targetFile);
  rewind(targetFile);
  targetBuffer = (unsigned char *)malloc(targetSize * sizeof(unsigned char));
  fread(targetBuffer, targetSize, 1, targetFile);
  fclose(targetFile);
  
  // Steganographer starts here
  if (isEncoding == 1) {
    secretFile = fopen(secretFileName, "rb");
	  if (!secretFile) {
	    showError("secretFile does not exist!");
	  }
	  fseek(secretFile, 0, SEEK_END);
	  secretSize = ftell(secretFile);
	  rewind(secretFile);
	  secretBuffer = (unsigned char *)malloc(secretSize * sizeof(unsigned char));
	  fread(secretBuffer, secretSize, 1, secretFile);
	  fclose(secretFile);
  
    FILE *outputFile = fopen(outputFileName, "wb+");
    if (!outputFile) {
      printf("%s\n", outputFileName);
      showError("outputFile can't be opened for writing!");
    }
    unsigned char *outputBuffer = (unsigned char*)malloc(targetSize * sizeof(unsigned char));
    if (!outputBuffer) {
    	showError("outputBuffer cannot be allocated!");
    }
    steganograph(targetBuffer, targetSize, targetFileName, secretBuffer, secretSize, outputBuffer, activeBits);
    for (unsigned long i = 0; i < targetSize; i++) {
    	fprintf(outputFile, "%c", outputBuffer[i]);
    }
    fclose(outputFile);
  } else {
    secretFile = fopen(secretFileName, "w+");
	  if (!secretFile) {
	    showError("secretFile does not exist!");
	  }
	  unsigned long start = 3468;
	  unsigned long end = 17013;
	  secretSize = (unsigned long)(((end - start) / 8) / countBits(activeBits)) + 1;
	  secretBuffer = (unsigned char*)calloc(secretSize, 1);
   	deleteme(targetBuffer, targetSize, secretBuffer, secretSize, activeBits, start, end);
   	for (unsigned long i = 0; i < secretSize; i++) {
    	fprintf(secretFile, "%c", secretBuffer[i]);
    }
    fclose(secretFile);
  }
  return 0;
}
