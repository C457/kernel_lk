/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/system.h>

#include <asm/arch/globals.h>
#include <asm/arch/tcc_ckc.h>

extern void clock_init_early(void);
extern void gpio_init_early(void);

int arch_cpu_init(void)
{
	clock_init_early();
	gpio_init_early();

	return 0;
}
