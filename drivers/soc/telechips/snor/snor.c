/****************************************************************************
 *   FileName    : snor.c
 *   Description : Seiral Nor Flash Driver
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#include <common.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/tcc_ckc.h>
//#include <dev/gpio.h>
//#include <platform/gpio.h>

#include "snor.h"
#ifdef SNOR_TEST
#include <fwdn/fwupgrade.h>
#endif

PGPSBPORTCFG HwGPSBPORTCFG	= ((PGPSBPORTCFG)HwGPSB_PORTCFG_BASE);
PGPIO pGPIO					= ((PGPIO)HwGPIO_BASE);

#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
PGPSB HwGPSB				= ((PGPSB)HwGPSB0_BASE);
int GPSB_PERI_ID 			= PERI_GPSB0;
#else
PGPSB HwGPSB				= ((PGPSB)HwGPSB3_BASE);
int GPSB_PERI_ID 			= PERI_GPSB3;
#endif

#define SNOR_USE_FAST_READ

// #define SNOR_GPIO	// Not use GPSB: Max Clock < 2MHz  
#define SNOR_GPIO_CS		// CS Control GPIO

#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
	#if defined(TCC897X_GPA) || defined(TCC898X_GPA) 
	#define PORT_CLK	Hw13
	#define PORT_CS 	Hw14
	#define PORT_SI 	Hw15
	#define PORT_SO 	Hw16
	#else
	#define PORT_CLK	Hw8
	#define PORT_CS 	Hw9
	#define PORT_SI 	Hw7
	#define PORT_SO 	Hw10
	#endif
#else
	#define PORT_CLK	Hw12
	#define PORT_CS		Hw13
	#define PORT_SI		Hw15
	#define PORT_SO		Hw14
#endif
	volatile unsigned int *PORT_DATA_REG = NULL;
#endif

#define SNOR_PARTITION_NUM                  2

#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
#define SNOR_BOOT_1ST_AREA_SECTOR_NUM      	16  // lk 1st boot rom area 1M
#define SNOR_BOOT_2ND_AREA_SECTOR_NUM      	16  // lk 2nd boot rom area 1M
#define SNOR_KERNEL_AREA_SECTOR_NUM			112
#else
#define SNOR_BOOT_1ST_AREA_SECTOR_NUM      	4  // lk 1st boot rom area	4K
#define SNOR_BOOT_2ND_AREA_SECTOR_NUM      	4  // lk 2nd boot rom area	4K
#define SNOR_KERNEL_AREA_SECTOR_NUM			120
#endif

#define SNOR_BOOT_AREA_SECTOR_NUM			(SNOR_BOOT_1ST_AREA_SECTOR_NUM + SNOR_BOOT_2ND_AREA_SECTOR_NUM)

#define SNOR_CMD_WRITE_STATUS			0x01
#define SNOR_CMD_PAGE_PROGRAM			0x02
#define SNOR_CMD_READ					0x03
#define SNOR_CMD_WRITE_DISABLE			0x04
#define SNOR_CMD_READ_STATUS			0x05
#define SNOR_CMD_WRITE_ENABLE			0x06
#define SNOR_CMD_FAST_READ				0x0B
#define SNOR_CMD_RDSCUR					0x2B
#define SNOR_CMD_READ_ID				0x9F
#define SNOR_CMD_DEEP_POWER_DOWN		0xB9
#define SNOR_CMD_CHIP_ERASE				0xC7
#define SNOR_CMD_SECTOR_ERASE			0x20
#define SNOR_CMD_BLOCK_ERASE			0xD8
#define SNOR_CMD_READ_BSTATUS 			0x3C
#define SNOR_CMD_GBLK 					0x7E
#define SNOR_CMD_GBULK 					0x98
#define SNOR_CMD_SBULK 					0x39
#define SNOR_CMD_WPSEL 					0x68
#define SNOR_CMD_EXSO 					0xC1
#define SNOR_DUMMPY_DATA				0x00

#define SNOR_ADDRESSED_COMMAND(cmd , addr)			( (((addr)&0xFF)<<24) | (((addr)&0xFF00)<<8) | (((addr)&0xFF0000)>>8) | ((cmd)&0xFF) )

struct SNOR_PRODUCT_INFO {
	char *name;
	unsigned char ManufID;
	unsigned short DevID;
	unsigned int SectorSize;
	unsigned int TotalSector;
	unsigned int fSCLK;
	unsigned int fCLK;
};

int gpsb_clk;
unsigned int uDiv=0;
unsigned int uReadDiv=0;
unsigned int uFReadDiv=0;
unsigned int uWriteDiv=0;
unsigned int uEraseDiv=0;

/* enable/disable debugging messages */
#define	DEBUG_TCC_SNOR
//#undef	DEBUG_TCC_SNOR

#ifdef  DEBUG_TCC_SNOR
# define SNOR_DEBUG(fmt, args...)	printf(fmt ,##args)
#else
# define SNOR_DEBUG(fmt, args...)
#endif


volatile int snor_nop_count = 0;
#define _ASM_NOP_ { snor_nop_count++; }

/************************************************************************************************************

                                        Serial Nor Flash Product Table

************************************************************************************************************/
struct SNOR_PRODUCT_INFO gSNOR_ProductTable[] =
{
		// NAME				// MID	// DID	// Sector Size	// TotalSector	// fSCLK(MHz)	// CLK(MHz)
	{"MXIC-MX25L4005",		0xC2,	0x2013,	0x10000,		8,				20,				20	},	// MXIC, 512KB
	{"MXIC-MX25L8005",		0xC2,	0x2014,	0x10000,		16,				20,				20	},	// MXIC, 1MB
	{"MXIC-MX25L6436E",		0xC2,	0x2017,	0x10000,		128,			80,/*ehk23 high speed can't SNOR read*/				20	},	// MXIC, 8MB
	{"MXIC-MX25L1283F",		0xC2,	0x2018,	0x10000,		256,			80,				20	},	// MXIC, 16MB	
	{"WINBOND-25Q64FVSIG",	0xEF,	0x4017,	0x10000,		128,			80,				20	},	// MXIC, 8MB
};

struct SNOR_PRODUCT_INFO *g_pSNOR_CurrentProduct = 0;

