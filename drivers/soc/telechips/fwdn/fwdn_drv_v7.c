/****************************************************************************
 *   FileName    : Fwdn_drv_v7.c
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

/****************************************************************************

  Revision History

 ****************************************************************************

 ****************************************************************************/

//#include <common.h>
#include <asm/arch/fwdn/fwupgrade.h>
#include <asm/arch/fwdn/Disk.h>
#include <asm/arch/fwdn/file.h>
#include <asm/arch/fwdn/fwdn_drv_v7.h>
#include <asm/arch/fwdn/fwdn_sdmmc.h>
#include <asm/arch/sdmmc/sd_memory.h>
#include <asm/arch/sdmmc/sd_bus.h>

#if 0
#if defined(CONFIG_TCC_BOOT_EMMC)
#define _EMMC_BOOT 1
#define KERNEL_ADDR 0x80008000
#endif
#endif

#define FWDN_TRACE(fmt...)				printf(fmt)

extern void target_reinit(int);
extern 	int FWDN_SNOR_Write_Firmware( unsigned char * pucRomFile_Buffer, unsigned int uiROMFileSize);

#define	byte_of(X)					( *(volatile unsigned char *)((X)) )

#define TRANSFER_UNIT_SIZE			(64*512)
#define TRANSFER_UNIT_SECTOR_SIZE	(TRANSFER_UNIT_SIZE>>9)
#define Tbuffersize  				(128*512)	// It should be larger then TRANSFER_UNIT_SECTOR_SIZE
#define TbufferSectorSize			(Tbuffersize>>9)

static unsigned int fwdndBuf[Tbuffersize/4];
//static unsigned int veriBuf[Tbuffersize/4];


const unsigned	CRC32_TABLE[256] = {
	0x00000000,	0x90910101,	0x91210201,	0x01B00300,
	0x92410401,	0x02D00500,	0x03600600,	0x93F10701,
	0x94810801,	0x04100900,	0x05A00A00,	0x95310B01,
	0x06C00C00,	0x96510D01,	0x97E10E01,	0x07700F00,
	0x99011001,	0x09901100,	0x08201200,	0x98B11301,
	0x0B401400,	0x9BD11501,	0x9A611601,	0x0AF01700,
	0x0D801800,	0x9D111901,	0x9CA11A01,	0x0C301B00,
	0x9FC11C01,	0x0F501D00,	0x0EE01E00,	0x9E711F01,
	0x82012001,	0x12902100,	0x13202200,	0x83B12301,
	0x10402400,	0x80D12501,	0x81612601,	0x11F02700,
	0x16802800,	0x86112901,	0x87A12A01,	0x17302B00,
	0x84C12C01,	0x14502D00,	0x15E02E00,	0x85712F01,
	0x1B003000,	0x8B913101,	0x8A213201,	0x1AB03300,
	0x89413401,	0x19D03500,	0x18603600,	0x88F13701,
	0x8F813801,	0x1F103900,	0x1EA03A00,	0x8E313B01,
	0x1DC03C00,	0x8D513D01,	0x8CE13E01,	0x1C703F00,
	0xB4014001,	0x24904100,	0x25204200,	0xB5B14301,
	0x26404400,	0xB6D14501,	0xB7614601,	0x27F04700,
	0x20804800,	0xB0114901,	0xB1A14A01,	0x21304B00,
	0xB2C14C01,	0x22504D00,	0x23E04E00,	0xB3714F01,
	0x2D005000,	0xBD915101,	0xBC215201,	0x2CB05300,
	0xBF415401,	0x2FD05500,	0x2E605600,	0xBEF15701,
	0xB9815801,	0x29105900,	0x28A05A00,	0xB8315B01,
	0x2BC05C00,	0xBB515D01,	0xBAE15E01,	0x2A705F00,
	0x36006000,	0xA6916101,	0xA7216201,	0x37B06300,
	0xA4416401,	0x34D06500,	0x35606600,	0xA5F16701,
	0xA2816801,	0x32106900,	0x33A06A00,	0xA3316B01,
	0x30C06C00,	0xA0516D01,	0xA1E16E01,	0x31706F00,
	0xAF017001,	0x3F907100,	0x3E207200,	0xAEB17301,
	0x3D407400,	0xADD17501,	0xAC617601,	0x3CF07700,
	0x3B807800,	0xAB117901,	0xAAA17A01,	0x3A307B00,
	0xA9C17C01,	0x39507D00,	0x38E07E00,	0xA8717F01,
	0xD8018001,	0x48908100,	0x49208200,	0xD9B18301,
	0x4A408400,	0xDAD18501,	0xDB618601,	0x4BF08700,
	0x4C808800,	0xDC118901,	0xDDA18A01,	0x4D308B00,
	0xDEC18C01,	0x4E508D00,	0x4FE08E00,	0xDF718F01,
	0x41009000,	0xD1919101,	0xD0219201,	0x40B09300,
	0xD3419401,	0x43D09500,	0x42609600,	0xD2F19701,
	0xD5819801,	0x45109900,	0x44A09A00,	0xD4319B01,
	0x47C09C00,	0xD7519D01,	0xD6E19E01,	0x46709F00,
	0x5A00A000,	0xCA91A101,	0xCB21A201,	0x5BB0A300,
	0xC841A401,	0x58D0A500,	0x5960A600,	0xC9F1A701,
	0xCE81A801,	0x5E10A900,	0x5FA0AA00,	0xCF31AB01,
	0x5CC0AC00,	0xCC51AD01,	0xCDE1AE01,	0x5D70AF00,
	0xC301B001,	0x5390B100,	0x5220B200,	0xC2B1B301,
	0x5140B400,	0xC1D1B501,	0xC061B601,	0x50F0B700,
	0x5780B800,	0xC711B901,	0xC6A1BA01,	0x5630BB00,
	0xC5C1BC01,	0x5550BD00,	0x54E0BE00,	0xC471BF01,
	0x6C00C000,	0xFC91C101,	0xFD21C201,	0x6DB0C300,
	0xFE41C401,	0x6ED0C500,	0x6F60C600,	0xFFF1C701,
	0xF881C801,	0x6810C900,	0x69A0CA00,	0xF931CB01,
	0x6AC0CC00,	0xFA51CD01,	0xFBE1CE01,	0x6B70CF00,
	0xF501D001,	0x6590D100,	0x6420D200,	0xF4B1D301,
	0x6740D400,	0xF7D1D501,	0xF661D601,	0x66F0D700,
	0x6180D800,	0xF111D901,	0xF0A1DA01,	0x6030DB00,
	0xF3C1DC01,	0x6350DD00,	0x62E0DE00,	0xF271DF01,
	0xEE01E001,	0x7E90E100,	0x7F20E200,	0xEFB1E301,
	0x7C40E400,	0xECD1E501,	0xED61E601,	0x7DF0E700,
	0x7A80E800,	0xEA11E901,	0xEBA1EA01,	0x7B30EB00,
	0xE8C1EC01,	0x7850ED00,	0x79E0EE00,	0xE971EF01,
	0x7700F000,	0xE791F101,	0xE621F201,	0x76B0F300,
	0xE541F401,	0x75D0F500,	0x7460F600,	0xE4F1F701,
	0xE381F801,	0x7310F900,	0x72A0FA00,	0xE231FB01,
	0x71C0FC00,	0xE151FD01,	0xE0E1FE01,	0x7070FF00
};

