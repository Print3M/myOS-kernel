#pragma once
#include <libc/stdbool.h>
#include <libc/stdint.h>
#include <memory/mmap.h>

#define FRAME_SZ 4096 // In bytes

// States of page frame
typedef uint8_t frame_state;
#define FREE_FRAME	   0x00
#define LOCKED_FRAME   0x01
#define RESERVED_FRAME 0x02

typedef struct {
	void *frame_addr;	   // Start address of page frame
	frame_state state;     // State of page frame
} PageFrame;

typedef struct {
	size_t free_frames;		// Number of free frames
	size_t reserved_frames; // Number of reserved frames
	size_t locked_frames;		// Number of frames in use
	size_t frames;			// Number of all frames
	PageFrame *array;		// Array of frames
	bool initialized;		// Is physical memory initialized?
} PhysicalMemory;

PhysicalMemory *init_physical_memory(void);

// Page frame allocation functions
void PhysicalMemory__init(void *page_frames_array_buffer);
size_t PhysicalMemory__get_frame_index_by_ptr(void *mem_ptr);
size_t PhysicalMemory__get_frames_array_sz();
frame_state PhysicalMemory__get_frame_state(size_t index);
void PhysicalMemory__lock_frame(size_t index);
void PhysicalMemory__free_frame(size_t index);
void PhysicalMemory__reserve_frame(size_t index);
void PhysicalMemory__unreserve_frame(size_t index);

// Utils
void *pmem_request_frame(void);