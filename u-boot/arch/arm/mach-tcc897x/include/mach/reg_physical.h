/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#ifndef _TCC897X_REG_PHYSICAL_H_
#define _TCC897X_REG_PHYSICAL_H_

#include <asm/arch/iomap.h>
#include <asm/arch/globals.h>

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

#include "structures_smu_pmu.h"

#define HwCKC_BASE						(0x74000000)

#define HwPIC_BASE						(0x74100000)
#define HwVIC_BASE						(0x74100200)

#define HwTMR_BASE						(0x74300000)

#define HwPMU_BASE						(0x74400000)

#define HwSMUI2C_BASE						(0x74500000)

#define HwGPIO_BASE						(0x74200000)
#define HwGPIOA_BASE						(0x74200000)
#define HwGPIOB_BASE						(0x74200040)
#define HwGPIOC_BASE						(0x74200080)
#define HwGPIOD_BASE						(0x742000C0)
#define HwGPIOE_BASE						(0x74200100)
#define HwGPIOF_BASE						(0x74200140)
#define HwGPIOG_BASE						(0x74200180)
#define HwGPIOHDMI_BASE						(0x742001C0)
#define HwGPIOADC_BASE						(0x74200200)
#define HwGPIOSD_BASE						(0x74200240)

#define HwSMUCONFIG_BASE					(0x74600000)


/*******************************************************************************
*
*	TCC897X DataSheet PART 3 GRAPHIC BUS
*
********************************************************************************/

#define HwGPU_BASE						(0x70000000)

#define HwGRPBUSCONFIG_BASE					(0x70004000)


/*******************************************************************************
*
*	TCC897X DataSheet PART 4 MEMORY BUS
*
********************************************************************************/

#define HwMBUS_MMU_BASE						(0x73000000)

#define HwMBUSCFG_BASE						(0x73810000)


/*******************************************************************************
*
*	TCC897X DataSheet PART 5 IO BUS
*
********************************************************************************/
#include "structures_io.h"
#define VIOC_TCC8970
#define HwMPEFEC_BASE						(0x76010000)

#define HwSDMMC0_BASE						(0x76020000)
#define HwSDMMC1_BASE						(0x76020200)
#define HwSDMMC2_BASE						(0x76020400)
#define HwSDMMC3_BASE						(0x76020600)
#define HwSDMMC_CHCTRL_BASE					(0x76020800)
#if (1)
// TODO: please move this in the device source
#define HwSDMMC_CHCTRL_CH0_CAP0					(HwSDMMC_CHCTRL_BASE + 0x18)
#define HwSDMMC_CHCTRL_CH0_CAP1					(HwSDMMC_CHCTRL_BASE + 0x1C)
#define HwSDMMC_CHCTRL_CH1_CAP0					(HwSDMMC_CHCTRL_BASE + 0x30)
#define HwSDMMC_CHCTRL_CH1_CAP1					(HwSDMMC_CHCTRL_BASE + 0x34)
#define HwSDMMC_CHCTRL_CH2_CAP0					(HwSDMMC_CHCTRL_BASE + 0x48)
#define HwSDMMC_CHCTRL_CH2_CAP1					(HwSDMMC_CHCTRL_BASE + 0x4C)
#define HwSDMMC_CHCTRL_CH3_CAP0					(HwSDMMC_CHCTRL_BASE + 0x60)
#define HwSDMMC_CHCTRL_CH3_CAP1					(HwSDMMC_CHCTRL_BASE + 0x64)
#endif

#define HwGDMA0_BASE						(0x76030000)
#define HwGDMA1_BASE						(0x76030100)
#define HwGDMA2_BASE						(0x76030200)

#define HwOVERLAYMIXER_BASE					(0x72400000)

#define HwPWM_BASE						(0x76050000)

#define HwSMC_BASE						(0x76060000)

#define HwRTC_BASE						(0x76062000)

#define HwTSADC_BASE						(0x76064000)

#define HwEDI_BASE						(0x76065000)

#define HwPROTECT_BASE						(0x76067000)

#define HwAXIBM_BASE						(0x76068000)

#define HwREMOTE_BASE						(0x76070000)

#define HwAUDIO1_ADMA_BASE					(0x76100000)
#define HwAUDIO1_DAI_BASE					(0x76101000)
#define HwAUDIO1_CDIF_BASE					(0x76101080)
#define HwAUDIO1_SPDIFTX_BASE					(0x76102000)

#define HwAUDIO0_ADMA_BASE					(0x76200000)
#define HwAUDIO0_DAI_BASE					(0x76201000)
#define HwAUDIO0_CDIF_BASE					(0x76201080)
#define HwAUDIO0_SPDIFTX_BASE					(0x76202000)
#define HwAUDIO0_SPDIFRX_BASE					(0x76202800)

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

#define HwIDE_BASE						(0x76400000)

#define HwTSIF_CTRL0_BASE					(0x76500000)
#define HwTSIF_CFG0_BASE					(0x76510000)
#define HwTSIF_DMA0_BASE					(0x76520000)

#define HwNFC_BASE						(0x76600000)
#define HwECC_BASE                              (0x76600200)
#define HwECCBASE                               (0x76600208)

#define HwSMC0_BASE						(0x76700000)
#define HwSMC1_BASE						(0x76740000)
#define HwSMC2_BASE						(0x76780000)
#define HwSMC3_BASE						(0x767C0000)

