#pragma once
#include <libc/stdbool.h>
#include <libc/stdint.h>

typedef struct __attribute__((packed)) {
	size_t size;
	uint8_t *buffer;
} Bitmap;

Bitmap *Bitmap_init(size_t no_fields, void *buffer, size_t buff_size);
bool Bitmap_get(Bitmap *self, uint64_t index);
void Bitmap_set(Bitmap *self, uint64_t index, bool value);