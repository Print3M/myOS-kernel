#include "gdt.h"

/*
	Segmentation is still required in x86-64 but we use that as many
	other kernels - we are making flat model using four segments. Each
	segment holds entire address space.

	To better understanding why whe are using segmentation
	to make just flat-model:
		https://wiki.osdev.org/Segmentation#Notes_Regarding_C

*/

GDT global_descriptor_table = {
	.null = {.limit0 = 0x0,
			 .base0 = 0x0,
			 .base1 = 0x0,
			 .flags1 = 0x0,
			 .limit1_flags2 = 0x0,
			 .base2 = 0x0},
	.kernel_code = {.limit0 = 0x0,
					.base0 = 0x0,
					.base1 = 0x0,
					.flags1 = 0x9a,
					.limit1_flags2 = 0xa0,
					.base2 = 0x0},
	.kernel_data = {.limit0 = 0x0,
					.base0 = 0x0,
					.base1 = 0x0,
					.flags1 = 0x92,			// 10010010
					.limit1_flags2 = 0xa0,  // 10100000
					.base2 = 0x0},
	.user_code = {.limit0 = 0x0,
				  .base0 = 0x0,
				  .base1 = 0x0,
				  .flags1 = 0x9a,
				  .limit1_flags2 = 0xa0,
				  .base2 = 0x0},
	.user_data = {.limit0 = 0x0,
				  .base0 = 0x0,
				  .base1 = 0x0,
				  .flags1 = 0x92,
				  .limit1_flags2 = 0xa0,
				  .base2 = 0x0},
};

GDT_Descriptor gdt_descriptor = {.size = sizeof(GDT) - 1,
								 .offset = (uint64_t) &global_descriptor_table};

void init_gdt(void) { load_gdt(&gdt_descriptor); }