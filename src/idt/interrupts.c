#include <idt/interrupts.h>
#include <libc/stdbool.h>
#include <libc/stdlib.h>
#include <kutils.h>

__attribute__((interrupt)) void page_fault_handler(struct interrupt_frame *frame) {
	printf("Page Fault Interrupt here! \n");

	while (true)
		;
}

__attribute__((interrupt)) void divide_by_zero_handler(struct interrupt_frame *frame) {
	kpanic("division by zero");

	while (true)
		;
}