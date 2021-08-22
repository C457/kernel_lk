/****************************************************************************
 *   FileName    : fwdn_snor.c
 *   Description : Serial NOR F/W downloader function
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#include <common.h>
#include <malloc.h>

#ifdef SNOR_BOOT_INCLUDE
#include <asm/arch/fwdn/fwdn_drv_v7.h>
#include <asm/arch/fwdn/fwupgrade.h>
#include "../snor/snor.h"
#if defined(CONFIG_ARCH_TCC897X)
#include "../snor/SNOR_Loader_TCC897X.h"
#elif defined(CONFIG_ARCH_TCC898X)
#include "../snor/SNOR_Loader_TCC898X.h"
#endif

#include <asm/arch/sdmmc/sd_memory.h>
DECLARE_GLOBAL_DATA_PTR;

#if defined(TSBM_ENABLE) || defined(TCC_NSK_ENABLE)
#include <tcsb/nsk.h>
#endif

#define BOOTROM_COPY_NUM		2

unsigned int g_progress = 0;
unsigned int g_progress_step = 0;

#if defined(TSBM_ENABLE)
	#define			FWDN_WRITE_BUFFER_SIZE		(2*1024*1024)
#elif defined(TCC_NSK_ENABLE)
	#define			FWDN_WRITE_BUFFER_SIZE		(1024*1024)
#else
	#define			FWDN_WRITE_BUFFER_SIZE		(32*1024)
#endif
unsigned int	tmpBuf[FWDN_WRITE_BUFFER_SIZE];

#define SNOR_PAGE_SIZE			0x100
#define SNOR_LOADER_BASE_ADDRESS	SNOR_Loader_BASE_ADDRESS //SNOR_Loader_TCC897X_BASE_ADDRESS
#define SNOR_LOADER_ROM				SNOR_Loader_ROM

extern const unsigned				CRC32_TABLE[];

/* enable/disable debugging messages */
#define	DEBUG_TCC_FWDN_SNOR
#undef	DEBUG_TCC_FWDN_SNOR

#ifdef  DEBUG_TCC_FWDN_SNOR
# define SNOR_FWDN_DEBUG(fmt, args...)	printf(fmt ,##args)
# define SNOR_FWDN_ADDR_DEBUG(fmt, args...)	printf(fmt ,##args)
# define SNOR_FWDN_LOG(fmt, args...)	printf(fmt ,##args)
#else
# define SNOR_FWDN_DEBUG(fmt, args...)
# define SNOR_FWDN_ADDR_DEBUG(fmt, args...)
# define SNOR_FWDN_LOG(fmt, args...)		printf(fmt ,##args)
#endif

#define SNOR_WRITE_DATA_VERIFY
#define SNOR_WRITE_DATA_VERIFY_BUFFER_SIZE	(8*1024)

extern int memcmp(const void *, const void *, unsigned int);
extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);

//=============================================================================
//*
//*
//*                           [ CONST DATA DEFINE ]
//*
//*
//=============================================================================

//=============================================================================
//*
//*
//*                     [ EXTERN VARIABLE & FUNCTIONS DEFINE ]
//*
//*
//=============================================================================
extern unsigned int						g_uiFWDN_OverWriteSNFlag;
extern FWDN_DEVICE_INFORMATION			FWDN_DeviceInformation;

unsigned int snor_round_addr(unsigned int size)
{
	int res;
	res = ((size + SNOR_PAGE_SIZE - 1) & 0xFFFFFF00);
	SNOR_FWDN_DEBUG("%s[0x%08X->0x%08X]\n", __func__, size, res);
	return 	res;
}

int FwdnWriteCodeSerialNor(unsigned uSFBase, unsigned uiROMFileSize)
{
	unsigned int uSFLast = uSFBase+uiROMFileSize;
	unsigned int uiSrcAddr = 0;
	/*---------------------------------------------------------------------
		Check ROM Image Size
	----------------------------------------------------------------------*/
	//if (pSFInfo == 0)
	SNOR_FWDN_DEBUG("[%s: %d]\n", __func__, __LINE__);
	if(SNOR_GetBootSize() == 0){
		SNOR_FWDN_DEBUG("err 1\n");
		return	ERR_FWUG_NOT_EXISTMEMORY;
	}

	SNOR_FWDN_DEBUG("[%s: %d]\n", __func__, __LINE__);

	if (uiROMFileSize >  SNOR_GetBootSize()){
		
		SNOR_FWDN_DEBUG("err 2\n");
		return	ERR_FWUG_FAIL_ROMFILESIZEBIG;
	}

	/*---------------------------------------------------------------------
		Firmware Download with F/W upgrade function
	----------------------------------------------------------------------*/
	//#define			FWDN_WRITE_BUFFER_SIZE		(8*1024)
	//unsigned int writeBuffer[FWDN_WRITE_BUFFER_SIZE/4];
	//unsigned int verifBuffer[FWDN_WRITE_BUFFER_SIZE/4];
	unsigned char*writeBuffer = (unsigned char*)ROMFILE_TEMP_BUFFER;
	unsigned char*verifBuffer = (unsigned char*)(ROMFILE_TEMP_BUFFER+FWDN_WRITE_BUFFER_SIZE);
	
	while(uiROMFileSize)
	{
		unsigned int packetSize = (uiROMFileSize>FWDN_WRITE_BUFFER_SIZE)? FWDN_WRITE_BUFFER_SIZE : uiROMFileSize;
					
		if (FWDN_DRV_FirmwareWrite_Read(uiSrcAddr,(unsigned char*)writeBuffer,packetSize,(uSFBase*100)/uSFLast) != 0){
			SNOR_FWDN_DEBUG("err 4\n");
			return ERR_FWUG_FAIL_COMMUNICATION;
		}

		uiSrcAddr += packetSize;
		SNOR_Write(uSFBase,writeBuffer,packetSize);
		SNOR_Read(uSFBase,verifBuffer,packetSize);
		if(memcmp(writeBuffer,verifBuffer,packetSize)!=0){
			SNOR_FWDN_DEBUG("err 5\n");
			return ERR_FWUG_FAIL_COMPARE;
		}
		uSFBase += packetSize;
		uiROMFileSize -= packetSize;
	}
	
	return	SUCCESS;
}

