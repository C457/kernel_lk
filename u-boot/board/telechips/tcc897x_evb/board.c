#include <common.h>
#include <asm/arch/iomap.h>
#include <asm/arch/usb/reg_physical.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/uart.h>
#include <asm/arch/i2c.h>

#include <lcd.h>
#include <pca950x.h>

DECLARE_GLOBAL_DATA_PTR;


extern void clock_init(void);
extern 	int tcc_sdhci_init(u32 regbase, int index, int bus_width);
extern 	void tcc_nand_init(void);

//target/tcc897x-xxx/gpio.c
/* Please keep the order: GPA, GPB, ,,, GPG, GPHDMI, GPSD3, GPSD0 */
struct gpio_cfg init_tbl[] = {
	{ TCC_GPA(5)	,GPIO_FN0 , GPIO_OUTPUT|GPIO_HIGH,	GPIO_PULLDISABLE },
	{ TCC_GPC(24)	,GPIO_FN0 , GPIO_OUTPUT|GPIO_HIGH,	GPIO_PULLDISABLE },	// PWR_SHDN
	{ TCC_GPG(19)	,GPIO_FN0 , GPIO_INPUT           ,	GPIO_PULLDISABLE },	// RST#_IRQ to prevent NAND crash

	{ TCC_GPF(22)	,GPIO_FN9 , GPIO_OUTPUT|GPIO_HIGH,	GPIO_PULLDISABLE },	// tx
	{ TCC_GPF(23)	,GPIO_FN9 , GPIO_INPUT,		 	  	GPIO_PULLDISABLE }, // rx

	{ TCC_GPE(13)	,GPIO_FN8 , GPIO_INPUT,				GPIO_PULLDISABLE },	// tx
	{ TCC_GPE(14)	,GPIO_FN8 , GPIO_INPUT, 			GPIO_PULLDISABLE }, // rx

	{ TCC_GPHDMI(0) ,GPIO_FN0 , GPIO_INPUT, 			GPIO_PULLDISABLE },  //CEC
	{ TCC_GPHDMI(1) ,GPIO_FN0 , GPIO_INPUT, 			GPIO_PULLDISABLE },  //HPD
	{ TCC_GPHDMI(2) ,GPIO_FN0 , GPIO_INPUT, 			GPIO_PULLDISABLE },  //I2C_SDA
	{ TCC_GPHDMI(3) ,GPIO_FN0 , GPIO_INPUT, 			GPIO_PULLDISABLE },  //I2C_SCL
};

