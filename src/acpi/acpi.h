/*
	See ACPI Specification:
		For general long description what is going on:
			5. ACPI Software Programming Model
		For structures and essentials start with:
			5.2.5. Root System Description Pointer

	Shortly:
	RSDP.rsdt_addr -> RSDT.sdt_table[] = (example) [APIC, HPET, FACP, MADT]

	Longer:
	Kernel gets the address of ACPI RSDP structure from the bootloader.
	The ACPI RSDP structure has address RSDT structure.
	RSDT structure ends with table of 32-bit SDT addresses.
	To use some ACPI device/abilities we need to get right SDT structure.
	Every SDT structure starts with standard SDT Header but then it has
	custom fields and values for specific ACPI device and so on.
*/
#pragma once
#include <libc/stdint.h>

typedef uint8_t acpi_version;
#define ACPI_1_VERSION 0
#define ACPI_2_VERSION 2

#define RSDP_SIGNATURE "RSD PTR "
#define RSDT_SIGNATURE "RSDT"

// For all available signatures, see: Table 5.5 and 5.6
#define HPET_STD_SIGNATURE "HPET"
#define APIC_STD_SIGNATURE "APIC"

// See: Table 5.3
typedef struct __attribute__((packed)) {
	// RSDP for ACPI 1.0
	char signature[8];
	uint8_t checksum;
	uint8_t oem_id[6];
	uint8_t revision;
	uint32_t rsdt_addr;
} ACPI_RSDP;

// See: Table 5.4
typedef struct __attribute__((packed)) {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oem_id[6];
	char oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
} ACPI_SDT_Header;

// Read: 5.2.3.2. Generic Address Structure
typedef struct __attribute__((packed)) {
	uint8_t address_space_id;
	uint8_t reg_bit_width;
	uint8_t reg_bit_offset;
	uint8_t access_size;
	uint64_t address;
} ACPI_GAS;

// ACPI Spec., Table 5.7
typedef struct __attribute__((packed)) {
	ACPI_SDT_Header header;
	uint32_t sdt_table[]; // Table of 32-bit addresses of SDT headers
} ACPI_RSDT;

ACPI_SDT_Header *get_acpi_std_header(const char *signature);
acpi_version get_acpi_version(void);