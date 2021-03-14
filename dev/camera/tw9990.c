
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
#include <dev/camera/tw9990.h>
#include <daudio_ver.h>
#include <daudio_settings.h>

struct sensor_gpio gpio_info = {
	.pwr_port = (int)NULL,
	.pwd_port = (int)NULL,
	.rst_port = TCC_GPG(16),

	.vsync = 0,//TCC_GPF(2),
	.hsync = 0,//TCC_GPF(1),
	.pclk  = TCC_GPF(0),

	.data = {TCC_GPF(3), TCC_GPF(4), TCC_GPF(5), TCC_GPF(6), TCC_GPF(7), TCC_GPF(8), TCC_GPF(9), TCC_GPF(10)},

	.data_func = GPIO_FN1
};

static struct sensor_reg cvbs_composite[] = {
	//cmd, value
    {0x01, 0x78},
    {0x02, 0x40},
    {0x03, 0xA0},
    {0x04, 0x00},
    {0x05, 0x00},
    {0x06, 0x00},
    {0x1C, 0x08},
    {0x07, 0x02},
    {0x08, 0x12},
    {0x09, 0xF0},
    {0x0A, 0x10},
    {0x0B, 0xD0},
    {0x0C, 0xCC},
    {0x0D, 0x00},
    {0x0F, 0x00},
#if 0  //ie settig   
    {0x10, 0x00},
    {0x11, 0x64},
    {0x12, 0x21},
    {0x13, 0x80},
    {0x14, 0x80},
    {0x15, 0x00},
#endif    
    {0x12, 0x21},
    {0x16, 0x00},
    {0x17, 0x30},
    {0x18, 0x44},
    {0x19, 0x58},
    {0x1A, 0x0A},
    {0x1B, 0x00},
    {0x1D, 0x7F},
    {0x1E, 0x08},
    {0x1F, 0x00},
    {0x20, 0xA0},
    {0x21, 0x42},
    {0x22, 0xF0},
    {0x23, 0xD8},
    {0x24, 0xBC},
    {0x25, 0xB8},
    {0x26, 0x44},
    {0x27, 0x2A},
    {0x28, 0x00},
    {0x29, 0x00},
    {0x2A, 0x78},
    {0x2B, 0x44},
    {0x2C, 0x40},
    {0x2D, 0x14},
    {0x2E, 0xA5},
    {0x2F, 0xE0},
    {0x30, 0x00},
    {0x31, 0x10},
    {0x32, 0xFF},
    {0x33, 0x05},
    {0x34, 0x1A},
    {0x35, 0xA0},
    {0x4C, 0x05},
    {0x4D, 0x40},
    {0x4E, 0x00},
    {0x4F, 0x00},
    {0x50, 0xA0},
    {0x51, 0x22},
    {0x52, 0x31},
    {0x53, 0x80},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x6B, 0x06},
    {0x6C, 0x24},
    {0x6D, 0x00},
    {0x6E, 0x20},
    {0x6F, 0x13},
    {0xAE, 0x1A},
    {0xAF, 0x80},
	{0xFF, 0xFF}
};

static struct sensor_reg cvbs_component[] = {
	{0xFF, 0xFF}
};

static struct sensor_reg s_video[] = {
    {0x01, 0x78},
    {0x02, 0x50},
    {0x03, 0xA0},
    {0x04, 0x00},
    {0x05, 0x00},
    {0x06, 0x00},
    {0x07, 0x02},
    {0x08, 0x12},
    {0x09, 0xF0},
    {0x0A, 0x10},
    {0x0B, 0xD0},
    {0x0C, 0xCC},
    {0x0D, 0x00},
    {0x0F, 0x00},
#if 0  //ie settig   
    {0x10, 0x00},
    {0x11, 0x64},
    {0x12, 0x21},
    {0x13, 0x80},
    {0x14, 0x80},
    {0x15, 0x00},
#endif 
    {0x12, 0x21},
    {0x16, 0x00},
    {0x17, 0x30},
    {0x18, 0x44},
    {0x19, 0x58},
    {0x1A, 0x0A},
    {0x1B, 0x00},
    {0x1C, 0x07},
    {0x1D, 0x7F},
    {0x1E, 0x08},
    {0x1F, 0x00},
    {0x20, 0xA0},
    {0x21, 0x42},
    {0x22, 0xF0},
    {0x23, 0xD8},
    {0x24, 0xBC},
    {0x25, 0xB8},
    {0x26, 0x44},
    {0x27, 0x2A},
    {0x28, 0x00},
    {0x29, 0x00},
    {0x2A, 0x78},
    {0x2B, 0x44},
    {0x2C, 0x40},
    {0x2D, 0x14},
    {0x2E, 0xA5},
    {0x2F, 0xE0},
    {0x30, 0x00},
    {0x31, 0x10},
    {0x32, 0xFF},
    {0x33, 0x05},
    {0x34, 0x1A},
    {0x35, 0xA0},
    {0x4C, 0x05},
    {0x4D, 0x40},
    {0x4E, 0x00},
    {0x4F, 0x00},
    {0x50, 0xA0},
    {0x51, 0x22},
    {0x52, 0x31},
    {0x53, 0x80},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x6B, 0x06},
    {0x6C, 0x24},
    {0x6D, 0x00},
    {0x6E, 0x20},
    {0x6F, 0x13},
    {0xAE, 0x1A},
    {0xAF, 0x80},
	{0xFF, 0xFF}
};

