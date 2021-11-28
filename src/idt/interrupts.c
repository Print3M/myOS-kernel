#include "./interrupts.h"
#include "../stdbool.h"
#include "../stdlib.h"

__attribute__((interrupt)) void page_fault_handler(struct interrupt_frame *frame) {
	printf("Page Fault Interrupt here! \n");

	while (true)
		;
}