#define HwTSIF_CTRL1_BASE					(0x76800000)
#define HwTSIF_CFG1_BASE					(0x76810000)
#define HwTSIF_DMA1_BASE					(0x76820000)

#define HwGPSB_BASE						(0x76900000)

#define HwIOBUSCFG_BASE						(0x76066000)


/*******************************************************************************
GPSB Controller Register Define
********************************************************************************/
#define HwGPSB0_BASE							(0x76900000)
#define HwGPSB1_BASE							(0x76910000)
#define HwGPSB2_BASE							(0x76920000)
#define HwGPSB_PORTCFG_BASE						(0x76960000)
#define HwGPSB_PIDTABLE_BASE					(0x76970000)

// Status
#define	HwGPSB_STAT_WOR				Hw8											// Write FIFO over-run error flag
#define	HwGPSB_STAT_RUR				Hw7											// Read FIFO under-run error flag
#define	HwGPSB_STAT_WUR				Hw6											// Write FIFO under-run error flag
#define	HwGPSB_STAT_ROR				Hw5											// Read FIFO over-run error flag
#define	HwGPSB_STAT_RF				Hw4											// Read FIFO full flag
#define	HwGPSB_STAT_WE				Hw3											// Write FIFO empty flag
#define	HwGPSB_STAT_RNE				Hw2											// Read FIFO not empty flag
#define	HwGPSB_STAT_WTH				Hw1											// Wrtie FIFO valid entry count is under threshold
#define	HwGPSB_STAT_RTH				Hw0											// Read FIFO valid entry increased over threshold

#define	HwGPSB_PIDT(X)				*(volatile unsigned long *)(HwGPSB_PIDTABLE_BASE+(X)*4)	// R/W, PID Table Register
#define	HwGPSB_PIDT_CH2				Hw31											// Channel 2 enable
#define	HwGPSB_PIDT_CH1				Hw30											// Channel 1 enable
#define	HwGPSB_PIDT_CH0				Hw29											// Channel 0 enable

#define	HwGPSB_DMAICR_ISD			Hw29										// IRQ Status for "Done Interrupt"
#define	HwGPSB_DMAICR_ISP			Hw28										// IRQ Status for "Packet Interrupt"
#define	HwGPSB_DMAICR_IRQS_TRANS	Hw20										// IRQ is generated when transmit is done.
#define	HwGPSB_DMAICR_IRQS_RCV		HwZERO										// IRQ is generated when receiving is done.
#define	HwGPSB_DMAICR_IED_EN		Hw17										// Enable "Done Interrupt"
#define	HwGPSB_DMAICR_IEP_EN		Hw16										// Enable "Packet Interrupt"
#define	HwGPSB_DMAICR_IRQPCNT_MASK	(Hw13-1)

// DMA Control
#define	HwGPSB_DMACTRL_DTE			Hw31										// Transmit DMA request enable
#define	HwGPSB_DMACTRL_DRE			Hw30										// Receive DMA request enable
#define	HwGPSB_DMACTRL_CT			Hw29										// Continuous mode enable
#define	HwGPSB_DMACTRL_END			Hw28										// Byte endian mode register
#define	HwGPSB_DMACTRL_MP			Hw19										// PID match mode register
#define	HwGPSB_DMACTRL_MS			Hw18										// Sync byte match control register
#define	HwGPSB_DMACTRL_TXAM(X)		((X)*Hw16)
#define	HwGPSB_DMACTRL_TXAM_MULTI	HwGPSB_DMACTRL_TXAM(0)						// TX address is rolling within full packet range
#define	HwGPSB_DMACTRL_TXAM_FIXED	HwGPSB_DMACTRL_TXAM(1)						// TX address is fixed to TXBASE
#define	HwGPSB_DMACTRL_TXAM_SINGLE	HwGPSB_DMACTRL_TXAM(2)						// TX address is rolling within 1 packet range
#define	HwGPSB_DMACTRL_TXAM_MASK	HwGPSB_DMACTRL_TXAM(3)
#define	HwGPSB_DMACTRL_RXAM(X)		((X)*Hw14)
#define	HwGPSB_DMACTRL_RXAM_MULTI	HwGPSB_DMACTRL_RXAM(0)						// RX address is rolling within full packet range
#define	HwGPSB_DMACTRL_RXAM_FIXED	HwGPSB_DMACTRL_RXAM(1)						// RX address is fixed to RXBASE
#define	HwGPSB_DMACTRL_RXAM_SINGLE	HwGPSB_DMACTRL_RXAM(2)						// RX address is rolling within 1 packet range
#define	HwGPSB_DMACTRL_RXAM_MASK	HwGPSB_DMACTRL_RXAM(3)
#define	HwGPSB_DMACTRL_MD_NOR		HwZERO										// normal mode
#define	HwGPSB_DMACTRL_MD_MP2TS		Hw4											// MPEG2-TS mode
#define	HwGPSB_DMACTRL_PCLR			Hw2											// Clear TX/RX Packet Counter
#define	HwGPSB_DMACTRL_EN			Hw0											// DMA enable

