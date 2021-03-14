
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <reg.h>
#include <platform/iomap.h>
#include <dev/fbcon.h>
#include <plat/cpu.h>

#include <platform/irqs.h>
#include <platform/tcc_ckc.h>

#include <i2c.h>
#include <dev/gpio.h>
#include <tcc_lcd.h>
#include <platform/gpio.h>
#include <platform/vioc_cam_plugin.h>

#include <dev/camera/camera.h>
#include <dev/camera/sensor_if.h>
#include <dev/camera/lvds.h>
#include <daudio_settings.h>

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

static void lvds_set_gpio_pin(void) {
	int idxData;

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
		dprintf(INFO,"gpio pin is %d\n", gpio_info.data[idxData]);
		gpio_config(gpio_info.data[idxData], gpio_info.data_func);
	}

	volatile GPION *gpiod = (volatile GPION*)(HwGPIOD_BASE);

	gpiod->GPIS.nREG |= 0x7FF;

	// Power-up.
//	gpio_set(gpio_info.pwr_port, 1);
//	gpio_set(gpio_info.pwd_port, 1);
//	gpio_set(gpio_info.rst_port, 0);

	dprintf(INFO, "!@#---- %s() : set tw9990 rst gpio inactive \n", __func__);
	gpio_config(TCC_GPG(16), GPIO_OUTPUT);
	gpio_set(TCC_GPG(16), 1);  // sensor_reset_high
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

struct tcc_cif_parameters lvds_parameters_data = {
	//2017.12.22 - LVDS SVM Display Timing Fixed. PCLK(Positive Edge) HS(Active High), VS(Active High)
	//Because of setup time issue, AVN detects PCLK at negative edge
	.Cam_p_clock_pol = NEGATIVE_EDGE, //POSITIVE_EDGE,
	.Cam_v_sync_pol = ACT_HIGH,
	.Cam_h_sync_pol = ACT_HIGH,
	.Cam_de_pol = ACT_LOW,
	.Cam_field_bfield_low = OFF,
	.Cam_gen_field_en = ON,
	.Cam_conv_en = OFF,
	.Cam_hsde_connect_en = ON,
	.Cam_vs_mask = ON,
	.Cam_input_fmt = FMT_YUV422_8BIT,
	.Cam_data_order = ORDER_RGB,
	.Cam_intl_en = OFF,
	.Cam_intpl_en = OFF,
	.Cam_preview_w = 1920,
	.Cam_preview_h = 720,

//	.Lcdc_Frame_width = 800,//1024,
//	.Lcdc_Frame_height = 480,//600,
//	.Lcdc_Image_width = 800,//1024,
//	.Lcdc_Image_height = 480,//600,
//	.Lcdc_offset_x = 0,
//	.Lcdc_offset_y = 0,

	.Viqe_area = 0,
	.PGL_addr = 0,
	.Lcdc_address0 = 0,

	.Lcdc_address1 = (unsigned int)NULL,
	.Lcdc_address2 = (unsigned int)NULL,
//	.Lcdc_format = TCC_LCDC_IMG_FMT_RGB888_3,

	.CIF_Port_num = 0,

	.viocmg_info = {
		.main_display_id = 0,
//		.main_display_port
		.main_display_ovp = 24,
		.feature_early_view_enable = 1,
		.feature_early_view_use_viqe = 0,
		.feature_early_view_viqe_mode = 0,
		.feature_early_view_use_parking_line = 0,
		.early_cam_cifport = 0,
		.early_cam_vin_vin = 0,
		.early_cam_vin_rdma = 16,
		.early_cam_vin_wmix = 5,
		.early_cam_vin_wdma = 5,
		.early_cam_vin_scaler = 0,
		.early_cam_display_rdma = 1,
		.early_cam_gear_port = TCC_GPE(8),
		.early_cam_gear_port_active_level = 1,
		.early_cam_ovp = 19,
		.early_cam_preview_x = 0,
		.early_cam_preview_y = 0,

		.early_cam_preview_width = 0,
		.early_cam_preview_height = 0,
		.early_cam_preview_format = TCC_LCDC_IMG_FMT_RGB888,

		.early_cam_preview_additional_width = 0,
		.early_cam_preview_additional_height = 0,

		.early_cam_preview_crop_x = 0,
		.early_cam_preview_crop_y = 0,

		.early_cam_parking_line_x = 0,
		.early_cam_parking_line_y = 0,

		.early_cam_parking_line_width = 0,
		.early_cam_parking_line_height = 0,
		.early_cam_parking_line_format = TCC_LCDC_IMG_FMT_RGB888,
	},

	.Cam_brightness = 127,
	.Cam_contrast = 127,
	.Cam_saturation = 127
};

static int tcc_cif_i2c_write(unsigned char* data, unsigned short reg_bytes, unsigned short data_bytes) {
	unsigned short bytes = reg_bytes + data_bytes;
	int ret = 0;
	ret = i2c_xfer(LVDS_I2C_ADDR, bytes, data, 0, 0, LVDS_I2C_CH_NO);

	if(ret != 0)
	{
		dprintf(INFO,"write error!!!! \n");
		return -1;
	}
	return 0;
}

