
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#ifndef __TCC_LVDS_H
#define __TCC_LVDS_H

#define LVDS_I2C_ADDR 				0x94	//Module I2C Slave address , hklee
#define CAMERA_I2C_CMD_ID			0x01	//DES Slave Address Register
#define CAMERA_I2C_CMD_DES_LOCK 		0x04	//b7 LOCKED - 0:video link not locked, 1:video link locked
#define CAMERA_I2C_CMD_DES_RESET		0x01	//hklee, ?
#define LVDS_I2C_CH_NO				3	// I2C Channel number

static struct sensor_gpio
{
	int pwr_port;  // setting for power control pin
	int pwd_port;  // setting for stand by control pin
	int rst_port;  // setting for reset control pin

	int data_func;	// settung fot gpio function(data & sync)
	int data[25];  // max data pin count(24pin) + 1(for NULL pointer)
	int hsync;	   // setting for H - Sync polarity
	int vsync;	   // setting for V - Sync polarity
	int pclk;	   // Settung for P Clocks(CLKI) Phase

};

static struct sensor_reg {
	unsigned short reg;
	unsigned short val;
};

extern int sensor_if_connect_api(SENSOR_FUNC_TYPE * sensor_func);

#endif

