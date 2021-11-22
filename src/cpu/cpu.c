#include "cpu.h"
#include "../kutils.h"
#include "../stdbool.h"
#include "../stdint.h"
#include "../stdlib.h"

extern void _cpuid(uint32_t cpuid_eax, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

cpu_status _check_cpu_state(void) {
	uint32_t edx = 0;

	// ----------------------------------------------
	_cpuid(0x1, NULL, NULL, NULL, &edx);
	if (test_bit(edx, 0) == false) {
		return CPU_NO_FPU;
	}

	if (test_bit(edx, 5) == false) {
		return CPU_NO_MSR;
	}

	if (test_bit(edx, 9) == false) {
		return CPU_NO_APIC;
	}

	// ----------------------------------------------
	_cpuid(0x80000001, NULL, NULL, NULL, &edx);
	if (test_bit(edx, 29) == false) {
		return CPU_NO_LONG_MODE;
	}

	return CPU_SUCCESS;
}

void _print_cpu_err(const char *str) { printf("[!] CPU error: %s \n", str); }

cpu_status check_cpu_state(void) {
	cpu_status status = _check_cpu_state();

	switch (status) {
		case CPU_NO_LONG_MODE:
			_print_cpu_err("no long mode detected");
			break;
		case CPU_NO_FPU:
			_print_cpu_err("no FPU detected");
			break;
		case CPU_NO_APIC:
			_print_cpu_err("no APIC detected");
			break;
		case CPU_NO_MSR:
			_print_cpu_err("no MSRs detected");
		case CPU_SUCCESS:
			break;
		default:
			_print_cpu_err("undefined status detected");
			break;
	}

	return status;
}
