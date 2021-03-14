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

#include <globals.h>

typedef struct {
	unsigned VALUE		:16;
} TCC_DEF16BIT_IDX_TYPE;

typedef union {
	unsigned short		nREG;
	TCC_DEF16BIT_IDX_TYPE	bREG;
} TCC_DEF16BIT_TYPE;

typedef struct {
	unsigned VALUE		:32;
} TCC_DEF32BIT_IDX_TYPE;

typedef union {
	unsigned long		nREG;
	TCC_DEF32BIT_IDX_TYPE	bREG;
} TCC_DEF32BIT_TYPE;

/*******************************************************************************
*
*	TCC897X DataSheet PART 2 SMU & PMU
*
********************************************************************************/

#define HwGPIO_BASE						(0x74200000)
#define HwGPIOA_BASE						(0x74200000)
#define HwGPIOB_BASE						(0x74200040)
#define HwGPIOC_BASE						(0x74200080)
#define HwGPIOD_BASE						(0x742000C0)
#define HwGPIOE_BASE						(0x74200100)
#define HwGPIOF_BASE						(0x74200140)
#define HwGPIOG_BASE						(0x74200180)


/*******************************************************************************
*
*	TCC897X DataSheet PART 5 IO BUS
*
********************************************************************************/
#define VIOC_TCC8970

#define HwI2C_MASTER0_BASE					(0x76300000)
#define HwI2C_MASTER1_BASE					(0x76310000)
#define HwI2C_MASTER2_BASE					(0x76320000)
#define HwI2C_MASTER3_BASE					(0x76330000)
#define HwI2C_SLAVE0_BASE					(0x76340000)
#define HwI2C_SLAVE1_BASE					(0x76350000)
#define HwI2C_PORTCFG_BASE					(0x76360000)

#define HwUART0_BASE						(0x76370000)
#define HwUART1_BASE						(0x76380000)
#define HwUART2_BASE						(0x76390000)
#define HwUART3_BASE						(0x763A0000)
#define HwUART4_BASE						(0x763B0000)
#define HwUART5_BASE						(0x763C0000)
#define HwUART6_BASE						(0x763D0000)
#define HwUART7_BASE						(0x763E0000)
#define HwUART_PORTCFG_BASE					(0x763F0000)


/*******************************************************************************
*
*	TCC897X DataSheet PART 7 DISPLAY BUS
*
********************************************************************************/

#include "structures_display.h"

#define HwVIOC_BASE                             (0x72000000)

#define BASE_ADDR_VIOC                          HwVIOC_BASE

/* RDMA */
#define HwVIOC_RDMA_GAP     	(0x100)
#define HwVIOC_RDMA00           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0400)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA01           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0500)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA02           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0600)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA03           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0700)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA04           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0800)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA05           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0900)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_RDMA06           ( (BASE_ADDR_VIOC  + 0x0000 + 0x0A00)) // 64 words    // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
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
#endif



/* WMIX */
#define HwVIOC_WMIX0            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1800)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX1            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1900)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX2            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1A00)) // 64 words    // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_WMIX3            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1B00)) // 64 words    // TCC8910
#define HwVIOC_WMIX4            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1C00)) // 64 words    // TCC8910
#define HwVIOC_WMIX5            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1D00)) // 64 words    // TCC8910
#define HwVIOC_WMIX6            ( (BASE_ADDR_VIOC  + 0x0000 + 0x1E00)) // 64 words    // TCC8910
#endif

#define HwVIOC_WMIX0_ALPHA      ( (BASE_ADDR_VIOC  + 0x0000 + 0x1840)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX1_ALPHA      ( (BASE_ADDR_VIOC  + 0x0000 + 0x1940)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WMIX2_ALPHA      ( (BASE_ADDR_VIOC  + 0x0000 + 0x1A40)) // 64 words    // TCC8910 | TCC8010

/* SCALER */
#define HwVIOC_SC0              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2000)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_SC1              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2100)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_SC2              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2200)) // 64 words    // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_SC3              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2300)) // 64 words    // TCC8910
#endif

#if defined(VIOC_TCC8970)
#define HwVIOC_SC4              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2400)) // 64 words    // TCC8910
#define HwVIOC_SC5              ( (BASE_ADDR_VIOC  + 0x0000 + 0x2500)) // 64 words    // TCC8910
#endif

/* WDMA */
#define HwVIOC_WDMA00           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2800)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA01           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2900)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA02           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2A00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA03           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2B00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA04           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2C00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA05           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2D00)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_WDMA06           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2E00)) // 64 words    // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_WDMA07           ( (BASE_ADDR_VIOC  + 0x0000 + 0x2F00)) // 64 words    // TCC8910
#define HwVIOC_WDMA08           ( (BASE_ADDR_VIOC  + 0x0000 + 0x3000)) // 64 words    // TCC8910
#endif




