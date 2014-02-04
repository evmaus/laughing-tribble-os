/* The actual code for handling exceptions... eventually.  Currently does just about nothing other than
 * telling you that you've hit one.... if the serial port is running.  If it isn't, it'll try to get it running.
 * This is absolutely not how exceptions should be handled; this is to make sure that I have the right idea as far
 * as how to set up the exceptions table. */

#include <serial.h> 

const char* reset = "Reset exception";
const char* undef_inst = "Undefined instruction exception";
const char* software_exception = "Software interrupt exception";
const char* prefetch_exception = "Prefetch abort exception";
const char* data_exception = "Data abort exception";
const char* irq = "IRQ exception";
const char* fiq = "FIQ exception";

void __reset() __attribute__((naked));
void __undefined_instruction() __attribute__((naked));
void __software_interrupt() __attribute__((naked));
void __prefetch_abort() __attribute__((naked));
void __data_abort() __attribute__((naked));
void __irq_exception() __attribute__((naked));
void __fiq_exception() __attribute__((naked));

void __reset(){
    serial_puts(reset);
    while(1);
}

void __undefined_instruction(){
    serial_puts(undef_inst);
    while(1);
}

void __software_interrupt(){
    serial_puts(software_exception);
    while(1);
}

void __prefetch_abort(){
   serial_puts(prefetch_exception);
   while(1);
}

void __data_abort(){
   serial_puts(data_exception);
   while(1);
}

void __irq_exception(){
   serial_puts(irq);
   while(1);
}

void __fiq_exception(){
   serial_puts(fiq);
   while(1);
}
