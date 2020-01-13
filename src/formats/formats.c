#include "formats.h"
#include "wav/wav.h"

format detectFormat(unsigned char *buffer, unsigned long bufferSize) {
	if (detectWAV(buffer, bufferSize)) return WAV;
}