unsigned int FWUG_CalcCrc8(unsigned char *base, unsigned int length)
{
	unsigned int crcout = 0;
	unsigned int cnt;
	unsigned char	code, tmp;

	for(cnt=0; cnt<length; cnt++)
	{
		code = base[cnt];
		tmp = code^crcout;
		crcout = (crcout>>8)^CRC32_TABLE[tmp&0xFF];
	}
	return crcout;
}

unsigned int FWUG_CalcCrc8I(unsigned uCRCIN, unsigned char *base, unsigned int length)
{

	unsigned int crcout = uCRCIN;
	unsigned int cnt;
	unsigned char	code, tmp;

	for(cnt=0; cnt<length; cnt++)
	{
		code = base[cnt];
		tmp = code^crcout;
		crcout = (crcout>>8)^CRC32_TABLE[tmp&0xFF];
	}
	return crcout;

}

//==============================================================
//
//		Global Variables
//
//==============================================================
static const unsigned char gFWDN_FirmwareStorage		=
#if defined(_EMMC_BOOT)
		FWDN_DISK_TRIFLASH
#elif defined(_SNOR_BOOT)
		FWDN_DISK_SNOR
#elif defined(_NAND_BOOT)
		FWDN_DISK_NAND
#endif
;

unsigned int					g_uiFWDN_OverWriteSNFlag;
unsigned int					g_uiFWDN_WriteSNFlag;
FWDN_DEVICE_INFORMATION			FWDN_DeviceInformation;
//static NAND_DEVICE_INFO			gNAND_DEVICE_INFO;

static FXN_FWDN_DRV_FirmwareWrite_ReadFromHost		gfxnFWDN_DRV_FirmwareWrite_ReadFromHost;
static FXN_FWDN_DRV_Progress		gfxnFWDN_DRV_Progress = NULL;

unsigned int				gFWDN_DRV_ErrorCode;

#define SESSION_FLAG_MTD_MASK				(0x00000001)
#define SESSION_FLAG_MTD					(0x00000001)
static unsigned int				s_FWDN_DRV_SessionFlag=0xFFFFFFFF;

int target_is_emmc_boot(void)
{
#if defined(_EMMC_BOOT)
	return 1;
#else
	return 0;
#endif
}

#if defined(_NAND_BOOT)
// NAND's each disk's name
#if defined(NAND_KERNEL_INCLUDE)
static const char FWDN_NAND_AREA_0_NAME[]	= "KERNEL";
#else
static const char FWDN_NAND_AREA_0_NAME[]	= "NAND RO 0";
#endif
#if defined(_WINCE_) && defined(NAND_KERNEL_INCLUDE)
static const char FWDN_NAND_AREA_1_NAME[]	= "LOGO";
#else
static const char FWDN_NAND_AREA_1_NAME[]	= "NAND RO 1";
#endif
static const char FWDN_NAND_AREA_2_NAME[]	= "NAND RO 2";
static const char FWDN_NAND_AREA_3_NAME[]	= "NAND RO 3";
static const char FWDN_NAND_AREA_4_NAME[]	= "NAND RW 0";
static const char FWDN_NAND_AREA_5_NAME[]	= "NAND RW 1";
static const char FWDN_NAND_AREA_6_NAME[]	= "NAND RW 2";
static const char FWDN_NAND_AREA_7_NAME[]	= "NAND RW 3";
static const char FWDN_NAND_AREA_8_NAME[]	= "NAND RW 4";
static const char FWDN_NAND_AREA_9_NAME[]	= "NAND Data";
static const char *FWDN_NAND_AREA_NAME[] = {
	FWDN_NAND_AREA_0_NAME,
	FWDN_NAND_AREA_1_NAME,
	FWDN_NAND_AREA_2_NAME,
	FWDN_NAND_AREA_3_NAME,
	FWDN_NAND_AREA_4_NAME,
	FWDN_NAND_AREA_5_NAME,
	FWDN_NAND_AREA_6_NAME,
	FWDN_NAND_AREA_7_NAME,
	FWDN_NAND_AREA_8_NAME,
	FWDN_NAND_AREA_9_NAME,
};
#endif