// Interrupt Enable
#define	HwGPSB_INTEN_DW				Hw31										// DMA request enable for transmit FIFO
#define	HwGPSB_INTEN_DR				Hw30										// DMA request enable for receive FIFO
#define	HwGPSB_INTEN_SWD_BHW		Hw27										// Swap byte in half-word
#define	HwGPSB_INTEN_SWD_HWW		Hw26										// Swap half-word in word
#define	HwGPSB_INTEN_SRD_BHW		Hw25										// Swap byte in half-word
#define	HwGPSB_INTEN_SRD_HWW		Hw24										// Swap half-word in word
#define	HwGPSB_INTEN_CFGWTH(X)		((X)*Hw20)									// Write FIFO threshold for Interrupt or DMA Request
#define	HwGPSB_INTEN_CFGWTH_MASK	HwGPSB_INTEN_CFGWTH(7)
#define	HwGPSB_INTEN_CFGRTH(X)		((X)*Hw16)									// Read FIFO threshold for Interrupt or DMA Request
#define	HwGPSB_INTEN_CFGRTH_MASK	HwGPSB_INTEN_CFGRTH(7)
#define	HwGPSB_INTEN_RC				Hw15										// Clear status[8:0] at the end of read cycle
#define	HwGPSB_INTEN_WOR			Hw8											// Write FIFO over-run error interrupt enable
#define	HwGPSB_INTEN_RUR			Hw7											//Read FIFO under-run error flag interrupt enable
#define	HwGPSB_INTEN_WUR			Hw6											// Write FIFO under-run error flag interrupt enable
#define	HwGPSB_INTEN_ROR			Hw5											// Read FIFO over-run error flag interrupt enable
#define	HwGPSB_INTEN_RF				Hw4											// Read FIFO full flag interrupt enable
#define	HwGPSB_INTEN_WE				Hw3											// Write FIFO empty flag interrupt enable
#define	HwGPSB_INTEN_RNE			Hw2											// Read FIFO not empty flag interrupt enable
#define	HwGPSB_INTEN_WTH			Hw1											// Wrtie FIFO valid entry count is under threshold interrupt enable
#define	HwGPSB_INTEN_RTH			Hw0											// Read FIFO valid entry increased over threshold interrupt enable


// Mode
#define	HwGPSB_MODE_DIVLDV(X)		((X)*Hw24)
#define	HwGPSB_MODE_DIVLDV_MASK		HwGPSB_MODE_DIVLDV(255)
#define	HwGPSB_MODE_TRE				Hw23										// Master recovery time (TRE+1)*SCKO
#define	HwGPSB_MODE_THL				Hw22										// Master hold time (THL+1)*SCKO
#define	HwGPSB_MODE_TSU				Hw21										// Master setup time (TSU+1)*SCKO
#define	HwGPSB_MODE_PCS				Hw20										// Polarity control for CS(FRM) - Master Only
#define	HwGPSB_MODE_PCS_HIGH		Hw20
#define	HwGPSB_MODE_PCD				Hw19										// Polarity control for CMD(FRM)- Master Only
#define	HwGPSB_MODE_PCD_HIGH		Hw19
#define	HwGPSB_MODE_PWD				Hw18										// Polarity control for transmit data - Master Only
#define	HwGPSB_MODE_PWD_RISE		Hw18
#define	HwGPSB_MODE_PRD				Hw17										// Polarity control for receive data - Master Only
#define	HwGPSB_MODE_PRD_FALL		Hw17
#define	HwGPSB_MODE_PCK				Hw16										// Polarity control for serial clock
#define	HwGPSB_MODE_CRF				Hw15										// Clear receive FIFO counter
#define	HwGPSB_MODE_CWF				Hw14										// Clear transmit FIFO counter
#define	HwGPSB_MODE_BWS(X)			((X)*Hw8)									// Bit width Selection.(BWS+1. Valid = 7~31)
#define	HwGPSB_MODE_BWS_MASK		HwGPSB_MODE_BWS(31)
#define	HwGPSB_MODE_SD				Hw7											// Data shift direction control
#define	HwGPSB_MODE_LB				Hw6											// Data looop-back enable
#define	HwGPSB_MODE_CTF				Hw4											// Continuous transfer mode enable
#define	HwGPSB_MODE_EN				Hw3											// Operation enable
#define	HwGPSB_MODE_SLV				Hw2											// Slave mode configuration
#define	HwGPSB_MODE_MD_SPI			HwZERO										// SPI compatible
#define	HwGPSB_MODE_MD_SSP			Hw0											// SSP Compatible

/*******************************************************************************
*
*	TCC897X DataSheet PART 6 HSIO BUS
*
********************************************************************************/
#include "structures_hsio.h"
#define HwHSIOBUSCFG_BASE					(0x71EA0000)

/*******************************************************************************
*
*	TCC897X DataSheet PART 7 DISPLAY BUS
*
********************************************************************************/

