#pragma once
#include "../stdint.h"

#define PAGE_SZ 4096 // In bytes

typedef struct {
	uint32_t type;
	uint32_t pad;
	void *physical_addr;
	void *virtual_addr;
	uint64_t number_of_pages;
	uint64_t attribute;
} EfiMemoryDescriptor;

typedef struct {
	EfiMemoryDescriptor *memory_map;
	size_t map_sz;
	size_t descriptor_sz;
	uint64_t entries;
} MemoryData;

typedef enum {
	EFI_RESERVED_MEMORY_TYPE,
	EFI_LOADER_CODE,
	EFI_LOADER_DATA,
	EFI_BOOT_SERVICES_CODE,
	EFI_BOOT_SERVICES_DATA,
	EFI_RUNTIME_SERVICES_CODE,
	EFI_RUNTIME_SERVICES_DATA,
	EFI_CONVENTIONAL_MEMORY,
	EFI_UNUSABLE_MEMORY,
	EFI_ACPI_RECLAIM_MEMORY,
	EFI_ACPI_MEMORY_NVS,
	EFI_MEMORY_MAPPED_IO,
	EFI_MEMORY_MAPPED_IO_PORT_SPACE,
	EFI_PAL_CODE,
	EFI_MAX_MEMORY_TYPE
} efi_memory_type;

EfiMemoryDescriptor *
get_memory_descriptor(EfiMemoryDescriptor *memory_map, uint64_t n, size_t descriptor_sz);

uint64_t get_used_memory_size(MemoryData *memory);