#if defined(BOOTSD_KERNEL_INCLUDE)
static const char FWDN_SD_HD0_AREA_NAME[]	= "KERNEL";
#else
static const char FWDN_SD_HD0_AREA_NAME[]	= "SD Hidden 0";
#endif
#if defined(BOOTSD_KERNEL_INCLUDE)
static const char FWDN_SD_HD1_AREA_NAME[]	= "LOGO";
#else
static const char FWDN_SD_HD1_AREA_NAME[]	= "SD Hidden 1";
#endif
static const char FWDN_SD_HD2_AREA_NAME[]	= "SD Hidden 2";
static const char FWDN_SD_HD3_AREA_NAME[]	= "SD Hidden 3";
static const char FWDN_SD_HD4_AREA_NAME[]	= "SD Hidden 4";
static const char FWDN_SD_HD5_AREA_NAME[]	= "SD Hidden 5";
static const char FWDN_SD_HD6_AREA_NAME[]	= "SD Hidden 6";
static const char FWDN_SD_HD7_AREA_NAME[]	= "SD Hidden 7";
static const char FWDN_SD_HD8_AREA_NAME[]	= "SD Hidden 8";
static const char FWDN_SD_HD9_AREA_NAME[]	= "SD Hidden 9";
static const char FWDN_SD_HD10_AREA_NAME[]	= "SD Hidden 10";
static const char *FWDN_SD_HD_AREA_NAME[] = {
	FWDN_SD_HD0_AREA_NAME,
	FWDN_SD_HD1_AREA_NAME,
	FWDN_SD_HD2_AREA_NAME,
	FWDN_SD_HD3_AREA_NAME,
	FWDN_SD_HD4_AREA_NAME,
	FWDN_SD_HD5_AREA_NAME,
	FWDN_SD_HD6_AREA_NAME,
	FWDN_SD_HD7_AREA_NAME,
	FWDN_SD_HD8_AREA_NAME,
	FWDN_SD_HD9_AREA_NAME,
	FWDN_SD_HD10_AREA_NAME,
};
static const char FWDN_SD_DATA_AREA_NAME[]	= "SD Data";

MMC_DISK_INFO_T		triflashDiskInfo;


//==============================================================
//
//		External Variables
//
//==============================================================
extern unsigned long				FirmwareSize;

//extern unsigned long				ImageRwBase;

//extern char							FirmwareVersion[];

extern unsigned int					gMAX_ROMSIZE;
extern const unsigned int			CRC32_TABLE[];
//==============================================================
//
//		External Functionss
//
//==============================================================
#define FWDN_DRV_MESSAGE_SIZE_MAX		200
#define FWDN_DRV_MESSAGE_LINE_MAX		5
static unsigned short sFWDN_DRV_usMessageSize[FWDN_DRV_MESSAGE_LINE_MAX];
static unsigned char sFWDN_DRV_ucMessageLinePush;
static unsigned char sFWDN_DRV_ucMessageLinePop;
static unsigned int sFWDN_DRV_pMessage[FWDN_DRV_MESSAGE_LINE_MAX][FWDN_DRV_MESSAGE_SIZE_MAX/4];
void FWDN_DRV_ClearMessage(void)
{
	sFWDN_DRV_ucMessageLinePush = 0;
	sFWDN_DRV_ucMessageLinePop = 0;
}

int FWDN_DRV_AddMessage(const char *pErrMsg)
{
	if(sFWDN_DRV_ucMessageLinePush<FWDN_DRV_MESSAGE_LINE_MAX)
	{
		strncpy((char*)sFWDN_DRV_pMessage[sFWDN_DRV_ucMessageLinePush],pErrMsg,FWDN_DRV_MESSAGE_SIZE_MAX-1);
		sFWDN_DRV_usMessageSize[sFWDN_DRV_ucMessageLinePush] = strlen((char*)sFWDN_DRV_pMessage[sFWDN_DRV_ucMessageLinePush]) + 1;
		sFWDN_DRV_ucMessageLinePush++;
		return TRUE;
	}
	else
		return FALSE;
}

int FWDN_DRV_GetMessage(unsigned int **ppMessage, unsigned int *pLength)
{
	if(sFWDN_DRV_ucMessageLinePop<sFWDN_DRV_ucMessageLinePush)
	{
		*pLength = (unsigned int)sFWDN_DRV_usMessageSize[sFWDN_DRV_ucMessageLinePop];
		*ppMessage = sFWDN_DRV_pMessage[sFWDN_DRV_ucMessageLinePop++];
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void FWDN_DRV_Reset(void)
{
	// goto reset vector

	FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_RESET_NOT_SUPPORT);
}

int FWDN_DRV_SessionStart(void)
{
	FWDN_TRACE("===== FWDN Session Start! =====\n");

	s_FWDN_DRV_SessionFlag = 0x00000000;

	return TRUE;
}

int FWDN_DRV_SessionEnd(unsigned int bSuccess)
{
	if (!target_is_emmc_boot())
	{
		#if defined(_NAND_BOOT)
		NAND_RO_Area_Flush_WriteCache(0);
		#endif
	}

	if(bSuccess)
		FWDN_TRACE("===== FWDN Session End! =====\n");
	else
		FWDN_TRACE("===== FWDN Session is Failed!=====\n");

	s_FWDN_DRV_SessionFlag = 0xFFFFFFFF;

	return TRUE;
}

#if defined(_NAND_BOOT)
static void FWDN_DRV_NAND_ProgressHandler( unsigned int uiCurrent, unsigned int uiTotal )
{
	if(gfxnFWDN_DRV_Progress != NULL)
	{
		(*gfxnFWDN_DRV_Progress)( ( uiCurrent * 100 ) / uiTotal );
	}
}
#endif

int FWDN_DRV_Init(unsigned int bmFlag, const FXN_FWDN_DRV_Progress fxnFwdnDrvProgress, char *message, unsigned int messageSize)
{
	int res = 0;

	g_uiFWDN_OverWriteSNFlag = 0;

	gfxnFWDN_DRV_Progress = fxnFwdnDrvProgress;

	if(bmFlag & FWDN_DEVICE_INIT_BITMAP_RESERVED_MASK) {
		FWDN_DRV_AddMessage("Unknown Flag.");
		return -1;
	}

	if (target_is_emmc_boot()) {
		if(DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_INITIALIZE, NULL ) < 0 ) {
			FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_IOCTRL_DEV_INITIALIZE);
			res = -1;
		}

		if( bmFlag & FWDN_DEVICE_INIT_BITMAP_LOW_FORMAT ) {
#if defined(_EMMC_BOOT)
			if( erase_emmc(0, 0, 1) )
				printf("eMMC low format error has occurred!\n");
			else
				printf("eMMC erasing completed!\n");
#endif
		}
	}
