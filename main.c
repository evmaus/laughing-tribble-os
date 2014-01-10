/* main.c - the entry point for the kernel */
 
#include <stdint.h>
#include <devices/graphics.h> 
#include <serial.h>
 
#define UNUSED(x) (void)(x)
 
const char hello[] = "\r\nHello World\r\n";
const char halting[] = "\r\n*** system halting ***";
const char EOF = 3;
const char CR = 13;
const char BACKSPACE = 8;
uint32_t init_devices();
void at_runtime();
void shutdown();

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    uint32_t device_status=0;
    //Initialize the devices we're using.
    device_status=init_devices();
    if(!device_status){
       //Current sole runtime function.
       at_runtime();
    }
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
