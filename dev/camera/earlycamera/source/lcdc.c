
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.

  This software is licensed under the terms of the GNU General Public
  License version 2, as published by the Free Software Foundation, and
  may be copied, distributed, and modified under those terms.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
****************************************************************************/

#include "i2c.h"
#include "gpio.h"
#include "tcc_lcd.h"
#include "reg_physical.h"
#include "vioc_config.h"
#include "vioc_scaler.h"
#include "vioc_wdma.h"
#include "vioc_wmix.h"
#include "vioc_rdma.h"
#include "vioc_disp.h"
#include "vioc_vin.h"
#include "vioc_viqe.h"
#include "camera.h"
#include "lcdc.h"

extern struct viocmg_info * Viocmg_info;

static VIOC_IREQ_CONFIG	* pIREQConfig;
static VIOC_RDMA	* pRDMABase;
static VIOC_WMIX	* pWMIXBase;

int tcc_lcd_image_ch_set(struct tcc_lcdc_image_update * ImageInfo) {
	pIREQConfig = (VIOC_IREQ_CONFIG *)(HwVIOC_IREQ);	//vioc ireq
	pRDMABase = (VIOC_RDMA*)((unsigned int)(HwVIOC_RDMA00 + (HwVIOC_RDMA_GAP * Viocmg_info->early_cam_display_rdma)));
	
	pWMIXBase = (VIOC_WMIX*)((unsigned int)(HwVIOC_WMIX0 + (HwVIOC_RDMA_GAP * Viocmg_info->main_display_id)));

	BITSET(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_display_rdma)));			// RDMAn(display path) reset
	BITCLR(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_display_rdma)));			// RDMAn(Display path) clear
	
	VIOC_RDMA_SetImageY2RMode(pRDMABase, 0);

	if( ImageInfo->fmt >= TCC_LCDC_IMG_FMT_444SEP/*TCC_LCDC_IMG_FMT_YUV420SP*/ && ImageInfo->fmt <= TCC_LCDC_IMG_FMT_YUV422ITL1)
	        VIOC_RDMA_SetImageY2REnable(pRDMABase, TRUE);

	VIOC_RDMA_SetImageOffset(pRDMABase, ImageInfo->fmt, ImageInfo->Frame_width);
	VIOC_RDMA_SetImageFormat(pRDMABase, ImageInfo->fmt);
	VIOC_RDMA_SetImageSize(pRDMABase, ImageInfo->Frame_width, ImageInfo->Frame_height);
	
	// image address
	VIOC_RDMA_SetImageBase(pRDMABase, ImageInfo->addr0, ImageInfo->addr1, ImageInfo->addr2);
	
	// image position
	VIOC_WMIX_SetPosition(pWMIXBase, ImageInfo->Lcdc_layer, ImageInfo->offset_x, ImageInfo->offset_y);
	
	//mdelay(100);
	//tcc_rear_camera_display(R_Gear_Status_Check());
		
	return 0;
}

void tcc_rear_camera_display(unsigned char OnOff) {
	if (OnOff) { // display on.
		VIOC_WMIX_SetOverlayPriority(pWMIXBase, Viocmg_info->early_cam_ovp);
		VIOC_RDMA_SetImageEnable(pRDMABase);
		gpio_set(TCC_GPB(4), 1);
		Viocmg_info->early_cam_mode = 2;
			
	} else {
		VIOC_WMIX_SetOverlayPriority(pWMIXBase, Viocmg_info->main_display_ovp);			
		VIOC_RDMA_SetImageDisable(pRDMABase);
		gpio_set(TCC_GPB(4), 0);
		Viocmg_info->early_cam_mode = 0;
 	}
		
	VIOC_WMIX_SetUpdate(pWMIXBase);
}

void tcc_rear_camera_wmix_channel_prioty(void) {
	BITCSET(pWMIXBase->uROPC0.nREG, 0xFFFFFFFF, 0x0000C018);
	BITCSET(pWMIXBase->uCTRL.nREG, 0x0000001F, 24); // 24 :  layer3(image0) layer2(image1) layer1(image2) layer1(image3)
	BITCSET(pWMIXBase->uCTRL.nREG, (1<<16), (1<<16));
}
