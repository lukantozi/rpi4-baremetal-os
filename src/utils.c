#include "utils.h"

void write_reg32(unsigned long addr, unsigned int val)
{
    *(volatile unsigned int *)addr = val;
}

unsigned int read_reg32(unsigned long addr)
{
    return *(volatile unsigned int *)addr;
}
