/*
 * USB PHY (UTMI+) driver source code
 *
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

#include <common.h>

#include <asm/arch/usb/usb_defs.h>
#include <asm/arch/usb/usbphy.h>
#include <asm/arch/iomap.h>
#include <asm/arch/usb/reg_physical.h>
#include <asm/arch/tcc_ckc.h>
#include <asm/arch/usb/otgregs.h>

/* For Signature */
#define USBPHY_SIGNATURE			'U','S','B','P','H','Y','_'
#define USBPHY_VERSION				'V','2','.','0','0','0'
//static const unsigned char USBPHY_C_Version[] = {SIGBYAHONG, USBPHY_SIGNATURE, SIGN_OS ,SIGN_CHIPSET, USBPHY_VERSION, NULL};

#define UPCR0_PR						Hw14					// (1:enable)/(0:disable) Port Reset
#define UPCR0_CM_EN						Hw13					// Common Block Power Down Enable
#define UPCR0_CM_DIS					~Hw13					// Common Block Power Down Disable
#define UPCR0_RCS_11					(Hw12+Hw11)				// Reference Clock Select for PLL Block ; The PLL uses CLKCORE as reference
#define UPCR0_RCS_10					Hw12					// Reference Clock Select for PLL Block ; The PLL uses CLKCORE as reference
#define UPCR0_RCS_01					Hw11					// Reference Clock Select for PLL Block ; The XO block uses an external clock supplied on the XO pin
#define UPCR0_RCS_00					((~Hw12)&(~Hw11))		// Reference Clock Select for PLL Block ; The XO block uses the clock from a crystal
#define UPCR0_RCD_48					Hw10					// Reference Clock Frequency Select ; 48MHz
#define UPCR0_RCD_24					Hw9						// Reference Clock Frequency Select ; 24MHz
#define UPCR0_RCD_12					((~Hw10)&(~Hw9))		// Reference Clock Frequency Select ; 12MHz
#define UPCR0_SDI_EN					Hw8						// IDDQ Test Enable ; The analog blocks are powered down
#define UPCR0_SDI_DIS					~Hw8					// IDDQ Test Disable ; The analog blocks are not powered down
#define UPCR0_FO_SI						Hw7						// UTMI/Serial Interface Select ; Serial Interface
#define UPCR0_FO_UTMI					~Hw7					// UTMI/Serial Interface Select ; UTMI
#define UPCR0_VBDS						Hw6
#define UPCR0_DMPD						Hw5						// 1:enable / 0:disable the pull-down resistance on D-
#define UPCR0_DPPD						Hw4						// 1: enable, 0:disable the pull-down resistance on D+
#define UPCR0_VBD						Hw1						// The VBUS signal is (1:valid)/(0:invalid), and the pull-up resistor on D+ is (1:enabled)/(0:disabled)

#define UPCR1_TXFSLST(x)				((x&0xF)<<12)
#define UPCR1_SQRXT(x)					((x&0x7)<<8)
#define UPCR1_OTGT(x)					((x&0x7)<<4)
#define UPCR1_CDT(x)					(x&0x7)

#define UPCR2_TM_FS						Hw14
#define UPCR2_TM_HS						0
#define UPCR2_XCVRSEL_LS_ON_FS			(Hw13|Hw12)
#define UPCR2_XCVRSEL_LS				Hw13
#define UPCR2_XCVRSEL_FS				Hw12
#define UPCR2_XCVRSEL_HS				0
#define UPCR2_OPMODE_MASK				(Hw10|Hw9)
#define UPCR2_OPMODE_SYNC_EOP			(Hw10|Hw9)
#define UPCR2_OPMODE_DIS_BITS_NRZI		Hw10
#define UPCR2_OPMODE_NON_DRVING			Hw9
#define UPCR2_OPMODE_NORMAL				0
#define UPCR2_TXVRT(x)					((x&0xF)<<5)
#define UPCR2_TXRT(x)					((x&0x3)<<2)
#define UPCR2_TP_EN						Hw0
#define UPCR2_TP_DIS					0


static void USBPHY_Delay_100ms(void)
{
	mdelay(100);
}

void USBPHY_DEVICE_Detach(void)
{
	PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) TCC_USBPHYCFG_BASE;

	BITCSET(USBPHYCFG->UPCR2,UPCR2_OPMODE_MASK,UPCR2_OPMODE_NON_DRVING);
#if defined(TCC893X)
	USBPHYCFG->UPCR0 = 0x4840 | Hw9;
#else
	USBPHYCFG->UPCR0 = 0x4840;
#endif
	USBPHY_Delay_100ms();
}

void USBPHY_DEVICE_Attach(void)
{
	PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) TCC_USBPHYCFG_BASE;

	BITCSET(USBPHYCFG->UPCR2,UPCR2_OPMODE_MASK,UPCR2_OPMODE_NORMAL);
#if defined(TCC893X)
	USBPHYCFG->UPCR0 = 0x2842 | Hw9;
#else
	USBPHYCFG->UPCR0 = 0x2842;
#endif
}

void USBPHY_EnableClock(void)
{
	PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) TCC_USBPHYCFG_BASE;

	// VBDS=1 for the VBUSVLDEXT input is used. (not internal session valid comparator)
	// If not, D+ will be pulled up even at host mode.
	BITSET(USBPHYCFG->UPCR0,Hw6);
	// MODE=0 for USBOTG, SDI=0 for turn on all analog blocks, VBD=0 for disable the pull-up resister on D+
	BITCLR(USBPHYCFG->UPCR0,Hw15|Hw8|Hw1);
}

void USBPHY_Disable(void)
{
	PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) TCC_USBPHYCFG_BASE;

	// MODE=0 for USBOTG, SDI=0 for turn on all analog blocks, VBD=0 for disable the pull-up resister on D+
	BITSET(USBPHYCFG->UPCR0,Hw8);
}

#if defined(CONFIG_TCC897X) || defined(CONFIG_TCC898X)
void wait_phy_reset_timing (unsigned int count)
{
    unsigned int i;
    volatile unsigned int tmp;

    // Wait 10us
    tmp = 0;
    for (i=0;i<count * 100;i++)
        tmp++;
}
//#define dwc_msleep(dev, ms)	mdelay(ms)
#define dwc_msleep(ms)	wait_phy_reset_timing(ms)
#endif /* TCC897X */

void USBPHY_SetMode(USBPHY_MODE_T mode)
{
	PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) TCC_USBPHYCFG_BASE;
#if defined(CONFIG_TCC897X) || defined(CONFIG_TCC898X)
	if ( mode == USBPHY_MODE_RESET )
	{
		USBPHYCFG->UPCR0 = 0x03000015;
		USBPHYCFG->UPCR1 = 0x0334D543;
//		USBPHYCFG->UPCR2 = 0xC0;
		USBPHYCFG->UPCR2 = 0x4;
		USBPHYCFG->UPCR3 = 0x0;
		USBPHYCFG->UPCR4 = 0x0;
		USBPHYCFG->LCFG  = 0x30000000;
		
		tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 1);
		
		USBPHYCFG->LCFG = 0x0;							// assert prstn, adp_reset_n
		BITCLR(USBPHYCFG->UPCR0,(Hw20|Hw24|Hw31)); 		//disable PHYVALID_EN -> no irq, SIDDQ, POR
		dwc_msleep(1);
		BITSET(USBPHYCFG->LCFG, Hw29);					// prstn
		
		tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 0);		
	}
	else if ( mode == USBPHY_MODE_DEVICE )
	{		
#if 0		/* 015.06.23 */
		USBPHYCFG->UPCR0 = 0x03000015;
		USBPHYCFG->UPCR1 = 0x0334D543;
		USBPHYCFG->UPCR2 = 0x4;
		USBPHYCFG->UPCR3 = 0x0;
		USBPHYCFG->UPCR4 = 0x0;
		USBPHYCFG->LCFG  = 0x30000000;
		
		tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 1);

		USBPHYCFG->LCFG = 0x0;							// assert prstn, adp_reset_n
		//BITSET(USBPHYCFG->UPCR0, Hw25);
		BITCLR(USBPHYCFG->UPCR0,(Hw20|Hw24|Hw31)); 		//disable PHYVALID_EN -> no irq, SIDDQ, POR
		dwc_msleep(1);
		BITSET(USBPHYCFG->LCFG, Hw29);					// prstn

		tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 0);
#endif /* 0 */
	}
#else

	USBPHYCFG->UPCR1	=
					UPCR1_TXFSLST(7)	// FS/LS Pull-Up Resistance Adjustment = Design default
					| UPCR1_SQRXT(3)	// Squelch Threshold Tune = -5%
					| UPCR1_OTGT(4)		// VBUS Valid Threshold Adjustment = Design default
					| UPCR1_CDT(3);		// Disconnect Threshold Adjustment = Design default
	USBPHYCFG->UPCR2	=
					UPCR2_TM_HS
					| UPCR2_XCVRSEL_HS
					| UPCR2_OPMODE_NORMAL
					| UPCR2_TXVRT(8)	// HS DC voltage level adjustment = Design default
					| UPCR2_TXRT(0)		// HS Transmitter Rise/Fall Time Adjustment = Design default
					| UPCR2_TP_DIS;
	//USBPHYCFG->UPCR3	= 0x9000;	// OTG disable

	/*if ( mode == USB_MODE_NONE )
	{
		USBPHYCFG->UPCR0	= 0x2940;	// float DP,PM pins
	}
	else*/ if ( mode == USBPHY_MODE_RESET )
	{
#if defined(TCC893X)
		USBPHYCFG->UPCR0	= 0x4870 | Hw9;
#else
		USBPHYCFG->UPCR0	= 0x4870;
#endif
	}
	else if ( mode == USBPHY_MODE_OTG )
	{
#if defined(TCC893X)
		USBPHYCFG->UPCR0	= 0x80002800 | Hw9;
#else
		USBPHYCFG->UPCR0	= 0x80002800;
#endif
	}
	else if ( mode == USBPHY_MODE_HOST )
	{
		//HwUSBPHYCFG->UPCR0	= 0x2872;
#if defined(TCC893X)
		USBPHYCFG->UPCR0	= 0x800028b2 | Hw9;
#else
		USBPHYCFG->UPCR0	= 0x800028b2;
#endif
	}
	else if ( mode == USBPHY_MODE_DEVICE )
	{
#if defined(TCC893X)
		USBPHYCFG->UPCR0	= 0x2842 | Hw9;
#else
		USBPHYCFG->UPCR0	= 0x2842;
#endif
	}
#endif /* TCC897X */
}

#ifndef OTGID_ID_HOST
#define OTGID_ID_HOST						(~Hw9)
#endif
#ifndef OTGID_ID_DEVICE
#define OTGID_ID_DEVICE						Hw9
#endif

