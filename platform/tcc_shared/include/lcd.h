/***************************************************************************************
*	FileName    : lcd.h
*	Description : lcd Functions
****************************************************************************************
*
*	TCC Board Support Package
*	Copyright (c) Telechips, Inc.
*	ALL RIGHTS RESERVED
*
****************************************************************************************/
#ifndef __LCD_H__
#define __LCD_H__

#if defined(TCC_HDMI_USE)
	#include <lcd/HDMI_TCC.h>
#endif//TCC_USE_HDMI

#if defined(DEFAULT_DISPLAY_LCD)
	#if defined(ED090NA) || defined(FLD0800)
		#define BOOT_USE_LVDS
	#else
		#define BOOT_USE_LCD
	#endif
#endif

#if defined(DEFAULT_DISPLAY_HDMI)
#define BOOT_USE_HDMI
#endif//

#endif	/*__LCD_H__*/
