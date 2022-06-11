/* About paging on x86 you can read in Intel Manual vol. 3
	4.1.1 - specific options for each mode of paging
	4.2   - overview of paging, description of translation
	4.5   - specific 4-level paging documentation
*/
#pragma once
#include <framebuffer/framebuffer.h>
#include <libc/stdbool.h>
#include <libc/stdint.h>
#include <memory/pmem.h>

#define PAGE_SZ FRAME_SZ

#define IA32_EFER 0xC0000080

#define EFER_LME		 8
#define CR3_PWT			 3
#define CR3_PCD			 4
#define BITS_9_MASK		 0x1ff
#define PAGE_OFFSET_BITS 12

/*
	Every paging structure is 4096 Bytes in size and contains a
	number of individual entries. With the 4-level paging modes,
	each  entry is 64 bits (8 bytes); there are thus 512 entries
	in each structure. 4-level paging translates 48-bit linear
	addresses to 52-bit physical addresses. Although 52 bits
	corresponds to 4 PBytes, linear addresses are limited to 48 bits;
	at most 256 TBytes of linear-address space may be accessed at
	any given time. 4-level paging translates linear addresses using
	a hierarchy of in-memory paging structures located using the
	contents of CR3, which is used to locate the first paging-structure.
*/

typedef struct {
	uint16_t pml4_entry; // PageMapLevel4
	uint16_t pdpt_entry; // PageDirectoryPointerTable
	uint16_t pd_entry;	 // PageDirectory
	uint16_t pt_entry;	 // PageTable
} VaddrIndexes;

typedef struct __attribute__((packed)) {
	// Description: Intel Manual vol. 3, Table 4-20
	uint8_t present : 1;
	uint8_t rw : 1;
	uint8_t us : 1;
	uint8_t pwt : 1;
	uint8_t pcd : 1;
	uint8_t a : 1;
	uint8_t d : 1;
	uint8_t pat : 1;
	uint8_t g : 1;
	uint8_t ignored1 : 3;
	uint64_t page_addr : 32;
	uint8_t reserved1 : 8;
	uint8_t ignored3 : 7;
	uint8_t protection_key : 4;
	uint8_t xd : 1;
} PT_Entry; // PageTable

typedef struct __attribute__((packed)) {
	// Description: Intel Manual vol. 3, Table 4-19
	uint8_t present : 1;
	uint8_t rw : 1;
	uint8_t us : 1;
	uint8_t pwt : 1;
	uint8_t pcd : 1;
	uint8_t a : 1;
	uint8_t ignored1 : 1;
	uint8_t ps : 1;
	uint8_t ignored2 : 4;
	uint64_t PT_addr : 32;
	uint8_t reserved1 : 8;
	uint16_t ignored3 : 11;
	uint8_t xd : 1;
} PD_Entry; // PageDirectory

typedef struct __attribute__((packed)) {
	// Description: Intel Manual vol. 3, Table 4-17
	uint8_t present : 1;
	uint8_t rw : 1;
	uint8_t us : 1;
	uint8_t pwt : 1;
	uint8_t pcd : 1;
	uint8_t a : 1;
	uint8_t ignored1 : 1;
	uint8_t ps : 1;
	uint8_t ignored2 : 4;
	uint64_t PD_addr : 32;
	uint8_t reserved1 : 8;
	uint16_t ignored3 : 11;
	uint8_t xd : 1;
} PDPT_Entry; // PageDirectoryPointerTable

typedef struct __attribute__((packed)) {
	// Description: Intel Manual vol. 3, Table 4-15
	uint8_t present : 1;
	uint8_t rw : 1;
	uint8_t us : 1;
	uint8_t pwt : 1;
	uint8_t pcd : 1;
	uint8_t a : 1;
	uint8_t ignored1 : 1;
	uint8_t ps : 1;
	uint8_t ignored2 : 4;
	uint64_t PDPT_addr : 32;
	uint8_t reserved1 : 8;
	uint16_t ignored3 : 11;
	uint8_t xd : 1;
} PML4_Entry; // PageMapLevel4

bool is_page_present(void *vaddr);
void map_vaddr_to_paddr(void *vaddr, void *paddr);
PML4_Entry *init_paging(void);