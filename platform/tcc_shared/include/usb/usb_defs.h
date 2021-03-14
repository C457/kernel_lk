/*
 * Copyright (c) 2010 Telechips, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the 
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _USB_DEFS_H
#define _USB_DEFS_H

#if defined(TCC893X)
#define HwPIC						((PPIC)&(HwPIC_BASE))
#define HwUSBPHYCFG					((PUSBPHYCFG)&(HwUSBPHYCFG_BASE))
#define HwUSB20OTG					((PUSB20OTG)&(HwUSB20OTG_BASE))
#elif TCC897X
#else
typedef struct _USBPHYCFG
{
	volatile unsigned	UPCR0;		// 0x0A4  R/W    USB PHY Configuration Register0
	volatile unsigned	UPCR1;		// 0x0A8  R/W    USB PHY Configuration Register1
	volatile unsigned	UPCR2;		// 0x0AC  R/W    USB PHY Configuration Register2
	volatile unsigned	UPCR3;		// 0x0B0  R/W    USB PHY Configuration Register3
	volatile unsigned	UPCR4;		// 0x0B4  R/W    USB PHY Configuration Register3
	volatile unsigned	UPCR5;		// 0x0B8  R/W    USB PHY Configuration Register3
	volatile unsigned	LCFG;		// 0x0BC  R/W    USB PHY Configuration Register3
} *PUSBPHYCFG;
#warning
#endif

#ifndef TCC_USB_30_USE
#define USBPHY_INCLUDE
#define OTGCORE_INCLUDE
#define OTGDEV_IO_INCLUDE
#else
#define DWCCORE_INCLUDE
#define USB30DEV_IO_INCLUDE
#endif

#define USBDEV_VTC_INCLUDE
//#define USBDEV_MSC_INCLUDE
#define USBDEV_FASTBOOT_INCLUDE

//===================================================================
//
//		DRIVER SIGNATURE
//
//===================================================================
#define SIGBYAHONG			'S','I','G','B','Y','A','H','O','N','G','_'
#define SIGN_OS				'L','I','N','U','X','_'

#if defined(TCC893X)
#define SIGN_CHIPSET			'T','C','C','8','9','3','X','_'
#elif defined(TCC896X)
#define SIGN_CHIPSET			'T','C','C','8','9','6','X','_'
#elif defined(TCC897X)
#define SIGN_CHIPSET			'T','C','C','8','9','7','X','_'
#else
#error
#endif

#ifndef NULL
#define NULL				0
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef TRUE
#define TRUE				1
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef dim
#define dim(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef BITSET
#define BITSET(X, MASK) 			( (X) |= (unsigned int)(MASK) )
#endif
#ifndef BITSCLR
#define BITSCLR(X, SMASK, CMASK)	( (X) = ((((unsigned int)(X)) | ((unsigned int)(SMASK))) & ~((unsigned int)(CMASK))) )
#endif
#ifndef BITCSET
#define BITCSET(X, CMASK, SMASK)	( (X) = ((((unsigned int)(X)) & ~((unsigned int)(CMASK))) | ((unsigned int)(SMASK))) )
#endif
#ifndef BITCLR
#define BITCLR(X, MASK) 			( (X) &= ~((unsigned int)(MASK)) )
#endif
#ifndef BITXOR
#define BITXOR(X, MASK) 			( (X) ^= (unsigned int)(MASK) )
#endif
#ifndef ISZERO
#define ISZERO(X, MASK) 			(  ! (((unsigned int)(X)) & ((unsigned int)(MASK))) )
#endif

extern int uart_send_printf(const char *str , ... );
//#define USBDEV_DEBUG(fmt,args...)		uart_send_printf(fmt,##args)
#define USBDEV_DEBUG(fmt,...)
//#define USBDEV_ERROR(fmt,args...)		uart_send_printf(fmt,##args)
#define USBDEV_ERROR(fmt,...)

#endif //_USB_DEFS_H