#if defined(_NAND_BOOT)
	else {
		if(bmFlag & FWDN_DEVICE_INIT_BITMAP_DEBUG_LEVEL_FORMAT_MASK) {
			switch(bmFlag&FWDN_DEVICE_INIT_BITMAP_DEBUG_LEVEL_FORMAT_MASK) {
				case FWDN_DEVICE_INIT_BITMAP_DEBUG_LEVEL_FORMAT_ERASE_ONLY:
					FWDN_DRV_AddMessage("Format - Erase Only");
					res = NAND_Init(0,NAND_INIT_TYPE_LOWFORMAT_DEBUG_ERASE_ONLY,FWDN_DRV_NAND_ProgressHandler);
					break;
				default:
					FWDN_DRV_AddMessage("Unknown Init Flag.");
					return -1;
					break;
			}
		}
		else if(bmFlag & FWDN_DEVICE_INIT_BITMAP_UPDATE) {
			res = NAND_Init(0,NAND_INIT_TYPE_LOWFORMAT,FWDN_DRV_NAND_ProgressHandler);
		}
		else if(bmFlag & FWDN_DEVICE_INIT_BITMAP_DUMP) {
		}
		else {
			NAND_ERROR_T errNAND = NAND_Init(0,NAND_INIT_TYPE_NORMAL,FWDN_DRV_NAND_ProgressHandler);
			if(errNAND == NAND_SUCCESS)
				res = 0;
			else if(errNAND == NAND_ERROR_INIT_AREA_CHANGE) {
				memcpy(message, "NAND partition changes is detected.\nIt will erase whole NAND.", messageSize);
				message[messageSize-1] = 0;
				res = 1;
			}
			else {
				FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_IOCTRL_DEV_INITIALIZE);
				res = -1;
			}
		}
	}
#endif
	return res;
}

static void _loc_strncpy(char *dst, const char *src, unsigned int n)
{
	while(n--) {
		*dst++=*src;
		if(*src++==0)
			break;
	}
}

static int _loc_strcmp(const char *dst, const char *src)
{
	while( *dst!=0 || *src!=0 )
	{
		if( *dst++ != *src++ )
			return -1;
	}
	return 0;
}

static void _loc_register_area(unsigned int index, const char *name, unsigned int nSector)
{
	memset(FWDN_DeviceInformation.area[index].name,0,16);
	_loc_strncpy(FWDN_DeviceInformation.area[index].name,name,16);
	FWDN_DeviceInformation.area[index].nSector = nSector;
}

//static void _loc_register_default_disk(unsigned int diskID)
//{
//	_loc_register_disk(0,diskID);
//}

pFWDN_DEVICE_INFORMATION FWDN_DRV_GetDeviceInfo(void)
{
#if defined (NKUSE)
	memset(&FWDN_DeviceInformation,0,sizeof(FWDN_DeviceInformation));
#else
	unsigned int i, nDisk=0, iSize=0;

	memset(&FWDN_DeviceInformation,0,sizeof(FWDN_DeviceInformation));

	switch(gFWDN_FirmwareStorage)
	{
		case FWDN_DISK_NAND:
			iSize = FwdnGetNandSerial(FWDN_DeviceInformation.DevSerialNumber,sizeof(FWDN_DeviceInformation.DevSerialNumber));
			if(iSize == 32)
				FWDN_DeviceInformation.DevSerialNumberType = SN_VALID_32;
			else
				FWDN_DeviceInformation.DevSerialNumberType = SN_NOT_EXIST;
			break;
		case FWDN_DISK_TRIFLASH:
			fwdn_mmc_get_serial_num();
			break;
		case FWDN_DISK_HDD:
			FwdnGetNorSerial();
			break;
		case FWDN_DISK_SNOR:
			FwdnGetSerialNorSerial();
			break;
	}

	// Reset All Area Information
	for(i=0;i<FWDN_AREA_LIST_MAX;i++)
		_loc_register_area(i,"",0);

	if (target_is_emmc_boot()) {
		unsigned long 	ulDataTotalSector;
		unsigned long	ulHiddenCount;

		DISK_Ioctl(DISK_DEVICE_TRIFLASH,DEV_GET_HIDDEN_COUNT, (void *)&ulHiddenCount);
		for(i=0;i<ulHiddenCount;i++) {
			ioctl_diskhidden_t stDiskHidden;
			stDiskHidden.ulIndex = (int)i;
			stDiskHidden.ulTotalSector = 0;
			DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_GET_HIDDEN_SIZE, (void *)&stDiskHidden);

			if(stDiskHidden.ulTotalSector) {
				_loc_register_area(
								nDisk++,
								FWDN_SD_HD_AREA_NAME[i],
								stDiskHidden.ulTotalSector
				);
			}
		}

		//SD data area total size
		DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_GET_DISK_SIZE, (void *)&ulDataTotalSector);

		_loc_register_area(
						nDisk++,
						FWDN_SD_DATA_AREA_NAME,
#if 0
#########
#if !defined(BOOT_USING_MBR)
						ulDataTotalSector + 1 // because of sd/mmc sector count == > EXTCSD.sectorcount -1
#else
						ulDataTotalSector
#endif
#endif
						ulDataTotalSector

		);
	}
	else {
#if defined(_NAND_BOOT)
		for(i=0; i<NAND_Area_GetCount(0);i++) {
			unsigned int uiTotalSector = NAND_Area_GetTotalSectorCount(0, i);
			if(uiTotalSector != 0) {
				_loc_register_area(
								nDisk++,
								FWDN_NAND_AREA_NAME[i],
								uiTotalSector
				);
			}
		}
#endif
	}


#endif
	return &FWDN_DeviceInformation;
}

int	FWDN_DRV_SerialNumberWrite(unsigned char *serial, unsigned int overwrite)
{
	int	res	= -1;

	switch (gFWDN_FirmwareStorage) {
		case FWDN_DISK_NAND:
			res = FwdnSetNandSerial( serial, overwrite);
			break;
		case FWDN_DISK_TRIFLASH:
			res = FwdnSetBootSDSerial( serial, overwrite);
			break;
		case FWDN_DISK_HDD:
			res = FwdnSetNorSerial( serial, overwrite);
			break;
		case FWDN_DISK_SNOR:
			res = FwdnSetSerialFlashSerial( serial, overwrite);
			break;
	}

	if ( res == 0)
		return TRUE;
	else
		return FALSE;
}