static void FwdnGenerateSerialNorBootHeader(unsigned uSFBase, unsigned uFWBase, unsigned uFWSize,
							sSFBootHeader *pSFBootHeader, unsigned uOSN)
{
	unsigned int	uiCRC;

	/*---------------------------------------------------------------------
		Read Headder
	----------------------------------------------------------------------*/
	SNOR_Read(uSFBase, pSFBootHeader, sizeof(*pSFBootHeader));

	/*---------------------------------------------------------------------
		Verify Header if overwrite flag is off.
	----------------------------------------------------------------------*/
	if (uOSN == 0)
	{
		// Read Header Information.
		uiCRC = FWUG_CalcCrc8((unsigned char*)pSFBootHeader, sizeof(*pSFBootHeader) - 4);
		if (uiCRC != pSFBootHeader->uCRC)
			uOSN	= 1;
		else if (FWDN_FNT_SetSN(pSFBootHeader->SerialNumber, 0) != SUCCESS)
			uOSN	= 1;
	}

	/*---------------------------------------------------------------------
		Set Booting Parameter
	----------------------------------------------------------------------*/
	pSFBootHeader->MID			= 0xFFFF;
	pSFBootHeader->DID			= 0xFFFF;
	pSFBootHeader->SectorSize	= 0xFFFFFFFF;
	pSFBootHeader->BlockSize	= 0xFFFFFFFF;
	pSFBootHeader->BlockNum		= 0xFFFFFFFF;
	pSFBootHeader->uStart		= uFWBase;
	pSFBootHeader->uDest		= uFWBase;
	pSFBootHeader->uLength		= uFWSize;
//	pSFBootHeader->uDLDV		= 0x00000000;
	pSFBootHeader->uDLDV		= 0x8000000f;

	memset(pSFBootHeader->Name,0xFF,sizeof(pSFBootHeader->Name));
	memset(pSFBootHeader->Rsv,0xFF,sizeof(pSFBootHeader->Rsv));
	if (uOSN)
	{
		/*---------------------------------------------------------------------
			Overwrite New Serial Number
		----------------------------------------------------------------------*/
		FWDN_FNT_InsertSN(pSFBootHeader->SerialNumber);
 	}
}

int	FwdnWriteSerialNorDataArea(unsigned int uiROMFileSize)
{
	int nSector;
	
	SNOR_UnitErase(SNOR_GetDataAreaAddress(), SNOR_GetDataAreaSize());

	nSector =( (uiROMFileSize + 511) >> 9);
	SNOR_FWDN_DEBUG("[%s: %d] Romfile Size: %d, Sector count: %d\n", __func__, __LINE__, uiROMFileSize, nSector);

	SNOR_DataAreaUpdate_Write(0, nSector, (void*)ROMFILE_TEMP_BUFFER);

	return 0;
}


int FWDN_SNOR_romfile_read( unsigned char * pReadBuffer, unsigned int nRomFileSize)
{
	int ret = 0;

	g_progress = 10;
	
	ret = FWDN_DRV_FirmwareWrite_Read( 0,	// rom file binary offset
									  (unsigned char*)pReadBuffer,
									  nRomFileSize, // rom file total size
									  g_progress );
	if(ret != 0) {
		SNOR_FWDN_DEBUG("[%s] Firmware Read Fail!\n", __func__);
		ret = ERR_FWUG_FAIL_COMMUNICATION;
	} else {
		SNOR_FWDN_DEBUG("[%s] Firmware Read Success!\n", __func__);
	}

	return ret;
}

int FWDN_SNOR_Write_Header( sTCCBootRomHeader *spTCCBootRomHeader, unsigned char *pucRomFile_Buffer, unsigned int *puiSNorAddress )
{
	sSFBootHeader sfBootHeader;
	sSFBootHeader sfBootHeaderVerif;
	unsigned int uiSNORBL1_Base_addr, uiSNORBL1_size;
	unsigned int *puiRomfile_Buffer = (unsigned int*)pucRomFile_Buffer;

	uiSNORBL1_Base_addr = puiRomfile_Buffer[ (spTCCBootRomHeader->uSnorLoaderBaseAddr - spTCCBootRomHeader->uFirmwareBaseAddress )>> 2];
	uiSNORBL1_size = puiRomfile_Buffer[ (spTCCBootRomHeader->uSnorLoaderSize - spTCCBootRomHeader->uFirmwareBaseAddress )>> 2];

	SNOR_FWDN_DEBUG("[%s] uiSNORBL1_base_addr: 0x%08X\n", __func__, uiSNORBL1_Base_addr);
	SNOR_FWDN_DEBUG("[%s] uiSNORBL1_size: 0x%08X\n", __func__, uiSNORBL1_size);
	
	// This function should be called before chip erasing
	//SNOR_FWDN_LOG(" +FwdnGenerateSerialNorBootHeader\n");
	FwdnGenerateSerialNorBootHeader( SNOR_GetBootAddress(),
									uiSNORBL1_Base_addr,	
									uiSNORBL1_size,
									&sfBootHeader,
									g_uiFWDN_OverWriteSNFlag);
	//SNOR_FWDN_LOG(" -FwdnGenerateSerialNorBootHeader\n");

	//===============================
	// Partition Info
	//===============================
	sfBootHeader.kernel_addr	= SNOR_GetKernelAddress();
	sfBootHeader.kernel_size	= SNOR_GetKernelSize();
	sfBootHeader.data_addr		= SNOR_GetDataAreaAddress();
	sfBootHeader.data_size		= SNOR_GetDataAreaSize();
	sfBootHeader.boot_part_size = SNOR_GetBoot1stRomPartSize();
	
	/*---------------------------------------------------------------------
		Calc CRC
	----------------------------------------------------------------------*/
	sfBootHeader.uCRC = FWUG_CalcCrc8((unsigned char*)&sfBootHeader, sizeof(sSFBootHeader) - 4);
	
	SNOR_FWDN_LOG("[%s] Boot partition Erase: 0x%08x++0x%08x\n", __func__, SNOR_GetBootAddress(), SNOR_GetBootSize());
	SNOR_UnitErase(SNOR_GetBootAddress(), SNOR_GetBootSize());

	*puiSNorAddress = SNOR_GetBootAddress();

	// Write Boot Header
	SNOR_FWDN_LOG("[%s] Write: 0x%08x++0x%08x\n", __func__, *puiSNorAddress, sizeof(sSFBootHeader));
	SNOR_Write(*puiSNorAddress,&sfBootHeader,sizeof(sSFBootHeader));
	SNOR_Read(*puiSNorAddress,&sfBootHeaderVerif,sizeof(sSFBootHeader));
	
	SNOR_FWDN_DEBUG("sfBootHeader.MID = %x \n", sfBootHeader.MID);
	SNOR_FWDN_DEBUG("sfBootHeader.DID = %x \n", sfBootHeader.DID);
	SNOR_FWDN_DEBUG("sfBootHeader.SectorSize = %x \n", sfBootHeader.SectorSize);
	SNOR_FWDN_DEBUG("sfBootHeader.BlockSize = %x \n", sfBootHeader.BlockSize);
	SNOR_FWDN_DEBUG("sfBootHeader.BlockNum = %x \n", sfBootHeader.BlockNum);
	SNOR_FWDN_DEBUG("sfBootHeader.uStart = %x \n", sfBootHeader.uStart);
	SNOR_FWDN_DEBUG("sfBootHeader.uDest = %x \n", sfBootHeader.uDest);
	SNOR_FWDN_DEBUG("sfBootHeader.uLength = %x \n", sfBootHeader.uLength);
	SNOR_FWDN_DEBUG("sfBootHeader.uDLDV = %x \n", sfBootHeader.uDLDV);
	SNOR_FWDN_DEBUG("sfBootHeader.Name = %s \n", sfBootHeader.Name);
	SNOR_FWDN_DEBUG("sfBootHeader.SerialNumber = %s \n", sfBootHeader.SerialNumber);
	SNOR_FWDN_DEBUG("sfBootHeader.kernel_addr = %x \n", sfBootHeader.kernel_addr);
	SNOR_FWDN_DEBUG("sfBootHeader.data_addr = %x \n", sfBootHeader.data_addr);
	SNOR_FWDN_DEBUG("sfBootHeader.data_size = %x \n", sfBootHeader.data_size);
	SNOR_FWDN_DEBUG("sfBootHeader.boot_part_size = %x \n", sfBootHeader.boot_part_size);
	SNOR_FWDN_DEBUG("sfBootHeader.uCRC = %x \n", sfBootHeader.uCRC);

	if(memcmp(&sfBootHeader,&sfBootHeaderVerif,sizeof(sSFBootHeader))!=0){
		SNOR_FWDN_LOG("Verify Boot Header Error!!\n");
		return ERR_FWUG_FAIL_COMPARE;
	}
	
	*puiSNorAddress += snor_round_addr(sizeof(sfBootHeader));

	SNOR_FWDN_LOG("[%s] Return Addr: 0x%08x\n", __func__, *puiSNorAddress);

	return 0;
}


