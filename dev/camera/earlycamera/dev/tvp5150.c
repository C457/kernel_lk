
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#include <debug.h>
#include <i2c.h>
#include <gpio.h>
#include <sensor_if.h>
#include <tvp5150.h>
#include <delay.h>

struct sensor_gpio gpio_info = {
#if defined(CONFIG_CHIP_TCC8970)
	.pwr_port = TCC_GPEXT2(18),
	.pwd_port = TCC_GPEXT2(21),
	.rst_port = TCC_GPEXT2(20),

	.vsync = TCC_GPF(2),
	.hsync = TCC_GPF(1),
	.pclk  = TCC_GPF(0),

	.data = {TCC_GPF(3), TCC_GPF(4), TCC_GPF(5), TCC_GPF(6), TCC_GPF(7), TCC_GPF(8), TCC_GPF(9), TCC_GPF(10)},

	.data_func = GPIO_FN1
#elif defined(CONFIG_CHIP_TCC8960)
#if (HW_REV == 0x1001)
	.pwr_port = TCC_GPF(24),
	.pwd_port = TCC_GPF(13),
	.rst_port = TCC_GPF(14),

	.vsync = TCC_GPF(2),
	.hsync = TCC_GPF(1),
	.pclk  = TCC_GPF(0),

	.data = {TCC_GPF(3), TCC_GPF(4), TCC_GPF(5), TCC_GPF(6), TCC_GPF(7), TCC_GPF(8), TCC_GPF(9), TCC_GPF(10)},

	.data_func = GPIO_FN1
#endif
#endif
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

static struct sensor_reg sensor_regs_initialize[] = {
	{0x03, 0x2D},  // GPCL set to Output!!
	{0x12, 0x04},
	{0xFF, 0xFF}
};

struct sensor_reg * sensor_regs_type_and_encode[CAM_TYPE_MAX][CAM_ENC_MAX] = {
	// CAM_TYPE_CVBS
	{
		sensor_regs_initialize,		// CAM_ENC_NTSC
		NULL,				// CAM_ENC_PAL
	},
};

static int sensor_write_reg(unsigned char * out_buf, unsigned int nReg, unsigned int nData) {
	unsigned char out_len = nReg + nData;
	int ret = 0;
	
	ret = i2c_xfer(TVP5150_I2C_ADDR, out_len, out_buf, NULL, 0, TVP5150_I2C_CH_NO);
	if(ret != 0) {
		log("reg: 0x%02x, val: 0x%02x, write error!!!!\n", out_buf[0], out_buf[1]);
		return -1;
	}
	return 0;
}

static int sensor_read_reg(unsigned char cmd, unsigned char * value) {
	unsigned char send_data[1], read_data[1];

	log("addr: 0x%x, ch: 0x%x\n", TVP5150_I2C_ADDR, TVP5150_I2C_CH_NO);
	
	send_data[0] = cmd;
	read_data[0] = 0;

	i2c_xfer(TVP5150_I2C_ADDR, 1, send_data, 1, read_data, TVP5150_I2C_CH_NO);

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
	unsigned char	reg = 0x88;
	unsigned char	val = 0;
	int		ret = -1;
	
	sensor_read_reg(reg, &val);
	// 1, 2, 3 bit : Hsync lock, Vsync lock, Color subcarrier lock status
	ret = (val != 0xff && val & 0x0e) ? 0 : -1;
	
	log("reg: 0x%2x, val: 0x%02x, signal: %c\n", reg, val, (ret == 0) ? 'O' : 'X');
	
	return ret;
}

int sensor_if_connect_api(SENSOR_FUNC_TYPE * sensor_func) {
	sensor_func->close		= sensor_close;
	sensor_func->open		= sensor_open;
	sensor_func->tune		= sensor_tune;
	sensor_func->video_check	= sensor_video_check;
	return 0;
}

