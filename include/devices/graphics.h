/* graphics.h -- functions for interacting with the screen. */

#ifndef GRAPHICS_H
#define GRAPHICS_H

uint32_t graphics_init();
void graphics_render_test();
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);

#endif