int FWDN_SNOR_Write_BL1( sTCCBootRomHeader *spTCCBootRomHeader, unsigned char *pucRomFile_Buffer, unsigned int *puiSNorAddress )
{
	unsigned int uRomSize = 0;
	#ifdef SNOR_WRITE_DATA_VERIFY
	unsigned char *verifBuffer = NULL;
	unsigned int uRomddressOffset = 0;
	unsigned int	i;
	#endif
	unsigned int uiSNORBL1_addr, uiSNORBL1_size;
	unsigned int *puiRomfile_Buffer = (unsigned int *)pucRomFile_Buffer;
	uiSNORBL1_addr = spTCCBootRomHeader->uSnorLoaderStart - spTCCBootRomHeader->uFirmwareBaseAddress;
	uiSNORBL1_size = puiRomfile_Buffer[ (spTCCBootRomHeader->uSnorLoaderSize - spTCCBootRomHeader->uFirmwareBaseAddress )>> 2];
	SNOR_FWDN_DEBUG("[%s] uiSNORBL1_addr: 0x%08X\n", __func__, uiSNORBL1_addr);
	SNOR_FWDN_DEBUG("[%s] uiSNORBL1_size: 0x%08X\n", __func__, uiSNORBL1_size);
	
	// Write Serial Nor Flash Loader
	SNOR_FWDN_LOG("[%s] Write: 0x%08x++0x%08x\n", __func__, *puiSNorAddress, uiSNORBL1_size);
	SNOR_Write(*puiSNorAddress, &pucRomFile_Buffer[uiSNORBL1_addr], uiSNORBL1_size);

	#ifdef SNOR_WRITE_DATA_VERIFY
	SNOR_FWDN_DEBUG("[%s] +SNOR_WRITE_DATA_VERIFY\n", __func__);
	uRomSize = uiSNORBL1_size;
	verifBuffer = malloc(SNOR_WRITE_DATA_VERIFY_BUFFER_SIZE);
	if (!verifBuffer) {
		printf("[%s: %d] Low memory(cannot allocate memory for verify)\n", __func__, __LINE__);
		return ERR_FWUG_FAIL_MEMORY_ALLOCATION;
	}

	while(uRomSize>0)
	{
		unsigned int uPacketSize = (uRomSize>SNOR_WRITE_DATA_VERIFY_BUFFER_SIZE)? SNOR_WRITE_DATA_VERIFY_BUFFER_SIZE : uRomSize;
		SNOR_FWDN_DEBUG("[%s] Read: 0x%08x++0x%08x\n", __func__, (*puiSNorAddress + uRomddressOffset), uPacketSize);
		SNOR_Read(*puiSNorAddress + uRomddressOffset, verifBuffer, uPacketSize);
		if(memcmp((void*)(&pucRomFile_Buffer[uiSNORBL1_addr+uRomddressOffset]), verifBuffer, uPacketSize)!=0)
		{
			SNOR_FWDN_LOG("[%s] ERR_FWUG_FAIL_COMPARE\n", __func__);
			for (i = 0; i < uPacketSize; ++i){
				if((pucRomFile_Buffer[uiSNORBL1_addr+uRomddressOffset + i]) != verifBuffer[i])
					printf("[%d][%02x][%02x]\n", i, (pucRomFile_Buffer[uiSNORBL1_addr+uRomddressOffset + i]), verifBuffer[i]);
			}
			return ERR_FWUG_FAIL_COMPARE;
		}
		
		uRomSize -= uPacketSize;
		uRomddressOffset += uPacketSize;
	}

	if(verifBuffer != NULL)
		free(verifBuffer);
	SNOR_FWDN_DEBUG("[%s] -SNOR_WRITE_DATA_VERIFY: OK!\n", __func__);
	#endif /* SNOR_WRITE_DATA_VERIFY */
	
	*puiSNorAddress += snor_round_addr(uiSNORBL1_size);
	SNOR_FWDN_LOG("[%s] Return Addr: 0x%08x\n", __func__, *puiSNorAddress);

	return 0;
}

