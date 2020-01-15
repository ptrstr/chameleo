#pragma once

unsigned char detectBitmap(unsigned char *buffer, unsigned long long int bufferSize);
void getBitmapOffsets(unsigned long long int *offsets, unsigned char *buffer, unsigned long long int bufferSize);