static void loc_SNOR_PortConfig (void)
{
	// Warning : Channel and Port should be 1:1 mapping
	// Ch0, Port 12
#if defined(CONFIG_ARCH_TCC897X)
	#if defined(TCC897X_GPA)
	HwGPSBPORTCFG->PCFG0.nREG = 0x03020001;	// use port 1(GPIO_A[16:13])
	#else
	HwGPSBPORTCFG->PCFG0.nREG = 0x03020109; // use port 9(GPIO_D[10:07])
	#endif
#elif defined(CONFIG_ARCH_TCC898X)
	#if defined(TCC898X_GPA)
	HwGPSBPORTCFG->PCFG0.nREG = 0x03020100; // use port 1(GPIO_A[16:13])
	#else
	HwGPSBPORTCFG->PCFG0.nREG = 0x03000102; // use port 9(GPIO_D[10:07])
	#endif
#else
	HwGPSBPORTCFG->PCFG0.nREG = 0x0302010C;
#endif

	// GPSB Interface Port 12 (SFRM(12)
#ifdef SNOR_GPIO
	#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
		#if defined(TCC897X_GPA) || defined(TCC898X_GPA)
		BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x00000000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000000);
		#else
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x00000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000000);

		BITSET(pGPIO->GPDEN.nREG, PORT_CLK|PORT_CS|PORT_SI);
		BITCLR(pGPIO->GPDEN.nREG, PORT_SO);
		BITCLR(pGPIO->GPDDAT.nREG, PORT_CLK|PORT_CS|PORT_SI);

		PORT_DATA_REG = (unsigned int *)&pGPIO->GPDDAT.nREG;

		#endif
	#else
	BITCSET(pGPIO->GPEFN1.nREG, 0xFFFF0000, 0x00000000);
	BITSET(pGPIO->GPEEN.nREG, PORT_CLK|PORT_CS|PORT_SI);
	BITCLR(pGPIO->GPEEN.nREG, PORT_SO);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CLK|PORT_CS|PORT_SI);

	PORT_DATA_REG = (unsigned int *)&pGPIO->GPEDAT.nREG;
	#endif
#elif defined(SNOR_GPIO_CS)
	#if defined(CONFIG_ARCH_TCC897X)
		#if defined(TCC897X_GPA)
		// SCLK, SI, SO [ A13, A15, A16] Func (6)
		// CS GPIO_A14
		BITCSET(pGPIO->GPAFN1.nREG, 0xF0F00000, 0x60600000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000006);
		BITSET(pGPIO->GPAEN.nREG, PORT_CS);
		BITCLR(pGPIO->GPADAT.nREG, PORT_CS);
		PORT_DATA_REG = (unsigned int *)&pGPIO->GPADAT.nREG;
		#else
		// SCLK, SI, SO [ D7, D8, D10] Func (6)
		// CS GPIO_D9
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x60000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000F0F, 0x00000606);
		BITSET(pGPIO->GPDEN.nREG, PORT_CS);
		BITCLR(pGPIO->GPDDAT.nREG, PORT_CS);
		PORT_DATA_REG = (unsigned int *)&pGPIO->GPDDAT.nREG;
		#endif
 	#elif defined(CONFIG_ARCH_TCC898X)
		#if defined(TCC898X_GPA)
		// SCLK, SI, SO [ A13, A15, A16] Func (5)
		// CS GPIO_A14
		BITCSET(pGPIO->GPAFN1.nREG, 0xF0F00000, 0x50500000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000005);
		BITSET(pGPIO->GPAEN.nREG, PORT_CS);
		BITCLR(pGPIO->GPADAT.nREG, PORT_CS);
		PORT_DATA_REG = (unsigned int *)&pGPIO->GPADAT.nREG;
		#else
		// SCLK, SI, SO [ D7, D8, D10] Func (4)
		// CS GPIO_D9
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x40000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000F0F, 0x00000404);
		BITSET(pGPIO->GPDEN.nREG, PORT_CS);
		BITCLR(pGPIO->GPDDAT.nREG, PORT_CS);
		PORT_DATA_REG = (unsigned int *)&pGPIO->GPDDAT.nREG;
		#endif	
	#else
	//  SCLK, SI, SO [ E12, E14, E15] Func (4)
	// CS GPIO_E13
	BITCSET(pGPIO->GPEFN1.nREG,0xFFFF0000,0x44040000);
	BITSET(pGPIO->GPEEN.nREG, PORT_CS);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CS);
	PORT_DATA_REG = (unsigned int *)&pGPIO->GPEDAT.nREG;
	#endif
#else
	#if defined(CONFIG_ARCH_TCC897X)
		#if defined(TCC897X_GPA)
		// SCLK, SI, SO, CS[ A13, A14, A15, A16] Func (6)
		BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x66600000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000006);
		#else
		// SCLK, SI, SO, CS [ D7, D8, D9, D10] Func (6)
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x60000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000666);
		#endif
	#elif defined(CONFIG_ARCH_TCC898X)
		#if defined(TCC898X_GPA)
		// SCLK, SI, SO, CS[ A13, A14, A15, A16] Func (5)
		BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x55500000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000005);
			#else
		// SCLK, SI, SO, CS [ D7, D8, D9, D10] Func (4)
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x40000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000444);
		#endif
	#else
	// /CS, SCLK, SI, SO [ E13, E12, E14, E15] Func (4)
	BITCSET(pGPIO->GPEFN1.nREG,0xFFFF0000,0x44440000);
	#endif
#endif

#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
	#if defined(TCC897X_GPA) || defined(TCC898X_GPA)
	BITCSET(pGPIO->GPACD0.nREG,0xFC000000,0x54000000);
	BITCSET(pGPIO->GPACD1.nREG,0x00000003,0x00000001);
	#else
	BITCSET(pGPIO->GPDCD0.nREG,0x003FC000,0x00154000);
	#endif
#else
	BITCSET(pGPIO->GPECD0.nREG,0xFF000000,0x55000000);
#endif

#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
	// WP = 1 [A5]
	BITCSET(pGPIO->GPAFN0.nREG,0x00F00000,0x00000000);
	BITSET(pGPIO->GPAEN.nREG,Hw5);
	BITSET(pGPIO->GPADAT.nREG,Hw5);
#else
	// WP = 1 [E19]
	BITCSET(pGPIO->GPEFN2.nREG,0x0000F000,0x00000000);
	BITSET(pGPIO->GPEEN.nREG,Hw19);
	BITSET(pGPIO->GPEDAT.nREG,Hw19);
#endif
	
#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC898X)
	// HOLD = 1 [A6]
	BITCSET(pGPIO->GPAFN0.nREG,0x0F000000,0x00000000);
	BITSET(pGPIO->GPAEN.nREG,Hw6);
	BITSET(pGPIO->GPADAT.nREG,Hw6);
#else
	// HOLD = 1 [E18]
	BITCSET(pGPIO->GPEFN2.nREG,0x00000F00,0x00000000);
	BITSET(pGPIO->GPEEN.nREG,Hw18);
	BITSET(pGPIO->GPEDAT.nREG,Hw18);
#endif
	
}
static void SNOR_SetFReadClk(void)
{
//	BITCSET(HwGPSB->MODE.nREG,0xFF000000, uFReadDiv<<24);
//	BITSET(HwGPSB->MODE.nREG,	Hw17);
}

