#include <acpi/apic/lvt.h>
#include <cpu/mmio.h>
#include <libc/stdint.h>

void LVT_set_entry(void *reg, LVT_Entry *entry) {
	// See: Figure 10-8 Local Vector Table (LVT)
	mmio_write32(reg, *((uint32_t *) entry));
}