struct uart_port_map_type uart_port_map[] = {
	/*   txd          rxd          cts          rts          fn_sel */
	/*   txd          rxd          cts          rts          fn_sel */
	{0 , TCC_GPA(7),  TCC_GPA(8),  TCC_GPA(10), TCC_GPA(9),  GPIO_FN(7)  },	// UT_TXD(0)
	{1 , TCC_GPA(13), TCC_GPA(14), TCC_GPA(16), TCC_GPA(15), GPIO_FN(7)  },	// UT_TXD(1)
	{2 , TCC_GPB(0) , TCC_GPB(1) , TCC_GPB(3) , TCC_GPB(2) , GPIO_FN(10) },	// UT_TXD(2)
	{3 , TCC_GPB(11), TCC_GPB(12), TCC_GPB(14), TCC_GPB(13), GPIO_FN(10) }, // UT_TXD(3)
	{4 , TCC_GPB(19), TCC_GPB(20), TCC_GPB(22), TCC_GPB(21), GPIO_FN(10) }, // UT_TXD(4)
	{5 , TCC_GPB(25), TCC_GPB(26), TCC_GPB(28), TCC_GPB(27), GPIO_FN(10) }, // UT_TXD(5)
	{6 , TCC_GPC(0),  TCC_GPC(1),  TCC_GPC(3),  TCC_GPC(2),  GPIO_FN(6)  },	// UT_TXD(6)
	{7 , TCC_GPC(10), TCC_GPC(11), TCC_GPC(13), TCC_GPC(12), GPIO_FN(6)  },	// UT_TXD(7)
	{8 , TCC_GPC(23), TCC_GPC(24), TCC_GPC(26), TCC_GPC(25), GPIO_FN(6)  },	// UT_TXD(8)
	{9 , TCC_GPD(4),  TCC_GPD(5),  TCC_GPD(7),  TCC_GPD(6),  GPIO_FN(7)  },	// UT_TXD(9)
	{10, TCC_GPD(11), TCC_GPD(12), TCC_GPD(14), TCC_GPD(13), GPIO_FN(7)  }, // UT_TXD(10)
	{11, TCC_GPD(17), TCC_GPD(18), TCC_GPD(20), TCC_GPD(19), GPIO_FN(7)  }, // UT_TXD(11)
	{12, TCC_GPE(4),  TCC_GPE(5),  TCC_GPE(7),  TCC_GPE(6),  GPIO_FN(7)  }, // UT_TXD(12)
	{13, TCC_GPE(11), TCC_GPE(12), TCC_GPE(14), TCC_GPE(13), GPIO_FN(7)  },	// UT_TXD(13)
	{14, TCC_GPE(20), TCC_GPE(21), TCC_GPE(23), TCC_GPE(22), GPIO_FN(7)  },	// UT_TXD(14)
	{15, TCC_GPE(27), TCC_GPE(28), TCC_GPE(30), TCC_GPE(29), GPIO_FN(7)  },	// UT_TXD(15)
	{16, TCC_GPF(0),  TCC_GPF(1),  TCC_GPF(3),  TCC_GPF(2),  GPIO_FN(9)  },	// UT_TXD(16)
	{17, TCC_GPF(2),  TCC_GPF(3),  TCC_GPF(1),  TCC_GPF(0),  GPIO_FN(10) }, // UT_TXD(17)
	{18, TCC_GPF(4),  TCC_GPF(5),  TCC_GPF(7),  TCC_GPF(6),  GPIO_FN(9)  },	// UT_TXD(18)
	{19, TCC_GPF(6),  TCC_GPF(7),  TCC_GPF(9),  TCC_GPF(8),  GPIO_FN(10) }, // UT_TXD(19)
	{20, TCC_GPF(13), TCC_GPF(14), TCC_GPF(12), TCC_GPF(11), GPIO_FN(9)  },	// UT_TXD(20)
	{21, TCC_GPF(17), TCC_GPF(18), TCC_GPF(20), TCC_GPF(19), GPIO_FN(9)  },	// UT_TXD(21)
	{22, TCC_GPF(22), TCC_GPF(23), TCC_GPF(25), TCC_GPF(24), GPIO_FN(9)  },	// UT_TXD(22)
	{23, TCC_GPF(26), TCC_GPF(27), TCC_GPF(29), TCC_GPF(28), GPIO_FN(9)  },	// UT_TXD(23)
	{24, TCC_GPG(0),  TCC_GPG(1),  TCC_GPG(3),  TCC_GPG(2),  GPIO_FN(3)  }, // UT_TXD(24)
	{25, TCC_GPG(4),  TCC_GPG(5),  TCC_GPG(7),  TCC_GPG(6),  GPIO_FN(3)  }, // UT_TXD(25)
	{26, TCC_GPG(8),  TCC_GPG(9),  TCC_GPG(11), TCC_GPG(10), GPIO_FN(3)  }, // UT_TXD(26)
	{27, TCC_GPG(15), TCC_GPG(14), TCC_GPG(12), TCC_GPG(13), GPIO_FN(3)  }, // UT_TXD(27)
	{28, TCC_GPG(19), TCC_GPG(18), TCC_GPG(16), TCC_GPG(17), GPIO_FN(3)  }, // UT_TXD(28)
};

unsigned int NUM_UART_PORT = ARRAY_SIZE(uart_port_map);

