/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <serial.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/uart.h>

DECLARE_GLOBAL_DATA_PTR;

#ifndef BITCSET
#define BITCSET(X, CMASK, SMASK)	( (X) = ((((unsigned int)(X)) & ~((unsigned int)(CMASK))) | ((unsigned int)(SMASK))) )
#endif

struct uart_stat {
	unsigned long base;
	int ch;
};

static const struct uart_stat uart[] = {
	{ HwUART0_BASE, 0 },
	{ HwUART1_BASE, 1 },
	{ HwUART2_BASE, 2 },
	{ HwUART3_BASE, 3 },
	{ HwUART4_BASE, 4 },
	{ HwUART5_BASE, 5 },
	{ HwUART6_BASE, 6 },
	{ HwUART7_BASE, 7 },
};

extern struct uart_port_map_type uart_port_map[];
extern unsigned int NUM_UART_PORT;

/*
 * Table with supported baudrates (defined in config_xyz.h)
 */
static const unsigned long baudrate_table[] = CONFIG_SYS_BAUDRATE_TABLE;

void uart_set_port_mux(unsigned int ch, unsigned int port)
{
	unsigned int idx;
	PUARTPORTCFG pUARTPORTCFG = (PUARTPORTCFG)HwUART_PORTCFG_BASE;

	for (idx=0 ; idx<NUM_UART_PORT ; idx++) {
		if (uart_port_map[idx].id == port)
			break;
	}

	if (idx >= NUM_UART_PORT) {
		return;
	}

	if (ch < 4)
		BITCSET(pUARTPORTCFG->PCFG0.nREG, 0xFF<<(ch*8), port<<(ch*8));
	else if (ch < 8)
		BITCSET(pUARTPORTCFG->PCFG1.nREG, 0xFF<<((ch-4)*8), port<<((ch-4)*8));

#if defined(GPIO_CONFIG_ONLY_DEBUG)
	if (ch ==  CONFIG_DEBUG_UART_CH) {
		gpio_config(uart_port_map[idx].tx_port, uart_port_map[idx].fn_sel);	// TX
		gpio_config(uart_port_map[idx].rx_port, uart_port_map[idx].fn_sel|GPIO_PULLUP);	// RX
	}
#else
	gpio_config(uart_port_map[idx].tx_port, uart_port_map[idx].fn_sel);	// TX
	gpio_config(uart_port_map[idx].rx_port, uart_port_map[idx].fn_sel|GPIO_PULLUP);	// RX
#endif
}

static void uart_set_gpio(uint ch)
{
	PUARTPORTCFG pUARTPORTCFG = (PUARTPORTCFG)HwUART_PORTCFG_BASE;

	//Bruce, should be initialized to not used port.
	if(gd->have_console == 0){
		pUARTPORTCFG->PCFG0.nREG = 0xFFFFFFFF;
		pUARTPORTCFG->PCFG1.nREG = 0xFFFFFFFF;
	}

	switch(ch){
		case 0:
			#if defined(CONFIG_TCC_SERIAL0_PORT)
				uart_set_port_mux(0, CONFIG_TCC_SERIAL0_PORT);
			#endif
			break;
		case 1:
			#if defined(CONFIG_TCC_SERIAL1_PORT)
				uart_set_port_mux(1, CONFIG_TCC_SERIAL1_PORT);
			#endif
			break;
		case 2:
			#if defined(CONFIG_TCC_SERIAL2_PORT)
				uart_set_port_mux(2, CONFIG_TCC_SERIAL2_PORT);
			#endif
			break;
		case 3:
			#if defined(CONFIG_TCC_SERIAL3_PORT)
				uart_set_port_mux(3, CONFIG_TCC_SERIAL3_PORT);
			#endif
			break;
		case 4:
			#if defined(CONFIG_TCC_SERIAL4_PORT)
				uart_set_port_mux(4, CONFIG_TCC_SERIAL4_PORT);
			#endif
			break;
		case 5:
			#if defined(CONFIG_TCC_SERIAL5_PORT)
				uart_set_port_mux(5, CONFIG_TCC_SERIAL5_PORT);
			#endif
			break;
		case 6:
			#if defined(CONFIG_TCC_SERIAL6_PORT)
				uart_set_port_mux(6, CONFIG_TCC_SERIAL6_PORT);
			#endif
			break;
		case 7:
			#if defined(CONFIG_TCC_SERIAL7_PORT)
				uart_set_port_mux(7, CONFIG_TCC_SERIAL7_PORT);
			#endif
			break;
		default:
			break;
	}
}

