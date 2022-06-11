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
#include <libc/stdbool.h>
#include <libc/stdint.h>

// General HPET registers offset
// See: Table 2 Memory-Mapped Registers
#define HPET_CAP_REG		0x000
#define HPET_CONFIG_REG		0x010
#define HPET_INT_STATUS_REG 0x020
#define HPET_COUNTER_REG	0x0f0

// See: Figure 3 General Capability and ID Register
typedef struct __attribute__((packed)) {
	uint8_t rev_id;
	uint8_t num_tim_cap : 5;
	uint8_t count_size_cap : 1;
	uint8_t reserved1 : 1;
	uint8_t leg_route_cap : 1;
	uint16_t vendor_id;
	uint32_t counter_clk_period;
} HPET_CapReg;

// See: Figure 4 General Configuration Register
typedef struct __attribute__((packed)) {
	uint8_t enable_cnf : 1; // Global enable timer flag
	uint8_t leg_rt_cnf : 1;
	uint64_t reserved1 : 62;
} HPET_ConfigReg;

// See: Table 3. HPET Description Table
typedef struct __attribute__((packed)) {
	ACPI_SDT_Header header;
	uint32_t event_timer_block_id;
	ACPI_GAS base_address;
	uint8_t hpet_number;
	uint16_t min_clock_tick_periodic_mode;
	uint8_t page_protection;
} HPET_DescTable;

volatile void *HPET_get_reg_ptr(uint64_t reg);
void HPET_write_reg(uint64_t reg, uint64_t val);
uint64_t HPET_read_reg(uint64_t reg);
HPET_DescTable *HPET_get_desc_table(void);
HPET_DescTable *HPET_init(void);
bool HPET_is_counter_enabled(void);
void HPET_enable_counter(void);
void HPET_disable_counter(void);
uint64_t HPET_get_main_counter(void);
uint8_t HPET_get_no_timers(void);
uint64_t HPET_get_mmio_sz(void);