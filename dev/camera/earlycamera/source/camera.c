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
/****************************************************************************
   Written by S.W.Hwang (Platform Group BSP Team, Telechips Inc.)
****************************************************************************/

#include <debug.h>
#include <string.h>

#include "i2c.h"
#include <gpio.h>
#include "tcc_lcd.h"
#include "reg_physical.h"
#include "vioc_config.h"
#include "vioc_vin.h"
#include "vioc_viqe.h"
#include "vioc_deintls.h"
#include "vioc_scaler.h"
#include "vioc_wmix.h"
#include "vioc_wdma.h"
#include "vioc_rdma.h"
#include "vioc_fifo.h"
#include "vioc_disp.h"
#include "lcdc.h"
#include "camera.h"
#include "delay.h"
#include "sensor_if.h"

struct tcc_cif_parameters* CM4SyncParameters;
struct viocmg_info * Viocmg_info;
struct tcc_lcdc_image_update Image_info;
int pgl_enable;

static VIOC_WMIX* 		pWMIXBase;
static VIOC_WDMA*		pWDMABase;
static VIOC_VIN* 		pVINBase;
static VIOC_SC* 		pSCBase;
static VIOC_RDMA*		pRDMABase;
static DDICONFIG*		pDDIConfig;
static VIOC_IREQ_CONFIG* 	pIREQConfig;
static VIQE*			pVIQEBase;
#if defined(PLATFORM_TCC897X)
static unsigned int *		pDeintls;
#endif

#define PREVIEW_BUFFER_NUMBER	4
unsigned int addrPreview[PREVIEW_BUFFER_NUMBER];

void tcc_cif_parameters_dump(void) {
	printk("\r\ntcc_cif_parameters_dump------------------------------------------\r\n");
	printk("Cam_p_clock_pol = %d\r\n", CM4SyncParameters->Cam_p_clock_pol);
	printk("Cam_v_sync_pol = %d\r\n", CM4SyncParameters->Cam_v_sync_pol);
	printk("Cam_h_sync_pol = %d\r\n", CM4SyncParameters->Cam_h_sync_pol);
	printk("Cam_de_pol = %d\r\n", CM4SyncParameters->Cam_de_pol);
	printk("Cam_field_bfield_low = %d\r\n", CM4SyncParameters->Cam_field_bfield_low);
	printk("Cam_gen_field_en = %d\r\n", CM4SyncParameters->Cam_gen_field_en);
	printk("Cam_conv_en = %d\r\n", CM4SyncParameters->Cam_conv_en);
	printk("Cam_hsde_connect_en = %d\r\n", CM4SyncParameters->Cam_hsde_connect_en);
	printk("Cam_vs_mask = %d\r\n", CM4SyncParameters->Cam_vs_mask);
	printk("Cam_input_fmt = %d\r\n", CM4SyncParameters->Cam_input_fmt);
	printk("Cam_data_order = %d\r\n", CM4SyncParameters->Cam_data_order);
	printk("Cam_intl_en = %d\r\n", CM4SyncParameters->Cam_intl_en);
	printk("Cam_intpl_en = %d\r\n", CM4SyncParameters->Cam_intpl_en);
	printk("Cam_format = %d\r\n", CM4SyncParameters->Cam_format);
	printk("Cam_preview_w = %d\r\n", CM4SyncParameters->Cam_preview_w);
	printk("Cam_preview_h = %d\r\n", CM4SyncParameters->Cam_preview_h);
	printk("Lcdc_address0 = 0x%x\r\n", CM4SyncParameters->Lcdc_address0);
	printk("Lcdc_address1 = 0x%x\r\n", CM4SyncParameters->Lcdc_address1);
	printk("Lcdc_address2 = 0x%x\r\n", CM4SyncParameters->Lcdc_address2);
	printk("Guide_line_area = 0x%x\r\n", CM4SyncParameters->PGL_addr);
	printk("Viqe_area = 0x%x\r\n", CM4SyncParameters->Viqe_area);
	printk("Camera_type = 0x%d\r\n", CM4SyncParameters->Camera_type);

	printk("main_display_id = %d\r\n", Viocmg_info->main_display_id);
	printk("main_display_port = %d\r\n", Viocmg_info->main_display_port);
	printk("main_display_ovp = %d\r\n", Viocmg_info->main_display_ovp);

	printk("feature_early_view_enable = %d\r\n", Viocmg_info->feature_early_view_enable);
	printk("feature_early_view_use_viqe = %d\r\n", Viocmg_info->feature_early_view_use_viqe);
	printk("feature_early_view_viqe_mode = %d\r\n", Viocmg_info->feature_early_view_viqe_mode);
	printk("feature_early_view_use_parking_line = %d\r\n", Viocmg_info->feature_early_view_use_parking_line);

	printk("early_cam_cifport = %d\r\n", Viocmg_info->early_cam_cifport);
	printk("early_cam_vin_vin = %d\r\n", Viocmg_info->early_cam_vin_vin);
	printk("early_cam_vin_rdma = %d\r\n", Viocmg_info->early_cam_vin_rdma);
	printk("early_cam_vin_wmix = %d\r\n", Viocmg_info->early_cam_vin_wmix);
	printk("early_cam_vin_wdma = %d\r\n", Viocmg_info->early_cam_vin_wdma);
	printk("early_cam_vin_scaler = %d\r\n", Viocmg_info->early_cam_vin_scaler);
	printk("early_cam_display_rdma = %d\r\n", Viocmg_info->early_cam_display_rdma);

	printk("early_cam_mode = %d\r\n", Viocmg_info->early_cam_mode);
	printk("early_cam_ovp = %d\r\n", Viocmg_info->early_cam_ovp);

	printk("early_cam_preview_x = %d\r\n", Viocmg_info->early_cam_preview_x);
	printk("early_cam_preview_y = %d\r\n", Viocmg_info->early_cam_preview_y);
	printk("early_cam_preview_width = %d\r\n", Viocmg_info->early_cam_preview_width);
	printk("early_cam_preview_height = %d\r\n", Viocmg_info->early_cam_preview_height);
	printk("early_cam_preview_format = %d\r\n", Viocmg_info->early_cam_preview_format);

	printk("early_cam_preview_additional_width = %d\r\n", Viocmg_info->early_cam_preview_additional_width);
	printk("early_cam_preview_additional_height = %d\r\n", Viocmg_info->early_cam_preview_additional_height);

	printk("early_cam_parking_line_x = %d\r\n", Viocmg_info->early_cam_parking_line_x);
	printk("early_cam_parking_line_y = %d\r\n", Viocmg_info->early_cam_parking_line_y);
	printk("early_cam_parking_line_width = %d\r\n", Viocmg_info->early_cam_parking_line_width);
	printk("early_cam_parking_line_height = %d\r\n", Viocmg_info->early_cam_parking_line_height);
	printk("early_cam_parking_line_format = %d\r\n", Viocmg_info->early_cam_parking_line_format);
}

