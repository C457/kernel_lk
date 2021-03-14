#ifndef __TW9921__
#define __TW9921__

#include <i2c.h>
#include <platform/gpio.h>

#include <daudio_settings.h>

#define TW9921_RESET_GPIO		TCC_GPG(14)

#define MODULE_NODE "tw9921"
#define CAM_I2C_NAME "tcc_camera,0x88"

#define SLAVE_ADDRESS_TW9921 	0x88
#define I2C_CH_TW9921 			I2C_CH_MASTER3

#define TW9921_SUCCESS_					1
#define TW9921_SUCCESS_I2C				2
#define TW9921_SUCCESS_WRITE_SETTING	3

#define TW9921_FAIL						300
#define TW9921_FAIL_I2C					301
#define TW9921_FAIL_READ_SETTING		302
#define TW9921_FAIL_WRITE_SETTING		303

#define TW9921_I2C_RETRY_COUNT			3

#define TW9921_I2C_BRIGHTNESS		0x10
#define TW9921_I2C_CONTRAST			0x11
#define TW9921_I2C_HUE				0x15
#define TW9921_I2C_SATURATION_U		0x13
#define TW9921_I2C_SATURATION_V		0x14

struct TW9921_REG
{
	unsigned char cmd;
	unsigned char value;
};

void tw9921_init();
int tw9921_set_ie(int mode, unsigned char value);

#endif
