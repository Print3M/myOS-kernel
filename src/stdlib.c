#include "console/console.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdint.h"

void printf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	Console__printf(&console, fmt, args);
	va_end(args);
}

size_t str_len(char *str) {
	// Returns string length without '\0' char
	size_t count = 0;

	while (str[count] != '\0') {
		count++;
	}

	return count;
}

char *str_reverse(char *str, int len) {
	char temp;

	for (int i = 0; i < (len / 2); i++) {
		// Switch two chars
		temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}

	return str;
}

void *memcpy(void *dest, void *src, size_t n) {
	uint8_t *d = (uint8_t *) dest;
	uint8_t *s = (uint8_t *) src;

	while (n--) {
		*d++ = *s++;
	}

	return dest;
}

void *memset(void *ptr, uint8_t v, size_t n) {
	uint8_t *p = (uint8_t *) ptr;

	while (n--) {
		*p++ = v;
	}

	return ptr;
}

char *itoa(uint64_t num, char *buf, uint8_t base) {
	// This simple implementation has been copied from:
	//      https://www.geeksforgeeks.org/implement-itoa/
	//
	// How it works:
	// 		123 => "123\0"

	int i = 0;

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) {
		buf[i++] = '0';
		buf[i] = '\0';
		return buf;
	}

	// Process individual digits
	while (num != 0) {
		int rem = num % base;
		buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	buf[i] = '\0'; // Append string terminator

	// Reverse the string
	buf = str_reverse(buf, i);

	return buf;
}