/****************************************************************************
 *
 * Copyright (C) 2016 Telechips Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions
 * andlimitations under the License.
 ****************************************************************************/

#ifndef _PLATFORM_TCC_CKC_H_
#define _PLATFORM_TCC_CKC_H_

/* Peripheral Clocks */
enum {/* Peri. Name */
	PERI_TCX = 0,		/* X Clock of Timer (PCLKCTRL00) */
	PERI_TCT,		/* T Clock of Timer (PCLKCTRL01) */
	PERI_TCZ,		/* Z Clock of Timer (PCLKCTRL02) */
	PERI_LCDTIMER,		/* Timer Clock of LCD (PCLKCTRL03) */
	PERI_LCD0,		/* Core Clock of LCD0 (PCLKCTRL05) */
	PERI_LCD1,		/* Core Clock of LCD1 (PCLKCTRL05) */
	PERI_LCD_CPUIF,		/* Core Clock of LCD CPU I/F (PCLKCTRL06) */
	PERI_SFMC,		/* Core Clock of SFMC (PCLKCTRL07) */	
	PERI_PHY_HSIC,		/* PHY Clock of HSIC (PCLKCTRL08) */
	PERI_GMAC,		/* Core Clock of GMAC (PCLKCTRL09) */
	PERI_GMAC_PTP,		/* PTP Clock of GMAC (PCLKCTRL10) */
	PERI_PCIE0_AUX,		/* AUX Clock of PCIE0 (PCLKCTRL11) */
	PERI_PCIE0_REF_EXT,	/* Ext. Ref. Clock of PCIE0 (PCLKCTRL12) */
	PERI_PCIE1_AUX,		/* AUX Clock of PCIE0 (PCLKCTRL13) */
	PERI_PCIE1_REF_EXT,	/* Ext. Ref. Clock of PCIE0 (PCLKCTRL14) */
	PERI_USB20H,		/* Core Clock of USB20HOST (PCLKCTRL15) */
	PERI_USB_OTG_ADP,	/* ADP Clock of USBOTG (PCLKCTRL16) */
	PERI_USB_OTG,		/* Core Clock of USBOTG (PCLKCTRL17) */
	PERI_HSIC,		/* Core Clock of HSIC (PCLKCTRL18) */
	PERI_RMT,		/* Core Clock of Remote (PCLKCTRL19) */
	PERI_SDMMC0,		/* Core Clock of SDMMC0 (PCLKCTRL20) */ 
	PERI_SDMMC1,		/* Core Clock of SDMMC1 (PCLKCTRL21) */
	PERI_SDMMC2,		/* Core Clock of SDMMC2 (PCLKCTRL22) */
	PERI_MDAI0,		/* Core Clock of Audio0 (PCLKCTRL23) */
	PERI_MFLT0_DAI,		/* Filter Clock of Audio0 (PCLKCTRL24) */
	PERI_MSPDIF0,		/* SPDIF Clock of Audio0 (PCLKCTRL25) */
	PERI_MDAI1,		/* Core Clock of Audio1 (PCLKCTRL26) */
	PERI_MFLT1_DAI,		/* Filter Clock of Audio1 (PCLKCTRL27) */
	PERI_MSPDIF1,		/* SPDIF Clock of Audio1 (PCLKCTRL28) */
	PERI_PDM,		/* Core Clock of PDM (PCLKCTRL29) */
	PERI_TSADC,		/* Core Clock of TSADC (PCLKCTRL30) */
	PERI_I2C0,		/* Clock of I2C Master0 (PCLKCTRL31) */
	PERI_I2C1,		/* Clock of I2C Master1 (PCLKCTRL32) */
	PERI_I2C2,		/* Clock of I2C Master2 (PCLKCTRL33) */
	PERI_I2C3,		/* Clock of I2C Master3 (PCLKCTRL34) */
	PERI_UART0,		/* Core Clock of UART0 (PCLKCTRL35) */
	PERI_UART1,		/* Core Clock of UART1 (PCLKCTRL36) */
	PERI_UART2,		/* Core Clock of UART2 (PCLKCTRL37) */
	PERI_UART3,		/* Core Clock of UART3 (PCLKCTRL38) */
	PERI_UART4,		/* Core Clock of UART4 (PCLKCTRL39) */
	PERI_UART5,		/* Core Clock of UART5 (PCLKCTRL40) */
	PERI_UART6,		/* Core Clock of UART6 (PCLKCTRL41) */
	PERI_UART7,		/* Core Clock of UART7 (PCLKCTRL42) */
	PERI_GPSB0,		/* Core Clock of GPSB0 (PCLKCTRL43) */
	PERI_GPSB1,		/* Core Clock of GPSB1 (PCLKCTRL44) */
	PERI_GPSB2,		/* Core Clock of GPSB2 (PCLKCTRL45) */
	PERI_GPSBMS0,		/* Core Clock of GPSB0 w/o DMA (PCLKCTRL46) */
	PERI_GPSBMS1,		/* Core Clock of GPSB1 w/o DMA (PCLKCTRL47) */
	PERI_GPSBMS2,		/* Core Clock of GPSB2 w/o DMA (PCLKCTRL48) */
	PERI_I2C4,		/* Clock of I2C Master4 (PCLKCTRL49) */
	PERI_I2C5,		/* Clock of I2C Master5 (PCLKCTRL50) */
	PERI_MFLT0_CDIF,	/* Filter Clock of Audio0(CDIF) (PCLKCTRL51) */
	PERI_I2C6,		/* Clock of I2C Master6 (PCLKCTRL52) */
	PERI_I2C7,		/* Clock of I2C Master7 (PCLKCTRL53) */
	PERI_MFLT1_CDIF,	/* Filter Clock of Audio1(CDIF) (PCLKCTRL54) */
	PERI_IC_TIMER0,		/* Clock of Input Capture Timer0 (PCLKCTRL55) */
	PERI_MDAI2,		/* Core Clock of Audio2 (PCLKCTLR56) */
	PERI_MFLT2_DAI,		/* Filter Clock of Audio2 (PCLKCTRL57) */
	PERI_MFLT2_CDIF,	/* Filter Clock of Audio2(CDIF) (PCLKCTRL58) */
	PERI_MSPDIF2,		/* SPDIF Clock of Audio2 (PCLKCTRL59) */
	PERI_MDAI3,		/* Core Clock of Audio3 (PCLKCTRL60) */
	PERI_MFLT3_DAI,		/* Filter Clock of Audio3 (PCLKCTRL61) */
	PERI_MFLT3_CDIF,	/* Filter Clock of Audio3(CDIF) (PCLKCTRL62) */
	PERI_MSPDIF3,		/* SPDIF Clock of Audio3 (PCLKCTRL63) */
	PERI_IC_TIMER1,		/* Clock of Input Capture Timer1 (PCLKCTRL64) */
	PERI_IC_TIMER2,		/* Clock of Input Capture Timer2 (PCLKCTRL65) */
	PERI_ADC0,		/* Core Clock of ADC0 (PCLKCTRL66) */
	PERI_ADC1,		/* Core Clock of ADC1 (PCLKCTRL67) */
	PERI_CAN,		/* Core Clock of CAN (PCLKCTRL68) */
	PERI_TSRX0,		/* Counter Clock of TS RX0 (PCLKCTRL69) */
	PERI_TSRX1,		/* Counter Clock of TS RX1 (PCLKCTRL70) */
	PERI_TSRX2,		/* Counter Clock of TS RX2 (PCLKCTRL71) */
	PERI_TSRX3,		/* Counter Clock of TS RX3 (PCLKCTRL72) */
	PERI_CB_WDT,		/* Clock of Watch-Dog Timer(CBUS) (PCLKCTRL73) */
	PERI_CB_STAMP,		/* Clock of Time Stamp(CBUS) (PCLKCTRL74) */
	PERI_OUT0,		/* Ext. Peri-Clock Out of Port 0 (PCLKCTRL75) */
	PERI_OUT1,		/* Ext. Peri-Clock Out of Port 1 (PCLKCTRL76) */
	PERI_OUT2,		/* Ext. Peri-Clock Out of Port 2 (PCLKCTRL77) */
	PERI_OUT3,		/* Ext. Peri-Clock Out of Port 3 (PCLKCTRL78) */
	PERI_OUT4,		/* Ext. Peri-Clock Out of Port 4 (PCLKCTRL79) */
	PERI_OUT5,		/* Ext. Peri-Clock Out of Port 5 (PCLKCTRL80) */
	PERI_PCIE0_APB,		/* APB Clock of PCIe PHY0 (PCLKCTRL81) */
	PERI_PCIE1_APB,		/* APB Clock of PCIe PHY1 (PCLKCTRL82) */
	PERI_TSRX_FLT,		/* Filter Clock of TS RX Reg. Slice (PCLKCTRL83) */
	PERI_PLL_IN,		/* PLL Input Source Clock (PCLKCTRL84) */
	PERI_AUX_IN0,		/* AUX Reference Clock-In0 (PCLKCTRL85) */
	PERI_AUX_IN1,		/* AUX Reference Clock-In1 (PCLKCTRL86) */
	PERI_AUX_OUT0,		/* AUX Reference Clock-Out0 (PCLKCTRL87) */
	PERI_AUX_OUT1,		/* AUX Reference Clock-Out1 (PCLKCTRL88) */
	PERI_MAX,
};

