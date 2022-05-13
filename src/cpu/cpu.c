#include <cpu/cpu.h>
#include <kutils.h>
#include <libc/stdbool.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>

CPU_ModelInfo cpu_model_info;
CPU_Info cpu_info;

cpu_status _check_cpu_state(void) {
	uint32_t edx = 0;

	// ----------------------------------------------
	get_cpuid(0x1, NULL, NULL, NULL, &edx);
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
	get_cpuid(0x80000001, NULL, NULL, NULL, &edx);
	if (test_bit(edx, 29) == false) {
		return CPU_NO_LONG_MODE;
	}

	return CPU_SUCCESS;
}

void _print_cpu_err(const char *str) { printf("[!] CPU error: %s \n", str); }

void _get_cpu_brand(void) {
	uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

	get_cpuid(0x80000002, &eax, &ebx, &edx, &ecx);
	memcpy(&cpu_model_info.brand[0], &eax, 4);
	memcpy(&cpu_model_info.brand[4], &ebx, 4);
	memcpy(&cpu_model_info.brand[8], &edx, 4);
	memcpy(&cpu_model_info.brand[12], &ecx, 4);

	get_cpuid(0x80000003, &eax, &ebx, &edx, &ecx);
	memcpy(&cpu_model_info.brand[16], &eax, 4);
	memcpy(&cpu_model_info.brand[20], &ebx, 4);
	memcpy(&cpu_model_info.brand[24], &edx, 4);
	memcpy(&cpu_model_info.brand[28], &ecx, 4);

	get_cpuid(0x80000004, &eax, &ebx, &edx, &ecx);
	memcpy(&cpu_model_info.brand[32], &eax, 4);
	memcpy(&cpu_model_info.brand[36], &ebx, 4);
	memcpy(&cpu_model_info.brand[40], &edx, 4);
	memcpy(&cpu_model_info.brand[44], &ecx, 4);

	cpu_model_info.brand[48] = '\0';
}

void get_cpu_info(void) {
	// To know more about certain registers, see:
	// https://en.wikipedia.org/wiki/CPUID
	uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

	// Get manufacturer id
	get_cpuid(0x0, NULL, &ebx, &ecx, &edx);
	memcpy(&cpu_model_info.manufacturer[0], &ebx, 4);
	memcpy(&cpu_model_info.manufacturer[4], &edx, 4);
	memcpy(&cpu_model_info.manufacturer[8], &ecx, 4);
	cpu_model_info.manufacturer[12] = '\0';

	// Get model info
	get_cpuid(0x1, &eax, &ebx, NULL, &edx);
	cpu_model_info.stepping_id = eax & 0xf;
	eax >>= 4;
	cpu_model_info.model_id = eax & 0xf;
	eax >>= 4;
	cpu_model_info.family_id = eax & 0xf;
	eax >>= 4;
	cpu_model_info.type = eax & 0x3;
	eax >>= 4;

	if (cpu_model_info.model_id == 0xf) {
		// Extended model id
		cpu_model_info.model_id += eax & 0xf;
	}
	eax >>= 4;

	if (cpu_model_info.family_id == 0xf) {
		// Extended family id
		cpu_model_info.family_id += eax & 0xff;
	}

	_get_cpu_brand();

	// Get CPU technical info
	cpu_info.logical_cores = (ebx >> 16) & 0xff;
	cpu_info.hyperthreading = test_bit(edx, 28);

	get_cpuid(0x80000008, NULL, NULL, &ecx, NULL);
	cpu_info.physical_cores = (uint8_t) ecx;
	cpu_info.physical_cores += 1;
}

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