static void uart_init_port(int port, uint baud)
{
	uint16_t baud_divisor = (CONFIG_TCC_SERIAL_CLK/ 16 / baud);

	uart_reg_write(port, UART_LCR, LCR_EPS | LCR_STB | LCR_WLS_8);	/* 8 data, 1 stop, no parity */
	uart_reg_write(port, UART_IER, 0);
	uart_reg_write(port, UART_LCR, LCR_BKSE | LCR_EPS | LCR_STB | LCR_WLS_8);	/* 8 data, 1 stop, no parity */
	uart_reg_write(port, UART_DLL, baud_divisor & 0xff);
	uart_reg_write(port, UART_DLM, (baud_divisor >> 8) & 0xff);
	uart_reg_write(port, UART_FCR, FCR_FIFO_EN | FCR_RXFR | FCR_TXFR | Hw4 | Hw5);
	uart_reg_write(port, UART_LCR, LCR_EPS | LCR_STB | LCR_WLS_8);	/* 8 data, 1 stop, no parity */
}

int uart_putc(int port, char c )
{
	/* wait for the last char to get out */
	while (!(uart_reg_read(port, UART_LSR) & LSR_THRE));
	uart_reg_write(port, UART_THR, c);
	return 0;
}

int uart_getc(int port, bool wait)  /* returns -1 if no data available */
{
	if (wait) {
		/* wait for data to show up in the rx fifo */
		while (!(uart_reg_read(port, UART_LSR) & LSR_DR))
			;
	} else {
		if (!(uart_reg_read(port, UART_LSR) & LSR_DR))
			return -1;
	}
	return uart_reg_read(port, UART_RBR);
}

void uart_flush_tx(int port)
{
	/* wait for the last char to get out */
	while (!(uart_reg_read(port, UART_LSR) & LSR_TEMT))
		;
}

void uart_flush_rx(int port)
{
	/* empty the rx fifo */
	while (uart_reg_read(port, UART_LSR) & LSR_DR) {
		volatile char c = uart_reg_read(port, UART_RBR);
		(void)c;
	}
}

