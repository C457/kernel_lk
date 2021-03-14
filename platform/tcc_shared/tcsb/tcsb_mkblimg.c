/****************************************************************************
 *   FileName    : tcsb_mkblimg.c
 *   Description : To make the bootloader image
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved
 This source code contains confidential information of Telechips.
 Any unauthorized use without a written permission of Telechips including not 
 limited to re-distribution in source or binary form is strictly prohibited.
 This source code is provided "AS IS" and nothing contained in this source 
 code shall constitute any express or implied warranty of any kind, 
 including without limitation, any warranty of merchantability, 
 fitness for a particular purpose or non-infringement of any patent, 
 copyright or other third party intellectual property right. 
 No warranty is made, express or implied, regarding the information's 
 accuracy, completeness, or performance.
 In no event shall Telechips be liable for any claim, damages or other 
 liability arising from, out of or in connection with this source 
 code or the use in the source code.
 This source code is provided subject to the terms of a Mutual 
 Non-Disclosure Agreement between Telechips and Company.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#if defined(CONFIG_ARM_TRUSTZONE)
#include <platform/smc.h>
#endif

#define FILE_OFFSET_BASE_ADDR			0X60
#define FILE_OFFSET_INIT_CODE			0X70

#define ROM_TNFTL_STRUCTURE_VERSION_OFFSET		0x78
#define ROM_BL1_NAND_OFFSET						0x7C

#define DEBUG

#if defined(DEBUG)
#define DBG 	printf
#else
#define DBG
#endif

#define BOOT_MAGIC "ANDROID@"
#define SECURE_TAG "TCSB"
#define ROM_TYPE "LBS_"
#if defined(NAND_USED)
#define STORAGE_TYPE "NAND"
#elif defined(EMMC_USED)
#define STORAGE_TYPE "EMMC"
#endif
#if defined(TCC896X)
#define TCC_TYPE "893X"	//"896X"
#elif defined(TCC897X)
#define TCC_TYPE "897X"	//"897X"
#else //if defined(TCC893X)
#define TCC_TYPE "893X"
#endif
#define FWDN_TYPE "V7  "


/*
 * SECURE BOOT HEADER FOR WHEN FIRMWARE DOWNLOAD
 * MAGIC CODE		8 BYTE
 * SECURE TAGS		4 BYTE
 * ROM TYPE		4 BYTE
 * BASE ADDRESS		4 BYTE
 * STORAGE TYPE		4 BYTE
 * FWDN TYPE		4 BYTE
 * TCC TYPE		4 BYTE
 * DRAM INIT START	8 BYTE
 * DRAM INIT SIZE 	8 BYTE
 * BOOTLOADER START	8 BYTE
 * BOOTLOADER SIZE	8 BYTE
 * NAND BL1 START	8 BYTE
 * NAND BL1 SIZE	8 BYTE
#if defined(CONFIG_ARM_TRUSTZONE)
 * TARGET ADDRESS	8 BYTE
 * TZOS START		8 BYTE // for Trust Zone
 * TZOS	SIZE		8 BYTE // for Trust Zone
 * RESERVED		400 BYTE
#else
 * RESERVED			448 BYTE
#endif
 * TOTAL SIZE 		512 BYTE
 *
 */
struct TCC_SB_HEADER {
	unsigned char magic[8];
	unsigned char sc_tag[4];
	unsigned char rom_type[4];
	unsigned int base_address;
	unsigned char st_type[4];
	unsigned char fwdn_type[4];
	unsigned char tcc_type[4];
	unsigned long long dram_init_start;
	unsigned long long dram_init_size;
	unsigned long long bootloader_start;
	unsigned long long bootloader_size;
	unsigned long long nand_bl1_start;
	unsigned long long nand_bl1_size;
	unsigned long long target_address;
#if defined(CONFIG_ARM_TRUSTZONE)
	unsigned long long revs;
	unsigned long long tzos_start;
	unsigned long long tzos_size;
	unsigned char rev2[400];
#else
	unsigned char rev2[424];
#endif
};

