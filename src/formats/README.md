# Creating a format
*Replace where it says $FORMAT by the format ID*

A format needs 4 basic functions:
```
// Function called at very beginning to detect if the current file is of the format $FORMAT
uint8_t detect$FORMAT(uint8_t *buffer, uint64_t bufferSize);

// Function called just after detect$FORMAT. Must get basic offsets
void get$FORMATOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);

// Function called before going to steganographer. Useful for decoding formats that require so.
void start$FORMATBuffer(uint8_t **buffer, uint64_t *bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);

// Function called before writing output. Useful for encoding for formats that require so.
void end$FORMATBuffer(uint8_t **buffer, uint64_t *bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);
```

## The header

The header requires a header (ironic):
```
#pragma once
#include "../utility.h"
```

## Structuring

A basic structure is required:
```
formats/$FORMAT/
formats/$FORMAT/$FORMAT.c
formats/$FORMAT/$FORMAT.h
```

## External Stuff

### formats/formats.h

You will need to add the $FORMAT to the FORMAT enum in formats.h

### formats/formats.c

You will need to add calls to the functions created by $FORMAT in their respective parent callers.