int FWDN_DRV_FirmwareWrite(unsigned int fwSize, FXN_FWDN_DRV_FirmwareWrite_ReadFromHost fxnFWDN_DRV_FirmwareWrite_ReadFromHost)
{
	int ret = 0;
	gfxnFWDN_DRV_FirmwareWrite_ReadFromHost = fxnFWDN_DRV_FirmwareWrite_ReadFromHost;

	switch (gFWDN_FirmwareStorage)
	{
		case FWDN_DISK_NAND:
		{
			unsigned char *pucRomBuffer = (unsigned char*)FWUG_GetTempBuffer(fwSize);
			if(pucRomBuffer == NULL)
				return ERR_FWUG_FAIL_MEMORY_ALLOCATION;
			if(FWDN_DRV_FirmwareWrite_Read(0, pucRomBuffer, fwSize, 100))
					return ERR_FWUG_FAIL_COMMUNICATION;
			return 	FwdnWriteNandFirmware(pucRomBuffer,fwSize);
		}
		case FWDN_DISK_TRIFLASH:
			ret = fwdn_mmc_update_bootloader( fwSize, 0 );
			ret = fwdn_mmc_update_bootloader( fwSize, 1 );
			return ret;
		//case FWDN_DISK_HDD:
		//case FWDN_DISK_NOR:
		//	return	FwdnWriteNorFlashFirmware(fwSize);
		case FWDN_DISK_SNOR:
			return  FWDN_SNOR_Write_Firmware(NULL, fwSize);
	}

	return	0;
}

int FWDN_DRV_FirmwareWrite_Read(unsigned int addr, unsigned char *buff, unsigned int size, unsigned int percent)
{
	return (*gfxnFWDN_DRV_FirmwareWrite_ReadFromHost)(buff, size, addr, percent);
}

int _loc_AreaRead(char *name, unsigned int lba, unsigned short nSector, void *buff)
{
	unsigned int i;

	if (target_is_emmc_boot()) {
		ioctl_diskrwpage_t rwHiddenPage;

		if( _loc_strcmp(FWDN_SD_DATA_AREA_NAME,name) == 0
				)
			return DISK_ReadSector(DISK_DEVICE_TRIFLASH, 0, lba, nSector, buff);

		for( i = 0; i < BOOTSD_HIDDEN_MAX_COUNT; i++ ) {
			if( _loc_strcmp(FWDN_SD_HD_AREA_NAME[i],name) == 0 ) {
				rwHiddenPage.start_page		= lba;
				rwHiddenPage.rw_size		= nSector;
				rwHiddenPage.hidden_index	= i;
				rwHiddenPage.buff			= buff;
				return DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_HIDDEN_READ_PAGE, (void *) &rwHiddenPage);
			}
		}
	}
	else {
#if defined(_NAND_BOOT)
		for(i=0 ; i<NAND_Area_GetCount(0); i++) {
			if(_loc_strcmp(FWDN_NAND_AREA_NAME[i],name) == 0)
				return NAND_Area_ReadSector(0, i, lba, nSector, buff);
		}
#endif
	}

	return -1;
}

int _loc_AreaWrite(char *name, unsigned int lba, unsigned short nSector, void *buff)
{
	unsigned int i;

	if (target_is_emmc_boot()) {
		ioctl_diskrwpage_t rwHiddenPage;

		if( _loc_strcmp(FWDN_SD_DATA_AREA_NAME,name) == 0
				)
			return DISK_WriteSector(DISK_DEVICE_TRIFLASH, 0, lba, nSector, buff);

		for(i = 0; i < BOOTSD_HIDDEN_MAX_COUNT; i++) {
			if(_loc_strcmp(FWDN_SD_HD_AREA_NAME[i],name) == 0) {
				rwHiddenPage.start_page		= lba;
				rwHiddenPage.rw_size		= nSector;
				rwHiddenPage.hidden_index	= i;
				rwHiddenPage.buff			= buff;
				return DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_HIDDEN_WRITE_PAGE, (void *) &rwHiddenPage);
			}
		}
	}
	else {
#if defined(_NAND_BOOT)
		for(i=0 ; i<NAND_Area_GetCount(0); i++) {
			if(_loc_strcmp(FWDN_NAND_AREA_NAME[i],name) == 0)
				return NAND_Area_WriteSector(0, i, lba, nSector, buff);
		}
#endif
	}

	return -1;
}

int FWDN_DRV_AREA_Write( char *name
								,unsigned int lba
								,unsigned int nSector
								,unsigned int nRemain
								,FXN_FWDN_DRV_Response_RequestData fxnFwdnDrvResponseRequestData
								,FXN_FWDN_DRV_DirectReadFromHost fxnFwdnDrvDirectReadFromHost )
{
//#define BUNCH_SECTOR_SIZE		(0x80000000>>9)
#define BUNCH_SECTOR_SIZE		(1024/*KB*/*2)

	unsigned char *receiveBuf;// = (unsigned char *)fwdndBuf;
	//unsigned char *verifyBuf  = (unsigned char *)veriBuf;
	unsigned short nReceiveSector;
	unsigned int nReceiveBytes;
	int _result = TRUE;
	unsigned int nBunch;

	if( nSector > BUNCH_SECTOR_SIZE )
	{
		nBunch = BUNCH_SECTOR_SIZE;
	}
	else
	{
		nBunch = nSector;
	}
	nSector -= nBunch;
	fxnFwdnDrvResponseRequestData(nBunch<<9);

		while(nBunch)
		{
			nReceiveBytes = fxnFwdnDrvDirectReadFromHost((void**)&receiveBuf);
			if((nReceiveBytes&0x1FF)!=0 || receiveBuf == NULL)
			//if( fxnFwdnDrvReadFromHost(receiveBuf, nReceiveSector<<9) != nReceiveSector<<9 )
			{
				return FALSE;
			}
			nReceiveSector = (unsigned short)(nReceiveBytes>>9);
			nBunch -= nReceiveSector;

			if(nBunch == 0 && nSector > 0 && _result == TRUE)
			{
				if( nSector > BUNCH_SECTOR_SIZE )
				{
					nBunch = BUNCH_SECTOR_SIZE;
				}
				else
				{
					nBunch = nSector;
				}
				nSector -= nBunch;
				fxnFwdnDrvResponseRequestData(nBunch<<9);
			}

			if(_result == TRUE && _loc_AreaWrite(name,lba,nReceiveSector,receiveBuf)!=0)
			{
				FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_AREA_WRITE);
				_result = FALSE;
			}
			//else if(_result == TRUE && _loc_DiskRead(name,lba,nReceiveSector,verifyBuf)!=0)
			//{
			//	FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_AREA_READ);
			//	_result = FALSE;
			//}
			//else if _result == TRUE && (memcmp(receiveBuf, verifyBuf, nReceiveSector<<9) != 0)
			//{
			//	FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_AREA_WRITE_COMPARE);
			//	_result = FALSE;
			//}

			lba += nReceiveSector;
		}
	return _result;
}

