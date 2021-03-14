/*
 * Copyright (c) 2010 Telechips, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <platform/iomap.h>
#include <dev/fbcon.h>
#include <plat/cpu.h>

#include <lcd.h>
#include <i2c.h>
#include <dev/gpio.h>

#include <platform/reg_physical.h>
#include <platform/irqs.h>
#include <platform/tcc_ckc.h>
#include <platform/gpio.h>

#include <tcc_lcd.h>
#include <tca_ddic_interface.h>

#include "vioc/vioc_wmix.h"
#include "vioc/vioc_rdma.h"
#include "vioc/vioc_wdma.h"
#include "vioc/vioc_outcfg.h"
#include "vioc/vioc_disp.h"
#include "vioc/vioc_scaler.h"
#include "vioc/vioc_config.h"

#include <splash/splashimg.h>

#include <TCC_JPU_C6.h>

#define _LCD_32BPP_           //LCD output format setting
#define LCDC_FB_BPP		32

//#if defined(DEFAULT_DISPLAY_HDMI)
#include <regs-hdmi.h>
#include <hdmi_v1_4_hdmi.h>
#include <hdmi_v1_4_audio.h>
//#endif
#include <daudio_settings.h>
#include "../../../app/aboot/recovery.h"

#if defined(DISPLAY_SPLASH_SCREEN_DIRECT)
// Define boot image infomation
#include <lcd/logo_24bit.h>
#include <lcd/low.h>

#if defined(HDMI_1920_720_12_3)
#define LCDC_FB_WIDTH	1920
#define LCDC_FB_HEIGHT	720
#else
#define LCDC_FB_WIDTH	800
#define LCDC_FB_HEIGHT	480
#endif

#if defined(TARGET_TCC897X_LCN)
#define GPIO_LCD_ON 		(GPIO_NC)
#define GPIO_LCD_BL 		(GPIO_NC)
#define GPIO_LCD_DISPLAY	(GPIO_NC)
#define GPIO_LCD_RESET		(GPIO_NC)
#endif

static struct fbcon_config fb_lb = {
	.height		= 275,
	.width		= 481,
	.format		= FB_FORMAT_RGB565,
	.bpp		= LCDC_FB_BPP,
	.update_start	= NULL,
	.update_done	= NULL,
	.base = lowbattery_logo
};

static struct fbcon_config fb_cfg = {
	.height		= LCDC_FB_HEIGHT,
	.width		= LCDC_FB_WIDTH,
	.format		= FB_FORMAT_RGB565,
	.bpp		= LCDC_FB_BPP,
	.update_start	= NULL,
	.update_done	= NULL,
	.base = telechips_logo
};

static struct fbcon_config fb_fr = {
	.height         = LCDC_FB_HEIGHT,
	.width          = LCDC_FB_WIDTH,
	.format         = FB_FORMAT_RGB565,
	.bpp            = LCDC_FB_BPP,
	.update_start   = NULL,
	.update_done    = NULL,
	.base = factory_reset_msg
};
#else
static struct fbcon_config fb_cfg;
static struct fbcon_config fb_fr;
#endif//

// Boot loader image momory location.
//#define BOOT_LOGO_MEMORY 		((VIDEO_BASE+VIDEO_SIZE) - ((fb_cfg.width * fb_cfg.height) *4))

#define MAX_SPLASH_IMAGE_WIDTH		800
#define MAX_SPLASH_IMAGE_HEIGHT		480

static struct fbcon_config fb0_cfg_splash;
static struct fbcon_config fb1_cfg_splash;

pmap_info_t gPmap[PMAP_MAX];

enum {
	LCDC_COMPOSITE_NTSC,
	LCDC_COMPOSITE_PAL,
	LCDC_COMPOSITE_MAX
};

enum {
	LCDC_COMPONENT_480I_NTSC,
	LCDC_COMPONENT_576I_PAL,
	LCDC_COMPONENT_720P,
	LCDC_COMPONENT_1080I,
	LCDC_COMPONENT_MAX
};

static char defalut_composite_resolution = LCDC_COMPOSITE_NTSC;
static char defalut_component_resolution = LCDC_COMPONENT_720P;

#define LCD_LCDC_NUM 	1

#define GPIO_LCD_ON         TCC_GPA(4)
#define GPIO_LCD_BL         TCC_GPA(4)
#define GPIO_LCD_DISPLAY    TCC_GPA(4)
#define GPIO_LCD_RESET      TCC_GPA(4)

#define HwVIOC_CONFIG_MISC1		(0x7200A084)
#define HwVIOC_CONFIG_DEV_SEL		(0x7200A0BC)

struct atag_tcc_entry {
	char output;
	char resolution;
	char hdmi_resolution;
	char composite_resolution;
	char component_resolution;
	char hdmi_mode;
};

extern struct atag_tcc_entry tcc_param;


//buffalo+
//2016.08.18
static void display_bootlogo_HDMI(struct fbcon_config *fb_cfg)
{
	int snapshot_header = 0;
	int ret = -1;

	dprintf(INFO , "display_bootlogo_HDMI \n");

	VARIANT_CODE_INFO_t variant_code_info;

	snapshot_header = snapshot_boot_header();
	dprintf(INFO , "snapshot_boot_header %d \n", snapshot_header);

	ret = read_variant_setting(&variant_code_info);
	if (ret == 0) {
		printf("[DEBUG] variant code -------------------------  \r\n");
		printf("[DEBUG] id : %x %x %x %x  \r\n", variant_code_info.id[0], variant_code_info.id[1], variant_code_info.id[2], variant_code_info.id[3]);
		printf("[DEBUG] version : %d  \r\n", variant_code_info.version);
		printf("[DEBUG] size : %d  \r\n", variant_code_info.size);
		printf("[DEBUG] variant code [0] : %d  \r\n", variant_code_info.variant_code[0]);
		printf("[DEBUG] variant code [1] : %d  \r\n", variant_code_info.variant_code[1]);
		printf("[DEBUG] --------------------------------------  \r\n");
	}

	if (snapshot_header == -1) {
		struct recovery_message Msg;
		if(get_recovery_message(&Msg) == 0) {
			if(0 == strcmp("boot-recovery", Msg.command)) {
				dprintf(INFO , "Not valid snapshot header, But recovery update \n");
				if ( (ret == 0)
						&& (variant_code_info.version == VARIANT_STRUCT_VERSION)
						&& (strncmp(variant_code_info.id, VARIANT_STRUCT_ID, 4) == 0)) {
					if (variant_code_info.variant_code[0] == 1) {
						splash_image_load("bluelink", fb_cfg);
					}
					else {
						splash_image_load("bootlogo", fb_cfg);
					}
				}
				else {
					splash_image_load("bluelink", fb_cfg);
				}
			}
			else {
				dprintf(INFO , "Not valid snapshot header \n");
				splash_image_load("quickboot", fb_cfg);
			}
		}
		else {
			dprintf(INFO , "Not valid snapshot header \n");
			splash_image_load("quickboot", fb_cfg);
		}
	}
	else {
		dprintf(INFO , "Valid snapshot header \n");

		if (ret == 0) {
			if ((variant_code_info.version == VARIANT_STRUCT_VERSION)
					&& (strncmp(variant_code_info.id, VARIANT_STRUCT_ID, 4) == 0)) {
				if (variant_code_info.variant_code[0] == 1) {
					splash_image_load("bluelink", fb_cfg);
				}
				else {
					splash_image_load("bootlogo", fb_cfg);
				}
			}
			else {
				// not valid variant code data
				splash_image_load("bootlogo", fb_cfg);
			}
		}
		else {
			dprintf(INFO, "read_variant_setting error (%d) \r\n", ret);
			// not read emmc data
			splash_image_load("bootlogo", fb_cfg);
		}
	}
}


static void lcdc_set_logo(unsigned char lcdc_num, unsigned lcd_wd, unsigned lcd_ht, struct fbcon_config *fb_con)
{
	struct tcc_lcdc_image_update Image_info;
	dprintf(INFO, "%s fb_cfg base:%p xres:%d yres:%d bpp:%d \n", __func__, fb_con->base, fb_con->width, fb_con->height, fb_con->bpp);

	Image_info.addr0 = (unsigned int)fb_con->base;
	Image_info.Lcdc_layer = 0;
	Image_info.enable = 1;

	Image_info.Frame_width = lcd_wd;
	Image_info.Frame_height = lcd_ht;

	Image_info.Image_width = fb_con->width;
	Image_info.Image_height = fb_con->height;

#if defined(DISPLAY_SCALER_USE)
	Image_info.offset_x =  0;
	Image_info.offset_y =  0;
#else
	if(Image_info.Image_width > lcd_wd)
		Image_info.Image_width = lcd_wd;

	if(lcd_wd > Image_info.Image_width)
		Image_info.offset_x = (lcd_wd - Image_info.Image_width) / 2;
	else
		Image_info.offset_x =  0;

	if(Image_info.Image_height > lcd_ht)
		Image_info.Image_height = lcd_ht;

	if(lcd_ht > Image_info.Image_height)
		Image_info.offset_y = (lcd_ht - Image_info.Image_height) / 2;
	else
		Image_info.offset_y = 0;
#endif

	Image_info.fmt = TCC_LCDC_IMG_FMT_RGB888;
	tcclcd_image_ch_set(lcdc_num, &Image_info);

	mdelay(1);
}

//#if defined(DEFAULT_DISPLAY_HDMI)
extern void hdmi_start(void);
extern void hdmi_ddi_config_init (void);
extern int hdmi_set_hdmi_mode(int mode);
extern int hdmi_set_video_mode(const struct HDMIVideoParameter * const hdmi_video_mode);
extern int hdmi_set_audio_mode(const struct HDMIAudioParameter * const hdmi_audio_mode);
static void lcdc_io_init_hdmi(unsigned char lcdc_num)
{
	uint width, height;
	unsigned int factory_reset = 0;
	VIOC_DISP *pDISP;
	VIOC_WMIX *pWIMX;
	VIOC_RDMA *pRDMA;

	stLTIMING HDMI_TIMEp;
	stLCDCTR pCtrlParam;
	volatile PDDICONFIG pDDI_Config = (PDDICONFIG)HwDDI_CONFIG_BASE;

	struct fbcon_config *fb_con, *fb_con_splash;
	struct lcd_panel *panel_info;

	const struct HDMIVideoParameter video = {
#if (HDMI_MODE_TYPE == 1)
		/*	video.mode 				=*/	HDMI,
