#pragma once

unsigned char detectJFIF(unsigned char *buffer, unsigned long long int bufferSize);
void getJFIFOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize);