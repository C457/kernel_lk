/**************************************************************************************
 *
 * FILE NAME   : usb_manager.c
 * DESCRIPTION : This is common usb device source code
 *
 * ====================================================================================
 *
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 * ====================================================================================
 *
 * LAST UPDATED: $Date$ by $Author$
 *
 * FILE HISTORY:
 * 	Date: 2009.02.12	Start source coding
 *
 **************************************************************************************/
#include <asm/arch/clock.h>
#include <asm/arch/usb/usb_defs.h>
#include <asm/arch/usb/usb_manager.h>
#include <asm/arch/usb/usb_device.h>
#include <asm/arch/usb/reg_physical.h>
#include <asm/arch/tcc_ckc.h>

/* For Signature */
#define USB_MANAGER_SIGNATURE	'U','S','B','_','M','A','N','A','G','E','R','_'
#define USB_MANAGER_VERSION		'V','2','.','0','0','1'
static const unsigned char USB_MANAGER_C_Version[] = {SIGBYAHONG, USB_MANAGER_SIGNATURE, SIGN_OS ,SIGN_CHIPSET, USB_MANAGER_VERSION, '\0'};

#if defined(USBPHY_INCLUDE)
#include <asm/arch/usb/usbphy.h>
#endif

#if defined(OTGCORE_INCLUDE)
#include <asm/arch/usb/otgcore.h>
#endif

#if defined(OTGDEV_IO_INCLUDE)
#include <asm/arch/usb/otgdev_io.h>
#endif

#include <asm/arch/irqs.h>


#if defined(DWCCORE_INCLUDE)
#include <asm/arch/usb/usb3.0/usb30dev.h>
#endif

//static USB_MODE_T g_USB_Mode;
//static char g_USB_DEVICE_fPrevOn;
//static char g_USB_DEVICE_fEnable;
//static USB_DEVICE_MODE_T g_USB_DEVICE_mode;

void USB_DEVICE_SetDefaultSerialNumber(void);

unsigned char* USB_TellLibraryVersion(void)
{
	return (unsigned char*)USB_MANAGER_C_Version;
}

#if defined(CONFIG_TCC898X)
#define HwOTG_LCFG1 (*((volatile unsigned int *)(0x111A011C)))
#endif

void USB_LINK_Reset(void)
{
#if defined(CONFIG_TCC897X) || defined(CONFIG_TCC898X)
#ifdef TCC_USB_30_USE
	// nop
#else

#if defined(CONFIG_TCC898X)
	BITCLR(HwOTG_LCFG1, 0x8000);
#endif

#if defined(CONFIG_TCC897X) || defined(CONFIG_TCC898X)
	tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 1);
	{
		volatile unsigned int t=10000;
		while(t-->0);
	}
	tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 0);
#else

	tca_ckc_setioswreset(IOBUS_USB20OTG, 1);
	{
		volatile unsigned int t=10000;
		while(t-->0);
	}
	tca_ckc_setioswreset(IOBUS_USB20OTG, 0);
#endif
#endif
#else
#endif
}

#if defined(FEATURE_USB_ISR)
enum handler_return {
	INT_NO_RESCHEDULE = 0,
	INT_RESCHEDULE,
};
static enum handler_return USB_Isr(void *arg)
#else
void USB_Isr(void)
#endif
{
	USBDEV_Isr();
	
#if defined(FEATURE_USB_ISR)
	return INT_NO_RESCHEDULE;
#endif
}