int FWDN_DRV_AREA_Read( char *name
								,unsigned int lba
								,unsigned int nSector
								,unsigned int nRemain
								,FXN_FWDN_DRV_Response_NotifyData fxnFwdnDrvResponseNotifyData
								,FXN_FWDN_DRV_SendToHost fxnFwdnDrvSendToHost )
{
//#define BUNCH_SECTOR_SIZE		(0x80000000>>9)
//#define BUNCH_SECTOR_SIZE		(1024/*KB*/*2)

	//unsigned char *receiveBuf;// = (unsigned char *)fwdndBuf;
	//unsigned char *verifyBuf  = (unsigned char *)veriBuf;
	//unsigned short nReceiveSector;
	//unsigned int nReceiveBytes;
	//int _result = TRUE;
	//unsigned int nBunch;

	return FALSE;
}

int FWDN_DRV_AREA_CalcCRC( char *name
							,unsigned int lba
							,unsigned int nSector
							,unsigned int nRemain
							,unsigned int *pCrc
							,FXN_FWDN_DRV_SendStatus fxnFwdnDrvSendStatus )
{
#define SEND_STATUS_EVERY_N_SECTOR		2048
	unsigned char *_buf  = (unsigned char *)fwdndBuf;

	unsigned short nReadSector;
	unsigned int nCalcedSector = 0;
	unsigned int nNextSendStatusSector = SEND_STATUS_EVERY_N_SECTOR;

	{
		if(nRemain!=0)
		{
			return FALSE;
		}
	}

	while(nSector)
	{
		if(nCalcedSector>nNextSendStatusSector)
		{
			nNextSendStatusSector += SEND_STATUS_EVERY_N_SECTOR;
			fxnFwdnDrvSendStatus(nCalcedSector,0,0);
		}

		if( nSector > TbufferSectorSize )
			nReadSector = TbufferSectorSize;
		else
			nReadSector = (unsigned short)nSector;

		if(_loc_AreaRead(name,lba,nReadSector,_buf)!=0)
		{
			FWDN_DRV_SetErrorCode(ERR_FWDN_DRV_AREA_READ);
			return FALSE;
		}
		*pCrc = FWUG_CalcCrc8I(*pCrc,_buf,(unsigned int)nReadSector<<9);

		lba += nReadSector;
		nCalcedSector += nReadSector;
		nSector -= nReadSector;
	}

	return TRUE;
}

#define	FWDN_DRV_BLOCKREAD_BUFFER_SIZE		(32*1024)		// because of LGE CDC
unsigned char FWDN_DRV_DUMP_InfoRead(void *pBuf)
{
	//unsigned int  i = 0;
	unsigned char length = 0;

	return length;
}

int FWDN_DRV_DUMP_BlockRead(unsigned int Param0, unsigned int Param1, unsigned int Param2,
											FXN_FWDN_DRV_Response_NotifyData fxnFwdnDrvResponseNotifyData,
											FXN_FWDN_DRV_SendToHost fxnFwdnDrvSendToHost)
{
	return FALSE;
}

int FWDN_DRV_DUMP_BlockWrite(unsigned int Param0, unsigned int Param1, unsigned int Param2,
											FXN_FWDN_DRV_Response_RequestData fxnFwdnDrvResponseRequestData,
											FXN_FWDN_DRV_ReadFromHost fxnFwdnDrvReadFromHost)
{
	return FALSE;
}

static int _loc_GetAreaSize(char *name)
{
#if defined (_NAND_BOOT)
	unsigned int i;
#endif
	if (target_is_emmc_boot())
		return 0;
#if defined (_NAND_BOOT)
	for(i=0 ; i<NAND_Area_GetCount(0); i++) {
		if(_loc_strcmp(FWDN_NAND_AREA_NAME[i],name) == 0)
			return NAND_Area_GetTotalSectorCount(0, i);
	}

	return 0;
#else
	return -1;
#endif
}

#define DUMP_ADDR KERNEL_ADDR + 0x100000

int _loc_Physical_AreaRead( void *buff, FXN_FWDN_DRV_Response_NotifyData fxnFwdnDrvResponseNotifyData, FXN_FWDN_DRV_SendToHost fxnFwdnDrvSendToHost )
{

	return FALSE;

#if 0
	ioctl_diskphysical_t 	ioctl_physical_info;
	int res = FALSE;
	unsigned int uiBlockAddress;;
	unsigned int uiTotalPhyBlockNum;
	unsigned int uiNANDPhyReadSize, uiBlockSize;
	unsigned int uiUsbBufferIndex, uiRequestSize;

	unsigned char *pucUsbBuffer = (unsigned char*)DUMP_ADDR;

	// Get Total NAND Physical Block Number.
	DISK_Ioctl( DISK_DEVICE_NAND, DEV_GET_PHYSICAL_  (void*) &ioctl_physical_info );

	uiTotalPhyBlockNum	= ioctl_physical_info.uiBlockNumberOfDevice;
	uiNANDPhyReadSize	= ioctl_physical_info.uiPageSize + ioctl_physical_info.uiSpareSize;
	uiBlockSize			= uiNANDPhyReadSize << ioctl_physical_info.uiShiftPpB;

	FWDN_TRACE("[Physical Total Block:%d] [Page:%d] [Spare:%d] [BlockSize : %d]\n", uiTotalPhyBlockNum,
		   		ioctl_physical_info.uiPageSize, ioctl_physical_info.uiSpareSize, uiBlockSize );

	// Read All Physical Block.
	for( uiBlockAddress = 0 ; uiBlockAddress < uiTotalPhyBlockNum; uiBlockAddress++ )
	{
		res = NAND_ReadPhysicalBlock( uiBlockAddress, uiNANDPhyReadSize, (void*)DUMP_ADDR );

		uiRequestSize 	 = uiBlockSize;
		uiUsbBufferIndex = 0;

		fxnFwdnDrvResponseNotifyData( uiRequestSize, 0 );

		while( uiRequestSize > 0 )
		{
			unsigned int packetSize = uiRequestSize>FWDN_DRV_BLOCKREAD_BUFFER_SIZE? FWDN_DRV_BLOCKREAD_BUFFER_SIZE : uiRequestSize;
			uiRequestSize -= packetSize;
			fxnFwdnDrvSendToHost( &pucUsbBuffer[uiUsbBufferIndex], packetSize );
			uiUsbBufferIndex += packetSize;
		}
	}

	return res;
#endif
}

