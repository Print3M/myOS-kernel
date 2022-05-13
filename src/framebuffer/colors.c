#include <libc/stdint.h>

uint64_t rgb_to_int(uint8_t red, uint8_t green, uint8_t blue) {
	uint64_t result = 0;
	result = blue;
	result += green << 8;
	result += red << 16;

	return result;
}

