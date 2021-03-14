#include <daudio_ver.h>
#include <stdlib.h>
#include <debug.h>

#if defined(CONFIG_DAUDIO_PRINT_DEBUG_MSG)
#define DEBUG_VER   1
#else
#define DEBUG_VER	0
#endif

#if (DEBUG_VER)
#define LKPRINT(fmt, args...) dprintf(INFO, "[cleinsoft board version] " fmt, ##args)
#else
#define LKPRINT(args...) do {} while(0)
#endif


#define VIN         1800         //Input Voltage : 1.8V
#define DIV         0x0FFF       //ADC Maximum DATA

#define ADCCONV(X)  (X + 1) * VIN / DIV

typedef struct ADC_Range
{
	unsigned char ver;
	unsigned long min;
	unsigned long max;
} ADC_Range_t;

static ADC_Range_t hw_ranges[] =
{
	{DAUDIOKK_HW_1ST, 0, 199},		//HW Ver.01 - 0.15V
	{DAUDIOKK_HW_2ND, 200, 399}, 		//HW Ver.02 - 0.32V
	{DAUDIOKK_HW_3RD, 400, 599},   		//HW Ver.03 - 0.51V
	{DAUDIOKK_HW_4TH, 600, 799},	   	//HW Ver.04 - 0.73V
	{DAUDIOKK_HW_5TH, 800, 999},    	//HW Ver.05 - 0.9V
	{DAUDIOKK_HW_6TH, 1000, 1199},   	//HW Ver.06 - 1.11V
	{DAUDIOKK_HW_7TH, 1200, 1399},   	//HW Ver.07 - 1.31V
	{DAUDIOKK_HW_8TH, 1400, 1599},   	//HW Ver.08 - 1.48V
	{DAUDIOKK_HW_9TH, 1600, 1800},    	//HW Ver.09 - 1.69V
};


static ADC_Range_t main_ranges[] =
{
	{DAUDIOKK_PLATFORM_WS4, 0, 199},           //Platform Ver.01 - 0.0V
	{DAUDIOKK_PLATFORM_WS5, 200, 399},       //Platform Ver.02 - 0.32V
	{DAUDIOKK_PLATFORM_WS3, 400, 599},       //Platform Ver.03 - 0.51V
	{DAUDIOKK_PLATFORM_WS6, 600, 799},      //Platform Ver.04 - 0.73V
	{DAUDIOKK_PLATFORM_WS7, 800, 999},     //Platform Ver.05 -  0.90V
	{DAUDIOKK_PLATFORM_WS8, 1000, 1199},     //Platform Ver.06 - 1.11V
	{DAUDIOKK_PLATFORM_WS9, 1200, 1399},     //Platform Ver.07 - 1.31V
	{DAUDIOKK_PLATFORM_WS10, 1400, 1599},     //Platform Ver.08 - 1.48V
	{DAUDIOKK_PLATFORM_WS11, 1600, 1800},     //Platform Ver.09 - 1.69V
};