unsigned int cm4_get_camera_type(void) {
	return CM4SyncParameters->Camera_type;
}

#define SYNC_BASE_SIZE	0x1000
#define SYNC_BASE_ADDR	(HwCORTEXM4_DATA_MEM_BASE + HwCORTEXM4_DATA_MEM_SIZE - SYNC_BASE_SIZE)

void tcc_cif_sync_parameter(void) {
	CM4SyncParameters = (struct tcc_cif_parameters *)SYNC_BASE_ADDR;
	Viocmg_info = (struct viocmg_info *)(&CM4SyncParameters->viocmg_info);

//	tcc_cif_parameters_dump();
}

int tcc_cif_reset_vioc_path(void) {
	pIREQConfig = (VIOC_IREQ_CONFIG *)(HwVIOC_IREQ);	//vioc ireq

	BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1 << 31));						// reset AsyncFIFO
	BITSET(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_display_rdma)));		// reset RDMA
	BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1 << (Viocmg_info->early_cam_vin_wdma)));		// reset WDMA
	BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1 << (Viocmg_info->early_cam_vin_wmix+9)));		// reset WMIXer
	BITSET(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_vin_scaler+28)));		// reset SCaler
	if(Viocmg_info->feature_early_view_use_viqe)
		BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1<<16));					// reset VIQE
	else
		BITSET(pIREQConfig->uSOFTRESET.nREG[1], (1<<17));					// reset DEINTL_S
	BITSET(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_vin_vin +	24)));		// reset VIN
	if(Viocmg_info->feature_early_view_use_parking_line)
		BITSET(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_vin_rdma)));	// reset PGL
	mdelay(1);
	if(Viocmg_info->feature_early_view_use_parking_line)
		BITCLR(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_vin_rdma)));	// clear PGL
	BITCLR(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_vin_vin +	24)));		// clear VIN
	if(Viocmg_info->feature_early_view_use_viqe)
		BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1<<16));					// clear VIQE
	else
		BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1<<17));					// clear DEINTL_S
	BITCLR(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_vin_scaler+28)));		// clear SCaler
	BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1 << (Viocmg_info->early_cam_vin_wmix+9)));		// clear WMIXer
	BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1 << (Viocmg_info->early_cam_vin_wdma)));		// clear WDMA
	BITCLR(pIREQConfig->uSOFTRESET.nREG[0], (1 << (Viocmg_info->early_cam_display_rdma)));		// clear RDMA
	BITCLR(pIREQConfig->uSOFTRESET.nREG[1], (1 << 31));						// clear AsyncFIFO

	return 0;
}

void tcc_cif_read_regs(unsigned char slave_addr, int i2c_channel, unsigned char cmd, unsigned char *value)
{
	unsigned char send_data[1], read_data[1];
//	unsigned int decoder_chip = cm3_get_decoder_chip();

	log("start. \n");

	send_data[0] = cmd;
	read_data[0] = 0;

	i2c_xfer(slave_addr, 1, send_data, 1, read_data, i2c_channel);
	*value = read_data[0];
//	*value = 1;
	log("end. \n");
}

int R_Gear_Status_Check() {
	int ckeck_gpio_port = gpio_get(Viocmg_info->early_cam_gear_port);
	return (ckeck_gpio_port == Viocmg_info->early_cam_gear_port_active_level) ? 1 : 0;
}

void tcc_R_Gear_port_init()
{
	gpio_config(Viocmg_info->early_cam_gear_port,GPIO_INPUT);
	gpio_config(TCC_GPB(4),GPIO_OUTPUT|GPIO_LOW);
}

