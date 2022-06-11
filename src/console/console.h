#pragma once
#include <framebuffer/framebuffer.h>
#include <libc/stdarg.h>
#include <libc/stdbool.h>
#include <libc/stdint.h>

#define CURSOR_MAX_LINES   37
#define CURSOR_MAX_COLUMNS 100

typedef struct {
	uint16_t line;
	uint16_t column;
	uint16_t max_lines;
	uint16_t max_columns;
	uint32_t fg_color;
	uint32_t bg_color;
} ConsoleCursor;

typedef struct {
	uint8_t magic[2];
	uint8_t mode;
	uint8_t charsize;
} PsfHeader;

typedef struct {
	PsfHeader *psf_header;
	uint8_t glyph_height;
	uint8_t glyph_width;
	void *glyph_buffer;
} ConsoleFont;

typedef struct {
	ConsoleFont *font;
	ConsoleCursor *cursor;
	Framebuffer *framebuffer;
	bool initialized;
} Console;

void ConsoleCursor__new_line(ConsoleCursor *self);
void ConsoleCursor__next(ConsoleCursor *self);
void ConsoleCursor__back(ConsoleCursor *self);
void ConsoleCursor__reset(ConsoleCursor *self);
void ConsoleCursor__gotoxy(ConsoleCursor *self, uint16_t x, uint16_t y);
void ConsoleCursor__set_fg_color(ConsoleCursor *self, uint32_t color);
void ConsoleCursor__set_bg_color(ConsoleCursor *self, uint32_t color);

Console *Console__init(ConsoleFont *font);
void Console__clear_screen(Console *self);
void Console__print_char(Console *self, uint16_t chr);
void Console__print_str(Console *self, char *str);
void Console__print_int(Console *self, uint64_t num, uint8_t base);
void Console__printf(Console *self, char *str, va_list args);