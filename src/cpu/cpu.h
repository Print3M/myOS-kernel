#pragma once
#include "../stdbool.h"
#include "../stdint.h"

#define IA32_EFER 0xC0000080

typedef uint16_t cpu_status;
#define CPU_ERROR		 0x0000
#define CPU_SUCCESS		 0x0001
#define CPU_NO_LONG_MODE 0x0002
#define CPU_NO_FPU		 0x0003
#define CPU_NO_APIC		 0x0004
#define CPU_NO_MSR		 0x0005

cpu_status check_cpu_state(void);

extern uint64_t read_msr(uint32_t msr_no);
extern void write_msr(uint32_t msr_no, uint64_t value);