#pragma once

unsigned char detectPNG(unsigned char *buffer, unsigned long long int bufferSize);
void getPNGOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize);
void startPNGBuffer(unsigned char **buffer, unsigned long long int bufferSize, unsigned long long int **offsets, unsigned long long int offsetsSize);
void endPNGBuffer(unsigned char **buffer, unsigned long long int bufferSize, unsigned long long int **offsets, unsigned long long int offsetsSize);
