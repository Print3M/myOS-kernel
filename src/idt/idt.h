/*
	About interrupts and exceptions in general read (Intel Manual vol. 3):
		6. Interrupt and exception handling

*/
#pragma once
#include <libc/stdint.h>

#define ALL_IDT_ENTRIES 256

// Table 3-2 - System-Segment and Gate-Descriptor Types
#define CALL_GATE	   12
#define INTERRUPT_GATE 14
#define TRAP_GATE	   15

// Vector (entries) indexes in IDT
#define DIVIDE_BY_ZERO_INDEX 0
#define PAGE_FAULT_INDEX	 14

typedef struct __attribute__((packed)) {
	// Figure 6-8 - 64-Bit IDT Gate Descriptor
	uint16_t offset1;
	uint16_t segment_selector;
	uint8_t ist : 3;
	uint8_t ignore1 : 5;
	uint8_t gate_type : 4;
	uint8_t ignore2 : 1;
	uint8_t dpl : 2;
	uint8_t present : 1;
	uint16_t offset2;
	uint32_t offset3;
	uint32_t reserved1;
} IDT_DescriptionEntry;

typedef struct __attribute__((packed)) {
	// Figure 6-1 - Relationship of the IDTR and IDT
	uint16_t limit;
	uint64_t base_addr;
} IDTR;

void init_idt(void);