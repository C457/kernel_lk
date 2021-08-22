#include <config.h>
#include <common.h>
#include <lcd.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/globals.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/clock.h>
#include <asm/arch/tcc_lcd.h>

/*
 * board_init_f(arch/arm/lib/board.c) calls lcd_setmem() which needs
 * panel_info.vl_col, panel_info.vl_row and panel_info.vl_bpix to reserve
 * FB memory at a very early stage, i.e even before exynos_fimd_parse_dt()
 * is called. So, we are forced to statically assign it.
 */
 
DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_LCD_AT070TN93

static int at070tn93_panel_init(void);
static int at070tn93_set_power(int on);
static int at070tn93_set_backlight_level(int level);

vidinfo_t panel_info = {
	.name		= "AT070TN93",
	.manufacturer	= "INNOLUX",
	.id		= PANEL_ID_AT070TN93,
	.vl_col		= 800,
	.vl_row		= 480,
	.vl_bpix	= 4,		// 16
	.clk_freq	= 310000,
	.clk_div	= 2,
	.bus_width	= 24,
	.lpw		= 10,
	.lpc		= 800,
	.lswc		= 34,
	.lewc		= 130,
	.vdb		= 0,
	.vdf		= 0,
	.fpw1		= 1,
	.flc1		= 480,
	.fswc1		= 20,
	.fewc1		= 21,
	.fpw2		= 1,
	.flc2		= 480,
	.fswc2		= 20,
	.fewc2		= 21,
	.sync_invert	= IV_INVERT | IH_INVERT,

	.init		= at070tn93_panel_init,
	.set_power	= at070tn93_set_power,
	.set_backlight_level = at070tn93_set_backlight_level,
};


static int at070tn93_panel_init(void)
{
	struct lcd_platform_data *pdata = &(panel_info.dev);
	
	debug("%s : lcdc_id: %d  \n", __func__, panel_info.dev.lcdc_id);

	gpio_config(pdata->display_on, GPIO_OUTPUT);
	gpio_config(pdata->bl_on, GPIO_OUTPUT);
	gpio_config(pdata->reset, GPIO_OUTPUT);
	gpio_config(pdata->power_on, GPIO_OUTPUT);

	gpio_set_value(pdata->display_on, 0);
	gpio_set_value(pdata->bl_on, 0);
	gpio_set_value(pdata->reset, 0);
	gpio_set_value(pdata->power_on, 0);

	return 0;
}

static int at070tn93_set_power(int on)
{
	struct lcd_platform_data *pdata = &(panel_info.dev);

	debug("%s : %d ~ \n", __func__, on);


	if (on) {
		gpio_set_value(pdata->reset, 1);
		udelay(1000);
		gpio_set_value(pdata->power_on, 1);
		udelay(1000);
		gpio_set_value(pdata->reset, 0);
		udelay(1000);
		gpio_set_value(pdata->reset, 1);
		
		gpio_set_value(pdata->display_on, 1);
		mdelay(10);

		lcdc_initialize(&panel_info);
                #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
                if(pdata->lcdc_swap) {
                        if(pdata->lcdc_id == 0) {
		                LCDC_IO_Set(1, panel_info.bus_width);
                        } else {
                                LCDC_IO_Set(0, panel_info.bus_width);
                        }
                }
                else {
                #endif
                        LCDC_IO_Set(pdata->lcdc_id, panel_info.bus_width);
                #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
                }
                #endif
		mdelay(16);
	}
	else 
	{
		gpio_set_value(pdata->display_on, 0);
		mdelay(10);
		gpio_set_value(pdata->reset, 0);
		gpio_set_value(pdata->power_on, 0);
                #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
                if(pdata->lcdc_swap) {
                        if(pdata->lcdc_id == 0) {
                                LCDC_IO_Disable(1, panel_info.bus_width);
                        } else {
                                LCDC_IO_Disable(0, panel_info.bus_width);
                        }
                }
                else {
                #endif
                        LCDC_IO_Disable(pdata->lcdc_id, panel_info.bus_width);
                #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
                }
                #endif
	}
	return 0;
}


