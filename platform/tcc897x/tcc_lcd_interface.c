
/****************************************************************************
 *   FileName    : TCC_LCD_interfface.c
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
/*****************************************************************************
*
* Header Files Include
*
******************************************************************************/
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <reg.h>
#include <platform/iomap.h>
#include <dev/fbcon.h>
#include <plat/cpu.h>

#include <platform/reg_physical.h>
#include <lcd.h>
#include <platform/irqs.h>
#include <i2c.h>
#include <dev/gpio.h>
#include <platform/gpio.h>
#include <tcc_lcd.h>
#include "vioc/vioc_wmix.h"
#include "vioc/vioc_rdma.h"
#include "vioc/vioc_wdma.h"
#include "vioc/vioc_scaler.h"
#include "vioc/vioc_config.h"

#include <TCC_JPU_C6.h>

volatile int lcd_nop_count = 0;
#define _ASM_NOP_ { lcd_nop_count++; }

void lcd_delay_us(unsigned int us)
{
	int i;
	while(us--)
	{
		for(i=0 ; i<20 ; i++)
			_ASM_NOP_ 
	}
}

/*
Description : RGB LCD display port setting
DD_num : Display device block number
DP_num : Display port(GPIO) number {ex  (0: L0_Lxx) or  (1 :L1_Lxx)}
bit_per_pixle : bit per pixel
*/
#define	CFG_MISC		((HwVIOC_CONFIG)+0x0084)	//0x7200A084
void LCDC_IO_Set (char DD_num,  char DP_num, unsigned bit_per_pixel)
{
	int i;

	BITCSET(*(unsigned int*)CFG_MISC, 0x3 << (24 + (DP_num * 2)), DD_num << (24 + (DP_num * 2)));

	if(DP_num)	{
		gpio_config(TCC_GPE(26), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW|GPIO_CD1);	// LPXCLK
		gpio_config(TCC_GPE(0), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);		//LHSYNC
		gpio_config(TCC_GPE(1), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);		//LVSYNC
		gpio_config(TCC_GPE(27), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);	//LACBIAS
	}
	else {
		gpio_config(TCC_GPB(0), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW|GPIO_CD1);		// LPXCLK
		gpio_config(TCC_GPB(1), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);		//LHSYNC
		gpio_config(TCC_GPB(2), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);		//LVSYNC
		gpio_config(TCC_GPB(19), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);	//LACBIAS
	}

	switch (bit_per_pixel) {
		case 24:
			for(i = 18 ; i < 24; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(4 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
			}

		case 18:
			for(i = 16 ; i < 18; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(4 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
			}
			
		case 16:
			for(i = 8 ; i < 16; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(3 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
			}
			
		case 8:
			for(i = 0 ; i < 8; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(3 + i), GPIO_FN1|GPIO_OUTPUT|GPIO_LOW);
				}
			}
			break;
			
		default:
			// do nothing
			break;
	}

}

/*
Description : RGB LCD display port disasble (set to normal GPIO)
DP_num : Display port(GPIO) number {ex  (0: L0_Lxx) or  (1 :L1_Lxx)}
bit_per_pixle : bit per pixel
*/
void LCDC_IO_Disable (char DP_num, unsigned bit_per_pixel)
{
	int i;

	if(DP_num)	{
		gpio_config(TCC_GPE(0), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);		//LHSYNC
		gpio_config(TCC_GPE(1), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);		//LVSYNC
		gpio_config(TCC_GPE(26), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);	// LPXCLK
		gpio_config(TCC_GPE(27), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);	//LACBIAS
	}
	else {
		gpio_config(TCC_GPB(0), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);		// LPXCLK
		gpio_config(TCC_GPB(1), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);		//LHSYNC
		gpio_config(TCC_GPB(2), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);		//LVSYNC
		gpio_config(TCC_GPB(19), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);	//LACBIAS
	}


	switch (bit_per_pixel) {
		case 24:
			for(i = 18 ; i < 24; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(4 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
			}

		case 18:
			for(i = 16 ; i < 18; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(4 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
			}
			
		case 16:
			for(i = 8 ; i < 16; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(3 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
			}
			
		case 8:
			for(i = 0 ; i < 8; i++)	{
				if(DP_num)	{
					gpio_config(TCC_GPE(2 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
				else {
					gpio_config(TCC_GPB(3 + i), GPIO_FN0|GPIO_OUTPUT|GPIO_LOW);
				}
			}
			break;
			
		default:
			// do nothing
			break;
	}

}

void lcdc_initialize(char lcdctrl_num, struct lcd_panel *lcd_spec)
{

	VIOC_DISP * pDISPBase;
	VIOC_WMIX* pWIXBase;
	
	if(lcdctrl_num) // Set the address for LCDC0 or LCDC1
	{
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP1;
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX1;
	}
	else
	{
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP0;
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX0;
	}

	VIOC_WMIX_SetSize(pWIXBase, lcd_spec->xres, lcd_spec->yres);	
	VIOC_WMIX_SetOverlayPriority(pWIXBase, 24);	
#ifdef LCD_DISPLAY_Y2R_ENABLE
	VIOC_WMIX_SetBGColor(pWIXBase, 0, 0x80, 0x80, 0);	
#else
	VIOC_WMIX_SetBGColor(pWIXBase, 0, 0, 0, 0);	
#endif
	if(lcd_spec->bus_width == 24)
		//pDISPBase->uCTRL.bREG.PXDW = 0xC;
		BITCSET(pDISPBase->uCTRL.nREG, 0xF << 16 , 0xC << 16);
	else if(lcd_spec->bus_width == 18)
		//pDISPBase->uCTRL.bREG.PXDW = 0x5;
		BITCSET(pDISPBase->uCTRL.nREG, 0xF << 16 , 0x5 << 16) ;
	else
		//pDISPBase->uCTRL.bREG.PXDW = 0x3;
		BITCSET(pDISPBase->uCTRL.nREG, 0xF << 16 , 0x3 << 16);


	if(lcd_spec->sync_invert & ID_INVERT)
		//pDISPBase->uCTRL.bREG.ID = 1;
		BITCSET(pDISPBase->uCTRL.nREG, 1 << 15 , 1 << 15);


	if(lcd_spec->sync_invert & IV_INVERT)
		//pDISPBase->uCTRL.bREG.IV = 1;
		BITCSET(pDISPBase->uCTRL.nREG, 1 << 14 , 1 << 14);

	if(lcd_spec->sync_invert & IH_INVERT)
		//pDISPBase->uCTRL.bREG.IH = 1;
		BITCSET(pDISPBase->uCTRL.nREG, 1 << 13 , 1 << 13);

	if(lcd_spec->sync_invert & IP_INVERT)
		//pDISPBase->uCTRL.bREG.IP = 1;
		BITCSET(pDISPBase->uCTRL.nREG, 1 << 12 , 1 << 12);
	

#ifdef LCD_DISPLAY_Y2R_ENABLE
	BITCSET(pDISPBase->uCTRL.nREG, 1 << 4 , 1 << 4);
#else
	BITCSET(pDISPBase->uCTRL.nREG, 1 << 4 , 0 << 4);
#endif//

	//pDISPBase->uCTRL.bREG.CKG = 0; // clock gating enable 
	BITCSET(pDISPBase->uCTRL.nREG, 1 << 23 , 0 << 23);

	BITCSET(pDISPBase->uCTRL.nREG, 1 << 22 , 0 << 22);

	
	//pDISPBase->uCTRL.bREG.NI = 1;
	BITCSET(pDISPBase->uCTRL.nREG, 1 << 8 , 1 << 8);

// Set LCD clock
	BITCSET(pDISPBase->uCLKDIV.nREG,0x00FF0000, 1  << 16 );
	BITCSET(pDISPBase->uCLKDIV.nREG,0x000000FF, lcd_spec->clk_div/2);


// Background color
	BITCSET(pDISPBase->uBG.nREG,0xFFFFFFFF,0x00000000);

//	Horizontal timing
	BITCSET(pDISPBase->uLHTIME1.nREG, 0x00003FFF, (lcd_spec->xres - 1) );
	BITCSET(pDISPBase->uLHTIME1.nREG, 0x01FF0000, lcd_spec->lpw<< 16 );	
	BITCSET(pDISPBase->uLHTIME2.nREG, 0x01FF01FF, (lcd_spec->lswc << 16) | lcd_spec->lewc );

//	Vertical timing
	BITCSET(pDISPBase->uLVTIME1.nREG, 0x3F << 16 , lcd_spec->fpw1 << 16);	
	BITCSET(pDISPBase->uLVTIME1.nREG, 0x00003FFF, lcd_spec->yres -1 );	
	BITCSET(pDISPBase->uLVTIME2.nREG, 0x01FF01FF, (lcd_spec->fswc1 << 16) | lcd_spec->fewc1 );	
	BITCSET(pDISPBase->uLVTIME3.nREG, 0x3F << 16 , lcd_spec->fpw2 << 16);	
	BITCSET(pDISPBase->uLVTIME3.nREG, 0x00003FFF, lcd_spec->yres-1 );
	BITCSET(pDISPBase->uLVTIME4.nREG, 0x01FF01FF, (lcd_spec->fswc2 << 16) | lcd_spec->fewc2 );

	
	BITCSET(pDISPBase->uLSIZE.nREG, 0x1FFF1FFF, (lcd_spec->yres << 16) | lcd_spec->xres );


	BITCSET(pDISPBase->uCTRL.nREG,1,1);

}


void tcc_lcdc_dithering_setting(char lcdc)
{

	VIOC_DISP * pDISPBase;

	if(lcdc) // Set the address for LCDC0 or LCDC1
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP1;
	else
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP0;

	/* dithering option */
	BITCSET(pDISPBase->uCTRL.nREG, 0xF << 16 , 0x5 << 16) ;
	BITCSET(pDISPBase->uDITHCTRL.nREG, 0xFFFFFFFF, 0xC0000000) ;
	BITCSET(pDISPBase->uDMAT.nREG[0],  0xFFFFFFFF, 0x6e4ca280) ;
	BITCSET(pDISPBase->uDMAT.nREG[1],  0xFFFFFFFF, 0x5d7f91b3) ;

}


void tcclcd_image_ch_set(char lcdctrl_num, struct tcc_lcdc_image_update *ImageInfo)
{
	VIOC_RDMA *pRDMA;
	VIOC_WMIX *pWIXBase;

	if (lcdctrl_num) {
		pRDMA = (VIOC_RDMA *)((unsigned int)HwVIOC_RDMA04 + ImageInfo->Lcdc_layer * 0x100);
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX1;
	} else {
		pRDMA = (VIOC_RDMA *)((unsigned int)HwVIOC_RDMA00 + ImageInfo->Lcdc_layer * 0x100);
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX0;
	}

	dprintf(INFO, "%s lcdc:%d lcdc_ch:%d pRDMA:0x%08x\n", \
			__func__, \
			lcdctrl_num, \
			ImageInfo->Lcdc_layer, \
			(unsigned int)pRDMA);
	dprintf(INFO, "%s pos_x:%d pos_y:%d img_wd:%d img_ht:%d lcd_wd:%d lcd_ht:%d fmt:%d \n", \
			__func__, \
			ImageInfo->offset_x, ImageInfo->offset_y, \
			ImageInfo->Image_width, ImageInfo->Image_height, \
			ImageInfo->Frame_width, ImageInfo->Frame_height,
			ImageInfo->fmt);

	#if defined(DISPLAY_SCALER_USE)
	{
		VIOC_SC *pSC;
		char scaler_num;
		char scaler_rdma;

		if (lcdctrl_num) {
			pSC = (VIOC_SC *)HwVIOC_SC0;
			scaler_num = 0;
			scaler_rdma = VIOC_SC_RDMA_04 + ImageInfo->Lcdc_layer;
		} else {
			pSC = (VIOC_SC *)HwVIOC_SC2;
			scaler_num = 2;
			scaler_rdma = VIOC_SC_RDMA_00 + ImageInfo->Lcdc_layer;
		}

		/* set SCALER before plugging-in */
		VIOC_SC_SetBypass(pSC, OFF);
		VIOC_SC_SetSrcSize(pSC, ImageInfo->Image_width, ImageInfo->Image_height);
		VIOC_SC_SetDstSize(pSC, ImageInfo->Frame_width, ImageInfo->Frame_height);
		VIOC_SC_SetOutSize(pSC, ImageInfo->Frame_width, ImageInfo->Frame_height);
		VIOC_SC_SetUpdate(pSC);

		VIOC_CONFIG_PlugIn(scaler_num, scaler_rdma);
	}
	#endif

	VIOC_RDMA_SetImageY2RMode(pRDMA, 0); /* Y2RMode Default 0 (Studio Color) */

	if ( ImageInfo->fmt >= TCC_LCDC_IMG_FMT_444SEP && ImageInfo->fmt <= TCC_LCDC_IMG_FMT_YUV422ITL1)	{
#ifdef LCD_DISPLAY_Y2R_ENABLE
		VIOC_RDMA_SetImageR2YEnable(pRDMA, FALSE);
		VIOC_RDMA_SetImageY2REnable(pRDMA, FALSE);
#else
		VIOC_RDMA_SetImageR2YEnable(pRDMA, FALSE);
		VIOC_RDMA_SetImageY2REnable(pRDMA, TRUE);
#endif//
	}
	else	{
#ifdef LCD_DISPLAY_Y2R_ENABLE
		VIOC_RDMA_SetImageR2YEnable(pRDMA, TRUE);
		VIOC_RDMA_SetImageY2REnable(pRDMA, FALSE);
#else
		VIOC_RDMA_SetImageR2YEnable(pRDMA, FALSE);
		VIOC_RDMA_SetImageY2REnable(pRDMA, FALSE);
#endif//
	}
	
	VIOC_RDMA_SetImageOffset(pRDMA, ImageInfo->fmt, ImageInfo->Image_width);
	VIOC_RDMA_SetImageFormat(pRDMA, ImageInfo->fmt);
	VIOC_RDMA_SetImageSize(pRDMA, ImageInfo->Image_width, ImageInfo->Image_height);
	
	// image address
	VIOC_RDMA_SetImageBase(pRDMA, ImageInfo->addr0, ImageInfo->addr1, ImageInfo->addr2);
	
	// image position
	VIOC_WMIX_SetPosition(pWIXBase, ImageInfo->Lcdc_layer, ImageInfo->offset_x, ImageInfo->offset_y);

	// image enable
	if( ImageInfo->enable)
		VIOC_RDMA_SetImageEnable(pRDMA);
	else
		VIOC_RDMA_SetImageDisable(pRDMA);

	VIOC_WMIX_SetUpdate(pWIXBase);

}

unsigned int DEV_LCDC_Wait_signal(char lcdc)
{
	// TO DO
	#define MAX_LCDC_WAIT_TIEM 		0x70000000

	unsigned loop = 1;
	VIOC_DISP * pDISPBase;
	VIOC_RDMA * pRDMABase;
	
	if(lcdc == 0 ){
		pDISPBase = (VIOC_DISP*)((unsigned int)(HwVIOC_DISP0));
		pRDMABase = (VIOC_RDMA*)((unsigned int)(HwVIOC_RDMA00));
	}
	else{
		pDISPBase = (VIOC_DISP*)((unsigned int)(HwVIOC_DISP1));
		pRDMABase = (VIOC_RDMA*)((unsigned int)(HwVIOC_RDMA04));
	}
	
	if ( ISZERO(pDISPBase->uCTRL.nREG, HwDISP_LEN) )
		return FALSE;

	// UI rdma channel check enable.
	if ( ISZERO(pRDMABase->uCTRL.nREG, HwDMA_IEN))
		return FALSE;

	BITSET(pRDMABase->uSTATUS.nREG, Hw4);

	while(TRUE && (loop < MAX_LCDC_WAIT_TIEM))
	{
		if(ISSET(pRDMABase->uSTATUS.nREG, Hw4))
			break;

		//printf("DEV_LCDC_Wait_signal cnt = %d \n",loop);
		loop++;
	}
	
 	return loop;
}


void tcclcd_gpio_config(unsigned n, unsigned flags)
{
	if(n == GPIO_NC)
	{
		// no connect
	}
	else if(n >= GPIO_EXT1)
	{
		// extend gpio 
	}
	else
	{
		gpio_config(n, flags);
	}
}
void tcclcd_gpio_set_value(unsigned n, unsigned on)
{

	
#if 1
	if(n == GPIO_NC)
	{
		// no connect
	}
	else if(n >= GPIO_EXT1)
	{
		// extend gpio 
	}
	else
	{
		gpio_set(n, on);
	}
#else
	gpio_set(n, on);
#endif

}

int splash_image_reset_vioc_path(void) {
	int idx_rdma, idx_wmix, idx_wdma;

	idx_rdma = 13;
	idx_wmix = 3;
	idx_wdma = 3;

	static VIOC_IREQ_CONFIG	* pIREQConfig;
	pIREQConfig = (VIOC_IREQ_CONFIG *)(HwVIOC_IREQ);	//vioc ireq

	BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1 << (idx_wdma)));			// reset WDMA
	BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1 << (idx_wmix+9)));		// reset WMIXer
	BITSET(pIREQConfig->uSOFTRESET.nREG[0], (1 << (idx_rdma)));			// reset PGL

	mdelay(1);

	BITCLR(pIREQConfig->uSOFTRESET.nREG[0], (1 << (idx_rdma)));			// clear PGL
	BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1 << (idx_wmix+9)));		// clear WMIXer
	BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1 << (idx_wdma)));			// clear WDMA

	return 0;
}

