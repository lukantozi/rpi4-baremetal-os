#include "utils.h"

void write_reg32(unsigned long addr, unsigned int val)
{
    *(volatile unsigned int *)addr = val;
}

unsigned int read_reg32(unsigned long addr)
{
    return *(volatile unsigned int *)addr;
}

void write_addr32(unsigned long addr, unsigned long val)
{
    *(volatile unsigned long *)addr = val;
}

unsigned long read_addr32(unsigned long addr)
{
    return *(volatile unsigned long *)addr;
}
