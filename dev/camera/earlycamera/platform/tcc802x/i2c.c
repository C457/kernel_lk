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
#include <i2c.h>
#include <tcc_ckc.h>
#include <globals.h>

#define i2c_readl	readl
#define i2c_writel	writel
#define I2C0_BASE			0x76300000
#define I2C1_BASE			0x76310000
#define I2C2_BASE			0x76320000
#define I2C3_BASE			0x76330000
#define I2C_PCFG_BASE		0x763C0000

#define I2C_SMU_BASE		TCC_SMU_I2C_CH0_BASE

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
		.IRQSTR = I2C_PCFG_BASE + 0x10,
		.regs = (volatile struct tcc_i2c_regs *)(I2C0_BASE),
		.peri_name = PERI_I2C0,
		.iobus_name = IOBUS_I2C_M0,
		.is_reset = 0,
	},
	[I2C_CH_MASTER1] = {
		.clk = 400,
		.IRQSTR = I2C_PCFG_BASE + 0x10,
		.regs = (volatile struct tcc_i2c_regs *)(I2C1_BASE),
		.peri_name = PERI_I2C1,
		.iobus_name = IOBUS_I2C_M1,
		.is_reset = 0,
	},
	[I2C_CH_MASTER2] = {
		.clk = 100,
		.IRQSTR = I2C_PCFG_BASE + 0x10,
		.regs = (volatile struct tcc_i2c_regs *)(I2C2_BASE),
		.peri_name = PERI_I2C2,
		.iobus_name = IOBUS_I2C_M2,
		.is_reset = 0,
	},
	[I2C_CH_MASTER3] = {
		.clk = 100,
		.IRQSTR = I2C_PCFG_BASE + 0x10,
		.regs = (volatile struct tcc_i2c_regs *)(I2C3_BASE),
		.peri_name = PERI_I2C3,
		.iobus_name = IOBUS_I2C_M3,
		.is_reset = 0,
	},
};

static int wait_intr(int i2c_name)
{
	volatile unsigned long cnt = 0;

#if 1	
	//while (!(i2c_readl(i2c[i2c_name].IRQSTR) & (1<<i2c_name))) {
	while(1)
	{
		cnt++;
		// check i2c status register interrupt flag
		if ((i2c[i2c_name].regs->SR & Hw0))
			break;
		if (cnt > 100000) {
			printk("i2c-tcc: time out!\n");
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
			printk("i2c-tcc: time out!\n");
			return -1;
		}
	}
#endif

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

