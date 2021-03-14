/*
 * Copyright (c) 2008 Travis Geiselbrecht
 * Copyright (c) 2010 Telechips, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <reg.h>
#include <iomap.h>
#include <reg_physical.h>

#define DEBUG_UART	0

struct uart_stat {
	volatile unsigned int base;
	int ch;
};

static struct uart_stat uart[] = {
	{ TCC_UART0_BASE, 0 },
	{ TCC_UART1_BASE, 1 },
	{ TCC_UART2_BASE, 2 },
	{ TCC_UART3_BASE, 3 },
	{ TCC_UART4_BASE, 4 },
	{ TCC_UART5_BASE, 5 },
	{ TCC_UART6_BASE, 6 },
	{ TCC_UART7_BASE, 7 },
};

#define uart_reg_write(p, a, v)	writel(v, uart[p].base + (a))
#define uart_reg_read(p, a)		readl(uart[p].base + (a))

#define UART_RBR	0x00		/* receiver buffer register */
#define UART_THR	0x00		/* transmitter holding register */
#define UART_DLL	0x00		/* divisor latch (LSB) */
#define UART_IER	0x04		/* interrupt enable register */
#define UART_DLM	0x04		/* divisor latch (MSB) */
#define UART_IIR	0x08		/* interrupt ident. register */
#define UART_FCR	0x08		/* FIFO control register */
#define UART_LCR	0x0C		/* line control register */
#define UART_MCR	0x10		/* MODEM control register */
#define UART_LSR	0x14		/* line status register */
#define UART_MSR	0x18		/* MODEM status register */
#define UART_SCR	0x1C		/* scratch register */
#define UART_AFT	0x20		/* AFC trigger level register */
#define UART_SIER	0x50		/* interrupt enable register */

#define LSR_DR		0x01
#define LSR_THRE	0x20		/* transmitter holding register empty */
#define LSR_TEMT	0x40		/* transmitter empty */

void uart_init(void);
void uart_flush_tx(int port);
void uart_flush_rx(int port);
int uart_putc(int port, char c);
void _dputc(char c);
int _dputs(const char * str);

void uart_init(void) {
	uart_flush_rx(DEBUG_UART);
}

void uart_flush_tx(int port) {
	/* wait for the last char to get out */
	while (!(uart_reg_read(port, UART_LSR) & LSR_TEMT));
}

void uart_flush_rx(int port) {
	/* empty the rx fifo */
	while (uart_reg_read(port, UART_LSR) & LSR_DR) {
		volatile char c = uart_reg_read(port, UART_RBR);
		(void)c;
	}
}

int uart_putc(int port, char c)
{
	/* wait for the last char to get out */
	while (!(uart_reg_read(port, UART_LSR) & LSR_THRE));

	uart_reg_write(port, UART_THR, c);

	return 0;
}

void _dputc(char c) {
	if(c == '\n')
		uart_putc(DEBUG_UART, '\r');
	uart_putc(DEBUG_UART, c);
}

int _dputs(const char * str) {
        while(*str != 0) {
                _dputc(*str++);
        }

        return 0;
}

