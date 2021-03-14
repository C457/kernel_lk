/****************************************************************************
 *   FileName    : lcd_FLD0800.c
 *   Description : support for FLD0800 LVDS Panel
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall constitute any express or implied warranty of any kind, including without limitation, any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright or other third party intellectual property right. No warranty is made, express or implied, regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement between Telechips and Company.
*
****************************************************************************/
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <tcc_lcd.h>


#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <reg.h>
#include <platform/iomap.h>
#include <dev/fbcon.h>

#if 0//def FLD0800
#include <lcd.h>
#include <dev/gpio.h>
#include <platform/gpio.h>

#include <tnftl/IO_TCCXXX.h>

//#include <platform/reg_physical.h>
#include <platform/structures_display.h>


#define     LVDS_VCO_45MHz        45000000
#define     LVDS_VCO_60MHz        60000000
extern void tca_ckc_setpmupwroff( unsigned int periname , unsigned int isenable);
#ifdef BACKLIGHT_PDM
extern int tcc_pwm_config(unsigned int npwm, int duty_ns, int period_ns);
#endif//

unsigned int lvds_stbyb;

static int fld0800_panel_init(struct lcd_panel *panel)
{
	printf("%s : %d\n", __func__, 0);

	return 0;
}

static int fld0800_set_power(struct lcd_panel *panel, int on)
{
	unsigned int P, M, S, VSEL, TC;

	PDDICONFIG		pDDICfg 	= (DDICONFIG *)HwDDI_CONFIG_BASE;
	struct lcd_platform_data *pdata = &(panel->dev);
	printf("%s : %d \n", __func__, on);

	if (on) {
		lcdc_initialize(pdata->lcdc_num, panel);
//		LCDC_IO_Set(pdata->lcdc_num, panel->bus_width);
		
		//LVDS 6bit setting for internal dithering option !!!
		//tcc_lcdc_dithering_setting(pdata->lcdc_num);

		// LVDS reset	
	   	pDDICfg->LVDS_CTRL.bREG.RST = 1;
		pDDICfg->LVDS_CTRL.bREG.RST = 0;		



		BITCSET(pDDICfg->LVDS_TXO_SEL0.nREG, 0xFFFFFFFF, 0x13121110);
		BITCSET(pDDICfg->LVDS_TXO_SEL1.nREG, 0xFFFFFFFF, 0x09081514);
		BITCSET(pDDICfg->LVDS_TXO_SEL2.nREG, 0xFFFFFFFF, 0x0D0C0B0A);
		BITCSET(pDDICfg->LVDS_TXO_SEL3.nREG, 0xFFFFFFFF, 0x03020100);
		BITCSET(pDDICfg->LVDS_TXO_SEL4.nREG, 0xFFFFFFFF, 0x1A190504);
		BITCSET(pDDICfg->LVDS_TXO_SEL5.nREG, 0xFFFFFFFF, 0x0E171618);
		BITCSET(pDDICfg->LVDS_TXO_SEL6.nREG, 0xFFFFFFFF, 0x1F07060F);
		BITCSET(pDDICfg->LVDS_TXO_SEL7.nREG, 0xFFFFFFFF, 0x1F1E1F1E);
		BITCSET(pDDICfg->LVDS_TXO_SEL8.nREG, 0xFFFFFFFF, 0x001E1F1E);

		//LVDS P,M,S,VSEL select

		if( panel->clk_freq >= LVDS_VCO_45MHz && panel->clk_freq < LVDS_VCO_60MHz)
		{
			#if defined (VIOC_TCC8930)
				   M = 7; P = 7; S = 0; VSEL = 0; TC = 4;
			#elif defined (VIOC_TCC8960) || defined(VIOC_TCC8970)
//				   M = 10; P = 10; S = 1; VSEL = 0; TC = 4;
				   M = 10; P = 10; S = 2; VSEL = 0; TC = 4;
			#endif//
		}
		else
		{
			#if defined (VIOC_TCC8930)
					M = 10; P = 10; S = 0; VSEL = 0; TC = 4;		
			#elif defined (VIOC_TCC8960) || defined(VIOC_TCC8970)
//					M = 14; P = 14; S = 1; VSEL = 1; TC = 4;		
					M = 10; P = 10; S = 1; VSEL = 0; TC = 4;		
			#endif//
		}
           
		printf("%s : M  = %d, P = %d, S = %d, VSEL = %d, TC = %d \n", __func__, M, P, S, VSEL, TC);
		BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x00FFFFF0, (VSEL<<4)|(S<<5)|(M<<8)|(P<<15)|(TC<<21)); //LCDC1

		#if defined (VIOC_TCC8960) || defined(VIOC_TCC8970)
		pDDICfg->LVDS_MISC1.bREG.LC = 0;
		pDDICfg->LVDS_MISC1.bREG.CC = 0;
		pDDICfg->LVDS_MISC1.bREG.CMS = 0;
		pDDICfg->LVDS_MISC1.bREG.VOC = 1;
		#endif//
		// LVDS Select LCDC 1
		if(pdata->lcdc_num ==1)
			BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x3 << 30 , 0x1 << 30);
		else
			BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x3 << 30 , 0x0 << 30);


		pDDICfg->LVDS_CTRL.bREG.RST = 1;        //  reset

	    	// LVDS enable
	  	pDDICfg->LVDS_CTRL.bREG.EN = 1;   // enable


	}
	else 	{
		tcclcd_gpio_set_value(pdata->power_on, 0);
	}
	lcd_delay_us(1600000);
	
	return 0;
}