int tcc_cif_set_rdma(VIOC_RDMA* pRDMA, unsigned int base_addr, int width, int height, int fmt)
{
	VIOC_RDMA_SetImageFormat(pRDMA, fmt);
	VIOC_RDMA_SetImageSize(pRDMA, width, height);
	VIOC_RDMA_SetImageOffset(pRDMA, fmt, width);
	VIOC_RDMA_SetImageBase(pRDMA, base_addr, (unsigned int)NULL, (unsigned int)NULL);

	// Because of using YCbCr color space LUT formala to set VIN_LUT
	if(cm4_get_camera_type() == CAM_TYPE_LVDS) {
		VIOC_RDMA_SetImageR2YMode(pRDMA, 2);
		VIOC_RDMA_SetImageR2YEnable(pRDMA, 1);
	}
	//

	VIOC_RDMA_SetImageEnable(pRDMA);

	return 0;
}

int tcc_cif_set_wmix(VIOC_WMIX * pWMIX, int width, int height) {
	unsigned int layer		= 0x0;

#if 0
	// default
	unsigned int key_R		= 0xFF;
	unsigned int key_G		= 0xFF;
	unsigned int key_B		= 0xFF;
	unsigned int key_mask_R 	= 0xF8;
	unsigned int key_mask_G 	= 0xF8;
	unsigned int key_mask_B 	= 0xF8;
#else
	// for tw9921
	unsigned int key_R		= 0xFF;
	unsigned int key_G		= 0x00;
	unsigned int key_B		= 0xC6;
	unsigned int key_mask_R 	= 0xF8;
	unsigned int key_mask_G 	= 0xFC;
	unsigned int key_mask_B 	= 0xF8;
	
	// Because of using YCbCr color space LUT formala to set VIN_LUT
	unsigned int key_Y	= 0;
	unsigned int key_Cb	= 0;
	unsigned int key_Cr	= 0;
	//
#endif

	// Because of using YCbCr color space LUT formala to set VIN_LUT
	if(cm4_get_camera_type() == CAM_TYPE_LVDS) {
		key_Y	=  0.213*(double)key_R +  0.715*(double)key_G + 0.072*(double)key_B;
		key_Cb	= -0.117*(double)key_R + -0.394*(double)key_G + 0.511*(double)key_B + (double)128;
		key_Cr	=  0.511*(double)key_R -  0.464*(double)key_G - 0.047*(double)key_B + (double)128;
	
		log("key_Y : %d, key_Cb : %d, key_Cr : %d \n", key_Y, key_Cb, key_Cr);
	}
	//
	VIOC_WMIX_SetSize(pWMIX, width, height);

	if(Viocmg_info->feature_early_view_use_parking_line && pgl_enable) {
		// Because of using YCbCr color space LUT formala to set VIN_LUT
		if(cm4_get_camera_type() == CAM_TYPE_LVDS) {
			VIOC_WMIX_SetChromaKey(pWMIX, layer, ON, key_Y, key_Cb, key_Cr, key_mask_R, key_mask_G, key_mask_B);
		}
		else {
			VIOC_WMIX_SetChromaKey(pWMIX, layer, ON, key_R, key_G, key_B, key_mask_R, key_mask_G, key_mask_B);
		}
		VIOC_WMIX_SetPosition(pWMIX, layer+1, Viocmg_info->early_cam_parking_line_x, Viocmg_info->early_cam_parking_line_y);
		VIOC_CONFIG_WMIXPath(WMIX50, 1);
	}
	else
		VIOC_CONFIG_WMIXPath(WMIX50, 0);

	VIOC_WMIX_SetUpdate(pWMIX);

	return 0;
}

int tcc_cif_set_vin(void) {
	VIOC_VIN_SetSyncPolarity(pVINBase, !CM4SyncParameters->Cam_h_sync_pol, !CM4SyncParameters->Cam_v_sync_pol,	\
		CM4SyncParameters->Cam_field_bfield_low, CM4SyncParameters->Cam_de_pol,CM4SyncParameters->Cam_gen_field_en,!CM4SyncParameters->Cam_p_clock_pol);
	VIOC_VIN_SetCtrl(pVINBase, CM4SyncParameters->Cam_conv_en, CM4SyncParameters->Cam_hsde_connect_en, CM4SyncParameters->Cam_vs_mask, CM4SyncParameters->Cam_input_fmt, CM4SyncParameters->Cam_data_order);
	VIOC_VIN_SetInterlaceMode(pVINBase, CM4SyncParameters->Cam_intl_en, CM4SyncParameters->Cam_intpl_en);

	VIOC_VIN_SetImageSize(pVINBase, CM4SyncParameters->Cam_preview_w, CM4SyncParameters->Cam_preview_h);
	//2017.12.22 - LVDS SVM Display Timing Fixed. Horizontal Blank(128), Vertical Blank(4)
	//2018.06.15 - LVDS SVM PCLK Changed(2208 -> 2528). Horizontal Blank(128+320 = 448)
	if(CM4SyncParameters->Camera_type == CAM_TYPE_LVDS)
	{
		if(gpio_get(TCC_GPB(19)))
			VIOC_VIN_SetImageOffset(pVINBase, 448, 4, 0);
		else
			VIOC_VIN_SetImageOffset(pVINBase, 128, 4, 0);
	}
	else
		VIOC_VIN_SetImageOffset(pVINBase, 0, 0, 0);
	VIOC_VIN_SetImageCropSize(pVINBase, CM4SyncParameters->Cam_preview_w, CM4SyncParameters->Cam_preview_h);
	VIOC_VIN_SetImageCropOffset(pVINBase, 0, 0);
	VIOC_VIN_SetY2RMode(pVINBase, 2);
	// Because of using YCbCr color space LUT formala to set VIN_LUT
	if(cm4_get_camera_type() == CAM_TYPE_LVDS) {
		VIOC_VIN_SetY2REnable(pVINBase, 0);
		VIOC_VIN_SetLUT(pVINBase, &(pVINBase->uVIN_LUT_C0));
		VIOC_VIN_SetLUTEnable(pVINBase, ON, ON, ON);
	}
	else {
		VIOC_VIN_SetY2REnable(pVINBase, !Viocmg_info->feature_early_view_use_viqe);
		VIOC_VIN_SetLUTEnable(pVINBase, OFF, OFF, OFF);
	}

	VIOC_VIN_SetEnable(pVINBase, ON);

	return 0;
}