int splash_image_set_rdma(unsigned int idx, unsigned int width, unsigned int height, unsigned int fmt, unsigned int baseAddr0, unsigned int baseAddr1, unsigned int baseAddr2) {
//	unsigned int fmt;
	VIOC_RDMA*		pRDMA;

	//default path : rdma13 - wmix03 - wdma03
	pRDMA = (VIOC_RDMA *)(HwVIOC_RDMA00 + (HwVIOC_RDMA_GAP * idx));

	switch(fmt) {
	case YUV_FORMAT_420:
		fmt = TCC_LCDC_IMG_FMT_YUV420SP;
//		dprintf(INFO, "%s YUV_FORMAT_420 \n", __func__);
		break;
	case YUV_FORMAT_422:
//		dprintf(INFO, "%s YUV_FORMAT_422 \n", __func__);
		fmt = TCC_LCDC_IMG_FMT_YUV422SP;
		break;
	case YUV_FORMAT_224:
//		dprintf(INFO, "%s YUV_FORMAT_224 \n", __func__);
		break;
	case YUV_FORMAT_444:
//		dprintf(INFO, "%s YUV_FORMAT_444 \n", __func__);
		fmt = TCC_LCDC_IMG_FMT_444SEP;
		break;
	case YUV_FORMAT_400:
//		dprintf(INFO, "%s YUV_FORMAT_400 \n", __func__);
		break;
	default:
//		dprintf(INFO, "%s default \n", __func__);
		break;
	}

	VIOC_RDMA_SetImageFormat(pRDMA, fmt);
	VIOC_RDMA_SetImageSize(pRDMA, width, height);
	VIOC_RDMA_SetImageOffset(pRDMA, fmt, width);
	VIOC_RDMA_SetImageBase(pRDMA, baseAddr0, baseAddr1, baseAddr2);

	VIOC_RDMA_SetImageEnable(pRDMA);

	return 0;
}

