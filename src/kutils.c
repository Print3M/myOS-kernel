#include <cpu/cpu.h>
#include <framebuffer/colors.h>
#include <framebuffer/framebuffer.h>
#include <kernel.h>
#include <libc/stdlib.h>
#include <memory/pmem.h>

void kprint_err(const char *msg) { printf("[!] Kernel error: %s \n", msg); }

void kprint_succ(const char *msg) { printf("[+] Kernel success: %s \n", msg); }

void kprint_info(const char *msg) { printf("[*] Kernel info: %s \n", msg); }

void kprint_pmem_info(void) {
	kprint_info("physical memory info:");
	printf("\t free: frames = %d, memory = %dMB \n",
		   kernel.pmem->free_frames,
		   kernel.pmem->free_frames * FRAME_SZ / 1024 / 1024);
	printf("\t locked: frames = %d, memory = %dMB \n",
		   kernel.pmem->locked_frames,
		   kernel.pmem->locked_frames * FRAME_SZ / 1024 / 1024);
	printf("\t reserved: frames = %d, memory = %dMB \n",
		   kernel.pmem->reserved_frames,
		   kernel.pmem->reserved_frames * FRAME_SZ / 1024 / 1024);
	printf("\t size of array: %dMB \n", PhysicalMemory__get_frames_array_sz() / 1024 / 1024);
}

void kprint_cpu_info(void) {
	kprint_info("CPU info:");
	printf("\t - manufacturer:   %s \n", kernel.cpu->manufacturer);
	printf("\t - stepping id:    %d \n", kernel.cpu->stepping_id);
	printf("\t - family id:      %d \n", kernel.cpu->family_id);
	printf("\t - model id:       %d \n", kernel.cpu->model_id);
	printf("\t - type:           %d \n", kernel.cpu->type);
	printf("\t - brand:          %s \n", kernel.cpu->brand);
	printf("\t - logical cores:  %d \n", kernel.cpu->logical_cores);
	printf("\t - physical cores: %d \n", kernel.cpu->physical_cores);
	printf("\t - hyperthreading: %d \n", kernel.cpu->hyperthreading);
}

void kpanic(const char *msg) {
	Framebuffer__set_all_pixels(BLUE);
	ConsoleCursor__gotoxy(kernel.console->cursor, 0, 0);
	ConsoleCursor__set_bg_color(kernel.console->cursor, BLUE);
	ConsoleCursor__set_fg_color(kernel.console->cursor, WHITE);

	printf("Kernel panic!!! \n");
	printf("Error: %s", msg);
}