static struct sensor_reg sensor_regs_stop[] = {
	{0x03, 0xA7},
	{0xFF, 0xFF}
};

static struct sensor_reg sensor_regs_ie[] = {
    {0x10, 0x00},
    {0x11, 0x64},
    {0x13, 0x80},
    {0x14, 0x80},
    {0xFF, 0xFF}
};


struct sensor_reg * sensor_regs_type_and_encode[CAM_TYPE_MAX][CAM_ENC_MAX] = {
	// CAM_TYPE_DEFAULT
	{
		cvbs_composite, 	//cvbs_ntsc,
	},
	{
		s_video,			//svideo_ntsc,
	},
	{
		cvbs_component,		//component_ntsc,
	},
};

void tcc_set_gpio_pin(void) {
	int idxData;
	gpio_info.rst_port = TCC_GPG(16);
	// Configure power.
	gpio_config(gpio_info.pwr_port, GPIO_OUTPUT);
	gpio_config(gpio_info.pwd_port, GPIO_OUTPUT);
	gpio_config(gpio_info.rst_port, GPIO_OUTPUT);

	// Configure signal port.
	gpio_config(gpio_info.hsync, gpio_info.data_func);
	gpio_config(gpio_info.vsync, gpio_info.data_func);
	gpio_config(gpio_info.pclk,  gpio_info.data_func);

	// Configue data port.
	for(idxData=0; gpio_info.data[idxData] != (int)NULL; idxData++) {
		//dprintf(INFO,"gpio pin is %d\n", gpio_info.data[idxData]);		
		gpio_config(gpio_info.data[idxData], gpio_info.data_func);
	}

	// Power-up.
	gpio_set(gpio_info.pwr_port, 1);
	gpio_set(gpio_info.pwd_port, 1);
	gpio_set(gpio_info.rst_port, 0);
}

struct tcc_cif_parameters atv_parameters_data = {
	.Cam_p_clock_pol = NEGATIVE_EDGE,
	.Cam_v_sync_pol = ACT_HIGH,
	.Cam_h_sync_pol = ACT_HIGH,
	.Cam_de_pol = ACT_LOW,
	.Cam_field_bfield_low = OFF,
	.Cam_gen_field_en = OFF,
	.Cam_conv_en = ON,
	.Cam_hsde_connect_en = OFF,
	.Cam_vs_mask = OFF,
	.Cam_input_fmt = FMT_YUV422_8BIT,
	.Cam_data_order = ORDER_RGB,
	.Cam_intl_en = ON,
	.Cam_intpl_en = OFF,
	.Cam_preview_w = 720,
	.Cam_preview_h = 240,
	
//	.Lcdc_Frame_width = 800,//1024,
//	.Lcdc_Frame_height = 480,//600,
//	.Lcdc_Image_width = 800,//1024,
//	.Lcdc_Image_height = 480,//600,
//	.Lcdc_offset_x = 0,
//	.Lcdc_offset_y = 0,

	.Viqe_area =  0,
	.PGL_addr = 0,
	.Lcdc_address0 = 0,

	.Lcdc_address1 = (unsigned int)NULL,
	.Lcdc_address2 = (unsigned int)NULL,
//	.Lcdc_format = TCC_LCDC_IMG_FMT_RGB888_3,

	.CIF_Port_num = 4,

	.viocmg_info = {
//	.main_display_port
	.main_display_ovp = 24,
	.feature_early_view_enable = 1,
	.feature_early_view_use_viqe = 1,
	.feature_early_view_viqe_mode = 0,
#if defined (CONFIG_TCC_PARKING_GUIDE_LINE)
	.feature_early_view_use_parking_line = 1,
#else
	.feature_early_view_use_parking_line = 0,
#endif
	.early_cam_cifport = 4,
	.early_cam_vin_vin = 0,
	.early_cam_vin_rdma = 16,
	.early_cam_vin_wmix = 5,
	.early_cam_vin_wdma = 5,
	.early_cam_vin_scaler = 0,
#if defined(WIDE_LCD_DEFINE)
	.main_display_id = 0,
	.early_cam_display_rdma = 1,
#else
	.main_display_id = 1,
	.early_cam_display_rdma = 5,
#endif
	.early_cam_gear_port = TCC_GPE(8),
	.early_cam_gear_port_active_level = 1,
	.early_cam_ovp = 19,
	.early_cam_preview_x = 0,
	.early_cam_preview_y = 0,
	.early_cam_preview_width = 0,
	.early_cam_preview_height = 0,
	.early_cam_preview_format = TCC_LCDC_IMG_FMT_RGB888,
	.early_cam_preview_additional_width = 0,
	.early_cam_preview_additional_height = 7,
	.early_cam_preview_crop_x = 0,
	.early_cam_preview_crop_y = 7,
	.early_cam_parking_line_x = 0,
	.early_cam_parking_line_y = 0,
	.early_cam_parking_line_width = 0,
	.early_cam_parking_line_height = 0,
	.early_cam_parking_line_format = TCC_LCDC_IMG_FMT_RGB888,
	}
};

