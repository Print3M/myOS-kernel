#include <acpi/acpi.h>
#include <acpi/hpet/hpet.h>
#include <cpu/mmio.h>
#include <kutils.h>
#include <libc/stdbool.h>
#include <libc/stdlib.h>

static HPET_DescTable *hpet_desc = NULL;

volatile void *HPET_get_reg_ptr(uint64_t reg) {
	// Get pointer to the specific HPET register
	return (volatile void *) ((uint64_t) hpet_desc->base_address.address + reg);
}

void HPET_write_reg(uint64_t reg, uint64_t val) {
	volatile void *ptr = HPET_get_reg_ptr(reg);
	mmio_write64(ptr, val);
}

uint64_t HPET_read_reg(uint64_t reg) {
	volatile void *ptr = HPET_get_reg_ptr(reg);
	return mmio_read64(ptr);
}

HPET_DescTable *HPET_get_desc_table(void) {
	// Return pointer to the HPET Description Table
	return (HPET_DescTable *) get_acpi_std_header(HPET_STD_SIGNATURE);
}

HPET_DescTable *HPET_init(void) {
	// IMPORTANT: This function should be runned as early as possible
	// Get HPET description table and enable main counter
	hpet_desc = HPET_get_desc_table();
	HPET_enable_counter();

	return hpet_desc;
}

bool HPET_is_counter_enabled(void) {
	uint64_t reg = HPET_read_reg(HPET_CONFIG_REG);
	HPET_ConfigReg *config = (HPET_ConfigReg *) &reg;

	return config->enable_cnf;
}

void HPET_enable_counter(void) {
	// Set `enable` bit in HPET config register
	uint64_t reg = HPET_read_reg(HPET_CONFIG_REG);
	HPET_ConfigReg *config = (HPET_ConfigReg *) &reg;
	config->enable_cnf = 1;
	HPET_write_reg(HPET_CONFIG_REG, reg);
}

void HPET_disable_counter(void) {
	// Disable `enable` bit in HPET config register
	uint64_t reg = HPET_read_reg(HPET_CONFIG_REG);
	HPET_ConfigReg *config = (HPET_ConfigReg *) &reg;
	config->enable_cnf = 0;
	HPET_write_reg(HPET_CONFIG_REG, reg);
}

uint64_t HPET_get_main_counter(void) {
	// Get value from HPET main counter
	return HPET_read_reg(HPET_COUNTER_REG);
}

uint8_t HPET_get_no_timers(void) {
	// Get number of HPET counters from Capabilities register
	uint64_t reg = HPET_read_reg(HPET_CAP_REG);
	HPET_CapReg *cap = (HPET_CapReg *) &reg;

	return cap->num_tim_cap + 1;
}

uint64_t HPET_get_mmio_sz(void) {
	// Get size of HPET entire MMIO space in bytes
	// Calculate it by adding up width of all HPET registers
	// See: Table 2 Memory-Mapped Registers
	uint32_t result = 8 * 8; // General registers = 8 registers * 8 bytes
	uint8_t no_timers = HPET_get_no_timers();

	// result + number_of_timers * bytes_per_timer
	return result + no_timers * (4 * 8);
}