static void SNOR_SetReadClk(void)
{
//	BITCSET(HwGPSB->MODE.nREG,0xFF000000, uReadDiv<<24);
}

static void SNOR_SetNormalClk(void)
{
//	BITCSET(HwGPSB->MODE.nREG,0xFF000000, uDiv<<24);
}

static void SNOR_SetWriteClk(void)
{
//	BITCSET(HwGPSB->MODE.nREG,0xFF000000, uWriteDiv<<24);
}

static void SNOR_SetEraseClk(void)
{
//	BITCSET(HwGPSB->MODE.nREG,0xFF000000, uEraseDiv<<24);
}

void snor_delay_us(unsigned int us)
{
	int i;
	while(us--)
	{
		for(i=0 ; i<40; i++)
			_ASM_NOP_ 
	}
}

static void SNOR_SetPort(void)
{
#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
	BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_CS);
	snor_delay_us(1);
#endif
}

static void SNOR_ReleasePort(void)
{
#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
	BITSET((*(unsigned int*)PORT_DATA_REG), PORT_CS);
	snor_delay_us(1);
#endif
}

#if 0		/* 016.03.16 */
static void SNOR_SetPort_Erase(void)
{
#if defined(CONFIG_ARCH_TCC897X)
	#if defined(TCC897X_GPA)
	// SCLK, SI, SO, CS [ A13, A14, A15, A16] Func (6)
	BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x66600000);
	BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000006);
	#else
	// SCLK, SI, SO, CS [ D7, D8, D9, D10] Func (6)
	BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x60000000);
	BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000666);
	#endif
#elif defined(CONFIG_ARCH_TCC898X)
	#if defined(TCC898X_GPA)
	// SCLK, SI, SO, CS [ A13, A14, A15, A16] Func (5)
	BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x55500000);
	BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000005);
	#else
	// SCLK, SI, SO, CS [ D7, D8, D9, D10] Func (4)
	BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x40000000);
	BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000444);
	#endif
#else
	// /CS, SCLK, SI, SO [ E13, E12, E14, E15] Func (4)
	BITCSET(pGPIO->GPEFN1.nREG,0xFFFF0000,0x44440000);
#endif
}

static void SNOR_ReleasePort_Erase(void)
{
#if defined(SNOR_GPIO)
#if defined(CONFIG_ARCH_TCC897X)
	#if defined(TCC897X_GPA)
	BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x00000000);
	BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000000);
	#else
	BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x00000000);
	BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000000);
	#endif
#elif defined(CONFIG_ARCH_TCC898X)
	#if defined(TCC898X_GPA)
	BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x00000000);
	BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000000);
	#else
	BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x00000000);
	BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000000);
	#endif
#else
	BITCSET(pGPIO->GPEFN1.nREG, 0xFFFF0000, 0x00000000);
#endif
	BITSET(pGPIO->GPEEN.nREG, PORT_CLK|PORT_CS|PORT_SI);
	BITCLR(pGPIO->GPEEN.nREG, PORT_SO);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CLK|PORT_CS|PORT_SI);

	PORT_DATA_REG = (unsigned int *)&pGPIO->GPEDAT.nREG;
#elif defined(SNOR_GPIO_CS)
#if defined(CONFIG_ARCH_TCC897X)
	#if defined(TCC897X_GPA)
	BITCSET(pGPIO->GPAFN1.nREG, 0xF0F00000, 0x60600000);
	BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000006);
	#else
	BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x60000000);
	BITCSET(pGPIO->GPDFN1.nREG, 0x00000F0F, 0x00000606);
	#endif
#elif defined(CONFIG_ARCH_TCC898X)
	#if defined(TCC898X_GPA)
	BITCSET(pGPIO->GPAFN1.nREG, 0xF0F00000, 0x50500000);
	BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000005);
	#else
	BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x40000000);
	BITCSET(pGPIO->GPDFN1.nREG, 0x00000F0F, 0x00000404);
	#endif	
#else
	//  SCLK, SI, SO [ E12, E14, E15] Func (4)
	// CS GPIO_E13
	BITCSET(pGPIO->GPEFN1.nREG,0xFFFF0000,0x44040000);
#endif
	BITSET(pGPIO->GPEEN.nREG, PORT_CS);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CS);
#endif
}
#endif /* 0 */

static void SNOR_FlushBuffer (void)
{
	BITSET(HwGPSB->MODE.nREG,Hw14|Hw15);
	BITCLR(HwGPSB->MODE.nREG,Hw14|Hw15);
}

static unsigned char SNOR_SetBitWidth(unsigned char bitWidth)
{
	unsigned char prevBitWidth = (unsigned char)((HwGPSB->MODE.nREG & (0x1F<<8)) >> 8) + 1;
	BITCSET(HwGPSB->MODE.nREG,0x1F<<8,(bitWidth-1)<<8);
	return prevBitWidth;
}

static unsigned SNOR_Exchange_32bit(unsigned datum, unsigned flagContinue)
{
#ifdef SNOR_GPIO
	int i = 0;
	int j = 0;
	int nData[32];
	unsigned char cData = 0;
	unsigned rData = 0;
	
	BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_CLK);

	if (datum != 0)
	{
		//==================================
		// Data Serialize
		//==================================
		for ( j = 0; j < 4; j++)
		{
			if ( j == 0 )
				cData = (0xFF) & (datum >> 24);
			else if ( j == 1 )
				cData = (0xFF) & (datum >> 16);
			else if ( j == 2 )
				cData = (0xFF) & (datum >> 8);
			else
				cData = (0xFF) & (datum);
			
			for ( i = 0; i < 8; i++)
			{
				nData[(i + (j*8))] = PORT_CLK;
				if(cData & Hw0)
					nData[(i + (j*8))] |= PORT_SI;

				cData = cData >> 1;
			}
		}

		//==================================
		// Send Data
		//==================================
		for (i = 32; i >= 0; --i)
		{
			BITCSET((*(unsigned int*)PORT_DATA_REG), PORT_SI|PORT_CLK, nData[i]);
			BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_SI|PORT_CLK);
		}
	}
	else
	{
		//==================================
		// Receive Data
		//==================================
		for ( j = 0; j < 4; j++)
		{
			for ( i = 0; i < 8; i++)
			{
				BITSET((*(unsigned int*)PORT_DATA_REG), PORT_CLK);
				cData = (0xFF)&(cData << 1);
				if((*(unsigned int*)PORT_DATA_REG) & PORT_SO)
					cData |= Hw0;

				BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_CLK);
			}

			if (j !=4)
				rData |= (cData << (j*8));
		}	
	}
	return (unsigned)rData;