#else
		/*	video.mode 				=*/	DVI,
#endif
		/*	video.resolution 		=*/	gRefHdmiVideoModeList[HDMI_VIDEO_MODE_TYPE].vfmt_val,
		/*	video.colorSpace		=*/	HDMI_CS_RGB,
		/*	video.colorDepth		=*/	HDMI_CD_24,
		/*	video.colorimetry		=*/	HDMI_COLORIMETRY_NO_DATA,
		/*	video.pixelAspectRatio	=*/	gRefHdmiVideoModeList[HDMI_VIDEO_MODE_TYPE].ratio,
#if defined(TCC_HDMI_DRIVER_V1_4)
		/*  video.videoSrc          =*/ HDMI_SOURCE_EXTERNAL,
		/*  video.Video Structure   =*/ HDMI_2D_VIDEO_FORMAT
#endif
	};

	const struct HDMIAudioParameter audio = {
		/*	audio.inputPort 		=*/	I2S_PORT,
		/*	audio.outPacket 		=*/	HDMI_ASP,
		/*	audio.formatCode		=*/	LPCM_FORMAT,
		/*	audio.channelNum		=*/	CH_2,
		/*	audio.sampleFreq		=*/	SF_44KHZ,
		/*	audio.wordLength		=*/	WORD_16,

		/*	audio.i2sParam.bpc	=*/	I2S_BPC_16,
		/*	audio.i2sParam.format	=*/	I2S_BASIC,
		/*	audio.i2sParam.clk		=*/	I2S_64FS
	};

	dprintf(INFO, " LCDC NUM:%d   vfmt %d  , ratio:%d \n",  lcdc_num, gRefHdmiVideoModeList[HDMI_VIDEO_MODE_TYPE].vfmt_val, gRefHdmiVideoModeList[HDMI_VIDEO_MODE_TYPE].ratio);

	fb_con = &fb_cfg;

