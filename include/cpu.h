#ifndef CPU_H
#define CPU_H

#define WAKE_CPU1 0xe0
#define WAKE_CPU2 0xe8
#define WAKE_CPU3 0xf0

void cpu1(void);
void cpu2(void);
void cpu3(void);

#define INIT_CPU(N, INSTR)                                                  \
do {                                                                        \
    write_addr64((unsigned long)&spin_cpu##N, (unsigned long)(INSTR));      \
    write_addr64(WAKE_CPU##N, (unsigned long)&cpu123);                      \
} while(0)                                                                  \

#define INIT_CPUS      \
do {                   \
    INIT_CPU(1, cpu1); \
    INIT_CPU(2, cpu2); \
    INIT_CPU(3, cpu3); \
} while (0)            \

#endif