/*
static ADC_Range_t bt_ranges[] =
{
#if defined(INCLUDE_DAUDIO_ECO)
	{DAUDIOECO_BT_2MODULE_0, 0, 149},    // BT only + BT/WIFI Combo(BCM89355) - 0V ~ 0.149V
	{DAUDIOECO_BT_2MODULE_1, 150, 449},  // BT only + BT/WIFI Combo(BCM89335) - 0.15V ~ 0.449V
	{DAUDIOECO_BT_1MODULE_0, 450, 749},  // BT/WIFI Combo(BCM89335), monitor disassembled - 0.45V ~ 0.749V
	{DAUDIOECO_BT_1MODULE_1, 750, 1049}, // BT/WIFI Combo(BCM89335), monitor disassembled - 0.75 ~ 1.049V
	{DAUDIOECO_BT_1MODULE_2, 1050, 1349},//BT/WIFI Combo(BCM89335), monitor disassembled - 1.05 ~ 1.349V
	{DAUDIOECO_BT_1MODULE_3, 1350, 1649},//BT/WIFI Combo(BCM89335), monitor disassembled - 1.35 ~ 1.649V
	{DAUDIOECO_BT_2MODULE_2, 1650, 1949},// BT only + BT/WIFI Combo(BCM89335) - 1.65V ~ 1.949V
	{DAUDIOECO_BT_2MODULE_3, 1950, 2249},// BT only + BT/WIFI Combo(BCM89335) - 1.95V ~ 2.249V
	{DAUDIOECO_BT_RESERVED1, 2250, 2549}, // BT reserved1 - 2.25V ~ 2.549V
	{DAUDIOECO_BT_RESERVED2, 2550, 2849}, // BT reserved2 - 2.55V ~ 2.849V
	{DAUDIOECO_BT_ONLY_CK, 2850, 3149},  // BT only, carkit - 2.85V ~ 3.149V
	{DAUDIOECO_BT_ONLY_SMD, 3150, 3300}, // BT only, SMD - 3.15V ~ 3.3V
#else
	{DAUDIO_BT_2MODULE_0, 0, 149},    // BT only + BT/WIFI Combo(BCM89355) - 0V ~ 0.149V
	{DAUDIO_BT_2MODULE_1, 150, 449},  // BT only + BT/WIFI Combo(BCM89335) - 0.15V ~ 0.449V
	{DAUDIO_BT_1MODULE_0, 450, 749},  // BT/WIFI Combo(BCM89335), mornitor disassembled - 0.45V ~ 1.649V
	{DAUDIO_BT_1MODULE_1, 750, 1049}, // BT/WIFI Combo(BCM89335) - 0.75 ~ 1.049V
	{DAUDIO_BT_1MODULE_2, 1050, 1349},//BT/WIFI Combo(BCM89335) - 1.05 ~ 1.349V
	{DAUDIO_BT_1MODULE_3, 1350, 1649},//BT/WIFI Combo(BCM89335) - 1.35 ~ 1.649V
	{DAUDIO_BT_2MODULE_2, 1650, 1949},// BT only + BT/WIFI Combo(BCM89335) - 1.65V ~ 1.949V
	{DAUDIO_BT_2MODULE_3, 1950, 2249},// BT only + BT/WIFI Combo(BCM89335) - 1.95V ~ 2.249V
	{DAUDIO_BT_RESERVED, 2250, 2849}, // BT reserved - 2.25V ~ 2.849V
	{DAUDIO_BT_ONLY_CK, 2850, 3149},  // BT only, carkit - 2.85V ~ 3.149V
	{DAUDIO_BT_ONLY_SMD, 3150, 3300}, // BT only, SMD - 3.15V ~ 3.3V
#endif
};
*/
static ADC_Range_t lcd_ranges[] =
{
	{DAUDIOKK_LCD_10_25_1920_720_ADD_ON, 0, 0},
	{DAUDIOKK_LCD_10_25_1920_720_IN_CELL,100, 270},
	{DAUDIOKK_LCD_10_25_1920_720_TIYANMA, 280, 470}, 
	{DAUDIOKK_LCD_10_25_1920_720_DEPARTED,480, 630},
	{DAUDIOKK_LCD_08_00_800_480_ADD_ON, 640, 810},
	{DAUDIOKK_LCD_08_O0_800_480_ADD_ON_PIO, 820, 980},
	{DAUDIOKK_LCD_10_25_1920_720_IN_CELL_TOUCH_KEY, 990, 1140},
	{DAUDIOKK_LCD_07_00_1280_720_IN_CELL,1150, 1330},
	{DAUDIOKK_LCD_07_00_800_480_ADD_ON, 1340, 1520},
	{DAUDIOKK_LCD_07_00_800_480_IN_CELL,1530, 1700},
	{DAUDIOKK_LCD_RESERVED, 1710, 1890},
};