void tcc_cif_set_viqe(char bUseVIQE)
{
	unsigned int plugin_val;

	if(bUseVIQE) {
		log("Deinterlacer: VIQE\n");

		unsigned int	viqe_width	= 0;
		unsigned int	viqe_height	= 0;
		unsigned int	format		= FMT_FC_YUV420;
		unsigned int	bypass_deintl	= VIOC_VIQE_DEINTL_MODE_3D;
		unsigned int deintl_base0 = CM4SyncParameters->Viqe_area;
		unsigned int deintl_base1 = deintl_base0 + (CM4SyncParameters->Cam_preview_w*CM4SyncParameters->Cam_preview_h*2*2);
		unsigned int deintl_base2 = deintl_base1 + (CM4SyncParameters->Cam_preview_w*CM4SyncParameters->Cam_preview_h*2*2);
		unsigned int deintl_base3 = deintl_base2 + (CM4SyncParameters->Cam_preview_w*CM4SyncParameters->Cam_preview_h*2*2);

		unsigned int	cdf_lut_en	= OFF;
		unsigned int	his_en		= OFF;
		unsigned int	gamut_en	= OFF;
		unsigned int	d3d_en		= OFF;
		unsigned int	deintl_en	= ON;

		switch(Viocmg_info->early_cam_vin_vin)
		{
				case 0:
						plugin_val = 0xA; // VIOC_VIQE_VIN00
						break;
				case 1:
						plugin_val = 0xC; // VIOC_VIQE_VIN01
						break;
				case 2:
						plugin_val = 0x8; // VIOC_VIQE_VIN02
						break;
				case 3:
						plugin_val = 0x9; // VIOC_VIQE_VIN03
						break;
		}

		VIOC_CONFIG_PlugIn(VIOC_VIQE, plugin_val /*VIOC_VIQE_VIN_00*/);
		VIOC_VIQE_SetDeintlMode(pVIQEBase, 2);
		VIOC_VIQE_SetImageY2RMode(pVIQEBase,2);
		VIOC_VIQE_SetImageY2REnable(pVIQEBase,ON);
		VIOC_VIQE_SetControlRegister(pVIQEBase, viqe_width, viqe_height, format);
		VIOC_VIQE_SetDeintlRegister(pVIQEBase, format, OFF, viqe_width, viqe_height, bypass_deintl, \
			(unsigned int)deintl_base0, (unsigned int)deintl_base1, (unsigned int)deintl_base2, (unsigned int)deintl_base3);
		VIOC_VIQE_SetControlEnable(pVIQEBase, cdf_lut_en, his_en, gamut_en, d3d_en, deintl_en);

		// [ IM002A-863 / IS001A-2552 ] @ 2016-09-08: Jay
		log("CM3(E-CAM): pVIQEBase->cDEINTL_COMP.nVIQE_FC_MISC.nREG = 0x%08x\n", &pVIQEBase->cDEINTL_COMP.nVIQE_FC_MISC.nREG);
		log("CM3(E-CAM): pVIQEBase->cDEINTL.nDI_FMT = 0x%08x\n", &pVIQEBase->cDEINTL.nDI_FMT);
		BITCSET(pVIQEBase->cDEINTL_COMP.nVIQE_FC_MISC.nREG, 0x0000f000, (1 << 12));     // 0x160
		BITCSET(pVIQEBase->cDEINTL.nDI_FMT,         0x00000001, (1 <<  0));     // 0x2E8

		// [ IM002A-863 / IS001A-2552 ] @ 2016-09-08: Jay
		log("CM3(E-CAM): pVIQEBase->cDEINTL.nDI_ENGINE0 = 0x%08x\n", &pVIQEBase->cDEINTL.nDI_ENGINE0);
		log("CM3(E-CAM): pVIQEBase->cDEINTL.nDI_ENGINE4 = 0x%08x\n", &pVIQEBase->cDEINTL.nDI_ENGINE4);
		BITCSET(pVIQEBase->cDEINTL.nDI_ENGINE0,     0xffffffff, 0x0204ff08);    // 0x284
		BITCSET(pVIQEBase->cDEINTL.nDI_ENGINE4,     0xffffffff, 0x124f2582);    // 0x294

		// [ IM002A-863 / IS001A-2552 ] @ 2016-09-22: Jay
		log("CM3(E-CAM): pVIQEBase->cDEINTL.nDI_CTRL = 0x%08x\n", &pVIQEBase->cDEINTL.nDI_CTRL);
		log("CM3(E-CAM): pVIQEBase->cDEINTL.nDI_ENGINE3 = 0x%08x\n", &pVIQEBase->cDEINTL.nDI_ENGINE3);
		BITCSET(pVIQEBase->cDEINTL.nDI_CTRL, ((1<<5)|(1<<4)|(1<<0)), ((0<<5)|(0<<4)|(0<<0))); // 0x280
		BITCSET(pVIQEBase->cDEINTL.nDI_ENGINE3, 0xfff00000,  (0<<20));   // 0x290
	} else {
		log("Deinterlacer: DEINTL_S\n");

		VIOC_CONFIG_PlugIn(VIOC_DEINTLS, 10 /*VIOC_VIQE_VIN_00*/);
#if defined(PLATFORM_TCC897X)
		VIOC_DEINTLS_SetDeIntlMode(pDeintls, 3);
#endif
	}
}