#else
	if(flagContinue != 0)
	{
		BITSET(HwGPSB->MODE.nREG,Hw4); // set continuous mode
	}

	HwGPSB->PORT.nREG = datum;

	if(flagContinue == 0)
	{
		BITCLR(HwGPSB->MODE.nREG,Hw4);	// release continuous mode
	}


	//if ( HwGPSB->STAT.nREG != 0xA)
	//	_dprintf("0x%08X\n",	HwGPSB->STAT.nREG);

	while(ISZERO(HwGPSB->STAT.nREG,Hw2));	// check receive FIFO not empty flag

	return (HwGPSB->PORT.nREG);
#endif
}

static unsigned char SNOR_Exchange_8bit(unsigned char datum, unsigned flagContinue)
{
#ifdef SNOR_GPIO
	int i = 0;
	int nData[8];
	unsigned char rData = 0;
	
	BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_CLK);

	if (datum != 0)
	{
		//==================================
		// Data Serialize
		//==================================
		for ( i = 0; i < 8; i++)
		{
			nData[i] = PORT_CLK;
			if(datum & Hw0)
				nData[i] |= PORT_SI;

			datum = datum >> 1;
		}

		//==================================
		// Send Data
		//==================================
		for (i = 8; i >= 0; --i)
		{
			BITCSET((*(unsigned int*)PORT_DATA_REG), PORT_SI|PORT_CLK, nData[i]);
			BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_SI|PORT_CLK);
		}
	}
	else
	{
		//==================================
		// Receive Data
		//==================================
		for ( i = 0; i < 8; i++)
		{
			BITSET((*(unsigned int*)PORT_DATA_REG), PORT_CLK);
			rData = (0xFF)&(rData << 1);
			if((*(unsigned int*)PORT_DATA_REG) & PORT_SO)
				rData |= Hw0;

			BITCLR((*(unsigned int*)PORT_DATA_REG), PORT_CLK);
		}
	}
	return (unsigned char)rData;
#else	
	if(flagContinue != 0)
	{
		BITSET(HwGPSB->MODE.nREG,Hw4); // set continuous mode
	}

	HwGPSB->PORT.nREG = ((unsigned int)datum)<<24;

	if(flagContinue == 0)
	{
		BITCLR(HwGPSB->MODE.nREG,Hw4);	// release continuous mode
	}

	while(ISZERO(HwGPSB->STAT.nREG,Hw2));	// check receive FIFO not empty flag

	return (unsigned char)((HwGPSB->PORT.nREG)>>24);
#endif
}

static void SNOR_ReadID(unsigned char *pMID, unsigned short *pDevID)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);

	SNOR_Exchange_8bit(SNOR_CMD_READ_ID,1);
	*pMID = (unsigned char)(SNOR_Exchange_8bit(0,1));
	*pDevID = (unsigned short)(SNOR_Exchange_8bit(0,1));
	*pDevID = ((*pDevID)<<8) | (unsigned short)(SNOR_Exchange_8bit(0,0));

	SNOR_SetBitWidth(prevBitWidth);
}

static int SNOR_CheckMemoryType(void)
{
	unsigned char MID;
	unsigned short DID;
	int i;

	SNOR_ReadID(&MID,&DID);
	SNOR_DEBUG("SNOR Info MID: 0x%x, DID: 0x%x\n", MID, DID);
	
	for( i=0 ; i<(int)(sizeof(gSNOR_ProductTable)/sizeof(gSNOR_ProductTable[0])); i++) 
	{
		if(gSNOR_ProductTable[i].ManufID == MID && gSNOR_ProductTable[i].DevID == DID)
		{
			g_pSNOR_CurrentProduct = &gSNOR_ProductTable[i];
			return TRUE;
		}
	}

	return FALSE;
}

static void SNOR_WriteEnable(void)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	SNOR_Exchange_8bit(SNOR_CMD_WRITE_ENABLE,0);
	SNOR_SetBitWidth(prevBitWidth);
}

static void SNOR_Check_RDSCUR(void)
{
#if 0		/* 012.11.27 */
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	while(1)
	{
		unsigned char nStatus;
		SNOR_Exchange_8bit(SNOR_CMD_RDSCUR,1);
		nStatus = (unsigned char)(SNOR_Exchange_8bit(0,0));
		if(ISZERO(nStatus,Hw0))
		{
			//printf("write_success!\n");
			
		}
		else
			printf("write_fail!\n");

		break;
	}
	SNOR_SetBitWidth(prevBitWidth);
#endif /* 0 */
}

static void SNOR_WaitWriteEnableComplete(void)
{
#if 0		/* 012.11.27 */
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	while(1)
	{
		unsigned char nStatus;
		SNOR_Exchange_8bit(SNOR_CMD_READ_STATUS,1);
		nStatus = (unsigned char)(SNOR_Exchange_8bit(0,0));
		if(!ISZERO(nStatus,Hw1))	//check WEL
			break;
	}
	SNOR_SetBitWidth(prevBitWidth);
#endif /* 0 */
}

static void SNOR_WaitWriteComplete_cs(void)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	while(1)
	{
		unsigned char nStatus;

	SNOR_SetPort();// cs - l		
		SNOR_Exchange_8bit(SNOR_CMD_READ_STATUS,1);
		nStatus = (unsigned char)(SNOR_Exchange_8bit(0,0));

	SNOR_ReleasePort();// cs - h

		if(ISZERO(nStatus,Hw0))	//check WIP
			break;
	}
	SNOR_SetBitWidth(prevBitWidth);
}

static void SNOR_WaitWriteComplete(void)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	while(1)
	{
		unsigned char nStatus;
		SNOR_Exchange_8bit(SNOR_CMD_READ_STATUS,1);
		nStatus = (unsigned char)(SNOR_Exchange_8bit(0,0));
		if(ISZERO(nStatus,Hw0))	//check WIP
			break;
	}
	SNOR_SetBitWidth(prevBitWidth);
}

#if 0		/* 016.03.16 */
static void SNOR_WriteDisable(void)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	SNOR_Exchange_8bit(SNOR_CMD_WRITE_DISABLE,0);
	SNOR_SetBitWidth(prevBitWidth);
}
#endif /* 0 */

static unsigned char SNOR_ReadStatus(void)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	unsigned char status;
	SNOR_Exchange_8bit(SNOR_CMD_READ_STATUS,1);
	status = SNOR_Exchange_8bit(SNOR_DUMMPY_DATA,0);
	SNOR_SetBitWidth(prevBitWidth);
	return status;
}

static void SNOR_WriteStatus(unsigned char status)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);
	SNOR_WriteEnable();
	SNOR_Exchange_8bit(SNOR_CMD_WRITE_STATUS,1);
	SNOR_Exchange_8bit(status,0);
	SNOR_WaitWriteComplete();
	SNOR_SetBitWidth(prevBitWidth);
}

