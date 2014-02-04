/* main.c - the entry point for the kernel */
/* cannibalized from http://wiki.osdev.org/ARM_RaspberryPi_Tutorial_C which is under CC0 */
 
#include <stdint.h>
#include <devices/graphics.h> 
#include <serial.h>
#include <devices/mmio.h>  //Included while we get interrupts working. 
#include <memory.h>

#define UNUSED(x) (void)(x)
 
const char hello[] = "\r\nHello World\r\n";
const char halting[] = "\r\n*** system halting ***";
const char EOF = 3;
const char CR = 13;
const char BACKSPACE = 8;
uint32_t init_devices();
void at_runtime();
void shutdown();

/*
 * Enabling exceptions
 */
extern void* _exceptions_vector;

void _enable_exceptions(){
    //Basically we have to put the exceptions vector at 0x0.
   memcpy_word(0x0, &_exceptions_vector, 16);
}

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    void* exceptions_base=0x0;
    uint32_t device_status=0;
    uint32_t i;
    //Initialize the devices we're using.
    device_status=init_devices();
    /*if(!device_status){
       //Current sole runtime function.
       at_runtime();
    }*/
    //Print out the exception vector
    //
    serial_puts("Testing memory copy: word");
    memcpy_word(exceptions_base, hello, 4);
    serial_puts(exceptions_base);

    serial_putIntHex((uint32_t)_exceptions_vector);
    serial_puts("Exception Vector is at:");
    serial_putIntHex((uint32_t) &_exceptions_vector);
    serial_puts("Which holds:");
    serial_putIntHex((uint32_t) _exceptions_vector);
    serial_puts("Exception Vector holds:");
    for(i=(uint32_t) &_exceptions_vector; i<=(uint32_t) &_exceptions_vector+64; i+=4){
        serial_putIntHex(i);
        serial_putIntHex(mmio_read(i));
    }

    serial_puts("Enabling exceptions.");
    //So first we'll try setting interrupts.
    _enable_exceptions();
    serial_puts("Vector at 0x0 is: ");
    //We're going to check the values of 0, ... 1c.
    for(i=0; i <= 64; i+=4)
    	serial_putIntHex(mmio_read(i));
    
    serial_puts("Done enabling exceptions.");

    //Try calling an exception.
    asm volatile("swi 0");
    //Shutdown/cleanup functions post runtime.
    shutdown();
}

void shutdown(){
    serial_puts(halting);
}

const char* init_devices_err = "Error in initializing graphics";
uint32_t init_devices(){
    uint32_t status=0;
    serial_init();
    serial_puts(hello);
    status=graphics_init();
    if(status!=0){
        serial_puts(init_devices_err);
        return 1;
    }
    return 0;
}

const char* hex_conv_str = "Enter a value to convert to hex:";
//Current runtime stuff.  Currently just does some echo-ing things, going to get other stuff working soon.
void at_runtime(){
    while(1){
        graphics_render_test();
        serial_puts(hex_conv_str);
        serial_putIntHex(serial_getInt(10));
    }
}
