#pragma once
#include <libc/stdint.h>
#include <console/console.h>
#include <framebuffer/framebuffer.h>
#include <memory/mmap.h>

typedef struct {
	Framebuffer *framebuffer;
	ConsoleFont *font;
	MemoryData *memory;
} BootloaderData;