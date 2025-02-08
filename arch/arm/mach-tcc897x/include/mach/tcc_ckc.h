/****************************************************************************
 *
 * Copyright (C) 2014 Telechips Inc.
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

#ifndef _TCC_CKC_H_
#define _TCC_CKC_H_


#define XIN_CLK_RATE	(24*1000*1000)	// 24MHz
#define XTIN_CLK_RATE	32768	// 32.768kHz
#define HDMI_CLK_RATE	(27*1000*1000)
#define HDMI_PCLK_RATE	27	// dummy value for set lcdc peri source to hdmi pclk
#define EXT0_CLK_RATE	(24*1000*1000)	// 24MHz
#define EXT1_CLK_RATE	(24*1000*1000)	// 24MHz

#define CKC_DISABLE	0
#define CKC_ENABLE	1
#define CKC_NOCHANGE	2

#define MAX_TCC_PLL	5
#define MAX_CLK_SRC	(MAX_TCC_PLL*2 + 2)	// XIN, XTIN


/* PLL channel index */
enum {
	PLL_0=0,
	PLL_1,
	PLL_2,
	PLL_3,
	PLL_4,
	PLL_DIV_0,
	PLL_DIV_1,
	PLL_DIV_2,
	PLL_DIV_3,
	PLL_DIV_4,
	PLL_XIN,
	PLL_XTIN,
};

/* CLKCTRL channel index */
enum {
	FBUS_CPU0 = 0,		// CLKCTRL0 (Cortex-A7 MP)
	FBUS_CPU1,		// CLKCTRL1 (Cortex-A7 SP)
	FBUS_MEM,		// CLKCTRL2
	FBUS_DDI,		// CLKCTRL3
	FBUS_GPU,		// CLKCTRL4
	FBUS_IO,		// CLKCTRL5
	FBUS_VBUS,		// CLKCTRL6
	FBUS_CODA,		// CLKCTRL7
	FBUS_HSIO,		// CLKCTRL8
	FBUS_SMU,		// CLKCTRL9
	FBUS_G2D,		// CLKCTRL10
	FBUS_CMBUS,		// CLKCTRL11 (Cortex-M4)
	FBUS_HEVC_VCE,		// CLKCTRL12
	FBUS_HEVC_VCPU,		// CLKCTRL13
	FBUS_HEVC_BPU,		// CLKCTRL14
	FBUS_MAX
};

/* Peripheral Clocks */
enum {/* Peri. Name */
	PERI_TCX = 0,		// 0
	PERI_TCT,
	PERI_TCZ,
	PERI_LCD0,
	PERI_LCDSI0,
	PERI_LCD1,		// 5
	PERI_LCDSI1,
	PERI_RESERVED07,
	PERI_LCDTIMER,
	PERI_USB_OTG,
	PERI_GMAC_PTP,		// 10
	PERI_GMAC,
	PERI_USB_OTG_ADP,
	PERI_IOB_CEC,
	PERI_RESERVED14,
	PERI_OUT0,		// 15
	PERI_RESERVED16,
	PERI_HDMI,
	PERI_HDMIA,
	PERI_OUT1,
	PERI_REMOTE,		// 20
	PERI_SDMMC0,
	PERI_SDMMC1,
	PERI_SDMMC2,
	PERI_SDMMC3,
	PERI_ADAI3,		// 25
	PERI_ADAM3,
	PERI_SPDIF3,
	PERI_ADAI0,
	PERI_ADAM0,
	PERI_SPDIF0,		// 30
	PERI_PDM,
	PERI_CMBUS_CNT,
	PERI_ADC,
	PERI_I2C0,
	PERI_I2C1,		// 35
	PERI_I2C2,
	PERI_I2C3,
	PERI_UART0,
	PERI_UART1,
	PERI_UART2,		// 40
	PERI_UART3,
	PERI_UART4,
	PERI_UART5,
	PERI_UART6,
	PERI_UART7,		// 45
	PERI_GPSB0,
	PERI_GPSB1,
	PERI_GPSB2,
	PERI_RESERVED49,
	PERI_RESERVED50,	// 50
	PERI_RESERVED51,
	PERI_I2C6,
	PERI_I2C7,
	PERI_OUT2,
	PERI_OUT3,		// 55
	PERI_OUT4,
	PERI_OUT5,
	PERI_TSTX0,
	PERI_RESERVED59,
	PERI_CMBUS_TSIF0,	// 60
	PERI_CMBUS_TSIF1,
	PERI_CMBUS_TSIF2,
	PERI_CMBUS_TSIF3,
	PERI_RESERVED64,
	PERI_I2C4,		// 65
	PERI_I2C5,
	PERI_HDMI_PCLK,
	PERI_ADAI1,
	PERI_ADAM1,
	PERI_SPDIF1,		// 70
	PERI_ADAI2,
	PERI_ADAM2,
	PERI_SPDIF2,
	PERI_MAX,
};

