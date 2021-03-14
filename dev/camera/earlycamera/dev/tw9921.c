
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#include <debug.h>
#include <i2c.h>
#include <gpio.h>
#include <sensor_if.h>
#include <tw9921.h>
#include <delay.h>

struct sensor_gpio gpio_info = {
	.pwr_port = NULL,
	.pwd_port = NULL,
	.rst_port = TCC_GPG(14),

	.vsync = 0,
	.hsync = 0,
	.pclk  = TCC_GPF(0),

	.data = {TCC_GPF(3), TCC_GPF(4), TCC_GPF(5), TCC_GPF(6), TCC_GPF(7), TCC_GPF(8), TCC_GPF(9), TCC_GPF(10)},

	.data_func = GPIO_FN1
};

void sensor_set_gpio_pin(void) {
	int idxData;

	// Configure power.
	gpio_config(gpio_info.pwr_port, GPIO_OUTPUT);
	gpio_config(gpio_info.pwd_port, GPIO_OUTPUT);
	gpio_config(gpio_info.rst_port, GPIO_OUTPUT);

	// Configure signal port.
	gpio_config(gpio_info.hsync, gpio_info.data_func);
	gpio_config(gpio_info.vsync, gpio_info.data_func);
	gpio_config(gpio_info.pclk,  gpio_info.data_func);

	// Configue data port.
	for(idxData=0; gpio_info.data[idxData] != NULL; idxData++) {
		log("gpio pin is %d\n", gpio_info.data[idxData]);
		gpio_config(gpio_info.data[idxData], gpio_info.data_func);
	}

	// Power-up.
	gpio_set(gpio_info.pwr_port, 1);
	gpio_set(gpio_info.pwd_port, 1);
	gpio_set(gpio_info.rst_port, 0);
}

static struct sensor_reg cvbs_component[] = {
	{0x01, 0x68},
	{0x02, 0x44},
	{0x03, 0x21},
	{0x04, 0x00},
	{0x05, 0x10},
	{0x06, 0x03},
	{0x1C, 0x0F}, // mobis - initialize issue
	{0x07, 0x02},
	{0x08, 0x14},
	{0x09, 0xF3},
	{0x0A, 0x0b},
	{0x0B, 0xD0},
	{0x0C, 0xCC},
	{0x10, 0x00},
	{0x11, 0x64},
	{0x12, 0x11},
	{0x13, 0x80},
	{0x14, 0x80},
	{0x15, 0x00},
	{0x17, 0x30},
	{0x18, 0x44},
	{0x1A, 0x10},
	{0x1B, 0x00},
//	{0x1C, 0x0F},   mobis - initialize issue
	{0x1D, 0x7F},
	{0x1E, 0x08},
	{0x1F, 0x00},
	{0x20, 0x50},
	{0x21, 0x42},
	{0x22, 0xF0},
	{0x23, 0xD8},
	{0x24, 0xBC},
	{0x25, 0xB8},
	{0x26, 0x44},
	{0x27, 0x38},
	{0x28, 0x00},
	{0x29, 0x00},
	{0x2A, 0x78},
	{0x2B, 0x44},
	{0x2C, 0x30},
	{0x2D, 0x14},
	{0x2E, 0xA5},
	{0x2F, 0X24},
	{0x30, 0x00},
	{0x31, 0x10},
	{0x32, 0x00},
	{0x33, 0x85},
	{0x34, 0x1A},
	{0x35, 0x00},
	{0x36, 0xE8},
	{0x37, 0x12},
	{0x38, 0x01},
	{0xC0, 0x01},
	{0xC1, 0x07},
	{0xC2, 0x11},
	{0xC3, 0x03},
	{0xC4, 0x5A},
	{0xC5, 0x00},
	{0xC6, 0x20},
	{0xC7, 0x04},
	{0xC8, 0x00},
	{0xC9, 0x06},
	{0xCA, 0x06},
	{0xCB, 0x30},
	{0xCC, 0x00},
	{0xCD, 0x54},
	{0xD0, 0x00},
	{0xD1, 0xF0},
	{0xD2, 0xF0},
	{0xD3, 0xF0},
	{0xD4, 0x00},
	{0xD5, 0x00},
	{0xD6, 0x10},
	{0xD7, 0x70},
	{0xD8, 0x00},
	{0xD9, 0x04},
	{0xDA, 0x80},
	{0xDB, 0x80},
	{0xDC, 0x20},
	{0xE0, 0x00},
	{0xE1, 0x45},
	{0xE2, 0xD9},
	{0xE3, 0x00},
	{0xE4, 0x00},
	{0xE5, 0x00},
	{0xE6, 0x00},
	{0xE7, 0x2A},
	{0xE8, 0x0F},
	{0xE9, 0x75},
	{0xFF, 0xFF}
};

