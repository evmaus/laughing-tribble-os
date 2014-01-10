/*
 * The Raspberry Pi uses a (as far as I can tell) very undocumented system of mailboxes for communicating with
 * the GPU.  You basically write things to a location and the GPU reads them and then acts on it and returns a
 * status.
 * The goal of the code here is to define code for writing to the screen.
*/

#include <devices/mmio.h>
#include <serial.h>
//#include <devices/gpu.h>

enum {
    //The mailbox base address:
    GPUFB_BASE = 0x2000B880,
    GPUFB_READ = (GPUFB_BASE + 0x0),
    GPUFB_POLL = (GPUFB_BASE + 0x10),
    GPUFB_SENDER = (GPUFB_BASE + 0x14),
    GPUFB_STATUS = (GPUFB_BASE + 0x18),
    GPUFB_CONFIG = (GPUFB_BASE + 0x1C),
    GPUFB_WRITE = (GPUFB_BASE + 0x20),
};

struct framebuffer {
    uint32_t width;  // Real Width
    uint32_t height;  // Real Height
    uint32_t vwidth; // Virtual width
    uint32_t vheight; // Virtual Height
    uint32_t pitch;     // Set by the GPU, set 0.
    uint32_t depth;    // Bit depth
    uint32_t xoffset;   // X offset
    uint32_t yoffset;   // Y offset
    uint32_t gpu_ptr;   //Pointer to the GPU, set by it.
    uint32_t gpu_size;  // Size of the FB in bytes.
} fb __attribute__ ((aligned(4))) = {1024, 768, 1024, 768, 0, 32, 0, 0, 0, 0};

uint32_t graphics = 0;

/*
 *  Mailbox functions.  As they are currently only being used by the framebuffer, we are keeping them
 *  local to this file.
 */
const char* mailbox_write_wait = "Waiting...";
const char* mailbox_write_entry = "\r\nEntering mailbox write\r\n";
const char* mailbox_write_err = "\r\nError in mailbox write: invalid input\r\n";
void mailbox_write(uint32_t data, uint32_t channel){
    //Validating input; from reading, the lowest 4 bits need to be all 0.
    serial_puts(mailbox_write_entry);

    if(!(data & 0xF) && (channel < 16)){
        while((mmio_read(GPUFB_STATUS) & 0x8000000));
        //{    serial_puts(mailbox_write_wait); 
        //     serial_putIntHex(GPUFB_STATUS); 
        //     serial_putIntHex(mmio_read(GPUFB_STATUS));
        //} //Wait until we can send.
        mmio_write(GPUFB_WRITE, data+channel);
    } else {
        serial_puts(mailbox_write_err);
    }
    //You should write an error handling system soon, so that you can get debugging output.
}

const char* mailbox_read_entry = "\r\nEntering mailbox read\r\n";
const char* mailbox_read_err = "\r\nMailbox Read Error: invalid channel.\r\n";

uint32_t mailbox_read(uint32_t channel){
   uint32_t chanRead=16;
   uint32_t data=0;   

   serial_puts(mailbox_read_entry);

   //Make sure we have a valid channel
   if(channel<16){
       do{
           while((GPUFB_STATUS & 0x4000000)); //Wait until we can read.
           data = mmio_read(GPUFB_READ);
           chanRead = (data & 0xF); //Get the channel that we read from.
       } while(chanRead!=channel);
       return (data & 0xFFFFFFF0); // Return the data.
   } else {
       serial_puts(mailbox_read_err);
       return 0;
   }
}


uint32_t graphics_init(){
     uint32_t response=0;
     uint32_t fb_loc = (uint32_t) &fb;
     mailbox_write(fb_loc+0x40000000, 1);
     response = mailbox_read(1);
     if(response!=0){
         return 1;
     } else {
         serial_putIntHex(fb.width);
         serial_putIntHex(fb.height);
         serial_putIntHex(fb.vwidth);
         serial_putIntHex(fb.vheight);
         serial_putIntHex(fb.gpu_size);
         serial_putIntHex(fb.depth);
         return 0;
     }
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color){
    uint32_t * pixel = 0;
    if((color >> fb.depth) == 0 && (x < fb.vwidth) && (y<fb.vheight)){ //Make sure we have appropriate color depth, x, y.
        pixel = (uint32_t*) (fb.gpu_ptr + x*4+y*4*fb.vwidth);
        *pixel = color;
    }
}

//const char* grt_enter = "Entering the graphics render test.";
void graphics_render_test(){
   uint32_t color=0;
//   uint32_t* base = (uint32_t*) fb.gpu_ptr;
   //serial_puts(grt_enter);
   for(int i=0; i<fb.vwidth; i++){
      for(int j=0; j<fb.vheight; j++){
	   //*base = color;
	   //serial_putIntHex((uint32_t)base);
           //serial_putIntHex(*base);
           draw_pixel(i, j, color);
           color++;
	   //base+=1;
      }
   }
}




