#include <acpi/apic/apic.h>
#include <cpu/cpu.h>
#include <libc/stdint.h>

uint8_t APIC__get_local_id(void) {
	// Read chapter: 10.4.6 Local APIC ID
	// See: Figure 10-6. Local APIC ID Register
	uint32_t id_reg = *((uint32_t *) APIC_ID_REG);
	id_reg >>= 24;

	return (uint8_t) id_reg;
}

uint8_t APIC__get_init_id(void) {
	uint32_t ebx = 0;
	get_cpuid(0x1, NULL, &ebx, NULL, NULL);
	ebx >>= 24;

	return (uint8_t) ebx;
}

void APIC__enable(void) {
	uint64_t msr = read_msr(APIC_BASE_MSR);
	msr |= 1 << 11;
	write_msr(APIC_BASE_MSR, msr);
}

uint8_t APIC__get_version(void) {
	// Read chapter: 10.4.8 Local APIC Version Register
	return *((uint8_t *) APIC_VERSION_REG);
}

uint64_t APIC__get_msr(void) { return read_msr(APIC_BASE_MSR); }