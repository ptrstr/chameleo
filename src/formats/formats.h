#pragma once

typedef enum {
	NULLFORMAT,
	WAVE,
	BITMAP,
	JFIF,
} FORMAT;

FORMAT getFormat(unsigned char *buffer, unsigned long long int bufferSize);
void getFormatOffsets(unsigned char *buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int ***offsets, unsigned long long int *offsetsSize);