/* I/O Bus pwdn/swreset */
enum {
	IOBUS_IC_TC0,		//  0
	IOBUS_IC_TC1,		//  1
	IOBUS_IC_TC2,		//  2
	IOBUS_CAN,		//  3
	IOBUS_ADC,		//  4
	IOBUS_ASRC_DMA,		//  5
	IOBUS_SRC,		//  6
	IOBUS_DMA0,		//  7
	IOBUS_DMA1,		//  8
	IOBUS_DMA2,		//  9
	IOBUS_DMA,		// 10
	IOBUS_RESERVED11,	// 11
	IOBUS_PWM,		// 12
	IOBUS_RESERVED13,	// 13
	IOBUS_I2C_S2,		// 14
	IOBUS_RESERVED15,	// 15
	IOBUS_REMOCON,		// 16
	IOBUS_TSADC,		// 17
	IOBUS_RESERVED18,	// 18
	IOBUS_RESERVED19,	// 19
	IOBUS_PROT,		// 20
	IOBUS_ADMA0,		// 21
	IOBUS_DAI0,		// 22
	IOBUS_SPDIF0,		// 23
	IOBUS_AUDIO0,		// 24
	IOBUS_ADMA1,		// 25
	IOBUS_DAI1,		// 26
	IOBUS_SPDIF1,		// 27
	IOBUS_AUDIO1,		// 28
	IOBUS_I2C_M0,		// 29
	IOBUS_I2C_M1,		// 30
	IOBUS_I2C_M2,		// 31
	IOBUS_I2C_M3,		// 32  0
	IOBUS_I2C_S0,		// 33  1
	IOBUS_I2C_S1,		// 34  2
	IOBUS_I2C0,		// 35  3
	IOBUS_I2C1,		// 36  4
	IOBUS_ADMA2,		// 37  5
	IOBUS_DAI2,		// 38  6
	IOBUS_SPDIF2,		// 39  7
	IOBUS_AUDIO2,		// 40  8
	IOBUS_ADMA3,		// 41  9
	IOBUS_DAI3,		// 42 10
	IOBUS_SPDIF3,		// 43 11
	IOBUS_AUDIO3,		// 44 12
	IOBUS_RESERVED45,	// 45 13
	IOBUS_RESERVED46,	// 46 14
	IOBUS_RESERVED47,	// 47 15
	IOBUS_RESERVED48,	// 48 16
	IOBUS_GPSB_0,		// 49 17
	IOBUS_RESERVED50,	// 50 18
	IOBUS_GPSB0,		// 51 19
	IOBUS_GPSB1,		// 52 20
	IOBUS_GPSB2,		// 53 21
	IOBUS_GPSB3,		// 54 22
	IOBUS_GPSB4,		// 55 23
	IOBUS_GPSB5,		// 56 24
	IOBUS_GPSB_1,		// 57 25
	IOBUS_RESERVED58,	// 58 26
	IOBUS_I2C_S3,		// 59 27
	IOBUS_I2C_M4,		// 60 28
	IOBUS_I2C_M5,		// 61 29
	IOBUS_I2C_M6,		// 62 30
	IOBUS_I2C_M7,		// 63 31
	IOBUS_UART0,		// 64  0
	IOBUS_UART1,		// 65  1
	IOBUS_UART2,		// 66  2
	IOBUS_UART3,		// 67  3
	IOBUS_UART4,		// 68  4
	IOBUS_UART5,		// 69  5
	IOBUS_UART6,		// 70  6
	IOBUS_UART7,		// 71  7
	IOBUS_SMARTCARD0,	// 72  8
	IOBUS_RESERVED73,	// 73  9
	IOBUS_RESERVED74,	// 74 10
	IOBUS_RESERVED75,	// 75 11
	IOBUS_RESERVED76,	// 76 12
	IOBUS_RESERVED77,	// 77 13
	IOBUS_RESERVED78,	// 78 14
	IOBUS_RESERVED79,	// 79 15
	IOBUS_UDMA0,		// 80 16
	IOBUS_UDMA1,		// 81 17
	IOBUS_UDMA2,		// 82 18
	IOBUS_UDMA3,		// 83 19
	IOBUS_UART_SMARTCARD0,	// 84 20
	IOBUS_UART_SMARTCARD1,	// 85 21
	IOBUS_RESERVED86,	// 86 22
	IOBUS_RESERVED87,	// 87 23
	IOBUS_RESERVED88,	// 88 24
	IOBUS_RESERVED89,	// 89 25
	IOBUS_RESERVED90,	// 90 26
	IOBUS_RESERVED91,	// 91 27
	IOBUS_RESERVED92,	// 92 28
	IOBUS_RESERVED93,	// 93 29
	IOBUS_RESERVED94,	// 94 30
	IOBUS_RESERVED95,	// 95 31
	IOBUS_GDMA1,            // 96  0
	IOBUS_CIPHER,           // 97  1
	IOBUS_SDMMC0,           // 98  2
	IOBUS_SDMMC1,           // 99  3
	IOBUS_SDMMC2,           //100  4
	IOBUS_SDMMC,            //101  5
	IOBUS_SMC,              //102  6
	IOBUS_NFC,              //103  7
	IOBUS_EDICFG,           //104  8
	IOBUS_EDI,              //105  9
	IOBUS_RTC,              //106 10
	IOBUS_MAX = 128,
};

#endif /* _PLATFORM_TCC_CKC_H_ */
