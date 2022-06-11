#pragma once
#include <libc/stdint.h>

#define NANOSECONDS_PER_TICK 100

#define TIMER_BLOCK_SZ	4 * 8 // 4 * 64-bit registers
#define TIMER_FIRST_OFF 0x100

#define TIMER_PERIODIC_TYPE 0
#define TIMER_ONE_SHOT_TYPE 1

// Register offsets: Table 2. Memory-Mapped Registers
#define TIMER_CONFIG_REG 0x0
#define TIMER_COMPAR_REG 0x8
#define TIMER_FSB_REG	 0xf

// Read: 2.3.8 Timer N Configuration and Capabilities Register
typedef struct __attribute__((packed)) {
	uint8_t reserved1 : 1;
	uint8_t tn_int_type_cnf : 1;  // interrupts: 0=level triggered, 1=edge triggered
	uint8_t tn_int_enb_cnf : 1;	  // enable interrupts
	uint8_t tn_type_cnf : 1;	  // set periodic or one-shot type
	uint8_t tn_per_int_cap : 1;	  // is periodic available?
	uint8_t tn_size_cap : 1;	  // timer size, 0=32-bits, 1=64-bits
	uint8_t tn_val_set_cnf : 1;	  //
	uint8_t reserved2 : 1;		  //
	uint8_t tn_32mode_cnf : 1;	  //
	uint8_t tn_int_route_cnf : 5; // routing for interrupt to I/O APIC
	uint8_t tn_fsb_en_cnf : 1;
	uint8_t tn_fsb_int_del_cap : 1;
	uint16_t reserved3;
	uint32_t tn_int_route_cap;
} HTimerConfigReg;

// Read: 2.3.10 Timer N FSB Interrupt Route Register
typedef struct __attribute__((packed)) {
	uint32_t tn_fsb_int_val;
	uint32_t tn_fsb_int_addr;
} HTimerFsbReg;

typedef struct __attribute__((packed)) {
	HTimerConfigReg config_reg;
	uint64_t timer_reg;
	HTimerFsbReg fsb_reg;
	uint64_t reserved;
} HTimer;

volatile void *HTimer_get_timer_reg_ptr(uint8_t timer_n, uint64_t reg);
void HTimer_write_reg(uint8_t timer_n, uint64_t reg, uint64_t val);
uint64_t HTimer_read_reg(uint8_t timer_n, uint64_t reg);
void HTimer_set_config(uint8_t timer_n, HTimerConfigReg *reg);
void HTimer_set_compar(uint8_t timer_n, uint64_t val);
uint64_t HTimer_get_compar(uint8_t timer_n);
void HTimer_set_one_shot(uint8_t timer_n, uint64_t ms_time);
