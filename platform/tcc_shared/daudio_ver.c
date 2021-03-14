#include <daudio_ver.h>
#include <stdlib.h>
#include <platform/gpio.h>
#include <dev/gpio.h>

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

typedef struct ADC_Range {
	unsigned char ver;
	unsigned long min;
	unsigned long max;
} ADC_Range_t;

static ADC_Range_t hw_ranges[] = {
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


static ADC_Range_t main_ranges[] = {
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

static ADC_Range_t bt_ranges[] = {
	{DAUDIOKK_BT_VER_1, 0, 199},
	{DAUDIOKK_BT_VER_2, 200, 399},
	{DAUDIOKK_BT_VER_3, 400, 599},
	{DAUDIOKK_BT_VER_4, 600, 799},
	{DAUDIOKK_BT_VER_5, 800, 999},
	{DAUDIOKK_BT_VER_6, 1000, 1199},
	{DAUDIOKK_BT_VER_7, 1200, 1399},
	{DAUDIOKK_BT_VER_8, 1400, 1599},
	{DAUDIOKK_BT_VER_9, 1600, 1800},
};
static ADC_Range_t lcd_ranges[11];

static ADC_Range_t lcd_versions_oe_int[11] = {
	{DAUDIOKK_LCD_OI_10_25_1920_720_INCELL_Si, 0, 0},
	{DAUDIOKK_LCD_OI_10_25_1920_720_OGS_TEMP, 100, 270},
	{DAUDIOKK_LCD_OI_RESERVED1, 280, 470},
	{DAUDIOKK_LCD_OI_RESERVED2, 480, 630},
	{DAUDIOKK_LCD_OI_RESERVED3, 640, 810},
	{DAUDIOKK_LCD_OI_RESERVED4, 820, 980},
	{DAUDIOKK_LCD_OI_RESERVED5, 990, 1140},
	{DAUDIOKK_LCD_OI_10_25_1920_720_INCELL_LTPS, 1150, 1340},
	{DAUDIOKK_LCD_OI_08_00_1280_720_OGS_Si, 1350, 1520},
	{DAUDIOKK_LCD_OI_RESERVED6, 1530, 1710},
	{DAUDIOKK_LCD_OI_DISCONNECTED, 1710, 1890},
};

static ADC_Range_t lcd_versions_pio_int[11] = {
	{DAUDIOKK_LCD_PI_RESERVED1, 0, 0},
	{DAUDIOKK_LCD_PI_RESERVED2, 100, 270},
	{DAUDIOKK_LCD_PI_RESERVED3, 280, 470},
	{DAUDIOKK_LCD_PI_RESERVED4, 480, 630},
	{DAUDIOKK_LCD_PI_RESERVED5, 640, 810},
	{DAUDIOKK_LCD_PI_10_25_1920_720_PIO_AUO, 820, 980},
	{DAUDIOKK_LCD_PI_08_00_800_400_PIO_TRULY, 990, 1140},
	{DAUDIOKK_LCD_PI_RESERVED6, 1150, 1340},
	{DAUDIOKK_LCD_PI_RESERVED7, 1350, 1520},
	{DAUDIOKK_LCD_PI_RESERVED8, 1530, 1710},
	{DAUDIOKK_LCD_PI_DISCONNECTED, 1710, 1890},
};


static ADC_Range_t lcd_versions_oe_de[11] = {
	{DAUDIOKK_LCD_OD_RESERVED1, 0, 0},
	{DAUDIOKK_LCD_OD_RESERVED2, 100, 270},
	{DAUDIOKK_LCD_OD_RESERVED3, 280, 470},
	{DAUDIOKK_LCD_OD_10_25_1920_720_INCELL_Si, 480, 630},
	{DAUDIOKK_LCD_OD_12_30_1920_720_INCELL_Si, 640, 810},
	{DAUDIOKK_LCD_OD_10_25_1920_720_INCELL_LTPS, 820, 980},
	{DAUDIOKK_LCD_OD_08_00_1280_720_OGS_Si, 990, 1140},
	{DAUDIOKK_LCD_OD_RESERVED4, 1150, 1340},
	{DAUDIOKK_LCD_OD_RESERVED5, 1350, 1520},
	{DAUDIOKK_LCD_OD_RESERVED6, 1530, 1710},
	{DAUDIOKK_LCD_OD_DISCONNECTED, 1710, 1890},
};

static ADC_Range_t lcd_versions_pio_de[11] = {
	{DAUDIOKK_LCD_PD_RESERVED1, 0, 0},
	{DAUDIOKK_LCD_PD_RESERVED2, 100, 270},
	{DAUDIOKK_LCD_PD_RESERVED3, 280, 470},
	{DAUDIOKK_LCD_PD_RESERVED4, 480, 630},
	{DAUDIOKK_LCD_PD_RESERVED5, 640, 810},
	{DAUDIOKK_LCD_PD_RESERVED6, 820, 980},
	{DAUDIOKK_LCD_PD_RESERVED7, 990, 1140},
	{DAUDIOKK_LCD_PD_RESERVED8, 1150, 1340},
	{DAUDIOKK_LCD_PD_RESERVED9, 1350, 1520},
	{DAUDIOKK_LCD_PD_RESERVED10, 1530, 1710},
	{DAUDIOKK_LCD_PD_DISCONNECTED, 1710, 1890},
};

static int lcd_version_match(int oem, int mon)
{
	if((oem == 1) && (mon == 1))
		memcpy(lcd_ranges, lcd_versions_oe_int, sizeof(lcd_versions_oe_int));
	else if((oem == 0) && (mon == 1))
		memcpy(lcd_ranges, lcd_versions_pio_int, sizeof(lcd_versions_pio_int));
	else if ((oem == 1) && (mon == 0))
		memcpy(lcd_ranges, lcd_versions_oe_de, sizeof(lcd_versions_oe_de));
	else if ((oem == 0) && (mon == 0))
		memcpy(lcd_ranges, lcd_versions_pio_de, sizeof(lcd_versions_pio_de));
	else
		return -1;
	return 0;
}

static unsigned long adcs[DAUDIO_VER_MAX];
static unsigned char vers[DAUDIO_VER_MAX];
static int count = 0;
static unsigned long gpio_adcs[10];
static int sizeof_adc_range_t(int what)
{
	int ret = 0;

	switch (what) {
		case DAUDIO_VER_HW:
			ret = sizeof(hw_ranges) / sizeof(hw_ranges[0]);
			break;

		case DAUDIO_VER_MAIN:
			ret = sizeof(main_ranges) / sizeof(main_ranges[0]);
			break;

		case DAUDIO_VER_BT:
			ret = sizeof(bt_ranges) / sizeof(lcd_ranges[0]);
			break;

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
	int oem, mon;
	int retval;

	switch (what) {
		case DAUDIO_VER_HW:
			ret = hw_ranges;
			break;

		case DAUDIO_VER_MAIN:
			ret = main_ranges;
			break;

		case DAUDIO_VER_BT:
			ret = bt_ranges;
			break;

		case DAUDIO_VER_LCD: {
			oem = gpio_get(GPIO_PORTB | 24);
			mon = gpio_get(GPIO_PORTB | 13);
			retval = lcd_version_match(oem, mon);
			if(retval)
				dprintf(CRITICAL, "[ERROR] The table is out of boundary-HW Problem\n");
			ret = lcd_ranges;
			break;
		}
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
	ADC_Range_t* adc_range = get_range(what);
	int size = sizeof_adc_range_t(what);

	if(adc_range) {
		adc = ADCCONV(adc);
		for(i = 0 ; i < size ; i++) {
			if(adc_range[i].min <= adc && adc <= adc_range[i].max) {
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

	if(adc_range) {
		adc = ADCCONV(adc);
		//dprintf(INFO, "adc_valid_check_adc(%d)(%d)(%d)\n",adc,what,ver);

		max = adc_range[ver].max;
		min = adc_range[ver].min;
		margin = (max - min) / 10;

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
	ADC_Range_t *temp_range = get_range(what);
	int size = sizeof_adc_range_t(what);
	unsigned char default_ver[DAUDIO_VER_MAX] = {DAUDIOKK_LCD_OI_10_25_1920_720_INCELL_Si, DAUDIOKK_PLATFORM_WS4, DAUDIOKK_HW_1ST, DAUDIOKK_BT_VER_1};
	unsigned char ret = default_ver[what];		//default board
	unsigned long min, max;

	if (temp_range) {
		for (i = 0; i < size; i++) {
			min = temp_range[i].min;
			max = temp_range[i].max;

			if (adc >= min && adc <= max) {
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

	dprintf(SPEW, "%s\n", __func__);

	for (i = 0; i < DAUDIO_VER_MAX; i++) {
		temp = *(adc + i);
		adcs[i] = (temp >= 0) ? ADCCONV(temp) : -1;
		vers[i] = parse_ver(adcs[i], i);
		//dprintf(INFO, "init_daudio_ver(%d)(%d)(%d)\n",i,adcs[i], vers[i]);
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


unsigned char get_daudio_bt_ver(void)
{
	return vers[DAUDIO_VER_BT];
}


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


unsigned long get_daudio_bt_adc(void)
{
	return adcs[DAUDIO_VER_BT];
}

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


