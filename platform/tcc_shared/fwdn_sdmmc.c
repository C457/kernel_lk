/****************************************************************************
 *   FileName    : fwdn_SDMMC.c
 *   Description : SDMMC F/W downloader function
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#if defined(_LINUX_) || defined(_WINCE_)
#include "IO_TCCXXX.h"
#include <string.h>
//#include "fwdn_protocol.h"
#include <fwdn/fwdn_drv_v7.h>
#include <fwdn/fwupgrade.h>
#endif

#include <fwdn/fwdn_sdmmc.h>
#include <fwdn/Disk.h>
#include <sdmmc/sd_memory.h>
#include <sdmmc/sd_update.h>
#include <debug.h>

#if defined(_LINUX_)
#define	LPRINTF	printf
#elif defined(_WINCE_)
#define	LPRINTF	EdbgOutputDebugString
#else
#define	LPRINTF
#endif

//=============================================================================
//*
//*
//*                     [ EXTERN VARIABLE & FUNCTIONS DEFINE ]
//*
//*
//=============================================================================
extern const unsigned int		CRC32_TABLE[256];
extern unsigned int				g_uiFWDN_OverWriteSNFlag;
extern unsigned int				g_uiFWDN_WriteSNFlag;

extern unsigned int				FWDN_FNT_SetSN(unsigned char* ucTempData, unsigned int uiSNOffset);
extern void						FWDN_FNT_InsertSN(unsigned char * pSerialNumber);
extern void						FWDN_FNT_VerifySN(unsigned char* ucTempData, unsigned int uiSNOffset);
extern void						IO_DBG_SerialPrintf_(char *format, ...);

extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);
extern void *malloc(size_t size);


#define			CRC32_SIZE	4
unsigned int	tempBuffer[FWDN_WRITE_BUFFER_SIZE];

#define FWDN_PROGRESS_RATE(A, B)  ((A *100) / B)

extern unsigned long  InitRoutine_Start;
extern unsigned long  InitRoutine_End;



static int fwdn_mmc_header_write(
		unsigned long base_addr,
		unsigned long config_size,
		unsigned long rom_size,
		unsigned int boot_partition,
		int fwdn_mode)
{
	return mmc_update_header( MEMBASE, config_size, rom_size, boot_partition, fwdn_mode );
}

static int fwdn_mmc_config_write( unsigned int rom_size,  // ks_8930
		unsigned int config_size, unsigned char *addr, int boot_partition)
{
	return mmc_update_config_code(rom_size, config_size, addr, boot_partition); //ks_8930
}

static int fwdn_mmc_rom_write (
		unsigned int rom_size ,
		unsigned int config_code_size ,
		unsigned char *buf ,
		/*unsigned int buf_size ,*/
		int boot_partition )
{
	/*FWDN_DRV_FirmwareWrite_Read(0, buf, rom_size ,FWDN_PROGRESS_RATE(0,rom_size));*/
	return mmc_update_rom_code( rom_size, config_code_size, buf, boot_partition );
}

int fwdn_mmc_update_bootloader(unsigned int rom_size, int boot_partition)
{
	unsigned char *buf = (unsigned char*)tempBuffer;
	unsigned int config_size = (unsigned int)&InitRoutine_End - (unsigned int)&InitRoutine_Start;
#if defined(TSBM_ENABLE)
	BOOTSD_sHeader_Info_T secure_h;
#endif
	if( mmc_update_valid_test(rom_size) )
		return -1;

	page_t p, q;
	unsigned int current_buf_index = 0;
	unsigned int rom_size_bak = rom_size;
	unsigned int full_size_of_buf = FWDN_WRITE_BUFFER_SIZE;
	unsigned int request_buf_size = FWDN_WRITE_BUFFER_SIZE;
	unsigned int partial_buf_size = 0;

#if defined(TSBM_ENABLE)
	FWDN_DRV_FirmwareWrite_Read(current_buf_index, buf, 512, FWDN_PROGRESS_RATE(512, rom_size_bak));
	if (init_secure_head(&secure_h, buf))
		return -1;

	if (!memcmp(secure_h.sc_tag, SECURE_TAG, SECURE_TAG_SIZE))
	{
		config_size = secure_h.dram_init_size;
		rom_size = secure_h.bootloader_size;
		if( fwdn_mmc_header_write(secure_h.base_address, config_size-CRC32_SIZE, rom_size, boot_partition, 1) )
			return -1;

		current_buf_index += 512;

		FWDN_DRV_FirmwareWrite_Read(current_buf_index, buf, config_size, FWDN_PROGRESS_RATE(current_buf_index, rom_size_bak));
		if( fwdn_mmc_config_write(rom_size, config_size, buf, boot_partition) )
			return -1;

		current_buf_index += config_size;
		config_size -= CRC32_SIZE;
	}
	else
#endif
	{
		if( fwdn_mmc_header_write( MEMBASE, config_size, rom_size, boot_partition, 1 ) )
			return -1;

		if( fwdn_mmc_config_write( rom_size, config_size, (unsigned char *)&InitRoutine_Start, boot_partition ) )
			return -1;
	}

	printf("[%s] Start to write rom code to boot_partition no.%d\n", __func__, boot_partition);
	request_buf_size = full_size_of_buf >> 1;
	request_buf_size &= 0xfffffe00;

	memset(buf, 0x0, request_buf_size);

	p.start_page = BOOTSD_GetROMAddr(rom_size, config_size+CRC32_SIZE, boot_partition);
	p.buff = buf;
	p.boot_partition = boot_partition;

	while (rom_size)
	{
		partial_buf_size = (rom_size > request_buf_size) ? request_buf_size : rom_size;
		p.rw_size = BYTE_TO_SECTOR(partial_buf_size);
		FWDN_DRV_FirmwareWrite_Read(current_buf_index, p.buff, partial_buf_size,
				FWDN_PROGRESS_RATE(current_buf_index, rom_size_bak));

		if (DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_BOOTCODE_WRITE_PAGE, (void*)&p) < 0)
		{
			printf("[%s] Fail to write rom code\n", __func__);
			return -1;
		}
	
		q.start_page = p.start_page;
		q.rw_size = p.rw_size;
		q.hidden_index = p.hidden_index;
		//q.buff = &buf[0] + request_buf_size;
		q.buff = p.buff + request_buf_size;
		q.boot_partition = p.boot_partition;
	
		if (DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_BOOTCODE_READ_PAGE, (void*)&q) ||
			memcmp(p.buff, q.buff, (p.rw_size)<<9))
		{
			printf("[%s] Fail to verify rom code\n", __func__);
			return -1;
		}
		rom_size -= partial_buf_size;
		current_buf_index += request_buf_size;
		p.start_page += p.rw_size;
		printf(".");
	}
	printf("\n");

	return 0;
}

