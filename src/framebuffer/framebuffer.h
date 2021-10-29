#pragma once
#include "../stdint.h"

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
	uint64_t fg_color;
	uint64_t bg_color;
} Glyph;

// There is no Framebuffer__init function because
// framebuffer's structure is provided by bootloader
void Framebuffer__set_pixel(Framebuffer *self, uint16_t x, uint16_t y, uint64_t color);
void Framebuffer__clear_screen(Framebuffer *self);
void Framebuffer__print_glyph(Framebuffer *self, Glyph *glyph);