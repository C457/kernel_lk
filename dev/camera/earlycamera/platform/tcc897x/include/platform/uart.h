
#ifndef _UART_H_
#define _UART_H_

extern void uart_init(void);
extern int _dgetc(char * c);
extern void _dputc(char c);
extern int _dputs(const char * str);

#endif
