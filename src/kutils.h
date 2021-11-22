#pragma once

#define set_bit(var, pos)	((var) | 1 << pos)
#define clear_bit(var, pos) ((var) & ~(1 << pos))
#define test_bit(var, pos)	((var) & (1 << pos))

void kprint_err(const char *msg);
void kprint_succ(const char *msg);
void kprint_info(const char *msg);

void kprint_pmem_info(void);