void SNOR_WP(int mode)
{
	unsigned char prevBitWidth;
	unsigned char status;

	SNOR_SetPort();
	SNOR_SetNormalClk();

	prevBitWidth = SNOR_SetBitWidth(8);

	status = SNOR_ReadStatus();
	#if defined(TCC_DEBUG_SNOR)
	printf("+status = 0x%x\n",status);
	#endif
	//remove protected area
	if(mode) {
	#if defined(TCC_DEBUG_SNOR)
		printf("wp set\n");
	#endif
		BITSET(status,0x7C);
	} else {
	#if defined(TCC_DEBUG_SNOR)
		printf("wp clear\n");
	#endif
		BITCLR(status,0x7C);
	}
	
	SNOR_WriteStatus(status);

	status = SNOR_ReadStatus();
	#if defined(TCC_DEBUG_SNOR)
	printf("-status = 0x%x\n",status);	
	#endif
	SNOR_SetBitWidth(prevBitWidth);
	SNOR_ReleasePort();
}

int SNOR_Init(void)
{
	//int input_freq;
	int res;
	gpsb_clk = 3000000;
	
	loc_SNOR_PortConfig();
	tcc_set_peri(GPSB_PERI_ID, ENABLE, gpsb_clk);/* set GPSB0 bus clock */
	gpsb_clk = tcc_get_peri(GPSB_PERI_ID);

	HwGPSB->MODE.nREG =
	           ( (0x0) << 0 ) |	// (2) operation mode ; 00:SPI compatible
	           ( (0x0) << 2 ) |	// (1) Slave mode configuration ; 0:master mode
	           ( (31)  << 8 ) |	// (5) Bit width selection (BWS+1)
	           ( (5)   << 24) |	// (8) clock divider load value

	           //Hw23 | 		//Master recovery time 
								//tRECV = (TRE + 1) * (SCKO period) 
								
	           //Hw22 | 		//Master hold time 
								//tHOLD = (THL + 1) * (SCKO period) 
								
	           //Hw21 | 		//Master setup time 
								//tSETUP = (TSU + 1) * (SCKO period) 

	           //Hw20 | 		//Polarity control for CS(FRM) - Master Only 
								//¡®0¡¯ for active low (default) / must be ¡®0¡¯ for SPI 
								//¡®1¡¯ for active high 
								
	           //Hw19 | 		//Polarity control for CMD(FRM) - Master only
								//¡®0¡¯ for active high / must be ¡®0¡¯ for SSP 
								//¡®1¡¯ for active low 
								
	           //Hw18 |			//Polarity control for transmitting data - Master Only
	           					//¡®0¡¯ for falling edge of SCK 
								//¡®1¡¯ for rising edge of SCK / must be ¡®1¡¯ for SSP
								
	           Hw17 |			//Polarity control for receiving data - Master only 
	           					//¡®0¡¯ for rising edge of SCK 
								//¡®1¡¯ for falling edge of SCK / must be ¡®1¡¯ for SSP 
	           ( (0) );
	
	HwGPSB->INTEN.nREG =
	           ( (0x1) << 24 ) |	// RX byte swap in half-word
	           ( (0x1) << 25 ) |	// RX half-word swap in word
	           ( (0x1) << 26 ) |	// TX byte swap in half-word
	           ( (0x1) << 27 ) |	// TX half-word swap in word
	           ( (0) );

	BITSET(HwGPSB->MODE.nREG,Hw3);	//operation enable

	SNOR_FlushBuffer();

	snor_delay_us(1000);
	res = SNOR_CheckMemoryType();
	if (res == TRUE)
	{
		unsigned int freq_100Hz;

		//=========================================
		// SCLK Set
		//=========================================
		freq_100Hz = SNOR_GetReadFrequency()*10000;
		printf("GPSB Peri CLK: %d, fSCLK: %d, CLK: %d\n",gpsb_clk, freq_100Hz, SNOR_GetFrequency()*10000);
		uDiv = 0;	
		while((gpsb_clk/((uDiv+1)*2)) > freq_100Hz && uDiv<0xFF)
		{
			uDiv++;
		}

		uFReadDiv = uDiv;
		printf("Fast Read Clk: %d uDiv: %d\n", gpsb_clk/((uDiv+1)*2), uDiv);

		//=========================================
		// CLK Set
		//=========================================
		freq_100Hz = SNOR_GetFrequency()*10000;
		uReadDiv = 0;
		while((gpsb_clk/((uReadDiv+1)*2)) > freq_100Hz && uReadDiv<0xFF)
		{
			uReadDiv++;
		}

		uWriteDiv = uReadDiv;
		uEraseDiv = uReadDiv;
		
		printf("Normal OP CLK: %d uDiv: %d\n",gpsb_clk/((uReadDiv+1)*2), uReadDiv);
		
		BITCSET(HwGPSB->MODE.nREG,0xFF000000, uReadDiv<<24);
	}

	SNOR_ReleasePort();
	
	printf("snor total size: %3d MB\n", SNOR_GetTotalSize() >> 20);
	printf("[boot part] start: 0x%08x, size: %8d Kbyte\n", SNOR_GetBootAddress(), SNOR_GetBootSize()>>10);
	printf("[kern part] start: 0x%08x, size: %8d Kbyte\n", SNOR_GetKernelAddress()>>10, SNOR_GetKernelSize()>>10);
	if ( SNOR_PARTITION_NUM == 3)
		printf("[data part] start: 0x%08x, size: %8d Kbyte\n", SNOR_GetDataAreaAddress()>>10, SNOR_GetDataAreaSize()>>10);
	
	return res;
}

unsigned int SNOR_GetSectorSize(void)
{
	if(g_pSNOR_CurrentProduct)
		return g_pSNOR_CurrentProduct->SectorSize;
	else
		return 0;
}

unsigned int SNOR_GetSectorCount(void)
{
	if(g_pSNOR_CurrentProduct)
		return g_pSNOR_CurrentProduct->TotalSector;
	else
		return 0;
}

unsigned int SNOR_GetTotalSize(void)
{
	if(g_pSNOR_CurrentProduct)
		return (g_pSNOR_CurrentProduct->TotalSector*g_pSNOR_CurrentProduct->SectorSize);
	else
		return 0;
}

//-------------------------------------
unsigned int SNOR_GetBootAddress(void)
{
	return 0;
}

unsigned int SNOR_GetBootSize(void)
{
	return (SNOR_BOOT_AREA_SECTOR_NUM * g_pSNOR_CurrentProduct->SectorSize);
}

unsigned int SNOR_GetKernelAddress(void)
{
	return SNOR_GetBootSize();
}

unsigned int SNOR_GetBoot1stRomPartSize(void)
{
	return (SNOR_BOOT_1ST_AREA_SECTOR_NUM * g_pSNOR_CurrentProduct->SectorSize);
}

