#pragma once

unsigned char detectJPEG(unsigned char *buffer, unsigned long long int bufferSize);
void getJPEGOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize);
