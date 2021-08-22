/****************************************************************************
 *   FileName    : sd_hw.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#ifndef _SD_HW_H
#define _SD_HW_H

#include <asm/arch/sdmmc/sd_regs.h>
#include <asm/arch/sdmmc/tcc_sdmmc.h>

#define SLOT_ATTR_BOOT			(1<<0)
#define SLOT_ATTR_UPDATE		(1<<1)

#ifndef NULL
#define NULL 0
#endif
#ifndef dim
#define dim(x) (sizeof(x)/sizeof(x[0]))
#endif
#define SD_ASSERT(x)				if(!(x)) while(1);
#define	_tca_delay()				{ volatile int i; for (i=0; i<1000; i++); }

#define DDR_CLOCK 50000000
#define SDR_CLOCK 50000000

#define SDCHCTRL_SD0CMDDAT	0x008
#define SDCHCTRL_SD1CMDDAT	0x00C
#define SDCHCTRL_SD2CMDDAT	0x010
#define SDCHCTRL_SD3CMDDAT	0x014

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
/////No #ifdef, #ifndef right///////////////////////////

typedef enum {
	SLOT_MAX_SPEED_NORMAL,
	SLOT_MAX_SPEED_HIGH,
	SLOT_MAX_SPEED_DDR
} SLOT_MAX_SPEED_T;

typedef struct {
	unsigned char ucHostNum;
	unsigned long ulSdSlotRegAddr;

	unsigned int uiSlotAttr;
	int iMaxBusWidth;
	SLOT_MAX_SPEED_T eSlotSpeed;
 	int (*CardDetect)(void);
 	int (*WriteProtect)(void);
	unsigned long CAPPREG0_CH;
	unsigned long CAPPREG1_CH;
} SD_HW_SLOTINFO_T;

void SD_HW_Initialize(void);
int SD_HW_Get_CardDetectPort(int iSlotIndex);
int SD_HW_Get_WriteProtectPort(int iSlotIndex);
PSDSLOTREG_T SD_HW_GetSdSlotReg(int iSlotIndex);
int SD_HW_GetMaxBusWidth(int iSlotIndex);
int SD_HW_IsSupportHighSpeed(int iSlotIndex);
int SD_HW_GetTotalSlotCount(void);
int SD_HW_GetBootSlotIndex(void);
int SD_HW_GetUpdateSlotIndex(void);
int SD_HW_IS_HIGH_SPEED(int slot_num);
int SD_HW_IS_DDR_MODE(int slot_num);
int SD_HW_SET_DDR_CAP(int slot_num);
int CardDetectPort(void);
int WriteProtectPort(void);

#endif //_SD_HW_H
