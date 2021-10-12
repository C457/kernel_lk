/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#include <common.h>
#include <asm/io.h>

#include <asm/arch/globals.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/tcc_ckc.h>
#include <asm/arch/sram_map.h>
#include <asm/arch/tcc_ddr.h>
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef BITCSET
#define BITCSET(X, CMASK, SMASK)	( (X) = ((((unsigned int)(X)) & ~((unsigned int)(CMASK))) | ((unsigned int)(SMASK))) )
#endif

#define addr_clk(x) (0x74000000+x)
#define addr_mem(x) (0x73000000+x)
#define time2cycle(time, tCK)		 ((int)((time + tCK - 1) / tCK))

#ifdef round_up
#undef round_up
#endif
#define round_up(val, div)           ((unsigned)((val) + (div) - 1) / (div))

#ifdef round_down
#undef round_down
#endif
#define round_down(val, div)         ((val) / (div))

#ifdef max
#undef max
#endif
#define max(a, b)                       ((a) > (b) ? (a):(b))

#ifdef abs
#undef abs
#endif
#define abs(a)                   ((a)>(0) ? (a):(-a))

#define dmc0_ctl(x) (*(volatile unsigned long *)addr_mem(0x500000+(x*4)))
#define dmc0_phy(x) (*(volatile unsigned long *)addr_mem(0x880000+(x*4)))
#define HALT_CFG0	(*(volatile unsigned long *)(0x7382001C))
#define HALT_STS0	(*(volatile unsigned long *)(0x7382000C))
#if defined(TARGET_TCC8970_STB) || defined(TARGET_TCC8975_STB)
#define GPIO_F(x)	(*(volatile unsigned long *)(0x74200140 + (x*4)))
#endif

#define CKC_CHANGE_FUNC_ADDR        0x10003000
#define CKC_CHANGE_FUNC_SIZE        0x1000
#define CKC_CHANGE_ARG_BASE         (CKC_CHANGE_FUNC_ADDR + CKC_CHANGE_FUNC_SIZE)
#define CKC_CHANGE_ARG_SIZE         0x100
#define CKC_CHANGE_ARG(x)           (*(volatile unsigned int *)(CKC_CHANGE_ARG_BASE + (4*(x))))
#define CKC_CHANGE_STACK_TOP        0x10010000

#define PREVENT_DIGITAL_DLL_FOR_RECALIBRATION
#define BIT_LEVELING_CLOCK_CHANGE
//#define DYNAMIC_SCL
//#define FIXED_CL_CWL
//#define DRAM_REG_DUMP
#ifdef DA9062_PMIC
#define PRE_SET_PMIC
#endif

#define PLL3_VALUE	0x06025A04	// 540MHz@0.9V for CPU0

//#define PLL4_VALUE	0x060244C3	// 550Mhz Momory
//#define PLL4_VALUE	0x06026404	// 600Mhz Memory
//#define PLL4_VALUE	0x06013C04	// 720Mhz Memory
//#define PLL4_VALUE	0x06012F83	// 760Mhz Memory
//#define PLL4_VALUE	0x06013203	// 800Mhz Memory
//#define PLL4_VALUE	0x06014B04	// 900Mhz Memory
//#define PLL4_VALUE	0x06014DC4	// 933Mhz Memory
//#define PLL4_VALUE	0x06013E83	// 1000Mhz Memory

#if defined(TARGET_TCC8975_STICK)
#define PLL4_VALUE	0x06012F83	// 760Mhz Memory
#else
#define PLL4_VALUE	0x06014DC4	// 933Mhz Memory
#endif

#define PLL2_VALUE	0x06034403	// temp.. io pll. (272MHz)

#define MEMPLL_FVCO_kHz	((((PLL4_VALUE&0x0000FFC0)>>6)*24000)/(PLL4_VALUE&0x0000003F))	// FVCO=(m*FIN)/p
#define MEMPLL_kHz	(MEMPLL_FVCO_kHz>>((PLL4_VALUE&0x00070000)>>16))		// FOUT=FVCO/(2^s)
#define MEMBUS_CLK	(MEMPLL_kHz/1000)
#define DDR3_CLK	(MEMBUS_CLK)
#define tCK		(1000000/(DDR3_CLK))

#if defined(PRE_SET_PMIC)
#define EARLY_I2C_GPIO(x) (*(volatile unsigned long *)(0x74200100 + (x*4)))
#define EARLY_I2C_GPIO_N 13
#define EARLY_I2C_FN 8
#define EARLY_I2C_PORT 18
#define EARLY_I2C_MASTER(x)  (*(volatile unsigned long *)(0x76300000 + (x*4))) // i2c master #0
#define EARLY_I2C_CFG(x)	 (*(volatile unsigned long *)(0x76360000 + (x*4))) // i2c port cfg

inline static void early_i2c_enable(void)
{

	*(volatile unsigned long *)(0x76066018) &= 0xDFFFFFFF;
	*(volatile unsigned long *)(0x76066018) |= 0x20000000;

	*(volatile unsigned long *)(0x74000108) = 0x24000003; // Master #0 24Mhz XIN
}

inline static void early_i2c_port(void)
{
	EARLY_I2C_GPIO(13) &= ~0xF00000;
	EARLY_I2C_GPIO(13) |= 0x800000;
	EARLY_I2C_GPIO(13) &= ~0xF000000;
	EARLY_I2C_GPIO(13) |= 0x8000000;

	EARLY_I2C_CFG(0) = 0xFFFFFFFF;
	EARLY_I2C_CFG(1) = 0xFFFFFFFF;
	EARLY_I2C_CFG(0) &= ~(0xFF);

	*(volatile unsigned long *)(0x76066010) |= 0x20000000;
	EARLY_I2C_CFG(0) |= EARLY_I2C_PORT;
	EARLY_I2C_MASTER(0) = 2; // 400khz SCL
	EARLY_I2C_MASTER(1) = 0xC0; // Core enable, INT enable
	EARLY_I2C_MASTER(3) |= 0x1; // clear interrupt
}

inline static int early_i2c_write(unsigned char slave_addr, unsigned char out_len, unsigned char* out_buf)
{
	volatile unsigned long cnt = 0;
	volatile int i = 0;

	/* WRITE  */
	EARLY_I2C_MASTER(2) =  slave_addr | 0;
	EARLY_I2C_MASTER(3) = 0x90; // slaver addr, start bit
	while(!(EARLY_I2C_CFG(3)  & 0x1)){
		cnt++;
		if(cnt > 100000)
			return -1; // time out
	}
	EARLY_I2C_MASTER(3) |= 0x1; // clear interrupt

	for(i=0; i<out_len; i++){
		EARLY_I2C_MASTER(2) = out_buf[i];
		EARLY_I2C_MASTER(3) = 0x10;

		while(!(EARLY_I2C_CFG(3)  & 0x1)){
			cnt++;
			if(cnt > 100000)
				return -1; // time out
		}
		EARLY_I2C_MASTER(3) |= 0x1; // clear interrupt
	}

	EARLY_I2C_MASTER(3) = 0x40; // stop bit
	while(!(EARLY_I2C_CFG(3)  & 0x1)){
		cnt++;
		if(cnt > 100000)
			return -1; // time out
	}
	EARLY_I2C_MASTER(3) |= 0x1; // clear interrupt

	return 0;
}



inline static void pmic_write(unsigned char cmd, unsigned char value)
{
	unsigned char send_data[2];
	send_data[0] = cmd;
	send_data[1] = value;
	early_i2c_write(0xb0, 2, send_data);
}


inline static void set_core_voltage(int value)
{
	volatile int i;
	pmic_write(0xA7 , (value-800)/20);
	pmic_write(0xA0, 0xE0);
	i=64000; while(i--);
}

#endif

#if defined(DRAM_REG_DUMP)
// Example, TCC8970_EVM UART TX GPIO 22 ----------------------------------
#define EARLY_UART_GPIO(x) (*(volatile unsigned long *)(0x74200140 + (x*4))) //
#define EARLY_UART_GPIO_N 22
#define EARLY_UART_FN 9
#define EARLY_UART_PORT 22
// ----------------------------------------------------------------------
#define EARLY_UART 0x76370000 // UART0
#define EARLY_UART_PCLK 0x74000118 //UART0 PCLK
#define EARLY_UART_REG(x) (*(volatile unsigned long *)(EARLY_UART + (x)))
#define EARLY_UART_CFG (*(volatile unsigned long *)(0x763F0000))
inline static void uart_set_gpio(void)
{
	int fn_gpio_offset;
	fn_gpio_offset = EARLY_UART_GPIO_N%8;

	if(EARLY_UART_GPIO_N < 8){
		EARLY_UART_GPIO(12) &= ~(0xf<<(fn_gpio_offset*4));
		EARLY_UART_GPIO(12) |= (EARLY_UART_FN<<(fn_gpio_offset*4));
	}
	else if(EARLY_UART_GPIO_N < 16){
		EARLY_UART_GPIO(13) &= ~(0xf<<(fn_gpio_offset*4));
		EARLY_UART_GPIO(13) |= (EARLY_UART_FN<<(fn_gpio_offset*4));
	}
	else if(EARLY_UART_GPIO_N < 24){
		EARLY_UART_GPIO(14) &= ~(0xf<<(fn_gpio_offset*4));
		EARLY_UART_GPIO(14) |= (EARLY_UART_FN<<(fn_gpio_offset*4));
	}
	else if(EARLY_UART_GPIO_N < 32){
		EARLY_UART_GPIO(15) &= ~(0xf<<(fn_gpio_offset*4));
		EARLY_UART_GPIO(15) |= (EARLY_UART_FN<<(fn_gpio_offset*4));
	}
	EARLY_UART_CFG &= 0xFF;
	EARLY_UART_CFG |= EARLY_UART_PORT;

}
inline static void uart_set_clock(void)
{
	*(volatile unsigned long *)(EARLY_UART_PCLK) = 0x24000000; // uart0 clock 24Mhz(XIN)
}
inline static void uart_flush_buffer(void)
{
	volatile char temp;
	while(EARLY_UART_REG(0x14) & 0x01){
		temp = EARLY_UART_REG(0x0);
		(void)temp;
	}
}
inline static uart_set_baudrate(int baud)
{
	uint16_t baud_divisor = (24000000/*uart clock*// 16 / baud);

	EARLY_UART_REG(0xC) =  0x10 | 0x4 | 0x3;	/* 8 data, 1 stop, no parity */
	EARLY_UART_REG(0x4) =  0;
	EARLY_UART_REG(0xC) =  0x80 | 0x10 | 0x4 | 0x3;	/* 8 data, 1 stop, no parity */
	EARLY_UART_REG(0x0) =  baud_divisor & 0xff;
	EARLY_UART_REG(0x4) =  (baud_divisor >> 8) & 0xff;
	EARLY_UART_REG(0x8) =  0x1 | 0x2 | 0x4 | 0x10 | 0x20;
	EARLY_UART_REG(0xC) =  0x10 | 0x4 | 0x3;	/* 8 data, 1 stop, no parity */
}
inline static void uart_put_char(char x)
{
	while(!(EARLY_UART_REG(0x14) & 0x20))
		;

	EARLY_UART_REG(0x0) = x;
}