// DISP Control Reg
#define HwDISP_EVP                      Hw31                                // External Vsync Polarity
#define HwDISP_EVS                      Hw30                                // External Vsync Enable
#define HwDISP_R2YMD                    (Hw29+Hw28)                         // RGB to YCbCr Conversion Option
#define HwDISP_ADVI                     Hw26                                // Advanced interlaced mode
#define HwDISP_656                      Hw24                                // CCIR 656 Mode
#define HwDISP_CKG                      Hw23                                // Clock Gating Enable for Timing Generator
#define HwDISP_SREQ                     Hw22                                // stop request
#define HwDISP_Y2RMD				Hw21 + Hw20				// YUV to RGB converter mode register
#define HwDISP_PXDW                     (Hw19+Hw18+Hw17+Hw16)               // PXDW
#define HwDISP_ID                       Hw15                                // Inverted Data Enable
#define HwDISP_IV                       Hw14                                // Inverted Vertical Sync
#define HwDISP_IH                       Hw13                                // Inverted Horizontal Sync
#define HwDISP_IP                       Hw12                                // Inverted Pixel Clock
#define HwDISP_CLEN                     Hw11                                // Clipping Enable
#define HwDISP_R2Y                      Hw10                                // RGB to YCbCr Converter Enable for Output 
#define HwDISP_DP                       Hw9                                 // Double Pixel Data
#define HwDISP_NI                       Hw8                                 // Non-Interlace
#define HwDISP_TV                       Hw7                                 // TV mode 
#define HwDISP_SRST                     Hw6                                 // Device display reset 
#define HwDISP_Y2R				Hw4						// YUV to RGB converter enable register
#define HwDISP_SWAP                     (Hw3+Hw2+Hw1)                       // Output RGB overlay swap
#define HwDISP_LEN                      Hw0                                 // LCD Controller Enable



// RDMA Control Reg
#define HwDMA_INTL                      Hw31                                // Interlaced Image
#define HwDMA_BFMD                      Hw30                                // Bfield mode
#define HwDMA_BF                        Hw29                                // Bottom field
#define HwDMA_IEN                       Hw28                                // Image Display Function for Each Image
#define HwDMA_STRM                      Hw27                                // streaming mode
#define HwDMA_3DMD				Hw26 + Hw25				// 3D mode type
#define HwDMA_ASEL                      Hw24                                // Image Displaying Function for Each Image
#define HwDMA_UVI                       Hw23                                // UV ineterpolation
#define HwDMA_R2YMD				Hw19+Hw18				// RGB to YUV converter mode register
#define HWDMA_R2Y				Hw17					// RGB to YUV converter enable register
#define HwDMA_UPD                       Hw16                                // data update enable
#define HwDMA_PD                        Hw15                                // Bit padding
#define HwDMA_SWAP				Hw14+Hw13+Hw12			// RGB swap register
#define HwDMA_AEN				Hw11					// Alpha enable register
#define HwDMA_Y2RMD                     (Hw10+Hw9)                          // YCbCr to RGB Conversion Option
#define HwDMA_Y2R                       Hw8                                 // YCbCr to RGB Conversion Enable Bit
#define HwDMA_BR                        Hw7                                 // Bit Reverse
#define HwDMA_FMT                       (Hw4+Hw3+Hw2+Hw1+Hw0)               // Image Format

//DISP status

#define HwLSTATUS_VS                    Hw31                                // Monitoring vertical sync
#define HwLSTATUS_BUSY                  Hw30                                // Busy signal
#define HwLSTATUS_EF                    Hw29                                // Even-Field(Read Only). 0:Odd field or frame, 1:Even field or frame
#define HwLSTATUS_DEOF                  Hw28                                // DMA End of Frame flag
#define HwLSTATUS_I0EOF                 Hw27                                // Image 0 End of Frame flag
#define HwLSTATUS_I1EOF                 Hw26                                // Image 1 End of Frame flag
#define HwLSTATUS_I2EOF                 Hw25                                // Image 2 End of Frame flag
#define HwLSTATUS_IE2F                  Hw12                                // Image 2 end-of-frame falling edge flag
#define HwLSTATUS_IE2R                  Hw11                                // Image 2 end-of-frame rising edge flag
#define HwLSTATUS_IE1F                  Hw10                                // Image 1 end-of-frame falling edge flag
#define HwLSTATUS_IE1R                  Hw9                                 // Image 1 end-of-frame rising edge flag
#define HwLSTATUS_IE0F                  Hw8                                 // Image 0 end-of-frame falling edge flag
#define HwLSTATUS_IE0R                  Hw7                                 // Image 0 end-of-frame rising edge flag
#define HwLSTATUS_DEF                   Hw6                                 // DMA end-of-frame falling edge flag
#define HwLSTATUS_SREQ                  Hw5                                 // Device stop request
#define HwLSTATUS_DD                    Hw4                                 // Disable done
#define HwLSTATUS_RU                    Hw3                                 // Register update flag
#define HwLSTATUS_VSF                   Hw2                                 // VS falling flag
#define HwLSTATUS_VSR                   Hw1                                 // VS rising flag
#define HwLSTATUS_FU                    Hw0                                 // LCD output fifo under-run flag.

#include "structures_display.h"

#define HwVIOC_BASE                             (0x72000000)

#define BASE_ADDR_CPUIF                         (0x72100000)
#define BASE_ADDR_VIOC                          HwVIOC_BASE
/* DISP */
#define HwVIOC_DISP0            ( (BASE_ADDR_VIOC  + 0x0000 + 0x0000)) // 64 words    // TCC8910 | TCC8010
#define HwVIOC_DISP1            ( (BASE_ADDR_VIOC  + 0x0000 + 0x0100)) // 64 words    // TCC8910 
#define HwVIOC_DISP2            ( (BASE_ADDR_VIOC  + 0x0000 + 0x0200)) // 64 words    // TCC8910

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

#if defined(VIOC_TCC8970)
#define HwVIOC_MC0              ( (BASE_ADDR_VIOC  + 0x0000 + 0x1600)) // 64 words    // TCC8910
#define HwVIOC_MC1              ( (BASE_ADDR_VIOC  + 0x0000 + 0x1700)) // 64 words    // TCC8910
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

