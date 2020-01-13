#include "formats.h"
#include "wav/wav.h"

FORMAT detectFormat(unsigned char *buffer, unsigned long bufferSize) {
	if (detectWAV(buffer, bufferSize)) return WAV;
	return NULL;
}
