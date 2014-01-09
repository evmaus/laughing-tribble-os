/*
 * Serial.c:  wraps the UART driver; provides basic io along the serial line.
 * This is mostly temporary/for use in debugging, but I think the UART should
 * be wrapped up.
 * Includes stuff for sending numbers as well as characters.
 */

#include <devices/uart.h>
#include <serial.h>

const uint8_t interactive=1;
uint8_t serial_initialized=0;

void serial_init(){
    uart_init();
    serial_initialized=1;
}

uint8_t serial_state(){
    return serial_initialized;
}

char serial_getc(){
//    if(!serial_initialized) serial_init();
    return uart_getc();
}

// Reads up to (len-1) characters into str (or up to the terminating character).
void serial_gets(char * str, uint32_t len, char term){
    int i;
    char c=255;
    if(!serial_initialized) serial_init(); //safety
    // Read in buffer length-1 characters, or up to the terminator, or null.
    for(i=0; (i<len-1 && c!=term && c); i++){
        c = uart_getc();
        uart_putc(c);
        str[i]=c;
    }
    // Null terminate it.
    str[i]=0;
}

// Converts character c from lower to upper case.
// No effect if c is not a lowercase letter.
char toUpcase(char c){
     if((c > 96) && (c<123)){
         return (c-32);
     } else
         return c;
}

//Minimum helper function.
uint8_t min(uint8_t a, uint8_t b){
    if(a > b) return b;
    return a;
}

// Takes in c as a number or upper case character, returns it as a base 10 int.
// returns 0 if c is not a valid character.
// A=10, B=11, etc.
uint32_t toDec(char c, uint8_t base){
     if(c>47 && c<min(58, 48+base)) //numbers
         return (c-48);
     else if(c>64 && base > 10 && c < min(91, 65+(base-10))) //letters
         return (c-55);
     else
         return 0;
}

uint32_t serial_getInt(uint8_t base){
    char str[33]; // Lowest base taken is 2, and we're dealing with 32 bit integers.
    char* ptr=str;
    uint32_t ret=0;

    if(base>36) return 0;
    if(!serial_initialized) serial_init(); //safety
    
    serial_gets(str, 33, '\r');
    while(*ptr){
       *ptr = toUpcase(*ptr);
       if((*ptr > 47 && *ptr < 58) || (*ptr > 64 && *ptr < 91))
            ret=toDec(*ptr, base)+ret*base;
       ptr++;
    }
    return ret;
}


void serial_putc(const char c){
    //if(!serial_initialized) serial_init();
    uart_putc(c);
}

void serial_puts(const char* c){
    //if(!serial_initialized) serial_init();
    uart_puts(c);
    uart_putc('\r');
    uart_putc('\n');
}

char convIntToC(uint32_t i){
    if(i > 16) return 0; //ERROR.
    if(i < 10) return i+48;    
    if(i > 9) return i+55;
    return 0;
}

void serial_putIntHex(const uint32_t i){
    uint32_t temp=i;
    uint8_t shift=0;
    if(!serial_initialized) serial_init();
    uart_putc('0');
    uart_putc('x'); 
    for(shift=0; shift<32; shift+=4){
       uart_putc(convIntToC( (temp << shift & (1 << 31 | 1 << 30 | 1 << 29 | 1 << 28)) >> 28));
    }
    uart_putc('\r'); uart_putc('\n');
}
