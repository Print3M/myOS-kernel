#include <acpi/apic/apic.h>
#include <cpu/cpu.h>
#include <cpu/mmio.h>
#include <kutils.h>
#include <libc/stdint.h>

void APIC_write_msr(uint64_t value) {
	// Write into APIC MSR
	// See: Figure 10-5. IA32_APIC_BASE MSR
	write_msr(APIC_BASE_MSR, value);
}

uint64_t APIC_read_msr(void) {
	// Get APIC MSR value
	// See: Figure 10-5. IA32_APIC_BASE MSR
	return read_msr(APIC_BASE_MSR);
}

uint8_t APIC_get_local_id(void) {
	// Get local APIC id from APIC ID register
	// Read: 10.4.6 Local APIC ID
	uint32_t id_reg = mmio_read32((void *) APIC_ID_REG);
	id_reg >>= 24;

	return (uint8_t) id_reg;
}

uint8_t APIC_get_init_id(void) {
	// Get init APIC id from CPUID
	// Read: 10.4.6 Local APIC ID
	uint32_t ebx = 0;
	get_cpuid(0x1, NULL, &ebx, NULL, NULL);
	ebx >>= 24;

	return (uint8_t) ebx;
}

void APIC_enable(void) {
	// Enable APIC by writing enable byte into APIC MSR
	// Read: 10.4.3 Enabling or Disabling the Local APIC
	uint64_t msr = read_msr(APIC_BASE_MSR);
	msr = set_bit(msr, 11);
	APIC_write_msr(msr);
}

uint8_t APIC_get_version(void) {
	// Read: 10.4.8 Local APIC Version Register
	return mmio_read8((void *) APIC_VERSION_REG);
}
