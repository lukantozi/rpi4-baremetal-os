#ifndef UTILS_H
#define UTILS_H

void write_reg32(unsigned long addr, unsigned int val);
unsigned int read_reg32(unsigned long addr);
void write_addr64(unsigned long addr, unsigned long val);
unsigned long read_addr64(unsigned long addr);
int get_current_el(void);

#endif
