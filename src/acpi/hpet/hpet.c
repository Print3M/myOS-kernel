#include <acpi/acpi.h>
#include <acpi/hpet/hpet.h>

HPET_DescTable *get_hpet_desc_table(void) {
	// Return pointer to the HPET Description Table
	return (HPET_DescTable *) get_acpi_std_header(HPET_STD_SIGNATURE);
}