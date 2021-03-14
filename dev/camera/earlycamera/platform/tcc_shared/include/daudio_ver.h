#ifndef __DAUDIO_BOARD_VER__
#define __DAUDIO_BOARD_VER__

typedef enum daudio_board_ver_data
{
	DAUDIOKK_PLATFORM_WS4=1,
	DAUDIOKK_PLATFORM_WS5,
	DAUDIOKK_PLATFORM_WS3,
	DAUDIOKK_PLATFORM_WS6,
	DAUDIOKK_PLATFORM_WS7,
	DAUDIOKK_PLATFORM_WS8,
	DAUDIOKK_PLATFORM_WS9,
	DAUDIOKK_PLATFORM_WS10,
	DAUDIOKK_PLATFORM_WS11,
} daudio_board_ver_data_t;

/*
typedef enum daudio_ver_bt
{
#if defined(INCLUDE_DAUDIO_ECO)
	DAUDIOECO_BT_2MODULE_0,
	DAUDIOECO_BT_2MODULE_1,
	DAUDIOECO_BT_1MODULE_0,
	DAUDIOECO_BT_1MODULE_1, 
	DAUDIOECO_BT_1MODULE_2, 
	DAUDIOECO_BT_1MODULE_3, 
	DAUDIOECO_BT_2MODULE_2, 
	DAUDIOECO_BT_2MODULE_3, 
	DAUDIOECO_BT_RESERVED1, 
	DAUDIOECO_BT_RESERVED2, 
	DAUDIOECO_BT_ONLY_CK, 
	DAUDIOECO_BT_ONLY_SMD,
#else
	DAUDIO_BT_2MODULE_0,
	DAUDIO_BT_1MODULE_0,
	DAUDIO_BT_RESERVED,
	DAUDIO_BT_ONLY_CK,
	DAUDIO_BT_ONLY_SMD,
	DAUDIO_BT_2MODULE_1,
	DAUDIO_BT_2MODULE_2,
	DAUDIO_BT_2MODULE_3,
	DAUDIO_BT_1MODULE_1,
	DAUDIO_BT_1MODULE_2,
	DAUDIO_BT_1MODULE_3,
#endif
} daudio_ver_bt_t;
*/

typedef enum daudio_ver_data
{
	DAUDIO_VER_LCD = 0,
	DAUDIO_VER_MAIN,
	DAUDIO_VER_HW,
	//DAUDIO_VER_BT,
	DAUDIO_VER_MAX
} daudio_ver_data_t;

typedef enum daudio_ver_hw
{
	DAUDIOKK_HW_1ST=1,
	DAUDIOKK_HW_2ND,
	DAUDIOKK_HW_3RD,
	DAUDIOKK_HW_4TH,
	DAUDIOKK_HW_5TH,
	DAUDIOKK_HW_6TH,
	DAUDIOKK_HW_7TH,
	DAUDIOKK_HW_8TH,
	DAUDIOKK_HW_9TH,
} daudio_ver_hw_t;
typedef enum daudio_ver_lcd
{

	DAUDIOKK_LCD_10_25_1920_720_IN_CELL, //LGD IN_CELL
	DAUDIOKK_LCD_10_25_1920_720_ADD_ON, //LGD OGS
	DAUDIOKK_LCD_10_25_1920_720_TIYANMA,  //TIYANMA
	DAUDIOKK_LCD_10_25_1920_720_DEPARTED, //DEPARTED
	DAUDIOKK_LCD_08_00_800_480_ADD_ON, //
	DAUDIOKK_LCD_08_O0_800_480_ADD_ON_PIO, //PIO AUO LCD Shared with PIO(10.25, 19270*720, PIO_AUO_LCD)==>Have to chaneged after adc variant is chagend according to pull-up/down.
	DAUDIOKK_LCD_10_25_1920_720_IN_CELL_TOUCH_KEY, //LGD_INCELL_TOUCH_KEY
	DAUDIOKK_LCD_07_00_1280_720_IN_CELL,
	DAUDIOKK_LCD_07_00_800_480_ADD_ON,
	DAUDIOKK_LCD_07_00_800_480_IN_CELL,
	DAUDIOKK_LCD_RESERVED, // Temp. PIO3.0
} daudio_ver_lcd_t;
void init_daudio_ver(unsigned long *adc);
int adc_valid_check(unsigned long adc, int what, int ver);
unsigned char get_daudio_hw_ver(void);
unsigned char get_daudio_main_ver(void);
//unsigned char get_daudio_bt_ver(void);
unsigned char get_daudio_lcd_ver(void);
unsigned long get_daudio_hw_adc(void);
unsigned long get_daudio_main_adc(void);
//unsigned long get_daudio_bt_adc(void);
unsigned long get_daudio_lcd_adc(void);
#endif
