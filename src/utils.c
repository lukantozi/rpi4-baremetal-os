#include "utils.h"

void put32(unsigned long addr, unsigned int val)
{
    *(volatile unsigned int *)addr = val;
}

unsigned int get32(unsigned long addr)
{
    return *(volatile unsigned int *)addr;
}
