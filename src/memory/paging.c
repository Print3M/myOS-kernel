#include "paging.h"
#include "../cpu/cpu.h"
#include "../framebuffer/framebuffer.h"
#include "../kutils.h"
#include "../stdbool.h"
#include "../stdint.h"
#include "../stdlib.h"
#include "pmem.h"

PML4_Entry PML4[512] __attribute__((aligned(0x1000)));

extern void _enable_cpu_4_level_paging();
extern void _get_cr4();

void set_cr3_register(uint64_t paddr, bool pwt, bool pcd) {
	/*
		:paddr - physical address of first paging-translation struct
		:pwt   - page-level write-through
		:pcd   - page-level cache disable

		What is going on here? Checkout:
			Intel Manual vol. 3, chapter: 4.5 - 4-level paging
	*/
	uint64_t cr3 = paddr;

	if (pwt)
		cr3 = set_bit(cr3, CR3_PWT);
	if (pcd)
		cr3 = set_bit(cr3, CR3_PCD);

	__asm__ volatile("mov %0, %%cr3" ::"r"(cr3));
}

void _extract_vaddr_paging_indexes(uint64_t vaddr, VaddrIndexes *indexes) {
	/*
		IN  :vaddr   - virtual address to perform extraction
		OUT :indexes - return struct of indexes
	*/
	vaddr >>= PAGE_OFFSET_BITS; // Remove first 12 bytes (page offset)

	indexes->pt_entry = vaddr & BITS_9_MASK; // PageTable entry
	vaddr >>= 9;

	indexes->pd_entry = vaddr & BITS_9_MASK; // PageDirectory entry
	vaddr >>= 9;

	indexes->pdpt_entry = vaddr & BITS_9_MASK; // PageDirectoryPointerTable entry
	vaddr >>= 9;

	indexes->pml4_entry = vaddr & BITS_9_MASK; // PageMapLevel4 entry
}

void map_vaddr_to_paddr(void *vaddr, void *paddr) {
	// Go down the paging-structure (by :vaddr) to the page and set its :paddr

	// TODO: What if pmem_request_frame returns NULL
	// TODO: What if pmem_request_frame returns addr higher than 44 bits (32 + PAGE_OFFSET)?

	VaddrIndexes indexes;
	_extract_vaddr_paging_indexes((uint64_t) vaddr, &indexes);

	// ==================== PML4 =========================
	PML4_Entry *pml4_entry = &PML4[indexes.pml4_entry];
	PDPT_Entry *PDPT;
	if (pml4_entry->present == false) {
		// If not already present, create PDPT
		PDPT = (PDPT_Entry *) pmem_request_frame();
		memset(PDPT, 0, FRAME_SZ);

		// Set PML4 entry
		pml4_entry->PDPT_addr = (uint64_t) PDPT >> PAGE_OFFSET_BITS;
		pml4_entry->present = true;
		pml4_entry->rw = true;
	} else {
		PDPT = (PDPT_Entry *) (pml4_entry->PDPT_addr << PAGE_OFFSET_BITS);
	}

	// ===================== PDPT =========================
	PDPT_Entry *pdpt_entry = &PDPT[indexes.pdpt_entry];
	PD_Entry *PD;
	if (pdpt_entry->present == false) {
		// If PDPT entry not present, create PD and set PDPT entry
		PD = (PD_Entry *) pmem_request_frame();
		memset(PD, 0, FRAME_SZ);

		// Set PDPT entry
		pdpt_entry->PD_addr = (uint64_t) PD >> PAGE_OFFSET_BITS;
		pdpt_entry->present = true;
		pdpt_entry->rw = true;
	} else {
		PD = (PD_Entry *) (pdpt_entry->PD_addr << PAGE_OFFSET_BITS);
	}

	// ====================== PD ===========================
	PD_Entry *pd_entry = &PD[indexes.pd_entry];
	PT_Entry *PT;
	if (pd_entry->present == false) {
		// If PD entry not present, create PT and set PD entry
		PT = (PT_Entry *) pmem_request_frame();
		memset(PT, 0, FRAME_SZ);

		// Set PD entry
		pd_entry->PT_addr = (uint64_t) PT >> PAGE_OFFSET_BITS;
		pd_entry->present = true;
		pd_entry->rw = true;
	} else {
		PT = (PT_Entry *) (pd_entry->PT_addr << PAGE_OFFSET_BITS);
	}

	// Set PT entry (actual page)
	PT_Entry *pt_entry = &PT[indexes.pt_entry];
	pt_entry->page_addr = (uint64_t) paddr >> PAGE_OFFSET_BITS;
	pt_entry->present = true;
	pt_entry->rw = true;
}

bool is_page_present(void *vaddr) {
	VaddrIndexes indexes;
	_extract_vaddr_paging_indexes((uint64_t) vaddr, &indexes);

	PML4_Entry *pml4_entry = &PML4[indexes.pml4_entry];
	if (pml4_entry->present == false) {
		return false;
	}
	PDPT_Entry *PDPT = (PDPT_Entry *) (pml4_entry->PDPT_addr << PAGE_OFFSET_BITS);

	PDPT_Entry *pdpt_entry = &PDPT[indexes.pdpt_entry];
	if (pdpt_entry->present == false) {
		return false;
	}
	PD_Entry *PD = (PD_Entry *) (pdpt_entry->PD_addr << PAGE_OFFSET_BITS);

	PD_Entry *pd_entry = &PD[indexes.pd_entry];
	if (pd_entry->present == false) {
		return false;
	}
	PT_Entry *PT = (PT_Entry *) (pd_entry->PT_addr << PAGE_OFFSET_BITS);

	PT_Entry *pt_entry = &PT[indexes.pt_entry];
	return pt_entry->present;
}

paging_status init_paging(Framebuffer *framebuffer) {
	// Intel Manual vol. 3:
	//  4.1.1 - specific options to set for 4-level paging
	_enable_cpu_4_level_paging();

	// Set IA32_EFER.LME bit
	uint64_t msr = read_msr(IA32_EFER);
	write_msr(IA32_EFER, set_bit(msr, EFER_LME));

	// Identy paging (1:1)
	for (size_t i = 0; i < physical_memory.frames; i++) {
		void *addr = physical_memory.array[i].frame_addr;
		map_vaddr_to_paddr(addr, addr);
	}

	// Identy paging for framebuffer
	for (size_t i = 0; i < framebuffer->buffer_size / FRAME_SZ; i += 1) {
		void *addr = (void *) ((uint64_t) framebuffer->base_address + i * FRAME_SZ);
		map_vaddr_to_paddr(addr, addr);
	}

	// Load PML4 structure -> enable paging
	set_cr3_register((uint64_t) &PML4, false, false);

	// Test paging abilities
	size_t test_addr = 0x99999999999; // Way more than physical memory
	uint8_t *test_addr_ptr = (uint8_t *) test_addr;
	map_vaddr_to_paddr((void *) test_addr, (void *) 0x0);
	*test_addr_ptr = 123;
	return *test_addr_ptr == 123 ? PAGING_INIT_SUCCESS : PAGING_INIT_ERROR;
}