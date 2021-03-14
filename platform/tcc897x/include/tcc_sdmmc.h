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

// You should define the type, buswidth, speed of the sdmmc controller as follows.                                     
// The "#num" means the sdmmc controller number.                                                           

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define SLOT_TYPE_#num			{SLOT_ATTR_BOOT | SLOT_ATTR_UPDATE}                                    //
// #define SLOT_BUSWIDTH_#num		{1 | 4 | 8}                                                            //
// #define SLOT_MAX_SPEED_#num		{SLOT_MAX_SPEED_NORMAL | SLOT_MAX_SPEED_HIGH | SLOT_MAX_SPEED_DDR}     //
// #define CARD_DETECT_FUNC_#num	{CardDetectPort}                                                       //
// #define WRITE_PROTECT_FUNC_#num	{WriteProtectPort}                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SLOT_TYPE_2             SLOT_ATTR_BOOT
#define SLOT_BUSWIDTH_2         8
#define SLOT_MAX_SPEED_2        SLOT_MAX_SPEED_DDR
#define CARD_DETECT_FUNC_2      CardDetectPort
#define WRITE_PROTECT_FUNC_2    WriteProtectPort


// You should define card detction and write protection gpio as follows.
// If you don't have such gpios or don't care, you can define as NULL.

// Card Detection
#if (HW_REV == 0x1000) // TCC8970_EVM
	#define CD_GPIO TCC_GPB(14)
#else
	#define CD_GPIO NULL
#endif

// Write Protection
#if (HW_REV == 0x1000) // TCC8970_EVM
	#define WP_GPIO TCC_GPB(7)
#else
	#define WP_GPIO NULL
#endif

// You should define data, command and clock gpios as follows.
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

// If you want to turn off the power of the SD Card, you should define the power gpio of the SD Card.
#if (HW_REV == 0x1000) // TCC8970_EVM
	#define SDCARD_PW TCC_GPEXT2(7)
#else
	#define SDCARD_PW NULL
#endif