unsigned int SNOR_GetKernelSize(void)
{
	unsigned int rKernelSize;
	
	if ( SNOR_PARTITION_NUM == 2 )
		rKernelSize = (SNOR_GetTotalSize() - SNOR_GetBootSize());
	else if ( SNOR_PARTITION_NUM == 3 )
		rKernelSize = (SNOR_KERNEL_AREA_SECTOR_NUM * g_pSNOR_CurrentProduct->SectorSize);

	return rKernelSize;
}

unsigned int SNOR_GetDataAreaAddress(void)
{
	return (SNOR_GetBootSize() + SNOR_GetKernelSize());
}

unsigned int SNOR_GetDataAreaSize(void)
{
	return (SNOR_GetTotalSize() - SNOR_GetBootSize() - SNOR_GetKernelSize());
}

unsigned int SNOR_GetFrequency(void)
{
	if(g_pSNOR_CurrentProduct)
		return g_pSNOR_CurrentProduct->fCLK;
	else
		return 8/*default*/;
}

unsigned int SNOR_GetReadFrequency(void)
{
	if(g_pSNOR_CurrentProduct)
		return g_pSNOR_CurrentProduct->fSCLK;
	else
		return 8/*default*/;
}
	
void SNOR_ChipErase(void)
{
	unsigned char prevBitWidth;
	unsigned char status;

	SNOR_SetPort();
	SNOR_SetEraseClk();

	prevBitWidth = SNOR_SetBitWidth(8);

	SNOR_ReleasePort();
	SNOR_SetPort();

	status = SNOR_ReadStatus();

	SNOR_ReleasePort();
	if(status&0x7C)
	{
		//remove protected area
		BITCLR(status,0x7C);
	SNOR_SetPort();
		SNOR_WriteStatus(status);
	SNOR_ReleasePort();

	}

	SNOR_SetPort();
		SNOR_WriteEnable();
	SNOR_ReleasePort();


	SNOR_SetPort();
		SNOR_Exchange_8bit(SNOR_CMD_CHIP_ERASE,0);
	SNOR_ReleasePort();

	SNOR_SetPort();		
		SNOR_WaitWriteComplete();
	SNOR_ReleasePort();

	SNOR_SetBitWidth(prevBitWidth);

	SNOR_ReleasePort();
}

static void SNOR_WriteByte(unsigned int address, const unsigned char *pBuffer, unsigned int lengthInByte)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);

	while(lengthInByte)
	{
		unsigned int i,nCountInPage;
		nCountInPage = 256 - (address&0xFF);
		if(nCountInPage>lengthInByte)
			nCountInPage = lengthInByte;

		SNOR_WriteEnable();
		SNOR_WaitWriteEnableComplete();
		
		SNOR_Exchange_8bit(SNOR_CMD_PAGE_PROGRAM, 1);
		SNOR_Exchange_8bit((unsigned char)((address>>16)&0xFF), 1);
		SNOR_Exchange_8bit((unsigned char)((address>>8)&0xFF), 1);
		SNOR_Exchange_8bit((unsigned char)((address)&0xFF), 1);
		for(i=0;i<nCountInPage;i++)
		{
			SNOR_Exchange_8bit(*pBuffer++,((i+1)<nCountInPage)?1:0);
		}
		SNOR_WaitWriteComplete();
		SNOR_Check_RDSCUR();

		address += nCountInPage;
		lengthInByte -= nCountInPage;
	}

	SNOR_SetBitWidth(prevBitWidth);
}

#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
static void SNOR_WriteWord(unsigned int address, const unsigned int *pBuffer, unsigned int lengthInByte)
{
	while(lengthInByte)
	{
		unsigned int i,nCountInPage,lengthInWord;
		nCountInPage = 256 - (address&0xFF);
		if(nCountInPage>lengthInByte)
			nCountInPage = lengthInByte;

	SNOR_SetPort();// cs - l
		SNOR_WriteEnable();
	SNOR_ReleasePort();// cs - h

		SNOR_WaitWriteEnableComplete();

	SNOR_SetPort();// cs - l
		SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_PAGE_PROGRAM,address), 1);
		lengthInWord = nCountInPage>>2;
		for(i=0;i<lengthInWord;i++)
		{
			SNOR_Exchange_32bit(*pBuffer++,((i+1)<lengthInWord)?1:0);
		}

	SNOR_ReleasePort();// cs - h

		SNOR_WaitWriteComplete_cs();
		SNOR_Check_RDSCUR();

	SNOR_ReleasePort();// cs - h

		address += nCountInPage;
		lengthInByte -= nCountInPage;
	}
}
#else
static void SNOR_WriteWord(unsigned int address, const unsigned int *pBuffer, unsigned int lengthInByte)
{
	while(lengthInByte)
	{
		unsigned int i,nCountInPage,lengthInWord;
		nCountInPage = 256 - (address&0xFF);
		if(nCountInPage>lengthInByte)
			nCountInPage = lengthInByte;

		SNOR_WriteEnable();
		SNOR_WaitWriteEnableComplete();

		SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_PAGE_PROGRAM,address), 1);
		lengthInWord = nCountInPage>>2;
		for(i=0;i<lengthInWord;i++)
		{
			SNOR_Exchange_32bit(*pBuffer++,((i+1)<lengthInWord)?1:0);
		}

		SNOR_WaitWriteComplete();
		SNOR_Check_RDSCUR();

		address += nCountInPage;
		lengthInByte -= nCountInPage;
	}
}
#endif

#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
void SNOR_BlockErase(unsigned int address)
{
	SNOR_ReleasePort();
	//printf("Erase Address: 0x%x\n", address);

	SNOR_SetPort();
		SNOR_WriteEnable();
	SNOR_ReleasePort();

	SNOR_WaitWriteEnableComplete();

	SNOR_SetPort();	// cs l		// 32 cycle
		SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_BLOCK_ERASE,address), 0);
	SNOR_ReleasePort(); // cs h

	SNOR_SetPort();	// cs l		// 16 cycle
		SNOR_WaitWriteComplete();
	SNOR_ReleasePort(); // cs h
}

void SNOR_SectorErase(unsigned int address)
{
	SNOR_ReleasePort();
	//printf("Erase Address: 0x%x\n", address);

	SNOR_SetPort();
		SNOR_WriteEnable();
	SNOR_ReleasePort();

	SNOR_WaitWriteEnableComplete();

	SNOR_SetPort();	// cs l		// 32 cycle
		SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_SECTOR_ERASE,address), 0);
	SNOR_ReleasePort(); // cs h

	SNOR_SetPort();	// cs l		// 16 cycle
		SNOR_WaitWriteComplete();
	SNOR_ReleasePort(); // cs h
}
#else
void SNOR_BlockErase(unsigned int address)
{
	SNOR_SetPort();
	SNOR_SetEraseClk();
	//printf("Erase Address: 0x%x\n", address);

	SNOR_WriteEnable();
	SNOR_WaitWriteEnableComplete();

	SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_BLOCK_ERASE,address), 0);

	SNOR_WaitWriteComplete();

	SNOR_Check_RDSCUR();
	SNOR_ReleasePort();
}

