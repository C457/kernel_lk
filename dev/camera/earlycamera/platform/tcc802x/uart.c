
#include <reg.h>
#include <iomap.h>
#include <uart.h>

// UART Base address 
#define UART0_BASE 	(TCC_UART_BASE+0x00000)
#define UART1_BASE 	(TCC_UART_BASE+0x10000)
#define UART2_BASE 	(TCC_UART_BASE+0x20000)
#define UART3_BASE 	(TCC_UART_BASE+0x30000)
#define UART4_BASE 	(TCC_UART_BASE+0x40000)
#define UART5_BASE 	(TCC_UART_BASE+0x50000)
#define UART6_BASE 	(TCC_UART_BASE+0x60000)
#define UART7_BASE 	(TCC_UART_BASE+0x70000)
#define UARTPORTCFG 	(TCC_UART_BASE+0x84000)

// UART Register Offset
#define OFFSET_DR 		(0x00) 		// Data register 	
#define OFFSET_RSR 		(0x04) 		// Receive Status register
#define OFFSET_ECR 		(0x04) 		// Error Clear register
#define OFFSET_FR 		(0x18) 		// Flag register
#define OFFSET_ILPR 	(0x20) 		// IrDA Low-power Counter register
#define OFFSET_IBRD 	(0x24) 		// Integer Baud rate register
#define OFFSET_FBRD 	(0x28) 		// Fractional Baud rate register
#define OFFSET_LCRH 	(0x2c) 		// Line Control register
#define OFFSET_CR 		(0x30) 		// Control register
#define OFFSET_IFLS 	(0x34) 		// Interrupt FIFO Level status register
#define OFFSET_IMSC 	(0x38) 		// Interrupt Mask Set/Clear register
#define OFFSET_TRIS 	(0x3c) 		// Raw Interrupt Status register
#define OFFSET_TMIS 	(0x40) 		// Masked Interrupt Status register 
#define OFFSET_ICR 		(0x44) 		// Interrupt Clear register
#define OFFSET_DMACR 	(0x48) 		// DMA Control register

#define FR_TXFE  	(1 << 7) 	// Transmit FIFO empty
#define FR_RXFF  	(1 << 6)	// Receive FIFO full
#define FR_TXFF  	(1 << 5) 	// Transmit FIFO full
#define FR_RXFE  	(1 << 4) 	// Receive FIFO empty
#define FR_BUSY  	(1 << 3) 	// UART busy

#define LCRH_SPS             (1 << 7)
#define LCRH_WLEN(x)         ((x) << 5)
#define LCRH_FEN             (1 << 4)
#define LCRH_STP2            (1 << 3)
#define LCRH_EPS             (1 << 2)
#define LCRH_PEN             (1 << 1)
#define LCRH_BRK             (1 << 0)

enum {
	WORD_LEN_5 = 0,
	WORD_LEN_6,
	WORD_LEN_7,
	WORD_LEN_8
};

#define CR_CTSEN             (1 << 15)
#define CR_RTSEN             (1 << 14)
#define CR_OUT2              (1 << 13)
#define CR_OUT1              (1 << 12)
#define CR_RTS               (1 << 11)
#define CR_DTR               (1 << 10)
#define CR_RXE               (1 << 9)
#define CR_TXE               (1 << 8)
#define CR_LPE               (1 << 7)
#define CR_IIRLP             (1 << 2)
#define CR_SIREN             (1 << 1)
#define CR_UARTEN            (1 << 0)

struct uart_stat {
	addr_t base;
	int ch;
};

static struct uart_stat uart[] = {
	{ UART0_BASE, 0 }, // NOTE : Smart Card uses UART Controller 7 
	{ UART1_BASE, 1 },
	{ UART2_BASE, 2 },
	{ UART3_BASE, 3 },
	{ UART4_BASE, 4 },
	{ UART5_BASE, 5 },
	{ UART6_BASE, 6 },
	{ UART7_BASE, 7 },
};

#define uart_reg_write(p, a, v)	writel(v, uart[p].base + (a))
#define uart_reg_read(p, a)	readl(uart[p].base + (a))

static int __uart_putc(int port, char c)
{
	while (uart_reg_read(port, OFFSET_FR) & FR_TXFF)
		;

	uart_reg_write(port, OFFSET_DR, c);

	return 0;
}

int uart_putc(int port, char c)
{
	if (c == '\n')
		__uart_putc(port, '\r');
	return __uart_putc(port, c);
}

void _dputc(char c)
{
	uart_putc(DEBUG_UART, c);
}

int _dputs(const char *str)
{
	while(*str != 0) {
		_dputc(*str++);
	}

	return 0;
}

