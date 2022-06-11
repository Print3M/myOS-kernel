#include <console/console.h>
#include <framebuffer/colors.h>
#include <framebuffer/framebuffer.h>
#include <kernel.h>
#include <libc/stdarg.h>
#include <libc/stdbool.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>

Console console = {.initialized = false};
ConsoleCursor cursor = {.line = 0,
						.column = 0,
						.max_lines = CURSOR_MAX_LINES,
						.max_columns = CURSOR_MAX_COLUMNS,
						.fg_color = WHITE,
						.bg_color = BLACK};

void ConsoleCursor__new_line(ConsoleCursor *self) {
	self->column = 0;
	self->line++;
}

void ConsoleCursor__next(ConsoleCursor *self) {
	if (self->column + 1 == self->max_columns) {
		ConsoleCursor__new_line(self);
	} else {
		self->column++;
	}
}

void ConsoleCursor__back(ConsoleCursor *self) {
	if (self->column == 0) {
		if (self->line > 0) {
			self->column = self->max_columns;
			self->line--;
		}
	} else {
		self->column--;
	}
}

void ConsoleCursor__reset(ConsoleCursor *self) {
	self->column = 0;
	self->line = 0;
}

void ConsoleCursor__gotoxy(ConsoleCursor *self, uint16_t x, uint16_t y) {
	self->column = x;
	self->line = y;
}

void ConsoleCursor__set_fg_color(ConsoleCursor *self, uint32_t color) { self->fg_color = color; }

void ConsoleCursor__set_bg_color(ConsoleCursor *self, uint32_t color) { self->bg_color = color; }

Console *Console__init(ConsoleFont *font) {
	if (console.initialized) {
		return &console;
	}

	console.cursor = &cursor;
	console.font = font;
	console.framebuffer = kernel.framebuffer;
	console.initialized = true;

	return &console;
}

void Console__print_char(Console *self, uint16_t chr) {
	switch (chr) {
		case '\n': {
			ConsoleCursor__new_line(self->cursor);
			break;
		}
		case '\t': {
			ConsoleCursor__next(self->cursor);
			ConsoleCursor__next(self->cursor);
			ConsoleCursor__next(self->cursor);
			break;
		}
		default: {
			// Get glyph bitmap ptr
			char *glyph_bitmap =
				(char *) self->font->glyph_buffer + (chr * self->font->psf_header->charsize);

			Glyph glyph = {.bitmap = glyph_bitmap,
						   .y_off = self->cursor->line * self->font->glyph_height,
						   .x_off = self->cursor->column * self->font->glyph_width,
						   .width = self->font->glyph_width,
						   .height = self->font->glyph_height,
						   .fg_color = self->cursor->fg_color,
						   .bg_color = self->cursor->bg_color};
			Framebuffer__print_glyph(&glyph);
			ConsoleCursor__next(self->cursor);
		}
	}
}

void Console__clear_screen(Console *self) {
	const uint8_t BLANK_CHAR = 0x20;

	ConsoleCursor__reset(self->cursor);
	for (uint32_t i = 0; i < self->cursor->max_lines * self->cursor->max_columns; i++) {
		Console__print_char(self, BLANK_CHAR);
	}
	ConsoleCursor__reset(self->cursor);
}

void Console__print_str(Console *self, char *str) {
	while (*str != '\0') {
		Console__print_char(self, *str);
		str++;
	}
}

void Console__print_int(Console *self, uint64_t num, uint8_t base) {
	char str[32];
	itoa(num, str, base);
	Console__print_str(self, str);
}

void Console__printf(Console *self, char *str, va_list args) {
	// Dynamic list of arguments
	char buf[4096]; // Buffer for result string
	memset(buf, 0, 4096);
	char temp_buf[64]; // 8*8 = 64 bits + 9th byte for '\0';
	memset(temp_buf, 0, 64);

	size_t len = str_len(str);
	size_t temp_len = 0;
	bool is_percent = false; // %x notation detector

	uint16_t j = 0;
	for (uint16_t i = 0; i < len; i++) {
		if (is_percent) {
			switch (str[i]) {
				case 's': {
					// If %s
					// Get next argument
					char *arg = va_arg(args, char *);
					temp_len = str_len(arg);

					// Add string to result buffer
					memcpy((void *) (buf + j), (void *) arg, temp_len);

					j += temp_len;
					break;
				}
				// case 'c': {
				// 	// If %c
				// 	uint8_t arg = va_arg(args, uint8_t);
				// 	buf[j] = arg;
				// 	j += 1;
				// 	break;
				// }
				case 'd': {
					// If %d
					uint64_t arg = va_arg(args, uint64_t);
					itoa(arg, temp_buf, 10);
					temp_len = str_len(temp_buf);
					memcpy((void *) (buf + j), (void *) temp_buf, temp_len);
					j += temp_len;
					break;
				}
				case 'p': {
					// If %p
					uint64_t arg = va_arg(args, uint64_t);
					itoa(arg, temp_buf, 16);
					temp_len = str_len(temp_buf);
					memcpy((void *) (buf + j), (void *) temp_buf, temp_len);
					j += temp_len;
					break;
				}
			}

			is_percent = false;
			continue;
		} else {
			if (str[i] == '%') {
				is_percent = true;
				continue;
			} else {
				// Add new char
				buf[j] = str[i];
				j++;
				continue;
			}
		}
	}

	buf[j] = '\0';

	Console__print_str(self, buf);
}