#if defined(DEFAULT_DISPLAY_LCD)
	gpio_set(TCC_GPEXT1(32), 1);   // V_5P0_EN
	gpio_set(TCC_GPEXT1(35), 1);   // LVDS_EN
#endif//

	if(lcdc_num) {
		pDISP = (VIOC_DISP *)HwVIOC_DISP1;
		pWIMX = (VIOC_WMIX *)HwVIOC_WMIX1;
		pRDMA = (VIOC_RDMA *)HwVIOC_RDMA04;
		tcc_set_peri(PERI_LCD1, ENABLE, HDMI_PCLK_RATE);
		VIOC_OUTCFG_SetOutConfig(VIOC_OUTCFG_HDMI, VIOC_OUTCFG_DISP1);
	}
	else {
		pDISP = (VIOC_DISP *)HwVIOC_DISP0;
		pWIMX = (VIOC_WMIX *)HwVIOC_WMIX0;
		pRDMA = (VIOC_RDMA *)HwVIOC_RDMA00;
		tcc_set_peri(PERI_LCD0, ENABLE, HDMI_PCLK_RATE);
		VIOC_OUTCFG_SetOutConfig(VIOC_OUTCFG_HDMI, VIOC_OUTCFG_DISP0);
	}

	tcc_set_ipisol_pwdn(PMU_ISOL_HDMI, 0);
	tcc_set_peri(PERI_HDMI_PCLK, ENABLE, (50 * 1000 * 1000));
	tcc_set_peri(PERI_HDMI, ENABLE, XIN_CLK_RATE);

	width = gRefHdmiVideoModeList[HDMI_VIDEO_MODE_TYPE].width;
	height = gRefHdmiVideoModeList[HDMI_VIDEO_MODE_TYPE].height;

	HDMI_TIMEp.lpw = LCDCTimimgParams[video.resolution].lpw;
	HDMI_TIMEp.lpc = LCDCTimimgParams[video.resolution].lpc + 1;
	HDMI_TIMEp.lswc = LCDCTimimgParams[video.resolution].lswc + 1;
	HDMI_TIMEp.lewc = LCDCTimimgParams[video.resolution].lewc + 1;

	HDMI_TIMEp.vdb = LCDCTimimgParams[video.resolution].vdb;
	HDMI_TIMEp.vdf = LCDCTimimgParams[video.resolution].vdf;
	HDMI_TIMEp.fpw = LCDCTimimgParams[video.resolution].fpw;
	HDMI_TIMEp.flc = LCDCTimimgParams[video.resolution].flc;
	HDMI_TIMEp.fswc = LCDCTimimgParams[video.resolution].fswc;
	HDMI_TIMEp.fewc = LCDCTimimgParams[video.resolution].fewc;
	HDMI_TIMEp.fpw2 = LCDCTimimgParams[video.resolution].fpw2;
	HDMI_TIMEp.flc2 = LCDCTimimgParams[video.resolution].flc2;
	HDMI_TIMEp.fswc2 = LCDCTimimgParams[video.resolution].fswc2;
	HDMI_TIMEp.fewc2 = LCDCTimimgParams[video.resolution].fewc2;
	dprintf("   %d %d %d  %d \n", HDMI_TIMEp.lpw, HDMI_TIMEp.lpc, HDMI_TIMEp.lswc, HDMI_TIMEp.lewc);
	dprintf("   %d %d %d  %d \n", HDMI_TIMEp.fpw, HDMI_TIMEp.flc, HDMI_TIMEp.fswc, HDMI_TIMEp.fewc);
	VIOC_DISP_SetTimingParam(pDISP, &HDMI_TIMEp);

	memset(&pCtrlParam, 0x00, sizeof(pCtrlParam));
	pCtrlParam.id = LCDCTimimgParams[video.resolution].id;
	pCtrlParam.iv = LCDCTimimgParams[video.resolution].iv;
	pCtrlParam.ih = LCDCTimimgParams[video.resolution].ih;
	pCtrlParam.ip = LCDCTimimgParams[video.resolution].ip;
	pCtrlParam.clen = 0;
	if(video.colorSpace == HDMI_CS_RGB)	{
		pCtrlParam.r2y = 0;
		pCtrlParam.pxdw = 12; //RGB888
	}
	else {
		pCtrlParam.r2y = 1;
		pCtrlParam.pxdw = 8; //RGB888
	}
	pCtrlParam.dp = LCDCTimimgParams[video.resolution].dp;
	pCtrlParam.ni = LCDCTimimgParams[video.resolution].ni;
	pCtrlParam.tv = LCDCTimimgParams[video.resolution].tv;
	pCtrlParam.opt = 0;
	pCtrlParam.stn = 0;
	pCtrlParam.evsel = 0;
	pCtrlParam.ovp = 0;

	VIOC_DISP_TurnOff(pDISP);
	VIOC_RDMA_SetImageDisable(pRDMA);

	VIOC_DISP_SetControlConfigure(pDISP, &pCtrlParam);

	VIOC_WMIX_SetOverlayPriority(pWIMX, 24);
	VIOC_WMIX_SetBGColor(pWIMX, 0x00, 0x00, 0x00, 0xff);
	VIOC_WMIX_SetSize(pWIMX, width, height);
	VIOC_WMIX_SetChromaKey(pWIMX, 0, 0, 0, 0, 0, 0xF8, 0xFC, 0xF8);
	VIOC_WMIX_SetUpdate(pWIMX);

	BITSET(pDDI_Config->PWDN.nREG, Hw2);
	BITCLR(pDDI_Config->SWRESET.nREG, Hw2);
	BITSET(pDDI_Config->SWRESET.nREG, Hw2);

	hdmi_ddi_config_init();

	hdmi_set_video_mode(&video);
	hdmi_set_hdmi_mode(video.mode);

	hdmi_check_phy_ready();

	VIOC_DISP_SetSize (pDISP, width, height);
	VIOC_DISP_SetBGColor(pDISP, 0, 0 , 0);
	VIOC_DISP_TurnOn(pDISP);

	if (video.mode == HDMI)
		hdmi_set_audio_mode(&audio);

	hdmi_start();

	if(factory_reset) {
		lcdc_set_logo(lcdc_num, width, height, fb_con);
	}
	else {
#if defined(DISPLAY_SPLASH_SCREEN_DIRECT)
		// load boot logo copy to to end of video memory
#if 0
		memcpy(BOOT_LOGO_MEMORY, fb_con->base, width * height * 4);
		fb_con->base = BOOT_LOGO_MEMORY ;
#else
		memcpy(gPmap[PMAP_FB_WMIXER].pbase, fb_con->base, width * height * 4);
		fb_con->base = gPmap[PMAP_FB_WMIXER].pbase;
#endif

//buffalo+
//2016.08.18
		display_bootlogo_HDMI(fb_con);
		lcdc_set_logo(lcdc_num, width, height, fb_con);
#else
		if(lcdc_num)
			fb_con_splash = &fb1_cfg_splash;
		else
			fb_con_splash = &fb0_cfg_splash;

		fb_con_splash->width = MAX_SPLASH_IMAGE_WIDTH;
		fb_con_splash->height = MAX_SPLASH_IMAGE_HEIGHT;
		fb_con_splash->stride = fb_con_splash->width;
		fb_con_splash->bpp = LCDC_FB_BPP;
		fb_con_splash->format = FB_FORMAT_RGB565;

		fb_con_splash->base = dma_alloc(4096, fb_con_splash->width * fb_con_splash->height * (fb_con_splash->bpp / 8));
		memset(fb_con_splash->base, 0x00, fb_con_splash->width * fb_con_splash->height * (fb_con_splash->bpp / 8));

		if (fb_con_splash->base == NULL)
			dprintf(INFO, "%s, lcdc: framebuffer alloc failed!\n", __func__);
		else {
			dprintf(INFO, "%s, splash image: width=%d, height=%d\n", __func__, width, height);
			if(splash_image_load("logo_hdmi", fb_con_splash) < 0) {
				lcdc_set_logo(lcdc_num, width, height, fb_con);
			}
			else {
				lcdc_set_logo(lcdc_num, width, height, fb_con_splash);
			}
		}
#endif
	}
}
//#endif //#if defined(DEFAULT_DISPLAY_HDMI) 

