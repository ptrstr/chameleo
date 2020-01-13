typedef enum {
	NULL,
	WAV
} FORMAT;

FORMAT detectFormat(unsigned char *buffer, unsigned long bufferSize);
unsigned long 