static int fld0800_set_backlight_level(struct lcd_panel *panel, int level)
{
	struct lcd_platform_data *pdata = &(panel->dev);

	printf("%s : %d\n", __func__, level);

	return 0;
}

#if 0
static struct lcd_panel fld0800_panel = {
	.name		= "FLD0800",
	.manufacturer	= "innolux",
	.id		= PANEL_ID_FLD0800,
	.xres		= 800,//1024,
	.yres		= 480,//600,
	.width		= 154,//153,
	.height		= 85,//90,
	.bpp		= 24,
	.clk_freq	= 33260000,//51200000,
	.clk_div	= 2,
	.bus_width	= 24,
	
	.lpw		= 12,//19,
	.lpc		= 800,//1024,
	.lswc		= 48,//147,
	.lewc		= 100,//147,
	.vdb		= 0,
	.vdf		= 0,

	.fpw1		= 1,//2,
	.flc1		= 480,//600,
	.fswc1		= 3,//10,
	.fewc1		= 90,//25,
	
	.fpw2		= 1,//2,
	.flc2		= 480,//600,
	.fswc2		= 3,//10,
	.fewc2		= 90,//25,

	.sync_invert	= IV_INVERT | IH_INVERT,
	.init		= fld0800_panel_init,
	.set_power	= fld0800_set_power,
	.set_backlight_level = fld0800_set_backlight_level,
};
#else
static struct lcd_panel fld0800_panel = {
	.name		= "FLD0800",
	.manufacturer	= "innolux",
	.id		= PANEL_ID_FLD0800,
	.xres		= 1280,//1024,
	.yres		= 720,//600,
	.width		= 60,//153,
	.height		= 72,//90,
	.bpp		= 24,

	.clk_freq	= 63700000,//51200000,
	.clk_div	= 2,
	.bus_width	= 24,
	
	.lpw		= 2,//19,
	.lpc		= 1280,//1024,
	.lswc		= 12,//147,
	.lewc		= 44,//147,

	.vdb		= 0,
	.vdf		= 0,

	.fpw1		= 2,//2,
	.flc1		= 720,//600,
	.fswc1		= 1,//10,
	.fewc1		= 67,//25,
	
	.fpw2		= 2,//2,
	.flc2		= 720,//600,
	.fswc2		= 1,//10,
	.fewc2		= 67,//25,

	.sync_invert	= IV_INVERT | IH_INVERT,
	.init		= fld0800_panel_init,
	.set_power	= fld0800_set_power,
	.set_backlight_level = fld0800_set_backlight_level,
};
#endif
struct lcd_panel *tccfb_get_panel(void)
{
	return &fld0800_panel;
}
#endif//FLD0800