inline static void uart_print_reg(unsigned int reg)
{
	volatile int i=0;
	char temp;

	for(i=0; i<8; i++)
	{
		temp = (char)((reg>>((7-i)*4)) & 0xF);
		if(temp < 10) temp = temp + 0x30;
		else temp = temp + 0x37;
		uart_put_char(temp);
	}
}
inline static void uart_dump_reg(void __iomem *reg, int width)
{
	volatile int i=0;

	for(i=0; i<width; i++)
	{
		if(i%4 == 0){
			uart_put_char('0');
			uart_put_char('x');
			uart_print_reg(reg+i*4);
			uart_put_char(' ');
		}
		uart_put_char('0');
		uart_put_char('x');
		uart_print_reg(readl(reg + i*4));
		uart_put_char(' ');
		if((i+1)%4 == 0) {uart_put_char('\r'); uart_put_char('\n'); }
	}

}
#endif


void ddr_self_refresh_enter(unsigned int shutdown);
void ddr_self_refresh_exit(void);

inline static void ddr_setpll(void __iomem *reg, unsigned value)
{
	unsigned int i;
	if (value & (1<<31)) {
		writel((value&0x7FFFFFFF)|(1<<26), reg);
		for (i=100 ; i ; i--);
		writel(readl(reg) | (1<<31), reg);
		for (i=100 ; i ; i--)
			while ((readl(reg) & (1<<26)) == 0);
	} else
		writel(readl(reg) & ~(1<<31), reg);
}

inline static void ddr_setfbus(void __iomem *reg, unsigned value)
{
	writel(value, reg);
	while(readl(reg) & (1<<29));
}

inline static void ddr_clock_set(void)
{
	volatile int i;

	/* Change CLKCTRL soruce to XIN */
	writel(0x00001F00, addr_clk(0x000000));		// CPU (Cortex-A7)
	writel(0x00001F00, addr_clk(0x000008));		// Memory Bus
	writel(0x00200014, addr_clk(0x000014));		// io bus
	writel(0x00200014, addr_clk(0x000024));		// smu bus
	writel(0x00200014, addr_clk(0x00002C));		// cortex-M4
#if !defined(TSBM_ENABLE)
	/* enable hsio block */
	writel(0x00000001, (void __iomem *)addr_clk(0x400100));           // pmu pwrup_hsbus
	while((readl((void __iomem *)addr_clk(0x400000))&0x1) == 0);       // check pwrsts0 0-bit
	writel(readl((void __iomem *)addr_clk(0x400010))|(1<<31), \
			(void __iomem *)addr_clk(0x400010));     // pmu_sysrst
	writel(readl((void __iomem *)addr_mem(0x810000))|(1<<29), \
			(void __iomem *)addr_mem(0x810000));  // membus_hclk
	writel(readl((void __iomem *)addr_mem(0x810004))|(1<<12), \
			(void __iomem *)addr_mem(0x810004));  // membus_rst
	writel(0x00200014, (void __iomem *)addr_clk(0x000020));           // hsio bus
#endif
	/* PLLs */
	ddr_setpll((void __iomem *)addr_clk(0x00004C), (1<<31)|PLL3_VALUE); // for cpu
	ddr_setpll((void __iomem *)addr_clk(0x000050), (1<<31)|PLL4_VALUE); // for mem
	ddr_setpll((void __iomem *)addr_clk(0x000048), (1<<31)|PLL2_VALUE);
	i=3200; while(i--);

	ddr_setfbus((void __iomem *)addr_clk(0x000000), 0x00001F09);	// CPU0 (Cortex-A7)
	ddr_setfbus((void __iomem *)addr_clk(0x000008), 0x00001F01);	// Memory Bus
	ddr_setfbus((void __iomem *)addr_clk(0x000014), 0x00200012);	// io bus
#if !defined(TSBM_ENABLE)
	ddr_setfbus((void __iomem *)addr_clk(0x000020), 0x00200012);    // hsio bus
#endif
	ddr_setfbus((void __iomem *)addr_clk(0x000024), 0x00200012);	// smu bus
	ddr_setfbus((void __iomem *)addr_clk(0x00002C), 0x00200012);	// cortex-M4
	i=3200; while(i--);

	writel(0x24000000, addr_clk(0x0000D4));		// sdmmc0 peri (24MHz)	// for emmcboot
	writel(0x24000000, addr_clk(0x0000D8));		// sdmmc1 peri (24MHz)	// for emmcboot
	writel(0x24000000, addr_clk(0x0000DC));		// sdmmc2 peri (24MHz)	// for emmcboot
	writel(0x24000000, addr_clk(0x0000E0));		// sdmmc3 peri (24MHz)	// for emmcboot
	i=3200; while(i--);
}

inline static void  HOST_CMD_LOAD0(unsigned a, unsigned b, unsigned c, unsigned d)
{
	(*(volatile unsigned long *)(0x73500080+a*4))=c;
	(*(volatile unsigned long *)(0x735000C0+a*4))=((max(d,1)-1)<<12)|b;
}

#define ret_mr1(wl, rtt_nom, dic) ((((wl)      & 0x1)   << 7) | \
                                   (((rtt_nom) & 0x4)   << 7) | \
                                   (((rtt_nom) & 0x2)   << 5) | \
                                   (((rtt_nom) & 0x1)   << 2) | \
                                   (((dic)     & 0x2)   << 4) | \
                                   (((dic)     & 0x1)   << 1))

#define ret_mr2(cwl, rtt_wr)  ((((rtt_wr)  & 0x3)   << 9) | \
                               ((((cwl)-5) & 0x7)   << 3))

inline static unsigned
wr2mr (unsigned n)
{
  unsigned wr_val = 0;
  switch (n)
    {
    case 5:
    case 6:
    case 7:
    case 8:
      wr_val = n - 4;
      break;
    case 9:
    case 10:
      wr_val = 5;
      break;
    case 11:
    case 12:
      wr_val = 6;
      break;
    case 13:
    case 14:
      wr_val = 7;
      break;
    case 15:
    case 16:
      wr_val = 0;
      break;
    default:
      wr_val = 0;
      break;
    }
  return wr_val;
}

inline static unsigned
cl2mr (unsigned n)
{
  unsigned cl;

  if (n > 11) cl = (((n - 12) << 2) | 1);
  else        cl = (((n -   4) << 2) | 0);

  return cl;
}

#define ret_mr0(wr, dll_rst, cl)      ((1<<12)|(wr2mr(wr)<<9)|(dll_rst<<8)|(cl2mr(cl)<<2))

#if defined(CONFIG_DDR3_2CS)
#define DRAM_CS_MASK (((1<<MEM_CS_BITS)-1)<<3)
#else
#define DRAM_CS_MASK (0)
#endif

#define address_mapping(mode) do                                      \
{                                                                     \
  const unsigned MEM_CS_BITS = 1;                                     \
  const unsigned COL_BITS_LOW = 3;                                    \
  const unsigned COL_BITS_HIGH = DDR3_COLBITS - COL_BITS_LOW;             \
                                                                      \
  col_addr_high_mask    = ((1 << COL_BITS_HIGH) - 1) << COL_BITS_LOW; \
  col_addr_low_mask     = ((1<<COL_BITS_LOW)-1);                          \
  bank_addr_high_rshift = 0;                                          \
  bank_addr_high_mask   = 0;                                          \
  cs_addr_mask          = DRAM_CS_MASK;         \
  bank_addr_mask        = ((1 << DDR3_BANKBITS) -1);                        \
  row_addr_mask         = ((1 << DDR3_ROWBITS) - 1);                      \
                                                                      \
  if (mode == 0)                                                      \
    {                                                                 \
      /* CS + ROW + BA + COL */                                       \
      col_addr_high_rshift  = COL_BITS_LOW - COL_BITS_LOW;            \
      cs_addr_rshift        = DDR3_ROWBITS + DDR3_COLBITS;                    \
      bank_addr_rshift      = DDR3_COLBITS;                               \
      row_addr_rshift       = DDR3_COLBITS + DDR3_BANKBITS;                     \
    }                                                                 \
  else if (mode == 2)                                                 \
    {                                                                 \
      /* ROW + COL_HIGH + BA + CS + COL_LOW*/                         \
      col_addr_high_rshift  = DDR3_BANKBITS + MEM_CS_BITS + COL_BITS_LOW - COL_BITS_LOW; \
      cs_addr_rshift          = COL_BITS_LOW - 3;                     \
      bank_addr_rshift      = MEM_CS_BITS + COL_BITS_LOW;             \
      row_addr_rshift       = DDR3_COLBITS + DDR3_BANKBITS + MEM_CS_BITS;       \
    }                                                                 \
 else if (mode == 3)                                                  \
   {                                                                  \
     /* CS + BA + ROW + COL */                                        \
     col_addr_high_rshift  = COL_BITS_LOW - COL_BITS_LOW;             \
     cs_addr_rshift       = DDR3_BANKBITS + DDR3_ROWBITS + DDR3_COLBITS - 3;        \
     bank_addr_rshift      = DDR3_ROWBITS + DDR3_COLBITS;                     \
     row_addr_rshift       = DDR3_COLBITS;                                \
   }                                                                  \
 else                                                                 \
   {                                                                  \
     /* CS + ROW + COL_HIGH + BA + COL_LOW */                                        \
     col_addr_high_rshift  = DDR3_BANKBITS + COL_BITS_LOW - COL_BITS_LOW;   \
     cs_addr_rshift        = DDR3_ROWBITS + DDR3_COLBITS + DDR3_BANKBITS - 3;       \
     bank_addr_rshift      = COL_BITS_LOW;                            \
     row_addr_rshift       = DDR3_COLBITS + DDR3_BANKBITS;                      \
   }                                                                  \
}                                                                     \
while (0)

/*===========================================================================
TYPE
===========================================================================*/
#if defined(DRAM_DDR3)
enum{
	PLL_VALUE = 0,
	CKC_CTRL_VALUE,
	CLK_RATE,
	DMC_AH,
	DMC_CH,
	DMC_DH,
	DMC_EH,
	DMC_FH,
	DMC_10H,
	DMC_11H,
	DMC_15H,
	DMC_20H,
	DMC_21H,
	DMC_22H,
	DMC_23H,
	DMC_24H,
	DMC_30H,
	DMC_31H,
	DMC_32H,
	DMC_33H,
	DMC_34H,
	DMC_46H,
	DMC_47H,
	DMC_54H,
	DMC_6FH
};
#else
	#error "not selected ddr type.."