#ifdef OTP_UID_INCLUDE
extern unsigned int TARGET_ADDR;
extern unsigned int SECURE_LK_SIZE;
int fwdn_mmc_otp_uid_include_update_bootloader(unsigned int rom_size, int boot_partition)
{
	unsigned char *buf = TARGET_ADDR;
	BOOTSD_sHeader_Info_T secure_h;

	rom_size = SECURE_LK_SIZE;

	if( mmc_update_valid_test(rom_size) )
		return -1;

	if( init_secure_head(&secure_h, buf) )
		return -1;

	printf("[otp_uid_include] secure_h.magic\t\t: %s\n", secure_h.magic);
	printf("[otp_uid_include] secure_h.magic\t\t: %x\n", secure_h.base_address);
	printf("[otp_uid_include] secure_h.dram_init_start\t: %llu\n", secure_h.dram_init_start);
	printf("[otp_uid_include] secure_h.dram_init_size\t: %llu\n", secure_h.dram_init_size);
	printf("[otp_uid_include] secure_h.bootloader_start\t: %llu\n", secure_h.bootloader_start);
	printf("[otp_uid_include] secure_h.bootloader_size\t: %llu\n", secure_h.bootloader_size);

	if( fwdn_mmc_header_write(
				MEMBASE,
				secure_h.dram_init_size-CRC32_SIZE, // remove crc size in advance
				secure_h.bootloader_size,
				boot_partition,
				1 ) )
		return -1;

	if( fwdn_mmc_config_write(
				secure_h.bootloader_size,
				secure_h.dram_init_size,
				(unsigned char*)(buf+(unsigned int)secure_h.dram_init_start),
				boot_partition ) )
		return -1;

	if( fwdn_mmc_rom_write(
				secure_h.bootloader_size,
				secure_h.dram_init_size-CRC32_SIZE, // remove crc size in advance
				buf + (unsigned int)secure_h.bootloader_start,
				boot_partition ) )
		return -1;

	return 0;
}
#endif


unsigned int FwdnReadBootSDFirmware(unsigned int master)
{
	return 0;
}

int fwdn_mmc_get_serial_num(void)
{
	return mmc_get_serial_num(NULL, 0);
}

int FwdnSetBootSDSerial(unsigned char *ucData, unsigned int overwrite)
{
	ioctl_diskinfo_t  disk_info;
	unsigned char   ucTempData[512];
	int       iRev;
	ioctl_diskrwpage_t  header;

	if (DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_GET_DISKINFO, (void *)&disk_info) < 0)
	{
		return -1;
	}
	if (disk_info.sector_size != 512)
		return -1;

	memcpy( FWDN_DeviceInformation.DevSerialNumber, ucData , 32);
	g_uiFWDN_OverWriteSNFlag  =  overwrite;
	g_uiFWDN_WriteSNFlag    = 0;

	if ( overwrite == 0 )
	{
		/*----------------------------------------------------------------
		 *       G E T S E R I A L N U M B E R
		 *             ----------------------------------------------------------------*/
		header.start_page = BOOTSD_GetHeaderAddr( 0 );
		header.rw_size    = 1;
		header.buff     = ucTempData;
		iRev  = DISK_Ioctl(DISK_DEVICE_TRIFLASH, DEV_BOOTCODE_READ_PAGE, (void *)&header);
		if(iRev != SUCCESS)
			return (-1);

		iRev = FWDN_FNT_SetSN(ucTempData, 32);
	}

	return 0;
}

