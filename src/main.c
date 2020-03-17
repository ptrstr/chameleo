#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "uByte/uByte.h"
#include "formats/formats.h"
#include "formats/utility.h"
#include "steganographer/steganographer.h"
#define VERSION "0.2.2"


#define showError(errorMessage) \
	{ fprintf(stderr, "Error: " errorMessage "\n"); goto errorfree; }
#define showHelp() \
	{ printf("" \
			"Usage: %s [OPTIONS]\n" \
			"All-In-One Steganography Tool\n" \
			"\nMandatory Arguments:\n" \
			"    -b, --active-bits    Set bits to overwrite\n" \
			"    -e, --encode         Set mode to ENCODING\n" \
			"    -d, --decode         Set mode to DECODING\n" \
			"    -t, --target         Set target file\n" \
			"                           ENCODING: This will be used as the source file\n" \
			"                           DECODING: This will be used as the encoded file\n" \
			"    -s, --secret         Set secret file\n" \
			"                           ENCODING: This will be used as the file containing the secret\n" \
			"                           DECODING: This will be used as the secret's original size in bytes\n" \
			"    -o, --output         Set output file. ONLY mandatory for ENCODING\n" \
			"                           ENCODING: This file will be used to output the steganographed target\n" \
			"                           DECODING: This file will be used to output the secret from the steganographed target\n" \
			"\nOptional Arguments:\n" \
			"    -h, --help           Print this message and exit\n" \
			"    -v, --version        Print version information and other information and exit\n" \
			"    --size-header        Will add a header to the steganographed data indicating the size of the data.\n" \
			"                         With this argument the -s argument can be suppressed in DECODING\n" \
			"    --offsets            Use custom offsets. lld must be decimal\n" \
			"                         Format:  --offsets OFFSETSLEN OFFSETSTART:OFFSETEND\n" \
			"                         Example: --offsets 2 8:98 132:157\n" \
			"On errors, the help message or the version information, the program will exit with exit code 1.\n" \
			"Otherwise it will return 0.\n" \
			, argv[0]); goto errorfree; }
#define showVersion() \
	{ printf("" \
			"chameleo " VERSION "\n" \
			"Copyright (C) 2020 ptrstr.\n" \
			"This program uses zlib for the PNG (de)compression.\n" \
			"\nSupported Formats:\n" \
			"    WAVE audio file (.wav)\n" \
			"    JFIF image file (.jpg)\n" \
			"    BMP image file (.bmp)\n" \
			"    PNG image file (.png)\n" \
			); goto errorfree; }