int tcc_camera_set_scaler(unsigned long overscan_position_x, unsigned long overscan_position_y,
	unsigned int overscan_additional_width, unsigned int overscan_additional_height, int width, int height) {
	unsigned int plugin_val;

	switch(Viocmg_info->early_cam_vin_vin) {
	case 0:
		plugin_val = 0x10; // VIOC_SC_VIN00
		break;
	case 1:
		plugin_val = 0x12; // VIOC_SC_VIN01
		break;
	case 2:
		plugin_val = 0x0C; // VIOC_SC_VIN02
		break;
	case 3:
		plugin_val = 0x0E; // VIOC_SC_VIN03
		break;
	}

	VIOC_CONFIG_PlugIn(VIOC_SC0 + Viocmg_info->early_cam_vin_scaler, plugin_val);
	VIOC_SC_SetBypass(pSCBase, OFF);

	VIOC_SC_SetDstSize(pSCBase, width+overscan_additional_width, height+overscan_additional_height);
	VIOC_SC_SetOutPosition(pSCBase, overscan_position_x, overscan_position_y);
	VIOC_SC_SetOutSize(pSCBase, width, height);
	VIOC_SC_SetUpdate(pSCBase);

	return 0;
}

void tcc_cif_set_wdma()
{
	VIOC_WDMA_SetImageFormat(pWDMABase, Image_info.fmt);
	VIOC_WDMA_SetImageSize(pWDMABase, Image_info.Frame_width, Image_info.Frame_height);
	VIOC_WDMA_SetImageOffset(pWDMABase, Image_info.fmt, Image_info.Frame_width);

	//set base address for write DMA.
	VIOC_WDMA_SetImageBase(pWDMABase, 	Image_info.addr0,Image_info.addr1, Image_info.addr2);

	// Because of using YCbCr color space LUT formala to set VIN_LUT
	if(cm4_get_camera_type() == CAM_TYPE_LVDS) {
		VIOC_WDMA_SetImageY2RMode(pWDMABase, 2);
		VIOC_WDMA_SetImageY2REnable(pWDMABase, 1);	
	}
	//

	// operating start in 1 frame
	VIOC_WDMA_SetImageEnable(pWDMABase, ON);

	// clear EOFF bit
	BITCSET(pWDMABase->uIRQSTS.nREG, 1<<6, 1<<6);

	VIOC_WDMA_SetIreqMask(pWDMABase, VIOC_WDMA_IREQ_EOFF_MASK, 0x0);
}

void tcc_cif_enable_async_fifo(unsigned int * buf, unsigned int nWDMA, unsigned int nRDMA0, unsigned int nRDMA1, unsigned int nRDMA2) {
	VIOC_FIFO * pFIFO = (VIOC_FIFO *)HwVIOC_FIFO;

	log("WDMA: 0x%x, RDMA0: 0x%x, RDMA1: 0x%x, RDMA2: 0x%x\n", nWDMA, nRDMA0, nRDMA1, nRDMA2);

	VIOC_ASYNC_FIFO_ConfigDMA(pFIFO, nWDMA, nRDMA0, nRDMA1, nRDMA2);
	VIOC_ASYNC_FIFO_ConfigEntry(pFIFO, buf);
	VIOC_ASYNC_FIFO_SetEnable(pFIFO, 1, 1, 0, 0);
}

void tcc_cif_disable_async_fifo(void) {
	VIOC_FIFO * pFIFO = (VIOC_FIFO *)HwVIOC_FIFO;

	VIOC_ASYNC_FIFO_SetEnable(pFIFO, 0, 0, 0, 0);
}