static struct lcd_panel * lcdc_io_init(unsigned char lcdc_num) {
	struct lcd_panel *panel;
	unsigned int 	lclk;

	panel = tccfb_get_panel();
	panel->dev.power_on = GPIO_LCD_ON;
	panel->dev.display_on = GPIO_LCD_DISPLAY;
	panel->dev.bl_on = GPIO_LCD_BL;
	panel->dev.reset = GPIO_LCD_RESET;
	panel->dev.lcdc_num = lcdc_num;
	panel->init(panel);

	if(lcdc_num) {
		tcc_set_peri(PERI_LCD1, ENABLE, panel->clk_freq * panel->clk_div);
		lclk  = tcc_get_peri(PERI_LCD1);
	}
	else {
		tcc_set_peri(PERI_LCD0, ENABLE, panel->clk_freq * panel->clk_div);
		lclk  = tcc_get_peri(PERI_LCD0);
	}
	dprintf("telechips %s lcdc:%d clk:%u Hz set clk:%u Hz \n", __func__, lcdc_num, panel->clk_freq, lclk);

	panel->set_power(panel, 1);

	dprintf("%s end\n", __func__);

	return panel;
}

#include <dev/camera/camera.h>
extern struct tcc_cif_parameters * parameters_data;

#define SZ_1MB 						(1024*1024)
#define ARRAY_16MBYTE(x)          	((((x) + ((16*SZ_1MB)-1))>> 24) << 24)
#define ARRAY_MBYTE(x)            	((((x) + (SZ_1MB-1))>> 20) << 20)
#define ARRAY_256KBYTE(x)         	((((x) + ((SZ_1MB/4)-1))>> 18) << 18)