#endif

inline static void unq_set_ip_dq_trim(int channel)
{
	unsigned n, b;
	unsigned val;

	for(n=0 ; n<4 ; n++)
	{
		for(b=0 ; b<8 ; b++)
		{
			if(channel == 0)
				dmc0_phy(0x4B) = (b<<8)|((BITLVL_DLY_WIDTH+1)*n);

			val = abs(IP_DQ_BITWISE_TRIM)&0x3F;
			if(IP_DQ_BITWISE_TRIM>0)
				val = ((1<<BITLVL_DLY_WIDTH))|val;

			if(channel == 0)
				dmc0_phy(0x65) = val;
		}
	}
}

inline static void unq_set_ip_dqs_trim(int channel)
{
	unsigned n, b;
	unsigned val;

	for(n=0 ; n<4 ; n++)
	{
		b = 8;
		if(channel == 0)
			dmc0_phy(0x4B) = (b<<8)|((BITLVL_DLY_WIDTH+1)*n);

		val = abs(IP_DQS_BITWISE_TRIM)&0x3F;
		if(IP_DQS_BITWISE_TRIM>0)
			val = ((1<<BITLVL_DLY_WIDTH))|val;

		if(channel == 0)
			dmc0_phy(0x65) = val;

	}
}

inline static void unq_set_op_dq_trim(int channel)
{
	unsigned n, b;
	unsigned val;

	for(n=0 ; n<4 ; n++)
	{
		for(b=0 ; b<8 ; b++)
		{
			if(channel == 0)
				dmc0_phy(0x4B) = (b<<8)|((BITLVL_DLY_WIDTH+1)*n);

			val = abs(OP_DQ_BITWISE_TRIM)&0x3F;
			if(OP_DQ_BITWISE_TRIM>0)
				val = ((1<<BITLVL_DLY_WIDTH))|val;

			if(channel == 0)
				dmc0_phy(0x68) = val;
		}
	}

	for(n=0 ; n<4 ; n++)
	{
		b = 8;
		if(channel == 0)
			dmc0_phy(0x4B) = (b<<8)|((BITLVL_DLY_WIDTH+1)*n);

		val = abs(OP_DM_BITWISE_TRIM)&0x3F;
		if(OP_DM_BITWISE_TRIM>0)
			val = ((1<<BITLVL_DLY_WIDTH))|val;

		if(channel == 0)
			dmc0_phy(0x68) = val;

	}
}

inline static void unq_set_op_dqs_trim(int channel)
{
	unsigned n, b;
	unsigned val;

	for(n=0 ; n<4 ; n++)
	{
		b = 8;
		if(channel == 0)
			dmc0_phy(0x4B) = (b<<8)|((BITLVL_DLY_WIDTH+1)*n);

		val = abs(OP_DQS_BITWISE_TRIM)&0x3F;
		if(OP_DQS_BITWISE_TRIM>0)
			val = ((1<<BITLVL_DLY_WIDTH))|val;

		if(channel == 0)
			dmc0_phy(0x68) = val;

	}
}

