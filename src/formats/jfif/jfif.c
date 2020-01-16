#include "jfif.h"
#include <stdlib.h>

unsigned char detectJFIF(unsigned char *buffer, unsigned long long int bufferSize) {
	return 1;
}

void getJFIFOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize) {
	(*offsetsSize)++;
	*offsets = (unsigned long long int**)calloc(sizeof(unsigned long long int*), *offsetsSize);
	if (!*offsets)
		return;
	*offsets[0] = (unsigned long long int*)calloc(sizeof(unsigned long long int*), 2);
	if (!*offsets[0])
		return;
		
	for (unsigned long long int i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0xFF && buffer[i+1] == 0xDA)
			*offsets[0][0] = i + 12;
		if (buffer[i] == 0xFF && buffer[i+1] == 0xD9)
			*offsets[0][1] = i;
	}
}
