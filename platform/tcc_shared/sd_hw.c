
#include <config.h>
#include <dev/gpio.h>
#include <platform/tcc_ckc.h>
#include <clock.h>
#include <platform/gpio.h>
#include <sdmmc/sd_hw.h>
#include <debug.h>
#include <platform/iomap.h>
#include <platform/timer.h>
#include <reg.h>
#include <plat/cpu.h>
#include <tcc_sdmmc.h>

#ifndef NULL
#define NULL 0
#endif
#ifndef dim
#define dim(x) (sizeof(x)/sizeof(x[0]))
#endif
#define SD_ASSERT(x)				if(!(x)) while(1);
#define	_tca_delay()				{ volatile int i; for (i=0; i<1000; i++); }

static int CardDetectPort(void)
{
 	if(CD_GPIO == NULL)
	{
		return 0;
	}
	else
	{
		return (gpio_get(CD_GPIO));
	}
}
static int WriteProtectPort(void)
{
	if(WP_GPIO == NULL)
	{
		return 0;
	}
	else
	{
		return (gpio_get(WP_GPIO));
	}  
}

/////No #ifdef, #ifndef right///////////////////////////
#ifndef SLOT_TYPE_0
#define SLOT_TYPE_0				0
#define SLOT_BUSWIDTH_0			4
#define SLOT_MAX_SPEED_0		SLOT_MAX_SPEED_NORMAL
#define CARD_DETECT_FUNC_0		NULL
#define WRITE_PROTECT_FUNC_0	NULL
#endif
#ifndef SLOT_TYPE_1
#define SLOT_TYPE_1				0
#define SLOT_BUSWIDTH_1			4
#define SLOT_MAX_SPEED_1		SLOT_MAX_SPEED_NORMAL
#define CARD_DETECT_FUNC_1		NULL
#define WRITE_PROTECT_FUNC_1	NULL
#endif
#ifndef SLOT_TYPE_2
#define SLOT_TYPE_2				0
#define SLOT_BUSWIDTH_2			4
#define SLOT_MAX_SPEED_2		SLOT_MAX_SPEED_NORMAL
#define CARD_DETECT_FUNC_2		NULL
#define WRITE_PROTECT_FUNC_2	NULL
#endif
#ifndef SLOT_TYPE_3
#define SLOT_TYPE_3				0
#define SLOT_BUSWIDTH_3			4
#define SLOT_MAX_SPEED_3		SLOT_MAX_SPEED_NORMAL
#define CARD_DETECT_FUNC_3		NULL
#define WRITE_PROTECT_FUNC_3	NULL
#endif

SD_HW_SLOTINFO_T sSdHwSlotInfo[] =
{
	{
		0/*SD Host Number*/, 
		HwSDMMC0_BASE/*SD Slot Register Address*/,
		SLOT_TYPE_0/*type*/, 
		SLOT_BUSWIDTH_0/*max bus width*/, 
		SLOT_MAX_SPEED_0/*speed*/, 
		CARD_DETECT_FUNC_0, 
		WRITE_PROTECT_FUNC_0,
		HwSDMMC_CHCTRL_CH0_CAP0,
		HwSDMMC_CHCTRL_CH0_CAP1
	},

	{
		1/*SD Host Number*/, 
		HwSDMMC1_BASE/*SD Slot Register Address*/,
		SLOT_TYPE_1/*type*/, 
		SLOT_BUSWIDTH_1/*max bus width*/, 
		SLOT_MAX_SPEED_1/*speed*/, 
		CARD_DETECT_FUNC_1, 
		WRITE_PROTECT_FUNC_1,
		HwSDMMC_CHCTRL_CH1_CAP0,
		HwSDMMC_CHCTRL_CH1_CAP1
	},

	{
		2/*SD Host Number*/, 
		HwSDMMC2_BASE/*SD Slot Register Address*/,
		SLOT_TYPE_2/*type*/, 
		SLOT_BUSWIDTH_2/*max bus width*/, 
		SLOT_MAX_SPEED_2/*speed*/, 
		CARD_DETECT_FUNC_2, 
		WRITE_PROTECT_FUNC_2,
		HwSDMMC_CHCTRL_CH2_CAP0,
		HwSDMMC_CHCTRL_CH2_CAP1
	},

	{
		3/*SD Host Number*/, 
		HwSDMMC3_BASE/*SD Slot Register Address*/,
		SLOT_TYPE_3/*type*/, 
		SLOT_BUSWIDTH_3/*max bus width*/, 
		SLOT_MAX_SPEED_3/*speed*/, 
		CARD_DETECT_FUNC_3, 
		WRITE_PROTECT_FUNC_3,
		HwSDMMC_CHCTRL_CH3_CAP0,
		HwSDMMC_CHCTRL_CH3_CAP1
	}
};
//////////////////////////////////////////////////////////////////////

