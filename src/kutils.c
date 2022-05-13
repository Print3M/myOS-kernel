#include <cpu/cpu.h>
#include <framebuffer/colors.h>
#include <framebuffer/framebuffer.h>
#include <libc/stdlib.h>
#include <memory/pmem.h>

void kprint_err(const char *msg) { printf("[!] Kernel error: %s \n", msg); }

void kprint_succ(const char *msg) { printf("[+] Kernel success: %s \n", msg); }

void kprint_info(const char *msg) { printf("[*] Kernel info: %s \n", msg); }

void kprint_pmem_info(void) {
	if (physical_memory.initialized == false) {
		kprint_err("physical memory is not initialized yet");
		return;
	}

	kprint_info("physical memory info:");

	printf("\t free: frames = %d, memory = %dMB \n",
		   physical_memory.free_frames,
		   physical_memory.free_frames * FRAME_SZ / 1024 / 1024);
	printf("\t used: frames = %d, memory = %dMB \n",
		   physical_memory.used_frames,
		   physical_memory.used_frames * FRAME_SZ / 1024 / 1024);
	printf("\t reserved: frames = %d, memory = %dMB \n",
		   physical_memory.reserved_frames,
		   physical_memory.reserved_frames * FRAME_SZ / 1024 / 1024);
	printf("\t size of array: %dMB \n", PhysicalMemory__get_frames_array_sz() / 1024 / 1024);
}

void kprint_cpu_info(void) {
	kprint_info("CPU info:");
	printf("\t - manufacturer:   %s \n", cpu_model_info.manufacturer);
	printf("\t - stepping id:    %d \n", cpu_model_info.stepping_id);
	printf("\t - family id:      %d \n", cpu_model_info.family_id);
	printf("\t - model id:       %d \n", cpu_model_info.model_id);
	printf("\t - type:           %d \n", cpu_model_info.type);
	printf("\t - brand:          %s \n", cpu_model_info.brand);
	printf("\t - logical cores:  %d \n", cpu_info.logical_cores);
	printf("\t - physical cores: %d \n", cpu_info.physical_cores);
	printf("\t - hyperthreading: %d \n", cpu_info.hyperthreading);
}

void kpanic(const char *msg) {
	Framebuffer__set_all_pixels(BLUE);
	ConsoleCursor__gotoxy(console.cursor, 0, 0);
	ConsoleCursor__set_bg_color(console.cursor, BLUE);
	ConsoleCursor__set_fg_color(console.cursor, WHITE);

	printf("Kernel panic!!! \n");
	printf("Error: %s", msg);
}