#if defined(VIOC_TCC8970)
#define HwVIOC_DISP0_MIX              ( (BASE_ADDR_VIOC  + 0x0000 + 0x3200)) // 64 words    // TCC8910
#define HwVIOC_DISP0_MD              ( (BASE_ADDR_VIOC  + 0x0000 + 0x3300)) // 64 words    // TCC8910
#define HwVIOC_DISP1_MIX              ( (BASE_ADDR_VIOC  + 0x0000 + 0x3400)) // 64 words    // TCC8910
#define HwVIOC_DISP1_MD              ( (BASE_ADDR_VIOC  + 0x0000 + 0x3500)) // 64 words    // TCC8910
#define HwVIOC_DISP2_MIX              ( (BASE_ADDR_VIOC  + 0x0000 + 0x3600)) // 64 words    // TCC8910
#define HwVIOC_DISP2_MD              ( (BASE_ADDR_VIOC  + 0x0000 + 0x3700)) // 64 words    // TCC8910
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

//*********************************************************************************************************************************************
// [15:14] == 2'b11
//*********************************************************************************************************************************************
/* VIOC TIMER */
#define HwVIOC_TIMER_BASE       ( (BASE_ADDR_VIOC  + 0xC000        ))                // TCC8910 | TCC8010
#define HwVIOC_TIMER            ( (BASE_ADDR_VIOC  + 0xC000        ))

/* VIQE */
#define HwVIOC_VIQE0_BASE       ( (BASE_ADDR_VIOC  + 0xD000        ))                // TCC8910 | TCC8010
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_VIQE1_BASE       ( (BASE_ADDR_VIOC  + 0xE000        ))                // TCC8910 
#endif
#define  HwVIOC_VIQE0                           (HwVIOC_VIQE0_BASE)
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define  HwVIOC_VIQE1                           (HwVIOC_VIQE1_BASE)
#define HwVIOC_MMU_BASE         ( (BASE_ADDR_VIOC  + 0xF000        ))                // TCC8910 
#define HwVIOC_MMU              ( (BASE_ADDR_VIOC  + 0xF000        ))
#endif


/* CPUIF */
#define HwVIOC_CPUIF            ( (BASE_ADDR_CPUIF + 0x0000        ))                // 
#define HwVIOC_CPUIF0           ( (BASE_ADDR_CPUIF + 0x0000        ))                // TCC8910 | TCC8010
// [09:08] == 2'b00,
// [09:08] == 2'b00, [07:06] == 2'b01, [05] == 1'b0
// [09:08] == 2'b00, [07:06] == 2'b01, [05] == 1'b1
// [09:08] == 2'b00, [07:06] == 2'b10, [05] == 1'b0
// [09:08] == 2'b00, [07:06] == 2'b10, [05] == 1'b1
#if defined (VIOC_TCC8930) || defined (VIOC_TCC8970)
#define HwVIOC_CPUIF1           ( (BASE_ADDR_CPUIF + 0x0100        ))                // TCC8910
#endif


/* CFG_DEV_SEL WMIXER <--> DISP setting path*/
#define 	P2_EN				Hw23
#define	DEV2_PATH			(Hw16|Hw17)
#define	P1_EN				Hw15
#define	DEV1_PATH			(Hw8|Hw9)
#define	P0_EN				Hw7
#define	DEV0_PATH			(Hw0|Hw1)

/* CFG_MISC1 DISP <--> output (HW) setting path*/

#define	LCD2_SEL			(Hw28|Hw29)
#define	LCD1_SEL			(Hw26|Hw27)
#define	LCD0_SEL			(Hw24|Hw25)


// [09:08] == 2'b01,
// [09:08] == 2'b01, [07:06] == 2'b01, [05] == 1'b0
// [09:08] == 2'b01, [07:06] == 2'b01, [05] == 1'b1
// [09:08] == 2'b01, [07:06] == 2'b10, [05] == 1'b0
// [09:08] == 2'b01, [07:06] == 2'b10, [05] == 1'b1
/* OUT CONFIGURE */
#define HwVIOC_OUTCFG           ( (BASE_ADDR_CPUIF + 0x0200        )) // [09:08] == 2'b10, 


#define HwNTSCPAL_BASE                          (0x72200000)
#define HwTVE_BASE								(0x72200000)
#define HwNTSCPAL_ENC_CTRL_BASE                 (0x72200800)

// Encoder Mode Control A
#define	HwTVECMDA_PWDENC_PD							Hw7											// Power down mode for entire digital logic of TV encoder
#define	HwTVECMDA_FDRST_1							Hw6											// Chroma is free running as compared to H-sync
#define	HwTVECMDA_FDRST_0							HwZERO										// Relationship between color burst & H-sync is maintained for video standards
#define	HwTVECMDA_FSCSEL(X)							((X)*Hw4)
#define	HwTVECMDA_FSCSEL_NTSC						HwTVECMDA_FSCSEL(0)							// Color subcarrier frequency is 3.57954545 MHz for NTSC
#define	HwTVECMDA_FSCSEL_PALX						HwTVECMDA_FSCSEL(1)							// Color subcarrier frequency is 4.43361875 MHz for PAL-B,D,G,H,I,N
#define	HwTVECMDA_FSCSEL_PALM						HwTVECMDA_FSCSEL(2)							// Color subcarrier frequency is 3.57561149 MHz for PAL-M
#define	HwTVECMDA_FSCSEL_PALCN						HwTVECMDA_FSCSEL(3)							// Color subcarrier frequency is 3.58205625 MHz for PAL-combination N
#define	HwTVECMDA_FSCSEL_MASK						HwTVECMDA_FSCSEL(3)
#define	HwTVECMDA_PEDESTAL							Hw3											// Video Output has a pedestal
#define	HwTVECMDA_NO_PEDESTAL						HwZERO										// Video Output has no pedestal
#define	HwTVECMDA_PIXEL_SQUARE						Hw2											// Input data is at square pixel rates.
#define	HwTVECMDA_PIXEL_601							HwZERO										// Input data is at 601 rates.
#define	HwTVECMDA_IFMT_625							Hw1											// Output data has 625 lines
#define	HwTVECMDA_IFMT_525							HwZERO										// Output data has 525 lines
#define	HwTVECMDA_PHALT_PAL							Hw0											// PAL encoded chroma signal output
#define	HwTVECMDA_PHALT_NTSC						HwZERO										// NTSC encoded chroma signal output