unsigned int tc_get_pmap_info(pmap_info_t *pmap)
{
	struct lcd_panel *panel_info;
	unsigned int total_size = 0;
	panel_info = tccfb_get_panel();

	pmap[PMAP_FB_WMIXER].size = \
								ARRAY_MBYTE(panel_info->xres * panel_info->yres * 2 * 2);
	pmap[PMAP_FB_WMIXER].pbase = MEMBASE - pmap[PMAP_FB_WMIXER].size;
	total_size += pmap[PMAP_FB_WMIXER].size;

	pmap[PMAP_FB_VIDEO].size = \
							   ARRAY_MBYTE(panel_info->xres * panel_info->yres * 4 * 3);
	pmap[PMAP_FB_VIDEO].pbase = pmap[PMAP_FB_WMIXER].pbase - pmap[PMAP_FB_VIDEO].size;
	total_size += pmap[PMAP_FB_VIDEO].size;

	pmap[PMAP_EARLYCAM_LOG].size = (3 * SZ_1MB);
	pmap[PMAP_EARLYCAM_LOG].pbase = pmap[PMAP_FB_VIDEO].pbase - pmap[PMAP_EARLYCAM_LOG].size;
	total_size += pmap[PMAP_EARLYCAM_LOG].size;

	pmap[PMAP_EARLYCAM_PGL].size = \
								   ARRAY_MBYTE(panel_info->xres * panel_info->yres * 4);
	pmap[PMAP_EARLYCAM_PGL].pbase = pmap[PMAP_EARLYCAM_LOG].pbase - pmap[PMAP_EARLYCAM_PGL].size;
	total_size += pmap[PMAP_EARLYCAM_PGL].size;

	pmap[PMAP_EARLYCAM_PREVIEW].size = \
									   ARRAY_MBYTE(panel_info->xres * panel_info->yres * 4 * 4);
	pmap[PMAP_EARLYCAM_PREVIEW].pbase = pmap[PMAP_EARLYCAM_PGL].pbase - pmap[PMAP_EARLYCAM_PREVIEW].size;
	total_size += pmap[PMAP_EARLYCAM_PREVIEW].size;

	//((720 * 240 * 4 *4 * 3) / 2 );
	pmap[PMAP_EARLYCAM_VIQE].size = \
									ARRAY_MBYTE(((parameters_data->Cam_preview_w * parameters_data->Cam_preview_h * 4 * 4 * 3) / 2 ));
	pmap[PMAP_EARLYCAM_VIQE].pbase = pmap[PMAP_EARLYCAM_PREVIEW].pbase - pmap[PMAP_EARLYCAM_VIQE].size;
	total_size += pmap[PMAP_EARLYCAM_VIQE].size;


#if 0
	dprintf(INFO, "xres : %d, yres : %d \n", panel_info->xres, panel_info->yres);
	dprintf(INFO, "FB_WMIXER(0x%x ~ 0x%x) size : 0x%x \n", \
			pmap[PMAP_FB_WMIXER].pbase, pmap[PMAP_FB_WMIXER].pbase + pmap[PMAP_FB_WMIXER].size, pmap[PMAP_FB_WMIXER].size);

	dprintf(INFO, "FB_VIDEO(0x%x ~ 0x%x) size : 0x%x \n", \
			pmap[PMAP_FB_VIDEO].pbase, pmap[PMAP_FB_VIDEO].pbase + pmap[PMAP_FB_VIDEO].size, pmap[PMAP_FB_VIDEO].size);

	dprintf(INFO, "EARLYCAM_LOG(0x%x ~ 0x%x) size : 0x%x \n", \
			pmap[PMAP_EARLYCAM_LOG].pbase, pmap[PMAP_EARLYCAM_LOG].pbase + pmap[PMAP_EARLYCAM_LOG].size, pmap[PMAP_EARLYCAM_LOG].size);

	dprintf(INFO, "EARLYCAM_PGL(0x%x ~ 0x%x) size : 0x%x \n", \
			pmap[PMAP_EARLYCAM_PGL].pbase, pmap[PMAP_EARLYCAM_PGL].pbase + pmap[PMAP_EARLYCAM_PGL].size, pmap[PMAP_EARLYCAM_PGL].size);

	dprintf(INFO, "EARLYCAM_PREVIEW(0x%x ~ 0x%x) size : 0x%x \n", \
			pmap[PMAP_EARLYCAM_PREVIEW].pbase, pmap[PMAP_EARLYCAM_PREVIEW].pbase + pmap[PMAP_EARLYCAM_PREVIEW].size, pmap[PMAP_EARLYCAM_PREVIEW].size);

	dprintf(INFO, "EARLYCAM_VIQE(0x%x ~ 0x%x) size : 0x%x \n", \
			pmap[PMAP_EARLYCAM_VIQE].pbase, pmap[PMAP_EARLYCAM_VIQE].pbase + pmap[PMAP_EARLYCAM_VIQE].size, pmap[PMAP_EARLYCAM_VIQE].size);
#endif

	return total_size;
}

