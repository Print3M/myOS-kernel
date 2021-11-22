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
#define FREE_FRAME	   0x00
#define LOCKED_FRAME   0x01
#define RESERVED_FRAME 0x02

typedef struct {
	void *frame_addr;  // Start address of page frame
	frame_state state; // State of page frame
} PageFrame;

typedef struct {
	size_t free_frames;		// Number of free frames
	size_t reserved_frames; // Number of reserved frames
	size_t used_frames;		// Number of frames in use
	size_t frames;			// Number of all frames
	PageFrame *array;		// Array of frames
	bool initialized;		// Is physical memory initialized?
} PhysicalMemory;

// Global "object" with physical_memory data
extern PhysicalMemory physical_memory;

pmem_status init_physical_memory(MemoryData *memory, void *stack_addr);

// Page frame allocation functions
void PhysicalMemory__init(MemoryData *memory, void *page_frames_array_buffer, void *stack_addr);
size_t PhysicalMemory__get_frame_index_by_ptr(void *mem_ptr);
size_t PhysicalMemory__get_frames_array_sz();
frame_state PhysicalMemory__get_frame_state(size_t index);
void PhysicalMemory__lock_frame(size_t index);
void PhysicalMemory__free_frame(size_t index);
void PhysicalMemory__reserve_frame(size_t index);
void PhysicalMemory__unreserve_frame(size_t index);

// Utils
void *pmem_request_frame(void);