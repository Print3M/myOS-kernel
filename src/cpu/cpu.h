#pragma once
#include <libc/stdbool.h>
#include <libc/stdint.h>

typedef uint16_t cpu_status;
#define CPU_ERROR		 0x0000
#define CPU_SUCCESS		 0x0001
#define CPU_NO_LONG_MODE 0x0002
#define CPU_NO_FPU		 0x0003
#define CPU_NO_APIC		 0x0004
#define CPU_NO_MSR		 0x0005

typedef struct {
	char manufacturer[13]; // + \0
	uint8_t stepping_id : 4;
	uint8_t type : 2;
	uint16_t model_id;
	uint16_t family_id;
	char brand[49]; // + \0
} CPU_ModelInfo;

typedef struct {
	bool hyperthreading;
	uint8_t physical_cores;
	uint8_t logical_cores;
} CPU_Info;

extern CPU_ModelInfo cpu_model_info;
extern CPU_Info cpu_info;

cpu_status check_cpu_state(void);
void get_cpu_info(void);

// Assembly functions
extern void
get_cpuid(uint32_t cpuid_eax, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);
extern uint64_t read_msr(uint32_t msr_no);
extern void write_msr(uint32_t msr_no, uint64_t value);