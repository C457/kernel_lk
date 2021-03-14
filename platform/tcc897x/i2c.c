/*
 * Copyright (c) 2011 Telechips, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include <debug.h>
#include <err.h>
#include <reg.h>
#include <string.h>
#include <platform.h>
#include <i2c.h>
#include <plat/cpu.h>
#include <platform/iomap.h>
#include <platform/reg_physical.h>
#include <dev/gpio.h>
#include <platform/gpio.h>
#include <platform/tcc_ckc.h>
#include <clock.h>

#define i2c_readl	readl
#define i2c_writel	writel

#define	I2C_PCFG0	0x00
#define	I2C_PCFG1	0x04
#define	I2C_ISTS	0x0C


/* read/write bit */
#define I2C_WR	0
#define I2C_RD	1

/* 
 * I2C register structure
 */
struct tcc_i2c_regs {
	volatile unsigned long PRES, CTRL, TXR, CMD, RXR, SR, TR;
};

/* 
 * register structure
 */
struct tcc_i2c {
	unsigned int clk;
	unsigned long IRQSTR;
	volatile struct tcc_i2c_regs *regs;
	unsigned int peri_name;
	unsigned int iobus_name;
	int is_reset;
};

static struct tcc_i2c i2c[I2C_CH_NUM] = {
	[I2C_CH_MASTER0] = {
		.clk = 400,
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER0_BASE,
		.peri_name = PERI_I2C0,
		.iobus_name = IOBUS_I2C_M0,
		.is_reset = 0,
	},
	[I2C_CH_MASTER1] = {
		.clk = 400,
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER1_BASE,
		.peri_name = PERI_I2C1,
		.iobus_name = IOBUS_I2C_M1,
		.is_reset = 0,
	},
	[I2C_CH_MASTER2] = {
		.clk = 100,
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER2_BASE,
		.peri_name = PERI_I2C2,
		.iobus_name = IOBUS_I2C_M2,
		.is_reset = 0,
	},
	[I2C_CH_MASTER3] = {
		.clk = 400,
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER3_BASE,
		.peri_name = PERI_I2C3,
		.iobus_name = IOBUS_I2C_M3,
		.is_reset = 0,
	},
	/* SMU_I2C Controller */
	[I2C_CH_SMU] = {
		.clk = 100,									/* SMU_I2C prescale value */
		.IRQSTR = HwSMUI2C_BASE + 0x80,	/* SMU_I2C ICLK register address */
		.regs = (volatile struct tcc_i2c_regs *)HwSMUI2C_BASE,
	},
};

static int wait_intr(int i2c_name)
{
	volatile unsigned long cnt = 0;

	if (i2c_name < I2C_CH_SMU) {
#if 1	
		while (!(i2c_readl(i2c[i2c_name].IRQSTR) & (1<<i2c_name))) {
			cnt++;
			if (cnt > 100000) {
				printf("i2c-tcc: time out!\n");
				return -1;
			}
		}
#else
	/* check RxACK */
	while (1) {
		cnt++;
		if ((i2c[i2c_name].regs->SR & Hw0)) {
			if (!(i2c[i2c_name].regs->SR & Hw7)) {
				break;
			}
		}
		if (cnt > 100000) {
			printf("i2c-tcc: time out!\n");
			return -1;
		}
	}
#endif
	} else {
		/* SMU_I2C wait */
		while (1) {
			cnt++;
			if (!(i2c[i2c_name].regs->SR & Hw1)) break;
			if (cnt > 100000) {
				printf("smu-i2c-tcc: time out!\n");
				return -1;
			}
		}
		for (cnt = 0; cnt <15; cnt++);
	}

	return 0;
}

