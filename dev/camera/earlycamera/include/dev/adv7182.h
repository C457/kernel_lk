
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#ifndef __ADV7182_H__
#define __ADV7182_H__

#if defined(PLATFORM_TCC802X)
#define ADV7182_I2C_ADDR			0x40   //Module I2C Slave address
#define ADV7182_I2C_CH_NO			0	   // I2C Channel number
#else
#define ADV7182_I2C_ADDR			0x42   //Module I2C Slave address
#define ADV7182_I2C_CH_NO			1	   // I2C Channel number
#endif

#endif//__ADV7182_H__

