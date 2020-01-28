#pragma once
#include <inttypes.h>

typedef enum {
	NULLFORMAT,
	WAVE,
	BITMAP,
	JPEG,
	PNG
} FORMAT;

FORMAT getFormat(unsigned char *buffer, uint64_t bufferSize);
void getFormatOffsets(unsigned char *buffer, uint64_t bufferSize, FORMAT format, uint64_t ***offsets, uint64_t *offsetsSize);
void startFormatBuffer(unsigned char **buffer, uint64_t *bufferSize, FORMAT format, uint64_t ***offsets, uint64_t *offsetsSize);
void endFormatBuffer(unsigned char **buffer, uint64_t *bufferSize, FORMAT format, uint64_t **offsets, uint64_t offsetsSize);
