/*
 * Copyright (C) 2016 Hosiroh <rohhosik@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <linux/err.h>
#include <linux/io.h>
#include <usb.h>
#include <asm/arch/gpio.h>
#include "ehci.h"

DECLARE_GLOBAL_DATA_PTR;

#define tcc_ehci_readl(r)       	readl(r)
#define tcc_ehci_writel(v,r)        writel(v,r)

#define TCC_EHCI_PHY_BCFG           0x00
#define TCC_EHCI_PHY_PCFG0          0x04
#define TCC_EHCI_PHY_PCFG1          0x08
#define TCC_EHCI_PHY_PCFG2          0x0C
#define TCC_EHCI_PHY_PCFG3          0x10
#define TCC_EHCI_PHY_PCFG4          0x14
#define TCC_EHCI_PHY_STS            0x18
#define TCC_EHCI_PHY_LCFG0          0x1C
#define TCC_EHCI_PHY_LCFG1          0x20
#define TCC_EHCI_HSIO_CFG           0x40

extern 	void gpio_set(unsigned n, unsigned on);

void tcc_USB20H_PICO_PHY_init(void)
{
    int i;
    //u32 temp;

    //if (tcc_ehci->otg_mux) {
    //    printk("this port is a otg mux.\n");
    //    temp = tcc_ehci_readl(tcc_ehci->phy_regs+0x4C);
    //    if(tcc_ehci->otg_mux)
    //        temp &= (~0x1);
    //    else
    //        temp |= 0x1;
    //    tcc_ehci_writel(temp, tcc_ehci->phy_regs+0x4C);
    //    tcc_ehci_phy_reset(tcc_ehci->phy_regs+0x4C);
    //}

    // Reset PHY Registers
    tcc_ehci_writel(0x03000115, CONFIG_USB_EHCI0_PHY_REG_ADDR + TCC_EHCI_PHY_PCFG0);
    tcc_ehci_writel(0x0334D143, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG1);
    tcc_ehci_writel(0x4, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG2);
    tcc_ehci_writel(0x0, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG3);
    tcc_ehci_writel(0x0, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG4);
    tcc_ehci_writel(0x30048020, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_LCFG0);

    // Set the POR
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0) | Hw31, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0);
    // Set the Core Reset
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_LCFG0) & 0xCFFFFFFF, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_LCFG0);

    // Wait 10 usec
    udelay(10);

    // Release POR
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0) & ~(Hw31), CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0);
    // Clear SIDDQ
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0) & ~(Hw24), CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0);
    // Set Phyvalid en
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG4) | Hw30, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG4);
    // Set DP/DM (pull down)
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG4) | 0x1400, CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG4);

    // Wait Phy Valid Interrupt
    i = 0;
    while (i < 10000) {
        if ((tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR + +TCC_EHCI_PHY_PCFG0) & Hw21)) break;
        i++;
        udelay(5);
    }
    printf("PHY valid check %s\x1b[0m\n",i>=9999?"fail!":"pass.");

    // Release Core Reset
    tcc_ehci_writel(tcc_ehci_readl(CONFIG_USB_EHCI0_PHY_REG_ADDR+TCC_EHCI_PHY_LCFG0) | 0x30000000, CONFIG_USB_EHCI0_PHY_REG_ADDR+TCC_EHCI_PHY_LCFG0);
}

static int get_tcc_ehci_base(int index, struct ehci_hccr **base)
{
	*base = (struct ehci_hccr *)CONFIG_USB_EHCI0_REG_ADDR;
	return 0;
}

void tcc_ehci_vbus_ctrl(int on_off)
{
	#if defined(CONFIG_TCC8980_STB) || defined(CONFIG_TCC8985_OTT) || defined(CONFIG_TCC8985_STICK)
	gpio_config(TCC_GPC(20),  GPIO_FN0|GPIO_OUTPUT);
	gpio_set(TCC_GPC(20), on_off);
	#else
	printf("\x1b[1;31m[%s:%d] port setup??\x1b[0m\n", __func__, __LINE__);
	#endif
}

int ehci_hcd_init(int index, enum usb_init_type init, struct ehci_hccr **hccr,
		  struct ehci_hcor **hcor)
{
	int ret;
	struct ehci_hccr *cr;
	struct ehci_hcor *or;

	tcc_ehci_vbus_ctrl(ON);

	tcc_USB20H_PICO_PHY_init();

	ret = get_tcc_ehci_base(index, &cr);
	if (ret < 0)
		return ret;
	or = (void *)cr + HC_LENGTH(ehci_readl(&cr->cr_capbase));

	*hccr = cr;
	*hcor = or;

	return 0;
}

int ehci_hcd_stop(int index)
{
	return 0;
}