static int SD_HW_IsSdHostNeeded(unsigned char ucHostIndex)
{
	int i;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].ucHostNum == ucHostIndex
			&& sSdHwSlotInfo[i].uiSlotAttr != 0)
			return 1;
	}

	return 0;
}

static void SD_HW_InitializePort(void)
{
	//////////// SD Card power control////////////////
	if(SDCARD_PW != NULL)
	{
		gpio_config(SDCARD_PW, GPIO_FN(0) | GPIO_OUTPUT | GPIO_LOW);
	}
	//////////////////////////////////////////////////

	//////////// boot sdmmc gpio function ////////////
	gpio_config(SDMMC_DATA0, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA1, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA2, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA3, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA4, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA5, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA6, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_DATA7, SDMMC_GPIO_FN | GPIO_CD2);

	gpio_config(SDMMC_CMD, SDMMC_GPIO_FN | GPIO_CD2);
	gpio_config(SDMMC_CLK, SDMMC_GPIO_FN | GPIO_CD3);
	//////////////////////////////////////////////////
}

#if defined(TCC893X)
#define DDR_CLOCK 40000000
#define SDR_CLOCK 48000000
#elif defined(TCC896X)
#define DDR_CLOCK 50000000
#define SDR_CLOCK 50000000
#elif defined(TCC897X)
#define DDR_CLOCK 50000000
#define SDR_CLOCK 50000000
#else
#error
#endif

static void SD_HW_InitializeClock(void)
{
	//=====================================================
	// SD Channel Control Register Clock Enable
	//=====================================================
	tcc_set_iobus_pwdn(IOBUS_SDMMC,0);		// SD Channel Control Register Clock Enable

	if(SD_HW_IsSdHostNeeded(0))
	{
		//=====================================================
		// SD Host Controller #0 Clock Enable
		//=====================================================
		if (SD_HW_IS_DDR_MODE(0))
			tcc_set_peri(PERI_SDMMC0, ENABLE, DDR_CLOCK);
		else
			tcc_set_peri(PERI_SDMMC0, ENABLE, SDR_CLOCK);
		_tca_delay();
		tcc_set_iobus_pwdn(IOBUS_SDMMC0,0);
	}

	if(SD_HW_IsSdHostNeeded(1))
	{
#if !(defined(CONFIG_CHIP_TCC8935S) || defined(CONFIG_CHIP_TCC8937S) || defined(TCC897X) )
		//=====================================================
		// SD Host Controller #1 Clock Enable
		//=====================================================
		if (SD_HW_IS_DDR_MODE(1))
			tcc_set_peri(PERI_SDMMC1, ENABLE, DDR_CLOCK);
		else
			tcc_set_peri(PERI_SDMMC1, ENABLE, SDR_CLOCK);
		_tca_delay();
		tcc_set_iobus_pwdn(IOBUS_SDMMC1,0);
#endif
	}

	if(SD_HW_IsSdHostNeeded(2))
	{
		//=====================================================
		// SD Host Controller #2 Clock Enable
		//=====================================================
		if (SD_HW_IS_DDR_MODE(2))
			tcc_set_peri(PERI_SDMMC2, ENABLE, DDR_CLOCK);
		else
			tcc_set_peri(PERI_SDMMC2, ENABLE, SDR_CLOCK);
		_tca_delay();
		tcc_set_iobus_pwdn(IOBUS_SDMMC2,0);
	}

	if(SD_HW_IsSdHostNeeded(3))
	{
		//=====================================================
		// SD Host Controller #3 Clock Enable
		//=====================================================
		if (SD_HW_IS_DDR_MODE(3))
			tcc_set_peri(PERI_SDMMC3, ENABLE, DDR_CLOCK);
		else
			tcc_set_peri(PERI_SDMMC3, ENABLE, SDR_CLOCK);
		_tca_delay();
		tcc_set_iobus_pwdn(IOBUS_SDMMC3,0);
	}
}