int FWDN_SNOR_Write_BootRom( sTCCBootRomHeader *spTCCBootRomHeader, unsigned char *pucRomFile_Buffer, unsigned int *puiSNorAddress )
{
	sSF2ndBootHeader sf2BootHeader;
	sSF2ndBootHeader sf2BootHeaderVerif;
	unsigned int 	uiSNorAddress_Start;
	unsigned int 	i;
	unsigned int	uiSrcAddr;
	unsigned int	uiRemainRomfileSize;
	unsigned char *verifBuffer = NULL;
	unsigned int uiRomfileSize;
	unsigned int	uiDRAMInitRom_addr;
	unsigned int	uiDRAMInitRom_size;
	unsigned int nPercent, nPercent_unit;

	uiDRAMInitRom_addr = spTCCBootRomHeader->uInitRoutine_StartBase;
	uiDRAMInitRom_size = spTCCBootRomHeader->uInitRoutine_StartLimit - uiDRAMInitRom_addr;
	SNOR_FWDN_DEBUG("[%s] uiDRAMInitRom_addr: 0x%08X\n", __func__, uiDRAMInitRom_addr);
	SNOR_FWDN_DEBUG("[%s] uiDRAMInitRom_size: 0x%08X\n", __func__, uiDRAMInitRom_size);
	uiRomfileSize = spTCCBootRomHeader->uFirmwareSize;
	
	uiSNorAddress_Start = *puiSNorAddress;

	verifBuffer = malloc(FWDN_WRITE_BUFFER_SIZE);
	if (!verifBuffer) {
		printf("[%s: %d] Low memory(cannot allocate memory for verify)\n", __func__, __LINE__);
		return ERR_FWUG_FAIL_MEMORY_ALLOCATION;
	}

	//======================================
	// Write BootRom (lk.rom or u-boot.rom)
	//======================================
	nPercent_unit = uiRomfileSize / FWDN_WRITE_BUFFER_SIZE;
	if(nPercent_unit == 0)
		nPercent_unit = 1;

	if( g_progress != 0 ) {
		g_progress_step = nPercent_unit;
		g_progress_step = (100 - g_progress) / (g_progress_step*BOOTROM_COPY_NUM);
	}
	
	nPercent_unit = 100 / (nPercent_unit*BOOTROM_COPY_NUM);
	
	for ( i = 0; i < BOOTROM_COPY_NUM; ++i) 
	{
		nPercent = 0;
		
		memset(&sf2BootHeader,0x00,sizeof(sSF2ndBootHeader));
		sf2BootHeader.uBaseAddress = spTCCBootRomHeader->uFirmwareBaseAddress;
		sf2BootHeader.uBootRomSize = uiRomfileSize;
		sf2BootHeader.uInitCodeBaseAddress = uiDRAMInitRom_addr - spTCCBootRomHeader->uFirmwareBaseAddress;
		sf2BootHeader.uInitCodeSize = uiDRAMInitRom_size;//((int)&InitRoutine_End - (int)&InitRoutine_Start);
		sf2BootHeader.uFrequency_MHz = SNOR_GetFrequency();
		sf2BootHeader.uCRC = FWUG_CalcCrc8((unsigned char*)&sf2BootHeader, sizeof(sSF2ndBootHeader) - 4);

		SNOR_FWDN_DEBUG("[%s] 2nd Header data ================\n", __func__);
		SNOR_FWDN_DEBUG("sf2BootHeader.uBaseAddress: 0x%08x\n", sf2BootHeader.uBaseAddress);
		SNOR_FWDN_DEBUG("sf2BootHeader.uLength: 0x%08x\n", sf2BootHeader.uBootRomSize);
		SNOR_FWDN_DEBUG("sf2BootHeader.uInitCodeAddress: 0x%08x\n", sf2BootHeader.uInitCodeAddress);
		SNOR_FWDN_DEBUG("sf2BootHeader.uInitCodeBaseAddress: 0x%08x\n", sf2BootHeader.uInitCodeBaseAddress);
		SNOR_FWDN_DEBUG("sf2BootHeader.uInitCodeSize: 0x%08x\n", sf2BootHeader.uInitCodeSize);
		SNOR_FWDN_DEBUG("sf2BootHeader.uFrequency_MHz: 0x%08x\n", sf2BootHeader.uFrequency_MHz);
		SNOR_FWDN_DEBUG("sf2BootHeader.uCRC: 0x%08x\n", sf2BootHeader.uCRC);

		SNOR_FWDN_LOG("[%s] Copy(%d) 2nd Header Write: 0x%08x++0x%08x\n", __func__, i,  *puiSNorAddress, sizeof(sSF2ndBootHeader));
		SNOR_Write(*puiSNorAddress, &sf2BootHeader, sizeof(sSF2ndBootHeader));
	
		SNOR_FWDN_DEBUG("[%s: %d]\n", __func__, __LINE__);
		
		SNOR_FWDN_DEBUG("[%s] 2nd Header data reload ================\n", __func__);
		SNOR_Read(*puiSNorAddress, &sf2BootHeaderVerif, sizeof(sSF2ndBootHeader));


		if(memcmp(&sf2BootHeader,&sf2BootHeaderVerif,sizeof(sSF2ndBootHeader))!=0){
			SNOR_FWDN_LOG("[%s] ERR_FWUG_FAIL_COMPARE\n", __func__);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uBaseAddress: 0x%08x\n", sf2BootHeaderVerif.uBaseAddress);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uLength: 0x%08x\n", sf2BootHeaderVerif.uBootRomSize);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uInitCodeAddress: 0x%08x\n", sf2BootHeaderVerif.uInitCodeAddress);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uInitCodeBaseAddress: 0x%08x\n", sf2BootHeaderVerif.uInitCodeBaseAddress);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uInitCodeSize: 0x%08x\n", sf2BootHeaderVerif.uInitCodeSize);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uFrequency_MHz: 0x%08x\n", sf2BootHeaderVerif.uFrequency_MHz);
			SNOR_FWDN_DEBUG("sf2BootHeaderVerif.uCRC: 0x%08x\n", sf2BootHeaderVerif.uCRC);			
			return ERR_FWUG_FAIL_COMPARE;
		}
		
		*puiSNorAddress += snor_round_addr(sizeof(sSF2ndBootHeader));

		// Write boot code
		SNOR_FWDN_LOG("[%s] Copy(%d) Boot Rom Write: 0x%08x++0x%08x\n", __func__, i, *puiSNorAddress, uiRomfileSize);
		uiSrcAddr = 0;
		uiRemainRomfileSize = uiRomfileSize;

		SNOR_FWDN_LOG("    ");
		while(uiRemainRomfileSize)
		{
			unsigned int packetSize = (uiRemainRomfileSize > FWDN_WRITE_BUFFER_SIZE)? FWDN_WRITE_BUFFER_SIZE : uiRemainRomfileSize;

			//dummy read for FWDN Progressbar
			if( g_progress_step != 0){
				if (FWDN_DRV_FirmwareWrite_Read(uiSrcAddr,(unsigned char*)verifBuffer, packetSize, g_progress) != 0){
					SNOR_FWDN_DEBUG("err 4\n");
					return ERR_FWUG_FAIL_COMMUNICATION;
				}
				g_progress += g_progress_step;
				if(g_progress > 100)
					g_progress = 100;
			}
			
			nPercent += nPercent_unit;
			SNOR_FWDN_LOG("\b\b\b\b%3d%%", nPercent);

			//SNOR_FWDN_DEBUG("[%s] - Boot Rom Write: 0x%08x++0x%08x\n", __func__, *puiSNorAddress, packetSize);
			SNOR_Write(*puiSNorAddress, &pucRomFile_Buffer[uiSrcAddr], packetSize);

			nPercent += nPercent_unit;
			SNOR_FWDN_LOG("\b\b\b\b%3d%%", nPercent);

			SNOR_Read(*puiSNorAddress, verifBuffer, packetSize);
			if(memcmp( &pucRomFile_Buffer[uiSrcAddr], verifBuffer, packetSize)!=0){
				SNOR_FWDN_DEBUG("err 5\n");
				return ERR_FWUG_FAIL_COMPARE;
			}

			uiSrcAddr += packetSize;
			*puiSNorAddress += packetSize;
			uiRemainRomfileSize -= packetSize;
		}
		
		*puiSNorAddress = uiSNorAddress_Start + SNOR_GetBoot1stRomPartSize();
		SNOR_FWDN_LOG("\b\b\b\b%3d%% OK.\n", 100);
	}

	free(verifBuffer);
	
	return 0;
}