int power_init_board(void)
{

#ifdef CONFIG_PCA950X
	/*
	GPIO EXPANDER U36
	BANK0					BANK1		BANK2		BANK3				BANK4
	0 - USB20H0VBUS_FAULT	0 - 		0 - 		0 - CODEC_GPIO0 	0 - V_5P0_EN
	1 - OTGVBUS_FAULT		1 - 		1 - 		1 - CODEC_GPIO1 	1 - OTG_EN
	2 - COMPASS_IRQ 		2 - 		2 - 		2 - CODEC_GPIO2 	2 - U20H0_EN
	3 - SDWF_WP 			3 - 		3 - 		3 - CODEC_GPIO3 	3 - HDMI_EN
	4 - SD2_WP				4 - 		4 - 		4 - CODEC_GPIO4 	4 - LVDS_EN
	5 - SD2_CD				5 - 		5 - 		5 - CODEC_GPIO5 	5 - PWR_CTL0
	6 - SMART_DET			6 - 		6 - 		6 - x				6 - PWR_CTL1
	7 - x					7 - 		7 - 		7 - CAS_PWR_SEL 	7 - x
	*/
	pca950x_init(GPIO_PORTEXT1, I2C_CH_MASTER0, 0, 0, 0xFF, 0xFF); // bank0 // direction IO0-0 ~ IO0-7
	//pca950x_init(GPIO_PORTEXT1, I2C_CH_MASTER0, 0, 1, 0xFF, 0x00); // bank1 // direction IO1-0 ~ IO1-7
	//pca950x_init(GPIO_PORTEXT1, I2C_CH_MASTER0, 0, 2, 0xFF, 0x00); // bank2 // direction IO2-0 ~ IO2-7
	pca950x_init(GPIO_PORTEXT1, I2C_CH_MASTER0, 0, 3, 0x00, 0xFF); // bank3 // direction IO3-0 ~ IO3-7
	pca950x_init(GPIO_PORTEXT1, I2C_CH_MASTER0, 0, 4, 0x00, 0xFB); // bank4 // direction IO4-0 ~ IO4-7

	/*
	GPIO EXPANDER U37
	BANK0					BANK1			BANK2				BANK3				BANK4
	0 - TVOUT_ON			0 - GPS_PWREN	0 - CAM0_ON 		0 - CODEC_ON		0 - x
	1 - LCD_ON				1 - GPS_PWDN	1 - P-CAM0_PWR_ON	1 - EXT_CODEC_ON	1 - x
	2 - LCD_DISP			2 - BT_ON		2 - CAM4_ON 		2 - MUTE_CTL		2 - x
	3 - TV_SLEEP#			3 - BT_RST# 	3 - CAM4_PWDN		3 - DXB_ON			3 - OTGVBUS_EN
	4 - CAS_ON				4 - BT_WAKE 	4 - CAM4_RST#		4 - DXB0_RST#		4 - USB20H0VBUS_EN
	5 - CAS_GP				5 - SDWF_RST#	5 - CAM4_STDBY		5 - DXB1_RST#		5 - EDI_RST#
	6 - CAS_RST#			6 - IPOD_ON 	6 - P-CAM4_PWR_ON	6 - DXB0_PD 		6 - EDI_ON
	7 - SD2_ON				7 - AUTH_RST#	7 - COMPASS_RST 	7 - DXB1_PD 		7 - x
	*/
	pca950x_init(GPIO_PORTEXT2, I2C_CH_MASTER0, 1, 0, 0x00, 0xFF); // bank0 // direction IO0-0 ~ IO0-7
	pca950x_init(GPIO_PORTEXT2, I2C_CH_MASTER0, 1, 1, 0x00, 0xFF); // bank1 // direction IO1-0 ~ IO0-7
	pca950x_init(GPIO_PORTEXT2, I2C_CH_MASTER0, 1, 2, 0x00, 0xFF); // bank2 // direction IO2-0 ~ IO0-7
	pca950x_init(GPIO_PORTEXT2, I2C_CH_MASTER0, 1, 3, 0x00, 0xFF); // bank3 // direction IO3-0 ~ IO0-7
	pca950x_init(GPIO_PORTEXT2, I2C_CH_MASTER0, 1, 4, 0x00, 0x68); // bank4 // direction IO4-0 ~ IO0-7
#endif

    clock_init();

	return 0;
}