// Encoder Mode Control B
#define	HwTVECMDB_YBIBLK_BLACK						Hw4											// Video data is forced to Black level for Vertical non VBI processed lines.
#define	HwTVECMDB_YBIBLK_BYPASS						HwZERO										// Input data is passed through forn non VBI processed lines.
#define	HwTVECMDB_CBW(X)							((X)*Hw2)
#define	HwTVECMDB_CBW_LOW							HwTVECMDB_CBW(0)							// Low Chroma band-width
#define	HwTVECMDB_CBW_MEDIUM						HwTVECMDB_CBW(1)							// Medium Chroma band-width
#define	HwTVECMDB_CBW_HIGH							HwTVECMDB_CBW(2)							// High Chroma band-width
#define	HwTVECMDB_CBW_MASK							HwTVECMDB_CBW(3)							// 
#define	HwTVECMDB_YBW(X)							((X)*Hw0)
#define	HwTVECMDB_YBW_LOW							HwTVECMDB_YBW(0)							// Low Luma band-width
#define	HwTVECMDB_YBW_MEDIUM						HwTVECMDB_YBW(1)							// Medium Luma band-width
#define	HwTVECMDB_YBW_HIGH							HwTVECMDB_YBW(2)							// High Luma band-width
#define	HwTVECMDB_YBW_MASK							HwTVECMDB_YBW(3)							// 

// Encoder Clock Generator
#define	HwTVEGLK_XT24_24MHZ							Hw4											// 24MHz Clock input
#define	HwTVEGLK_XT24_27MHZ							HwZERO										// 27MHz Clock input
#define	HwTVEGLK_GLKEN_RST_EN						Hw3											// Reset Genlock
#define	HwTVEGLK_GLKEN_RST_DIS						~Hw3										// Release Genlock
#define	HwTVEGLK_GLKE(X)							((X)*Hw1)
#define	HwTVEGLK_GLKE_INT							HwTVEGLK_GLKE(0)							// Chroma Fsc is generated from internal constants based on current user setting
#define	HwTVEGLK_GLKE_RTCO							HwTVEGLK_GLKE(2)							// Chroma Fsc is adjusted based on external RTCO input
#define	HwTVEGLK_GLKE_CLKI							HwTVEGLK_GLKE(3)							// Chroma Fsc tracks non standard encoder clock (CLKI) frequency
#define	HwTVEGLK_GLKE_MASK							HwTVEGLK_GLKE(3)							//
#define	HwTVEGLK_GLKEN_GLKPL_HIGH					Hw0											// PAL ID polarity is active high
#define	HwTVEGLK_GLKEN_GLKPL_LOW					HwZERO										// PAL ID polarity is active low

// Encoder Mode Control C
#define	HwTVECMDC_CSMDE_EN							Hw7											// Composite Sync mode enabled
#define	HwTVECMDC_CSMDE_DIS							~Hw7										// Composite Sync mode disabled (pin is tri-stated)
#define	HwTVECMDC_CSMD(X)							((X)*Hw5)
#define	HwTVECMDC_CSMD_CSYNC						HwTVECMDC_CSMD(0)							// CSYN pin is Composite sync signal
#define	HwTVECMDC_CSMD_KEYCLAMP						HwTVECMDC_CSMD(1)							// CSYN pin is Keyed clamp signal
#define	HwTVECMDC_CSMD_KEYPULSE						HwTVECMDC_CSMD(2)							// CSYN pin is Keyed pulse signal
#define	HwTVECMDC_CSMD_MASK							HwTVECMDC_CSMD(3)
#define	HwTVECMDC_RGBSYNC(X)						((X)*Hw3)
#define	HwTVECMDC_RGBSYNC_NOSYNC					HwTVECMDC_RGBSYNC(0)						// Disable RGBSYNC (when output is configured for analog EGB mode)
#define	HwTVECMDC_RGBSYNC_RGB						HwTVECMDC_RGBSYNC(1)						// Sync on RGB output signal (when output is configured for analog EGB mode)
#define	HwTVECMDC_RGBSYNC_G							HwTVECMDC_RGBSYNC(2)						// Sync on G output signal (when output is configured for analog EGB mode)
#define	HwTVECMDC_RGBSYNC_MASK						HwTVECMDC_RGBSYNC(3)

// DAC Output Selection
#define	HwTVEDACSEL_DACSEL_CODE0					HwZERO										// Data output is diabled (output is code '0')
#define	HwTVEDACSEL_DACSEL_CVBS						Hw0											// Data output in CVBS format