/****************************************************************************************
* FUNCTION :void sdram_init(void)
* DESCRIPTION :
* ***************************************************************************************/
inline static void sdram_init(int no_reset)
{
	volatile int i;
	uint nCL, nCWL;
	unsigned wait_cycle;
	unsigned temp;

//--------------------------------------------------------------------------
// Timing Parameter

	if(tCK >= 2500){ /* 2.5ns, 400MHz */
		nCL = 9; // uniquify guide
		nCWL = 6; // uniquify guide
	}else if(tCK >= 1875){ // 1.875ns, 533..MHz
		nCL = 9; // uniquify guide
		nCWL = 6;
	}else if(tCK >= 1500){ // 1.5 ns, 666..MHz
#if defined(CONFIG_DDR3_1600)
			nCL = 9;
#else
			nCL = 10;
#endif
			nCWL = 7;
	}else if(tCK >= 1250){ // 1.25 ns, 800MHz
		nCL = 11;
		nCWL = 8;
	}else if(tCK >= 1070){ // 1.07 ns, 933..MHz
		nCL = 13;
		nCWL = 9;
	}else if(tCK >= 935){ // 0.935 ns, 1066..MHz
		nCL = 14;
		nCWL = 10;
	}

	wait_cycle = HALF_RATE_MODE ? 2*tCK : tCK;

#if defined(FIXED_CL_CWL)
	nCL = 9;
	nCWL = 7;
#endif

// PHY Setting
	dmc0_phy(0x5C) = 0x1; // UNIQUIFY_IO_1
	while(!(dmc0_phy(0x5c)&0x2));
	dmc0_phy(0x5C) = 0x5;
	dmc0_phy(0x5C) = 0x1;

	temp = dmc0_phy(0x5C);

	for(i=0 ; i<MEM_STRB_WIDTH ; i++)
	{
		dmc0_phy(0x4B) = SLV_DLY_WIDTH*i;

		temp = dmc0_phy(0x59);
		if(TRIM1 < 0)
			temp = temp & ~(1<<i);
		else
			temp = temp | (1<<i);
		dmc0_phy(0x59) = temp;
		dmc0_phy(0x4c) = abs(TRIM1);

		temp = dmc0_phy(0x5A);
		if(TRIM3 < 0)
			temp = temp & ~(1<<i);
		else
			temp = temp | (1<<i);
		dmc0_phy(0x5A) = temp;
		dmc0_phy(0x4E) = abs(TRIM3);

	}

	dmc0_phy(0x6D) = (0<<20)|(0<<16)|1; // SCL_WINDOW_TRIM
	dmc0_phy(0x49) = ((0xA)<<28)|0x1000; // PHY_DLL_RECALIB
	dmc0_phy(0x57) = 0; //0x01000000; // UNQ_ANALOG_DLL_1


	dmc0_phy(0x48) = (0<<31)|(PREAMBLE_DLY<<29)|(1<<28)|(0<<24)|(0xB<<20)|(0xB<<16)|(0<<15)|(HALF_RATE_MODE<<14)|(0<<9)|(EXTRA_ONE_CLK<<8)|(MEMCTRL_DDS<<4)|MEMCTRL_TERM;
	// PHY_PAD_CTRL
	// reset_en_state, preamble_dly, receiver_en, clk_adjust_phy,clk_drive,adrctrl_drive,ddr1p2,half_rate,no_external_dll,extra_oen_clk,dq_dqs_drive,io_mode,odt_en,odt_sel

	dmc0_phy(0x46) = (1<<24)|(0<<16)|(0<<12)|(round_up(nCL,2)<<4)|(0<<3)|(1<<2)|(0<<1)|1;
	// SCL_CONFIG_1
	// ddr_odt_ctl_wr, ddr_odt_ctrl_rd, local_odt_ctrl, rd_cas_latency, dly_dfi_phyupd_ack, ddr3, ddr2, burst8

	dmc0_phy(0x47) = (SWAP_PHASE<<31)|(0<<30)|(0<<29)|(0<<28)|(1<<25)|((HALF_RATE_MODE && (((nCWL%2==0)?1:0)^SWAP_PHASE))<<24)|(0<<12)|(round_up(nCWL,2)<<8)|1;
	// SCL_CONFIG_2
	// swap_phase,rdfifo_enable,lpddr2,analog_dll_for_scl,scl_step,size,dly_dfi_wrdata,double_ref_dly,wr_cas,latency,scl_test_cs

	dmc0_phy(0x6F) = ((nCWL%2==0) && SWAP_PHASE);
	// SCL_CONFIG_4

	dmc0_phy(0x54) = (((RTT_WR<<9)|((nCWL-5)<<3))<<16)|(((nCWL-5)<<3));
	// WRLVL_DYN_ODT
	// dynamic_odt_on, dynamic_odt_off

	dmc0_phy(0x5B) = (~SCL_LANES&((1<<MEM_STRB_WIDTH)-1));
	// SCL_CONFIG_3

	{
		unsigned READ_TO_WRITE = 7;
		unsigned WRITE_TO_READ = 15;
		unsigned UNIQUIFY_NUM_STAGES_A2D =2;

		unsigned PHY_BL = 2;
		unsigned TRUE_RL = round_up(nCL, 2);
		unsigned TRUE_WL = round_up(nCWL, 2);
		unsigned CMD_TO_DDR = SWAP_PHASE ? 1 :2;

		unsigned TRIM_LAT = (READ_TO_WRITE+1+TRUE_RL-TRUE_WL + WRITE_TO_READ+1 + PHY_BL*2 - 2);
		unsigned READ_LAT = (READ_TO_WRITE+1+TRUE_RL-TRUE_WL + WRITE_TO_READ+1 + PHY_BL*2 + PHY_BL/2 + CMD_TO_DDR
		                               +TRUE_RL -2 -UNIQUIFY_NUM_STAGES_A2D +1);

		unsigned bit_lvl_wr_side_read_lat = (READ_LAT);
		unsigned bit_lvl_wr_side_trim_lat = (TRIM_LAT);
		unsigned dynamic_write_bit_leveling = 0;

		dmc0_phy(0x70) =  (bit_lvl_wr_side_read_lat<<12)|(bit_lvl_wr_side_trim_lat<<4)|dynamic_write_bit_leveling;
		// DYNAMIC_WRITE_BIT_LVL
	}

	{
		unsigned wr_lvl_on = (1<<7)|(((RTT_NOM&0x4)<<7)|((RTT_NOM&0x2)<<5)|((RTT_NOM&0x1)<<2))|(((DIC&0x2)<<4)|((DIC&0x1)<<1));
		unsigned wr_lvl_off = (((RTT_NOM&0x4)<<7)|((RTT_NOM&0x2)<<5)|((RTT_NOM&0x1)<<2))|(((DIC&0x2)<<4)|((DIC&0x1)<<1));

		dmc0_phy(0x55) = (wr_lvl_on<<16)|(wr_lvl_off<<0);
		//WRLVL_ON_OFF
	}


	{
		unsigned mas_dll_dly = 0;
		unsigned mas_dll_dly_by_2 = 0;

		while(mas_dll_dly ==0)
		{
			mas_dll_dly = dmc0_phy(0x4A);
			mas_dll_dly = mas_dll_dly >> 24;
		}

		mas_dll_dly = dmc0_phy(0x4A);
		mas_dll_dly_by_2 = mas_dll_dly >> 25;
		mas_dll_dly = mas_dll_dly >> 26;

		if(DLLS_TRIM_ADRCTRL_UPD == 1)
		{
			unsigned val;
			val = abs(DLLS_TRIM_ADRCTRL);
			if(DLLS_TRIM_ADRCTRL > 0)
				val = val | (1<<9);
			else
				val = val & (~(1<<9));

			dmc0_phy(0x4A) = val;
		}
		else
		{
			if(DLLS_TRIM_ADRCTRL_UPD == 2)
				dmc0_phy(0x4A) = 0x200 | mas_dll_dly_by_2;
			else
				dmc0_phy(0x4A) = 0x200 | mas_dll_dly;
		}


		for(i=0 ; i<MEM_CLOCKS ; i++)
		{
			dmc0_phy(0x4B) = i*(SLV_DLY_WIDTH+1);
			if(DLLS_TRIM_UPD == 1)
			{
				unsigned val;
				val = abs(DLLS_TRIM);
				if(DLLS_TRIM < 0)
					val = val & ~(1<<6);
				else
					val = val | (1<<6);

				dmc0_phy(0x69) = val;
			}
			else
			{
 				if(DLLS_TRIM_UPD == 2)
					dmc0_phy(0x69) = (1<<SLV_DLY_WIDTH) | mas_dll_dly_by_2;
				else
					dmc0_phy(0x69) = (1<<SLV_DLY_WIDTH) | mas_dll_dly;
			}

		}



		if(DLLS_TRIM_ADRCTRL_MA_UPD == 1)
		{
			unsigned val;
			val = abs(DLLS_TRIM_ADRCTRL_MA);
			if(DLLS_TRIM_ADRCTRL_MA > 0)
				val = val | (1<<27);
			else
				val = val & ~(1<<27);

			dmc0_phy(0x49) = MASTER_DLL | val;
		}
		else
		{
			if(DLLS_TRIM_ADRCTRL_MA_UPD == 2)
				dmc0_phy(0x49) = MASTER_DLL | mas_dll_dly_by_2|(1<<27);
			else
				dmc0_phy(0x49) = MASTER_DLL | mas_dll_dly|(1<<27);
		}

	}


	dmc0_phy(0x43) = (SCL_WAIT_LATENCY<<12)|(ANALOG_DLL_FOR_SCL<<9)|(0<<8)|(MAIN_CLK_DLY<<4)|6;
	// SCL_LATENCY

	if(IP_DQS_TRIM_UPD)
		unq_set_ip_dqs_trim(0);
	if(IP_DQ_TRIM_UPD)
		unq_set_ip_dq_trim(0);
	if(OP_DQ_TRIM_UPD)
		unq_set_op_dq_trim(0);
	if(OP_DQS_TRIM_UPD)
		unq_set_op_dqs_trim(0);

// Controller set init value

	dmc0_ctl(0x14) = (1<<6)|(0<<4)|(HALF_RATE_MODE<<3)|DRAM_MEM_WIDTH;

	for(i=0 ; i<NUM_AGENTS ; i++)
	{
		dmc0_ctl(0x0) = i*MIN_CMDACPT_WIDTH;
		dmc0_ctl(0x3) = 4;
	}

// address mapping
	{
		unsigned en_auto_prech = 0;
		unsigned col_addr_high_rshift;
		unsigned cs_addr_rshift;
		unsigned bank_addr_rshift;
		unsigned row_addr_rshift;

		unsigned row_addr_mask;
		unsigned dfi_init_start = 0;
		unsigned dfi_dram_clk_disable = 0;

		unsigned aprech_bit_pos = 10;
		unsigned col_addr_high_mask;

		unsigned col_addr_low_mask;

		unsigned bank_addr_high_rshift;
		unsigned bank_addr_high_mask;
		unsigned cs_addr_mask;
		unsigned bank_addr_mask;

		address_mapping(0);

		dmc0_ctl(0x5) = (en_auto_prech<<30)|(0<<29)|(col_addr_high_rshift<<24)|(0<<21)|(cs_addr_rshift<<16)|(0<<13)|(bank_addr_rshift<<8)|(0<<5)|row_addr_rshift;
		// MEM_CONFIG_1
		dmc0_ctl(0x6) = (dfi_dram_clk_disable<<21)|(dfi_init_start<<20)|(row_addr_mask<<0);
		// MEM_CONFIG_2
		dmc0_ctl(0x7) = (aprech_bit_pos<<16)|(col_addr_high_mask);
		// MEM_CONFIG_3
		dmc0_ctl(0x8)  = col_addr_low_mask;
		// MEM_CONFIG_4
		dmc0_ctl(0x9) = (bank_addr_high_rshift<<24)|(bank_addr_high_mask<<16)|(cs_addr_mask<<8)|(bank_addr_mask);
		// MEM_CONFIG_5
	}

	dmc0_ctl(0xB) = (0<<22)|(17<<12)|(0x3f<<4)|(0<<3)|(0<<2)|(0<<1)|0;
	// PWR_SAVE_ECC_CONFIG


// set timing value
	{
		unsigned rmw_dly, rd2wr_dly, wr2prech_dly, wr2rd_dly, wr2rd_csc_dly, nxt_dt_av_dly;

		unsigned wr2wr_csc_dly;
		unsigned wr2wr_dly    ;
		unsigned rd2rd_csc_dly;
		unsigned rd2rd_dly      ;

		unsigned sref_exit;

		int tRCD, tXS, tXP, tCKE, tCKESR;
		int RL, WL;


		RL = HALF_RATE_MODE ? round_up (nCL, 2)        : nCL;
		WL = HALF_RATE_MODE ? round_up ((nCWL + 1), 2) : nCWL;

		rmw_dly        = RL+C2D+MAX_LATENCY-WL+4+(ECC_DLY|HALF_RATE_SUPPORT ? 1 : 0);

		if (HALF_RATE_MODE)
		{
		      rd2wr_dly     = BL + 2 + round_down((nCL - nCWL), 2);
		      wr2wr_csc_dly = BL;
		      wr2wr_dly     = BL-1;
		      rd2rd_csc_dly = BL+1;
		      rd2rd_dly     = BL-1;
		      wr2rd_csc_dly = max((WL+BL) + max(round_up(DDR3_tWTR_ps, 2*tCK), round_up(DDR3_tWTR_ck, 2)) - 1, 3);
		      wr2rd_dly     = (WL+BL) + max(round_up(DDR3_tWTR_ps, 2*tCK), round_up(DDR3_tWTR_ck, 2));
		      wr2prech_dly  = round_up(DDR3_tWR_ps, 2*tCK) + (WL+BL);
		}
		else
		{
		      rd2wr_dly     = BL + 3 - 1 + RL - WL;
		      wr2wr_csc_dly = BL+1;
		      wr2wr_dly     = BL-1;
		      rd2rd_csc_dly = BL+2;
		      rd2rd_dly     = BL-1;
		      wr2rd_csc_dly = max((WL+BL)+max(round_up(DDR3_tWTR_ps, 2*tCK), DDR3_tWTR_ck) - BL, 4);
		      wr2rd_dly     = (WL+BL) + max(round_up(DDR3_tWTR_ps, 2*tCK), DDR3_tWTR_ck);
		      wr2prech_dly  = round_up(DDR3_tWR_ps, 2*tCK) + (WL+max(BL,3));
		}
		nxt_dt_av_dly = 0;

		if ((round_up(DDR3_tRCD_ps, tCK) % 2) == 0 && !SWAP_PHASE && HALF_RATE_MODE)
		{
			tRCD = round_up(DDR3_tRCD_ps, 2*tCK);
		}
		else
		{
			tRCD = round_up(DDR3_tRCD_ps, 2*tCK) - 1;
		}

		tXS       = AUTO_ZQC_ENABLE ? max(5, round_up((DDR3_tRFC_ps+10000), 2*tCK)) : 0;
		tCKE      = max(DDR3_tCKE_ck, round_up(DDR3_tCKE_ps, 2*tCK));
		tCKESR    = max(DDR3_tCKESR_ck, round_up(DDR3_tCKE_ps, 2*tCK)+1);
		tXP       = max(max(DDR3_tXP_ck, round_up(DDR3_tXP_ps, 2*tCK)), tCKE);
		sref_exit = HALF_RATE_MODE ? max(round_up(DDR3_tDLLK_ck, 2) - tXS, round_up(DDR3_tZQOPER_ck, 2))-2 :
		                               max(DDR3_tDLLK_ck               -tXS, DDR3_tZQOPER_ck             )-2 ;


		dmc0_ctl(0xA) = (1<<28)|(8<<24)|((round_up(DDR3_tRFC_ps, 2*tCK)-2)<<16)|(round_up(DDR3_tREFI_ps, 2*tCK)-2);
		//REF_CONFIG

		dmc0_ctl(0xC) = (rmw_dly<<28)|(rd2wr_dly<<24)|(wr2wr_csc_dly<<20)|(wr2wr_dly<<16)|(rd2rd_csc_dly<<12)|(rd2rd_dly<<8)|(wr2rd_csc_dly<<4)|wr2rd_dly;
		// DLY_CONFIG_1

		dmc0_ctl(0xD) = (((sref_exit & (1<<8))>>8)<<30)|
			                    (((nxt_dt_av_dly & (1<<4))>>4)<<29)|
			                    (((wr2rd_csc_dly & (1<<4))>>4)<<28)|
			                    (((wr2rd_dly & (1<<4))>>4)<<27)|
			                    (((wr2prech_dly & (1<<4))>>4)<<26)|
			                    (((tXP-1))<<20)|
			                    (((tCKE-1))<<16)|
			                    (((sref_exit))<<8)|
			                    (((tCKESR-1)) <<0);
		// DLY_CONFIG_2

		{
			unsigned rd_burst_end_dly = RL+3+(NATIVE_BURST8 ? 2:0);
			unsigned prech2ras_dly    = round_up(DDR3_tRP_ps, 2*tCK)-1;
			unsigned ras2cas_dly      = tRCD;
			unsigned ras2ras_dly      = max(round_up(DDR3_tRRD_ps, 2*tCK), round_up(DDR3_tRRD_ck, 2)) - 1;
			//unsigned wr2prech_dly     = ret4(wr2prech_dly);
			unsigned rd2prech_dly     = (HALF_RATE_MODE ? (max(round_up(DDR3_tRTP_ps, 2*tCK), round_up(DDR3_tRTP_ck, 2)) - 1) :
			                                 (max(round_up(DDR3_tRTP_ps, 2*tCK), DDR3_tRTP_ck) - 1));
			unsigned prech_all_dly    = round_up(DDR3_tRP_ps, 2*tCK);

			dmc0_ctl(0xE) = (rd_burst_end_dly<<28)|(prech2ras_dly<<24)|(ras2cas_dly<<20)|(ras2ras_dly<<16)|(wr2prech_dly<<12)|(rd2prech_dly<<8)|(prech_all_dly<<0);
			// DLY_CONFIG_3
  		}

		{
			unsigned zqc_dly = (HALF_RATE_MODE ? (ZQC_SHORT ? round_up(DDR3_tZQCS_ck, 2)-1 : round_up(DDR3_tZQOPER_ck, 2)-1) :
			                        (ZQC_SHORT ? DDR3_tZQCS_ck-1 : DDR3_tZQOPER_ck-1));
			unsigned tfaw_dly = round_up(DDR3_tFAW_ps, 2*tCK)-1;
			unsigned ras2ras_same_bank_dly = round_up(DDR3_tRC_ps, 2*tCK)-1;
			unsigned swap_phase = SWAP_PHASE;
			unsigned ras2prech_dly = round_up(DDR3_tRAS_ps, 2*tCK)-1;

			dmc0_ctl(0xf) = ((zqc_dly<<24)|(tfaw_dly<<16)|(ras2ras_same_bank_dly<<8)|(swap_phase<<5)|(ras2prech_dly<<0));
			// DLY_CONFIG_4
		}

		dmc0_ctl(0x15) = ((tXS-1)<<24)|(ZQC_SHORT<<23)|(AUTO_ZQC_ENABLE<<22)|ZQC_INTERVAL;
		// ZQC_CONFIG

		dmc0_ctl(0x16) = (0<<16)|2;
		// AUTO_SCL_CTRL

		#if defined(CONFIG_DDR3_2CS)
		dmc0_ctl(0x10) = 0*MEM_CHIP_SELECTS; // ODT_CONFIG
		dmc0_ctl(0x13) = (3<<16)|2;			 // ODT_EN_CONFIG
		dmc0_ctl(0x10) = 1*MEM_CHIP_SELECTS; // ODT_CONFIG
		dmc0_ctl(0x13) = (3<<16)|1;			 // ODT_EN_CONFIG
		#else
		dmc0_ctl(0x10) = 0*MEM_CHIP_SELECTS; // ODT_CONFIG
		dmc0_ctl(0x13) = (1<<16)|2;			 // ODT_EN_CONFIG
		dmc0_ctl(0x10) = 1*MEM_CHIP_SELECTS; // ODT_CONFIG
		dmc0_ctl(0x13) = (2<<16)|1;			 // ODT_EN_CONFIG
		#endif

		{
			unsigned odt_len_wr = (HALF_RATE_MODE) ? (3-nCWL%2) : 5;
			#if defined(CONFIG_DDR3_2CS)
			unsigned odt_len_rd = 5;
			#else
			unsigned odt_len_rd = (HALF_RATE_MODE) ? (2 + (((nCL-nCWL) % 2 == 0) && (nCL-nCWL > 0)) ? 1 : 0) : 5;
			#endif
			unsigned odt_wr_set_dly = 1;
			unsigned odt_rd_set_dly = (HALF_RATE_MODE) ? ( (nCL - nCWL - (nCL - nCWL > 0 ? 1 : 0))/2 ) : (RL-WL);
			unsigned odt_en_sel =  0;

			dmc0_ctl(0x10) = (odt_len_wr<<20)|(odt_len_rd<<16)|(odt_wr_set_dly<<12)|(odt_rd_set_dly<<8)|odt_en_sel;
			// ODT_CONFIG
		}

		dmc0_ctl(0x11) = (BIG_ENDIAN<<31)|(7<<28)|(3<<24)|((RL-3-C2D)<<16)|(0<<12)|(0<<8)|(0<<4)|(WL-C2D-1-(nCWL%2 == 0 && !SWAP_PHASE && HALF_RATE_MODE));
		//DATA_XFR_CONFIG
	}


// dram init
	dmc0_ctl(0x4) = 1; // MEM_START
	dmc0_ctl(0x50/4) = 0x00000048;

	if (!no_reset) {
		HOST_CMD_LOAD0(0, 0, CKE_LOW, round_up(max(10000, 5*tCK), wait_cycle));
		HOST_CMD_LOAD0(1, 0, CKE_LOW, 0);
	}

	dmc0_ctl(0x12) = 0x11; // HOST_CMD_ISSUE
//	while(dmc0_ctl(0x12)&0x10); //

	i=24000; while(i--);
#if defined(CONFIG_DRAM_16BIT_USED)
	dmc0_ctl(0x14) = 0x0000005A;
#else
	dmc0_ctl(0x14) = 0x00000058;
#endif

	i=64000; while(i--);

	HOST_CMD_LOAD0(0, 0, IDLE_NOP, round_up(max(DDR3_tXPR_ps, DDR3_tXPR_ck*tCK), wait_cycle));

	//MR2
	HOST_CMD_LOAD0(1, 0x200|((ret_mr2(nCWL, RTT_WR)&0xFF00)>>8), ((ret_mr2(nCWL,RTT_WR)&0x00FF)<<24)|CMD_MRS, round_up(DDR3_tMRD_ck*tCK, wait_cycle));

	//MR3
	HOST_CMD_LOAD0(2, 0x300, CMD_MRS, round_up(DDR3_tMRD_ck*tCK, wait_cycle));

	//MR1
	HOST_CMD_LOAD0(3, 0x100|((ret_mr1(0, RTT_NOM, DIC)&0xFF00)>>8), ((ret_mr1(0, RTT_NOM, DIC)&0x00FF)<<24)|CMD_MRS, round_up(max(DDR3_tMOD_ps, DDR3_tMOD_ck*tCK), wait_cycle));

	//MR0
	HOST_CMD_LOAD0(4, 0x000|((ret_mr0(round_up(DDR3_tWR_ps, tCK),1,nCL)&0xFF00)>>8),((ret_mr0(round_up(DDR3_tWR_ps, tCK),1,nCL)&0x00FF)<<24)|CMD_MRS, round_up(max(DDR3_tMOD_ps, DDR3_tMOD_ck*tCK), wait_cycle));

	//ZQCL
	HOST_CMD_LOAD0(5, 0x004, CMD_ZQC, round_up(DDR3_tZQINIT_ck*tCK, wait_cycle));

	dmc0_ctl(0x12) = 0x15; // HOST_CMD_ISSUE
//	while(dmc0_ctl(0x12)&0x10); //
	dmc0_ctl(0x4) = 0x3; // MEM_START

// bit leveling and scl
	dmc0_phy(0x53) = 0xf; // WRLVL_AUTOINC_TRIM
	dmc0_phy(0x4D) = 1<<16; //PHY_DLL_TRIM_2
	dmc0_phy(0x40) = (1<<30)|(1<<28);  // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// Enable gating for bit-leveling
	dmc0_phy(0x6E) = 0x0; // DISABLE_GATING_FOR_SCL
	// write in bit levelling data
	dmc0_phy(0x41) = 0xFF00FF00; // SCL_DATA_0
	dmc0_phy(0x42) = 0xFF00FF00; // SCL_DATA_1
	dmc0_phy(0x62) = 0x00080000; // PHY_SCL_START_ADDR
	dmc0_phy(0x40) = 0x11000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// load SCL data into PHY
	dmc0_phy(0x62) = 0x00000000; // PHY_SCL_START_ADDR
	dmc0_phy(0x41) = 0x789b3de0; // SCL_DATA_0
	dmc0_phy(0x42) = 0xf10e4a56; // SCL_DATA_1

	// Run bit leveling
	dmc0_phy(0x40) = 0x30400000; //SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// Disable gating for bit-leveling to allow proper SCL gating
	dmc0_phy(0x6E) = 0x01; // DISABLE_GATING_FOR_SCL
#if defined PREVENT_DIGITAL_DLL_FOR_RECALIBRATION
	{
	    unsigned tmp;
	    tmp = dmc0_phy(0x49);
	    tmp |= (1<<26);
	    dmc0_phy(0x49) = tmp;
	}
#endif
	// Write SCL data int DRAM
	dmc0_phy(0x40) = 0x11000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// Run SCL
	dmc0_phy(0x50) = 0x10; // SCL_MAIN_CLK_DELTA
	dmc0_phy(0x40) = 0x34000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear
	// Turn off x-prop fix in simulation
	dmc0_phy(0x6E) = 0x03; // DISABLE_GATING_FOR_SCL

#if defined(DYNAMIC_SCL)
	dmc0_phy(0x40) = 0xA4200000;
	dmc0_phy(0x67) = 0x0200002F;
#endif

#if defined(DRAM_REG_DUMP)
	uart_set_gpio();
	uart_set_clock();
	uart_set_baudrate(115200);
	uart_flush_buffer();
	uart_dump_reg(0x73880100, 0x40);
#endif

#if 0 //DRAM_DEBUG
	__asm__ volatile("sub r12, pc, #8\n");
	__asm__ volatile("bx r12\n");
#endif
}
// INIT
/*===========================================================================
* FUNCTION : void func_sdram_init(void)
* DESCRIPTION : copy these codes of function at sram for kenel. Device use them when it wake up.
===========================================================================*/
void func_sdram_init(void)
{
#if defined(DRAM_DDR3)
	ddr_clock_set();
 	sdram_init(1);
#else
	#error "not selected ddr type.."
#endif
}

