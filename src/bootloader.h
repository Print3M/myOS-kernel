#pragma once
#include <console/console.h>
#include <framebuffer/framebuffer.h>
#include <libc/stdint.h>
#include <memory/mmap.h>

typedef struct {
	Framebuffer *framebuffer;
	ConsoleFont *font;
	MemoryData *memory;
	void *apic_rsdp;
} BootloaderData;

extern BootloaderData *bootloader_data;
