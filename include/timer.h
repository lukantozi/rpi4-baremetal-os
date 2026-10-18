#ifndef TIMER_H
#define TIMER_H

unsigned int timer_read_counter(void);
void timer_test_polling(unsigned int interval);
void timer_arm(unsigned int interval);
void timer_handle(void);

#endif
