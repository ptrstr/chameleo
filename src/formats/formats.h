#pragma once

typedef enum {
	NULLFORMAT,
	WAV,
	BITMAP
} FORMAT;

FORMAT getFormat(unsigned char *buffer, unsigned long long int bufferSize);
unsigned long long int *getFormatOffsets(unsigned char *buffer, unsigned long long int bufferSize, FORMAT format);