int FWDN_SNOR_Write_Firmware( unsigned char * pucRomFile_Buffer, unsigned int uiROMFileSize)
{
	unsigned int	uSNorAddress;
	unsigned int	uiSNORBL1_size;
	unsigned int	uiTemp;
	int				res;
	unsigned int 	*puiRomfile_Buffer;
	unsigned char 	*pucRomFile = NULL;
	sTCCBootRomHeader *spTCCBootRomHeader;

	//=====================================================
	// Prepare Rom file 
	//=====================================================
	SNOR_FWDN_DEBUG("\x1b[1;24m[%s] Rom file size: %d byte @0x%08x..\x1b[0m\n", __func__, uiROMFileSize, (unsigned int)pucRomFile_Buffer);

	if( pucRomFile_Buffer == NULL) {
		pucRomFile = malloc(uiROMFileSize);
		if (!pucRomFile) {
			printf("[%s: %d] Low memory(cannot allocate memory for verify)\n", __func__, __LINE__);
			res = ERR_FWUG_FAIL_MEMORY_ALLOCATION;
			goto fwdn_snor_err;
		}

		// boot rom file read by usb
		FWDN_SNOR_romfile_read( pucRomFile, uiROMFileSize);
		spTCCBootRomHeader = (sTCCBootRomHeader *)pucRomFile;
	} else {
		pucRomFile = pucRomFile_Buffer;
		spTCCBootRomHeader = (sTCCBootRomHeader *)pucRomFile;
		uiROMFileSize = spTCCBootRomHeader->uFirmwareSize;
	}

	puiRomfile_Buffer = (unsigned int*)pucRomFile;
	
	//=====================================================
	// Parsing Romfile (LK: crt0.S / uboot: start.S)
	//=====================================================
	SNOR_FWDN_DEBUG("\x1b[1;24m[%s]Rom file Parsing..\x1b[0m\n", __func__);
	if( spTCCBootRomHeader->uSnorSig != 0x524F4E53 ) { // Check "SNOR" signature
		SNOR_FWDN_DEBUG("[%s] Can't Find SNOR signature\n", __func__);
		res = ERR_FWUG_FAIL_COMMUNICATION;
		goto fwdn_snor_err;
	}

	if( uiROMFileSize != spTCCBootRomHeader->uFirmwareSize ){

		SNOR_FWDN_DEBUG("[%s] Firmware Size missmatch [%d] != [%d]\n", __func__, uiROMFileSize, spTCCBootRomHeader->uFirmwareSize);
		res = ERR_FWUG_FAIL_COMMUNICATION;
		goto fwdn_snor_err;
	}

	SNOR_FWDN_DEBUG("[%s] Find SNOR signature\n", __func__);
	uiSNORBL1_size = puiRomfile_Buffer[ (spTCCBootRomHeader->uSnorLoaderSize - spTCCBootRomHeader->uFirmwareBaseAddress )>> 2];
	
	//=====================================================
	// SNOR Check...
	//=====================================================
	if(SNOR_GetBootSize() == 0) {
		SNOR_FWDN_DEBUG("[%s] SNOR Get Boot size Fail\n", __func__);
		res = ERR_FWUG_NOT_EXISTMEMORY;
		goto fwdn_snor_err;
	}

	uiTemp =  snor_round_addr(uiROMFileSize);
	uiTemp += snor_round_addr(sizeof(sSFBootHeader));
	uiTemp += snor_round_addr(uiSNORBL1_size);
	if (uiTemp > SNOR_GetBootSize()) {
		res = ERR_FWUG_FAIL_ROMFILESIZEBIG;
		goto fwdn_snor_err;
	}

	SNOR_FWDN_LOG("[%s] SNOR Boot Area Size  : 0x%08x (%6d kbyte)\n", __func__, SNOR_GetBootSize(), SNOR_GetBootSize()>>10);
	SNOR_FWDN_LOG("[%s] BootRom Size to Write: 0x%08x (%6d kbyte)\n", __func__, uiTemp, uiTemp>>10);

	//=====================================================
	// 1st Boot Header Write
	//=====================================================
	SNOR_FWDN_LOG("\x1b[1;24m[%s] 1st Boot Header Write...\x1b[0m\n", __func__);
	res = FWDN_SNOR_Write_Header( spTCCBootRomHeader, pucRomFile, &uSNorAddress );
	if(res != 0)
		goto fwdn_snor_err;

	//======================================
	// BL 1 Write
	//======================================
	SNOR_FWDN_LOG("\x1b[1;24m[%s] BL1 Write...\x1b[0m\n", __func__);
	res = FWDN_SNOR_Write_BL1( spTCCBootRomHeader, pucRomFile, &uSNorAddress );
	if(res != 0)
		goto fwdn_snor_err;

	//=====================================================
	// 2nd Boot Header Write
	//=====================================================
	SNOR_FWDN_LOG("\x1b[1;24m[%s] Boot Rom (+ 2nd Boot Header) Write...\x1b[0m\n", __func__);

	res = FWDN_SNOR_Write_BootRom( spTCCBootRomHeader, pucRomFile, &uSNorAddress );
	if(res != 0)
		goto fwdn_snor_err;

	SNOR_FWDN_LOG("Done.\n");
	if( (pucRomFile_Buffer == NULL) && ( pucRomFile != NULL))
		free(pucRomFile);
	
	return 0;
	
fwdn_snor_err:

	if( (pucRomFile_Buffer == NULL) && ( pucRomFile != NULL))
		free(pucRomFile);
	
	return res;
}

#if defined(TSBM_ENABLE) || defined(TCC_NSK_ENABLE)

#define BL1_NUM	1