// DAC Power Down
#define	HwTVEDACPD_PD_EN							Hw0											// DAC Power Down Enabled
#define	HwTVEDACPD_PD_DIS							~Hw0										// DAC Power Down Disabled

// Sync Control
#define	HwTVEICNTL_FSIP_ODDHIGH						Hw7											// Odd field active high
#define	HwTVEICNTL_FSIP_ODDLOW						HwZERO										// Odd field active low
#define	HwTVEICNTL_VSIP_HIGH						Hw6											// V-sync active high
#define	HwTVEICNTL_VSIP_LOW							HwZERO										// V-sync active low
#define	HwTVEICNTL_HSIP_HIGH						Hw5											// H-sync active high
#define	HwTVEICNTL_HSIP_LOW							HwZERO										// H-sync active low
#define	HwTVEICNTL_HSVSP_RISING						Hw4											// H/V-sync latch enabled at rising edge
#define	HwTVEICNTL_HVVSP_FALLING					HwZERO										// H/V-sync latch enabled at falling edge
#define	HwTVEICNTL_VSMD_START						Hw3											// Even/Odd field H/V sync output are aligned to video line start
#define	HwTVEICNTL_VSMD_MID							HwZERO										// Even field H/V sync output are aligned to video line midpoint
#define	HwTVEICNTL_ISYNC(X)							((X)*Hw0)
#define	HwTVEICNTL_ISYNC_FSI						HwTVEICNTL_ISYNC(0)							// Alignment input format from FSI pin
#define	HwTVEICNTL_ISYNC_HVFSI						HwTVEICNTL_ISYNC(1)							// Alignment input format from HSI,VSI,FSI pin
#define	HwTVEICNTL_ISYNC_HVSI						HwTVEICNTL_ISYNC(2)							// Alignment input format from HSI,VSI pin
#define	HwTVEICNTL_ISYNC_VFSI						HwTVEICNTL_ISYNC(3)							// Alignment input format from VSI,FSI pin
#define	HwTVEICNTL_ISYNC_VSI						HwTVEICNTL_ISYNC(4)							// Alignment input format from VSI pin
#define	HwTVEICNTL_ISYNC_ESAV_L						HwTVEICNTL_ISYNC(5)							// Alignment input format from EAV,SAV codes (line by line)
#define	HwTVEICNTL_ISYNC_ESAV_F						HwTVEICNTL_ISYNC(6)							// Alignment input format from EAV,SAV codes (frame by frame)
#define	HwTVEICNTL_ISYNC_FREE						HwTVEICNTL_ISYNC(7)							// Alignment is free running (Master mode)
#define	HwTVEICNTL_ISYNC_MASK						HwTVEICNTL_ISYNC(7)

// Offset Control
#define	HwTVEHVOFFST_INSEL(X)						((X)*Hw6)
#define	HwTVEHVOFFST_INSEL_BW16_27MHZ				HwTVEHVOFFST_INSEL(0)						// 16bit YUV 4:2:2 sampled at 27MHz
#define	HwTVEHVOFFST_INSEL_BW16_13P5MH				HwTVEHVOFFST_INSEL(1)						// 16bit YUV 4:2:2 sampled at 13.5MHz
#define	HwTVEHVOFFST_INSEL_BW8_13P5MHZ				HwTVEHVOFFST_INSEL(2)						// 8bit YUV 4:2:2 sampled at 13.5MHz
#define	HwTVEHVOFFST_INSEL_MASK						HwTVEHVOFFST_INSEL(3)
#define	HwTVEHVOFFST_VOFFST_256						Hw3											// Vertical offset bit 8 (Refer to HwTVEVOFFST)
#define	HwTVEHVOFFST_HOFFST_1024					Hw2											// Horizontal offset bit 10 (Refer to HwTVEHOFFST)
#define	HwTVEHVOFFST_HOFFST_512						Hw1											// Horizontal offset bit 9 (Refer to HwTVEHOFFST)
#define	HwTVEHVOFFST_HOFFST_256						Hw0											// Horizontal offset bit 8 (Refer to HwTVEHOFFST)

// Sync Output Control
#define	HwTVEHSVSO_VSOB_256							Hw6											// VSOB bit 8 (Refer to HwVSOB)
#define	HwTVEHSVSO_HSOB_1024						Hw5											// HSOB bit 10 (Refer to HwHSOB)
#define	HwTVEHSVSO_HSOB_512							Hw4											// HSOB bit 9 (Refer to HwHSOB)
#define	HwTVEHSVSO_HSOB_256							Hw3											// HSOB bit 8 (Refer to HwHSOB)
#define	HwTVEHSVSO_HSOE_1024						Hw2											// HSOE bit 10 (Refer to HwHSOE)
#define	HwTVEHSVSO_HSOE_512							Hw1											// HSOE bit 9 (Refer to HwHSOE)
#define	HwTVEHSVSO_HSOE_256							Hw0											// HSOE bit 8 (Refer to HwHSOE)

// Trailing Edge of Vertical Sync Control
#define	HwTVEVSOE_VSOST(X)							((X)*Hw6)									// Programs V-sync relative location for Odd/Even Fields.
#define	HwTVEVSOE_NOVRST_EN							Hw5											// No vertical reset on every field
#define	HwTVEVSOE_NOVRST_NORMAL						HwZERO										// Normal vertical reset operation (interlaced output timing)
#define	HwTVEVSOE_VSOE(X)							((X)*Hw0)									// Trailing Edge of Vertical Sync Control

