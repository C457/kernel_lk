/****************************************************************************
 * FileName    : reg_physical.h
 * Description : 
 ****************************************************************************
 *
 * Copyright (C) 2016 Telechips, Inc.
 * All Rights Reserved
 *
 ****************************************************************************/

#ifndef _PLATFORM_REG_PHYSICAL_H_
#define _PLATFORM_REG_PHYSICAL_H_

#include <globals.h>

/*******************************************************************************
*
*	TCC802X DataSheet PART 7 DISPLAY BUS
*
********************************************************************************/

#include "structures_display.h"

#define HwVIOC_BASE                             (0x72000000)


#define BASE_ADDR_VIOC                          HwVIOC_BASE
#define HwVIOC_CONFIG           ( (BASE_ADDR_VIOC  + 0xA000        ))                // TCC8910 | TCC8010
#define HwVIOC_IREQ             ( (BASE_ADDR_VIOC  + 0xA000 + 0x000))

/* RDMA */
#define HwVIOC_RDMA_GAP     	(0x100)
#define HwVIOC_RDMA00           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0400)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA01           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0500)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA02           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0600)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA03           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0700)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA04           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0800)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA05           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0900)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA06           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0A00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA07           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0B00)) // 64 words    // TCC8910
#define HwVIOC_RDMA08           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0C00)) // 64 words    // TCC8910
#define HwVIOC_RDMA09           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0D00)) // 64 words    // TCC8910
#define HwVIOC_RDMA10           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0E00)) // 64 words    // TCC8910
#define HwVIOC_RDMA11           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0F00)) // 64 words    // TCC8910
#define HwVIOC_RDMA12           ( (BASE_ADDR_VIOC  + 0x0000 + 0x1000)) // 64 words    // TCC8910
#define HwVIOC_RDMA13           ( (BASE_ADDR_VIOC  + 0x0000 + 0x1100)) // 64 words    // TCC8910
#define HwVIOC_RDMA14           ( (BASE_ADDR_VIOC  + 0x0000 + 0x1200)) // 64 words    // TCC8910
#define HwVIOC_RDMA15           ( (BASE_ADDR_VIOC  + 0x0000 + 0x1300)) // 64 words    // TCC8910
#define HwVIOC_RDMA16           ( (BASE_ADDR_VIOC  + 0x0000 + 0x1400)) // 64 words    // TCC8910
#define HwVIOC_RDMA17           ( (BASE_ADDR_VIOC  + 0x0000 + 0x1500)) // 64 words    // TCC8910

/* WMIX */
#define HwVIOC_WMIX0            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1800)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX1            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1900)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX2            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1A00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX3            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1B00)) // 64 words    // TCC8910
#define HwVIOC_WMIX4            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1C00)) // 64 words    // TCC8910
#define HwVIOC_WMIX5            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1D00)) // 64 words    // TCC8910
#define HwVIOC_WMIX6            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1E00)) // 64 words    // TCC8910

/* SCALER */
#define HwVIOC_SC0              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2000)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_SC1              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2100)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_SC2              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2200)) // 64 words    // TCC8910 | TCC8010

/* WDMA */
#define HwVIOC_WDMA00           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2800)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA01           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2900)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA02           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2A00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA03           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2B00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA04           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2C00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA05           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2D00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA06           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2E00)) // 64 words    // TCC8910 | TCC8010


#define HwVIOC_FIFO             ( (BASE_ADDR_VIOC  + 0x0000 + 0x3B00)) // 64 words    // TCC8910
/* VIN */
#define HwVIOC_VIN00		( (BASE_ADDR_VIOC  + 0x4000         ))

/* VIQE */
#define HwVIOC_VIQE0_BASE	( (BASE_ADDR_VIOC  + 0xD000        ))
#define HwVIOC_VIQE0		(HwVIOC_VIQE0_BASE)
/************************************************************************
*   DDI_CONFIG (Base Addr = 0x72380000) // R/W
*************************************************************************/

#define HwDDI_CONFIG_BASE                       (0x72380000)

/*******************************************************************************
*
*	TCC897X DataSheet PART 9 Cortex-M4 BUS
*
********************************************************************************/

#include "structures_cm4.h"
#define HwCORTEXM4_CODE_MEM_BASE		(0x79060000)
#define HwCORTEXM4_CODE_MEM_SIZE		(0x00020000)
#define HwCORTEXM4_DATA_MEM_BASE		(0x79080000)
#define HwCORTEXM4_DATA_MEM_SIZE		(0x00020000)
#define HwCORTEXM4_MAILBOX0_BASE		(0x790A0000)
#define HwCORTEXM4_MAILBOX1_BASE		(0x790B0000)


//===========================================================================================
// ex) REGSET(0xA0000000, nReg);
//     => *0xA0000000 = nReg
//
//     REGREAD (0xA0000000, nReg);
//     => nReg = *0xA0000000 
//===========================================================================================

#define WORDSET(a, d)				*((volatile unsigned int *) (a)) = (unsigned int) (d);
#define HWORDSET(a, d)				*((volatile unsigned short *) (a)) = (unsigned int) (d);
#define BYTESET(a, d)				*((volatile unsigned char *) (a)) = (unsigned int) (d);
#define WORDREAD(a, d)				d = *((volatile unsigned int *) (a));
#define HWORDREAD(a, d)				d = *((volatile unsigned short *) ((a)));
#define BYTEREAD(a, d)				d = *((volatile unsigned char *) (a));
#define REGSET(a, d)				*((volatile unsigned int *) (a)) = (unsigned int) (d);
#define REGREAD(a, d)				d = *((volatile unsigned int *) (a));

#endif /* _PLATFORM_REG_PHYSICAL_H_ */