void SNOR_SectorErase(unsigned int address)
{
	SNOR_SetPort();
	SNOR_SetEraseClk();
	//printf("Erase Address: 0x%x\n", address);

	SNOR_WriteEnable();
	SNOR_WaitWriteEnableComplete();

	SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_SECTOR_ERASE,address), 0);

	SNOR_WaitWriteComplete();

	SNOR_Check_RDSCUR();
	SNOR_ReleasePort();
}
#endif
int SNOR_Write(unsigned int address, const void *pBuffer, unsigned int length)
{
	unsigned int pBufAddr = (unsigned int)pBuffer;

	SNOR_SetPort();
	SNOR_SetWriteClk();

	//printf("Write Address: 0x%x\n", address);

	if((pBufAddr&0x3) || (length&0x3))
	{

		//printf("SNOR_WriteByte: 0x%x\n", address);

		SNOR_WriteByte(address,(unsigned char*)pBuffer,length);
	}
	else
	{
		SNOR_WriteWord(address,(unsigned int*)pBuffer,length);
	}

	SNOR_ReleasePort();
	return 0;
}

void SNOR_UnitErase(unsigned int address, unsigned int size)
{
	unsigned int lba = 0;
	unsigned int nPercent, nPercent_unit;

	unsigned int count_4k = 0;

	//printf("[SNOR Erase] addr: 0x%08x, size: %8d kbyte     ", address, size>>10);

     if((address & 0xFFFF) || ( size < (64*1024)))
     {
         count_4k = (address & 0xFFFF);
         count_4k = (size > (64*1024)) ? ((64*1024) - count_4k) : (count_4k + size);

         for(lba = 0;lba < count_4k; lba += 4*1024) {
           //	printf("Sector Erase : 0x%08x\n", address + lba);
			 SNOR_SectorErase(address + lba);
			 
         }

         address += count_4k;
         size -= count_4k;
     }

     nPercent = 0;
     nPercent_unit = ( size / (64*1024));
     for(lba = 0;lba < nPercent_unit; lba++) {
         //if(lba && !(lba%41))
         //  printf("\r\n");
         if (nPercent<99)
         {
         //  nPercent += 100/nPercent_unit;
             nPercent = (100*lba)/nPercent_unit;
             printf("\b\b\b\b%3d%%", nPercent);
         }

         //printf("Sector Erase: 0x%08x, nPercent_unit: %d, nPercent: %d\n", address, nP     ercent_unit, nPercent);
         SNOR_BlockErase(address);
         address += 64*1024;
         size -= 64*1024;
     }

     if(size > 0)
     {
         nPercent_unit = size % (4*1024);
         if(nPercent_unit) nPercent_unit = size / (4*1024) + 1;
         else nPercent_unit = size / (4*1024);

         for(lba = 0;lba < nPercent_unit; lba ++) {
     	  //printf("Sector Erase : 0x%08x\n", address);
             SNOR_SectorErase(address);
             address += 4*1024;
             size -= 4*1024;
         }

     }

#if 0
	nPercent = 0;
	nPercent_unit =	( size / (64*1024));
	if (nPercent_unit == 0)
		nPercent_unit = 1;
	
	nPercent_unit = (100 / nPercent_unit);

	if (nPercent_unit == 0)
		nPercent_unit = 1;


	if((address & 0xFFFF) || ( size < (64*1024)))
	{
		count_4k = (address & 0xFFFF);
		count_4k = (size > (64*1024)) ? ((64*1024) - count_4k) : (count_4k + size);

		for(lba = 0;lba < count_4k; lba += 4*1024) {
			//printf("Sector Erase : %d\n", address + lba);
			SNOR_SectorErase(address + lba);
		}

		address += count_4k;
		size -= count_4k;
	}

	//printf("[SNOR Erase] addr: 0x%08x, size: %8d kbyte     ", address, size>>10);
	
	for(lba = 0;lba < size; lba += 64*1024) {
		//if(lba && !(lba%41))
		//	printf("\r\n");
		//printf(".");
		if (nPercent<99)
		{
			nPercent += nPercent_unit;
		    printf("\b\b\b\b%3d%%", nPercent);
		}
		
		//printf("Block Erase : %d\n", address + lba);
		SNOR_BlockErase(address + lba);
	}
#endif

	printf("\b\b\b\b%3d%%", 100);
	printf(" OK.\r\n");
}

static void SNOR_ReadByte(unsigned int address, unsigned char *pBuffer, unsigned int lengthInByte)
{
	unsigned char prevBitWidth = SNOR_SetBitWidth(8);

	while(lengthInByte)
	{
		unsigned int i,nCountInPage;
		nCountInPage = 256 - (address&0xFF);
		if(nCountInPage>lengthInByte)
			nCountInPage = lengthInByte;

		SNOR_Exchange_8bit(SNOR_CMD_READ, 1);
		SNOR_Exchange_8bit((unsigned char)((address>>16)&0xFF), 1);
		SNOR_Exchange_8bit((unsigned char)((address>>8)&0xFF), 1);
		SNOR_Exchange_8bit((unsigned char)((address)&0xFF), 1);
		for(i=0;i<nCountInPage;i++)
		{
			pBuffer[i] = SNOR_Exchange_8bit(SNOR_DUMMPY_DATA,((i+1)<nCountInPage)?1:0);
		}

		address += nCountInPage;
		lengthInByte -= nCountInPage;
	}

	SNOR_SetBitWidth(prevBitWidth);
}

