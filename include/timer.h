#ifndef TIMER_H
#define TIMER_H

unsigned int timer_read_counter(void);
void timer_test_polling(unsigned int interval);
void timer_arm(unsigned int interval);
void timer_handle(void);
void timer_generic_test_counter(unsigned int iterations);
void timer_generic_test_frequency(void);
void timer_generic_test_polling(unsigned int interval);
void timer_generic_arm(unsigned int seconds);
void timer_generic_handle(void);

#endif
