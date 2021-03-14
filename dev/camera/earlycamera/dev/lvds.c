
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#include <debug.h>
#include <i2c.h>
#include <gpio.h>
#include <sensor_if.h>
#include <lvds.h>
#include <delay.h>

#define NULL		0

static struct sensor_gpio gpio_info = {
	.pwr_port = (int)NULL,
	.pwd_port = (int)NULL,
	.rst_port = (int)NULL,//TCC_GPG(14),

	.vsync = TCC_GPD(9),
	.hsync = TCC_GPD(10),
	.pclk  = TCC_GPD(8),

	.data = {TCC_GPD(0), TCC_GPD(1), TCC_GPD(2), TCC_GPD(3), TCC_GPD(4), TCC_GPD(5), TCC_GPD(6), TCC_GPD(7)},

	.data_func = GPIO_FN4
};

void lvds_set_gpio_pin(void) {
	int idxData;
	log("[lvds] start \n");

	// Configure power.
//	gpio_config(gpio_info.pwr_port, GPIO_OUTPUT);
//	gpio_config(gpio_info.pwd_port, GPIO_OUTPUT);
//	gpio_config(gpio_info.rst_port, GPIO_OUTPUT);

	// Configure signal port.
	gpio_config(gpio_info.hsync, gpio_info.data_func);
	gpio_config(gpio_info.vsync, gpio_info.data_func);
	gpio_config(gpio_info.pclk,  gpio_info.data_func);

	// Configue data port.
	for(idxData=0; gpio_info.data[idxData] != (int)NULL; idxData++) {
		log("gpio pin is %d\n", gpio_info.data[idxData]);		
		gpio_config(gpio_info.data[idxData], gpio_info.data_func);
	}

	// Power-up.
//	gpio_set(gpio_info.pwr_port, 1);
//	gpio_set(gpio_info.pwd_port, 1);
//	gpio_set(gpio_info.rst_port, 0);
}

static struct sensor_reg lvds_des_reset[] = {
	{0x01, 0x01},
	{0xFF, 0xFF}
};

#if 0
struct sensor_reg * sensor_regs_type_and_encode[CAM_TYPE_MAX][CAM_ENC_MAX] = {
	{
		lvds_des_reset,
	},
};
#endif

static int tcc_cif_i2c_write(unsigned char* data, unsigned short reg_bytes, unsigned short data_bytes) {
	unsigned short bytes = reg_bytes + data_bytes;
	int ret = 0;

//	log("[lvds] start \n");
	ret = i2c_xfer(LVDS_I2C_ADDR, bytes, data, 0, 0, LVDS_I2C_CH_NO);

	if(ret != 0)
	{
		printk("CM4 : write error!!!! \n");
		return -1; 
	}
	return 0;
}

static int write_regs(const struct sensor_reg reglist[]) {
	int err;
	int err_cnt = 0;
	unsigned char data[132];
	unsigned char bytes;
	const struct sensor_reg *next = reglist;

	log("[lvds] start \n");

	while (!((next->reg == 0xFF) && (next->val == 0xFF))) {
		if(next->reg == 0xFF && next->val != 0xFF) {
			mdelay(next->val);
			next++;
		} else {
			bytes = 0;
			data[bytes]= (unsigned char)next->reg&0xff; 	bytes++;
			data[bytes]= (unsigned char)next->val&0xff; 	bytes++;

			err = tcc_cif_i2c_write(data, 1, 1);
			if (err) {
				err_cnt++;
				if(err_cnt >= 3) {
					printk("CM4 : ERROR: Sensor I2C !!!! \n"); 
					return err;
				}
			} else {
				err_cnt = 0;
				next++;
			}
		}
	}
	return 0;
}

int lvds_tune(unsigned int camera_type, unsigned int camera_encode) {
	log("[lvds] start (camera_type : 0x%x camera_encode : 0x%x)\n", camera_type, camera_encode);
#if 0
	if(((CAM_TYPE_MAX <= camera_type) || (sensor_regs_type_and_encode[camera_type][camera_encode] == NULL)) ||
	   ((CAM_ENC_MAX <= camera_encode) || (sensor_regs_type_and_encode[camera_type][camera_encode] == NULL))) {
		printk("CM4 : %s - WRONG arguments\n", __func__);
		return -1;
	}
#endif
	return 0;//write_regs(sensor_regs_type_and_encode[camera_type][camera_encode]);
}


int lvds_open(int camera_type, int camera_encode) {
	int ret = 0, tmp;
	log("[lvds] start \n");

	return ret;
}

int lvds_close(void) {
	log("[lvds] start \n");
	return 0;
}

static int lvds_read_reg(unsigned char cmd, unsigned char *value)
{
	unsigned char send_data[1], read_data[1];

	log("[lvds] start (addr : 0x%x ch : 0x%x\n", LVDS_I2C_ADDR, LVDS_I2C_CH_NO);
	
	send_data[0] = cmd;
	read_data[0] = 0;

	i2c_xfer(LVDS_I2C_ADDR, 1, send_data, 1, read_data, LVDS_I2C_CH_NO);

	*value = read_data[0];

	return 0;
}

int lvds_video_check(void) {
	unsigned char val = 0;
	unsigned int gpio_b17 = 0;
	int ret, tmp = 0;

	log("[lvds] initializing check \n");
	
	while(1) {
		gpio_b17 = gpio_get(GPIO_PORTB|17);
		log("DES lock gpio : %d\n", gpio_b17);

		if((gpio_b17 == 0x01)) {
			if (tmp > 0) mdelay(100);
			ret = 0;
			break;
		}

		if(tmp >= 29) {
			printk("CM4 : fail to video signal check(%d). \n", tmp);
			ret = -1;
			tmp = 0;
			break;
		}
		else {
			log("%d times video signal checking... \n", tmp);
			mdelay(10);
			tmp++;
		}
	}
	return ret;
}

int lvds_sensor_connect_api(SENSOR_FUNC_TYPE * sensor_func) {
	dprintf(SPEW,"[%s] \n", __func__);
	sensor_func->close	= lvds_close;
	sensor_func->open	= lvds_open;
	sensor_func->tune	= lvds_tune;
	sensor_func->video_check = lvds_video_check;
	return 0;
}