//#if defined(DEFAULT_DISPLAY_LCD)
struct fbcon_config *lcdc_init(void) {

	unsigned LCDC_NUM;

	LCDC_NUM = 1;

	struct lcd_panel *panel_info;
	struct fbcon_config *fbcon_cfg;
	struct tcc_lcdc_image_update Image_info;

	unsigned int 	lclk;

	panel_info = tccfb_get_panel();
	panel_info->dev.power_on = GPIO_LCD_ON;
	panel_info->dev.display_on = GPIO_LCD_DISPLAY;
	panel_info->dev.bl_on = GPIO_LCD_BL;
	panel_info->dev.reset = GPIO_LCD_RESET;

	panel_info->dev.lcdc_num = LCDC_NUM;

	panel_info->init(panel_info);

	if(panel_info->dev.lcdc_num) {
		tcc_set_peri(PERI_LCD1, ENABLE, panel_info->clk_freq * panel_info->clk_div);
		lclk  = tcc_get_peri(PERI_LCD1);
	}
	else {
		tcc_set_peri(PERI_LCD0, ENABLE, panel_info->clk_freq * panel_info->clk_div);
		lclk  = tcc_get_peri(PERI_LCD0);
	}
	dprintf("telechips %s lcdc:%u Hz clk:%u Hz set clk:%d \n", __func__, LCDC_NUM, panel_info->clk_freq, lclk);
	dprintf(INFO, "lcdc: panel is %d x %d %dbpp\n",	panel_info->xres, panel_info->yres, fb_cfg.bpp);
	dprintf(INFO, "lcdc: size of telechips logo = %ld\n", sizeof(fb_cfg.base));

