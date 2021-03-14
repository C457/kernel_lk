/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#include <common.h>
#include <fdtdec.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/arch/iomap.h>
#include <asm/arch/reg_physical.h>

DECLARE_GLOBAL_DATA_PTR;

#define IOBUSCFG_HCLKEN0        (TCC_IOBUSCFG_BASE + 0x10)
#define IOBUSCFG_HCLKEN1        (TCC_IOBUSCFG_BASE + 0x14)
#define PMU_WATCHDOG            (TCC_PMU_BASE + 0x08)
#define PMU_CONFIG1             (TCC_PMU_BASE + 0x14)
#define PMU_USSTATUS            (TCC_PMU_BASE + 0x1C)

void reset_cpu(ulong reboot_reason)
{
	unsigned int usts;
	
	usts = (reboot_reason == 0x77665500) ? 1 : 0 ;

	writel(usts, PMU_USSTATUS);
	writel((readl(PMU_CONFIG1) & 0xCFFFFFFF), PMU_CONFIG1);

	writel(0xFFFFFFFF, IOBUSCFG_HCLKEN0);
	writel(0xFFFFFFFF, IOBUSCFG_HCLKEN1);

	while(1) writel((1<<31) | 1 , PMU_WATCHDOG);
}

#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void)
{

	printf("CPU:   %s \n", "TCC897x");

	return 0;
}
#endif

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	dcache_enable();
}
#endif