#if defined(TCC893X) || defined(TCC896X) || defined(TCC897X)
#define SDCHCTRL_SD0CMDDAT	0x008
#define SDCHCTRL_SD1CMDDAT	0x00C
#define SDCHCTRL_SD2CMDDAT	0x010
#define SDCHCTRL_SD3CMDDAT	0x014
#endif

void SD_HW_Initialize(void)
{
	SD_HW_InitializePort();
	SD_HW_InitializeClock();

	//=====================================================
	// Peri block reset
	//=====================================================
	/* Channel Control Register Reset */
	tcc_set_iobus_swreset(IOBUS_SDMMC, 1);
	_tca_delay();
	tcc_set_iobus_swreset(IOBUS_SDMMC, 0);
	_tca_delay();

	if(SD_HW_IsSdHostNeeded(0))
	{
		/* SD Host Controller #0 Reset */
		tcc_set_iobus_swreset(IOBUS_SDMMC0, 1);
		_tca_delay();
		tcc_set_iobus_swreset(IOBUS_SDMMC0, 0);
		_tca_delay();
//		pSDCTRL->SD0PRESET1.nREG = 0x0CFF9870;
	}

#ifndef TCC897X
	if(SD_HW_IsSdHostNeeded(1))
	{
		/* SD Host Controller #1 Reset */
		tcc_set_iobus_swreset(IOBUS_SDMMC1, 1);
		_tca_delay();
		tcc_set_iobus_swreset(IOBUS_SDMMC1, 0);
		_tca_delay();

//		pSDCTRL->SD1PRESET1.nREG = 0x0CFF9870;
	}
#endif

	if(SD_HW_IsSdHostNeeded(2))
	{
		/* SD Host Controller #2 Reset */
		tcc_set_iobus_swreset(IOBUS_SDMMC2, 1);
		_tca_delay();
		tcc_set_iobus_swreset(IOBUS_SDMMC2, 0);
		_tca_delay();

//		pSDCTRL->SD1PRESET1.nREG = 0x0CFF9870;
	}

	if(SD_HW_IsSdHostNeeded(3))
	{
		/* SD Host Controller #3 Reset */
		tcc_set_iobus_swreset(IOBUS_SDMMC3, 1);
		_tca_delay();
		tcc_set_iobus_swreset(IOBUS_SDMMC3, 0);
		_tca_delay();

//		pSDCTRL->SD3PRESET1.nREG = 0x0CFF9870;
	}

	_tca_delay();
#if defined(TCC893X)
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD0CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD1CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD2CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD3CMDDAT);
#elif defined(TCC896X)
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD0CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD1CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD2CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD3CMDDAT);
#elif defined(TCC897X)
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD0CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD1CMDDAT);
	writel(0x00000000, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD2CMDDAT);
	writel(0x07070707, HwSDMMC_CHCTRL_BASE+SDCHCTRL_SD3CMDDAT);
#else
#error
#endif
	_tca_delay();
}

static int SD_HW_Convert_SlotIndex_To_HwSlotInfoIndex(int iSlotIndex)
{
	int i,iSlotIndexTemp=0;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr == 0)
			continue;
		if(iSlotIndexTemp++ == iSlotIndex)
			return i;
	}
	SD_ASSERT(0);

	return 0;
}

static int SD_HW_Convert_HwSlotInfoIndex_To_SlotIndex(int iHwSlotIndex)
{
	int i,iSlotIndex=-1;

	SD_ASSERT( iHwSlotIndex < (int)dim(sSdHwSlotInfo) );
	for( i=0 ; i<=iHwSlotIndex ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr == 0)
			continue;
		iSlotIndex++;
	}
	SD_ASSERT(iSlotIndex>=0);

	return iSlotIndex;
}

int SD_HW_Get_CardDetectPort(int iSlotIndex)
{
	int iIndex = SD_HW_Convert_SlotIndex_To_HwSlotInfoIndex(iSlotIndex);

	SD_ASSERT( iIndex < (int)dim(sSdHwSlotInfo) );

	if(sSdHwSlotInfo[iIndex].CardDetect)
		return sSdHwSlotInfo[iIndex].CardDetect();

	return 1;
}

