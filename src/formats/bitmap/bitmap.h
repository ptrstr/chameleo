#pragma once

unsigned char detectBitmap(unsigned char *buffer, unsigned long long int bufferSize);
void getBitmapOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize);