/* I/O Bus pwdn/swreset */
enum {
	IOBUS_RESERVED00,	//  0
	IOBUS_RESERVED01,	//  1
	IOBUS_SDMMC0,		//  2
	IOBUS_RESERVED03,	//  3
	IOBUS_SDMMC2,		//  4
	IOBUS_SDMMC3,		//  5
	IOBUS_SDMMC,		//  6
	IOBUS_DMA0,		//  7
	IOBUS_DMA1,		//  8
	IOBUS_DMA2,		//  9
	IOBUS_DMA,		// 10
	IOBUS_RESERVED11,	// 11
	IOBUS_PWM,		// 12
	IOBUS_SMC,		// 13
	IOBUS_I2C_S2,		// 14
	IOBUS_RTC,		// 15
	IOBUS_REMOCON,		// 16
	IOBUS_TSADC,		// 17
	IOBUS_EDICFG,		// 18
	IOBUS_EDI,		// 19
	IOBUS_PROT,		// 20
	IOBUS_ADMA0,		// 21
	IOBUS_DAI0,		// 22
	IOBUS_SPDIF0,		// 23
	IOBUS_AUDIO0,		// 24
	IOBUS_ADMA3,		// 25
	IOBUS_DAI3,		// 26
	IOBUS_SPDIF3,		// 27
	IOBUS_AUDIO3,		// 28
	IOBUS_I2C_M0,		// 29
	IOBUS_I2C_M1,		// 30
	IOBUS_I2C_M2,		// 31
	IOBUS_I2C_M3,		// 32  0
	IOBUS_I2C_S0,		// 33  1
	IOBUS_I2C_S1,		// 34  2
	IOBUS_I2C,		// 35  3
	IOBUS_UART0,		// 36  4
	IOBUS_UART1,		// 37  5
	IOBUS_UART2,		// 38  6
	IOBUS_UART3,		// 39  7
	IOBUS_UART4,		// 40  8
	IOBUS_UART5,		// 41  9
	IOBUS_UART6,		// 42 10
	IOBUS_UART7,		// 43 11
	IOBUS_UART,		// 44 12
	IOBUS_RESERVED45,	// 45 13
	IOBUS_NFC,		// 46 14
	IOBUS_RESERVED47,	// 47 15
	IOBUS_RESERVED48,	// 48 16
	IOBUS_RESERVED49,	// 49 17
	IOBUS_TS0,		// 50 18
	IOBUS_GPSB0,		// 51 19
	IOBUS_GPSB1,		// 52 20
	IOBUS_GPSB2,		// 53 21
	IOBUS_RESERVED54,	// 54 22
	IOBUS_RESERVED55,	// 55 23
	IOBUS_RESERVED56,	// 56 24
	IOBUS_GPSB,		// 57 25
	IOBUS_CEC,		// 58 26
	IOBUS_I2C_S3,		// 59 27
	IOBUS_I2C_M4,		// 60 28
	IOBUS_I2C_M5,		// 61 29
	IOBUS_I2C_M6,		// 62 30
	IOBUS_I2C_M7,		// 63 31
	IOBUS_DMAC0,		// 64  0
	IOBUS_DMAC1,		// 65  1
	IOBUS_DMAC2,		// 66  2
	IOBUS_DMAC,		// 67  3
	IOBUS_ADMA1,		// 68  4
	IOBUS_DAI1,		// 69  5
	IOBUS_SPDIF1,		// 70  6
	IOBUS_AUDIO1,		// 71  7
	IOBUS_ADMA2,		// 72  8
	IOBUS_DAI2,		// 73  9
	IOBUS_SPDIF2,		// 74 10
	IOBUS_AUDIO2,		// 75 11
	IOBUS_RESERVED76,	// 76 12
	IOBUS_RESERVED77,	// 77 13
	IOBUS_RESERVED78,	// 78 14
	IOBUS_RESERVED79,	// 79 15
	IOBUS_RESERVED80,	// 80 16
	IOBUS_RESERVED81,	// 81 17
	IOBUS_RESERVED82,	// 82 18
	IOBUS_RESERVED83,	// 83 19
	IOBUS_RESERVED84,	// 84 20
	IOBUS_RESERVED85,	// 85 21
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
	IOBUS_MAX,
};

