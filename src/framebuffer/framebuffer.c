#include <framebuffer/framebuffer.h>
#include <libc/stdint.h>

// TODO: What is something use global framebuffer before initailization
Framebuffer *framebuffer = NULL;

void Framebuffer__set_pixel(uint16_t x, uint16_t y, uint64_t color) {
	// Write :color into calculated :framebuffer address
	// address = framebuffer's base address + y offset (from base adress) + x offset (from y)
	uint64_t *pixel =
		((uint8_t *) framebuffer->base_address + (y * framebuffer->pixels_per_scanline * framebuffer->bytes_per_pixel) +
		 (x * framebuffer->bytes_per_pixel));

	*pixel = color;
}

void Framebuffer__set_all_pixels(uint64_t color) {
	// Set solid color for entire framebuffer
	for (uint16_t x = 0; x < framebuffer->width; x++) {
		for (uint16_t y = 0; y < framebuffer->height; y++) {
			Framebuffer__set_pixel(x, y, color);
		}
	}
}

void Framebuffer__print_glyph(Glyph *glyph) {
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
			Framebuffer__set_pixel(x + glyph->x_off, y + glyph->y_off, color);
		}
		glyph->bitmap++;
	}
}
