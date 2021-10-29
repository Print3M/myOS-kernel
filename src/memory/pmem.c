#include "pmem.h"
#include "../stdbool.h"
#include "../stdlib.h"
#include "mmap.h"

const frame_state FREE_FRAME = 0x00;
const frame_state LOCKED_FRAME = 0x01;
const frame_state RESERVED_FRAME = 0x02;

PhysicalMemory physical_memory = {.initialized = false};

void __get_largest_free_mem_seg(MemoryData *memory, size_t *size, void **mem_seg) {
	// IN memory
	// OUT size - size of largest free memory segment
	// OUT mem_seg - address to largest free mem seg
	*size = 0;

	for (uint64_t i = 0; i < memory->entries; i++) {
		EfiMemoryDescriptor *desc =
			get_memory_descriptor(memory->memory_map, i, memory->descriptor_sz);

		if (desc->type == EFI_CONVENTIONAL_MEMORY) {
			size_t seg_sz = desc->number_of_pages * PAGE_SZ;

			if (seg_sz > *size) {
				*size = seg_sz;
				*mem_seg = desc->physical_addr;
			}
		}
	}
}

pmem_status init_physical_memory(MemoryData *memory, void *stack_addr) {
	void *largest_free_mem_seg = NULL;
	size_t largest_free_mem_seg_sz = 0;
	__get_largest_free_mem_seg(memory, &largest_free_mem_seg_sz, &largest_free_mem_seg);
	PhysicalMemory__init(memory, largest_free_mem_seg, stack_addr);

	return PMEM_INIT_SUCCESS;
}

void PhysicalMemory__set_frame_state(size_t index, frame_state state) {
	physical_memory.array[index].state = state;
}

frame_state PhysicalMemory__get_frame_state(size_t index) {
	return physical_memory.array[index].state;
}

int64_t PhysicalMemory__get_frame_index_by_ptr(void *mem_ptr) {
	/*
		Return index of page frame raleted with address passed in :mem_ptr.
		If doesn't found, return -1.
	*/
	size_t ptr = (size_t) mem_ptr;

	for (size_t i = 0; i < physical_memory.frames; i++) {
		PageFrame *page = physical_memory.array + i;
		size_t p1 = (size_t) page->frame_addr;
		size_t p2 = p1 + PAGE_SZ;

		// Check if ptr is between range (n <= ptr < n + 1)
		if (p1 <= ptr && p2 > ptr) {
			return i;
		}
	}
	return -1;
}

size_t PhysicalMemory__get_frames_array_sz() {
	// Amount of frames * byte * size of PageFrame structure
	return physical_memory.frames * 8 * sizeof(PageFrame); // In bytes
}

void PhysicalMemory__init(MemoryData *memory, void *page_frames_array_buffer, void *stack_addr) {
	if (physical_memory.initialized) {
		// Do not reinitalize
		return;
	}

	// Initialize - all frames set to free
	physical_memory.array = (PageFrame *) page_frames_array_buffer;
	physical_memory.free_memory = get_used_memory_size(memory);
	physical_memory.frames = physical_memory.free_memory / FRAME_SZ;
	physical_memory.free_frames = physical_memory.frames;
	physical_memory.used_frames = 0;
	physical_memory.used_memory = 0;
	physical_memory.reserved_frames = 0;
	physical_memory.reserved_memory = 0;

	// Set all frames in array to free
	for (uint64_t i = 0; i < physical_memory.frames; i++) {
		PhysicalMemory__set_frame_state(i, FREE_FRAME);
	}

	// Initialize frames by memory map recived from bootloader
	uint64_t frame_id = 0;
	uint64_t array_frame_id = 0;
	for (uint64_t i = 0; i < memory->entries; i++) {
		EfiMemoryDescriptor *desc =
			get_memory_descriptor(memory->memory_map, i, memory->descriptor_sz);

		// Get base address of memory map region
		uint64_t bs_addr = (uint64_t) desc->physical_addr;

		// Get start id of frame where frames' array is placed
		if (bs_addr == (uint64_t) page_frames_array_buffer) {
			array_frame_id = frame_id;
		}

		// Iterate over all pages in :desc memory region
		for (uint64_t n = 0; n < desc->number_of_pages; n++) {
			// Calculate frame address
			physical_memory.array[frame_id].frame_addr = (void *) (bs_addr + (n * FRAME_SZ));

			// Reserve frames of regions which are not supposed
			// to be used by kernel
			if (desc->type != EFI_CONVENTIONAL_MEMORY && desc->type != EFI_BOOT_SERVICES_CODE &&
				desc->type != EFI_BOOT_SERVICES_DATA) {
				PhysicalMemory__reserve_frame(frame_id);
			}
			frame_id++;
		}
	}

	// Lock frame with stack itself
	size_t stack_frame = PhysicalMemory__get_frame_index_by_ptr(stack_addr);
	PhysicalMemory__reserve_frame(stack_frame);

	// Lock frames where the frames' array is placed
	for (size_t i = 0; i < PhysicalMemory__get_frames_array_sz() / FRAME_SZ; i++) {
		PhysicalMemory__lock_frame(array_frame_id + i);
	}

	physical_memory.initialized = true;
}

void PhysicalMemory__lock_frame(size_t index) {
	frame_state state = PhysicalMemory__get_frame_state(index);

	if (state == FREE_FRAME) {
		physical_memory.free_frames -= 1;
		physical_memory.free_memory -= FRAME_SZ;
		physical_memory.used_frames += 1;
		physical_memory.used_memory += FRAME_SZ;
		PhysicalMemory__set_frame_state(index, LOCKED_FRAME);
	}
}

void PhysicalMemory__free_frame(size_t index) {
	frame_state state = PhysicalMemory__get_frame_state(index);

	if (state == LOCKED_FRAME) {
		physical_memory.free_frames += 1;
		physical_memory.free_memory += FRAME_SZ;
		physical_memory.used_frames -= 1;
		physical_memory.used_memory -= FRAME_SZ;
		PhysicalMemory__set_frame_state(index, FREE_FRAME);
	}
}

void PhysicalMemory__reserve_frame(size_t index) {
	frame_state state = PhysicalMemory__get_frame_state(index);

	if (state == FREE_FRAME) {
		physical_memory.free_frames -= 1;
		physical_memory.free_memory -= FRAME_SZ;
		physical_memory.reserved_frames += 1;
		physical_memory.reserved_memory += FRAME_SZ;
		PhysicalMemory__set_frame_state(index, RESERVED_FRAME);
	}
}

void PhysicalMemory__unreserve_frame(size_t index) {
	frame_state state = PhysicalMemory__get_frame_state(index);

	if (state == RESERVED_FRAME) {
		physical_memory.free_frames += 1;
		physical_memory.free_memory += FRAME_SZ;
		physical_memory.reserved_frames -= 1;
		physical_memory.reserved_memory -= FRAME_SZ;
		PhysicalMemory__set_frame_state(index, FREE_FRAME);
	}
}