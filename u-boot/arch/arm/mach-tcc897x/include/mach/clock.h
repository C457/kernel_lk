/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */



#ifndef __CLOCK_OPS_H__
#define __CLOCK_OPS_H__

#include <common.h>
#include <asm/arch/globals.h>

enum {
	CKC_OK = 0,
	CKC_FAILED,
	CKC_INVALIED_SRC,
	CKC_NO_OPS_DATA = 10,
};

typedef enum {
	PLLDIV_0 = 0,	/* Disable */
	PLLDIV_2 = 2,	/* 1/2 */
	PLLDIV_3,	/* 1/3 */
	PLLDIV_4,	/* 1/4 */
} plldiv_t;

struct tcc_ckc_ops {
	/* pmu pwdn */
	int (*ckc_pmu_pwdn)(int id, bool pwdn);
	int (*ckc_is_pmu_pwdn)(int id);

	/* software reset */
	int (*ckc_swreset)(int id, bool reset);

	/* ip isolrate */
	int (*ckc_ip_pwdn)(int id, bool pwdn);
	int (*ckc_is_ip_pwdn)(int id);

	/* pll */
	int (*ckc_pll_set_rate)(int id, unsigned long rate);
	unsigned long (*ckc_pll_get_rate)(int id);
	int (*ckc_plldiv_set)(int id, unsigned int div);

	/* clkctrl */
	int (*ckc_clkctrl_enable)(int id);
	int (*ckc_clkctrl_disable)(int id);
	int (*ckc_clkctrl_set_rate)(int id, unsigned long rate);
	unsigned long (*ckc_clkctrl_get_rate)(int id);
	int (*ckc_is_clkctrl_enabled)(int id);

	/* peripheral */
	int (*ckc_peri_enable)(int id);
	int (*ckc_peri_disable)(int id);
	int (*ckc_peri_set_rate)(int id, unsigned long rate);
	unsigned long (*ckc_peri_get_rate)(int id);
	int (*ckc_is_peri_enabled)(int id);

	/* display bus */
	int (*ckc_ddibus_pwdn)(int id, bool pwdn);
	int (*ckc_is_ddibus_pwdn)(int id);
	int (*ckc_ddibus_swreset)(int id, bool reset);

	/* graphic bus */
	int (*ckc_gpubus_pwdn)(int id, bool pwdn);
	int (*ckc_is_gpubus_pwdn)(int id);
	int (*ckc_gpubus_swreset)(int id, bool reset);

	/* io bus */
	int (*ckc_iobus_pwdn)(int id, bool pwdn);
	int (*ckc_is_iobus_pwdn)(int id);
	int (*ckc_iobus_swreset)(int id, bool reset);

	/* video bus */
	int (*ckc_vpubus_pwdn)(int id, bool pwdn);
	int (*ckc_is_vpubus_pwdn)(int id);
	int (*ckc_vpubus_swreset)(int id, bool reset);

	/* hsio bus */
	int (*ckc_hsiobus_pwdn)(int id, bool pwdn);
	int (*ckc_is_hsiobus_pwdn)(int id);
	int (*ckc_hsiobus_swreset)(int id, bool reset);

	/* g2d bus */
	int (*ckc_g2dbus_pwdn)(int id, bool pwdn);
	int (*ckc_is_g2dbus_pwdn)(int id);
	int (*ckc_g2dbus_swreset)(int id, bool reset);

	/* cortex-m bus */
	int (*ckc_cmbus_pwdn)(int id, bool pwdn);
	int (*ckc_is_cmbus_pwdn)(int id);
	int (*ckc_cmbus_swreset)(int id, bool reset);
};

extern void tcc_ckc_set_ops(struct tcc_ckc_ops *ops);

extern int tcc_set_ipisol_pwdn(int id, bool pwdn);
extern int tcc_set_pll(int id, bool en, unsigned long rate, plldiv_t div);
extern unsigned long tcc_get_pll(int id);
extern int tcc_set_clkctrl(int id, bool en, unsigned long rate);
extern unsigned long tcc_get_clkctrl(int id);
extern int tcc_set_peri(int bus_type, bool en, unsigned long rate);
extern unsigned long tcc_get_peri(int id);
extern int tcc_set_ddibus_pwdn(int id, bool pwdn);
extern int tcc_set_ddibus_swreset(int id, bool reset);
extern int tcc_set_iobus_pwdn(int id, bool pwdn);
extern int tcc_set_iobus_swreset(int id, bool reset);
extern int tcc_set_hsiobus_pwdn(int id, bool pwdn);
extern int tcc_set_hsiobus_swreset(int id, bool reset);
extern void tcc_ckc_reset_ops(void);

#endif /* __CLOCK_H__ */