int SD_HW_Get_WriteProtectPort(int iSlotIndex)
{
	int iIndex = SD_HW_Convert_SlotIndex_To_HwSlotInfoIndex(iSlotIndex);

	SD_ASSERT( iSlotIndex < (int)dim(sSdHwSlotInfo) );

	if(sSdHwSlotInfo[iIndex].WriteProtect)
		return sSdHwSlotInfo[iIndex].WriteProtect();

	return 0;
}

PSDSLOTREG_T SD_HW_GetSdSlotReg(int iSlotIndex)
{
	int iIndex = SD_HW_Convert_SlotIndex_To_HwSlotInfoIndex(iSlotIndex);

	SD_ASSERT( iIndex < (int)dim(sSdHwSlotInfo) );

	return (PSDSLOTREG_T)sSdHwSlotInfo[iIndex].ulSdSlotRegAddr;
}

int SD_HW_GetMaxBusWidth(int iSlotIndex)
{
	int iIndex = SD_HW_Convert_SlotIndex_To_HwSlotInfoIndex(iSlotIndex);
	SD_ASSERT( iIndex < (int)dim(sSdHwSlotInfo) );
	return sSdHwSlotInfo[iIndex].iMaxBusWidth;
}

int SD_HW_IsSupportHighSpeed(int iSlotIndex)
{
	int iIndex = SD_HW_Convert_SlotIndex_To_HwSlotInfoIndex(iSlotIndex);

	SD_ASSERT( iIndex < (int)dim(sSdHwSlotInfo) );

	if(sSdHwSlotInfo[iIndex].eSlotSpeed == SLOT_MAX_SPEED_HIGH)
		return 1;
	else
		return 0;
}

int SD_HW_GetTotalSlotCount(void)
{
	int i;
	int iCount=0;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr != 0)
			iCount++;
	}

	return iCount;
}

int SD_HW_GetBootSlotIndex(void)
{
	int i;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr & SLOT_ATTR_BOOT)
			return SD_HW_Convert_HwSlotInfoIndex_To_SlotIndex(i);
	}

	return -1;
}

int SD_HW_GetUpdateSlotIndex(void)
{
	int i;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr & SLOT_ATTR_UPDATE)
			return SD_HW_Convert_HwSlotInfoIndex_To_SlotIndex(i);
	}

	return -1;
}

void SDMMC_ms_delay(unsigned long ul_ms)
{
	mdelay(ul_ms);
}

extern void *memcpy(void *, const void *, unsigned int);

void SDMMC_memcpy(void *pvTgt, const void *pvSrc, unsigned int uiLen)
{
#ifdef _LINUX_
	if( ( ((unsigned int)pvTgt)&0x3 ) || ( ((unsigned int)pvSrc)&0x3 ) )
	{
		unsigned char *pucTgt = (unsigned char*)pvTgt;
		unsigned char *pucSrc = (unsigned char*)pvSrc;
		while(uiLen--)
			*pucTgt++ = *pucSrc++;
	}
	else
	{
		memcpy(pvTgt,pvSrc,uiLen);
	}
#else
	memcpy(pvTgt,pvSrc,uiLen);
#endif
}

int SD_HW_IS_HIGH_SPEED(int slot_num)
{
	int i;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr & SLOT_ATTR_BOOT) {
			//printf("slot num %d\n", i);
			return (int)sSdHwSlotInfo[i].eSlotSpeed;
		}
	}

	return 0;
#if 0
	return (int)sSdHwSlotInfo[slot_num].eSlotSpeed;
#endif
}

int SD_HW_IS_DDR_MODE(int slot_num)
{
	int i;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr & SLOT_ATTR_BOOT)
			return (sSdHwSlotInfo[i].eSlotSpeed == SLOT_MAX_SPEED_DDR);
	}
	return 0;
}

int SD_HW_SET_DDR_CAP(int slot_num)
{
	int i;

	for( i=0 ; i<(int)dim(sSdHwSlotInfo) ; i++ )
	{
		if(sSdHwSlotInfo[i].uiSlotAttr & SLOT_ATTR_BOOT) 
		{
			*(volatile unsigned long*)sSdHwSlotInfo[i].CAPPREG0_CH = 0x8CFF9870;
			*(volatile unsigned long*)sSdHwSlotInfo[i].CAPPREG1_CH = 0x00000007;
			//*(volatile unsigned long*)0x76020860 = 0x00000007;
			//*(volatile unsigned long*)0x76020864 = 0x8CFF9870;
		}
	}
	return 0;
}