static unsigned long adcs[DAUDIO_VER_MAX];
static unsigned char vers[DAUDIO_VER_MAX];
static int count = 0;
static unsigned long gpio_adcs[10];
static int sizeof_adc_range_t(int what)
{
	int ret = 0;

	switch (what)
	{
		case DAUDIO_VER_HW:
			ret = sizeof(hw_ranges) / sizeof(hw_ranges[0]);
			break;

		case DAUDIO_VER_MAIN:
			ret = sizeof(main_ranges) / sizeof(main_ranges[0]);
			break;

/*		case DAUDIO_VER_BT:
			ret = sizeof(bt_ranges) / sizeof(lcd_ranges[0]);
			break; */

		case DAUDIO_VER_LCD:
			ret = sizeof(lcd_ranges) / sizeof(lcd_ranges[0]);
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
}

static ADC_Range_t* get_range(int what)
{
	ADC_Range_t* ret = NULL;

	switch (what)
	{
		case DAUDIO_VER_HW:
			ret = hw_ranges;
			break;

		case DAUDIO_VER_MAIN:
			ret = main_ranges;
			break;

/*		case DAUDIO_VER_BT:
			ret = bt_ranges;
			break;*/

		case DAUDIO_VER_LCD:
			ret = lcd_ranges;
			break;

		default:
			ret = 0;
			break;
	}
	
	return ret;
}

int init_version(int what, unsigned long adc)
{
	int i;
    int ret = -1;
	int size = sizeof_adc_range_t(what);
	ADC_Range_t* adc_range = get_range(what);

	if(adc_range)
	{
		adc = ADCCONV(adc);
	//	dprintf(INFO, "init_version_adc(%d)(%d)\n", what, adc);

		for(i = 0 ; i < size ; i++)
		{
			if(adc_range[i].min <= adc && adc <= adc_range[i].max)
            {
				ret = i;
                break;
            }
		}
	}

	return ret;
}

int adc_valid_check(unsigned long adc, int what, int ver)
{
    int valid = 0;
	unsigned long max, min, margin;
	ADC_Range_t* adc_range = get_range(what);

	if(adc_range)
	{
		adc = ADCCONV(adc);
	//	dprintf(INFO, "adc_valid_check_adc(%d)(%d)(%d)\n",adc,what,ver);

		max = adc_range[ver].max;
		min = adc_range[ver].min;
		margin = (max - min)/10;

		if((min + margin) <= adc && adc <= (max - margin))
			valid = 1;
		else
			valid = 0;
				
	}
	return valid;
}

static unsigned char parse_ver(unsigned long adc, int what)
{
	int i;
	int size = sizeof_adc_range_t(what);
	unsigned char default_ver[3]= {DAUDIOKK_LCD_10_25_1920_720_ADD_ON,DAUDIOKK_PLATFORM_WS4,DAUDIOKK_HW_1ST};
	unsigned char ret = default_ver[what];		//default board
	unsigned long min, max;
	ADC_Range_t *temp_range = get_range(what);

	if (temp_range)
	{
		for (i = 0; i < size; i++)
		{
			min = temp_range[i].min;
			max = temp_range[i].max;

			if (adc >= min && adc <= max)
			{
				ret = temp_range[i].ver;
				count = i;
				break;
			}
		}
	}

	return ret;
}

void init_daudio_ver(unsigned long *adc)
{
	int i = 0;
    unsigned long temp;

    log("%s\n", __func__);

	for (i = 0; i < DAUDIO_VER_MAX; i++)
	{
        temp = *(adc + i);
		adcs[i] = (temp >= 0) ? ADCCONV(temp) : -1;
		vers[i] = parse_ver(adcs[i], i);
	//	dprintf(INFO, "init_daudio_ver(%d)(%d)(%d)\n",i,adcs[i], vers[i]);
	}
}

void init_daudio_adc(unsigned long adc, unsigned long num)
{	
	gpio_adcs[num] = adc;
}	


unsigned char get_daudio_hw_ver(void)
{
	return vers[DAUDIO_VER_HW];
}


unsigned char get_daudio_main_ver(void)
{
	return vers[DAUDIO_VER_MAIN];
}

/*
unsigned char get_daudio_bt_ver(void)
{
	return vers[DAUDIO_VER_BT];
}
*/

unsigned char get_daudio_lcd_ver(void)
{
	return vers[DAUDIO_VER_LCD];
}

unsigned long get_daudio_hw_adc(void)
{
	return adcs[DAUDIO_VER_HW];
}


unsigned long get_daudio_main_adc(void)
{
	return adcs[DAUDIO_VER_MAIN];
}

/*
unsigned long get_daudio_bt_adc(void)
{
	return adcs[DAUDIO_VER_BT];
}
*/
unsigned long get_daudio_lcd_adc(void)
{
	return adcs[DAUDIO_VER_LCD];
}
unsigned long get_daudiokk_adc_ain02(void)	
{
       return gpio_adcs[2];	
}	

unsigned long get_daudiokk_adc_ain03(void)
{	
       return gpio_adcs[3];
}	

unsigned long get_daudiokk_adc_ain06(void)	
{	
       return gpio_adcs[6];	
}	

unsigned long get_daudiokk_adc_ain07(void)
{	
     return gpio_adcs[7];	
}	

unsigned long get_daudiokk_adc_ain09(void)	
{	
       return gpio_adcs[9];	
}


