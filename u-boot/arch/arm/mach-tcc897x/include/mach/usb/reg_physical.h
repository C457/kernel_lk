/****************************************************************************
 * FileName    : reg_physical.h
 * Description : 
 ****************************************************************************
 *
 * Copyright (C) 2015 Telechips, Inc.
 * All Rights Reserved
 *
 ****************************************************************************/

#ifndef _PLATFORM_REG_PHYSICAL_H_
#define _PLATFORM_REG_PHYSICAL_H_
/*******************************************************************************
*
*	TCC898X DataSheet USB
*
********************************************************************************/
#include "structures_usb.h"

#define HwUSB20OTG_BASE						(0x71B00000)
#define HwUSBPHYCFG_BASE					(0x71EA0100)



#define HwUSB20HOST_OHCI1_BASE					(0x71100000)
#define HwUSB20HOST_EHCI0_BASE					(0x71200000)
#define HwUSB20HOST_OHCI0_BASE					(0x71300000)
#define HwUSB20HOST_EHCI1_BASE					(0x71400000)

#define HwGMAC_BASE						(0x71700000)

#define HwTRNG_BASE						(0x71EB0000)

#define HwCIPHER_BASE						(0x71EC0000)

#define HwHSIOBUSCFG_BASE					(0x71EA0000)
#define HwUSB20PHYCFG_BASE                    	(0x71EA0010)

#define HwUSB20HOST_EHCI_BASE					(0x71200000)

#define TCC_USB20OTG_BASE					HwUSB20OTG_BASE
#define TCC_USBPHYCFG_BASE					HwUSBPHYCFG_BASE
#endif /* _PLATFORM_REG_PHYSICAL_H_ */