int main(int argc, char *argv[]) {
	// Variable Initialization
	uByte *activeBits;
	uint64_t activeBitsSize = 0;
	uint8_t isEncoding = 2;

	uint8_t *targetBuffer = NULL;
	uint64_t targetSize;
	FILE *targetFile = NULL;
	char *targetFileName = NULL;

	uint8_t *secretBuffer = NULL;
	uint64_t secretSize;
	FILE *secretFile = NULL;
	char *secretFileName = NULL;

	uint8_t *outputBuffer = NULL; // Will only be allocated when steganographer is encoding. Declared here for the errorfree: label
	FILE *outputFile = NULL;
	char *outputFileName = NULL;

	uint64_t **offsets = NULL;
	uint64_t offsetsSize = 0;

	uint8_t isUsingHeader = 0;

	uint8_t isUsingCustomOffsets = 0;

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
				if (i + 1 >= argc) break;
				if (strlen(argv[i+1]) % 8 != 0)
					showError("Bit pattern invalid! Make sure you are entering full bytes!");
				activeBits = (uByte*)calloc(strlen(argv[i+1]) / 8, sizeof(uByte));
				activeBitsSize = strlen(argv[i+1]) / 8;
				for (uint64_t j = 0; j < strlen(argv[i+1]); j += 8) {
					uint8_t currentActiveBits[8] = {argv[i+1][j], argv[i+1][j+1], argv[i+1][j+2], argv[i+1][j+3], argv[i+1][j+4], argv[i+1][j+5], argv[i+1][j+6], argv[i+1][j+7]};
					activeBits[j/8].byte = strtoul((char*)currentActiveBits, NULL, 2);
				}
				setBit(&mandatoryCheck, 0, 1, 0);
			} else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encode") == 0) {
				isEncoding = 1;
				setBit(&mandatoryCheck, 1, 1, 0);
			} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decode") == 0) {
				isEncoding = 0;
				setBit(&mandatoryCheck, 1, 1, 0);
			} else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--target") == 0) {
				if (i + 1 >= argc) break;
				targetFileName = argv[i+1];
				setBit(&mandatoryCheck, 2, 1, 0);
			} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--secret") == 0) {
				if (i + 1 >= argc) break;
				secretFileName = argv[i+1];
				secretSize = strtoull(argv[i+1], NULL, 10);
				setBit(&mandatoryCheck, 3, 1, 0);
			} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				if (i + 1 >= argc) break;
				outputFileName = argv[i+1];
				setBit(&mandatoryCheck, 4, 1, 0);
			} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
				showHelp();
			} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
				showVersion();
			} else if (strcmp(argv[i], "--size-header") == 0) {
				isUsingHeader = 1;
			} else if (strcmp(argv[i], "--offsets") == 0) {
				isUsingCustomOffsets = 1;
				uint64_t offsetsToAdd = 0;
				sscanf(argv[i+1], "%" SCNu64, &offsetsToAdd);
				for (uint64_t j = 1; j <= offsetsToAdd; j++) {
					uint64_t startOffset = 0;
					uint64_t endOffset = 0;
					sscanf(argv[i+1+j], "%" SCNu64 ":%" SCNu64, &startOffset, &endOffset);
					addOffset(&offsets, &offsetsSize, startOffset, endOffset);
				}
			}
		}

		if (isUsingHeader && isEncoding == 0)
			setBit(&mandatoryCheck, 3, 1, 0);

		uint8_t requiredArguments = 4;
		if (getBit(mandatoryCheck, 1, 0) == 1 && isEncoding == 1)
			requiredArguments = 5;

		if (countBits(mandatoryCheck) < requiredArguments || isEncoding == 2) {
			for (uint8_t i = 0; i < requiredArguments; i++) {
				if (getBit(mandatoryCheck, i, 0) == 0) {
					signed char argument[6] = {'-', 0, 0, 0, 0, 0};
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
	if (!targetFile)
		showError("Target file could not be opened! Make sure it exists or you have permissions to view it.");
	fseek(targetFile, 0, SEEK_END);
	targetSize = ftell(targetFile);
	rewind(targetFile);
	targetBuffer = (uint8_t*)calloc(targetSize, 1);
	if (!targetBuffer)
		showError("Target buffer could not be allocated! Make sure enough memory is free on your system.");
	fread(targetBuffer, targetSize, 1, targetFile);
	fclose(targetFile);
	targetFile = NULL;

	// Get file format
	FORMAT format = getFormat(targetBuffer, targetSize);
	if (format == NULLFORMAT && !isUsingCustomOffsets)
		showError("The target filetype is unrecognized! See the -v argument to show all current supported formats.");

	if (isUsingCustomOffsets)
		format = NULLFORMAT;

	// Get file offsets for steganography
	getFormatOffsets(targetBuffer, targetSize, format, &offsets, &offsetsSize);
	if (!offsets)
		showError("An error occured while reading the file! Make sure enough memory is free on your system and that the file is not corrupted.");

	// Prepare buffer for exceptions
	startFormatBuffer(&targetBuffer, &targetSize, format, &offsets, &offsetsSize);

	outputFile = fopen(outputFileName, "wb");
	if (!outputFile)
		showError("Output file could not be opened for writing! Make sure you have permissions.");

	// Steganographer starts here
	if (isEncoding == 1) {
		// Open secret file for reading
		secretFile = fopen(secretFileName, "rb");
		if (!secretFile)
			showError("Secret file could not be opened! Make sure it exists or you have permissions to view it.");
		fseek(secretFile, 0, SEEK_END);
		secretSize = ftell(secretFile);
		rewind(secretFile);
		secretBuffer = (uint8_t*)calloc(secretSize, 1);
		if (!secretBuffer)
			showError("Secret buffer could not be allocated! Make sure enough memory is free on your system.");
		fread(secretBuffer, secretSize, 1, secretFile);
		fclose(secretFile);
		secretFile = NULL;

		// Add header if necessary
		if (isUsingHeader) {
			uint8_t *secretBufferBackup = (uint8_t*)calloc(secretSize, 1);
			if (!secretBufferBackup)
				showError("Secret buffer backup could not be allocated! Make sure enough memory is free on your system.");
			memcpy(secretBufferBackup, secretBuffer, secretSize);
			secretBuffer = (uint8_t*)realloc(secretBuffer, secretSize + sizeof(uint64_t));
			if (!secretBuffer)
				showError("Secret buffer could not be reallocated! Make sure enough memory is free on your system.");
			secretSize += sizeof(uint64_t);
			memcpy(secretBuffer, &secretSize, sizeof(uint64_t));
			memcpy(secretBuffer + sizeof(uint64_t), secretBufferBackup, secretSize - sizeof(uint64_t));
			free(secretBufferBackup);
		}

		// Allocate output
		outputBuffer = (uint8_t*)calloc(targetSize, 1);
		if (!outputBuffer)
			showError("Output buffer could not be allocated! Make sure enough memory is free on your system.");

		// Steganograph!
		uint8_t status = steganograph(targetBuffer, targetSize, secretBuffer, secretSize, &outputBuffer, offsets, offsetsSize, activeBits, activeBitsSize);
		if (status == 1)
			showError("File size mismatch! Secret file can't fit in target with this configuration!");
		if (status == 2)
			showError("An unhandled error happened.");
		if (status == 3)
			showError("Offsets are bigger than target file size.");

		// Finish off any exceptions
		endFormatBuffer(&outputBuffer, &targetSize, format, offsets, offsetsSize);

		printf("Successfully steganographed %" PRIu64 " bytes from %s in %s inside %s.\n", secretSize, secretFileName, targetFileName, outputFileName);

		// Do this to cancel any \0 errors
		fwrite(outputBuffer, 1, targetSize, outputFile);
	} else if (isEncoding == 0) {
		// Get size from header if present
		if (isUsingHeader) {
			uint8_t *secretChunkSize = (uint8_t*)calloc(1, sizeof(uint64_t));
			if (!secretChunkSize)
				showError("Secret chunk size buffer could not be allocated! Make sure enough memory is free on your system.");
			uint8_t status = desteganograph(targetBuffer, targetSize, &secretChunkSize, sizeof(uint64_t), offsets, offsetsSize, activeBits, activeBitsSize);
			if (status == 2)
				showError("An unhandled error happened.");
			if (status == 3)
				showError("Offsets are bigger than target file size.");
			memcpy(&secretSize, secretChunkSize, sizeof(uint64_t));
		}

		// Allocate output
		outputBuffer = (uint8_t*)calloc(secretSize, 1);
		if (!outputBuffer)
			showError("Output buffer could not be allocated! Make sure enough memory is free on your system.");

		// Desteganograph!
		uint8_t status = desteganograph(targetBuffer, targetSize, &outputBuffer, secretSize, offsets, offsetsSize, activeBits, activeBitsSize);
		if (status == 2)
			showError("An unhandled error happened.");
		if (status == 3)
			showError("Offsets are bigger than target file size.");

		printf("Successfully desteganographed %" PRIu64 " bytes from %s in %s.\n", secretSize, targetFileName, outputFileName);

		// Do this to cancel any \0 errors
		uint64_t beginningOffset = 0;
		if (isUsingHeader)
			beginningOffset = sizeof(uint64_t);
		fwrite(outputBuffer + beginningOffset, 1, secretSize - beginningOffset, outputFile);
	} else {
		showError("[visible confusion]");
	}

	fclose(outputFile);
	outputFile = NULL;

	free(targetBuffer);
	free(secretBuffer);
	free(outputBuffer);
	for (uint64_t i = 0; i < offsetsSize; i++)
		free(offsets[i]);
	free(offsets);

	return 0;

	// This part will never be called unless a goto is specified. It is used to free buffers and other allocated variables (safe panic)
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
	if (offsets)
		free(offsets);
	return 1;
}
