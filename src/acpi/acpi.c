#include <acpi/acpi.h>
#include <bootloader.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>
#include <memory/paging.h>

acpi_version get_acpi_version(void) {
	ACPI_RSDP *rsdp = bootloader_data->apic_rsdp;

	return rsdp->revision;
}

bool is_rsdp_sig_correct(void) {
	// Check RSDP signature
	ACPI_RSDP *rsdp = bootloader_data->apic_rsdp;

	char *rsdp_sig = RSDP_SIGNATURE;
	if (strncmp(rsdp->signature, rsdp_sig, 8) == 0) {
		return true;
	}
	return false;
}

bool is_rsdt_sig_correct(void) {
	// Check RSDT signature
	ACPI_RSDP *rsdp = bootloader_data->apic_rsdp;
	ACPI_RSDT *rsdt = rsdp->rsdt_addr;

	char *rsdt_sig = RSDT_SIGNATURE;
	if (strncmp(rsdt->header.signature, rsdt_sig, 4) == 0) {
		return true;
	}
	return false;
}

ACPI_SDT_Header *get_acpi_std_header(const char *signature) {
	// Return pointer to the ACPI Standard Description Table Header
	// identified by signature. If not found, return NULL.
	//
	// It iterates over sdt_table in RSDT and matches signatures one by one.
	// See all possible signatures: ACPI Spec., Table 5.5, Table 5.6

	ACPI_RSDP *rsdp = bootloader_data->apic_rsdp;
	ACPI_RSDT *rsdt = rsdp->rsdt_addr;
	ACPI_SDT_Header *sdt_header = NULL;

	// Calculate size of sdt_table (total amount of 32-bit addresses)
	size_t entries = rsdt->header.length - sizeof(ACPI_RSDT) / 4;

	for (uint64_t i = 0; i < entries; i++) {
		sdt_header = rsdt->sdt_table[i];

		// Check signature
		if (strncmp((const char *) sdt_header->signature, signature, 4) == 0) {
			return sdt_header;
		}
	}

	return NULL;
}