/* DEINTLS */
#define HwVIOC_DEINTLS          ( (BASE_ADDR_VIOC  + 0x0000 + 0x3800)) // 64 words    // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_FDLY0            ( (BASE_ADDR_VIOC  + 0x0000 + 0x3900)) // 64 words    // TCC8910
#define HwVIOC_FDLY1            ( (BASE_ADDR_VIOC  + 0x0000 + 0x3A00)) // 64 words    // TCC8910
#define HwVIOC_FIFO             ( (BASE_ADDR_VIOC  + 0x0000 + 0x3B00)) // 64 words    // TCC8910
#define HwVIOC_DEBLOCK          ( (BASE_ADDR_VIOC  + 0x0000 + 0x3C00)) // 64 words    // TCC8910
#endif
//*********************************************************************************************************************************************
// [15:14] == 2'b01
//*********************************************************************************************************************************************
/* VIN */
#define HwVIOC_VINDEMUX         ( (BASE_ADDR_VIOC  + 0xA800         ))                // TCC8910 | TCC8010
#define HwVIOC_VIN00            ( (BASE_ADDR_VIOC  + 0x4000         ))                // TCC8910 | TCC8010
#define HwVIOC_VIN01            ( (BASE_ADDR_VIOC  + 0x4400         ))                // TCC8910 | TCC8010 
#define HwVIOC_VIN10            ( (BASE_ADDR_VIOC  + 0x5000         ))                // TCC8910 | TCC8010 
#define HwVIOC_VIN11            ( (BASE_ADDR_VIOC  + 0x5400         ))                // TCC8910 | TCC8010 
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_VIN20            ( (BASE_ADDR_VIOC  + 0x6000         ))                // TCC8910 
#define HwVIOC_VIN21            ( (BASE_ADDR_VIOC  + 0x6400         ))                // TCC8910 
#define HwVIOC_VIN30            ( (BASE_ADDR_VIOC  + 0x7000         ))                // TCC8910 
#define HwVIOC_VIN31            ( (BASE_ADDR_VIOC  + 0x7400         ))                // TCC8910 
#endif

//*********************************************************************************************************************************************
// [15:14] == 2'b10
//*********************************************************************************************************************************************
#define HwVIOC_FILT2D           ( (BASE_ADDR_VIOC  + 0x8000        ))                // TCC8910
#define HwVIOC_LUT              ( (BASE_ADDR_VIOC  + 0x9000        ))                // TCC8910 | TCC8010
#define HwVIOC_LUT_TAB          ( (BASE_ADDR_VIOC  + 0x9400        ))
#define HwVIOC_CONFIG           ( (BASE_ADDR_VIOC  + 0xA000        ))                // TCC8910 | TCC8010
#define HwVIOC_IREQ             ( (BASE_ADDR_VIOC  + 0xA000 + 0x000)) //  16 word
#define HwVIOC_PCONFIG          ( (BASE_ADDR_VIOC  + 0xA000 + 0x040)) //  32 word
#define HwVIOC_POWER            ( (BASE_ADDR_VIOC  + 0xA000 + 0x0C0)) //  16 word
#define HwVIOC_FCODEC           ( (BASE_ADDR_VIOC  + 0xB000        ))                // TCC8910

/* VIQE */
#define HwVIOC_VIQE0_BASE       ( (BASE_ADDR_VIOC  + 0xD000        ))                // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_VIQE1_BASE       ( (BASE_ADDR_VIOC  + 0xE000        ))                // TCC8910 
#endif
#define  HwVIOC_VIQE0                           (HwVIOC_VIQE0_BASE)


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

#define HwCORTEXM4_TS_CIPHER_BASE               (0x79010000)
#define HwCORTEXM4_SECTION_FILTER_BASE          (0x79020000)
#define HwCORTEXM4_PKT_MEM_BASE                 (0x79030000)
#define HwCORTEXM4_CODE_MEM_BASE                (0x79080000)
#define HwCORTEXM4_CODE_MEM_SIZE                (0x00010000)
#define HwCORTEXM4_DATA_MEM_BASE                (0x79090000)
#define HwCORTEXM4_DATA_MEM_SIZE                (0x00010000)
#define HwCORTEXM4_MAILBOX0_BASE                (0x790A0000)
#define HwCORTEXM4_MAILBOX1_BASE                (0x790B0000)
#define HwCORTEXM4_MAILBOX2_BASE                (0x790C0000)
#define HwCORTEXM4_MAILBOX3_BASE                (0x790D0000)

#define HwMBOX_M            ((volatile MAILBOX		*)HwCORTEXM4_MAILBOX0_BASE)
#define HwMBOX_S            ((volatile MAILBOX		*)HwCORTEXM4_MAILBOX1_BASE)


/*******************************************************************************
*
*	ARM BUS
*
********************************************************************************/

#define HwARM_CPU_BASE						(0x77000000)
#define HwARM_GIC_DIST_BASE					(0x77101000)
#define HwARM_GIC_CPU_BASE					(0x77102000)

#define WORDSET(a, d)				*((volatile unsigned int *) (a)) = (unsigned int) (d);
#define HWORDSET(a, d)				*((volatile unsigned short *) (a)) = (unsigned int) (d);
#define BYTESET(a, d)				*((volatile unsigned char *) (a)) = (unsigned int) (d);
#define WORDREAD(a, d)				d = *((volatile unsigned int *) (a));
#define HWORDREAD(a, d)				d = *((volatile unsigned short *) ((a)));
#define BYTEREAD(a, d)				d = *((volatile unsigned char *) (a));
#define REGSET(a, d)				*((volatile unsigned int *) (a)) = (unsigned int) (d);
#define REGREAD(a, d)				d = *((volatile unsigned int *) (a));

#endif /* _PLATFORM_REG_PHYSICAL_H_ */
