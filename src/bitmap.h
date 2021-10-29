#pragma once
#include "stdint.h"
#include "stdbool.h"

typedef struct {
    size_t size;
    uint8_t *buffer;
} Bitmap;

Bitmap Bitmap__init(size_t size, void *buffer);
void Bitmap__zero(Bitmap *self);
bool Bitmap__get(Bitmap *self, uint64_t index);
void Bitmap__set(Bitmap *self, uint64_t index, bool value);