#ifdef GPIO_INIT_PORTCFG_USE
static int tlb_idx = 0;
void gpio_set_init_port(unsigned GPIO, int max_size)
{
	int i;
	for (i=0 ; i<max_size ; i++) {
		if (((GPIO+i) == init_tbl[tlb_idx].port) && (tlb_idx < (sizeof(init_tbl)/sizeof(init_tbl[0])))) {
			gpio_config(init_tbl[tlb_idx].port,
				init_tbl[tlb_idx].function|init_tbl[tlb_idx].direction|init_tbl[tlb_idx].pull);
			tlb_idx++;
		}
		else
		{
			/* set default status */
			gpio_config(GPIO+i, GPIO_FN(0)|GPIO_OUTPUT|GPIO_LOW|GPIO_PULLDISABLE);
		}
	}
}
#endif

void gpio_init_early(void)
{
#ifdef GPIO_INIT_PORTCFG_USE
	gpio_set_init_port(GPIO_PORTA, 16);
	gpio_set_init_port(GPIO_PORTB, 30);
	gpio_set_init_port(GPIO_PORTC, 30);
	gpio_set_init_port(GPIO_PORTD, 22);
	gpio_set_init_port(GPIO_PORTE, 32);
	gpio_set_init_port(GPIO_PORTF, 32);
	gpio_set_init_port(GPIO_PORTG, 20);
#else
	int i;

	for (i=0 ; i<(sizeof(init_tbl)/sizeof(init_tbl[0])) ; i++)
		gpio_config(init_tbl[i].port, init_tbl[i].function|init_tbl[i].direction|init_tbl[i].pull);

#endif
}

#ifdef CONFIG_LCD
vidinfo_t panel_info = {
    .vl_col =       800,
    .vl_row =       480,
};

const unsigned char logo_data[] =
{
#include "logo_480x272.c"
};

unsigned int get_board_fb_base(void)
{
#if 0		/* 016.05.29 */
	vidinfo_t vid;
	get_panel_info(&vid);

	return FB_SCALE0_BASE - (vid.vl_col * vid.vl_row * NBITS(vid.vl_bpix) / 8);;
#endif /* 0 */
}

void init_panel_info(vidinfo_t *vid)
{
#if 0		/* 016.05.29 */
	vid->dev.power_on = GPIO_LCD_ON;
	vid->dev.display_on = GPIO_LCD_DISPLAY;
	vid->dev.bl_on = GPIO_LCD_BL;
	vid->dev.reset = GPIO_LCD_RESET;
        #if defined(CONFIG_TCC_VIOCMG_SUPPORT)
	vid->dev.lcdc_id = CONFIG_MAIN_DISPLAY_ID;
        vid->dev.lcdc_swap = CONFIG_MAIN_DISPLAY_SWAP;
        #else
        vid->dev.lcdc_id = LCDC_NUM;
        #endif
	vid->dev.logo_data = (unsigned)logo_data;
#endif /* 0 */
}
#endif

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_TCC897X;

	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:\t%s\n", BOARD_NAME);
	return 0;
}
#endif

int board_mmc_init(bd_t *bd)
{
	int ret = 0;

	tcc_ckc_reset_ops();

	ret = tcc_sdhci_init(HwSDMMC3_BASE, 3, 8);
	//ret = tcc_sdhci_init(HwSDMMC2_BASE, 2, 4);

	return ret;
}

int get_board_rev(void)
{
	return CONFIG_TCC_BOARD_REV;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((long*)CONFIG_SYS_SDRAM_BASE, PHYS_DDR_SIZE*1024u*1024u );
	return 0;
}

