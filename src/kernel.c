#include <acpi/acpi.h>
#include <acpi/apic/apic.h>
#include <acpi/hpet/hpet.h>
#include <bootloader.h>
#include <console/console.h>
#include <cpu/cpu.h>
#include <framebuffer/colors.h>
#include <framebuffer/framebuffer.h>
#include <gdt/gdt.h>
#include <idt/idt.h>
#include <kutils.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>
#include <memory/mmap.h>
#include <memory/paging.h>
#include <memory/pmem.h>

BootloaderData *bootloader_data;

void _start(BootloaderData *bd) {
	bootloader_data = bd;

	// Framebuffer must be provided as soon as possible
	framebuffer = bootloader_data->framebuffer;

	ConsoleCursor cursor = {.line = 0,
							.column = 0,
							.max_lines = CURSOR_MAX_LINES,
							.max_columns = CURSOR_MAX_COLUMNS,
							.fg_color = WHITE,
							.bg_color = BLACK};

	Console__init(bootloader_data->font, &cursor, bootloader_data->framebuffer);
	Console__clear_screen(&console);

	printf("Pointer: %p", bootloader_data->apic_rsdp);

	cpu_status cpu_s = check_cpu_state();
	if (cpu_s != CPU_SUCCESS) {
		kprint_err("CPU state error");
	} else {
		kprint_succ("CPU state correct");
	}
	get_cpu_info();
	kprint_cpu_info();

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

	acpi_version version = get_acpi_version();
	switch (version) {
		case ACPI_1_VERSION:
			kprint_succ("ACPI 1.0 detected");
			break;
		case ACPI_2_VERSION:
			kprint_succ("ACPI 2.0 detected");
			break;
		default:
			kprint_err("unknown version of ACPI detected");
			break;
	}

	HPET_DescTable *desc = get_hpet_desc_table();
	printf("HPET addr: %p", desc);
	printf("HPET number: %d", desc->hpet_number);
	printf("HPET time_block: %p", desc->event_timer_block_id);

	// printf("Pointer: %p", desc);
	// printf("Length: %d", desc->header.length);

	// int test = *((int *) 0x4000000000);

	// Page fault
	// __asm__ volatile("int $0x0");
	// int* a = 0x8000000000000;
	// *a = 10;

	for (;;)
		;
}