	tc_get_pmap_info(&gPmap);

#ifdef DISPLAY_SPLASH_SCREEN_DIRECT
	fb_cfg.stride = fb_cfg.width;

	// load boot logo copy to to end of video memory
#if 0
	memcpy(BOOT_LOGO_MEMORY, fb_cfg.base, panel_info->xres * panel_info->yres * 4);
	fb_cfg.base = BOOT_LOGO_MEMORY ;
#else
	memcpy(gPmap[PMAP_FB_WMIXER].pbase, fb_cfg.base, panel_info->xres * panel_info->yres * 4);
	fb_cfg.base = gPmap[PMAP_FB_WMIXER].pbase;
#endif

#else
	fb_cfg.width = panel_info->xres;
	fb_cfg.height = panel_info->yres;
	fb_cfg.stride = fb_cfg.width;
	fb_cfg.bpp = LCDC_FB_BPP;
	fb_cfg.format = FB_FORMAT_RGB565;

	fb_cfg.base = dma_alloc(4096, panel_info->xres * panel_info->yres * (fb_cfg.bpp / 8)); //mem size setting by lcd size
	memset(fb_cfg.base, 0x00, panel_info->xres * panel_info->yres * (fb_cfg.bpp / 8));

	//  load boot logo copy to end of video memory
#if 0
	memcpy(BOOT_LOGO_MEMORY, fb_cfg.base, panel_info->xres * panel_info->yres * 4);
	fb_cfg.base = BOOT_LOGO_MEMORY ;
#else
	memcpy(gPmap[PMAP_FB_WMIXER].pbase, fb_cfg.base, panel_info->xres * panel_info->yres * 4);
	fb_cfg.base = gPmap[PMAP_FB_WMIXER].pbase;
#endif

