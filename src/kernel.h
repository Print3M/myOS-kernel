#pragma once
#include <acpi/acpi.h>
#include <acpi/hpet/hpet.h>
#include <console/console.h>
#include <cpu/cpu.h>
#include <framebuffer/framebuffer.h>
#include <gdt/gdt.h>
#include <idt/idt.h>
#include <memory/mmap.h>
#include <memory/paging.h>
#include <memory/pmem.h>

typedef struct {
	Framebuffer *framebuffer;
	ConsoleFont *font;
	MemoryData *memory;
	void *acpi_rsdp;
} BootloaderData;

typedef struct {
	void *stack;
	Framebuffer *framebuffer;
	MemoryData *memory;
	ACPI_RSDP *acpi_rsdp;
	Console *console;
	HPET_DescTable *hpet_desc;
	PhysicalMemory *pmem;
	GDT *gdt;
	PML4_Entry *pml4;
	IDT_DescriptionEntry *idt;
	CPU *cpu;
} KernelData;

extern KernelData kernel;