static unsigned long get_file_size(FILE* file)
{
	unsigned long f_len;
	fseek(file , 0L, SEEK_END);
	f_len = ftell(file);
	fseek(file, 0L, SEEK_SET);

	return f_len;
}

int print_ussage()
{
	return -1;
}

static int extract_dram_init(char *infile ,char *outfile)
{
	FILE *fd = fopen(infile , "r");
	FILE *wfd = fopen(outfile, "w");
	unsigned long f_len;
	unsigned char *rBuffer ,*dBuffer;
	unsigned int sInitCode , eInitCode, baseAddr, sizeInitCode;

	f_len = get_file_size(fd);

	rBuffer = malloc(f_len);
	memset(rBuffer , 0x00, f_len);

	if(!fread(rBuffer, sizeof(char), f_len, fd)){
		DBG("ROM FILE READ FAIL\n");
		return -1;
	}

	memcpy((void*)&baseAddr, &rBuffer[FILE_OFFSET_BASE_ADDR],4);
	memcpy((void*)&sInitCode, &rBuffer[FILE_OFFSET_INIT_CODE],4);
	memcpy((void*)&eInitCode, &rBuffer[FILE_OFFSET_INIT_CODE+4],4);

	sizeInitCode = eInitCode - sInitCode;

	DBG("[BASE ADDR : 0x%x]\n[INIT CODE - START : 0x%08X, END : 0x%08X, SIZE : 0x%X]\n"
			,baseAddr, sInitCode, eInitCode, sizeInitCode);

	dBuffer = &rBuffer[sInitCode - baseAddr];

	if(!fwrite(dBuffer, sizeof(char), sizeInitCode, wfd)){
		DBG("DRAM INIT CODE FILE WRITE FAIL\n");
		return -1;
	}

	free(rBuffer);

	return 0;

}

#if defined(NAND_USED)
/******************************************************************************
 *
 *	int				extract_NAND_BL1
 *
 *	Input	: infile  - lk bootloader.
 *  			  outfile - 1st Bootloader for NAND boot.
 *	Output	:
 *	Return	:
 *
 *	Description : extract 1st Bootloader for nand in lk bootloader.
 *
 *******************************************************************************/
static int extract_NAND_BL1(char *infile ,char *outfile)
{
	FILE *fd = fopen(infile , "r");
	FILE *wfd = fopen(outfile, "w");
	unsigned long f_len;
	unsigned char *rBuffer ,*dBuffer;
	unsigned int *nTempBuffer;
	unsigned int sBL1Code, sBL1Info, baseAddr, sizeInitCode;

	f_len = get_file_size(fd);

	rBuffer = malloc(f_len);
	memset(rBuffer , 0x00, f_len);

	if(!fread(rBuffer, sizeof(char), f_len, fd)){
		DBG("ROM FILE READ FAIL\n");
		return -1;
	}

	memcpy((void*)&baseAddr, &rBuffer[FILE_OFFSET_BASE_ADDR],4);
	memcpy((void*)&sBL1Code, &rBuffer[ROM_BL1_NAND_OFFSET],4);

	nTempBuffer	 = (unsigned int*) rBuffer;
	sizeInitCode = nTempBuffer[ ((sBL1Code - baseAddr)>>2) + 2 ];

	DBG("[BL1 CODE - START : 0x%08X, SIZE : 0x%X]\n"
			, sBL1Code, sizeInitCode);

	dBuffer = &rBuffer[sBL1Code - baseAddr];

	if(!fwrite(dBuffer, sizeof(char), sizeInitCode, wfd)){
		DBG("DRAM INIT CODE FILE WRITE FAIL\n");
		return -1;
	}

	free(rBuffer);

	return 0;
}
#endif