void tcc_cif_start_stream(void) {
	unsigned int	offset = 0, idxBuf = 0;

	//default path
	//preview path	: vin00		-	scaler0/viqe	-	wmix05	-	wdma05
	//pgl path		: rdma16	-
	pVINBase = (VIOC_VIN *)(HwVIOC_VIN00 + (0x1000 * Viocmg_info->early_cam_vin_vin));
	pRDMABase = (VIOC_RDMA*)(HwVIOC_RDMA00 + (HwVIOC_RDMA_GAP * Viocmg_info->early_cam_vin_rdma));
	pSCBase = (VIOC_SC *)(HwVIOC_SC0 + (HwVIOC_RDMA_GAP * Viocmg_info->early_cam_vin_scaler));
	pWMIXBase = (VIOC_WMIX *)(HwVIOC_WMIX0 + (HwVIOC_RDMA_GAP * Viocmg_info->early_cam_vin_wmix));
	pWDMABase = (VIOC_WDMA *)(HwVIOC_WDMA00 + (HwVIOC_RDMA_GAP * Viocmg_info->early_cam_vin_wdma));
	pDDIConfig = (DDICONFIG *)(HwDDI_CONFIG_BASE);	//ddi config
	pIREQConfig = (VIOC_IREQ_CONFIG *)(HwVIOC_IREQ);	//vioc ireq
	if(CM4SyncParameters->Cam_intl_en) {
		if(Viocmg_info->feature_early_view_use_viqe)
			pVIQEBase	= (VIQE		*)HwVIOC_VIQE0;
#if defined(PLATFORM_TCC897X)
		else
			pDeintls	= (unsigned int	*)HwVIOC_DEINTLS;
#endif
	}

	// allocate preview memory
	offset = Viocmg_info->early_cam_preview_width * Viocmg_info->early_cam_preview_height * 4;
	for(idxBuf=0; idxBuf < PREVIEW_BUFFER_NUMBER; idxBuf++) {
		addrPreview[idxBuf] = CM4SyncParameters->Lcdc_address0 + (offset * idxBuf);
		log("addrPreview[%d] = 0x%08x\n", idxBuf, addrPreview[idxBuf]);
	}

	// clear 1st memory.
	memset((void *)CM4SyncParameters->Lcdc_address0, 0, Viocmg_info->early_cam_preview_width * Viocmg_info->early_cam_preview_height * 4);

	// reset vioc component
	tcc_cif_reset_vioc_path();

	/***** set LCD display information *****/

	Image_info.Lcdc_layer = 1;
#if 0
	Image_info.Frame_width = CM4SyncParameters->Lcdc_Frame_width;
	Image_info.Frame_height = CM4SyncParameters->Lcdc_Frame_height;
	Image_info.Image_width = CM4SyncParameters->Lcdc_Image_width;
	Image_info.Image_height = CM4SyncParameters->Lcdc_Image_height;
	Image_info.offset_x = CM4SyncParameters->Lcdc_offset_x;
	Image_info.offset_y = CM4SyncParameters->Lcdc_offset_y;
	Image_info.addr0 = CM4SyncParameters->Lcdc_address0;
	Image_info.addr1 = CM4SyncParameters->Lcdc_address1;
	Image_info.addr2 = CM4SyncParameters->Lcdc_address2;
	Image_info.fmt = CM4SyncParameters->Lcdc_format;
#endif
	Image_info.Frame_width = Viocmg_info->early_cam_preview_width;
	Image_info.Frame_height = Viocmg_info->early_cam_preview_height;
	Image_info.Image_width = Viocmg_info->early_cam_parking_line_width;
	Image_info.Image_height = Viocmg_info->early_cam_parking_line_height;
	Image_info.offset_x = Viocmg_info->early_cam_preview_x;
	Image_info.offset_y = Viocmg_info->early_cam_preview_y;
	Image_info.addr0 = CM4SyncParameters->Lcdc_address0;
	Image_info.addr1 = CM4SyncParameters->Lcdc_address1;
	Image_info.addr2 = CM4SyncParameters->Lcdc_address2;
	Image_info.fmt = Viocmg_info->early_cam_preview_format;

	BITCSET(pDDIConfig->CIFPORT.nREG, 0x00077777, Viocmg_info->early_cam_cifport << (Viocmg_info->early_cam_vin_vin * 4));

	tcc_R_Gear_port_init();

	if(Viocmg_info->feature_early_view_use_parking_line && pgl_enable) {
		/* Global alpha settings  of parking guide-line*/
		VIOC_RDMA_SetImageAlphaEnable(pRDMABase, 1);
		VIOC_RDMA_SetImageAlpha(pRDMABase, 0xff,0xff);

		tcc_cif_set_rdma(pRDMABase, CM4SyncParameters->PGL_addr,	\
			Viocmg_info->early_cam_parking_line_width,		\
			Viocmg_info->early_cam_parking_line_height,		\
			Viocmg_info->early_cam_parking_line_format);
	}

	// WMIX path setting
	if(Viocmg_info->early_cam_vin_vin == 3) {
		pIREQConfig->uMISC.bREG.RDMA14 = 1;
	}

	// set VIN
	tcc_cif_set_vin();

	// set Deinterlacer
	if(CM4SyncParameters->Cam_intl_en)
		tcc_cif_set_viqe(Viocmg_info->feature_early_view_use_viqe);

	if(Viocmg_info->early_cam_preview_width != CM4SyncParameters->Cam_preview_w ||
		Viocmg_info->early_cam_preview_height != CM4SyncParameters->Cam_preview_h) {
		// set SCaler
		if(cm4_get_camera_type() == CAM_TYPE_LVDS) {
			tcc_camera_set_scaler(Viocmg_info->early_cam_preview_crop_x, Viocmg_info->early_cam_preview_crop_y,
					Viocmg_info->early_cam_preview_additional_width, Viocmg_info->early_cam_preview_additional_height,
					Viocmg_info->early_cam_preview_width, Viocmg_info->early_cam_preview_height);
		}
		else {
			tcc_camera_set_scaler(Viocmg_info->early_cam_preview_crop_x, Viocmg_info->early_cam_preview_crop_y,
					Viocmg_info->early_cam_preview_additional_width, Viocmg_info->early_cam_preview_additional_height,
					Viocmg_info->early_cam_preview_width - 640, Viocmg_info->early_cam_preview_height);
		}
	}

	// set WMIXer
	tcc_cif_set_wmix(pWMIXBase, Viocmg_info->early_cam_preview_width, Viocmg_info->early_cam_preview_height);

	// set WDMA
	tcc_cif_set_wdma();
	mdelay(70);

	// set Display
	tcc_lcd_image_ch_set(&Image_info);

	// set AsyncFIFO
	tcc_cif_enable_async_fifo(addrPreview,		\
		Viocmg_info->early_cam_vin_wdma,	\
		Viocmg_info->early_cam_display_rdma,	\
		Viocmg_info->early_cam_display_rdma,	\
		Viocmg_info->early_cam_display_rdma);
}

