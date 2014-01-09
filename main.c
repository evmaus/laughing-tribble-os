/* main.c - the entry point for the kernel */
 
#include <stdint.h>
 
#include <uart.h>
 
#define UNUSED(x) (void)(x)
 
const char hello[] = "\r\nHello World\r\n";
const char halting[] = "\r\n*** system halting ***";
const char EOF = 3;
const char CR = 13;
const char BACKSPACE = 8;
void command_mode();

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    uart_init();
 
    uart_puts(hello);
 
    command_mode();

    // Wait a bit
    //for(volatile int i = 0; i < 10000000; ++i) { }
 
    uart_puts(halting);
}

const uint32_t BUFFER_SIZE=100;


// Buffers a string of up to BUFFER_SIZE characters (not including the terminating zero)
// until a CR is sent.  It then echoes the string back on its own line.
// This state will end when an EOF is sent over the serial port. 
void command_mode(){
    char c;
    char buffer[BUFFER_SIZE+1];
    int count=0;
    
    while(1){
        if(count==(BUFFER_SIZE-1))
           count=0;
        c = uart_getc();
        
        if(c==EOF)
           break;
        if(c==CR) {
          //Send a carriage return and then a newline to set the line to the next line.
          uart_putc('\r');
          uart_putc('\n');
          
          //Terminate the string
          buffer[count]=c;
          buffer[count+1]='\0';
          
          // Echo it to the UART
          uart_puts(buffer);
          
          // Send a newline to set the line to the next line, and reset the buffer.
          uart_putc('\n');
          count=0;
        } else {
    	  //Echo the character.
    	  uart_putc(c);
    	  //If it's printable, print it.
    	  if(c >= 32){
    	     buffer[count]=c;
    	     count++;
          } else if(c==BACKSPACE) count--;
        }
    }
}
