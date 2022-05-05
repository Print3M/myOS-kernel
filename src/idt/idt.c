#include <idt/idt.h>
#include "../gdt/gdt.h"
#include "../stdint.h"
#include "../stdlib.h"
#include "./interrupts.h"

IDT_DescriptionEntry idt[ALL_IDT_ENTRIES];

void _set_interrupt(uint8_t idt_index, void *handler, uint16_t segment_selector) {
	IDT_DescriptionEntry *idt_entry = &idt[idt_index];
	idt_entry->present = 1;
	idt_entry->gate_type = INTERRUPT_GATE;
	idt_entry->segment_selector = segment_selector;

	uint32_t offset = (uint32_t) handler;
	idt_entry->offset1 = (uint16_t)(offset & 0xffff);
	offset >>= 16;
	idt_entry->offset2 = (uint16_t)(offset & 0xffff);
	offset >>= 16;
	idt_entry->offset3 = (uint32_t)(offset & 0xffffffff);
}

void init_idt(void) {
	// Load page fault handler
	_set_interrupt(PAGE_FAULT_INDEX, (void *) &page_fault_handler, KERNEL_CODE_SEGMENT_SELECTOR);

	// Load IDTR
	uint16_t limit = sizeof(IDT_DescriptionEntry) * ALL_IDT_ENTRIES;
	IDTR idtr = {.limit = limit, .base_addr = &idt};
	__asm__ volatile("lidt %0" ::"m"(idtr));
}