#if defined(TCC893X) || defined(TCC896X) 
void USB_Polling_Isr(void)
{
	USBDEV_Isr();
}
#endif
void USB_Init(USBDEV_T mode)
{
	extern void InitRoutine_Start(void);
	volatile void *tempPtr = InitRoutine_Start;
	// prevent from removing by optimized compilation
	if(tempPtr==NULL)
		InitRoutine_Start();
	//g_USB_Mode = USB_MODE_NONE;

	///////////////////////////////////////////
	// USB Driver Pre-Process
	///////////////////////////////////////////
	#if defined(USBPHY_INCLUDE)
	USBPHY_SetMode(USBPHY_MODE_RESET);
	#endif
	USB_LINK_Reset();

	///////////////////////////////////////////
	// OTG Core Initializing Sequence
	///////////////////////////////////////////
	#if defined(OTGCORE_INCLUDE)
	OTGCORE_Init();
	#elif defined(DWCCORE_INCLUDE)
	USB30DEV_DWC_CORE_Init();
	#endif

	///////////////////////////////////////////
	// USB Host Driver Initializing Sequence
	///////////////////////////////////////////

	///////////////////////////////////////////
	// USB Device Driver Initializing Sequence
	///////////////////////////////////////////
	{
		USBDEVICE_IO_DRIVER_T *pIoDriver;

		//USB_DEVICE_SetDefaultSerialNumber();

		#if defined(OTGDEV_IO_INCLUDE)
		pIoDriver = OTGDEV_IO_GetDriver();
		#elif defined(USB30DEV_IO_INCLUDE)
		pIoDriver = USB30DEV_IO_GetDriver();
		#else
		#endif
		USBDEV_Open(pIoDriver);
		USBDEV_SetClass(mode);

		//g_USB_DEVICE_fPrevOn = FALSE;
		//g_USB_DEVICE_fEnable = TRUE;
	}

	///////////////////////////////////////////
	// USB Driver Post-Process
	///////////////////////////////////////////
}

unsigned int USB_DEVICE_IsConnected(void)
{
	return (USBDEV_IsConnected()==TRUE) ? TRUE : FALSE;
}

//void USB_DEVICE_Enable(void)
//{
//	g_USB_DEVICE_fEnable = TRUE;
//}

//void USB_DEVICE_Disable(void)
//{
//	g_USB_DEVICE_fEnable = FALSE;
//}

void USB_DEVICE_On(void)
{
	//if( g_USB_DEVICE_fPrevOn == FALSE )
	{
		USBDEV_Init();
		#if defined(USBPHY_INCLUDE)
		USBPHY_SetMode(USBPHY_MODE_DEVICE);
		#endif
	}
}

//void USB_DEVICE_Off(void)
//{
//	if( g_USB_DEVICE_fPrevOn == TRUE )
//	{
//		g_USB_DEVICE_fPrevOn = FALSE;
//		#if defined(USBPHY_INCLUDE)
//		USBPHY_SetMode(USBPHY_MODE_RESET);
//		#endif
//		//BITSET(HwIOBUSCFG->HRSTEN0, HwIOBUSCFG_USB);
//		//IO_CKC_DisableBUS_USB();
//	}
//	USBDEV_DEBUG("USB_Device_Off\n");
//}

void USB_DEVICE_Run(void)
{
	USBDEV_Run();
}

//unsigned int USB_DEVICE_IsPreOccupied(void)
//{
//	if( g_USB_DEVICE_mode == MODE_MTP_PLAYBACK )
//		return FALSE;
//	else
//		return TRUE;
//}

//void USB_DEVICE_SetMode(USB_DEVICE_MODE_T mode)
//{
//	switch(mode)
//	{
//		case MODE_VTC:
//			USBDEV_SetClass(USBDEV_CLASS_VTC);
//			break;
//		case MODE_MSC:
//			USBDEV_SetClass(USBDEV_CLASS_MSC);
//			break;
//		case MODE_MTP:
//		case MODE_MTP_PLAYBACK:
//			USBDEV_SetClass(USBDEV_CLASS_MTP);
//			break;
//		//case MODE_PCDMB:
//		//	USBDEV_SetClass(USBDEV_CLASS_PCDMB);
//		//	break;
//	}
//	g_USB_DEVICE_mode = mode;
//}

//USB_DEVICE_MODE_T USB_DEVICE_GetMode(void)
//{
//	return g_USB_DEVICE_mode;
//}