static void SNOR_FReadWord(unsigned int address, unsigned int *pBuffer, unsigned int lengthInByte)
{

	unsigned int i,lengthInWord = lengthInByte>>2;
	unsigned char prevBitWidth;
	
	SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_FAST_READ,address), 1);
	prevBitWidth = SNOR_SetBitWidth(8);
	SNOR_Exchange_8bit(SNOR_DUMMPY_DATA, 1);	// 8 Dummy Cycles
	SNOR_SetBitWidth(prevBitWidth);

	for(i=0;i<lengthInWord;i++)
	{
		pBuffer[i] = SNOR_Exchange_32bit(SNOR_DUMMPY_DATA,((i+1)<lengthInWord)?1:0);
	}
}
#ifdef SNOR_USE_FAST_READ
#else
static void SNOR_ReadWord(unsigned int address, unsigned int *pBuffer, unsigned int lengthInByte)
{

	unsigned int i,lengthInWord = lengthInByte>>2;
	SNOR_Exchange_32bit(SNOR_ADDRESSED_COMMAND(SNOR_CMD_READ,address), 1);
	for(i=0;i<lengthInWord;i++)
	{
		pBuffer[i] = SNOR_Exchange_32bit(SNOR_DUMMPY_DATA,((i+1)<lengthInWord)?1:0);
	}
}
#endif
int SNOR_Read(unsigned int address, void *pBuffer, unsigned int length)
{
	unsigned int pBufAddr = (unsigned int)pBuffer;

	//printf("Read Address: 0x%x\n", address);

	SNOR_SetPort();
	SNOR_SetReadClk();
	
	if((pBufAddr&0x3) || (length&0x3))
	{
		SNOR_ReadByte(address, (unsigned char*)pBuffer, length);
	}
	else
	{
#ifdef SNOR_USE_FAST_READ
		SNOR_SetFReadClk();

		SNOR_FReadWord(address, (unsigned int*)pBuffer, length);
#else
		SNOR_ReadWord(address, (unsigned int*)pBuffer, length);
#endif
	}
	SNOR_ReleasePort();

	return 0;
}

int SNOR_Boot_Read(unsigned long ulLBA_addr, unsigned long ulSector, void *buff)
{
	unsigned int uiAddress = ulLBA_addr<<9;
	unsigned int uiLength = ulSector<<9;

	//printf("[tcboot read] lba: %d, sector: %d\n", ulLBA_addr, ulSector );
	uiAddress += SNOR_GetBootAddress();
	SNOR_Read(uiAddress,buff,uiLength);
	return 0;
}

int SNOR_Kernel_Read(unsigned long ulLBA_addr, unsigned long ulSector, void *buff)
{
	unsigned int uiAddress = ulLBA_addr<<9;
	unsigned int uiLength = ulSector<<9;
	//printf("[kernel read] lba: %d, sector: %d\n", ulLBA_addr, ulSector );

	if(ulLBA_addr>=0x00800000 || ulSector>=0x00800000){
		printf("error!\n");
		return -1;
	}
	
	uiAddress += SNOR_GetKernelAddress();
	//printf("[kernel read] lba: %3d, sector: %3d, addr: 0x%08X\n", ulLBA_addr, ulSector, uiAddress );
	SNOR_Read(uiAddress, buff, uiLength);
	return 0;
}

int SNOR_Kernel_Write(unsigned long ulLBA_addr, unsigned long ulSector, void *buff)
{
	unsigned int uiAddress = ulLBA_addr<<9;
	unsigned int uiLength = ulSector<<9;
	//printf("[kernel write] lba: 0x%08X, sector: %d\n", uiAddress, ulSector );

	if(ulLBA_addr>=0x00800000 || ulSector>=0x00800000)
		return -1;
	
	uiAddress += SNOR_GetKernelAddress();
//	printf("[kernel write] lba: %8d, sector: %3d, addr: 0x%08X\n", ulLBA_addr, ulSector,uiAddress );
	SNOR_Write(uiAddress,buff,uiLength);
	return 0;
}

//#define SNOR_DATA_DEBUG

int SNOR_DataAreaUpdate_Read(unsigned long ulLBA_addr, unsigned long ulSector, void *buff)
{
	unsigned int uiAddress = ulLBA_addr<<9;
	unsigned int uiLength = ulSector<<9;
	#ifdef SNOR_DATA_DEBUG
	unsigned int i,j, sector_addr;;
	unsigned int *pBufAddr = (unsigned int)buff;
	#endif
	printf("[Data Area read] lba: %d, sector: %d\n", (int)ulLBA_addr, (int)ulSector );

	if(ulLBA_addr>=0x00800000 || ulSector>=0x00800000)
		return -1;
	uiAddress += SNOR_GetDataAreaAddress();
	SNOR_Read(uiAddress,buff,uiLength);

	#ifdef SNOR_DATA_DEBUG
	sector_addr = ulLBA_addr;
	for ( i = 0; i < uiLength/4; i+=4 )
	{	
		if (j == 32)
		{
			printf("[Sector_Addr: %d]\n", sector_addr );
			++sector_addr;
			j = 0;
		}

		printf("%08X %08X %08X %08X\n", pBufAddr[i], pBufAddr[i+1], pBufAddr[i+2], pBufAddr[i+3]);
		++j;
	}
	#endif

	return 0;
}

int SNOR_DataAreaUpdate_Write(unsigned long ulLBA_addr, unsigned long ulSector, void *buff)
{
	unsigned int uiAddress = ulLBA_addr<<9;
	unsigned int uiLength = ulSector<<9;

	printf("[Data Area write] lba: %8d, sector: %3d\n", (int)ulLBA_addr, (int)ulSector );
	
	if(ulLBA_addr>=0x00800000 || ulSector>=0x00800000)
		return -1;
	uiAddress += SNOR_GetDataAreaAddress();
	SNOR_Write(uiAddress,buff,uiLength);
	return 0;
}

#ifdef SNOR_TEST
int SNOR_IO_TEST(void)
{
	unsigned int nSectorSize, nSectorCount;
	unsigned int nAddress;
	unsigned int *pWBuffer;
	unsigned int *pRBuffer;
	unsigned long uiAddress;
    int i, j;

	nSectorSize 	= SNOR_GetSectorSize();
	nSectorCount 	= SNOR_GetSectorCount();

	printf("[Sector Size: 0x%08X, Count: 0x%08X]\n", nSectorSize, nSectorCount );

	pRBuffer = ROMFILE_TEMP_BUFFER + nSectorSize;

#if 0
	pWBuffer = (unsigned int*)0x82000000;
#else
	pWBuffer = ROMFILE_TEMP_BUFFER;
	printf("[Test Data Init]\n");
	for ( j = 0; j < (nSectorCount>>2); ++j ) 
	{
		pWBuffer[j] = 0x12345678;
	}
#endif	
	printf("----------------------------------------\n");
	printf("Test Pattern : 1\n");
	printf("----------------------------------------\n");
	nAddress = 0;
	for ( i = 0; i < nSectorCount; ++i ) 
	{
		printf("Test [0x%08X]\n", nAddress);
		SNOR_BlockErase(nAddress);
		SNOR_Write(nAddress, pWBuffer, nSectorSize);
		SNOR_Read(nAddress, pRBuffer, nSectorSize);

		for ( j = 0; j < (nSectorCount>>2); ++j ) 
		{		
			if( pWBuffer[i] != pRBuffer[i]) {
	            printf("Error! [Addr: 0x%08X] [ 0x%08X != 0x%08X]\n", (nAddress + (i<<2)), pWBuffer[i], pRBuffer[i]);
			}
		}	
		nAddress += nSectorSize;
	}
}
#endif