/* Display Bus pwdn/swreset */
enum{
	DDIBUS_VIOC = 0,
	DDIBUS_NTSCPAL,
	DDIBUS_HDMI,
	DDIBUS_G2D,
	DDIBUS_MAX,
};

/* Graphic Bus pwdn/swreset */
enum{
	GPUBUS_GRP = 0,
	GPUBUS_MAX,
};

/* Video Bus pwdn/swreset */
enum{
	VIDEOBUS_JENC = 0,
	VIDEOBUS_RESERVED01,
	VIDEOBUS_COD,
	VIDEOBUS_VBUS,
	VIDEOBUS_HEVC,
	VIDEOBUS_MAX,
};

/* High-Speed I/O Bus pwdn/swreset */
enum{
	HSIOBUS_DWC_OTG = 0,
	HSIOBUS_RESERVED01,
	HSIOBUS_RESERVED02,
	HSIOBUS_GMAC,
	HSIOBUS_RESERVED04,
	HSIOBUS_RESERVED05,
	HSIOBUS_USB20H,
	HSIOBUS_RESERVED07,
	HSIOBUS_RESERVED08,
	HSIOBUS_RESERVED09,
	HSIOBUS_RESERVED10,
	HSIOBUS_RESERVED11,
	HSIOBUS_RESERVED12,
	HSIOBUS_RESERVED13,
	HSIOBUS_RESERVED14,
	HSIOBUS_RESERVED15,
	HSIOBUS_RESERVED16,
	HSIOBUS_RESERVED17,
	HSIOBUS_CIPHER,
	HSIOBUS_RESERVED19,
	HSIOBUS_RESERVED20,
	HSIOBUS_RESERVED21,
	HSIOBUS_TRNG,
	HSIOBUS_MAX,
};

/* IP Isolation Control Register (PMU_ISOL: 0x7440005C) */
enum{	// bit Name
	PMU_ISOL_OTP = 0,
	PMU_ISOL_RTC,
	PMU_ISOL_PLL,
	PMU_ISOL_ECID,
	PMU_ISOL_HDMI,
	PMU_ISOL_VDAC_3CH,
	PMU_ISOL_VDAC_1CH,
	PMU_ISOL_TSADC,
	PMU_ISOL_USB2P,
	PMU_ISOL_USBOTG,
	PMU_ISOL_RESERVED10,
	PMU_ISOL_LVDS,
	PMU_ISOL_MAX
};

typedef struct {
	unsigned int	fpll;
	unsigned int	en;
	unsigned int	p;
	unsigned int	m;
	unsigned int	s;
	unsigned int	src;
} tPMS;

typedef struct {
	unsigned int	freq;
	unsigned int	en;
	unsigned int	config;
	unsigned int	sel;
} tCLKCTRL;

typedef struct {
	unsigned int	periname;
	unsigned int	freq;
	unsigned int	md;
	unsigned int	en;
	unsigned int	sel;
	unsigned int	div;
} tPCLKCTRL;

extern int tcc_find_pms(tPMS *PLL, unsigned int srcfreq);
extern unsigned long tca_ckc_get_nand_iobus_clk(void);
extern int tcc_ckc_set_hdmi_audio_src(unsigned int src_id);
extern void tcc_ckc_init(void);

#endif /* _PLATFORM_TCC_CKC_H_ */
