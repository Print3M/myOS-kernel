#pragma once
#include "../bootloader.h"
#include "../stdbool.h"
#include "../stdint.h"
#include "mmap.h"

#define FRAME_SZ 4096 // In bytes

typedef uint16_t pmem_status;
#define PMEM_INIT_ERROR	  0x0000
#define PMEM_INIT_SUCCESS 0x0001

// States of page frame
typedef uint8_t frame_state;
extern const frame_state FREE_FRAME;
extern const frame_state LOCKED_FRAME;
extern const frame_state RESERVED_FRAME;

typedef struct {
	void *frame_addr;  // Starting address of page frame
	frame_state state; // State of page frame
} PageFrame;

typedef struct {
	size_t free_memory;		// Amount of memory
	size_t free_frames;		// Number of free frames
	size_t reserved_memory; // Amount of reserved memory (in bytes)
	size_t reserved_frames; // Number of reserved frames
	size_t used_memory;		// Amount of memory in use (in bytes)
	size_t used_frames;		// Number of frames in use
	size_t frames;			// Number of all frames
	PageFrame *array;		// Array of frames
	bool initialized;		// Is physical memory initialized?
} PhysicalMemory;

extern PhysicalMemory physical_memory;

pmem_status init_physical_memory(MemoryData *memory, void *stack_addr);

// Page frame allocation functions
void PhysicalMemory__init(MemoryData *memory, void *page_frames_array_buffer, void *stack_addr);
int64_t PhysicalMemory__get_frame_index_by_ptr(void *mem_ptr);
size_t PhysicalMemory__get_frames_array_sz();
frame_state PhysicalMemory__get_frame_state(size_t index);
void PhysicalMemory__lock_frame(size_t index);
void PhysicalMemory__free_frame(size_t index);
void PhysicalMemory__reserve_frame(size_t index);
void PhysicalMemory__unreserve_frame(size_t index);