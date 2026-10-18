#ifndef UART_H
#define UART_H

char mini_uart_receive_char(void);
void mini_uart_send_char(char c);
void mini_uart_send_string(const char *s);
char uart0_receive_char(void);
void uart0_send_char(char c);
void uart0_send_string(const char *s);
void uart_init(void);

#endif