int i2c_xfer(unsigned char slave_addr, 
		unsigned char out_len, unsigned char* out_buf, 
		unsigned char in_len, unsigned char* in_buf, 
		int i2c_name)
{
	int ret;
	int i = 0;

	/* 
	 * WRITE 
	 */
	if (out_len > 0) {
		i2c[i2c_name].regs->TXR = slave_addr | I2C_WR;
		i2c[i2c_name].regs->CMD = Hw7 | Hw4;

		ret = wait_intr(i2c_name);
		if (ret != 0) return ret;
		BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt

		for (i = 0; i < out_len; i++) {
			i2c[i2c_name].regs->TXR = out_buf[i];
			i2c[i2c_name].regs->CMD = Hw4;

			ret = wait_intr(i2c_name);
			if (ret != 0) return ret;
			BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt
		}

		if(in_len <= 0)
		{
			i2c[i2c_name].regs->CMD = Hw6;

			ret = wait_intr(i2c_name);
			if (ret != 0) return ret;
		}

		BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt
	}

	/* 
	 * READ
	 */
	if (in_len > 0) {
		i2c[i2c_name].regs->TXR = slave_addr | I2C_RD;
		i2c[i2c_name].regs->CMD = Hw7 | Hw4;

		ret = wait_intr(i2c_name);
		if (ret != 0) return ret;
		BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt

		for (i = 0; i < in_len; i++) {
			//i2c[i2c_name].regs->CMD = Hw5 | Hw3;
			if (i == (in_len - 1)) 
    			i2c[i2c_name].regs->CMD = Hw5 | Hw3;
    		else
    			i2c[i2c_name].regs->CMD = Hw5;

			ret = wait_intr(i2c_name);
			if (ret != 0) return ret;

			BITSET( i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt

			in_buf[i] =(unsigned char)i2c[i2c_name].regs->RXR;
		}

		i2c[i2c_name].regs->CMD = Hw6;

		ret = wait_intr(i2c_name);
		if (ret != 0) return ret;
		BITSET( i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt
	}

	return 0;
}

static void i2c_reset(int i2c_name)
{
	if (!i2c[i2c_name].is_reset) {
		tcc_set_iobus_swreset(i2c[i2c_name].iobus_name, 1);
		tcc_set_iobus_swreset(i2c[i2c_name].iobus_name, 0);
		i2c[i2c_name].is_reset = 1;
	}
}

static void i2c_set_gpio(int i2c_name, int port)
{
	void __iomem *portcfg = (void __iomem *)(HwI2C_PORTCFG_BASE + I2C_PCFG0);
	unsigned reg_shift = i2c_name*8;
	unsigned reg_values;

	if (i2c_name > I2C_CH_MASTER3)
		return;

	/* set i2c portmux cfg */
	reg_values = i2c_readl(portcfg) & ~(0xFF<<reg_shift);
	i2c_writel(reg_values | (port<<reg_shift), portcfg);

	/* set gpio functions selection */
	switch (port) {
	case 0:
		gpio_config(TCC_GPA(9), GPIO_FN8);
		gpio_config(TCC_GPA(10), GPIO_FN8);
		break;
	case 1:
		gpio_config(TCC_GPA(11), GPIO_FN8);
		gpio_config(TCC_GPA(12), GPIO_FN8);
		break;
	case 2:
		gpio_config(TCC_GPA(15), GPIO_FN8);
		gpio_config(TCC_GPA(16), GPIO_FN8);
		break;
	case 3:
		gpio_config(TCC_GPB(9), GPIO_FN11);
		gpio_config(TCC_GPB(10), GPIO_FN11);
		break;
	case 4:
		gpio_config(TCC_GPB(13), GPIO_FN11);
		gpio_config(TCC_GPB(14), GPIO_FN11);
		break;
	case 5:
		gpio_config(TCC_GPB(23), GPIO_FN11);
		gpio_config(TCC_GPB(24), GPIO_FN11);
		break;
	case 6:
		gpio_config(TCC_GPB(27), GPIO_FN11);
		gpio_config(TCC_GPB(28), GPIO_FN11);
		break;
	case 7:
		gpio_config(TCC_GPC(2), GPIO_FN7);
		gpio_config(TCC_GPC(3), GPIO_FN7);
		break;
	case 8:
		gpio_config(TCC_GPC(12), GPIO_FN7);
		gpio_config(TCC_GPC(13), GPIO_FN7);
		break;
	case 9:
		gpio_config(TCC_GPC(14), GPIO_FN7);
		gpio_config(TCC_GPC(15), GPIO_FN7);
		break;
	case 10:
		gpio_config(TCC_GPC(25), GPIO_FN7);
		gpio_config(TCC_GPC(26), GPIO_FN7);
		break;
	case 11:
		gpio_config(TCC_GPD(0), GPIO_FN8);
		gpio_config(TCC_GPD(1), GPIO_FN8);
		break;
	case 12:
		gpio_config(TCC_GPD(2), GPIO_FN8);
		gpio_config(TCC_GPD(3), GPIO_FN8);
		break;
	case 13:
		gpio_config(TCC_GPD(6), GPIO_FN8);
		gpio_config(TCC_GPD(7), GPIO_FN8);
		break;
	case 14:
		gpio_config(TCC_GPD(8), GPIO_FN8);
		gpio_config(TCC_GPD(9), GPIO_FN8);
		break;
	case 15:
		gpio_config(TCC_GPD(13), GPIO_FN8);
		gpio_config(TCC_GPD(14), GPIO_FN8);
		break;
	case 16:
		gpio_config(TCC_GPD(15), GPIO_FN8);
		gpio_config(TCC_GPD(16), GPIO_FN8);
		break;
	case 17:
		gpio_config(TCC_GPD(19), GPIO_FN8);
		gpio_config(TCC_GPD(20), GPIO_FN8);
		break;
	case 18:
		gpio_config(TCC_GPE(13), GPIO_FN8);
		gpio_config(TCC_GPE(14), GPIO_FN8);
		break;
	case 19:
		gpio_config(TCC_GPE(29), GPIO_FN8);
		gpio_config(TCC_GPE(30), GPIO_FN8);
		break;
	case 20:
		gpio_config(TCC_GPF(13), GPIO_FN10);
		gpio_config(TCC_GPF(14), GPIO_FN10);
		break;
	case 21:
		gpio_config(TCC_GPF(15), GPIO_FN10);
		gpio_config(TCC_GPF(16), GPIO_FN10);
		break;
	case 22:
		gpio_config(TCC_GPF(19), GPIO_FN10);
		gpio_config(TCC_GPF(20), GPIO_FN10);
		break;
	case 23:
		gpio_config(TCC_GPF(28), GPIO_FN10);
		gpio_config(TCC_GPF(29), GPIO_FN10);
		break;
	case 24:
		gpio_config(TCC_GPG(6), GPIO_FN4);
		gpio_config(TCC_GPG(7), GPIO_FN4);
		break;
	case 25:
		gpio_config(TCC_GPG(10), GPIO_FN4);
		gpio_config(TCC_GPG(11), GPIO_FN4);
		break;
	case 26:
		gpio_config(TCC_GPG(12), GPIO_FN4);
		gpio_config(TCC_GPG(13), GPIO_FN4);
		break;		
	case 27:
		gpio_config(TCC_GPG(15), GPIO_FN4);
		gpio_config(TCC_GPG(16), GPIO_FN4);
		break;		
	case 28:
		gpio_config(TCC_GPG(18), GPIO_FN4);
		gpio_config(TCC_GPG(19), GPIO_FN4);
		break;
	
	case 30:
		gpio_config(TCC_GPSD0(6), GPIO_FN4);
		gpio_config(TCC_GPSD0(7), GPIO_FN4);
		break;
	
	}
}

static void i2c_set_clock(int i2c_name, unsigned int i2c_clk_input_freq_khz)
{
	unsigned int prescale;

	prescale = i2c_clk_input_freq_khz / (i2c[i2c_name].clk * 5) - 1;
	i2c[i2c_name].regs->PRES = prescale;
	i2c[i2c_name].regs->CTRL = Hw7 | Hw6 | HwZERO;	/* start enable, stop enable, 8bit mode */
	BITSET(i2c[i2c_name].regs->CMD, Hw0);			/* clear pending interrupt */
}

static void i2c_enable(int i2c_name)
{
	int input_freq;

	tcc_set_peri(i2c[i2c_name].peri_name, ENABLE, 6000000);	/* I2C peri bus enable */
	tcc_set_iobus_pwdn(i2c[i2c_name].iobus_name, 0);				/* I2C io bus enable */
	input_freq = tcc_get_peri(i2c[i2c_name].peri_name);		/* get I2C bus clock */
	i2c_set_clock(i2c_name, (input_freq / 1000));
}

void i2c_init_early(void)
{
	void __iomem *portcfg = (void __iomem *)(HwI2C_PORTCFG_BASE + I2C_PCFG0);
	i2c_writel(0xFFFFFFFF, portcfg);
	i2c_writel(0xFFFFFFFF, portcfg+0x4);
}

void i2c_init(int ch, int port)
{
	switch (ch) {
	case I2C_CH_MASTER0:
	case I2C_CH_MASTER1:
	case I2C_CH_MASTER2:
	case I2C_CH_MASTER3:
		i2c_reset(ch);
		i2c_set_gpio(ch, port);
		i2c_enable(ch);
		break;
	case I2C_CH_SMU:
		i2c_writel(0x80000000, i2c[ch].IRQSTR);
		i2c[ch].regs->CTRL = 0;
		i2c[ch].regs->PRES = i2c[ch].clk;
		BITSET(i2c[ch].regs->CTRL, Hw7|Hw6);
		break;
	}
}

