/*
	About APIC in general read (Intel Manual vol. 3):
		10. Advanced Programmable Interrupt Controller (APIC)

	APIC consists of two parts: "local APIC" (LAPIC)
	and "I/O APIC":

	-   Local APIC receives interrupts from the processor’s interrupt pins,
	from internal sources and from an external I/O APIC (or other external
	interrupt controller). It sends these to the processor core for handling.
	This is the part which we can control and programm! Each local APIC
	consists of a set of APIC registers (see Table 10-1) and associated
	hardware that control the delivery of interrupts to the processor core.
	The APIC registers are memory mapped and can be read and written to using
	the MOV instruction.

	-    I/O APIC is part of Intel’s system chip set. Its primary function is
	to receive external interrupt events from the system and its associated
	I/O devices and relay them to the local APIC as interrupt messages.

	Local APIC handles interrupts from externally connected I/O devices
	and local interrupt sources (processor LINT0 and LINT1 pins, APIC timer,
	performance-monitoring counters, thermal sensors, internal APIC error
	detector). LAPIC manages all external interrupts for CPU. There is one
	LAPIC in each CPU in the system.

	APIC registers are memory-mapped to a 4-KByte region of the processor’s
	physical address space with an initial starting address of FEE00000H.
	All memory mapped local APIC registers, see: Table 10-1
*/
#pragma once
#include <libc/stdint.h>

#define APIC_BASE_MSR  0x1b
#define APIC_BASE_ADDR 0xfee0000

#define APIC_ID_REG		 (APIC_BASE_ADDR + 0x20)
#define APIC_VERSION_REG (APIC_BASE_ADDR + 0x30)

// Chapter: 10.4.4 Local APIC Status and Location
typedef struct __attribute__((packed)) {
	uint8_t reserved1;
	uint8_t bsp : 1;
	uint8_t reserved2 : 2;
	uint8_t is_enabled : 1;
	uint32_t base_addr : 24;
} APIC_msr;

void APIC__enable(void);
uint8_t APIC__get_local_id(void);
uint8_t APIC__get_init_id(void);
uint8_t APIC__get_version(void);