int FWDN_DRV_Dump(char *pName, unsigned int uiAddress, unsigned int uiSize,
											FXN_FWDN_DRV_Response_NotifyData fxnFwdnDrvResponseNotifyData,
											FXN_FWDN_DRV_SendToHost fxnFwdnDrvSendToHost)
{
	int res = FALSE;

	if (target_is_emmc_boot())
		return res;

#if defined (_NAND_BOOT)
	if( _loc_strcmp(pName,"boot") == 0 ) {
		uiSize = NAND_DumpExtArea( (unsigned char *)DUMP_ADDR );
		if( uiSize == 0 )
		{
			FWDN_TRACE(" NAND Dump Fail !!\n");
			return FALSE;
		}
		fxnFwdnDrvResponseNotifyData(uiSize,0);
		fxnFwdnDrvSendToHost((void*)DUMP_ADDR, uiSize);

		res = TRUE;
	}
	else if( _loc_strcmp( pName, "NAND" ) == 0 )
	{
		FWDN_TRACE(" NAND Physical Dump ! \n" );
		FWDN_TRACE(" Size : %d, NAME:%s, Address:%d \n", uiSize, pName, uiAddress );
		res = _loc_Physical_AreaRead((void*)DUMP_ADDR, fxnFwdnDrvResponseNotifyData, fxnFwdnDrvSendToHost );
	}
	else if( _loc_GetAreaSize(pName) != 0 )
#endif
	{
		if( uiSize == 0 )
		{
			uiAddress = 0;
			uiSize = _loc_GetAreaSize(pName);
		}
		while( uiSize > 0 )
		{
			unsigned int uiPacketSectorSize = (uiSize>(FWDN_DRV_BLOCKREAD_BUFFER_SIZE/512))? (FWDN_DRV_BLOCKREAD_BUFFER_SIZE/512) : uiSize;
			fxnFwdnDrvResponseNotifyData(uiPacketSectorSize<<9,0);
			_loc_AreaRead(pName, uiAddress, uiPacketSectorSize, (void*)DUMP_ADDR);
			fxnFwdnDrvSendToHost((void*)DUMP_ADDR, uiPacketSectorSize<<9 );
			uiAddress += uiPacketSectorSize;
			uiSize -= uiPacketSectorSize;
		}

		res = TRUE;
	}
	return res;
}

int FWDN_DRV_TOTALBIN_Read(FXN_FWDN_DRV_Response_NotifyData fxnFwdnDrvResponseNotifyData,
							FXN_FWDN_DRV_SendToHost fxnFwdnDrvSendToHost)
{
	return FALSE;
}

int FWDN_DRV_TOTALBIN_Write(FXN_FWDN_DRV_RquestData fxnFwdnDrvRRequestData)
{
	return FALSE;
}

unsigned int FWDN_FNT_SetSN(unsigned char* ucTempData, unsigned int uiSNOffset)
{
	unsigned int		uiVerifyCrc;
	unsigned int		uiTempCrc;

	if (memcmp( &ucTempData[20+uiSNOffset], "ZERO", 4 ) == 0)
	{
		uiVerifyCrc = (ucTempData[uiSNOffset + 16] <<24) ^ ( ucTempData[uiSNOffset + 17] <<16) ^
			( ucTempData[uiSNOffset + 18] << 8) ^ ( ucTempData[uiSNOffset + 19]);
		uiTempCrc = FWUG_CalcCrc8((uiSNOffset + ucTempData), 16 );

		if ( ( uiTempCrc == uiVerifyCrc ) || (uiVerifyCrc == 0x0000 )) 	//16 bytes Serial Exist
		{
			g_uiFWDN_WriteSNFlag = 1;
			return SUCCESS;
		}
	}
	else if (memcmp(&ucTempData[20+uiSNOffset], "FWDN", 4) == 0 || memcmp(&ucTempData[20+uiSNOffset], "GANG", 4) == 0)
	{
		uiVerifyCrc = ( ucTempData[uiSNOffset + 16] <<24) ^ ( ucTempData[uiSNOffset + 17] <<16) ^
			( ucTempData[uiSNOffset + 18] << 8) ^ ( ucTempData[uiSNOffset + 19]);
		uiTempCrc = FWUG_CalcCrc8( (uiSNOffset + ucTempData), 16 );
		if  (uiVerifyCrc == 0x0000 ) {
			g_uiFWDN_WriteSNFlag = 0; 										// cleared SN
			return 1;
		}
		else {
			if (memcmp(&ucTempData[52+uiSNOffset], "FWDN", 4) == 0 || memcmp(&ucTempData[52+uiSNOffset], "GANG", 4) == 0) {
				uiVerifyCrc = ( ucTempData[uiSNOffset + 48] <<24) ^ ( ucTempData[uiSNOffset + 49] <<16) ^
					( ucTempData[uiSNOffset + 50] << 8) ^ ( ucTempData[uiSNOffset + 51]);
				uiTempCrc = FWUG_CalcCrc8((uiSNOffset + ucTempData+32), 16 );
				if (( uiVerifyCrc == uiTempCrc ) && ( uiVerifyCrc != 0x0000 )) {
					g_uiFWDN_WriteSNFlag = 1;
					return SUCCESS;
				}
				else
					g_uiFWDN_WriteSNFlag = 0; 										// cleared SN
			}
			else
				g_uiFWDN_WriteSNFlag = 0; 										// cleared SN
		}
	}
	return 1;
}

