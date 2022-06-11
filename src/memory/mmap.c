#include <kernel.h>
#include <libc/stdint.h>
#include <memory/mmap.h>

EfiMemoryDescriptor *get_memory_descriptor(uint64_t n) {
	return (EfiMemoryDescriptor *) ((uint8_t *) kernel.memory->memory_map +
									(n * kernel.memory->descriptor_sz));
}

uint64_t get_number_of_page_frames(void) {
	// Calculate number of all available page frames by memory map.
	uint64_t result = 0;

	for (uint32_t i = 0; i < kernel.memory->entries; i++) {
		EfiMemoryDescriptor *desc = get_memory_descriptor(i);
		result += desc->number_of_pages;
	}

	return result;
}