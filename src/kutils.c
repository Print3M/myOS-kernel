#include <memory/pmem.h>
#include <libc/stdlib.h>

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