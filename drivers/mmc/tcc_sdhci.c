/*
 * (C) Copyright 2014 Telechips Inc.
 *  Telechips <linux@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/arch/sdmmc/sd_hw.h>
#include <asm/io.h>
#include <asm/arch/reg_physical.h>

int tcc_sdhci_init(u32 regbase, int index, int bus_width)
{
	struct sdhci_host *host = NULL;
	host = (struct sdhci_host *)calloc(1, sizeof(struct sdhci_host));
	if (!host) {
		printf("sdhci__host malloc fail!\n");
		return 1;
	}

	host->name = "tcc_sdhci";
	host->ioaddr = (void *)regbase;

	host->quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_BROKEN_VOLTAGE |
		SDHCI_QUIRK_BROKEN_R1B | SDHCI_QUIRK_32BIT_DMA_ADDR |
		SDHCI_QUIRK_WAIT_SEND_CMD | SDHCI_QUIRK_USE_WIDE8;
	host->voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);

#ifdef CONFIG_MMC_SDMA
	host->quirks &= !SDHCI_QUIRK_WAIT_SEND_CMD;
#endif

	//SD_HW_InitPower();
	SD_HW_Initialize();
	host->clock = 500000*100;
	host->set_clock = NULL;
	host->index = index;
	
	switch(index){
		case 0:
			writel(0x0CFF9870 , HwSDMMC_CHCTRL_CH0_CAP0);
			break;
		case 1:
			writel(0x0CFF9870 , HwSDMMC_CHCTRL_CH1_CAP0);
			break;
		case 2:
			writel(0x0CFF9870 , HwSDMMC_CHCTRL_CH2_CAP0);
			break;
		case 3:
			writel(0x0CFF9870 , HwSDMMC_CHCTRL_CH3_CAP0);
			break;
		default :
			break;
	}

	if (bus_width == 8)
		host->host_caps |= MMC_MODE_8BIT;

	return add_sdhci(host, 50000000, 400000);
}
