#include "bootloader.h"
#include "console/console.h"
#include "cpu/cpu.h"
#include "framebuffer/framebuffer.h"
#include "gdt/gdt.h"
#include "kutils.h"
#include "memory/mmap.h"
#include "memory/paging.h"
#include "memory/pmem.h"
#include "stdint.h"
#include "stdlib.h"

uint16_t dupa = 0xff;

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

	init_paging(bootloader_data->framebuffer);
	kprint_succ("4-level paging initialization success");

	// Check paging
	uint8_t *a = 0x9000000000;
	map_vaddr_to_paddr((void *) a, (void *) 0x1000);
	*a = 69;
	*a = 69;

	printf("\nValue at address 0x9000000000 = %d \n", *a);


	// kprint_succ("initialized succesfully \n");

	// kprint_pmem_info();
	// printf("Page entries=%d \n", page_entries);
	// printf("PD entries=%d \n", pdpt_entries);
	// printf("PDPT entries=%d \n", pdpt_entries);
	// printf("PML4 entries=%d \n", pml4_entries);

	// Test paging
	// void *frame = pmem_request_frame();
	// map_vaddr_to_paddr((void *) 0x8000000000, frame);
	// uint64_t *test = (uint64_t) 0x8000000000;
	// *test = 696969;
	// printf("Test=%d\n", *test);

	for (;;)
		;
}