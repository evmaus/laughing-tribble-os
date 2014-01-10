#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
//Functions for reading strings, int, character

void serial_init();  //Initializes the serial line.
uint8_t serial_state();
/* WARNING: This function should be protected in some way!  Current implementation allows arbitrary writes
   to memory if user code is allowed to call it and it resides in the kernel! 
   That would be bad! Only in there for debugging now. */
void serial_gets(char * str, uint32_t len, char term); // Reads up to the terminating character or len-1 characters
                                                       // into the string pointed to by str, and null terminates it.

uint32_t serial_getInt(uint8_t base); // Expects a number on the serial line, in the base specified.
                                      // Base must be less than 36; if greater, returns 0.  Expects
                                      // it to be counted in the normal way:  8, 9, A, B, etc. (or a, b).
                                      // Assumes that the string intended for conversion is null terminated.
                                      // ignores all characters that aren't appropriate for the base.
char serial_getc();  //Get a character from the serial line.

//Output
void serial_putc(const char c); // Write a character to the serial line.

void serial_puts(const char * c); // Write a null terminated string to the serial line.

void serial_putIntHex(const uint32_t i); //Write an integer in base 16 to the serial line.


#endif
