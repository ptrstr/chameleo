#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uByte.h"
#define VERSION "0.1"



#define showError(errorMessage) \
	printf("Error: " errorMessage "\n"); goto errorfree;
#define showHelp() \
	printf("" \
			"Usage: %s [OPTIONS]\n" \
			"All-In-One Steganography Tool\n" \
			"\nMANDATORY ARGUMENTS:\n" \
			"    -b, --active-bits    Set bits to overwrite\n" \
			"    -e, --encode         Set mode to ENCODING\n" \
			"    -d, --decode         Set mode to DECODING\n" \
			"    -t, --target         Set target file\n" \
			"                           ENCODING: This will be used as the source file\n" \
			"                           DECODING: This will be used as the encoded file\n" \
			"    -s, --secret         Set secret file\n" \
			"                           ENCODING: This will be used as the file containing the secret\n" \
			"                           DECODING: This will be used as the output file\n" \
			"    -o, --output         Set output file. ONLY mandatory for ENCODING\n" \
			"                           ENCODING: This file will be used to output the steganographed target\n" \
			"\nOPTIONAL ARGUMENTS:\n" \
			"    -h, --help           Print this message and exit\n" \
			"    -v, --version        Print version information and other information and exit\n" \
			"On errors, the help message or the version information, the program will exit with exit code 1.\n" \
			"Otherwise it will return 0.\n" \
			, argv[0]); goto errorfree;
#define showVersion() \
	printf("" \
			"stegano " VERSION "\n" \
			"Copyright (C) 2020 ptrstr.\n" \
			); goto errorfree;
int main(int argc, char *argv[]) {
	// Variable Initialization
	uByte activeBits;
	unsigned char isEncoding = 2;
	
	unsigned char *targetBuffer = NULL;
	unsigned long long int targetSize;
	FILE *targetFile = NULL;
	char *targetFileName;
	
	unsigned char *secretBuffer = NULL;
	unsigned long long int secretSize;
	FILE *secretFile = NULL;
	char *secretFileName = NULL;
	
	char *outputBuffer = NULL; // Will only be allocated when steganographer is encoding. Declared here for the errorfree: label
	FILE *outputFile = NULL;
	char *outputFileName = NULL;
	
	// Verify Parameters and Assign Them
	{ // In Scope to prevent access to unnecessary and temporary variables from other places in the function
		uByte mandatoryCheck;
		mandatoryCheck.byte = 0;
		/*
		 * bit0: -b
		 * bit1: -e, -d
		 * bit2: -t
		 * bit3: -s
		 * bit4: -o
		*/
		for (int i = 1; i < argc; i++) { // Skip first one because it is program's name
			if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--active-bits") == 0) {
				char *endPointer = NULL;
				activeBits.byte = strtoul(argv[i+1], &endPointer, 2);
				if (endPointer == argv[i+1]) showError("-b is invalid! Make sure you enter a byte as a binary number. I.E: 00000101");
				setBit(&mandatoryCheck, 0, 1, 0);
			} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decode") == 0) {
				isEncoding = 0;
				setBit(&mandatoryCheck, 1, 1, 0);
			} else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encode") == 0) {
				isEncoding = 1;
				setBit(&mandatoryCheck, 1, 1, 0);
			} else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--target") == 0) {
				targetFileName = argv[i+1];
				setBit(&mandatoryCheck, 2, 1, 0);
			} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--secret") == 0) {
				secretFileName = argv[i+1];
				setBit(&mandatoryCheck, 3, 1, 0);
			} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				outputFileName = argv[i+1];
				setBit(&mandatoryCheck, 4, 1, 0);
			} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
				showHelp();
			} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
				showVersion();
			}
		}
		unsigned char requiredArguments = 4;
		if (getBit(mandatoryCheck, 1, 0) == 1 && isEncoding == 1) requiredArguments = 5;
		if (countBits(mandatoryCheck) < requiredArguments) {
			for (unsigned char i = 0; i < requiredArguments; i++) {
				if (getBit(mandatoryCheck, i, 0) == 0) {
					signed char argument[5] = {'-', 0, 0, 0, 0};
					switch (i) {
						case 0:
							argument[1] = 'b';
							break;
						case 1:
							argument[1] = 'e';
							argument[2] = '/';
							argument[3] = '-';
							argument[4] = 'd';
							break;
						case 2:
							argument[1] = 't';
							break;
						case 3:
							argument[1] = 's';
							break;
						case 4:
							argument[1] = 'o';
							break;
					}
					printf("Missing argument %s.\n", argument);
				}
			}
			showHelp();
		}
	}
	
	// Open basic files
	targetFile = fopen(targetFileName, "rb");
	if (!targetFile) {
		showError("Target file could not be opened! Make sure it exists or you have permissions to view it.");
	}
	fseek(targetFile, 0, SEEK_END);
	targetSize = ftell(targetFile);
	rewind(targetFile);
	targetBuffer = (unsigned char*)malloc(targetSize * sizeof(unsigned char));
	if (!targetBuffer) {
		showError("Target buffer could not be allocated! Make sure enough memory is free on your system.");
	}
	fread(targetBuffer, targetSize, 1, targetFile);
	fclose(targetFile);
	
	// Steganographer starts here
	
	return 0;
	
	// This part will never be called unless a goto is specified. It is used to free buffers and other allocated variables
	errorfree:;
	if (targetBuffer)
		free(targetBuffer);
	if (targetFile)
		fclose(targetFile);
	if (secretBuffer)
		free(secretBuffer);
	if (secretFile)
		fclose(secretFile);
	if (outputBuffer)
		free(outputBuffer);
	if (outputFile)
		fclose(outputFile);
	return 1;
}