int splash_image_set_wmix(unsigned int idx, unsigned int width, unsigned int height) {
	VIOC_WMIX*		pWMIX;

	pWMIX = (VIOC_WMIX *)(HwVIOC_WMIX0 + (HwVIOC_RDMA_GAP * idx));

	VIOC_WMIX_SetSize(pWMIX, width, height);
	VIOC_WMIX_SetUpdate(pWMIX);

	return 0;
}

void splash_image_set_wdma(unsigned int idx, unsigned int width, unsigned int height, unsigned int fmt, unsigned int baseAddr) {
	VIOC_WDMA* pWDMA = (VIOC_WDMA *)(HwVIOC_WDMA00 + (HwVIOC_RDMA_GAP * idx));

	VIOC_WDMA_SetImageY2REnable(pWDMA, ON);
	VIOC_WDMA_SetImageFormat(pWDMA, fmt);
	VIOC_WDMA_SetImageSize(pWDMA, width, height);
	VIOC_WDMA_SetImageOffset(pWDMA, fmt, width);

	//set base address for write DMA.
	VIOC_WDMA_SetImageBase(pWDMA, \
						baseAddr, (unsigned int)NULL, (unsigned int)NULL);

	BITCSET(pWDMA->uIRQSTS.nREG, 1<<5, 1<<5);
	VIOC_WDMA_SetIreqMask(pWDMA, VIOC_WDMA_IREQ_EOFR_MASK, 0x0);

	// operating start in 1 frame
	VIOC_WDMA_SetImageEnable(pWDMA, OFF);

	while(1) {
		if(pWDMA->uIRQSTS.nREG & VIOC_WDMA_IREQ_EOFR_MASK) {
			splash_image_reset_vioc_path();
//			dprintf(INFO, "==== format converting complite!!(0x%x) \n", baseAddr);
			break;
		}
	}
}

