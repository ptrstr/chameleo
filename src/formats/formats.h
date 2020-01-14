#pragma once

typedef enum {
	NULLFORMAT,
	WAV
} FORMAT;

FORMAT getFormat(unsigned char *buffer, unsigned long bufferSize);
unsigned long *getFormatOffsets(unsigned char *buffer, unsigned long bufferSize, FORMAT format);