#if defined(CONFIG_ARM_TRUSTZONE)
static int mend_tzos_image(char *romfile, char * tzos)
{
	unsigned int tzos_magic[4] = {TZOS_MAGIC_M, 0, 0, 0};	//magic, base, offset, size
	struct TCC_TZOS_INFO *tzos_info;
	char* tBuf = 0;
	unsigned int * piBuf = 0;
	int i;

	unsigned int romsize = 0;
	unsigned int tzossize = 0;
	unsigned int lksize = 0;

	FILE* romfd = fopen(romfile, "r+");
	FILE* tzfd = fopen(tzos, "r+");

	romsize = get_file_size(romfd);
	tzossize = get_file_size(tzfd);
	if(romsize > tzossize)
		lksize = romsize -tzossize;
	else
	{
		DBG("[MEND TZOS] romsize(%d) < tzossize(%d) \n", romsize, tzossize);
		goto GOTO_RETURN;
	}

	tBuf = malloc(lksize);
	if(tBuf == 0)
	{
		DBG("[MEND TZOS] malloc Error \n");
		goto GOTO_RETURN;
	}
	memset(tBuf , 0x00 , lksize);
	piBuf = (unsigned int *)tBuf;

	fseek(romfd, 0L, SEEK_SET);
	if(fread(tBuf, sizeof(char), lksize, romfd) < 0)
	{
		DBG("[MEND TZOS] lk file read Error \n");
		return -1;
	}

	for(i=0; i<(lksize-16)/sizeof(int); i++)
	{
		if((piBuf[i] == TZOS_MAGIC_1)&&(piBuf[i+1] == TZOS_MAGIC_2)&&(piBuf[i+2] == TZOS_MAGIC_3)&&(piBuf[i+3] == TZOS_MAGIC_4))
			break;
	}
	if(i < (lksize-16)/sizeof(int))
	{
		//tzos_magic[0] = TZOS_MAGIC_M;
		tzos_magic[1] = TZ_SECUREOS_BASE;
		tzos_magic[2] = lksize;
		tzos_magic[3] = tzossize;

		fseek(romfd, i*sizeof(int), SEEK_SET);

		if(fwrite(&tzos_magic[0], sizeof(char), sizeof(tzos_magic), romfd) < 0)
		{
			DBG("[MEND TZOS] Write TZOS Info Error \n");
			return -1;
		}

		DBG("[MEND TZOS - BASE : 0x%08X, SIZE : 0x%X] [LK SIZE : 0x%X]\n"
				,TZ_SECUREOS_BASE, tzossize, lksize);
	}
	else
	{
		DBG("[MEND TZOS] MAGIC not found ! \n");
	}

GOTO_RETURN:
	if(tBuf != 0)
		free(tBuf);

	fclose(romfd);
	fclose(tzfd);

	return 0;
}
#endif


/******************************************************************************
 *
 *	int				make_header
 *
 *	Input	: secure_dram  - lk bootloader.
 *  			  outfile - 1st Bootloader for NAND boot.
 *	Output	:
 *	Return	:
 *
 *	Description : extract 1st Bootloader for nand in lk bootloader.
 *
 *******************************************************************************/
