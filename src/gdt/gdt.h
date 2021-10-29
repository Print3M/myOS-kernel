/*
	Interesting overview of GTD and segmentation (memory protection at all)
	is included in 'Intel's x86 Manual vol. 3A':
		3.1 MEMORY MANAGEMENT OVERVIEW
	Usage of segmentation on x86-64 (IA-32e), see:
		3.2.4 Segmentation in IA-32e Mode
	Setting up segment registers (not x86-64 related, still interesting), see:
		3.4.2 Segment Selectors
	About segment registers on x86-64, see:
		3.4.4 Segment Loading Instructions in IA-32e Mode
	About segment descriptors:
		3.4.5 Segment Descriptors
	About GDT (Segment Descriptors Table), see:
		3.5.1 Segment Descriptor Tables
*/
#pragma once
#include "../stdint.h"

struct GDT_Descriptor {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

struct AccessByte {
	unsigned pr : 1;
	unsigned privl : 2;
	unsigned s : 1;
	unsigned ex : 1;
	unsigned dc : 1;
	unsigned rw : 1;
	unsigned ac : 1;
} __attribute__((packed));

struct Flags {
	unsigned limit : 4;
	unsigned gr : 1;
	unsigned sz : 1;
	unsigned l : 1;
} __attribute__((packed));

struct GDT_SegmentDescriptor {
	uint16_t limit_0;
	uint16_t base_0;
	uint8_t base_1;
	AccessByte access_byte;
	Flags flags;
} __attribute__((packed));

struct GDT {
	GDT_SegmentDescriptor null;
	GDT_SegmentDescriptor kernel_code;
	GDT_SegmentDescriptor kernel_data;
	GDT_SegmentDescriptor user_null;
	GDT_SegmentDescriptor user_code;
	GDT_SegmentDescriptor user_data;
} __attribute__((packed)) __attribute__((aligned(0x1000)));