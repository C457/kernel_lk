/*
 *  Copyright (C) 2013 Telechips
 *  Telechips <linux@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/globals.h>
#include <asm/arch/clock.h>
#include <asm/arch/reg_physical.h>
#include <asm/gpio.h>
#include <asm/arch/tcc_ckc.h>
#include <asm/arch/i2c.h>
#include <asm/arch/sys_proto.h>
#include <power/da9062.h>
#include <power/rt5028.h>

enum {
    PWR_CPU,
    PWR_CPU0,
    PWR_CPU1,
    PWR_CORE,
    PWR_MEM,
    PWR_SDIO,
    PWR_IOD0,
    PWR_IOD1,
    PWR_IOD2,
    PWR_GV,
    PWR_IO,
    PWR_HDMI_12D,
    PWR_HDMI_33D,
    PWR_BOOST_5V,
};

DECLARE_GLOBAL_DATA_PTR;

void clock_init(void)
{
#if defined(CONFIG_POWER_DA9062)
	da9062_init(I2C_CH_MASTER0);
	da9062_set_voltage( PWR_CPU0,  1100);
	da9062_set_voltage( PWR_CORE,  1000);
	da9062_set_voltage(   PWR_GV,  1000);
#endif

#if defined(CONFIG_POWER_RT5028)
	rt5028_init(I2C_CH_MASTER3);
	rt5028_set_voltage(RT5028_ID_BUCK1, 1000);	// PWRCORE
	rt5028_set_voltage(RT5028_ID_BUCK2, 1500);	// MEM
	rt5028_set_voltage(RT5028_ID_BUCK3, 1100);	// PWRCPU
	rt5028_set_voltage(RT5028_ID_BUCK4, 3300);	// PWR_IO(PWRUSB|PWRLVDS|ETC)

	rt5028_set_voltage(RT5028_ID_LDO2, 1800);	// VD2_CORE
	rt5028_set_voltage(RT5028_ID_LDO2, 1800);	// PWRPLL25|PWROTP25
	rt5028_set_voltage(RT5028_ID_LDO3, 3300);	// WIFI/BT_IO
	rt5028_set_voltage(RT5028_ID_LDO4, 3300);	// WIFI/BT_VBAT
	rt5028_set_voltage(RT5028_ID_LDO5, 3300);	// GPS
	rt5028_set_voltage(RT5028_ID_LDO6, 3300);	// VD1_IO
	rt5028_set_voltage(RT5028_ID_LDO7, 1800);	// VD1_CORE
	rt5028_set_voltage(RT5028_ID_LDO8, 3300);	// VD2_IO
#endif

	tcc_set_clkctrl( FBUS_CPU0,       ENABLE,  900000000);
	tcc_set_clkctrl( FBUS_CPU1,      DISABLE,  800000000);
//	tcc_set_clkctrl( FBUS_MEM,        ENABLE,  600000000);	/* Do not change membus */
	tcc_set_clkctrl( FBUS_DDI,        ENABLE,  400000000);
	tcc_set_clkctrl( FBUS_GPU,        ENABLE,  525000000);
	tcc_set_clkctrl( FBUS_IO,         ENABLE,  250000000);
	tcc_set_clkctrl( FBUS_VBUS,      DISABLE,  360000000);
	tcc_set_clkctrl( FBUS_CODA,      DISABLE,  360000000);
#if !defined(TSBM_ENABLE)
	//tcc_set_clkctrl( FBUS_HSIO,       ENABLE,  333333334);
	tcc_set_clkctrl( FBUS_HSIO, 	  ENABLE,  250000000);	// for GMAC
#endif
	tcc_set_clkctrl( FBUS_SMU,        ENABLE,  200000000);
	tcc_set_clkctrl( FBUS_G2D,       DISABLE,  800000000);
#if !defined(TSBM_ENABLE) && !defined(CONFIG_ARM_TRUSTZONE)
	tcc_set_clkctrl( FBUS_CMBUS,     DISABLE,  300000000);
#endif
	tcc_set_clkctrl( FBUS_HEVC_VCE,  DISABLE,  360000000);
	tcc_set_clkctrl( FBUS_HEVC_VCPU, DISABLE,  360000000);
	tcc_set_clkctrl( FBUS_HEVC_BPU,  DISABLE,  360000000);

}

void clock_init_early(void)
{
	tcc_ckc_init();

	/* change clock sourt to XIN before change PLL values. */
	tcc_set_clkctrl(FBUS_IO,   ENABLE, XIN_CLK_RATE);
	tcc_set_clkctrl(FBUS_SMU,  ENABLE, XIN_CLK_RATE);
#if !defined(TSBM_ENABLE)
	tcc_set_clkctrl(FBUS_HSIO, ENABLE, XIN_CLK_RATE);
#endif
#if defined(TSBM_ENABLE) || defined(CONFIG_ARM_TRUSTZONE)
	tcc_set_clkctrl(FBUS_CMBUS, ENABLE, XIN_CLK_RATE);
#endif

	/* need to change cpu0/1 source.  pll6 -> pll0/pll1 */
	tcc_set_clkctrl( FBUS_CPU0,    ENABLE, 540000000);	/* 600MHz@0.9V */

	/*
	 * pll3: cpu(cortex-a7) only
	 * pll4: memory only
	 */
	tcc_set_pll(PLL_0,  ENABLE, 1000000000, PLLDIV_4);
#if !defined(CONFIG_USB_UPDATE)
	tcc_set_pll(PLL_1,  ENABLE,  768000000, PLLDIV_2);
#endif
	tcc_set_pll(PLL_2,  ENABLE, 1080000000, PLLDIV_3);	/* for setting 360MHz at VBUS */
//	tcc_set_pll(PLL_2,  ENABLE, 1188000000, PLLDIV_3);	/* for supporting component/composite */


	tcc_set_peri(PERI_UART0,  ENABLE,  48000000);
	tcc_set_peri(PERI_TCZ,	 ENABLE,  12000000);
	tcc_set_peri(PERI_TCT,	 ENABLE,  12000000);

	tcc_set_peri(PERI_GMAC,	 	ENABLE,  125000000);
	tcc_set_peri(PERI_GMAC_PTP,	ENABLE,   50000000);

	tcc_set_peri(PERI_USB_OTG, ENABLE,		48000000);
	tcc_set_peri(PERI_USB_OTG_ADP, ENABLE,		48000000);
}