static int at070tn93_set_backlight_level(int level)
{
	struct lcd_platform_data *pdata = &(panel_info.dev);
	
	debug("%s : %d\n", __func__, level);

	if (level == 0) 
		gpio_set_value(pdata->bl_on, 0);
	else
		gpio_set_value(pdata->bl_on, 1);

#if 0
	struct lcd_platform_data *pdata = &(panel_info.dev);
	
	debug("%s : %d\n", __func__, level);

	if (level == 0) {
		gpio_set_value(pdata->bl_on, 0);
	} else {
		#ifdef TCC892X
		if(HW_REV==0x1005 || HW_REV==0x1006 || HW_REV==0x1007|| HW_REV==0x1008 || HW_REV==0x2002 || HW_REV==0x2003 || HW_REV==0x2004 || HW_REV==0x2005 || HW_REV==0x2006 || HW_REV==0x2007 || HW_REV==0x2008 || HW_REV==0x2009)
			tca_tco_pwm_ctrl(0, pdata->bl_on, MAX_BACKLIGTH, level);
		else
			tca_tco_pwm_ctrl(1, pdata->bl_on, MAX_BACKLIGTH, level);
		#elif defined(TCC88XX) || defined(CONFIG_TCC893X_EVM)
			tca_tco_pwm_ctrl(0, pdata->bl_on, MAX_BACKLIGTH, level);
		#else
			printf("%s :  Do not support chipset !!!\n",__func__);
		#endif//
	}
#endif

	return 0;
}
#endif	//CONFIG_LCD_AT070TN93


#ifdef CONFIG_LCD_FLD0800
#define LVDS_VCO_45MHz 450000
#define LVDS_VCO_60MHz 600000
unsigned int lvds_stbyb;

static int fld0800_panel_init(void)
{
	struct lcd_platform_data *pdata = &(panel_info.dev);
        #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
        if(pdata->lcdc_swap) {
                if(!pdata->lcdc_id)
		        lvds_stbyb = (GPIO_PORTE |27);
	        else
		        lvds_stbyb = (GPIO_PORTB |19);
        }
        else {
        #endif
                if(pdata->lcdc_id == 1)
                        lvds_stbyb = (GPIO_PORTE |27);
                else
                        lvds_stbyb = (GPIO_PORTB |19);
        #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
        }
        #endif

	gpio_config(pdata->display_on, GPIO_OUTPUT);
	gpio_config(pdata->bl_on, GPIO_OUTPUT);
	gpio_config(pdata->reset, GPIO_OUTPUT);
	gpio_config(pdata->power_on, GPIO_OUTPUT);
	gpio_config(lvds_stbyb, GPIO_OUTPUT);

	gpio_set_value(pdata->display_on, 0);
	gpio_set_value(pdata->bl_on, 0);
	gpio_set_value(pdata->reset, 0);
	gpio_set_value(pdata->power_on, 0);
	gpio_config(lvds_stbyb, 0);

	debug("%s(lcdc%d)\n", __func__, pdata->lcdc_id);
	return 0;
}

