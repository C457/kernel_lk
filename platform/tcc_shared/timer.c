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
#include <reg.h>
#include <sys/types.h>

#include <platform/timer.h>
#include <platform/irqs.h>
#include <platform/iomap.h>
#include <platform/interrupts.h>
#include <platform/tcc_ckc.h>
#include <clock.h>
#include <kernel/thread.h>

#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

#define SPSS_TIMER_STATUS_DGT_EN    (1 << 0)

static platform_timer_callback timer_callback;
static void *timer_arg;
static time_t timer_interval;
static time_t tick_interval;

#define TIMER_REG(off)	*REG32(TCC_TIMER_BASE + (off))

#define TIMER0		0x00
#define TIMER1		0x10
#define TIMER2		0x20
#define TIMER3		0x30
#define TIMER4		0x40
#define TIMER5		0x50

#define TIMER_TCFG	0x00
#define TIMER_TCNT	0x04
#define TIMER_TREF	0x08
#define TIMER_TMREF	0x0C

#define TIMER_TC32EN	0x80		/* 32-bit counter enable / pre-scale value */
#define TIMER_TC32LDV	0x84		/* 32-bit counter load value */
#define TIMER_TC32CMP0	0x88		/* 32-bit counter match value 0 */
#define TIMER_TC32CMP1	0x8C		/* 32-bit */
#define TIMER_TC32PCNT	0x90
#define TIMER_TC32MCNT	0x94
#define TIMER_TC32IRQ	0x98

#define TIMER_TICK_RATE	12000000


static volatile uint32_t ticks;
static uint32_t ticks_per_sec = 0;

static enum handler_return timer_irq(void *arg)
{
	if (readl(HwTMR_BASE + 0x98) & (1<<31))
		writel((1<<31), (HwTMR_BASE + 0x98));
	ticks += timer_interval;
	return timer_callback(timer_arg, ticks);
}

status_t
platform_set_periodic_timer(platform_timer_callback callback,
			    void *arg, time_t interval)
{
	uint32_t tick_count = interval * (ticks_per_sec / 1000);
	register_int_handler(INT_TIMER1, timer_irq, 0);

	enter_critical_section();

	timer_callback = callback;
	timer_arg = arg;
	timer_interval = interval;

	tick_interval = interval;

	writel(0, (HwTMR_BASE + 0x80));	/* stop the timer */
	writel(0, (HwTMR_BASE + 0x84));
	writel((1<<19), (HwTMR_BASE + 0x98));
	writel((tick_count | (1 << 24)), (HwTMR_BASE + 0x80));
	unmask_interrupt(INT_TIMER1);

	exit_critical_section();
	return 0;
}

time_t current_time(void)
{
//+[TCCQB] To Check log time by timer.
//	return ticks;
	return TIMER_REG(TIMER_TC32MCNT) * tick_interval;
//-[TCCQB]
//

}

static void wait_for_timer_op(void)
{
#if (0)
	while (readl(SPSS_TIMER_STATUS) & SPSS_TIMER_STATUS_DGT_EN) ;
#endif
}

void platform_uninit_timer(void)
{
	writel(0x7fff, (HwTMR_BASE + 0x80));	/* stop the timer */
	writel(0x00000000, (HwTMR_BASE + 0x98));
}

void mdelay(volatile unsigned msecs)
{
	msecs *= 375;
	TIMER_REG(TIMER4+TIMER_TCNT) = 0x0;
	TIMER_REG(TIMER4+TIMER_TCFG) |= 0x1;
	while (TIMER_REG(TIMER4+TIMER_TCNT) < msecs);
	TIMER_REG(TIMER4+TIMER_TCFG) &= ~0x1;
	TIMER_REG(TIMER4+TIMER_TCNT) = 0x0;
}

void udelay(unsigned usecs)
{
	usecs *= 3;
	TIMER_REG(TIMER3+TIMER_TCNT) = 0x0;
	TIMER_REG(TIMER3+TIMER_TCFG) |= 0x1;
	while (TIMER_REG(TIMER3+TIMER_TCNT) < usecs);
	TIMER_REG(TIMER3+TIMER_TCFG) &= ~0x1;
	TIMER_REG(TIMER3+TIMER_TCNT) = 0x0;
}

/* Return current time in micro seconds */
bigtime_t current_time_hires(void)
{
	return ticks * 1000ULL;
}

/* Initialize timer */
void timer_init_early(void)
{
	/* 32-bit timer */
	tcc_set_peri(PERI_TCZ, ENABLE, 12000000);
	ticks_per_sec = tcc_get_peri(PERI_TCZ);

	/* timer3(16-bit) for udelay */
	tcc_set_peri(PERI_TCT, ENABLE, 12000000);
	TIMER_REG(TIMER3+TIMER_TCFG) = (1<<4);
	TIMER_REG(TIMER3+TIMER_TREF) = 0xFFFF;
	TIMER_REG(TIMER3+TIMER_TMREF) = 0x0;

	/* timer4(20-bit) for mdelay */
	tcc_set_peri(PERI_TCT, ENABLE, 12000000);
	TIMER_REG(TIMER4+TIMER_TCFG) = (4<<4);
	TIMER_REG(TIMER4+TIMER_TREF) = 0xFFFFF;
	TIMER_REG(TIMER4+TIMER_TMREF) = 0x0;
}

