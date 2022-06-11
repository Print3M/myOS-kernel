#include <acpi/hpet/hpet.h>
#include <acpi/hpet/timer.h>
#include <cpu/mmio.h>
#include <kernel.h>

volatile void *HTimer_get_timer_reg_ptr(uint8_t timer_n, uint64_t reg) {
	// Return pointer to the HPET n-timer block
	HTimer *timers = (HTimer *) (kernel.hpet_desc->base_address.address + TIMER_FIRST_OFF);
	uint64_t base_addr = (uint64_t)(timers + timer_n);
	return (volatile void *) (base_addr + reg);
}

void HTimer_write_reg(uint8_t timer_n, uint64_t reg, uint64_t val) {
	volatile void *ptr = HTimer_get_timer_reg_ptr(timer_n, reg);
	mmio_write64(ptr, val);
}

uint64_t HTimer_read_reg(uint8_t timer_n, uint64_t reg) {
	volatile void *ptr = HTimer_get_timer_reg_ptr(timer_n, reg);
	return mmio_read64(ptr);
}

void HTimer_set_config(uint8_t timer_n, HTimerConfigReg *reg) {
	// Set periodic or non-periodic type of HPET timer
	HTimer_write_reg(timer_n, TIMER_CONFIG_REG, *(uint64_t *) reg);
}

void HTimer_set_compar(uint8_t timer_n, uint64_t val) {
	// Read: 2.3.9.1 Register Definition and Usage Model
	HTimer_write_reg(timer_n, TIMER_COMPAR_REG, val);
}

uint64_t HTimer_get_compar(uint8_t timer_n) {
	// Get comparator of HPET timer
	return HTimer_read_reg(timer_n, TIMER_COMPAR_REG);
}

void HTimer_set_one_shot(uint8_t timer_n, uint64_t ms_time) {
	// Set timer to one-shot type. It fires interrupt only once.
	// IMPORTANT: This function assumes that HPET (main counter)
	// is already enabled.
	//
	// :timer_n  - number of timer
	// :ms_time  - amount of miliseconds till interrupt
	HTimerConfigReg config_reg = {
		.tn_int_enb_cnf = 1,
		.tn_type_cnf = TIMER_ONE_SHOT_TYPE,
		.tn_int_route_cnf = 0x0,
	};
	HTimer_set_config(timer_n, &config_reg);

	// Calculate ticks for comparator
	uint64_t ticks = ms_time * 1000000 / NANOSECONDS_PER_TICK;
	uint64_t counter = HPET_get_main_counter();
	HTimer_set_compar(timer_n, counter + ticks);
}
