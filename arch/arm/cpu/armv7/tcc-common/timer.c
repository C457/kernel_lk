#include <common.h>
#include <div64.h>
#include <asm/io.h>
#include <pwm.h>
#include <asm/arch/reg_physical.h>

DECLARE_GLOBAL_DATA_PTR;

#define REG32(addr) ((volatile uint32_t *)(addr))
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
#define TIMER_TICK_RATE 12000000 //ZCLK


int timer_init(void)
{
	//volatile PTIMER pTimer= (PTIMER)HwTMR_BASE;
	uint32_t ticks_per_interval = TIMER_TICK_RATE / CONFIG_SYS_HZ; // interval is in ms

	writel(0, (HwTMR_BASE + 0x80));	/* stop the timer */
	writel(0, (HwTMR_BASE + 0x84));
	writel((1<<19), (HwTMR_BASE + 0x98));
	writel((ticks_per_interval | (1 << 24)), (HwTMR_BASE + 0x80));

	return 0;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
//	volatile PTIMER pTimer= (PTIMER)HwTMR_BASE;

	return (unsigned long long)TIMER_REG(TIMER_TC32MCNT);
}

ulong timer_get_boot_us(void)
{
	return (ulong)(get_timer(0)*1000);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
unsigned long get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}