#if defined(NAND_USED)
static int make_header(char *romfile, char* normal_bl1, char *secure_dram ,char *secure_bootloader, char *secure_bl1, char *outfile)
#else
static int make_header(char *romfile, char *infile1 ,char *infile2, char *outfile)
#endif
{
	struct TCC_SB_HEADER *header;
	unsigned int base_address, target_address;
	FILE *romfd = fopen(romfile , "r");

#if defined(NAND_USED)
	FILE *dfd = fopen(secure_dram , "r");
	FILE *bfd = fopen(secure_bootloader, "r");
	FILE *nfd = fopen(secure_bl1 , "r");
	FILE *fp_normal_bl1 = fopen(normal_bl1, "r");
#else
	FILE *dfd = fopen(infile1 , "r");
	FILE *bfd = fopen(infile2, "r");
#endif
	FILE *hfd = fopen(outfile, "w");

	header = malloc(sizeof(struct TCC_SB_HEADER));

	memset(header , 0x0 , sizeof(struct TCC_SB_HEADER));

	fseek(romfd, FILE_OFFSET_BASE_ADDR, SEEK_SET);
	if(!fread(&base_address, sizeof(unsigned int), 1, romfd)){
		DBG("ROM FILE READ FAIL\n");
		return -1;
	}

	memcpy(header->magic, BOOT_MAGIC, sizeof(header->magic));
	memcpy(header->sc_tag, SECURE_TAG, sizeof(header->sc_tag));
	memcpy(header->rom_type, ROM_TYPE, sizeof(header->rom_type));
	header->base_address = base_address;
	memcpy(header->st_type, STORAGE_TYPE, sizeof(header->st_type));
	memcpy(header->fwdn_type, FWDN_TYPE, sizeof(header->fwdn_type));
	memcpy(header->tcc_type, TCC_TYPE, sizeof(header->tcc_type));
	header->dram_init_start = sizeof(struct TCC_SB_HEADER);
	header->dram_init_size = get_file_size(dfd);
	header->bootloader_start = header->dram_init_start + header->dram_init_size;
	header->bootloader_size = get_file_size(bfd);

#if defined(NAND_USED)
	header->nand_bl1_start = header->bootloader_start + header->bootloader_size;
	header->nand_bl1_size = get_file_size(nfd);
	fseek( fp_normal_bl1, 4, SEEK_SET );
	fread( &target_address, sizeof(unsigned int), 1, fp_normal_bl1 );
	header->target_address = target_address;
#else
	header->nand_bl1_start	= 0;
	header->nand_bl1_size	= 0;
	header->target_address	= 0;
#endif

	DBG("[MAKE HEADER] - [BaseAddress : 0x%08X] [TargetAddress : 0x%08X]\n\t[INIT CODE - START : 0x%llX, SIZE : 0x%llX]\n\t[BOOTLOADER - START : 0x%llX, SIZE : 0x%llX]\n\t[BL1 - START : 0x%llX, SIZE : 0x%llX]\n"
			, header->base_address, header->target_address, header->dram_init_start, header->dram_init_size, header->bootloader_start, header->bootloader_size, header->nand_bl1_start, header->nand_bl1_size);

	if(!fwrite(header, sizeof(char), 512, hfd)){
		DBG("Header FILE WRITE FAIL\n");
		return -1;
	}

	free(header);

#if defined(NAND_USED)
	fclose(dfd);fclose(bfd);fclose(nfd);fclose(fp_normal_bl1);
#else
	fclose(dfd); fclose(bfd);
#endif
	fclose(hfd);

	return 0;
}

main(int argc, char *argv[])
{
#if defined(NAND_USED)
	int nArgNum = 5;
#else
	int nArgNum = 4;
#endif

	if(argc < 3){
		return print_ussage();
	}

	if(argc == nArgNum){
		if(!strcmp("extract", argv[1])){
			extract_dram_init(argv[2], argv[3]);
#if defined(NAND_USED)
			extract_NAND_BL1(argv[2], argv[4]);
#endif
		}
	}

	if(argc > nArgNum){
		if(!strcmp("mkheader", argv[1])){
#if defined(NAND_USED)
			make_header(argv[2] , argv[3] , argv[4], argv[5], argv[6], argv[7]);
#else
			make_header(argv[2] , argv[3] , argv[4], argv[5]);
#endif
		}
	}

#if defined(CONFIG_ARM_TRUSTZONE)
	if(argc == 4){
		if(!strcmp("tzmend", argv[1])){
			mend_tzos_image(argv[2], argv[3]);
		}
	}
#endif

	return 0;
}
