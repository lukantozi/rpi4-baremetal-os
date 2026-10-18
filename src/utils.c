#include "utils.h"

void write_reg32(unsigned long addr, unsigned int val)
{
    *(volatile unsigned int *)addr = val;
}

unsigned int read_reg32(unsigned long addr)
{
    return *(volatile unsigned int *)addr;
}

void write_addr64(unsigned long addr, unsigned long val)
{
    *(volatile unsigned long *)addr = val;
}

unsigned long read_addr64(unsigned long addr)
{
    return *(volatile unsigned long *)addr;
}

int get_current_el(void)
{
    unsigned long el;
    __asm__ volatile("mrs %0, CurrentEl" : "=r" (el));
    return (el >> 2) & 0x3;
}

void vector_init(void)
{
    unsigned long addr;
    /* set vectr base address reg to vectors */
    __asm__ volatile("adr %0, vectors" : "=r" (addr));
    __asm__ volatile("msr vbar_el1, %0\n\t" "isb" : : "r" (addr) : "memory");
}

void enable_irq(void)
{
    __asm__ volatile("msr daifclr, #2" ::: "memory");
}

void disable_irq(void)
{
    __asm__ volatile("msr daifset, #2" ::: "memory");
}