	if (fb_cfg.base == NULL)
		dprintf(INFO, "lcdc: framebuffer alloc failed!\n");
#endif//

	dprintf(INFO, "fb_cfg base:0x%x xres:%d yres:%d bpp:%d \n", fb_cfg.base, fb_cfg.width, fb_cfg.height, fb_cfg.bpp);

	Image_info.addr0 = (unsigned int)fb_cfg.base;
	Image_info.Lcdc_layer = 0;

#if defined(DISPLAY_SPLASH_SCREEN_DIRECT)
	Image_info.enable = 1;
#else
	Image_info.enable = 0;
#endif

	Image_info.Frame_width = Image_info.Image_width = fb_cfg.width;
	Image_info.Frame_height = Image_info.Image_height = fb_cfg.height;

	dprintf(INFO, "Frame_width:%d Image_width:%d width:%d \n", Image_info.Frame_width, Image_info.Image_width, fb_cfg.width);
	dprintf(INFO, "Frame_height:%d Image_height:%d width:%d \n", Image_info.Frame_height, Image_info.Image_height, fb_cfg.height);

	if(Image_info.Image_width > panel_info->xres)
		Image_info.Image_width = panel_info->xres;

	if(panel_info->xres > Image_info.Frame_width)
		Image_info.offset_x = (panel_info->xres - Image_info.Frame_width) / 2;
	else
		Image_info.offset_x =  0;

	if(panel_info->yres > Image_info.Frame_height)
		Image_info.offset_y = (panel_info->yres - Image_info.Frame_height) / 2;
	else
		Image_info.offset_y = 0;

	Image_info.fmt = TCC_LCDC_IMG_FMT_RGB888;
	tcclcd_image_ch_set(panel_info->dev.lcdc_num, &Image_info);

	panel_info->set_power(panel_info, 1);

	mdelay(1);
	panel_info->set_backlight_level(panel_info, DEFAULT_BACKLIGTH);

	fbcon_cfg = &fb_cfg;

	return fbcon_cfg;
}
//#else

struct fbcon_config *lcdc_init_hdmi(void) {
	struct fbcon_config *fbcon_cfg;
	unsigned char lcdc_1st = 0; //LCD_CONTROLLER_0
	unsigned char lcdc_2nd = 1; //LCD_CONTROLLER_1
	unsigned int config_val;

	tc_get_pmap_info(&gPmap);

//#if defined(DEFAULT_DISPLAY_HDMI)
	lcdc_io_init_hdmi(lcdc_1st);
//#endif

	fbcon_cfg = &fb_cfg;

	mdelay(50);

	return fbcon_cfg;
}
//#endif


int display_splash_logo(struct fbcon_config *fb_con)
{

#if !defined(DISPLAY_SPLASH_SCREEN_DIRECT)
	struct lcd_panel *panel_info;
	panel_info = tccfb_get_panel();
	lcdc_set_logo(panel_info->dev.lcdc_num, panel_info->xres, panel_info->yres, fb_con);
#endif
	return 0;
}

