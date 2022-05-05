#include "bitmap.h"
#include <libc/stdbool.h>
#include <libc/stdint.h>

void __get_bitmap_offset(uint64_t index, uint64_t *byte_offset, uint8_t *bit_offset) {
	// Params:
	//  IN  index
	//  OUT byte_offset
	//  OUT bit_offset
	*byte_offset = index / 8;
	*bit_offset = index % 8;
}

void Bitmap__zero(Bitmap *self) {
	for (size_t i = 0; i < self->size; i++) {
		self->buffer[i] = 0b00000000;
	}
}

bool Bitmap__get(Bitmap *self, uint64_t index) {
	bool result;
	uint64_t byte_offset;
	uint8_t bit_offset;
	__get_bitmap_offset(index, &byte_offset, &bit_offset);

	// Get result byte
	result = self->buffer[byte_offset];

	// Get result bit
	if ((result << bit_offset) & 0b10000000) {
		return true;
	} else {
		return false;
	};
}

void Bitmap__set(Bitmap *self, uint64_t index, bool value) {
	uint64_t byte_offset;
	uint8_t bit_offset;
	__get_bitmap_offset(index, &byte_offset, &bit_offset);
	uint8_t bit_pattern = 0b10000000 >> bit_offset;

	// Set bit
	self->buffer[byte_offset] &= ~bit_pattern;
	if (value) {
		self->buffer[byte_offset] |= bit_pattern;
	}
}

Bitmap Bitmap__init(size_t size, void *buffer) {
	// Initialize bitmap struct and zero entire bitmap

	Bitmap bitmap = {.size = size, .buffer = (uint8_t *) buffer};
	Bitmap__zero(&bitmap);

	return bitmap;
}