// VBI Control Register
#define	HwTVEVCTRL_VBICTL(X)						((X)*Hw5)									// VBI Control indicating the current line is VBI.
#define	HwTVEVCTRL_VBICTL_NONE						HwTVEVCTRL_VBICTL(0)						// Do nothing, pass as active video.
#define	HwTVEVCTRL_VBICTL_10LINE					HwTVEVCTRL_VBICTL(1)						// Insert blank(Y:16, Cb,Cr: 128), for example, 10 through 21st line.
#define	HwTVEVCTRL_VBICTL_1LINE						HwTVEVCTRL_VBICTL(2)						// Insert blank data 1 line less for CC processing.
#define	HwTVEVCTRL_VBICTL_2LINE						HwTVEVCTRL_VBICTL(3)						// Insert blank data 2 line less for CC and CGMS processing.
#define	HwTVEVCTRL_MASK								HwTVEVCTRL_VBICTL(3)					
#define	HwTVEVCTRL_CCOE_EN							Hw4											// Closed caption odd field enable.
#define	HwTVEVCTRL_CCEE_EN							Hw3											// Closed caption even field enable.
#define	HwTVEVCTRL_CGOE_EN							Hw2											// Copy generation management system enable odd field.
#define	HwTVEVCTRL_CGEE_EN							Hw1											// Copy generation management system enable even field.
#define	HwTVEVCTRL_WSSE_EN							Hw0											// Wide screen enable.

// Connection between LCDC & TVEncoder Control
#define	HwTVEVENCON_EN_EN							Hw0											// Connection between LCDC & TVEncoder Enabled
#define	HwTVEVENCON_EN_DIS							~Hw0										// Connection between LCDC & TVEncoder Disabled

// I/F between LCDC & TVEncoder Selection
#define	HwTVEVENCIF_MV_1							Hw1											// reserved
#define	HwTVEVENCIF_FMT_1							Hw0											// PXDATA[7:0] => CIN[7:0], PXDATA[15:8] => YIN[7:0]
#define	HwTVEVENCIF_FMT_0							HwZERO										// PXDATA[7:0] => YIN[7:0], PXDATA[15:8] => CIN[7:0]




#define HwHDMI_CTRL_BASE                        (0x72300000)
#define HwHDMI_CORE_BASE                        (0x72310000)
#define HwHDMI_AES_BASE                         (0x72320000)
#define HwHDMI_SPDIF_BASE                       (0x72330000)
#define HwHDMI_I2S_BASE                         (0x72340000)
#define HwHDMI_CEC_BASE                         (0x72350000)

#define HwMIPI_DSI         				 ((BASE_ADDR_VIOC  + 0x3C0000)) // by sukhyun @ 2012-09-17 12:02:05

#define HwHDMI_BASE						(0x72300000)

/************************************************************************
*   DDI_CONFIG (Base Addr = 0x72380000) // R/W
*************************************************************************/

#define HwDDI_CONFIG_BASE                       (0x72380000)

// Power Down
#define HwDDIC_PWDN_G2D					Hw3		//G2D
#define HwDDIC_PWDN_HDMI                      Hw2           // HDMI
#define HwDDIC_PWDN_NTSC                      Hw1            // NTSL/PAL
#define HwDDIC_PWDN_LCDC                       Hw0         // LCDC

// Soft Reset
#define HwDDIC_PWDN_G2D					Hw3		//G2D
#define HwDDIC_SWRESET_HDMI                  Hw2           // HDMI
#define HwDDIC_SWRESET_NTSC                  Hw1              // NTSL/PAL
#define HwDDIC_SWRESET_LCDC                   Hw0             // LCDC

/*******************************************************************************
*
*	TCC897X DataSheet PART 8 VIDEO BUS
*
********************************************************************************/

#define HwVBUS_BASE						(0x75000000)

#define HwVBUS_ARB_CTL_BASE					(0x75100000)

#define HwVIDEOBUSCONFIG_BASE					(0x75100000)

#define HwVBUS_JPEG_ENC_BASE					(0x75300000)


/*******************************************************************************
*
*	TCC897X DataSheet PART 9 Cortex-M4 BUS
*
********************************************************************************/

#include "structures_cm4.h"
#define HwCORTEXM4_PKT_GEN_BASE					(0x79000000)
#define HwCORTEXM4_TS_CIPHER_BASE				(0x79010000)
#define HwCORTEXM4_SECTION_FILTER_BASE			(0x79020000)
#define HwCORTEXM4_PKT_MEM_BASE					(0x79030000)
#define HwCORTEXM4_CODE_MEM_BASE				(0x79080000)
#define HwCORTEXM4_DATA_MEM_BASE				(0x79090000)
#define HwCORTEXM4_MAILBOX0_BASE				(0x790A0000)
#define HwCORTEXM4_MAILBOX1_BASE				(0x790B0000)
#define HwCORTEXM4_TSD_CFG_BASE					(0x790F0000)


/*******************************************************************************
*
*	ARM BUS
*
********************************************************************************/

#define HwARM_CPU_BASE						(0x77000000)
#define HwARM_GIC_DIST_BASE					(0x77101000)
#define HwARM_GIC_CPU_BASE					(0x77102000)


#endif /* _PLATFORM_REG_PHYSICAL_H_ */
