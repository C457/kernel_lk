#include <tw9921.h>
#include <stdlib.h>
#include <stdio.h>
#include <daudio_ie.h>
#include <daudio_ver.h>

#include <dev/gpio.h>
#include <tcc_lcd.h>

#define CONFIG_DAUDIO_PRINT_DEBUG_MSG

#if defined(CONFIG_DAUDIO_PRINT_DEBUG_MSG)
#define DEBUG_TW9921	1
#else
#define DEBUG_TW9921	0
#endif

#if (DEBUG_TW9921)
#define LKPRINT(fmt, args...) dprintf(INFO, "[cleinsoft tw9921] " fmt, ##args)
#else
#define LKPRINT(args...) do {} while(0)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

static void set_cam_settings();
static int getVideoType();
static int getCamType();

static struct TW9921_REG TW9921_INIT_REGS[] =
{
	//james.kang@zentech.co.kr 140923
	//cmd, value
	{ 0x01, 0x68 },
	{ 0x02, 0x44 },
	{ 0x03, 0x20 },
	{ 0x04, 0x00 },
	{ 0x05, 0x10 },
	{ 0x06, 0x03 },
	{ 0x07, 0x02 },
//	{ 0x08, 0x16 },
	{ 0x08, 0x14 }, // vertical line 
	{ 0x09, 0xF3 },  // 09 -> F3
	{ 0x0A, 0x0B }, // 10 -> 0B
	{ 0x0B, 0xD0 },
	{ 0x0C, 0xCC },
	{ 0x10, 0x00 },
	{ 0x11, 0x64 },
	{ 0x12, 0x11 },
	{ 0x13, 0x80 },
	{ 0x14, 0x80 },
	{ 0x15, 0x00 },
	{ 0x17, 0x30 },
	{ 0x18, 0x44 },
	{ 0x1A, 0x10 },
	{ 0x1B, 0x00 },
	{ 0x1C, 0x0F },
	{ 0x1D, 0x7F },
 	{ 0x1E, 0x08 },
	{ 0x1F, 0x00 },
	{ 0x20, 0x50 },
	{ 0x21, 0x42 },
	{ 0x22, 0xF0 },
	{ 0x23, 0xD8 },
	{ 0x24, 0xBC },
	{ 0x25, 0xB8 },
	{ 0x26, 0x44 },
	{ 0x27, 0x38 },
	{ 0x28, 0x00 },
	{ 0x29, 0x00 },
	{ 0x2A, 0x78 },
	{ 0x2B, 0x44 },
	{ 0x2C, 0x30 },
	{ 0x2D, 0x14 },
	{ 0x2E, 0xA5 },
//	{ 0x2F, 0x26 },	// blue color
	{ 0x2F, 0X24 },	// black color
	{ 0x30, 0x00 },
	{ 0x31, 0x10 },
	{ 0x32, 0x00 },
//	{ 0x33, 0x05 },
	{ 0x33, 0x85 }, // FreeRun 60Hz
	{ 0x34, 0x1A },
	{ 0x35, 0x00 },
	{ 0x36, 0xE8 },
	{ 0x37, 0x12 },
	{ 0x38, 0x01 },
	{ 0xC0, 0x01 },
	{ 0xC1, 0x07 },
	{ 0xC2, 0x11 },
	{ 0xC3, 0x03 },
	{ 0xC4, 0x5A },
	{ 0xC5, 0x00 },
	{ 0xC6, 0x20 },
	{ 0xC7, 0x04 },
	{ 0xC8, 0x00 },
	{ 0xC9, 0x06 },
	{ 0xCA, 0x06 },
	{ 0xCB, 0x30 },
	{ 0xCC, 0x00 },
	{ 0xCD, 0x54 },
	{ 0xD0, 0x00 },
	{ 0xD1, 0xF0 },
	{ 0xD2, 0xF0 },
	{ 0xD3, 0xF0 },
	{ 0xD4, 0x00 },
	{ 0xD5, 0x00 },
	{ 0xD6, 0x10 },
	{ 0xD7, 0x70 },
	{ 0xD8, 0x00 },
	{ 0xD9, 0x04 },
	{ 0xDA, 0x80 },
	{ 0xDB, 0x80 },
	{ 0xDC, 0x20 },
	{ 0xE0, 0x00 },
	{ 0xE1, 0x45 },
	{ 0xE2, 0xD9 },
	{ 0xE3, 0x00 },
	{ 0xE4, 0x00 },
	{ 0xE5, 0x00 },
	{ 0xE6, 0x00 },
	{ 0xE7, 0x2A },
	{ 0xE8, 0x0F },
//	{ 0xE9, 0x14 }, pclk falling
	{ 0xE9, 0x75 },	// pclk rising
};

static struct TW9921_REG TW9921_ANALOG_INPUT_OPEN_REGS[] =
{
	{ 0x03, 0x21 },
};

static struct TW9921_REG TW9921_ANALOG_INPUT_CLOSE_REGS[] =
{
	{ 0x03, 0x27 },
};

static struct TW9921_REG TW9921_SVM_REGS[] =
{
	{ 0x09, 0xF4 }
};

