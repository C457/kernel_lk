/*
 * Copyright (c) 2008, Google Inc.
 * Copyright (c) 2013 Telechips, Inc.
 * All rights reserved.
 *
 * Copyright (c) 2009-2011, The Linux Foundation. All rights reserved.
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
 *  * Neither the name of Google, Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
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


#include <debug.h>
#include <reg.h>
#include <sys/types.h>
#include <platform/tcc_ckc.h>
#include <platform/iomap.h>
#include <clock.h>
#define PDM_INIT_CLOCK 		(40000)

struct tcc_chip {
	void __iomem		*pwm_base;
	void __iomem		*io_pwm_base;
	unsigned int 		pwm_pclk;

};

struct tcc_chip tcc896x_pwm = {
	.pwm_base = (void __iomem *)HwPWM_BASE,
	.io_pwm_base = (void __iomem *)(HwIOBUSCFG_BASE + 0x5C),
	.pwm_pclk = PDM_INIT_CLOCK,
};

struct tcc_chip *tcc = &tcc896x_pwm;

#define PWMEN				0x4
#define PWMMODE			0x8
#define PWMPSTN(X)			(0x10 + (8*X))
#define PWMOUT1(X)			(0x30 + (0x10* X))
#define PWMOUT2(X)			(0x34 + (0x10* X))
#define PWMOUT3(X)			(0x38 + (0x10* X))
#define PWMOUT4(X)			(0x3c + (0x10* X))
#define PWM_CLOCK 			(100 * 1000 * 1000) 	//100Mhz


#define pwm_writel	writel
#define pwm_readl	readl

static int tcc_pwm_enable(unsigned int npwm)
{
	unsigned int regs;

	regs = pwm_readl(tcc->pwm_base + PWMEN);

	if(!((0x1<<(npwm)) & regs))
	{
		pwm_writel(pwm_readl(tcc->pwm_base + PWMEN) | (0x10001<<(npwm)) , tcc->pwm_base + PWMEN);
		pwm_writel(0x001, (tcc->io_pwm_base));
	}
	return 0;
}

static void tcc_pwm_disable(unsigned int npwm)
{
	pwm_writel(pwm_readl(tcc->pwm_base + PWMEN) & ~(1<<(npwm)), tcc->pwm_base + PWMEN);
}


int tcc_pwm_config(unsigned int npwm, int duty_ns, int period_ns)
{
	unsigned int reg, bit_shift =0;

	unsigned long clk_freq, clk_period_ns;
	int k, reg_n, bit_n, hi_v;
	unsigned int pwm_out[4] ={0,};
	tcc_set_peri(PERI_PDM, ENABLE,  tcc->pwm_pclk);
	
	clk_freq = tcc_get_peri(PERI_PDM);

	printf("%s  duty_ns:%ld period_ns:%ld \n", __func__, duty_ns, period_ns);

	if(clk_freq == 0)
		goto clk_error;

	clk_period_ns = (1000 * 1000 * 1000) /clk_freq;
	

	for(k = 3; k ==0;k--)		{
		if(((clk_period_ns * 1000)* 2<<k) < period_ns)
			break;
	}

	hi_v = (128 *duty_ns)/period_ns;
	
	
	for( reg_n = 0; reg_n < (hi_v/32); reg_n++)
	{
		pwm_out[reg_n] = 0xFFFFFFFF;
	}

	if((hi_v/32) < 4)	{
		for( bit_n = 0; bit_n < (hi_v % 32); bit_n++)
			pwm_out[reg_n] = (1 << (bit_n)) | (pwm_out[reg_n]);
	}

	reg = pwm_readl(tcc->pwm_base + PWMMODE);
	bit_shift = 4 * npwm;

	reg = (reg & ~(0xF << bit_shift)) | 4 << bit_shift;  
	pwm_writel(reg, tcc->pwm_base + PWMMODE);  //phase mode

	reg = pwm_readl(tcc->pwm_base + PWMMODE);
	bit_shift = (2*npwm) + 24;
	reg = (reg & ~(0x3 << bit_shift)) | k  << bit_shift;
	pwm_writel(reg, tcc->pwm_base + PWMMODE);  //divide

	clk_period_ns = clk_period_ns * ( 2<<k);

	pwm_writel(pwm_out[0], tcc->pwm_base + PWMOUT1(npwm));  //duty_ns
	pwm_writel(pwm_out[1], tcc->pwm_base + PWMOUT2(npwm));  //duty_ns
	pwm_writel(pwm_out[2], tcc->pwm_base + PWMOUT3(npwm));  //duty_ns
	pwm_writel(pwm_out[3], tcc->pwm_base + PWMOUT4(npwm));  //duty_ns

	tcc_pwm_enable(npwm);	

	return 0;

clk_error:
	printf("%s ERROR clk_freq:%ld \n", __func__, clk_freq);

	return 0;
	
}

/* Initialize timer */
void pwm_init_early(void)
{
	pwm_writel(0x0, (HwPWM_BASE + 0x04));
}

