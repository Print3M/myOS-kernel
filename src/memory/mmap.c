#include "mmap.h"
#include "../bootloader.h"
#include "../stdint.h"

EfiMemoryDescriptor *
get_memory_descriptor(EfiMemoryDescriptor *memory_map, uint64_t n, size_t descriptor_sz) {
	return (EfiMemoryDescriptor *) ((uint8_t *) memory_map + (n * descriptor_sz));
}

uint64_t get_number_of_page_frames(MemoryData *memory) {
	// Calculate number of all available page frames by memory map.
	uint64_t result = 0;

	for (uint32_t i = 0; i < memory->entries; i++) {
		EfiMemoryDescriptor *desc =
			get_memory_descriptor(memory->memory_map, i, memory->descriptor_sz);

		result += desc->number_of_pages;
	}

	return result;
}