static int tcc_cif_i2c_read(unsigned char cmd, unsigned char *recv_data){
    int ret = -1;
    
    ret = i2c_xfer(TW9990_I2C_ADDR, 1, &cmd, 1, recv_data, TW9990_I2C_CH_NO);
    if(ret != 0)
        dprintf(INFO,"read error!!!! \n");
    else
        dprintf(INFO,"cmd : 0x%02x, val : 0x%02x\n", cmd, *recv_data);

    return ret;
}

static int tcc_cif_i2c_write(unsigned char* data, unsigned short reg_bytes, unsigned short data_bytes) {
	unsigned short bytes = reg_bytes + data_bytes;
	int ret = 0;
    unsigned char read_data = 0;
	ret = i2c_xfer(TW9990_I2C_ADDR, bytes, data, 0, 0, TW9990_I2C_CH_NO);

	if(ret != 0)
	{
		dprintf(INFO,"write error!!!! \n");
		ret = -1;
	}

	//tcc_cif_i2c_read(data[0], &read_data);

	return ret;
}

static int write_regs(const struct sensor_reg reglist[]) {
	int err;
	int err_cnt = 0;
	int sleep_cnt = 100;		
	unsigned char data[132];
	unsigned char bytes;
	const struct sensor_reg *next = reglist;

	//dprintf(INFO, "start \n");

	while (!((next->reg == 0xFF) && (next->val == 0xFF))) {
		if(next->reg == 0xFF) {
			thread_sleep(next->val);
//			mdelay(next->val);
			sleep_cnt = 100;
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

int sensor_tune(unsigned int camera_type, unsigned int camera_encode) {
	struct ie_setting_info info_ie_read;

	//dprintf(INFO, "!@#---- %s()\n", __func__);

	if(((CAM_TYPE_MAX <= camera_type) || (sensor_regs_type_and_encode[camera_type][camera_encode] == NULL)) || 
	   ((CAM_ENC_MAX <= camera_encode) || (sensor_regs_type_and_encode[camera_type][camera_encode] == NULL))) {
		dprintf(INFO, "!@#---- %s() - WRONG arguments\n", __func__);
		return -1;
	}

	dprintf(INFO, "Default@#---- %s() - twxxxx_cam_brightness=%d,twxxxx_cam_contrast=%d,twxxxx_cam_saturationu=%d\n", __func__,(char)sensor_regs_ie[0].val + 128 ,sensor_regs_ie[1].val ,sensor_regs_ie[2].val );

	read_ie_setting(&info_ie_read);
	
	//brightness : default(128) +-25, min(103), max(153)
	if(info_ie_read.twxxxx_cam_brightness >= 103 && info_ie_read.twxxxx_cam_brightness <= 153)
		sensor_regs_ie[0].val = info_ie_read.twxxxx_cam_brightness - 128;

	//contrast : default(100) +-25, min(75), max(125)
	if(info_ie_read.twxxxx_cam_contrast >= 75 && info_ie_read.twxxxx_cam_contrast <= 125)
		sensor_regs_ie[1].val = info_ie_read.twxxxx_cam_contrast;

	//saturation : default(128) +-25, min(103), max(153)
	if(info_ie_read.twxxxx_cam_saturation >= 103 && info_ie_read.twxxxx_cam_saturation <= 153)
	{
		sensor_regs_ie[2].val = info_ie_read.twxxxx_cam_saturation;
		sensor_regs_ie[3].val = info_ie_read.twxxxx_cam_saturation;
	}

	/*sensor_regs_ie[4].val = info_ie_read.twxxxx_cam_hue;*/

	write_regs(sensor_regs_ie);

	dprintf(INFO, "UserSetting@#---- %s() - twxxxx_cam_brightness=%d,twxxxx_cam_contrast=%d,twxxxx_cam_saturationu=%d\n", __func__, info_ie_read.twxxxx_cam_brightness, info_ie_read.twxxxx_cam_contrast, info_ie_read.twxxxx_cam_saturation);
	
	return write_regs(sensor_regs_type_and_encode[camera_type][camera_encode]);
}

int sensor_open(int camera_type, int camera_encode) {
	//dprintf(INFO, "!@#---- %s()\n", __func__);

	tcc_set_gpio_pin();
	
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

int sensor_close(void) {
	//dprintf(INFO, "!@#---- %s()\n", __func__);
	return 0;
}

int sensor_if_connect_api(SENSOR_FUNC_TYPE * sensor_func) {
	sensor_func->close	= sensor_close;
	sensor_func->open	= sensor_open;
	sensor_func->tune	= sensor_tune;

	return 0;
}

