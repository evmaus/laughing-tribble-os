#IFNDEF SERIAL_H
#DEFINE SERIAL_H

#include <stdint.h>
//Functions for reading strings, int, character

uint32_t initialized = 0;
void serial_init();

void serial_readString(char * str, uint32_t len); // Reads (len-1) characters into the string,
                                                  // then null-terminates it.
uint32_t serial_readIntDec(); //Expects a number on the serial line, works similar to
                              //atoi(), in that it reads up to a non-integer value.
uint32_t serial_readChar();  //Get a character from the serial line.

//Output
void serial_writeChar(char c); // Write a character to the serial line.

void serial_writeString(char * c); // Write a string to the serial line.

void serial_writeIntDec(uint32_t i); //Write an integer in base 10 to the serial line.

void serial_writeIntHex(uint32_t i); //Write an integer in base 16 to the serial line.

#ENDIF