/**************************************************************************
*  FUNCTION NAME :
*
*      void FWDN_FNT_VerifySN(unsigned char* ucTempData, unsigned int uiSNOffset);
*
*  DESCRIPTION : Verify the validity of Serial Number format
*
*  INPUT:
*			ucTempData	= base pointer of serial number format
*			uiSNOffset	= offset of serial number format
*
*  OUTPUT:	void - Return Type
*			update global structure of FWDN_DeviceInformation according to verification result.
*
**************************************************************************/
void FWDN_FNT_VerifySN(unsigned char* ucTempData, unsigned int uiSNOffset)
{
	unsigned int		uiVerifyCrc;
	unsigned int		uiTempCrc;

	/*---------------------------------------------------------------------
		Check Type for Serial Number of 16 digits
	----------------------------------------------------------------------*/
	if (memcmp(&ucTempData[20+uiSNOffset], "ZERO", 4) == 0)
	{
		uiVerifyCrc = (ucTempData[uiSNOffset + 16] <<24) ^ ( ucTempData[uiSNOffset + 17] <<16) ^
			( ucTempData[uiSNOffset + 18] << 8) ^ ( ucTempData[uiSNOffset + 19]);
		uiTempCrc = FWUG_CalcCrc8((uiSNOffset + ucTempData), 16 );

		if ( ( uiTempCrc == uiVerifyCrc ) || (uiVerifyCrc == 0x0000 ))
		{
			if (uiVerifyCrc == 0x0000 )
			{
				memset(FWDN_DeviceInformation.DevSerialNumber, 0x30, 16);
				memset(FWDN_DeviceInformation.DevSerialNumber+16, 0x30, 16);
				FWDN_DeviceInformation.DevSerialNumberType = SN_INVALID_16;
			}
			else
			{
				memcpy(FWDN_DeviceInformation.DevSerialNumber, (uiSNOffset + ucTempData), 16);
				memset(FWDN_DeviceInformation.DevSerialNumber+16, 0x30, 16);
				FWDN_DeviceInformation.DevSerialNumberType = SN_VALID_16;
			}
		}
	}
	/*---------------------------------------------------------------------
		Check Type for Serial Number of 32 digits
	----------------------------------------------------------------------*/
	else if (memcmp(&ucTempData[20+uiSNOffset], "FWDN", 4) == 0 || memcmp(&ucTempData[20+uiSNOffset], "GANG", 4) == 0)
	{
		uiVerifyCrc = ( ucTempData[uiSNOffset + 16] <<24) ^ ( ucTempData[uiSNOffset + 17] <<16) ^
			( ucTempData[uiSNOffset + 18] << 8) ^ ( ucTempData[uiSNOffset + 19]);
		uiTempCrc = FWUG_CalcCrc8(uiSNOffset + ucTempData, 16 );
		if  (uiVerifyCrc == 0x0000 )
		{
			memset(FWDN_DeviceInformation.DevSerialNumber, 0x30, 32);
			FWDN_DeviceInformation.DevSerialNumberType = SN_INVALID_32;
		}
		else
		{
			if (memcmp(&ucTempData[52+uiSNOffset], "FWDN", 4) == 0 || memcmp(&ucTempData[52+uiSNOffset], "GANG", 4) == 0)
			{
				uiVerifyCrc = ( ucTempData[uiSNOffset + 48] <<24) ^ ( ucTempData[uiSNOffset + 49] <<16) ^
					( ucTempData[uiSNOffset + 50] << 8) ^ ( ucTempData[uiSNOffset + 51]);
				uiTempCrc = FWUG_CalcCrc8(( uiSNOffset + ucTempData + 32), 16 );
				if (( uiVerifyCrc == uiTempCrc ) && ( uiVerifyCrc != 0x0000 ))
				{
					memcpy(FWDN_DeviceInformation.DevSerialNumber, (uiSNOffset + ucTempData), 16);
					memcpy(FWDN_DeviceInformation.DevSerialNumber+16, (uiSNOffset + ucTempData + 32 ), 16);
					FWDN_DeviceInformation.DevSerialNumberType = SN_VALID_32;
				}
				else
				{
					memset(FWDN_DeviceInformation.DevSerialNumber, 0x30, 32);
					FWDN_DeviceInformation.DevSerialNumberType = SN_INVALID_32;
				}
			}
			else
			{
				memset(FWDN_DeviceInformation.DevSerialNumber, 0x30, 32);
				FWDN_DeviceInformation.DevSerialNumberType = SN_INVALID_32;
			}
		}
	}
	else
	{
		memset(FWDN_DeviceInformation.DevSerialNumber, 0x30, 32);
		FWDN_DeviceInformation.DevSerialNumberType = SN_NOT_EXIST;
	}
}

void FWDN_FNT_InsertSN(unsigned char *pSerialNumber)
{
	unsigned	uCRC;

	memcpy (pSerialNumber, FWDN_DeviceInformation.DevSerialNumber, 16 );
	uCRC	= FWUG_CalcCrc8(pSerialNumber, 16 );

	pSerialNumber[16] = (uCRC & 0xFF000000) >> 24;
	pSerialNumber[17] = (uCRC & 0x00FF0000) >> 16;
	pSerialNumber[18] = (uCRC & 0x0000FF00) >> 8;
	pSerialNumber[19] = (uCRC & 0x000000FF) ;

	memcpy(pSerialNumber+20, "FWDN", 4);
	memset(pSerialNumber+24, 0x00, 8);
	memcpy(pSerialNumber+32, FWDN_DeviceInformation.DevSerialNumber+16, 16);
	uCRC	= FWUG_CalcCrc8(pSerialNumber+32, 16 );

	pSerialNumber[48] = (uCRC & 0xFF000000) >> 24;
	pSerialNumber[49] = (uCRC & 0x00FF0000) >> 16;
	pSerialNumber[50] = (uCRC & 0x0000FF00) >> 8;
	pSerialNumber[51] = (uCRC & 0x000000FF) ;

	memcpy(pSerialNumber+52, "FWDN", 4);
	memset(pSerialNumber+56, 0x00, 8);
}

unsigned char FWDN_DRV_FirmwareStorageID(void)
{
	return (unsigned int)gFWDN_FirmwareStorage;
}


/************* end of file *************************************************************/
