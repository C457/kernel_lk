
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#ifndef __LVDS_H__
#define __LVDS_H__

#define LVDS_I2C_ADDR				0x94	//Module I2C Slave address
#define CAMERA_I2C_CMD_ID			0x01	// DES Slave Address
#define CAMERA_I2C_CMD_DES_LOCK 		0x04	// b7 LOCKED
#define CAMERA_I2C_CMD_DES_RESET		0xFF	// ??
#define LVDS_I2C_CH_NO				3	// I2C Channel number

extern int lvds_sensor_connect_api(SENSOR_FUNC_TYPE * sensor_func);

#endif//__LVDS_H__