/*===========================================================================
* FUNCTION : InitRoutine_Start(void)
* DESCRIPTION :
===========================================================================*/
volatile void InitRoutine_Start(void)
{
#if defined(PRE_SET_PMIC)
	early_i2c_enable();
	early_i2c_port();
	set_core_voltage(1040);
#endif

#if defined(TARGET_TCC8970_STB) || defined(TARGET_TCC8975_STB)
	GPIO_F(1)  |= 0x30; // GPIO_F 4, 5 output
	GPIO_F(0)  |= 0x20; // GPIO_F 5 high -> Core 1.0v
#endif

#if defined(TSBM_ENABLE)
#endif

#if defined(DRAM_DDR3)
	ddr_clock_set();
	sdram_init(0);
#else
	#error "not selected ddr type.."
#endif
//	while(1);
	/* clear t32 debug check area (in SRAM) */
	*(volatile unsigned *)0x10008000 = 0;
}

/****************************************************************************************
* FUNCTION :void InitRoutine_End(void)
* DESCRIPTION :
* ***************************************************************************************/
volatile void InitRoutine_End(void)
{
}

/*===========================================================================
FUNCTION
===========================================================================*/
//static unsigned int get_cycle(unsigned int tck, unsigned int ps, unsigned int ck)
//{
//	unsigned int ret;
//
//	ret = time2cycle(ps, tck);
//
//	if(ret > ck)
//		return ret;
//	else
//		return ck;
//}