static int fld0800_set_power(int on)
{
	struct lcd_platform_data *pdata = &(panel_info.dev);
	PDDICONFIG pDDICfg = (DDICONFIG *)HwDDI_CONFIG_BASE;
	unsigned int P, M, S, VSEL, TC;

	if (on) {
		gpio_set_value(pdata->power_on, 1);
		udelay(20);

		gpio_set_value(lvds_stbyb, 1);
		gpio_set_value(pdata->reset, 1);
		gpio_set_value(pdata->display_on, 1);

		lcdc_initialize(&panel_info);

		/* LVDS reset */
		pDDICfg->LVDS_CTRL.bREG.RST =1;
		pDDICfg->LVDS_CTRL.bREG.RST =0;

		BITCSET(pDDICfg->LVDS_TXO_SEL0.nREG, 0xFFFFFFFF, 0x13121110);
		BITCSET(pDDICfg->LVDS_TXO_SEL1.nREG, 0xFFFFFFFF, 0x09081514);
		BITCSET(pDDICfg->LVDS_TXO_SEL2.nREG, 0xFFFFFFFF, 0x0D0C0B0A);
		BITCSET(pDDICfg->LVDS_TXO_SEL3.nREG, 0xFFFFFFFF, 0x03020100);
		BITCSET(pDDICfg->LVDS_TXO_SEL4.nREG, 0xFFFFFFFF, 0x1A190504);
		BITCSET(pDDICfg->LVDS_TXO_SEL5.nREG, 0xFFFFFFFF, 0x0E171618);
		BITCSET(pDDICfg->LVDS_TXO_SEL6.nREG, 0xFFFFFFFF, 0x1F07060F);
		BITCSET(pDDICfg->LVDS_TXO_SEL7.nREG, 0xFFFFFFFF, 0x1F1E1F1E);
		BITCSET(pDDICfg->LVDS_TXO_SEL8.nREG, 0xFFFFFFFF, 0x001E1F1E);

		/* LVDS P,M,S,VSEL select */
#if defined(CONFIG_TCC893X)
		if (panel_info.clk_freq >= LVDS_VCO_45MHz && panel_info.clk_freq < LVDS_VCO_60MHz) {
			M = 7; P = 7; S = 0; VSEL = 0; TC = 4;
		} else {
			M = 10; P = 10; S = 0; VSEL = 0; TC = 4;
		}
#else
		M = 10; P = 10; S = 0; VSEL = 0; TC = 4;
#endif
		BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x00FFFFF0, (VSEL<<4)|(S<<5)|(M<<8)|(P<<15)|(TC<<21));	// LCDC1

		/* LVDS Select LCDC 1 */
                #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
                if(pdata->lcdc_swap) {
                        if (!pdata->lcdc_id)
                                BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x3 << 30 , 0x1 << 30);
            		else
                                BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x3 << 30 , 0x0 << 30);
                }
                else {
                #endif
                        if (pdata->lcdc_id ==1)
                                BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x3 << 30 , 0x1 << 30);
                        else
                                BITCSET(pDDICfg->LVDS_CTRL.nREG, 0x3 << 30 , 0x0 << 30);
                #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
                }
                #endif
        		
                pDDICfg->LVDS_CTRL.bREG.RST = 1;	// reset

                /* LVDS enable */
                pDDICfg->LVDS_CTRL.bREG.EN = 1;
        } else {
                gpio_set_value(pdata->power_on, 0);
	}

	debug("%s(%d)\n", __func__, on);
	return 0;
}

static int fld0800_set_backlight_level(int level)
{
	struct lcd_platform_data *pdata = &(panel_info.dev);

	if (level == 0)
		gpio_set_value(pdata->bl_on, 0);
	else
		gpio_set_value(pdata->bl_on, 1);

	debug("%s(%d)\n", __func__, level);
	return 0;
}

vidinfo_t panel_info = {
	.name				= "FLD0800",
	.manufacturer		= "innolux",
	.id					= PANEL_ID_FLD0800,
	.vl_col				= 1024,
	.vl_row				= 600,
	.vl_bpix			= 4,		// bpp = (1 << vl_bpix)
	.clk_freq			= 512000,
	.clk_div			= 2,
	.bus_width			= 24,

	.lpw				= 20,
	.lpc				= 1024,
	.lswc				= 150,
	.lewc				= 150,
	.vdb				= 0,
	.vdf				= 0,

	.fpw1				= 2,
	.flc1				= 600,
	.fswc1				= 10,
	.fewc1				= 25,
	.fpw2				= 2,
	.flc2				= 600,
	.fswc2				= 10,
	.fewc2				= 25,
	.sync_invert		= IV_INVERT | IH_INVERT,

	.init				= fld0800_panel_init,
	.set_power			= fld0800_set_power,
	.set_backlight_level= fld0800_set_backlight_level,
};
#endif	//CONFIG_LCD_FLD0800