int fwdn_Write_SerialNor_secure_Firmware(unsigned int uiROMFileSize)
{
	unsigned char 	*buf = (unsigned char*)tmpBuf;
	unsigned char *dbg_buf;
	int				i, j;
	int				res;
	unsigned int	uSNorAddress, uSNorAddress_Start;
	unsigned int 	nImage_size;
	sSFBootHeader sfBootHeader;
	sSFBootHeader sfBootHeaderVerif;
	
	sSF2ndBootHeader sf2BootHeader;
	sSF2ndBootHeader sf2BootHeaderVerif;
	
	BOOTSD_sHeader_Info_T secure_h;

	#if defined(TCC_NSK_ENABLE)
	NSK_Header_Info_T nsk_h;
	memset(&nsk_h, 0x00, sizeof(NSK_Header_Info_T));
	#endif

	if(SNOR_GetBootSize() == 0)
		return	ERR_FWUG_NOT_EXISTMEMORY;

#if 0		/* 015.07.30 */
	{
		unsigned int uiTotalBootSize = uiROMFileSize;
		uiTotalBootSize += sizeof(sSFBootHeader);		// Header
		uiTotalBootSize += ((sizeof(SNOR_LOADER_ROM) + SNOR_PAGE_SIZE - 1) & 0xFFFFFF00);	// SNOR Loader
		if (uiTotalBootSize > SNOR_GetBootSize())
			return	ERR_FWUG_FAIL_ROMFILESIZEBIG;
		
		//SNOR_FWDN_LOG("SNOR_GetBootSize/cur Size/uiROMFileSize is[0x%x]/[0x%x]/[0x%x]\n",SNOR_GetBootAddress(),uiTotalBootSize,uiROMFileSize);
	}
#endif /* 0 */
	SNOR_FWDN_LOG("Boot Write start...\n");

	FWDN_DRV_FirmwareWrite_Read(0, buf, uiROMFileSize ,100 ); /*FWDN_PROGRESS_RATE(100));*/

	memset( &secure_h, 0x00, sizeof(BOOTSD_sHeader_Info_T) );
	memcpy( &secure_h, buf, sizeof(BOOTSD_sHeader_Info_T) );

	//dbg_buf = &secure_h; 
	//
	//for(i =0; i < 128; ++i){
	//	if( (i%8) == 0)
	//		printf("\n");
	//
	//	printf("%02X-%02X ", buf[i], dbg_buf[i]);
	//}

	//==================================
	// Make sSFBootHeader 
	//==================================
	{
		// This function should be called before chip erasing
#if defined(TCC_NSK_ENABLE)
		FwdnGenerateSerialNorBootHeader(SNOR_GetBootAddress(), 0, 0, &sfBootHeader, g_uiFWDN_OverWriteSNFlag);
#else
		// BL1 boot 
		FwdnGenerateSerialNorBootHeader( SNOR_GetBootAddress(),
										SNOR_LOADER_BASE_ADDRESS,
										(sizeof(SNOR_LOADER_ROM)),
										&sfBootHeader,
										g_uiFWDN_OverWriteSNFlag);
#endif
		//SNOR_FWDN_LOG(" -FwdnGenerateSerialNorBootHeader\n");
		//===============================
		// Partition Info
		//===============================
		sfBootHeader.kernel_addr	= SNOR_GetKernelAddress();
		sfBootHeader.kernel_size	= SNOR_GetKernelSize();
		sfBootHeader.data_addr		= SNOR_GetDataAreaAddress();
		sfBootHeader.data_size		= SNOR_GetDataAreaSize();

#if defined(TCC_NSK_ENABLE)
		sfBootHeader.boot_part_size = 0;
		sfBootHeader.uSF2Header_addr[0] = snor_round_addr(sizeof(sSFBootHeader));
		sfBootHeader.uNSKHeader_addr[0] = snor_round_addr(sfBootHeader.uSF2Header_addr[0] 
																+ sizeof(sSF2ndBootHeader) 
																+ secure_h.dram_init_size);
#else
		sfBootHeader.boot_part_size = snor_round_addr(sizeof(SNOR_LOADER_ROM));

		sfBootHeader.uSF2Header_addr[0] = sizeof(sSFBootHeader) + (sfBootHeader.boot_part_size << (BL1_NUM - 1));
		sfBootHeader.uNSKHeader_addr[0] = 0;
		sfBootHeader.uNSKHeader_addr[1] = 0;

		/*---------------------------------------------------------------------
			Calc CRC
		----------------------------------------------------------------------*/
		sfBootHeader.uCRC = FWUG_CalcCrc8((unsigned char*)&sfBootHeader, sizeof(sSFBootHeader) - 4);
		SNOR_FWDN_DEBUG("sfBootHeader.uCRC: 0x%08X\n", sfBootHeader.uCRC);
#endif
		SNOR_FWDN_DEBUG("sfBootHeader.uSF2Header_addr[0]: 0x%08X, size: 0x%08X\n", 
						 sfBootHeader.uSF2Header_addr[0], sizeof(sSF2ndBootHeader));
		SNOR_FWDN_DEBUG("sfBootHeader.uNSKHeader_addr[0]: 0x%08X, size: 0x%08X\n",
						 sfBootHeader.uNSKHeader_addr[0], sizeof(NSK_Header_Info_T));
	}
	
#if defined(TCC_NSK_ENABLE)
	//==================================
	// Make NSK Header
	//==================================
	{
		// copy cisco key
		memcpy(nsk_h.ucCiscoKey1ECDSA, (unsigned char*)(buf+(unsigned int)secure_h.kcisco1_start), secure_h.kcisco1_size);
		nsk_h.ucCiscoKey1ECDSASize = secure_h.kcisco1_size;
		nsk_h.ulCM4CodeBase = snor_round_addr(sfBootHeader.uNSKHeader_addr[0] + sizeof(NSK_Header_Info_T));
		nsk_h.ulCM4CodeSize = secure_h.cm4_size;
		nsk_h.ulCA7ACodeBase = snor_round_addr(nsk_h.ulCM4CodeBase + nsk_h.ulCM4CodeSize);
		nsk_h.ulCA7ACodeSize = secure_h.ca7in_size;
		nsk_h.ulCA7CodeBase = snor_round_addr(nsk_h.ulCA7ACodeBase + nsk_h.ulCA7ACodeSize);
		nsk_h.ulCA7CodeSize = secure_h.ca7ext_size;
		nsk_h.lob0 = secure_h.rev0_base;
		nsk_h.lob1 = secure_h.rev02;
		memcpy(nsk_h.rev0, (unsigned char*)(buf+(unsigned int)secure_h.rev00_start), secure_h.rev00_size + secure_h.rev01_size);
		memcpy(nsk_h.rev1, (unsigned char*)(buf+(unsigned int)secure_h.rev1_start), secure_h.rev1_size);
		nsk_h.lob2 = secure_h.rev03;
	}
#endif

	//======================================
	// Make sSF2ndBootHeader
	//======================================
	{

		memset(&sf2BootHeader,0x00,sizeof(sSF2ndBootHeader));
		sf2BootHeader.uBaseAddress 	= FirmwareBaseAddress;		//0x10001000;
		sf2BootHeader.uBootRomSize 	= secure_h.bootloader_size;

		sf2BootHeader.uInitCodeSize = secure_h.dram_init_size;
#if defined(TCC_NSK_ENABLE)
		sf2BootHeader.uInitCodeAddress = snor_round_addr(sizeof(sSFBootHeader) + sizeof(sSF2ndBootHeader));
		sf2BootHeader.uBootRomAddress = snor_round_addr(nsk_h.ulCA7CodeBase + nsk_h.ulCA7CodeSize);
		sfBootHeader.uSF2Header_addr[1] = snor_round_addr(sf2BootHeader.uBootRomAddress + sf2BootHeader.uBootRomSize);
		sfBootHeader.uNSKHeader_addr[1] = snor_round_addr(sfBootHeader.uSF2Header_addr[1]
																+ sizeof(sSF2ndBootHeader) 
																+ secure_h.dram_init_size);

		/*---------------------------------------------------------------------
			Calc CRC
		----------------------------------------------------------------------*/
		sfBootHeader.uCRC = FWUG_CalcCrc8((unsigned char*)&sfBootHeader, sizeof(sSFBootHeader) - 4);
		SNOR_FWDN_DEBUG("sfBootHeader.uCRC: 0x%08X\n", sfBootHeader.uCRC);		
#else
		sf2BootHeader.uBootRomAddress = snor_round_addr(sfBootHeader.uSF2Header_addr[0] + sizeof(sSF2ndBootHeader));
		sf2BootHeader.uInitCodeAddress = snor_round_addr(sf2BootHeader.uBootRomAddress + sf2BootHeader.uBootRomSize);
#endif
		
		sf2BootHeader.uInitCodeBaseAddress = 0;
		sf2BootHeader.uFrequency_MHz = SNOR_GetFrequency();
		sf2BootHeader.uCRC = FWUG_CalcCrc8((unsigned char*)&sf2BootHeader, sizeof(sSF2ndBootHeader) - 4);

		SNOR_FWDN_DEBUG("sf2BootHeader.uBaseAddress: 0x%08x\n", sf2BootHeader.uBaseAddress);
		SNOR_FWDN_DEBUG("sf2BootHeader.uBootRomSize: 0x%08x\n", sf2BootHeader.uBootRomSize);
		SNOR_FWDN_DEBUG("sf2BootHeader.uBootRomAddress: 0x%08x\n", sf2BootHeader.uBootRomAddress);

		SNOR_FWDN_DEBUG("sf2BootHeader.uInitCodeSize: 0x%08x\n", sf2BootHeader.uInitCodeSize);
		SNOR_FWDN_DEBUG("sf2BootHeader.uInitCodeAddress: 0x%08x\n", sf2BootHeader.uInitCodeAddress);
		//SNOR_FWDN_DEBUG("sf2BootHeader.uInitCodeBaseAddress: 0x%08x\n", sf2BootHeader.uInitCodeBaseAddress);
		SNOR_FWDN_DEBUG("sf2BootHeader.uFrequency_MHz: 0x%08x\n", sf2BootHeader.uFrequency_MHz);
		SNOR_FWDN_DEBUG("sf2BootHeader.uCRC: 0x%08x\n", sf2BootHeader.uCRC);
	}

	//=================================================================
	// SNOR_ChipErase 
	//=================================================================
	SNOR_FWDN_LOG(" SNOR_ChipErase\n");
	SNOR_UnitErase(SNOR_GetBootAddress(), SNOR_GetBootSize());

	//======================================
	// Write sSFBootHeader
	//======================================
	uSNorAddress = 0;
	SNOR_FWDN_LOG(" +sSFBootHeader		- addr: 0x%08X, size: %d\n", uSNorAddress, sizeof(sSFBootHeader));
	SNOR_Write(uSNorAddress,&sfBootHeader,sizeof(sSFBootHeader));
	SNOR_Read(uSNorAddress,&sfBootHeaderVerif,sizeof(sSFBootHeader));
	if(memcmp(&sfBootHeader,&sfBootHeaderVerif,sizeof(sSFBootHeader))!=0)
		return ERR_FWUG_FAIL_COMPARE;
	SNOR_FWDN_LOG(" -sSFBootHeader\n");

#if defined(TCC_NSK_ENABLE)
	//
#else
	//======================================
	// BL 1 Write
	//======================================
	{
		#define VERIF_BUFFER_SIZE	(8*1024)
		//unsigned int verifBuffer[VERIF_BUFFER_SIZE/4];
		unsigned char*verifBuffer = (unsigned char*)ROMFILE_TEMP_BUFFER;
		unsigned int uRomSize = sizeof(SNOR_LOADER_ROM);
		unsigned int uRomddressOffset = 0;
		// Write Serial Nor Flash Loader

		sfBootHeaderVerif.boot_part_size
		
		uSNorAddress = sizeof(sSFBootHeader);

		for( i = 0; i < BL1_NUM; ++i ){
			SNOR_FWDN_LOG(" +BL1 rom - addr: 0x%08X, size: %d\n", uSNorAddress, sizeof(SNOR_LOADER_ROM));
			SNOR_Write(uSNorAddress,SNOR_LOADER_ROM,sizeof(SNOR_LOADER_ROM));
			SNOR_FWDN_LOG(" -BL1 rom\n");

			#if 1	// Write Data Verify
			while(uRomSize>0)
			{
				unsigned int uPacketSize = (uRomSize>VERIF_BUFFER_SIZE)? VERIF_BUFFER_SIZE : uRomSize;
				SNOR_FWDN_DEBUG("[%s: %d] uSNorAddress: %d\n", __func__, __LINE__,uSNorAddress);
				SNOR_Read(uSNorAddress+uRomddressOffset,verifBuffer,uPacketSize);
				SNOR_FWDN_DEBUG("[%s: %d]\n", __func__, __LINE__);
				if(memcmp((void*)((unsigned int)SNOR_LOADER_ROM+uRomddressOffset),verifBuffer,uPacketSize)!=0)
				{
					SNOR_FWDN_DEBUG("[%s: %d]\n", __func__, __LINE__);
					return ERR_FWUG_FAIL_COMPARE;
				}
				
				uRomSize -= uPacketSize;
				uRomddressOffset += uPacketSize;
			}
			#endif /* 0 */
		
			uSNorAddress += snor_round_addr(sizeof(SNOR_LOADER_ROM));
		}
	}
#endif

	//======================================
	// Write sSF2ndBootHeader
	//======================================
	uSNorAddress = sfBootHeader.uSF2Header_addr[0];
	SNOR_FWDN_LOG(" +sSF2ndBootHeader	- addr: 0x%08X, size: %d\n", uSNorAddress, sizeof(sSF2ndBootHeader));
	SNOR_Write(uSNorAddress,&sf2BootHeader,sizeof(sSF2ndBootHeader));
	SNOR_Read(uSNorAddress,&sf2BootHeaderVerif,sizeof(sSF2ndBootHeader));
	if(memcmp(&sf2BootHeader,&sf2BootHeaderVerif,sizeof(sSF2ndBootHeader))!=0)
		return ERR_FWUG_FAIL_COMPARE;
	SNOR_FWDN_LOG(" -sSF2ndBootHeader\n");


#if defined(TCC_NSK_ENABLE)
	//======================================
	//Write DRAM Init code
	//======================================
	uSNorAddress = sf2BootHeader.uInitCodeAddress;
	SNOR_FWDN_LOG(" +DRAM Init code		- addr: 0x%08X, size: %d\n", uSNorAddress, sf2BootHeader.uInitCodeSize);
	SNOR_Write( uSNorAddress, 
				(unsigned char*)(buf+(unsigned int)secure_h.dram_init_start),
				sf2BootHeader.uInitCodeSize);
	SNOR_FWDN_LOG(" -DRAM Init code\n");
	
	//======================================
	//Write NSK Header
	//======================================
	uSNorAddress = sfBootHeader.uNSKHeader_addr[0];
	SNOR_FWDN_LOG(" +NSK Header 		- addr: 0x%08X, size: %d\n", uSNorAddress, sizeof(NSK_Header_Info_T));
	SNOR_Write(uSNorAddress, &nsk_h, sizeof(NSK_Header_Info_T));
	SNOR_FWDN_LOG(" -NSK Header\n");
	
	//======================================
	//Write CM4
	//======================================
	uSNorAddress = nsk_h.ulCM4CodeBase;
	SNOR_FWDN_LOG(" +CM4 Init code		- addr: 0x%08X, size: %d\n", uSNorAddress, nsk_h.ulCM4CodeSize);
	SNOR_Write(uSNorAddress,
				(unsigned char*)(buf+(unsigned int)secure_h.cm4_start),
				nsk_h.ulCM4CodeSize);




#if 0		/* 015.08.02 */
	{
		#define VERIF_BUFFER_SIZE	(8*1024)
		//unsigned int verifBuffer[VERIF_BUFFER_SIZE/4];
		unsigned char*verifBuffer = (unsigned char*)ROMFILE_TEMP_BUFFER;


		dbg_buf = (unsigned char*)(buf+(unsigned int)secure_h.cm4_start);
		
		for(i =0; i < 128; ++i){
			if( (i%8) == 0)
				printf("\n");
		
			printf("%02X ", dbg_buf[i]);
		}

		printf("--------------------------\n");
		SNOR_Read(uSNorAddress, (unsigned char*)(buf+(unsigned int)secure_h.cm4_start), 128);

		dbg_buf = (unsigned char*)(buf+(unsigned int)secure_h.cm4_start);

		for(i =0; i < 128; ++i){
			if( (i%8) == 0)
				printf("\n");

			printf("%02X ", dbg_buf[i]);
		}
	}
#endif /* 0 */




	SNOR_FWDN_LOG(" -CM4\n");

	//======================================
	//Write CA7A
	//======================================
	uSNorAddress = nsk_h.ulCA7ACodeBase;
	SNOR_FWDN_LOG(" +CA7A Init code		- addr: 0x%08X, size: %d\n", uSNorAddress, nsk_h.ulCA7ACodeSize);
	SNOR_Write(uSNorAddress,
				(unsigned char*)(buf+(unsigned int)secure_h.ca7in_start),
				nsk_h.ulCA7ACodeSize);
	SNOR_FWDN_LOG(" -CA7A\n");

	//======================================
	//Write CA7
	//======================================
	uSNorAddress = nsk_h.ulCA7CodeBase;
	SNOR_FWDN_LOG(" +CA7 Init code 		- addr: 0x%08X, size: %d\n", uSNorAddress, nsk_h.ulCA7CodeSize);
	SNOR_Write(uSNorAddress,
				(unsigned char*)(buf+(unsigned int)secure_h.ca7ext_start),
				nsk_h.ulCA7CodeSize);
	SNOR_FWDN_LOG(" -CA7\n");

	//======================================
	//Bootloader
	//======================================
	uSNorAddress = sf2BootHeader.uBootRomAddress;
	SNOR_FWDN_LOG(" +Bootloader 		- addr: 0x%08X, size: %d\n", uSNorAddress, sf2BootHeader.uBootRomSize);
	SNOR_Write(uSNorAddress,
				(unsigned char*)(buf+(unsigned int)secure_h.bootloader_start),
				sf2BootHeader.uBootRomSize);
	SNOR_FWDN_LOG(" -Bootloader\n");
#else
	//======================================
	//Bootloader
	//======================================
	uSNorAddress = sf2BootHeader.uBootRomAddress;
	SNOR_FWDN_LOG(" +Bootloader 		- addr: 0x%08X, size: %d\n", uSNorAddress, sf2BootHeader.uBootRomSize);
	SNOR_Write(uSNorAddress,
				(unsigned char*)(buf+(unsigned int)secure_h.bootloader_start),
				sf2BootHeader.uBootRomSize);
	SNOR_FWDN_LOG(" -Bootloader\n");

	//======================================
	//Write DRAM Init code
	//======================================
	uSNorAddress = sf2BootHeader.uInitCodeAddress;
	SNOR_FWDN_LOG(" +DRAM Init code		- addr: 0x%08X, size: %d\n", uSNorAddress, sizeof(sSF2ndBootHeader));
	SNOR_Write( uSNorAddress, 
				(unsigned char*)(buf+(unsigned int)secure_h.dram_init_start),
				sf2BootHeader.uInitCodeSize);
	SNOR_FWDN_LOG(" -DRAM Init code\n");
#endif

	SNOR_FWDN_LOG("Done.\n");

#if 1
	{
		//unsigned int verifBuffer[VERIF_BUFFER_SIZE/4];
		unsigned int *verifBuffer;

		uSNorAddress = 0;
		
		for(j = 0; j < 8; ++j) {
			printf("[dump snor: %4X--------------------------]", uSNorAddress);
			SNOR_Read(uSNorAddress, (unsigned char*)buf, 256);

			verifBuffer = &buf[0];
		
			for(i =0; i < 64; ++i){
				if( (i%4) == 0)
					printf("\n");
	
				printf("%08X ", verifBuffer[i]);
			}
			
			printf("\n");

			uSNorAddress += 256;
		}
	}
#endif

	return 0;
}
#endif

