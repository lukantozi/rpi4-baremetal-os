#ifndef UTILS_H
#define UTILS_H

void write_reg32(unsigned long addr, unsigned int val);
unsigned int read_reg32(unsigned long addr);
void write_addr64(unsigned long addr, unsigned long val);
unsigned long read_addr64(unsigned long addr);
int get_current_el(void);
void vector_init(void);
void enable_irq(void);
void disable_irq(void);
int strlen(const char *);
int strcmp(const char *s1, const char *s2);

#define REPORT_EL(el, uart)                                           \
do {                                                                  \
    switch (el) {                                                     \
        case 0: uart##_send_string("[Current EL] EL0\r\n"); break;    \
        case 1: uart##_send_string("[Current EL] EL1\r\n"); break;    \
        case 2: uart##_send_string("[Current EL] EL2\r\n"); break;    \
        case 3: uart##_send_string("[Current EL] EL3\r\n"); break;    \
    }                                                                 \
} while(0)                                                            \

#define TYPE(uart)                                  \
do {                                                \
    uart##_send_string("Type: ");                   \
    while (1) {                                     \
        uart##_send_char(uart##_receive_char());    \
    }                                               \
} while (0)                                         \

#define sizeof(x) ((unsigned int) ((char *)(&(x)+1) - (char *)(&(x))))

/* from entry.S */
extern unsigned long vectors;

#endif
