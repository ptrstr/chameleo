#pragma once

unsigned char detectWAVE(unsigned char *buffer, unsigned long long int bufferSize);
void getWAVEOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize);