int splash_image_convert_format(jpu_dec_outputs_t * p_jpu_out, struct fbcon_config *p_fb_cfg) {
	int idx_rdma, idx_wmix, idx_wdma;
//	unsigned int output_fmt = TCC_LCDC_IMG_FMT_UYVY;
	unsigned int output_fmt = TCC_LCDC_IMG_FMT_RGB888;

//	dprintf(INFO, "==== %s in\n", __func__);

	idx_rdma = 13;
	idx_wmix = 3;
	idx_wdma = 3;

	p_fb_cfg->format = output_fmt;

//	dprintf(INFO, "==== %s path reset start \n", __func__);
	splash_image_reset_vioc_path();
//	dprintf(INFO, "==== %s path reset end \n", __func__);

//	dprintf(INFO, "==== %s splash_image_set_rdma start \n", __func__);
	splash_image_set_rdma(idx_rdma, \
						p_jpu_out->m_iWidth, p_jpu_out->m_iHeight,\
						p_jpu_out->m_iYUVFormat, \
						(unsigned int)p_jpu_out->m_pCurrOut[0][0], \
						(unsigned int)p_jpu_out->m_pCurrOut[0][1], \
						(unsigned int)p_jpu_out->m_pCurrOut[0][2]);
//	dprintf(INFO, "==== %s splash_image_set_rdma end \n", __func__);

//	dprintf(INFO, "==== %s splash_image_set_wmix start \n", __func__);
	splash_image_set_wmix(idx_wmix, p_jpu_out->m_iWidth, p_jpu_out->m_iHeight);
//	dprintf(INFO, "==== %s splash_image_set_wmix end \n", __func__);

//	dprintf(INFO, "==== %s splash_image_set_wdma start \n", __func__);
	splash_image_set_wdma(idx_wdma, p_jpu_out->m_iWidth, p_jpu_out->m_iHeight, output_fmt, (unsigned int)p_fb_cfg->base);
//	dprintf(INFO, "==== %s splash_image_set_wdma end \n", __func__);

//	dprintf(INFO, "==== %s out\n", __func__);

	return 0;
}
void lcdc_display_off(char lcdc_ctrlNum)
{
	VIOC_DISP* pDISPBase;
	VIOC_WMIX* pWIXBase;
	VIOC_RDMA* pRDMABase;

	if(lcdc_ctrlNum) // Set the address for LCDC0 or LCDC1
	{
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP1;
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX1;
		pRDMABase = (VIOC_RDMA*)HwVIOC_RDMA04;
	}
	else
	{
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP0;
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX0;
		pRDMABase = (VIOC_RDMA*)HwVIOC_RDMA00;
	}
	printf("%s lcdc:%d pRDMA:0x%08x\n", __func__, lcdc_ctrlNum, pRDMABase);

	VIOC_RDMA_SetImageDisable(pRDMABase);

}

void lcdc_display_on(char lcdc_ctrlNum)
{
	VIOC_DISP * pDISPBase;
	VIOC_WMIX* pWIXBase;
	VIOC_RDMA* pRDMABase;

	if(lcdc_ctrlNum) // Set the address for LCDC0 or LCDC1
	{
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP1;
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX1;
		pRDMABase = (VIOC_RDMA*)HwVIOC_RDMA04;
	}
	else
	{
		pDISPBase = (VIOC_DISP *)HwVIOC_DISP0;
		pWIXBase = (VIOC_WMIX*)HwVIOC_WMIX0;
		pRDMABase = (VIOC_RDMA*)HwVIOC_RDMA00;
	}
	printf("%s lcdc:%d pRDMA:0x%08x\n", __func__, lcdc_ctrlNum,  pRDMABase);

	VIOC_RDMA_SetImageEnable(pRDMABase);
}