void tcc_cif_stop_stream(void) {
	VIOC_RDMA	* pDISPRDMABase = (VIOC_RDMA*)((HwVIOC_RDMA00 + (HwVIOC_RDMA_GAP * Viocmg_info->early_cam_display_rdma)));

	int		idxLoop = 0;

	// disable AsyncFIFO
	tcc_cif_disable_async_fifo();

	// disable RDMA
	if(VIOC_RDMA_GetImageEnable(pDISPRDMABase))
		VIOC_RDMA_SetImageDisable(pDISPRDMABase);

	// disable WDMA
	VIOC_WDMA_SetIreqMask(pWDMABase, VIOC_WDMA_IREQ_ALL_MASK, ON);	// disable WDMA interrupt
	VIOC_WDMA_SetImageDisable(pWDMABase);
	for(idxLoop=0; idxLoop<1000; idxLoop++) {
		if(pWDMABase->uIRQSTS.nREG & VIOC_WDMA_IREQ_EOFR_MASK)
			break;
		else
			log("[%02d] WDMA: %p, CTRL: 0x%08x, STATUS: 0x%08x\n", idxLoop, pWDMABase, \
				(unsigned)pWDMABase->uCTRL.nREG, (unsigned)pWDMABase->uIRQSTS.nREG);
		mdelay(1);
	}

	// disable WMIX, but don't care

	// disable SCaler
	VIOC_CONFIG_PlugOut(VIOC_SC0 + Viocmg_info->early_cam_vin_scaler);

	// disable Deinterlacer
	if(CM4SyncParameters->Cam_intl_en) {
		if(Viocmg_info->feature_early_view_use_viqe) {
			VIQE* pVIQE = (VIQE *)HwVIOC_VIQE0;
			VIOC_VIQE_SetControlEnable(pVIQE, OFF, OFF, OFF, OFF, OFF);
			VIOC_CONFIG_PlugOut(VIOC_VIQE);
#if defined(PLATFORM_TCC897X)
		} else {
			VIOC_CONFIG_PlugOut(VIOC_DEINTLS);
#endif
		}
	}

	// disable VIN
	VIOC_VIN_SetEnable(pVINBase, OFF);
	mdelay(1);

	// disable PGL
	if(Viocmg_info->feature_early_view_use_parking_line)
		VIOC_RDMA_SetImageDisable(pRDMABase);

	// reset vioc component
	tcc_cif_reset_vioc_path();
}

int tcc_cif_wdma_counter(void) {
	volatile unsigned int prev_addr, curr_addr;
	volatile int nCheck, idxCheck, delay = 10;

	curr_addr = pWDMABase->uCBASE;
	mdelay(delay);

	nCheck = 50;
	for(idxCheck=0; idxCheck<nCheck; idxCheck++) {
		prev_addr = curr_addr;
		mdelay(delay);
		curr_addr = pWDMABase->uCBASE;

		if(prev_addr != curr_addr)
			return 1;
		else
			log("[%d] prev_addr: 0x%08x, curr_addr: 0x%08x\n", idxCheck, prev_addr, curr_addr);
	}
	tcc_cif_vioc_register_dump();
	log("WDMA Current Address is the same.\n");
	return 0;
}

char tcc_video_check(void) {
	unsigned char value;

	log("start\n");
	value = sensor_if_video_check();
	log("return value : 0x%x \n", value);
	return value;
}

int tcc_cif_get_video(void) {
	int ret = 0;

	if(!tcc_cif_wdma_counter()) {
		tcc_rear_camera_display(0);

		printk("EarlyCamera Recovery!! \n");

		tcc_cif_stop_stream();
		mdelay(20);

		sensor_if_open(cm4_get_camera_type(), CAM_ENC_DEFAULT);
		mdelay(20);

		tcc_cif_start_stream();

		if(!sensor_if_video_check())
			ret = 1;
		else
			ret = 0;

		mdelay(10);
	}
	else
		ret = 1;

	return ret;
}


void tcc_cif_set_pgl(void) {
	if(Viocmg_info->feature_early_view_use_parking_line && pgl_enable) {
		/* Global alpha settings  of parking guide-line*/
		VIOC_RDMA_SetImageAlphaEnable(pRDMABase, 1);
		VIOC_RDMA_SetImageAlpha(pRDMABase, 0xff,0xff);

		tcc_cif_set_rdma(pRDMABase, CM4SyncParameters->PGL_addr, Viocmg_info->early_cam_parking_line_width,
							Viocmg_info->early_cam_parking_line_height, Viocmg_info->early_cam_parking_line_format);

		tcc_cif_set_wmix(pWMIXBase, Viocmg_info->early_cam_preview_width, Viocmg_info->early_cam_preview_height);
	}
}

