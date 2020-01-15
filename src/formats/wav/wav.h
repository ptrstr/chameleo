#pragma once

unsigned char detectWAV(unsigned char *buffer, unsigned long long int bufferSize);
void getWAVOffsets(unsigned long long int *offsets, unsigned char *buffer, unsigned long long int bufferSize);