/*===========================================================================
FUNCTION
===========================================================================*/
static unsigned int get_membus_ckc(unsigned int mem_freq)
{
	tPMS nPLL;
	//unsigned int tmp_src, tmp_div;
	//unsigned temp;

	nPLL.fpll = mem_freq;
	if (tcc_find_pms(&nPLL, XIN_CLK_RATE))
		return 0;


	//tmp_src = 0;	// XIN
	//tmp_div = 0xFF;	//
	CKC_CHANGE_ARG(CKC_CTRL_VALUE) = 0x00001F08;//0x00000008|(tmp_src&0x7)|((tmp_div<<8)&0xFF000);
	//temp = 0x00001F08;
	CKC_CHANGE_ARG(PLL_VALUE) =  ((nPLL.s&0x7) << 16 ) | (1 << 26) | (0 << 19) | ((nPLL.m&0x3FF) << 6) | (nPLL.p&0x3F);
	//temp = ((nPLL.s&0x7) << 16 ) | (1 << 26) | (0 << 19) | ((nPLL.m&0x3FF) << 6) | (nPLL.p&0x3F);
	CKC_CHANGE_ARG(CLK_RATE) = (nPLL.fpll/1000000);
	//temp = (nPLL.fpll/1000000);

	return CKC_CHANGE_ARG(CLK_RATE);
}


/*===========================================================================
FUNCTION
===========================================================================*/
void copy_dram_param(void)
{
#if 0
	DRAM_P_ARG(tRAS_ps) = DDR3_tRAS_ps;
	DRAM_P_ARG(tRAS_ck) = DDR3_tRAS_ck;
	DRAM_P_ARG(tRC_ps) = DDR3_tRC_ps;
	DRAM_P_ARG(tRC_ck) = DDR3_tRC_ck;
	DRAM_P_ARG(tRRD_ps) = DDR3_tRRD_ps;
	DRAM_P_ARG(tRRD_ck) = DDR3_tRRD_ck;
	DRAM_P_ARG(tCCD_ck) = DDR3_tCCD_ck;
	DRAM_P_ARG(tMRD_ck) = DDR3_tMRD_ck;
	DRAM_P_ARG(tRTP_ps) = DDR3_tRTP_ps;
	DRAM_P_ARG(tRTP_ck) = DDR3_tRTP_ck;
	DRAM_P_ARG(tRCD_ps) = DDR3_tRCD_ps;
	DRAM_P_ARG(tRCD_ck) = DDR3_tRCD_ck;
	DRAM_P_ARG(tRP_ps) = DDR3_tRP_ps;
	DRAM_P_ARG(tRP_ck) = DDR3_tRP_ck;
	DRAM_P_ARG(tWTR_ps) = DDR3_tWTR_ps;
	DRAM_P_ARG(tWTR_ck) = DDR3_tWTR_ck;
	DRAM_P_ARG(tRAS_MAX_ps) = DDR3_tRAS_MAX_ps;
	DRAM_P_ARG(tMOD_ps) = DDR3_tMOD_ps;
	DRAM_P_ARG(tMOD_ck) = DDR3_tMOD_ck;
	DRAM_P_ARG(tCKE_ps) = DDR3_tCKE_ps;
	DRAM_P_ARG(tCKE_ck) = DDR3_tCKE_ck;
	DRAM_P_ARG(tWR_ps) = DDR3_tWR_ps;
	DRAM_P_ARG(tWR_ck) = DDR3_tWR_ck;
	DRAM_P_ARG(tFAW_ck) = DDR3_tFAW_ck;
	DRAM_P_ARG(tRFC_ps) = DDR3_tRFC_ps;
	DRAM_P_ARG(tREFI_ps) = DDR3_tREFI_ps;
	DRAM_P_ARG(tDLLK_ck) = DDR3_tDLLK_ck;
	DRAM_P_ARG(tXPDLL_ps) = DDR3_tXPDLL_ps;
	DRAM_P_ARG(tXPDLL_ck) = DDR3_tXPDLL_ck;
	DRAM_P_ARG(tXP_ps) = DDR3_tXP_ps;
	DRAM_P_ARG(tXP_ck) = DDR3_tXP_ck;
	DRAM_P_ARG(tXS_ps) = DDR3_tXS_ps;
	DRAM_P_ARG(tXS_ck) = DDR3_tXS_ck;
	DRAM_P_ARG(tXSDLL_ck) = DDR3_tXSDLL_ck;
	DRAM_P_ARG(tCKSRX_ps) = DDR3_tCKSRX_ps;
	DRAM_P_ARG(tCKSRX_ck) = DDR3_tCKSRX_ck;
	DRAM_P_ARG(tCKSRE_ps) = DDR3_tCKSRE_ps;
	DRAM_P_ARG(tCKSRE_ck) = DDR3_tCKSRE_ck;
	DRAM_P_ARG(BURST_LEN) = DDR3_BURST_LEN;
	DRAM_P_ARG(BURST_TYPE) = DDR3_READ_BURST_TYPE;
	DRAM_P_ARG(AL) = DDR3_AL;
	DRAM_P_ARG(ODT) = DDR3_ODT;
	DRAM_P_ARG(DIC) = DDR3_DIC;
	DRAM_P_ARG(MAX_SPEED) = DDR3_MAX_SPEED;
#endif
}

#ifdef EARLY_CARMERA
void copy_dram_init(void)
{
	int i;
	unsigned src = (unsigned)func_sdram_init;
	unsigned dest = DRAM_INIT_BASE;

	for(i=0;i<KERNEL_INIT_FUNC_SIZE;i+=4)
		*(volatile unsigned long *)(dest+i) = *(volatile unsigned long *)(src+i);

}
#else
void copy_dram_init(void)
{
	memcpy((void*)SDRAM_INIT_FUNC_ADDR, (const void *)func_sdram_init, SDRAM_INIT_FUNC_SIZE);
	memcpy((void*)SDRAM_SELF_REF_ENTER_ADDR, (const void *)ddr_self_refresh_enter, SDRAM_SELF_REF_ENTER_SIZE);
	memcpy((void*)SDRAM_SELF_REF_EXIT_ADDR, (const void *)ddr_self_refresh_exit, SDRAM_SELF_REF_EXIT_SIZE);
}
#endif


