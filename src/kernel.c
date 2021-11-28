#include "bootloader.h"
#include "console/console.h"
#include "cpu/cpu.h"
#include "framebuffer/framebuffer.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "kutils.h"
#include "memory/mmap.h"
#include "memory/paging.h"
#include "memory/pmem.h"
#include "stdint.h"
#include "stdlib.h"

void _start(BootloaderData *bootloader_data) {
	ConsoleCursor cursor = {
		.line = 0,
		.column = 0,
		.max_lines = CURSOR_MAX_LINES,
		.max_columns = CURSOR_MAX_COLUMNS,
	};

	Console__init(bootloader_data->font, &cursor, bootloader_data->framebuffer);
	Console__clear_screen(&console);

	cpu_status cpu_s = check_cpu_state();
	if (cpu_s != CPU_SUCCESS) {
		kprint_err("CPU state error");
	} else {
		kprint_succ("CPU state correct");
	}

	pmem_status pmem_s = init_physical_memory(bootloader_data->memory, bootloader_data);
	if (pmem_s != PMEM_INIT_SUCCESS) {
		kprint_err("Physical memory initialization error");
	} else {
		kprint_succ("Physical memory initalization success");
		kprint_pmem_info();
	}

	init_gdt();
	kprint_succ("GDT initalization success");

	paging_status paging_s = init_paging(bootloader_data->framebuffer);

	if (paging_s == PAGING_INIT_SUCCESS) {
		kprint_succ("4-level paging initialization success");
	} else {
		kprint_err("4-level paging initialization error");
	}

	init_idt();
	kprint_succ("IDT initialization success");

	// Page fault
	__asm__ volatile("int $0x0e");
	// int* a = 0x8000000000000;
	// *a = 10;

	for (;;)
		;
}