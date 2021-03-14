/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>

DECLARE_GLOBAL_DATA_PTR;

int tcc_set_ipisol_pwdn(int id, bool pwdn)
{
	if (gd->arch.tcc.clock_ops->ckc_ip_pwdn && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_ip_pwdn(id, pwdn);

	return -CKC_NO_OPS_DATA;
}

int tcc_set_pll(int id, bool en, unsigned long rate, plldiv_t div)
{
	if (div) {
		if (gd->arch.tcc.clock_ops->ckc_plldiv_set && gd->arch.tcc.clock_ops)
			gd->arch.tcc.clock_ops->ckc_plldiv_set(id, (div>1)?(div-1):0);
	}
	if (gd->arch.tcc.clock_ops->ckc_pll_set_rate && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_pll_set_rate(id, rate);

	return -CKC_NO_OPS_DATA;
}

unsigned long tcc_get_pll(int id)
{
	if (gd->arch.tcc.clock_ops->ckc_pll_get_rate && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_pll_get_rate(id);

	return 0;
}

int tcc_set_clkctrl(int id, bool en, unsigned long rate)
{
	int ret;
	if (gd->arch.tcc.clock_ops->ckc_clkctrl_set_rate && gd->arch.tcc.clock_ops) {
		ret = gd->arch.tcc.clock_ops->ckc_clkctrl_set_rate(id, rate);
		if (ret)
			return ret;

		if (en) {
			if (gd->arch.tcc.clock_ops->ckc_pmu_pwdn)
				gd->arch.tcc.clock_ops->ckc_pmu_pwdn(id, 0);
			if (gd->arch.tcc.clock_ops->ckc_swreset)
				gd->arch.tcc.clock_ops->ckc_swreset(id, 0);
			if (gd->arch.tcc.clock_ops->ckc_clkctrl_enable)
				ret = gd->arch.tcc.clock_ops->ckc_clkctrl_enable(id);
		}
		else {
			if (gd->arch.tcc.clock_ops->ckc_clkctrl_disable)
				ret = gd->arch.tcc.clock_ops->ckc_clkctrl_disable(id);
			if (gd->arch.tcc.clock_ops->ckc_swreset)
				gd->arch.tcc.clock_ops->ckc_swreset(id, 1);
			if (gd->arch.tcc.clock_ops->ckc_pmu_pwdn)
				gd->arch.tcc.clock_ops->ckc_pmu_pwdn(id, 1);
		}
		return ret;
	}

	return -CKC_NO_OPS_DATA;
}

unsigned long tcc_get_clkctrl(int id)
{
	if (gd->arch.tcc.clock_ops->ckc_clkctrl_get_rate && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_clkctrl_get_rate(id);

	return 0;
}

int tcc_set_peri(int id, bool en, unsigned long rate)
{
	int ret;
	if (gd->arch.tcc.clock_ops->ckc_peri_set_rate && gd->arch.tcc.clock_ops) {
		ret = gd->arch.tcc.clock_ops->ckc_peri_set_rate(id, rate);
		if (ret)
			return ret;

		if (en) {
			if (gd->arch.tcc.clock_ops->ckc_peri_enable)
				ret = gd->arch.tcc.clock_ops->ckc_peri_enable(id);
		}
		else {
			if (gd->arch.tcc.clock_ops->ckc_peri_disable)
				ret = gd->arch.tcc.clock_ops->ckc_peri_disable(id);
		}
		return ret;
	}

	return -CKC_NO_OPS_DATA;
}

unsigned long tcc_get_peri(int id)
{
	if (gd->arch.tcc.clock_ops->ckc_peri_get_rate && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_peri_get_rate(id);

	return 0;
}

int tcc_set_ddibus_pwdn(int id, bool pwdn)
{
	if (gd->arch.tcc.clock_ops->ckc_ddibus_pwdn && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_ddibus_pwdn(id, pwdn);

	return -CKC_NO_OPS_DATA;
}

int tcc_set_ddibus_swreset(int id, bool reset)
{
	if (gd->arch.tcc.clock_ops->ckc_ddibus_swreset && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_ddibus_swreset(id, reset);

	return -CKC_NO_OPS_DATA;
}

int tcc_set_iobus_pwdn(int id, bool pwdn)
{
	if (gd->arch.tcc.clock_ops->ckc_iobus_pwdn && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_iobus_pwdn(id, pwdn);

	return -CKC_NO_OPS_DATA;
}

int tcc_set_iobus_swreset(int id, bool reset)
{
	if (gd->arch.tcc.clock_ops->ckc_iobus_swreset && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_iobus_swreset(id, reset);

	return -CKC_NO_OPS_DATA;
}

int tcc_set_hsiobus_pwdn(int id, bool pwdn)
{
	if (gd->arch.tcc.clock_ops->ckc_hsiobus_pwdn && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_hsiobus_pwdn(id, pwdn);

	return -CKC_NO_OPS_DATA;
}

int tcc_set_hsiobus_swreset(int id, bool reset)
{
	if (gd->arch.tcc.clock_ops->ckc_hsiobus_swreset && gd->arch.tcc.clock_ops)
		return gd->arch.tcc.clock_ops->ckc_hsiobus_swreset(id, reset);

	return -CKC_NO_OPS_DATA;
}

/*
 * Register clock ops
 */
void tcc_ckc_set_ops(struct tcc_ckc_ops *ops)
{
	gd->arch.tcc.clock_ops = ops;
}