/*===========================================================================
FUNCTION
===========================================================================*/
static void get_ddr_param(unsigned int mem_freq)
{
#if defined(DRAM_DDR3)

	unsigned tck = 0;
	unsigned nCL = 0;
	unsigned nCWL = 0;
	unsigned wait_cycle;

	tck = (1000000/mem_freq);

	if(tck >= 2500){ /* 2.5ns, 400MHz */
		nCL = 9;
		nCWL = 6;
	}else if(tck >= 1875){ // 1.875ns, 533..MHz
		nCL = 9;
		nCWL = 6;
	}else if(tck >= 1500){ // 1.5 ns, 666..MHz
#if defined(CONFIG_DDR3_1600)
			nCL = 9;
#else
			nCL = 10;
#endif
			nCWL = 7;
	}else if(tck >= 1250){ // 1.25 ns, 800MHz
		nCL = 11;
		nCWL = 8;
	}else if(tck >= 1070){ // 1.07 ns, 933..MHz
		nCL = 13;
		nCWL = 9;
	}else if(tck >= 935){ // 0.935 ns, 1066..MHz
		nCL = 14;
		nCWL = 10;
	}

	wait_cycle = HALF_RATE_MODE ? 2*tck : tck;

	CKC_CHANGE_ARG(DMC_46H)= (1<<24)|(0<<16)|(0<<12)|(round_up(nCL,2)<<4)|(0<<3)|(1<<2)|(0<<1)|1;
	// SCL_CONFIG_1
	// ddr_odt_ctl_wr, ddr_odt_ctrl_rd, local_odt_ctrl, rd_cas_latency, dly_dfi_phyupd_ack, ddr3, ddr2, burst8

	CKC_CHANGE_ARG(DMC_47H) = (SWAP_PHASE<<31)|(0<<30)|(0<<29)|(0<<28)|(1<<25)|((HALF_RATE_MODE && (((nCWL%2==0)?1:0)^SWAP_PHASE))<<24)|(0<<12)|(round_up(nCWL,2)<<8)|1;
	// SCL_CONFIG_2
	// swap_phase,rdfifo_enable,lpddr2,analog_dll_for_scl,scl_step,size,dly_dfi_wrdata,double_ref_dly,wr_cas,latency,scl_test_cs

	CKC_CHANGE_ARG(DMC_6FH) = ((nCWL%2==0) && SWAP_PHASE);
	// SCL_CONFIG_4

	CKC_CHANGE_ARG(DMC_54H) = (((RTT_WR<<9)|((nCWL-5)<<3))<<16)|(((nCWL-5)<<3));
	// WRLVL_DYN_ODT
	// dynamic_odt_on, dynamic_odt_off

	{
		unsigned rmw_dly, rd2wr_dly, wr2prech_dly, wr2rd_dly, wr2rd_csc_dly, nxt_dt_av_dly;

		unsigned wr2wr_csc_dly;
		unsigned wr2wr_dly    ;
		unsigned rd2rd_csc_dly;
		unsigned rd2rd_dly      ;

		unsigned sref_exit;

		int tRCD, tXS, tXP, tCKE, tCKESR;
		int RL, WL;


		RL = HALF_RATE_MODE ? round_up (nCL, 2)        : nCL;
		WL = HALF_RATE_MODE ? round_up ((nCWL + 1), 2) : nCWL;

		rmw_dly        = RL+C2D+MAX_LATENCY-WL+4+(ECC_DLY|HALF_RATE_SUPPORT ? 1 : 0);

		if (HALF_RATE_MODE)
		{
		      rd2wr_dly     = BL + 2 + round_down((nCL - nCWL), 2);
		      wr2wr_csc_dly = BL;
		      wr2wr_dly     = BL-1;
		      rd2rd_csc_dly = BL+1;
		      rd2rd_dly     = BL-1;
		      wr2rd_csc_dly = max((WL+BL) + max(round_up(DDR3_tWTR_ps, 2*tck), round_up(DDR3_tWTR_ck, 2)) - 1, 3);
		      wr2rd_dly     = (WL+BL) + max(round_up(DDR3_tWTR_ps, 2*tck), round_up(DDR3_tWTR_ck, 2));
		      wr2prech_dly  = round_up(DDR3_tWR_ps, 2*tck) + (WL+BL);
		}
		else
		{
		      rd2wr_dly     = BL + 3 - 1 + RL - WL;
		      wr2wr_csc_dly = BL+1;
		      wr2wr_dly     = BL-1;
		      rd2rd_csc_dly = BL+2;
		      rd2rd_dly     = BL-1;
		      wr2rd_csc_dly = max((WL+BL)+max(round_up(DDR3_tWTR_ps, 2*tck), DDR3_tWTR_ck) - BL, 4);
		      wr2rd_dly     = (WL+BL) + max(round_up(DDR3_tWTR_ps, 2*tck), DDR3_tWTR_ck);
		      wr2prech_dly  = round_up(DDR3_tWR_ps, 2*tck) + (WL+max(BL,3));
		}
		nxt_dt_av_dly = 0;

		if ((round_up(DDR3_tRCD_ps, tck) % 2) == 0 && !SWAP_PHASE && HALF_RATE_MODE)
		{
			tRCD = round_up(DDR3_tRCD_ps, 2*tck);
		}
		else
		{
			tRCD = round_up(DDR3_tRCD_ps, 2*tck) - 1;
		}

		tXS       = AUTO_ZQC_ENABLE ? max(5, round_up((DDR3_tRFC_ps+10000), 2*tck)) : 0;
		tCKE      = max(DDR3_tCKE_ck, round_up(DDR3_tCKE_ps, 2*tck));
		tCKESR    = max(DDR3_tCKESR_ck, round_up(DDR3_tCKE_ps, 2*tck)+1);
		tXP       = max(max(DDR3_tXP_ck, round_up(DDR3_tXP_ps, 2*tck)), tCKE);
		sref_exit = HALF_RATE_MODE ? max(round_up(DDR3_tDLLK_ck, 2) - tXS, round_up(DDR3_tZQOPER_ck, 2))-2 :
		                               max(DDR3_tDLLK_ck               -tXS, DDR3_tZQOPER_ck             )-2 ;


		CKC_CHANGE_ARG(DMC_AH) = (1<<28)|(8<<24)|((round_up(DDR3_tRFC_ps, 2*tck)-2)<<16)|(round_up(DDR3_tREFI_ps, 2*tck)-2);
		//REF_CONFIG

		CKC_CHANGE_ARG(DMC_CH) = (rmw_dly<<28)|(rd2wr_dly<<24)|(wr2wr_csc_dly<<20)|(wr2wr_dly<<16)|(rd2rd_csc_dly<<12)|(rd2rd_dly<<8)|(wr2rd_csc_dly<<4)|wr2rd_dly;
		// DLY_CONFIG_1

		CKC_CHANGE_ARG(DMC_DH) = (((sref_exit & (1<<8))>>8)<<30)|
			                    (((nxt_dt_av_dly & (1<<4))>>4)<<29)|
			                    (((wr2rd_csc_dly & (1<<4))>>4)<<28)|
			                    (((wr2rd_dly & (1<<4))>>4)<<27)|
			                    (((wr2prech_dly & (1<<4))>>4)<<26)|
			                    (((tXP-1))<<20)|
			                    (((tCKE-1))<<16)|
			                    (((sref_exit))<<8)|
			                    (((tCKESR-1)) <<0);
		// DLY_CONFIG_2

		{
			unsigned rd_burst_end_dly = RL+3+(NATIVE_BURST8 ? 2:0);
			unsigned prech2ras_dly    = round_up(DDR3_tRP_ps, 2*tck)-1;
			unsigned ras2cas_dly      = tRCD;
			unsigned ras2ras_dly      = max(round_up(DDR3_tRRD_ps, 2*tck), round_up(DDR3_tRRD_ck, 2)) - 1;
			//unsigned wr2prech_dly     = ret4(wr2prech_dly);
			unsigned rd2prech_dly     = (HALF_RATE_MODE ? (max(round_up(DDR3_tRTP_ps, 2*tck), round_up(DDR3_tRTP_ck, 2)) - 1) :
			                                 (max(round_up(DDR3_tRTP_ps, 2*tck), DDR3_tRTP_ck) - 1));
			unsigned prech_all_dly    = round_up(DDR3_tRP_ps, 2*tck);

			CKC_CHANGE_ARG(DMC_EH)= (rd_burst_end_dly<<28)|(prech2ras_dly<<24)|(ras2cas_dly<<20)|(ras2ras_dly<<16)|(wr2prech_dly<<12)|(rd2prech_dly<<8)|(prech_all_dly<<0);
			// DLY_CONFIG_3
  		}

		{
			unsigned zqc_dly = (HALF_RATE_MODE ? (ZQC_SHORT ? round_up(DDR3_tZQCS_ck, 2)-1 : round_up(DDR3_tZQOPER_ck, 2)-1) :
			                        (ZQC_SHORT ? DDR3_tZQCS_ck-1 : DDR3_tZQOPER_ck-1));
			unsigned tfaw_dly = round_up(DDR3_tFAW_ps, 2*tck)-1;
			unsigned ras2ras_same_bank_dly = round_up(DDR3_tRC_ps, 2*tck)-1;
			unsigned swap_phase = SWAP_PHASE;
			unsigned ras2prech_dly = round_up(DDR3_tRAS_ps, 2*tck)-1;

			CKC_CHANGE_ARG(DMC_FH) = ((zqc_dly<<24)|(tfaw_dly<<16)|(ras2ras_same_bank_dly<<8)|(swap_phase<<5)|(ras2prech_dly<<0));
			// DLY_CONFIG_4
		}

		CKC_CHANGE_ARG(DMC_15H) = ((tXS-1)<<24)|(ZQC_SHORT<<23)|(AUTO_ZQC_ENABLE<<22)|ZQC_INTERVAL;
		// ZQC_CONFIG


		{
			unsigned odt_len_wr = (HALF_RATE_MODE) ? (3-nCWL%2) : 5;
			unsigned odt_len_rd = (HALF_RATE_MODE) ? (2 + (((nCL-nCWL) % 2 == 0) && (nCL-nCWL > 0)) ? 1 : 0) : 5;
			unsigned odt_wr_set_dly = 1;
			unsigned odt_rd_set_dly = (HALF_RATE_MODE) ? ( (nCL - nCWL - (nCL - nCWL > 0 ? 1 : 0))/2 ) : (RL-WL);
			unsigned odt_en_sel =  0;

			CKC_CHANGE_ARG(DMC_10H) = (odt_len_wr<<20)|(odt_len_rd<<16)|(odt_wr_set_dly<<12)|(odt_rd_set_dly<<8)|odt_en_sel;
			// ODT_CONFIG
		}

		CKC_CHANGE_ARG(DMC_11H) = (BIG_ENDIAN<<31)|(7<<28)|(3<<24)|((RL-3-C2D)<<16)|(0<<12)|(0<<8)|(0<<4)|(WL-C2D-1-(nCWL%2 == 0 && !SWAP_PHASE && HALF_RATE_MODE));
		//DATA_XFR_CONFIG
	}


	//MR1
//	HOST_CMD_LOAD1(3, 0x100|((ret_mr1(0, RTT_NOM, DIC)&0xFF00)>>8), ((ret_mr1(0, RTT_NOM, DIC)&0x00FF)<<24)|CMD_MRS, round_up(max(DDR3_tMOD_ps, DDR3_tMOD_ck*tck), wait_cycle));


	CKC_CHANGE_ARG(DMC_20H) = IDLE_NOP;
	CKC_CHANGE_ARG(DMC_30H) = (max(round_up(max(DDR3_tXS_ps, DDR3_tXS_ck*tck), wait_cycle), 1) -1)<<12 | 0;

	CKC_CHANGE_ARG(DMC_21H) = ((ret_mr0(round_up(DDR3_tWR_ps, tck),1,nCL)&0x00FF)<<24)|CMD_MRS;
	CKC_CHANGE_ARG(DMC_31H) = ((max(round_up(DDR3_tMRD_ck*tck, wait_cycle),1)-1)<<12)|(0x000|((ret_mr0(round_up(DDR3_tWR_ps, tck),1,nCL)&0xFF00)>>8));

//	CKC_CHANGE_ARG(DMC_22H) = ((ret_mr1(0, RTT_NOM, DIC)&0x00FF)<<24)|CMD_MRS;
//	CKC_CHANGE_ARG(DMC_32H) = ((max(round_up(max(DDR3_tMOD_ps, DDR3_tMOD_ck*tck), wait_cycle),1)-1)<<12)|(0x100|((ret_mr1(0, RTT_NOM, DIC)&0xFF00)>>8));

	CKC_CHANGE_ARG(DMC_22H) = ((ret_mr2(nCWL,RTT_WR)&0x00FF)<<24)|CMD_MRS;
	CKC_CHANGE_ARG(DMC_32H) = ((max(round_up(DDR3_tDLLK_ck*tck, wait_cycle), 1)-1)<<12)|(0x200|((ret_mr2(nCWL, RTT_WR)&0xFF00)>>8));


#else
	#error "not selected ddr type.."
#endif
}

void ddr_self_refresh_enter(unsigned int shutdown)
{
	volatile unsigned i;

//------------------------------------------------------------------------------------------------
// hold dram traffic
	HALT_CFG0 = 0x1;
	while((HALT_STS0&0xC00) != 0xC00);
	i=3200; while(i--);
//------------------------------------------------------------------------------------------------
// enter self-refresh mode

	dmc0_ctl(0xB) = (dmc0_ctl(0xB) & ~((1<<3)|(0x3FF<<12)));
	dmc0_ctl(0xB) = dmc0_ctl(0xB) | ((1<<3)|(1<<12));
	i=3200; while(i--);

	if(shutdown){
	    dmc0_ctl(0x5C) &= (~0x1);
	    i=320000; while(i--);
	    dmc0_ctl(0x48) &= (~0x10000000);
	}
}

void ddr_self_refresh_exit(void)
{
	volatile int i;
	unsigned dat;
//------------------------------------------------------------------------------------------------
// exit self-refresh mode
	dmc0_ctl(0xB) = (dmc0_ctl(0xB) & ~(1<<3));
//------------------------------------------------------------------------------------------------
// TRIM

	// Disable gating for bit-leveling to allow proper SCL gating
	dmc0_phy(0x6E) = 0x1; // DISABLE_GATING_FOR_SCL
	// Write SCL data int DRAM
	dmc0_phy(0x40) = 0x11000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// Run SCL
	dmc0_phy(0x50) = 0x10; // SCL_MAIN_CLK_DELTA
	dmc0_phy(0x40) = 0x34000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	dat = dmc0_phy(0x40);
	dat = dat & 0xf;
	// Turn off x-prop fix in simulation
	dmc0_phy(0x6E) = 0x03; // DISABLE_GATING_FOR_SCL
//------------------------------------------------------------------------------------------------
// resume dram traffic
	HALT_CFG0 = 0x0;
	i=3200; while(i--);
}