static void serial_setbrg_dev(const int dev_index)
{
	uint baudrate = gd->baudrate;
	int i;
	if(dev_index !=  CONFIG_DEBUG_UART_CH)
	{
		switch(dev_index)
		{
			case 0:
				#if defined(CONFIG_TCC_SERIAL0_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL0_BAUDRATE;
				#endif
				break;
			case 1:
				#if defined(CONFIG_TCC_SERIAL1_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL1_BAUDRATE;
				#endif
				break;
			case 2:
				#if defined(CONFIG_TCC_SERIAL2_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL2_BAUDRATE;
				#endif
				break;
			case 3:
				#if defined(CONFIG_TCC_SERIAL3_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL3_BAUDRATE;
				#endif
				break;
			case 4:
				#if defined(CONFIG_TCC_SERIAL4_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL4_BAUDRATE;
				#endif
				break;
			case 5:
				#if defined(CONFIG_TCC_SERIAL5_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL5_BAUDRATE;
				#endif
				break;
			case 6:
				#if defined(CONFIG_TCC_SERIAL6_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL6_BAUDRATE;
				#endif
				break;
			case 7:
				#if defined(CONFIG_TCC_SERIAL7_BAUDRATE)
					baudrate = CONFIG_TCC_SERIAL7_BAUDRATE;
				#endif
				break;
			default:
				break;
		}
		printf("tccser%d baudrate : %d\n", dev_index, baudrate);
	}else{
		baudrate = gd->baudrate;
	}

	for (i = 0; i < ARRAY_SIZE(baudrate_table); ++i) {
		if (baudrate == baudrate_table[i])
			break;
	}

	if (i == ARRAY_SIZE(baudrate_table)) {
			printf("## tccser%d Baudrate %d bps not supported\n",
				dev_index,
				baudrate);
		return;
	}

	uart_init_port(dev_index, baudrate);
	uart_flush_rx(dev_index);
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 */
static int serial_init_dev(const int dev_index)
{
	if(( CONFIG_DEBUG_UART_CH == dev_index) && gd->have_console){
		debug("tccser%d baudrate : %d (Debug Uart)\n",dev_index, gd->baudrate);
		return 0;
	}
	uart_set_gpio(dev_index);
	serial_setbrg_dev(dev_index);

	return 0;
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
static int serial_getc_dev(const int dev_index)
{
	return uart_getc(dev_index, 1);
}

/*
 * Output a single byte to the serial port.
 */
static void serial_putc_dev(const char c, const int dev_index)
{
	if (c == '\n')
		uart_putc(dev_index, '\r');
	uart_putc(dev_index, c);
}

/*
 * Test whether a character is in the RX buffer
 */
static int serial_tstc_dev(const int dev_index)
{
	return (uart_reg_read(dev_index, UART_LSR) & LSR_DR) != 0;
}

static void serial_puts_dev(const char *s, const int dev_index)
{
	while(*s != 0) {
		serial_putc_dev(*s++, dev_index);
	}
}

/* Multi serial device functions */
#define DECLARE_TCC_SERIAL_FUNCTIONS(port) \
static int tcc_serial##port##_init(void) { return serial_init_dev(port); } \
static void tcc_serial##port##_setbrg(void) { serial_setbrg_dev(port); } \
static int tcc_serial##port##_getc(void) { return serial_getc_dev(port); } \
static int tcc_serial##port##_tstc(void) { return serial_tstc_dev(port); } \
static void tcc_serial##port##_putc(const char c) { serial_putc_dev(c, port); } \
static void tcc_serial##port##_puts(const char *s) { serial_puts_dev(s, port); }

#define INIT_TCC_SERIAL_STRUCTURE(port, __name) {	\
	.name	= __name,				\
	.start	= tcc_serial##port##_init,		\
	.stop	= NULL,					\
	.setbrg	= tcc_serial##port##_setbrg,		\
	.getc	= tcc_serial##port##_getc,		\
	.tstc	= tcc_serial##port##_tstc,		\
	.putc	= tcc_serial##port##_putc,		\
	.puts	= tcc_serial##port##_puts,		\
}
#if defined(CONFIG_TCC_SERIAL0_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(0);
struct serial_device tcc_serial0_device =
	INIT_TCC_SERIAL_STRUCTURE(0, "tccser0");
#endif
#if defined(CONFIG_TCC_SERIAL1_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(1);
struct serial_device tcc_serial1_device =
	INIT_TCC_SERIAL_STRUCTURE(1, "tccser1");
#endif
#if defined(CONFIG_TCC_SERIAL2_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(2);
struct serial_device tcc_serial2_device =
	INIT_TCC_SERIAL_STRUCTURE(2, "tccser2");
#endif
#if defined(CONFIG_TCC_SERIAL3_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(3);
struct serial_device tcc_serial3_device =
	INIT_TCC_SERIAL_STRUCTURE(3, "tccser3");
#endif
#if defined(CONFIG_TCC_SERIAL4_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(4);
struct serial_device tcc_serial4_device =
	INIT_TCC_SERIAL_STRUCTURE(4, "tccser4");
#endif
#if defined(CONFIG_TCC_SERIAL5_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(5);
struct serial_device tcc_serial5_device =
	INIT_TCC_SERIAL_STRUCTURE(5, "tccser5");
#endif
#if defined(CONFIG_TCC_SERIAL6_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(6);
struct serial_device tcc_serial6_device =
	INIT_TCC_SERIAL_STRUCTURE(6, "tccser6");
#endif
#if defined(CONFIG_TCC_SERIAL7_PORT)
DECLARE_TCC_SERIAL_FUNCTIONS(7);
struct serial_device tcc_serial7_device =
	INIT_TCC_SERIAL_STRUCTURE(7, "tccser7");
#endif

__weak struct serial_device *default_serial_console(void)
{
#if defined(CONFIG_TCC_SERIAL0_PORT) && ( CONFIG_DEBUG_UART_CH == 0)
	return &tcc_serial0_device;
#elif defined(CONFIG_TCC_SERIAL1_PORT) && ( CONFIG_DEBUG_UART_CH == 1)
	return &tcc_serial1_device;
#elif defined(CONFIG_TCC_SERIAL2_PORT) && ( CONFIG_DEBUG_UART_CH == 2)
	return &tcc_serial2_device;
#elif defined(CONFIG_TCC_SERIAL3_PORT) && ( CONFIG_DEBUG_UART_CH == 3)
	return &tcc_serial3_device;
#elif defined(CONFIG_TCC_SERIAL4_PORT) && ( CONFIG_DEBUG_UART_CH == 4)
	return &tcc_serial4_device;
#elif defined(CONFIG_TCC_SERIAL5_PORT) && ( CONFIG_DEBUG_UART_CH == 5)
	return &tcc_serial5_device;
#elif defined(CONFIG_TCC_SERIAL6_PORT) && ( CONFIG_DEBUG_UART_CH == 6)
	return &tcc_serial6_device;
#elif defined(CONFIG_TCC_SERIAL7_PORT) && ( CONFIG_DEBUG_UART_CH == 7)
	return &tcc_serial7_device;
#else
	#error "TCC Serial debug port configuration error"
#endif
}

void tcc_serial_initialize(void)
{
#ifdef CONFIG_TCC_SERIAL7_PORT
	serial_register(&tcc_serial7_device);
#endif
#ifdef CONFIG_TCC_SERIAL6_PORT
	serial_register(&tcc_serial6_device);
#endif
#ifdef CONFIG_TCC_SERIAL5_PORT
	serial_register(&tcc_serial5_device);
#endif
#ifdef CONFIG_TCC_SERIAL4_PORT
	serial_register(&tcc_serial4_device);
#endif
#ifdef CONFIG_TCC_SERIAL3_PORT
	serial_register(&tcc_serial3_device);
#endif
#ifdef CONFIG_TCC_SERIAL2_PORT
	serial_register(&tcc_serial2_device);
#endif
#ifdef CONFIG_TCC_SERIAL1_PORT
	serial_register(&tcc_serial1_device);
#endif
#ifdef CONFIG_TCC_SERIAL0_PORT
	serial_register(&tcc_serial0_device);
#endif
//#else
//	#error "TCC Serial Port not defined"
//#endif

}
