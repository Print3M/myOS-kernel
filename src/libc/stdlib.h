#pragma once
#include <libc/stdint.h>

// In/out functions
void printf(char *fmt, ...);

// String operations
size_t str_len(char *str);
char *str_reverse(char *str, int len);

// Conversions
char *itoa(uint64_t num, char *buf, uint8_t base);

// Memory operations
void *memset(void *ptr, uint8_t v, size_t n);
void *memcpy(void *dest, void *src, size_t n);