static struct sensor_reg s_video[] = {
	{0x01, 0x68},
	{0x02, 0x54},
	{0x03, 0x21},
	{0x04, 0x00},
	{0x05, 0x10},
	{0x06, 0x00},
	{0x1C, 0x0F}, // mobis - initialize issue
	{0x07, 0x02},
	{0x08, 0x14},
	{0x09, 0xF3},
	{0x0A, 0x0b},
	{0x0B, 0xD0},
	{0x0C, 0xCC},
	{0x10, 0x00},
	{0x11, 0x64},
	{0x12, 0x11},
	{0x13, 0x80},
	{0x14, 0x80},
	{0x15, 0x00},
	{0x17, 0x30},
	{0x18, 0x44},
	{0x1A, 0x10},
	{0x1B, 0x00},
//	{0x1C, 0x0F},	mobis - initialize issue
	{0x1D, 0x7F},
	{0x1E, 0x08},
	{0x1F, 0x00},
	{0x20, 0x50},
	{0x21, 0x42},
	{0x22, 0xF0},
	{0x23, 0xD8},
	{0x24, 0xBC},
	{0x25, 0xB8},
	{0x26, 0x44},
	{0x27, 0x38},
	{0x28, 0x00},
	{0x29, 0x00},
	{0x2A, 0x78},
	{0x2B, 0x44},
	{0x2C, 0x30},
	{0x2D, 0x14},
	{0x2E, 0xA5},
	{0x2F, 0X24},
	{0x30, 0x00},
	{0x31, 0x10},
	{0x32, 0x00},
	{0x33, 0x85},
	{0x34, 0x1A},
	{0x35, 0x00},
	{0x36, 0xE8},
	{0x37, 0x12},
	{0x38, 0x01},
	{0xC0, 0x01},
	{0xC1, 0x07},
	{0xC2, 0x11},
	{0xC3, 0x03},
	{0xC4, 0x5A},
	{0xC5, 0x00},
	{0xC6, 0x20},
	{0xC7, 0x04},
	{0xC8, 0x00},
	{0xC9, 0x06},
	{0xCA, 0x06},
	{0xCB, 0x30},
	{0xCC, 0x00},
	{0xCD, 0x54},
	{0xD0, 0x00},
	{0xD1, 0xF0},
	{0xD2, 0xF0},
	{0xD3, 0xF0},
	{0xD4, 0x00},
	{0xD5, 0x00},
	{0xD6, 0x10},
	{0xD7, 0x70},
	{0xD8, 0x00},
	{0xD9, 0x04},
	{0xDA, 0x80},
	{0xDB, 0x80},
	{0xDC, 0x20},
	{0xE0, 0x00},
	{0xE1, 0x45},
	{0xE2, 0xD9},
	{0xE3, 0x00},
	{0xE4, 0x00},
	{0xE5, 0x00},
	{0xE6, 0x00},
	{0xE7, 0x2A},
	{0xE8, 0x0F},
	{0xE9, 0x75},
	{0xFF, 0xFF}
};

struct sensor_reg * sensor_regs_type_and_encode[CAM_TYPE_MAX][CAM_ENC_MAX] = {
	// CAM_TYPE_CVBS
	{
		cvbs_component, 		// CAM_ENC_NTSC
		NULL,				// CAM_ENC_PAL
	},
	// CAM_TYPE_SVIDEO
	{
		s_video,			// CAM_ENC_NTSC
		NULL,				// CAM_ENC_PAL
	},
	// CAM_TYPE_COMPONENT
	{
		cvbs_component,			// CAM_ENC_NTSC
		NULL,				// CAM_ENC_PAL
	},
};