static void lcd_panel_on(vidinfo_t *vid)
{
	unsigned int 	lclk;
	struct tcc_lcdc_image_update ImageInfo;
		
	if(panel_info.dev.lcdc_id)
	{
		tca_ckc_setperi(PERI_LCD1,ENABLE, panel_info.clk_freq * panel_info.clk_div);
		lclk  = tca_ckc_getperi(PERI_LCD1);
	}
	else
	{
		tca_ckc_setperi(PERI_LCD0,ENABLE, panel_info.clk_freq * panel_info.clk_div);
		lclk  = tca_ckc_getperi(PERI_LCD0);
	}
	
	debug("telechips tcc893x %s lcdc:%d clk:%d set clk:%d \n", __func__, panel_info.dev.lcdc_id, (int)panel_info.clk_freq, lclk);

	memset(&ImageInfo, 0, sizeof(ImageInfo));

	ImageInfo.addr0 = (unsigned int) panel_info.lcdbase;
	ImageInfo.Lcdc_layer = 0;
	ImageInfo.enable = 1;

	ImageInfo.Frame_width = ImageInfo.Image_width = panel_info.vl_col;
	ImageInfo.Frame_height = ImageInfo.Image_height = panel_info.vl_row;

	debug("Frame_width:%d Image_width:%d \n",ImageInfo.Frame_width, ImageInfo.Image_width);
	debug("Frame_height:%d Image_height:%d \n",ImageInfo.Frame_height, ImageInfo.Image_height);

	#if 0
	if(ImageInfo.Image_width > lcd_wd)
		ImageInfo.Image_width = lcd_wd;

	if(lcd_wd > ImageInfo.Frame_width)
		ImageInfo.offset_x = (lcd_wd - ImageInfo.Frame_width)/2;
	else
		ImageInfo.offset_x =  0;

	if(lcd_ht > ImageInfo.Frame_height)
		ImageInfo.offset_y = (lcd_ht - ImageInfo.Frame_height)/2;
	else
		ImageInfo.offset_y = 0;
	#endif	
	
	#ifdef _LCD_32BPP_
	ImageInfo.fmt = TCC_LCDC_IMG_FMT_RGB888;
	#else
	ImageInfo.fmt = TCC_LCDC_IMG_FMT_RGB565;
	#endif
	
	tcclcd_image_ch_set(panel_info.dev.lcdc_id, &ImageInfo);

   	panel_info.set_power(1);

	udelay(1000);
	panel_info.set_backlight_level(DEFAULT_BACKLIGTH);

}

void lcd_ctrl_init(void *lcdbase)
{
	debug("lcd_ctrl_init 0x%x\r\n", (unsigned int)lcdbase);

	init_panel_info(&panel_info);

	debug("panel_info.clk_freq = %d\r\n", (int)panel_info.clk_freq);
	
	panel_info.lcdbase = (unsigned int)lcdbase;
	panel_info.init();
}

void lcd_enable(void)
{
	#ifdef CONFIG_SPLASH_SCREEN_ALIGN
	#define BMP_ALIGN_CENTER	0x7FFF
	bmp_display(panel_info.dev.logo_data, BMP_ALIGN_CENTER, BMP_ALIGN_CENTER);
	#else
	bmp_display(panel_info.dev.logo_data, 0, 0);
	#endif

	lcd_panel_on(&panel_info);
}

/* dummy function */
void lcd_setcolreg(ushort regno, ushort red, ushort green, ushort blue)
{
	return;
}

void get_panel_info(vidinfo_t *vid)
{
	if(vid) {
		memcpy(vid, &panel_info, sizeof(panel_info));
	}
}

int tcc_get_panel_id(void)
{
	return panel_info.id;
}
