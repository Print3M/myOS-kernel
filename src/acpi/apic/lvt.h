/*
	Read: 10.5 Handling Local Interrupts

	The Local Vector Table (LVT) allows software to specify the manner in
	which the local interrupts are delivered to the CPU. It consists of the
	32-bit APIC registers (see Figure 10-8), one for each local interrupt.

	The Intel architectures define 256 vector numbers, ranging from 0
	through 255 in IDT. Local and I/O APICs support 240 of these vectors
	(in the range of 16 to 255) as valid interrupts. When an interrupt vector
	in the range of 0 to 15 is sent or received through the local APIC,
	the APIC indicates an illegal vector in its Error Status Register.
	x86-64 reserves vectors from range 16-31 for predefined interrupts
	but the local APIC doesn't treat vectors in this range as illegal.
*/

#pragma once
#include <acpi/apic/apic.h>
#include <libc/stdint.h>

// For better understanding of the following values
// read: 10.5.1 Local Vector Table
#define LVT_CMCI_REG  (APIC_BASE_ADDR + 0x2f0)
#define LVT_LINT0_REG (APIC_BASE_ADDR + 0x350)
#define LVT_LINT1_REG (APIC_BASE_ADDR + 0x360)
#define LVT_ERROR_REG (APIC_BASE_ADDR + 0x370)

// Delivery modes
#define DELIVERY_MODE_FIXED	  0x0
#define DELIVERY_MODE_SMI	  0x2
#define DELIVERY_MODE_NMI	  0x4
#define DELIVERY_MODE_INIT	  0x5
#define DELIVERY_MODE_EXT_INT 0x7

// Delivery statuses
#define DELIVERY_STATUS_IDLE	0x0
#define DELIVERY_STATUS_PENDING 0x1

// See: Figure 10-8. Local Vector Table (LVT)
typedef struct __attribute__((packed)) {
	uint8_t vector;
	uint8_t delivery_mode : 3;
	uint8_t reserved1 : 1;
	uint8_t delivery_status : 1;
	uint8_t pin_polarity : 1;
	uint8_t remote_irr : 1;
	uint8_t trigger_mode : 1;
	uint8_t mask : 1;
	uint16_t reserved2 : 15;
} LVT_Entry;