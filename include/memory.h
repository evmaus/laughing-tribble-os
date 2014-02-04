#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

//Copys num bytes from source to dest.
void* memcpy_byte(void* dest, const void* source, uint32_t num);

// Exactly like memcpy_byte, but num is the number of words, rather
// than bytes.
void* memcpy_word(void* dest, const void* source, uint32_t num);

#endif
