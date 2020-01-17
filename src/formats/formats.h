#pragma once

typedef enum {
	NULLFORMAT,
	WAVE,
	BITMAP,
	JFIF,
	PNG
} FORMAT;

FORMAT getFormat(unsigned char *buffer, unsigned long long int bufferSize);
void getFormatOffsets(unsigned char *buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int ***offsets, unsigned long long int *offsetsSize);
void startFormatBuffer(unsigned char **buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int **offsets, unsigned long long int offsetsSize)
void endFormatBuffer(unsigned char **buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int **offsets, unsigned long long int offsetsSize);
