/*
 * (C) Copyright 2016 Telechips
 * Telechips <linux@telechips.com>
 *
 * Configuation settings for the TCC8971 EVB board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_TCC8971_EVB_H
#define __CONFIG_TCC8971_EVB_H

#include <configs/tcc897x-common.h>

/**********************************************************
 * Define board revision
 **********************************************************/
#define BOARD_NAME					"TCC897X EVB"
#define CONFIG_TCC_BOARD_REV		0x1000

/***********************************************************
 * Physical Memory configuration for U-Boot
 ***********************************************************/
#define PHYS_DDR_BASE				0x80000000	/* DDR Base Address */
#define PHYS_DDR_SIZE				(2048)		/* 2048 MB */

#define CONFIG_NR_DRAM_BANKS		1  			/* numbers of ATAG_MEM */

/*
 * Memory Map for U-boot
 */
#define CONFIG_SYS_SDRAM_BASE		PHYS_DDR_BASE /* DDR base */
#ifdef CONFIG_USB_UPDATE
#define CONFIG_SYS_TEXT_BASE		0x80000000 /* text base */
#else
#define CONFIG_SYS_TEXT_BASE		0x82000000 /* text base */
#endif
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + (PHYS_DDR_SIZE << 20)) /* stack pointer */
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x8000) /* default img load address */
#define CONFIG_STANDALONE_LOAD_ADDR	0x90000000 /* Standalone programs base addr (ex. hello world) */
#define CONFIG_TAGS_ADDR			0x80000100

#define BASE_ADDR					(CONFIG_SYS_TEXT_BASE)
#define SCRATCH_ADDR     			(BASE_ADDR + 0x06000000)
#define KERNEL_ADDR					(BASE_ADDR + 0x00008000)
#define MEMBASE						(CONFIG_SYS_TEXT_BASE)

/**********************************************************
 * DDR Configurations
 **********************************************************/
#define PRE_SET_PMIC
#define DRAM_DDR3
#define	CONFIG_DDR3_MODULE_512MB
#define CONFIG_DDR3_1866
#define CONFIG_DDR3_IF_16
#define CONFIG_DDR3_2CS

/**********************************************************
 * PMIC Driver
 **********************************************************/
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_DA9062

/**********************************************************
 * LCD Configurations
 **********************************************************/
#define GPIO_LCD_ON					(GPIO_PORTEXT2|1)
#define GPIO_LCD_BL					TCC_GPC(23)
#define GPIO_LCD_DISPLAY			(GPIO_PORTEXT2|2)
#define GPIO_LCD_RESET				TCC_GPB(29)
#define LCDC_NUM					1

#define GPIO_LVDS_STBYB				TCC_GPB(19)
#define GPIO_LVDS_EN				TCC_GPEXT1(36)
#define GPIO_V_5P0_EN				TCC_GPEXT1(32)

/**********************************************************
 * USB Driver
 **********************************************************/
#define GPIO_OTG_VBUS				GPIO_NC

/**********************************************************
 * I2C Configurations
 **********************************************************/
#define I2C_PORT_NC					0xFFFFFFFF

#define I2C_CH0_PORT				18
#define I2C_CH1_PORT				20
#define I2C_CH2_PORT				21
#define I2C_CH3_PORT				I2C_PORT_NC

#endif
