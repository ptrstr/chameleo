#pragma once

unsigned char detectWAV(unsigned char *buffer, unsigned long bufferSize);
unsigned long *getWAVOffsets(unsigned char *buffer, unsigned long bufferSize);
