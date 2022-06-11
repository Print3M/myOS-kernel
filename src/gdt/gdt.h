/*
	Interesting overview of GTD and segmentation (memory protection at all)
	is included in 'Intel's x86 Manual vol. 3A':
		3.1 MEMORY MANAGEMENT OVERVIEW
	Overview of different memory models utilising segmentation:
		3.2 USING SEGMENTS
	Usage of segmentation on x86-64 (IA-32e):
		3.2.4 Segmentation in IA-32e Mode
	About segment registers on x86-64:
		3.4.4 Segment Loading Instructions in IA-32e Mode
	About segment descriptors:
		3.4.5 Segment Descriptors
	About GDT (Segment Descriptors Table):
		3.5.1 Segment Descriptor Tables
*/
#pragma once
#include <libc/stdint.h>

#define KERNEL_CODE_SEGMENT_SELECTOR 8
#define KERNEL_DATA_SEGMENT_SELECTOR 16
#define USER_CODE_SEGMENT_SELECTOR	 24
#define USER_DATA_SEGMENT_SELECTOR	 32

typedef struct __attribute__((packed)) {
	uint16_t size;
	uint64_t offset;
} GDTR;

typedef struct __attribute__((packed)) {
	uint16_t limit0;
	uint16_t base0;
	uint8_t base1;
	uint8_t flags1;
	uint8_t limit1_flags2;
	uint8_t base2;
} GDT_SegmentDescriptor;

// Minimal implementation of Protected Flat Model
// 	3.2.2. Protected Flat Model
// 	"Similar design is used by several popular multitasking OS'es."
typedef struct __attribute__((packed, aligned(0x1000))) {
	GDT_SegmentDescriptor null;
	GDT_SegmentDescriptor kernel_code;
	GDT_SegmentDescriptor kernel_data;
	GDT_SegmentDescriptor user_code;
	GDT_SegmentDescriptor user_data;
} GDT;
// https://stackoverflow.com/questions/11770451/what-is-the-meaning-of-attribute-packed-aligned4
// __attribute__((packed)) -> use the smallest possible space for structu (avoid padding)
// __attribute__((aligned(0x1000))) ->  its start address can be divided by 0x1000

GDT *init_gdt(void);
extern void load_gdt(GDTR *gdtr_ptr);