void tcc_cif_vioc_register_dump(void) {
	struct reg_test {
		unsigned int * reg;
		unsigned int cnt;
	};

	struct reg_test regList[] = {
	//	{ (unsigned int *)HwVIOC_RDMA16,		12 },
		{ (unsigned int *)HwVIOC_VIN00, 		16 },
		{ (unsigned int *)HwVIOC_SC0,			 8 },
		{ (unsigned int *)HwVIOC_WMIX5, 		28 },
		{ (unsigned int *)HwVIOC_WDMA05,		18 },
//		{ (unsigned int *)HwVIOC_RDMA00,		12 },
		{ (unsigned int *)HwVIOC_RDMA01,		12 },
		{ (unsigned int *)HwVIOC_RDMA02,		12 },
//		{ (unsigned int *)HwVIOC_RDMA03,		12 },
		{ (unsigned int *)HwVIOC_WMIX0, 		28 },
//		{ (unsigned int *)HwVIOC_RDMA04,		12 },
//		{ (unsigned int *)HwVIOC_RDMA05,		12 },
//		{ (unsigned int *)HwVIOC_RDMA06,		12 },
//		{ (unsigned int *)HwVIOC_RDMA07,		12 },
//		{ (unsigned int *)HwVIOC_WMIX1, 		28 },
//		{ (unsigned int *)HwVIOC_VIQE0, 		18 },
		{ (unsigned int *)0x7238000c,	 		8 },
		{ (unsigned int *)0x7200a040,	 		28 },
		{ (unsigned int *)0x7200a054, 			28 },
		{ (unsigned int *)(0x74200000+GPIO_PORTD),			16 },
	};

	unsigned int * addr;
	unsigned int reg, idxLoop, nReg, idxReg;

	printk("\n\n");
	for(idxLoop=0; idxLoop<sizeof(regList)/sizeof(regList[0]); idxLoop++) {
		addr	= regList[idxLoop].reg;
		nReg	= regList[idxLoop].cnt;

		for(idxReg=0; idxReg<nReg; idxReg++) {
			if((idxReg%4) == 0)
				printk("\n%08x: ", (unsigned int)(addr + idxReg));

			REGREAD((unsigned int)(addr + idxReg), reg);
			printk("%08x ", reg);
		}
		printk("\n");
	}
	printk("\n\n");
}

void tcc_cif_ie_lut(void) {
	int vR, vG, vB, i;
	int fsaturation, fcontrast;

	int rgb[257] = {0, };
	int _video_brightness = CM4SyncParameters->Cam_brightness;
	int _video_contrast = CM4SyncParameters->Cam_contrast;
	int _video_saturation = CM4SyncParameters->Cam_saturation;

	const int MULTI_NUM = 1000;
	const int SATURATION_RANGE = 4; // -X ~ +X
	const int CONTRAST_RANGE = 4; // -X ~ +X

	if (_video_brightness < 0 || _video_brightness > 255)
		_video_brightness = 127;
	if (_video_contrast < 0 || _video_contrast > 255)
		_video_contrast = 127;
	if (_video_saturation < 0 || _video_saturation > 255)
		_video_saturation = 127;

	for (i = 0; i < 256; i++)
	{
		//contrast brightness
		if(_video_contrast > 127)
			fcontrast = (1 * MULTI_NUM) + ((_video_contrast -127) * MULTI_NUM * (CONTRAST_RANGE - 1)) / 127;
		else
			fcontrast = (_video_contrast * MULTI_NUM) / 127;

		vR = ((i - 16)*fcontrast)/MULTI_NUM + 16;

		//brightness
		if(vR > 255)    vR = 255;
		if(vR < 0)      vR = 0;
		if(_video_brightness >= 127)
			vR = vR + ((_video_brightness - 127) * 2);
		else
			vR = vR + ((_video_brightness - 127) * 2) - 1;

		//saturation
		if(_video_saturation > 127)
			fsaturation = (1 * MULTI_NUM) + ((_video_saturation - 127) * MULTI_NUM * (SATURATION_RANGE - 1)) / 127;
		else
			fsaturation = (_video_saturation * MULTI_NUM) / 127;

		//fsaturation = (1* MULTI_NUM)+ ((((_video_saturation - 127) * MULTI_NUM) / 127) * SATURATION_RANGE);

		vG = vB = (((i - 128)*fsaturation * fcontrast)/MULTI_NUM)/MULTI_NUM + 128;

		if(vR > 255)    vR = 255;
		if(vR < 0)      vR = 0;
		if(vG > 255)    vG = 255;
		if(vG < 0)      vG = 0;
		if(vB > 255)    vB = 255;
		if(vB < 0)      vB = 0;

		rgb[i] =((vR & 0xff) << 16) | ((vG & 0xff) << 8) | ((vB & 0xff));

		//VPRINTK("%s count: %d rgb:0x%2x\n", __func__, i, rgb[i]);
	}
	printk("%s _video_brightness:%d  _video_contrast:%d  _video_saturation:%d \n",
			__func__, _video_brightness, _video_contrast, _video_saturation);

	VIOC_VIN_SetLUT_by_table(pVINBase, rgb);

	return;
}
