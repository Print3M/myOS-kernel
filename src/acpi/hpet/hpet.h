/*
	HPET (High Precision Event Timer)

	Specification URL:
	https://www.intel.com/content/dam/www/public/us/en/documents/technical-specifications/software-developers-hpet-spec-1-0a.pdf

	HPET Architecture defines a set of timers that can be used by the operating
	system. The timers are implemented as a single up-counter with a set of
	comparators. The counter increases monotonically. When software does two
	consecutive reads of the counter, the second read will never return a value
	that is less than the first read. Each individual timer can generate an
	interrupt when the value in its match register equals the value of the
	free-running counter. Some of the timers can be enabled to generate a
	periodic interrupt.

	The registers associated with these timers are memory-mapped.
	The list of registers in specification: Table 2 Memory-Mapped Registers
*/
#pragma once
#include <acpi/acpi.h>
#include <libc/stdint.h>

// See: Table 3. HPET Description Table
typedef struct __attribute__((packed)) {
	ACPI_SDT_Header header;
	uint32_t event_timer_block_id;
	uint8_t base_address[12];
	uint8_t hpet_number;
	uint16_t min_clock_tick_periodic_mode;
	uint8_t page_protection;
} HPET_DescTable;

HPET_DescTable *get_hpet_desc_table(void);