/*===========================================================================
FUNCTION
===========================================================================*/
void _change_mem_clock(void)
{
#if defined(DRAM_DDR3)
	volatile int i;
	unsigned master_dll, mas_dll_dly, dat;
//------------------------------------------------------------------------------------------------
// hold dram traffic

	HALT_CFG0 = 0x1;
	while((HALT_STS0&0xC00) != 0xC00);
	i=3200; while(i--);

//------------------------------------------------------------------------------------------------
// enter self-refresh mode

	dmc0_ctl(0xB) = (dmc0_ctl(0xB) & ~((1<<3)|(0x3FF<<12)));
	dmc0_ctl(0xB) = dmc0_ctl(0xB) | ((1<<3)|(0x8<<12));
	i=3200; while(i--);
//------------------------------------------------------------------------------------------------
// clock change
	// :TODO
#if 1
	*(volatile unsigned long *)addr_clk(0x000008) = 0x00001F00; //CKC_CHANGE_ARG(CKC_CTRL_VALUE);  // mem bus
	while((*(volatile unsigned long *)addr_clk(0x000008))&(1<<29)); // CFGREQ
	i=100; while(i--);
	*(volatile unsigned long *)addr_clk(0x000050) =  CKC_CHANGE_ARG(PLL_VALUE) | 0x00100000; //pll1 for mem , lock_en
	*(volatile unsigned long *)addr_clk(0x000050) |= 0x80000000; //pll1 for mem , lock_en
	i=20; while(i--)while((*(volatile unsigned long *)addr_clk(0x000050) & 0x00800000) == 0);
	*(volatile unsigned long *)addr_clk(0x000008) = 0x00001F01;  // mem bus
	while((*(volatile unsigned long *)addr_clk(0x000008))&(1<<29)); // CFGREQ
	i=10000; while(i--);
#endif
//------------------------------------------------------------------------------------------------
// re-initailize
	dmc0_phy(0x46) = CKC_CHANGE_ARG(DMC_46H);
	dmc0_phy(0x47) = CKC_CHANGE_ARG(DMC_47H);
	dmc0_phy(0x6f) = CKC_CHANGE_ARG(DMC_6FH);
	dmc0_phy(0x54) = CKC_CHANGE_ARG(DMC_54H);

	dmc0_ctl(0xa) = CKC_CHANGE_ARG(DMC_AH);   // timing
	dmc0_ctl(0xc) = CKC_CHANGE_ARG(DMC_CH);
	dmc0_ctl(0xd) = CKC_CHANGE_ARG(DMC_DH);
	dmc0_ctl(0xe) = CKC_CHANGE_ARG(DMC_EH);
	dmc0_ctl(0xf) = CKC_CHANGE_ARG(DMC_FH);

	dmc0_ctl(0x15) = CKC_CHANGE_ARG(DMC_15H);

	dmc0_ctl(0x16) = (0<<16)|2;
	i=3200; while(i--);
	// AUTO_SCL_CTRL
	#if 1 // temporary prevent
	dmc0_ctl(0x10) = 0*MEM_CHIP_SELECTS; // ODT_CONFIG
	dmc0_ctl(0x13) = (1<<16)|2;			 // ODT_EN_CONFIG
	dmc0_ctl(0x10) = 1*MEM_CHIP_SELECTS; // ODT_CONFIG
	dmc0_ctl(0x13) = (2<<16)|1;			 // ODT_EN_CONFIG

	dmc0_ctl(0x10) = CKC_CHANGE_ARG(DMC_10H);
	dmc0_ctl(0x11) = CKC_CHANGE_ARG(DMC_11H);
	#endif

//------------------------------------------------------------------------------------------------
// exit self-refresh mode
	dmc0_ctl(0xB) = (dmc0_ctl(0xB) & ~(1<<3));
	i=3200; while(i--);

//------------------------------------------------------------------------------------------------
// MR setting and TRIM
	dmc0_ctl(0x20) = CKC_CHANGE_ARG(DMC_20H);
	dmc0_ctl(0x30) = CKC_CHANGE_ARG(DMC_30H);
	dmc0_ctl(0x21) = CKC_CHANGE_ARG(DMC_21H);
	dmc0_ctl(0x31) = CKC_CHANGE_ARG(DMC_31H);
	dmc0_ctl(0x22) = CKC_CHANGE_ARG(DMC_22H);
	dmc0_ctl(0x32) = CKC_CHANGE_ARG(DMC_32H);

	dmc0_ctl(0x12) = 0x12; // HOST_CMD_ISSUE
//	while((dmc0_ctl(0x12)&0x10));  // temporary prevent

	dat = dmc0_phy(0x49);
	dat = (dat>>8) & 0x3FFFF;
	i=3200; while(i--);

	mas_dll_dly = 0;
	master_dll = (10<<28)|0x1000;

	while(mas_dll_dly==0)
	{
		mas_dll_dly = dmc0_phy(0x4A);
		mas_dll_dly = mas_dll_dly  >> 24;
	}

	mas_dll_dly = dmc0_phy(0x4A);

	mas_dll_dly = (mas_dll_dly >> 24) >> 1;
	dmc0_phy(0x4A) = 0x200|mas_dll_dly;
	dmc0_phy(0x49) = master_dll | mas_dll_dly | (1<<27);
	dmc0_phy(0x69) = (1<<SLV_DLY_WIDTH)|mas_dll_dly;

	#if defined(BIT_LEVELING_CLOCK_CHANGE) // temporary prevent
	dmc0_phy(0x53) = 0xf;
	dmc0_phy(0x4D) = 1<<16;
	dmc0_phy(0x40) = (1<<30)|(1<<28); // SCL_START
	while((dmc0_ctl(0x12)&0x10));
	while((dmc0_phy(0x40)&0x10000000));
	#endif
	dat = dmc0_phy(0x52);
	dat = dat & 0xf;

	i=3200; while(i--);
	#if defined(BIT_LEVELING_CLOCK_CHANGE) // temporary prevent
	// Enable gating for bit-leveling
	dmc0_phy(0x6E) = 0x0; // DISABLE_GATING_FOR_SCL
	// write in bit levelling data
	dmc0_phy(0x41) = 0xFF00FF00; // SCL_DATA_0
	dmc0_phy(0x42) = 0xFF00FF00; // SCL_DATA_1
	dmc0_phy(0x62) = 0x00080000; // PHY_SCL_START_ADDR
	dmc0_phy(0x40) = 0x11000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// load SCL data into PHY
	dmc0_phy(0x62) = 0x00000000; // PHY_SCL_START_ADDR
	dmc0_phy(0x41) = 0x789b3de0; // SCL_DATA_0
	dmc0_phy(0x42) = 0xf10e4a56; // SCL_DATA_1
	dmc0_phy(0x40) = 0x30400000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear


	dat = dmc0_phy(0x6B);
	#endif

	// Disable gating for bit-leveling to allow proper SCL gating
	dmc0_phy(0x6E) = 0x1; // DISABLE_GATING_FOR_SCL
#if defined PREVENT_DIGITAL_DLL_FOR_RECALIBRATION
	{
	    unsigned tmp;
	    tmp = dmc0_phy(0x49);
	    tmp |= (1<<26);
	    dmc0_phy(0x49) = tmp;
	}
#endif
	// Write SCL data int DRAM
	dmc0_phy(0x40) = 0x11000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	// Run SCL
	dmc0_phy(0x50) = 0x10; // SCL_MAIN_CLK_DELTA
	dmc0_phy(0x40) = 0x34000000; // SCL_START
	while(dmc0_phy(0x40)&0x10000000); // wait bit clear

	dat = dmc0_phy(0x40);
	dat = dat & 0xf;
	// Turn off x-prop fix in simulation
	dmc0_phy(0x6E) = 0x03; // DISABLE_GATING_FOR_SCL

#if defined(DYNAMIC_SCL)
	dmc0_phy(0x40) = 0xA4200000;
	dmc0_phy(0x67) = 0x0200002F;
#endif

//------------------------------------------------------------------------------------------------
// resume dram traffic
	HALT_CFG0 = 0x0;
	i=3200; while(i--);
#endif
}

/*===========================================================================
FUNCTION
===========================================================================*/
int change_mem_clock(unsigned int freq)
{
	int i;
	typedef void (*FuncPtr)(void);
	unsigned flags, tmp, stk;
	unsigned int   mem_freq;
	FuncPtr pFunc = (FuncPtr)(CKC_CHANGE_FUNC_ADDR);

	//Bruce_temp_8920, memcpy가 오동작한다.
	//memcpy((void *)CKC_CHANGE_FUNC_ADDR, (void*)_change_mem_clock, CKC_CHANGE_FUNC_SIZE);
	{
		unsigned src = (unsigned)_change_mem_clock;
		unsigned dest = CKC_CHANGE_FUNC_ADDR;

		for(i=0;i<CKC_CHANGE_FUNC_SIZE;i+=4)
			*(volatile unsigned long *)(dest+i) = *(volatile unsigned long *)(src+i);
	}

	mem_freq = get_membus_ckc(freq);
	get_ddr_param(mem_freq);


	__asm__ __volatile__( \
	"mrs	%0, cpsr\n" \
	"cpsid	i\n" \
	"mrs	%1, cpsr\n" \
	"orr	%1, %1, #128\n" \
	"msr	cpsr_c, %1" \
	: "=r" (flags), "=r" (tmp) \
	: \
	: "memory", "cc");

	//CKC_CHANGE_STACK_TOP
	__asm__ __volatile__( \
	"mov	%0, sp \n" \
	"ldr	sp, =0x10010000 \n" \
	: "=r" (stk) \
	: \
	: "memory", "cc");

	//arm_invalidate_tlb();

	{
		uint32_t val;
		__asm__ volatile("mrc	p15, 0, %0, c1, c0, 0" : "=r" (val));
		val &= ~0x1;
		__asm__ volatile("mcr	p15, 0, %0, c1, c0, 0" :: "r" (val));
	}

	pFunc();

	{
		uint32_t val;
		__asm__ volatile("mrc	p15, 0, %0, c1, c0, 0" : "=r" (val));
		val |= 0x1;
		__asm__ volatile("mcr	p15, 0, %0, c1, c0, 0" :: "r" (val));
	}

	__asm__ __volatile__( \
	"mov	sp, %0 \n" \
	: \
	: "r" (stk) \
	: "memory", "cc");

	__asm__ __volatile__( \
	"msr	cpsr_c, %0\n " \
	"cpsid	i" \
	: \
	: "r" (flags) \
	: "memory", "cc");

	return 0;
}

/*===========================================================================
FUNCTION
===========================================================================*/
int sdram_test(void)
{
	return 0;
}

/************* end of file *************************************************************/

