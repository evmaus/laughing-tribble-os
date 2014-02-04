#include <memory.h>
#include <stdint.h>

// This copies (num) bytes from src to dest, one at a time.
void* memcpy_byte(void* dest, const void* src, uint32_t num){

    int i;
    /* We're copying the actual bytes here.  We cast to a character pointer because
       c doesn't like dereferencing a void pointer. */
    for(i=0; i<num; i+=1){
        *((char*)(((uint32_t)dest)+i)) = *((char*)(((uint32_t)src)+i));
    }
    return dest;
}


// This copies (num) words from src to dest, one at a time.
void* memcpy_word(void* dest, const void* src, uint32_t num){

    int i;
    /* We're copying the actual bytes here.  We cast to a character pointer because
       c doesn't like dereferencing a void pointer. */
    for(i=0; i<num*4; i+=4){
        *((uint32_t*)(((uint32_t)dest)+i)) = *((uint32_t*)(((uint32_t)src)+i));
    }
    return dest;
    
}