static int tw9921_i2c_read(unsigned char cmd, unsigned char *recv_data)
{
	int ret = -1, count = TW9921_I2C_RETRY_COUNT;

	do {
		ret = i2c_xfer(SLAVE_ADDRESS_TW9921, 1, &cmd, 1, recv_data, I2C_CH_TW9921);
		if (ret == 0)
			return ret;
		else
			count--;

	} while(count > 0);

	return ret;
}

static int tw9921_i2c_write(unsigned char cmd, unsigned char value)
{
	unsigned char send_data[2];
	int ret = -1, count = TW9921_I2C_RETRY_COUNT;
	send_data[0] = cmd;
	send_data[1] = value;

	do {
		ret = i2c_xfer(SLAVE_ADDRESS_TW9921, 2, send_data, 0, 0, I2C_CH_TW9921);
		if (ret == 0)
			return ret;
		else
			count--;

	} while(count > 0);

	return ret;
}

int tw9921_set_ie(int mode, unsigned char value)
{
	int ret = FAIL;

	if (value == 0) return ret;

	switch (mode)
	{
		case SET_CAM_BRIGHTNESS:
			value -= 127;
			ret = tw9921_i2c_write(TW9921_I2C_BRIGHTNESS, value);
			break;

		case SET_CAM_CONTRAST:
			ret = tw9921_i2c_write(TW9921_I2C_CONTRAST, value);
			break;

		case SET_CAM_HUE:
			value -= 127;
			ret = tw9921_i2c_write(TW9921_I2C_HUE, value);
			break;

		case SET_CAM_SATURATION:
			ret = tw9921_i2c_write(TW9921_I2C_SATURATION_U, value);
			if (ret == 0)
				ret = tw9921_i2c_write(TW9921_I2C_SATURATION_V, value);
			break;
	}

	return ret;
}

void tw9921_init()
{
	int i, ret = 0;
	unsigned char *read_data = 0;
	const int size = ARRAY_SIZE(TW9921_INIT_REGS);

	//TW9921 reset
	gpio_set(TW9921_RESET_GPIO, 0);
	gpio_set(TW9921_RESET_GPIO, 1);
	lcd_delay_us(10);

	//read id
	//ret = tw9921_i2c_read(0x00, read_data);
	if (ret == 0)
	{
		LKPRINT("%s TW9921 id: 0x%x \n", __func__, *read_data);
	}
	else
	{
		LKPRINT("%s failed to read id.\n", __func__);
	}

	for (i = 0; i < size; i++)
	{
		const unsigned char cmd = TW9921_INIT_REGS[i].cmd;
		const unsigned char value = TW9921_INIT_REGS[i].value;

		ret = tw9921_i2c_write(cmd, value);
	}

	set_cam_settings();

	LKPRINT("%s finished.\n", __func__);
}

/* FUNCTION : static void set_cam_settings()			
 * Author : hkleelkh@mobis.co.kr					
 * Description :							
 * This function set pre-defined settings according to a cam type. */	

static void set_cam_settings()
{
	int i, ret, size;
	struct TW9921_REG* cam_reg = NULL;

	switch(getCamType())
	{
		case SVM_CAM:
			cam_reg = TW9921_SVM_REGS;
			size = ARRAY_SIZE(TW9921_SVM_REGS);
			LKPRINT("%s - SVM mode\n", __func__);
			break;
		default:
			break;
	}

	if(cam_reg)
	{
		for(i = 0 ; i < size ; i++)
		{
			const unsigned char cmd = cam_reg[i].cmd;
			const unsigned char value = cam_reg[i].value;

			ret = tw9921_i2c_write(cmd, value);

			LKPRINT("%s cmd : 0x%x, val : 0x%x\n", __func__, cmd, value);
		}
	}
	else
		LKPRINT("%s - Default mode\n", __func__);
}

/* FUNCTION : static int getVideoType()			
 * Author : hkleelkh@mobis.co.kr					
 * Description :							
 * This function defines a video type from gpiof26,27. 
 * It can be different depending on the main board version 
 * DAUDIO_BOARD_5TH(D-Audio) : define with the gpiof26,27 pin value
 * DAUDIO_BOARD_6TH(D-Audio+) : same way, but it will be changed. (add f23)*/	

static int getVideoType()
{
	int videoType;

	/* CVBS : 0, S_VIDEO : 1, COMPONENT : 2*/
	videoType = ((gpio_get(GPIO_PORTF|26) << 1) | gpio_get(GPIO_PORTF|27));

	//if(videoType > 2)
		videoType = CVBS;

	return videoType;
}

/* FUNCTION : static int getCamType()			
 * Author : hkleelkh@mobis.co.kr					
 * Description :							
 * This function gets a cam type from the video type. 
 * It can be different depending on the main board version 
 * DAUDIO_BOARD_5TH(D-Audio) : depending on the video type
 * DAUDIO_BOARD_6TH(D-Audio+) : same way, but it will be changed */	

static int getCamType()
{
	int camType;

	switch(getVideoType())
	{
		case S_VIDEO:
		case COMPONENT:
			camType = SVM_CAM;
			break;
		default:
			camType = DIRECT_CAM;
			break;
	}

	return camType;
}


