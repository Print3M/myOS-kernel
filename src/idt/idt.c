#include <gdt/gdt.h>
#include <idt/idt.h>
#include <idt/interrupts.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>

IDT_DescriptionEntry idt[ALL_IDT_ENTRIES];

void _set_idt_entry(uint8_t gate_type,
					uint8_t idt_index,
					void (*handler)(),
					uint16_t segment_selector) {
	IDT_DescriptionEntry *idt_entry = &idt[idt_index];
	idt_entry->present = 1;
	idt_entry->gate_type = gate_type;
	idt_entry->segment_selector = segment_selector;

	uint64_t offset = (uint64_t) handler;
	idt_entry->offset1 = (uint16_t)(offset & 0xffff);
	offset >>= 16;
	idt_entry->offset2 = (uint16_t)(offset & 0xffff);
	offset >>= 16;
	idt_entry->offset3 = (uint32_t)(offset & 0xffffffff);
}

IDT_DescriptionEntry *init_idt(void) {
	// Load interrupt handlers into IDT
	_set_idt_entry(
		INTERRUPT_GATE, PAGE_FAULT_INDEX, &page_fault_handler, KERNEL_CODE_SEGMENT_SELECTOR);
	_set_idt_entry(
		TRAP_GATE, DIVIDE_BY_ZERO_INDEX, &divide_by_zero_handler, KERNEL_CODE_SEGMENT_SELECTOR);

	// Load IDT to IDTR
	uint16_t limit = sizeof(IDT_DescriptionEntry) * ALL_IDT_ENTRIES;
	IDTR idtr = {.limit = limit, .base_addr = (void *) &idt};
	__asm__ volatile("lidt %0" ::"m"(idtr));

	return (IDT_DescriptionEntry *) idt;
}