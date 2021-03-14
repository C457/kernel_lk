 /* 
 *
 * Copyright (C) 2015 Telechips, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include <asm/arch/gpio.h>

#define EMMC51 0
#if 0
#define HwSDMMC0_BASE						NULL
#define HwSDMMC1_BASE						NULL
#define HwSDMMC2_BASE						NULL
#define HwSDMMC3_BASE						NULL
#define HwSDMMC_CHCTRL_BASE					NULL

#define HwSDMMC_CHCTRL_CH0_CAP0					(HwSDMMC_CHCTRL_BASE + 0x04)
#define HwSDMMC_CHCTRL_CH0_CAP1					(HwSDMMC_CHCTRL_BASE + 0x08)
#define HwSDMMC_CHCTRL_CH1_CAP0					(HwSDMMC_CHCTRL_BASE + 0x50)
#define HwSDMMC_CHCTRL_CH1_CAP1					(HwSDMMC_CHCTRL_BASE + 0x54)
#define HwSDMMC_CHCTRL_CH2_CAP0					(HwSDMMC_CHCTRL_BASE + 0xA0)
#define HwSDMMC_CHCTRL_CH2_CAP1					(HwSDMMC_CHCTRL_BASE + 0xA4)
#define HwSDMMC_CHCTRL_CH3_CAP0					NULL
#define HwSDMMC_CHCTRL_CH3_CAP1					NULL
#endif

#define HwSD_CD_WP0     *(volatile unsigned int *)(HwSDMMC_CHCTRL_BASE + 0x4C)
#define HwSD_CD_WP1     *(volatile unsigned int *)(HwSDMMC_CHCTRL_BASE + 0x9C)
#define HwSD_CD_WP2     *(volatile unsigned int *)(HwSDMMC_CHCTRL_BASE + 0xEC)
#define HwSD_CAP0_0     *(volatile unsigned int *)(HwSDMMC_CHCTRL_BASE + 0x04)
#define HwSD_CAP1_0     *(volatile unsigned int *)(HwSDMMC_CHCTRL_BASE + 0x54)
#define HwSD_CAP2_0     *(volatile unsigned int *)(HwSDMMC_CHCTRL_BASE + 0xA4)

#define IOBUS_SDMMC1 NULL
// You should define the type, buswidth, speed of the sdmmc controller as follows.                                     
// The "#num" means the sdmmc controller number.                                                           

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define SLOT_TYPE_#num			{SLOT_ATTR_BOOT | SLOT_ATTR_UPDATE}                                    //
// #define SLOT_BUSWIDTH_#num		{1 | 4 | 8}                                                            //
// #define SLOT_MAX_SPEED_#num		{SLOT_MAX_SPEED_NORMAL | SLOT_MAX_SPEED_HIGH | SLOT_MAX_SPEED_DDR}     //
// #define CARD_DETECT_FUNC_#num	{CardDetectPort}                                                       //
// #define WRITE_PROTECT_FUNC_#num	{WriteProtectPort}                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_DAUDIO_KK
#define SLOT_TYPE_3             SLOT_ATTR_BOOT
#define SLOT_BUSWIDTH_3         8
#define SLOT_MAX_SPEED_3        SLOT_MAX_SPEED_DDR
#define CARD_DETECT_FUNC_3      CardDetectPort
#define WRITE_PROTECT_FUNC_3    WriteProtectPort
#else
#define SLOT_TYPE_2             SLOT_ATTR_BOOT
#define SLOT_BUSWIDTH_2         8
#define SLOT_MAX_SPEED_2        SLOT_MAX_SPEED_DDR
#define CARD_DETECT_FUNC_2      CardDetectPort
#define WRITE_PROTECT_FUNC_2    WriteProtectPort
#endif


// You should define card detction and write protection gpio as follows.
// If you don't have such gpios or don't care, you can define as NULL.

// Card Detection
#ifndef CONFIG_DAUDIO_KK
#define CD_GPIO TCC_GPB(14)

// Write Protection
#define WP_GPIO TCC_GPB(7)
#else
#define CD_GPIO NULL
#define WP_GPIO NULL
#endif

// You should define data, command and clock gpios as follows.
#ifndef CONFIG_DAUDIO_KK
#define SDMMC_DATA0		TCC_GPA(9)
#define SDMMC_DATA1		TCC_GPA(10)
#define SDMMC_DATA2		TCC_GPA(11)
#define SDMMC_DATA3		TCC_GPA(12)
#define SDMMC_DATA4		TCC_GPA(13)
#define SDMMC_DATA5		TCC_GPA(14)
#define SDMMC_DATA6		TCC_GPA(15)
#define SDMMC_DATA7		TCC_GPA(16)
#define SDMMC_CMD		TCC_GPA(8)
#define SDMMC_CLK		TCC_GPA(7)
#define SDMMC_GPIO_FN	GPIO_FN3 
#define SDMMC_RST		NULL
#else
#define SDMMC_DATA0		TCC_GPSD0(2)
#define SDMMC_DATA1		TCC_GPSD0(3)
#define SDMMC_DATA2		TCC_GPSD0(4)
#define SDMMC_DATA3		TCC_GPSD0(5)
#define SDMMC_DATA4		TCC_GPSD0(6)
#define SDMMC_DATA5		TCC_GPSD0(7)
#define SDMMC_DATA6		TCC_GPSD0(8)
#define SDMMC_DATA7		TCC_GPSD0(9)
#define SDMMC_CMD		TCC_GPSD0(1)
#define SDMMC_CLK		TCC_GPSD0(0)
#define SDMMC_GPIO_FN	GPIO_FN1
#define SDMMC_RST		NULL
#endif

#define SDMMC2_DATA0	NULL
#define SDMMC2_DATA1	NULL
#define SDMMC2_DATA2	NULL
#define SDMMC2_DATA3	NULL
#define SDMMC2_DATA4	NULL
#define SDMMC2_DATA5	NULL
#define SDMMC2_DATA6	NULL
#define SDMMC2_DATA7	NULL
#define SDMMC2_CMD		NULL
#define SDMMC2_CLK		NULL
#define SDMMC2_GPIO_FN	NULL

// If you want to turn off the power of the SD Card, you should define the power gpio of the SD Card.
#ifndef CONFIG_DAUDIO_KK
#define SDCARD_PW TCC_GPEXT2(7)
#else
#define SDCARD_PW NULL
#endif

#define SD_BOOT 0
// If you want to adjust CMD/DATA Output Delay Register, you should change the following defines
#define ODELAY_ENABLE 1
#define ODELAY_CH0 0x00000000
#define ODELAY_CH1 0x00000000
#define ODELAY_CH2 0x00000000
#define ODELAY_CH3 0x07070707