int FwdnGetSerialNorSerial(void)
{
	sSFBootHeader	sfHeader;

	/*----------------------------------------------------------------
					G E T	S E R I A L	N U M B E R
	----------------------------------------------------------------*/
	//if (TCCAPI_SF_ReadPAGE((unsigned int)readData, 0, 1) < 0)
	//	return ERR_FWUG_FAIL;
	SNOR_Read(0, &sfHeader, sizeof(sfHeader));

	/*----------------------------------------------------------------
					V E R I F Y	S E R I A L	N U M B E R
	----------------------------------------------------------------*/
	FWDN_FNT_VerifySN(sfHeader.SerialNumber, 0);

	return 0;
}

int FwdnSetSerialFlashSerial(unsigned char *ucData, unsigned int overwrite)
{
	memcpy((void*)FWDN_DeviceInformation.DevSerialNumber, ucData, 32);
	g_uiFWDN_OverWriteSNFlag = overwrite;

	return 0;
}

int SNOR_GetSerialNumber(char* Serial)
{
	unsigned int i;
	sSFBootHeader	sfHeader;
	SNOR_Read(0, &sfHeader, sizeof(sfHeader));

	for(i=0; i<16; i++)
		Serial[i] = sfHeader.SerialNumber[i];
	for(i=16 ; i<32; i++)
		Serial[i]= sfHeader.SerialNumber[i+16];

	return 0;
}

#endif //SNOR_BOOT_INCLUDE


