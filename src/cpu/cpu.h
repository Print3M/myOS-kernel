#pragma once
#include "../stdbool.h"
#include "../stdint.h"

typedef uint16_t cpu_status;

#define CPU_ERROR		 0x0000
#define CPU_SUCCESS		 0x0001
#define CPU_NO_LONG_MODE 0x0002
#define CPU_NO_FPU		 0x0003
#define CPU_NO_APIC		 0x0004

cpu_status check_cpu_state(void);