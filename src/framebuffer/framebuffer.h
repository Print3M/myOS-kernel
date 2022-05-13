#pragma once
#include <libc/stdint.h>

typedef struct {
	void *base_address;
	size_t buffer_size;
	uint16_t width;
	uint16_t height;
	uint16_t pixels_per_scanline;
	uint8_t bytes_per_pixel;
} Framebuffer;

typedef struct {
	char *bitmap;
	uint16_t x_off;
	uint16_t y_off;
	uint8_t height;
	uint8_t width;
	uint32_t fg_color;
	uint32_t bg_color;
} Glyph;

// Global framebuffer object
extern Framebuffer *framebuffer;

// There is no Framebuffer__init function because
// framebuffer is global struct which is provided
// by bootloader
void Framebuffer__set_pixel(uint16_t x, uint16_t y, uint64_t color);
void Framebuffer__set_all_pixels(uint64_t color);
void Framebuffer__print_glyph(Glyph *glyph);