static int sensor_write_reg(unsigned char * out_buf, unsigned int nReg, unsigned int nData) {
	unsigned char out_len = nReg + nData;
	int ret = 0;
	
	ret = i2c_xfer(TW9921_I2C_ADDR, out_len, out_buf, NULL, 0, TW9921_I2C_CH_NO);
	if(ret != 0) {
		log("reg: 0x%02x, val: 0x%02x, write error!!!!\n", out_buf[0], out_buf[1]);
		return -1;
	}
	return 0;
}

static int sensor_read_reg(unsigned char cmd, unsigned char * value) {
	unsigned char send_data[1], read_data[1];

	log("addr: 0x%x, ch: 0x%x\n", TW9921_I2C_ADDR, TW9921_I2C_CH_NO);
	
	send_data[0] = cmd;
	read_data[0] = 0;

	i2c_xfer(TW9921_I2C_ADDR, 1, send_data, 1, read_data, TW9921_I2C_CH_NO);

	*value = read_data[0];

	return 0;
}

static int write_regs(const struct sensor_reg reglist[]) {
	const struct sensor_reg * next = reglist;
	unsigned char data[132];
	unsigned char bytes;
	int err_cnt = 0;
	int ret = -1;

	while(!((next->reg == 0xFF) && (next->val == 0xFF))) {
		if(next->reg == 0xFF && next->val != 0xFF) {
			mdelay(next->val);
			next++;
		} else {
			bytes = 0;
			data[bytes] = (unsigned char)next->reg & 0xff; 	bytes++;
			data[bytes] = (unsigned char)next->val & 0xff; 	bytes++;

			ret = sensor_write_reg(data, 1, 1);
			if(ret) {
				err_cnt++;
				if(err_cnt >= 3) {
					printk("CM4 : ERROR: Sensor I2C !!!! \n"); 
					return ret;
				}
			} else {
				err_cnt = 0;
				next++;
			}
		}
	}
	return 0;
}

int sensor_close(void) {
	return 0;
}

int sensor_tune(int camera_type, int camera_encode) {
	log("camera_type: 0x%x, camera_encode: 0x%x\n", camera_type, camera_encode);
	
	if((CAM_TYPE_MAX <= camera_type) || (CAM_ENC_MAX <= camera_encode)) {
		log("function index is WRONG.\n");
		return -1;
	}
	
	if(sensor_regs_type_and_encode[camera_type][camera_encode] == NULL) {
		log("function is NULL.\n");
		return -1;
	}
	return write_regs(sensor_regs_type_and_encode[camera_type][camera_encode]);
}

int sensor_open(int camera_type, int camera_encode) {
	sensor_set_gpio_pin();
	
	// Power-up sequence
	gpio_set(gpio_info.pwr_port, 1);	// sensor_power_enable
	tcc_cif_delay(10);
	
	gpio_set(gpio_info.pwd_port, 1);	// sensor_powerdown_disable
	tcc_cif_delay(40);
	
	gpio_set(gpio_info.rst_port, 0);	// sensor_reset_high
	tcc_cif_delay(10);

	gpio_set(gpio_info.rst_port, 1);	// sensor_reset_high
	tcc_cif_delay(15);
	
	return sensor_tune(camera_type, camera_encode);
}

int sensor_video_check(void) {
	unsigned char readVal, tmp = 0;
	int ret;

	while(1) {
		sensor_read_reg(0x01, &readVal);
		log("[TW9921] initializing check/count : 0x%x/%d \n", readVal, tmp);

		if ((readVal&0xEB) == 0x68) {
			if (tmp > 0) mdelay(130);
			ret = 0;
			break;
		}
		if (tmp >= 9) { // waiting for 1000ms.
			printk("CM4(E-CAM):	fail to video signal check(%d). \n", tmp);
			ret = -1;
			break;
		} else {
			mdelay(100);
			tmp++;
		}
	}
	return ret;
}

int sensor_connect_api(SENSOR_FUNC_TYPE * sensor_func) {
	sensor_func->close		= sensor_close;
	sensor_func->open		= sensor_open;
	sensor_func->tune		= sensor_tune;
	sensor_func->video_check	= sensor_video_check;
	return 0;
}

