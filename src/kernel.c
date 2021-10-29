#include "bootloader.h"
#include "console/console.h"
#include "cpu/cpu.h"
#include "framebuffer/framebuffer.h"
#include "memory/mmap.h"
#include "memory/pmem.h"
#include "stdint.h"
#include "stdlib.h"
#include "kutils.h"

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

	kprint_succ("initialized succesfully \n");

	for (;;)
		;
}