static int tcc_cif_i2c_read(unsigned char cmd, unsigned char *value)
{
	unsigned char send_data[1], read_data[1];
	int ret;

	dprintf(INFO,"tcc_cif_i2c_read() - start. \n");

	send_data[0] = cmd;
	read_data[0] = 0;

	ret = i2c_xfer(LVDS_I2C_ADDR, 1, send_data, 1, read_data, LVDS_I2C_CH_NO);
	if(ret != 0)
	{
		dprintf(INFO,"read error!!!! \n");
		return -1;
	}

	*value = read_data[0];

	dprintf(INFO,"tcc_cif_i2c_read() - end. \n");
}

static int write_regs(const struct sensor_reg reglist[]) {
	int err;
	int err_cnt = 0;
	unsigned char data[132];
	unsigned char bytes;
	const struct sensor_reg *next = reglist;

	while (!((next->reg == 0xFF) && (next->val == 0xFF))) {
		if(next->reg == 0xFF) {
			thread_sleep(next->val);
//			mdelay(next->val);
			next++;
		} else {
			bytes = 0;
			data[bytes]= (unsigned char)next->reg&0xff; 	bytes++;
			data[bytes]= (unsigned char)next->val&0xff; 	bytes++;

			err = tcc_cif_i2c_write(data, 1, 1);
			if (err) {
				err_cnt++;
				if(err_cnt >= 3) {
					dprintf(INFO,"ERROR: Sensor I2C !!!! \n");
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
	struct ie_setting_info info_ie_read;

	dprintf(INFO, "Default@#---- %s() - lut_cam_brightness=%d,lut_cam_contrast=%d,lut_cam_saturation=%d\n", __func__, lvds_parameters_data.Cam_brightness, lvds_parameters_data.Cam_contrast, lvds_parameters_data.Cam_saturation);

	read_ie_setting(&info_ie_read);

	//Default(127) +-25, min(102), max(152)
	if(info_ie_read.twxxxx_cam_brightness >= 102 && info_ie_read.twxxxx_cam_brightness <= 152)
		lvds_parameters_data.Cam_brightness = info_ie_read.twxxxx_cam_brightness;
	if(info_ie_read.twxxxx_cam_contrast >= 102 && info_ie_read.twxxxx_cam_contrast <= 152)
		lvds_parameters_data.Cam_contrast = info_ie_read.twxxxx_cam_contrast;
	if(info_ie_read.twxxxx_cam_saturation >= 102 && info_ie_read.twxxxx_cam_saturation <= 152)
		lvds_parameters_data.Cam_saturation = info_ie_read.twxxxx_cam_saturation;

	dprintf(INFO, "UserSetting@#---- %s() - lut_cam_brightness=%d,lut_cam_contrast=%d,lut_cam_saturation=%d\n", __func__, info_ie_read.twxxxx_cam_brightness, info_ie_read.twxxxx_cam_contrast, info_ie_read.twxxxx_cam_saturation);

	return 0;
}

int lvds_open(void) {
	unsigned char val[2] = {0};
	unsigned char addr = 0;
	unsigned int gpio_g14 = 0;
	int ret;
	dprintf(INFO, "!@#---- %s()\n", __func__);

	lvds_set_gpio_pin();
	tcc_cif_i2c_read(CAMERA_I2C_CMD_ID , &addr);	//hklee
	dprintf(INFO,"[LVDS] DES device ID 0x%x \n", addr);

#if 0
		val[0] = 0x07;		//config register : 0x07
		val[1] = 0;
		tcc_cif_i2c_read(val[0], &val[1]);
		dprintf(INFO,"[LVDS] DES config default val =0x%x\n", val[1]);
//		val[1] |= 0x8C;		//bit7 0->1, bit2 0->1
		val[1] |= 0x84;		//bit7 0->1, bit2 0->1
//		val[1] = 0x85;		//bit7 0->1, bit2 0->1
		tcc_cif_i2c_write(val, 1, 1);
		tcc_cif_i2c_read(val[0], &val[1]);
		dprintf(INFO,"[LVDS] DES config new val =0x%x\n", val[1]);

		val[0] = 0x05;		//eqtune register : 0x05
		val[1] = 0;
		tcc_cif_i2c_read(val[0], &val[1]);
		dprintf(INFO,"[LVDS] DES eqtune default val =0x%x\n", val[1]);
		val[1] |= 0x40;		//bit6 0->1
		tcc_cif_i2c_write(val, 1, 1);
		tcc_cif_i2c_read(val[0], &val[1]);
		dprintf(INFO,"[LVDS] DES eqtune new val =0x%x\n", val[1]);

/*		gpio_g14 = gpio_get(GPIO_PORTG|14);
		tcc_cif_i2c_read(CAMERA_I2C_CMD_DES_LOCK , &val );
		dprintf(INFO,"[LVDS] DES reset gpio : %d, DES general status(Lock) val =%d \n", gpio_g14, val);
		if((gpio_g14 == 0x00) && ((val & 0x01) == 0x00)) {
			dprintf(INFO,"[LVDS] val 0x%x \n", val);
			val = 0x01;
			ret = write_regs(lvds_des_reset);
		}*/
#endif
	return lvds_tune(0,0);
}

int lvds_close(void) {
	dprintf(INFO, "!@#---- %s()\n", __func__);
	return 0;
}

int lvds_if_connect_api(SENSOR_FUNC_TYPE * sensor_func) {
	sensor_func->close	= lvds_close;
	sensor_func->open	= lvds_open;
	sensor_func->tune	= lvds_tune;

	return 0;
}

