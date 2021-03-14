
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.

  This software is licensed under the terms of the GNU General Public
  License version 2, as published by the Free Software Foundation, and
  may be copied, distributed, and modified under those terms.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
****************************************************************************/
#ifndef _LCD_H_
#define	_LCD_H_

#include "tcc_lcd.h"

extern void tcc_rear_camera_display(unsigned char OnOff);
extern int tcc_lcd_image_ch_set(struct tcc_lcdc_image_update *ImageInfo);
extern void tcc_rear_camera_wmix_channel_prioty();

#endif

