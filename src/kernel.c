#include <acpi/acpi.h>
#include <acpi/apic/apic.h>
#include <acpi/hpet/hpet.h>
#include <acpi/hpet/timer.h>
#include <console/console.h>
#include <cpu/cpu.h>
#include <framebuffer/colors.h>
#include <framebuffer/framebuffer.h>
#include <gdt/gdt.h>
#include <idt/idt.h>
#include <kernel.h>
#include <kutils.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>
#include <memory/mmap.h>
#include <memory/paging.h>
#include <memory/pmem.h>

KernelData kernel;

void _start(BootloaderData *bd) {
	kernel.stack = (void *) bd;
	kernel.acpi_rsdp = (ACPI_RSDP *) bd->acpi_rsdp;
	kernel.framebuffer = bd->framebuffer;
	kernel.memory = bd->memory;

	kernel.console = Console__init(bd->font);
	Console__clear_screen(kernel.console);

	kernel.cpu = CPU_init();
	cpu_status cpu_s = check_cpu_state();
	if (cpu_s != CPU_SUCCESS) {
		kprint_err("CPU state error");
	} else {
		kprint_succ("CPU state correct");
	}
	kprint_cpu_info();

	kernel.pmem = init_physical_memory();
	kprint_succ("Physical memory initalization success");
	kprint_pmem_info();

	kernel.gdt = init_gdt();
	kprint_succ("GDT initalization success");

	kernel.hpet_desc = HPET_init();
	kprint_succ("HPET initialization success");

	kernel.pml4 = init_paging();
	kprint_succ("4-level paging initialization success");

	kernel.idt = init_idt();
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

	printf("LAPIC ID     : %d \n", APIC_get_local_id());
	printf("APIC version : %d \n", APIC_get_version());
	printf("HPET timers  : %d \n", HPET_get_no_timers());
	printf("HPET enabled : %d \n", HPET_is_counter_enabled());
	printf("HPET counter : %d \n", HPET_get_main_counter());
	printf("HPET counter : %d \n", HPET_get_main_counter());

	APIC_enable();
	HTimer_set_one_shot(0, 1000);
	printf("Comparator: %d \n", HTimer_get_compar(0));

	for (;;)
		;
}