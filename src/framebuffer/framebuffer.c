#include "framebuffer.h"
#include "../stdint.h"

void Framebuffer__set_pixel(Framebuffer *self, uint16_t x, uint16_t y, uint64_t color) {
	// Write :color into calculated :framebuffer address
	// address = framebuffer's base address + y offset (from base adress) + x offset (from y)
	uint64_t *pixel =
		((uint8_t *) self->base_address + (y * self->pixels_per_scanline * self->bytes_per_pixel) +
		 (x * self->bytes_per_pixel));

	*pixel = color;

}

void Framebuffer__clear_screen(Framebuffer *self) {
	// Set entire framebuffer to black pixels (clear screen simulation)
	for (uint16_t x = 0; x < self->width; x++) {
		for (uint16_t y = 0; y < self->height; y++) {
			Framebuffer__set_pixel(self, x, y, 0x0);
		}
	}
}

void Framebuffer__print_glyph(Framebuffer *self, Glyph *glyph) {
	// Iterate through glyph pixels in glyph bitmap
	for (uint16_t y = 0; y < glyph->height; y++) {
		for (uint16_t x = 0; x < glyph->width; x++) {
			// Print bitmap pixels
			uint64_t color;
			if ((*glyph->bitmap & (0b10000000 >> x)) > 0) {
				// Glyph color
				color = glyph->fg_color;
			} else {
				// Background color
				color = glyph->bg_color;
			}
			Framebuffer__set_pixel(self, x + glyph->x_off, y + glyph->y_off, color);
		}
		glyph->bitmap++;
	}
}
