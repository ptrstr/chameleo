typedef enum {
	WAV
} format;

format detectFormat(unsigned char *buffer, unsigned long bufferSize);
