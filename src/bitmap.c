#include <bitmap.h>
#include <libc/stdbool.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>

void _get_bitmap_offset(uint64_t index, uint64_t *byte_offset, uint8_t *bit_offset) {
	// Params:
	//  IN  index
	//  OUT byte_offset
	//  OUT bit_offset
	*byte_offset = index / 8;
	*bit_offset = index % 8;
}

bool Bitmap_get(Bitmap *self, uint64_t index) {
	bool result = 0;
	uint64_t byte_offset = 0;
	uint8_t bit_offset = 0;
	_get_bitmap_offset(index, &byte_offset, &bit_offset);

	// Get result byte
	result = self->buffer[byte_offset];

	// Get result bit
	return (result << bit_offset) & (1 << 7) ? true : false;
}

void Bitmap_set(Bitmap *self, uint64_t index, bool value) {
	uint64_t byte_offset = 0;
	uint8_t bit_offset = 0;
	_get_bitmap_offset(index, &byte_offset, &bit_offset);
	uint8_t bit_pattern = (1 << 7) >> bit_offset;

	// Set bit
	self->buffer[byte_offset] &= ~bit_pattern;
	if (value) {
		self->buffer[byte_offset] |= bit_pattern;
	}
}

Bitmap *Bitmap_init(size_t no_fields, void *buffer, size_t buff_size) {
	// :fields 		- number of bits (fields) in bitmap
	// :buffer 		- allocated buffer for bitmap
	// :buff_size:  - buffer size in bytes

	// Bitmap too big for buffer (minus .size field)
	if (no_fields > (buff_size - sizeof(size_t)) * 8) {
		return NULL;
	}

	memset(buffer, 0x0, buff_size);

	Bitmap *bitmap = (Bitmap *) buffer;
	bitmap->size = no_fields;
	bitmap->buffer = (uint8_t *) buffer;

	return bitmap;
}