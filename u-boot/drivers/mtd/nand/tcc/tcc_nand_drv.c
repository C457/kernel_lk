/****************************************************************************
 *   FileName    : nand_drv_v8.c
 *   Description : NAND driver for application.
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall 
constitute any express or implied warranty of any kind, including without limitation, any warranty 
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
or other third party intellectual property right. No warranty is made, express or implied, 
regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
 *
 ****************************************************************************/
#include <common.h>
#include <asm/arch/gpio.h>
#include <asm/arch/globals.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/structures_smu_pmu.h>
#include <asm/arch/fwdn/Disk.h>
#include <asm/io.h>

#include <malloc.h>
#include <nand.h>
#include <watchdog.h>
#include <linux/mtd/partitions.h>
#include "tcc_nand_drv.h"
#include "nand_def.h"
#include "nand_regs.h"
#include "nand_board_v8.h"
#include "nand_io_v8.h"
#include "nand_utils.h"

NAND_IO_DEVINFO		gNAND_IO_DeviceInfo;
TNFTL_OBJECT_T		gpTnftlObj;
#define NAND_BAD_BLOCK 1

int NAND_DRV_GetFeatureOfNAND_MTD( NAND_IO_DEVINFO *NAND_DRV_deviceInfo)
{
	int res=TRUE;

	if(NAND_IO_GetFeatureOfNAND_MTD(&gNAND_IO_DeviceInfo, &gpTnftlObj) != TRUE)
	{
		ND_ERROR("[TCC NAND] Failed to get feature of nand\n");
		res = FALSE;
	}
	memcpy( NAND_DRV_deviceInfo, &gNAND_IO_DeviceInfo, sizeof( gNAND_IO_DeviceInfo ) );
	gpTnftlObj.uiCurrentDieIndex = 0;
	return res;
}

#ifdef CONFIG_SYS_NAND_READY_PIN
int tcc_nand_ready( struct mtd_info *mtd )
{
	//ND_TRACE(KERN_INFO "[test] tcc_nand_ready\n");
	return NAND_IO_IsReady();
}
#endif

int tcc_nand_isbad(struct mtd_info *mtd, loff_t ofs)
{
	unsigned int 		 	page = ofs >> (ffs(mtd->writesize) - 1);
	unsigned int uiBlockAddress;
	int res=0;

	uiBlockAddress = page >> gNAND_IO_DeviceInfo.ShiftPpB;
	res = NAND_IO_IsBad( &gpTnftlObj, uiBlockAddress );
	if (res != 0)
	{
		ND_ERROR("[TCC NAND] Bad block : page = %d (%d block)\n ", page, uiBlockAddress);
		res = NAND_BAD_BLOCK;
	}
	return res;
}

int tcc_nand_markbad(struct mtd_info *mtd, loff_t ofs)
{
	unsigned int 		 	page = ofs >> (ffs(mtd->writesize) - 1);
	unsigned int uiBlockAddress;

	uiBlockAddress = page >> gNAND_IO_DeviceInfo.ShiftPpB;
	ND_TRACE("[TCC NAND] Mark Bad : page : %d (%d block)\n ", page, uiBlockAddress);
	NAND_IO_MarkBad( &gpTnftlObj, uiBlockAddress );

	return 0;
}
void tcc_nand_select_chip(struct mtd_info *mtd, int chip)
{
	//ND_TRACE("[TCC NAND] nand select chip (%d)\n", chip);
	NAND_IO_EnableChipSelect( chip );

}

int tcc_mtd_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	unsigned long long writesize_shift;
	unsigned long long addr;
	unsigned int page;
	unsigned int blockaddr;
	loff_t len;
	int ret=0;

	//ND_TRACE("\n[TCC NAND] %s: addr=0x%012llx, len=%llu\n", __func__, instr->addr, instr->len);

	writesize_shift = ffs(mtd->writesize)-1;
	addr = (unsigned long long)instr->addr;
	len = instr->len;
	page = ((unsigned long long)addr) >> writesize_shift;
	blockaddr = page >> gNAND_IO_DeviceInfo.ShiftPpB;
	instr->state = MTD_ERASING;

	while (len) {		
		if(NAND_IO_EraseBlock( &gpTnftlObj, blockaddr) != TRUE)
		{
			ND_ERROR("[TCC NAND] Erase block failed!!\n");
			ret = -1;
		}

		if (ret != SUCCESS) {
			instr->state = MTD_ERASE_FAILED;
			instr->fail_addr =
				((loff_t) page << writesize_shift);
			ND_ERROR("[TCC NAND] %s: can't erase block @ %d [PageAddr:%d]\n", __func__,
			       blockaddr , page);

			goto erase_exit;
		}
		len -= mtd->erasesize;
		page += 1 << (gNAND_IO_DeviceInfo.ShiftPpB - writesize_shift);
	}
	instr->state = MTD_ERASE_DONE;
	instr->fail_addr = 0xffffffff;

erase_exit:

	ret = instr->state == MTD_ERASE_DONE ? 0 : -EIO;

	/* Do call back function */
	if (!ret)
		mtd_erase_callback(instr);

	return ret;
}

int tcc_nand_erase_block(struct mtd_info *mtd, int page)
{
	unsigned int ret;
	unsigned int uiBlockAddress;

	uiBlockAddress = page >> gNAND_IO_DeviceInfo.ShiftPpB;
	//ND_TRACE(KERN_INFO "[TCC NAND] tcc_nand_erase_block (%d blk - %d page)\n", uiBlockAddress, page);
	ret = NAND_IO_EraseBlock( &gpTnftlObj, uiBlockAddress );
	if (ret != TRUE)
		ND_ERROR("[TCC NAND] Erase block failed!! (%d blk - %d page)\n", uiBlockAddress, page);
	
	return ret;
}

int tcc_nand_read_page(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int oob_required, int page)
{
	//read_page(mtd, nand, buf, page, 1);
	uint8_t *oobbuf = chip->buffers->ecccode;
	//unsigned int tmp_buf[8192];
	//unsigned char *tmp_p=(unsigned char *)tmp_buf;
	int res = -1;
	//ND_TRACE(KERN_INFO "[TCC NAND] tcc_nand_read_page (page addr=%d)\n", page);
	
	memset(buf, 0xFF, (unsigned char)gNAND_IO_DeviceInfo.Feature.PageSize);
	memset(oobbuf, 0xFF, (unsigned char)gNAND_IO_DeviceInfo.Feature.SpareSize);

	if(NAND_IO_ReadPage( &gpTnftlObj, page, (unsigned int*)buf, (unsigned int*)oobbuf, TNFTL_TAG_BYTE_SIZE, FALSE) == TRUE)
	//if(NAND_IO_ReadPage( &gpTnftlObj, page, tmp_buf, (unsigned int*)oobbuf, MTD_TAG_BYTE_SIZE, FALSE) == TRUE)
	{
		#if 0
		unsigned int i;
// print data & spare

		ND_TRACE("\nData : \n");

		for (i=0; i<gNAND_IO_DeviceInfo.Feature.PageSize; i++)
		{
			if((i%16)==0)
				printf("\n");			
			_ND_TRACE("%02X ", buf[i]);		
		}
		_ND_TRACE("\n");


		ND_TRACE("\nSpare : \n");
		for (i=0; i<gNAND_IO_DeviceInfo.Feature.SpareSize; i++)
			_ND_TRACE("%02X", oobbuf[i]);
		_ND_TRACE("\n");
		#endif
		res = 0;

	}
	else
	{
		unsigned int i;
		ND_ERROR(KERN_INFO "[TCC NAND] tcc_nand_read_page failed!! (page addr=%d)\n", page);
		for (i=0; i<gNAND_IO_DeviceInfo.Feature.PageSize; i++)
			_ND_TRACE("%02X", buf[i]);
		_ND_TRACE("\n");
	}

	return res;
}

int tcc_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offset, int data_len, const uint8_t *buf, 
			int oob_required, int page,	int cached, int raw)
{
	int res=-1;
	uint8_t *oobbuf = chip->buffers->ecccode;
	
	cached = 0;
	//ND_TRACE(KERN_INFO "[TCC NAND] tcc_nand_write_page!!(p-%d)\n", page);

	if(NAND_IO_WritePage( &gpTnftlObj, page, (unsigned int*)buf, (unsigned int*)oobbuf, TNFTL_TAG_BYTE_SIZE, FALSE) == TRUE)
	{
		#if 0
		unsigned int i;
		/*
		for (i=0; i<gNAND_IO_DeviceInfo.Feature.PageSize; i++)
			_ND_TRACE("%02X", buf[i]);
		_ND_TRACE("\n");
		*/
		for (i=0; i<gNAND_IO_DeviceInfo.Feature.SpareSize; i++)
			_ND_TRACE("%02X", oobbuf[i]);
		_ND_TRACE("\n");
		#endif
		res = 0;
	}
	else
	{
		#if 1
		unsigned int i;
		for (i=0; i<gNAND_IO_DeviceInfo.Feature.PageSize; i++)
			_ND_TRACE("%02X", buf[i]);
		_ND_TRACE("\n");
		#endif
		ND_TRACE("[TCC NAND] tcc_nand_write_page failed!! (page addr=%d)\n", page);
	}	

	return res;	
}

int tcc_nand_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{	
	//uint8_t *oobbuf = chip->pagebuf;
	unsigned int oobbuf[1024];
	int res=-1;
	ND_TRACE("[TCC NAND] tcc_nand_read_oob!!\n");
	if (NAND_IO_ReadSpareArea_MTD( &gNAND_IO_DeviceInfo, page, oobbuf, TNFTL_TAG_BYTE_SIZE ) == 0)
		res = 0;
	
	return res;
	
}

int tcc_mtd_read_oob (struct mtd_info *mtd, loff_t from, struct mtd_oob_ops *ops)
{	
	unsigned page;
	unsigned int writesize_shift=0;
	uint8_t databuf[16384];
	uint8_t oobbuf[512];
	uint8_t *bufpoi, *oob, *buf;
	int res = -1;
	
	ND_TRACE("[TCC NAND] tcc_mtd_read_oob!!\n");
	
	writesize_shift = ffs(mtd->writesize) - 1;
	page = from >> writesize_shift;

	buf = ops->datbuf;
	oob = ops->oobbuf;
	//ND_TRACE(KERN_INFO "[TCC NAND] tcc_mtd_read_oob(page=%d)!!\n", page);
	
	if(buf)
	{		
		bufpoi = buf;
		
		if(NAND_IO_ReadPage( &gpTnftlObj, page, (unsigned int*)bufpoi, (unsigned int*)oob/*oobbuf*/ , TNFTL_TAG_BYTE_SIZE, FALSE) == TRUE)
		{
		res = 0;
		}
		else
		{
			ND_ERROR("[TCC NAND] Read OOB failed(page : %d)\n", page);
		}

		if (oob) {
			memcpy(oob, oobbuf, ops->ooblen);
		}
	}
	else
	{		
		if(NAND_IO_ReadPage( &gpTnftlObj, page, (unsigned int *)(databuf), (unsigned int *)(oobbuf), TNFTL_TAG_BYTE_SIZE, FALSE) == TRUE)
		{
/*	
			unsigned int tmp_i;
			ND_TRACE("OOB : ");
			for(tmp_i=0; tmp_i<TNFTL_TAG_BYTE_SIZE; tmp_i++)
				_ND_TRACE("%02X ", oobbuf[tmp_i]);
			_ND_TRACE("\n");
			*/
		res = 0;
	}
	else
	{
		ND_ERROR("[TCC NAND] Read OOB failed(page : %d)\n", page);
	}

		if (oob) {
			//unsigned int tmp_i;
			memcpy(oob, oobbuf, ops->ooblen);
			/*
			ND_TRACE("OOB_memcpy : ");
			for(tmp_i=0; tmp_i<TNFTL_TAG_BYTE_SIZE; tmp_i++)
				_ND_TRACE("%02X ", oob[tmp_i]);
			_ND_TRACE("\n");	
			*/
		}	
		//ND_TRACE("read success!!\n");
		ops->oobretlen = ops->ooblen;
	}	
	
	return res;
	
}

int tcc_mtd_write_oob(struct mtd_info *mtd, loff_t to, struct mtd_oob_ops *ops)
{
	//struct tcc_nand_chip *chip = mtd->priv;
	struct nand_chip *chip = mtd->priv;
	unsigned page;
	unsigned int chipnr=0, uiRealPageAddr=0;
	uint8_t *oob;
	uint8_t *buf;
	int ret = 0;
	int bytes = mtd->writesize;
	int write_shift = (ffs(mtd->writesize) - 1);
	page = to >> write_shift;

	ND_DEBUG("[TCC NAND] %s: to=0x%llx, datbuf=%p, len=%d, oobbuf=%p, mode=%d\n",
		 __func__, to, ops->datbuf, ops->len, ops->oobbuf, ops->mode);
			
	if (ops->datbuf) {
		uint32_t writelen = ops->len;
		ops->retlen = 0;
		if (!writelen)
			goto write_exit;

		/* Invalidate the page cache, when we write to the cached page */
//		if (to <= (chip->cached_page << write_shift) &&
//		    (chip->cached_page << write_shift) < (to + ops->len))
//			chip->cached_page = -1;

		if( writelen < mtd->writesize )
		{
			memset( chip->buffers->databuf, 0xFF, mtd->writesize );
			memcpy( chip->buffers->databuf, ops->datbuf, writelen );

			oob = ops->oobbuf;
			buf = &chip->buffers->databuf[0];						
		}
		else
		{
		oob = ops->oobbuf;
		buf = ops->datbuf;
		}	

		while (1) {
			if(NAND_IO_WritePage( &gpTnftlObj, page, (unsigned int*)buf, (unsigned int*)oob, TNFTL_TAG_BYTE_SIZE, FALSE) == TRUE)
			{
				ret = SUCCESS;
			}
			else
			{
				ret = -1;
			}

			if (ret != SUCCESS) {
				ND_ERROR("%s : MTD Write Error. [ChipNo:%d] [PageAddr:%d] [res:0x%08X]\n", 
						__func__, chipnr, uiRealPageAddr, ret );
				ret = -EIO;
				goto write_exit;
			}
			
			if( writelen < mtd->writesize )
				writelen -= writelen;
			else
			writelen -= bytes;

			ND_DEBUG("writelen = %d, bytes = %d\n", writelen, bytes);
			if (!writelen)
				break;

			buf += bytes;
			page++;

			if( writelen < mtd->writesize )
			{
				memset( chip->buffers->databuf, 0xFF, mtd->writesize );
				memcpy( chip->buffers->databuf, buf, writelen );

				buf = &chip->buffers->databuf[0];			
			}
		}
		ops->retlen = ops->len - writelen;
		if (unlikely(oob))
			ops->oobretlen = ops->ooblen;
	} else {
		// TODO: write out-of-band
		ops->oobretlen = ops->ooblen;
	}

write_exit:
	return ret;
}

uint8_t tcc_nand_read_byte(struct mtd_info *mtd)
{
	unsigned char ucReadByte;
	unsigned int uiColaddr=0;
	NAND_IO_ReadUserSizeData( &gNAND_IO_DeviceInfo, uiColaddr, 1, &ucReadByte );	
	//ND_TRACE("%02X\n", ucReadByte);
	return (ucReadByte| NAND_STATUS_WP);
}

u16 tcc_nand_read_word(struct mtd_info *mtd)
{
	unsigned int uiReadWord;
	unsigned int uiColaddr=0;
	
	NAND_IO_ReadUserSizeData( &gNAND_IO_DeviceInfo, uiColaddr, 4, (unsigned char*)&uiReadWord);	
	ND_DEBUG("%04X\n", uiReadWord);
	return uiReadWord;
}

void tcc_nand_getdevinfo(ioctl_diskphysical_t *ioctl_physical_info)
{
	ioctl_physical_info->uiDieCount				= gpTnftlObj.uiDieCount;
	ioctl_physical_info->uiBlockNumberOfDevice	= gpTnftlObj.uiBlocksPerDie;
	ioctl_physical_info->uiPageNumberOfBlock	= gpTnftlObj.uiPagesPerBlock;
	ioctl_physical_info->uiShiftPpB				= gpTnftlObj.uiPagesPerBlockShift;
	ioctl_physical_info->uiPageSize				= gNAND_IO_DeviceInfo.Feature.PageSize;
	ioctl_physical_info->uiSpareSize			= gNAND_IO_DeviceInfo.Feature.SpareSize;
}

void tcc_print_info()
{
	ND_TRACE("ChipNo = %d\n", gNAND_IO_DeviceInfo.ChipNo);
	ND_TRACE("CmdMask = %d\n", gNAND_IO_DeviceInfo.CmdMask);
	ND_TRACE("CodewordSize = %d\n", gNAND_IO_DeviceInfo.CodewordSize);
	ND_TRACE("EccCodeSize = %d\n", gNAND_IO_DeviceInfo.EccCodeSize);
	ND_TRACE("EccCodeSizeTotalInDword = %d\n", gNAND_IO_DeviceInfo.EccCodeSizeTotalInDword);
	ND_TRACE("EccType = %d\n", gNAND_IO_DeviceInfo.EccType);
	ND_TRACE("ExtInterleaveUsable = %d\n", gNAND_IO_DeviceInfo.ExtInterleaveUsable);
	ND_TRACE("fInterleaveUsable = %d\n", gNAND_IO_DeviceInfo.fInterleaveUsable);
	ND_TRACE("fReadCommandSent = %d\n", gNAND_IO_DeviceInfo.fReadCommandSent);
	ND_TRACE("fReadRetryNow = %d\n", gNAND_IO_DeviceInfo.fReadRetryNow);
	ND_TRACE("fSkipDataAreaRandomizing = %d\n", gNAND_IO_DeviceInfo.fSkipDataAreaRandomizing);
	ND_TRACE("IoStatus = %d\n", gNAND_IO_DeviceInfo.IoStatus);
	ND_TRACE("ShiftBytesPerSector = %d\n", gNAND_IO_DeviceInfo.ShiftBytesPerSector);
	ND_TRACE("ShiftPpB = %d\n", gNAND_IO_DeviceInfo.ShiftPpB);
	ND_TRACE("ucDieIndex = %d\n", gNAND_IO_DeviceInfo.ucDieIndex);
	ND_TRACE("ucInterleavingStatus = %d\n", gNAND_IO_DeviceInfo.ucInterleavingStatus);
	ND_TRACE("uiInterleavingPageAddr = %d(1)\n", gNAND_IO_DeviceInfo.uiInterleavingPageAddr[0]);
	ND_TRACE("uiInterleavingPageAddr = %d(2)\n", gNAND_IO_DeviceInfo.uiInterleavingPageAddr[1]);	
	ND_TRACE("usCodewordsPerPage = %d\n", gNAND_IO_DeviceInfo.usCodewordsPerPage);
	ND_TRACE("usShiftCodewordsPerPage = %d\n", gNAND_IO_DeviceInfo.usShiftCodewordsPerPage);
	
	ND_TRACE("[Feature]BBpD = %d\n", gNAND_IO_DeviceInfo.Feature.BBpD);
	ND_TRACE("[Feature]BpD = %d\n", gNAND_IO_DeviceInfo.Feature.BpD);
	ND_TRACE("[Feature]DeviceName = %s\n", *gNAND_IO_DeviceInfo.Feature.DeviceName);
	ND_TRACE("[Feature]MediaType = %lu\n", gNAND_IO_DeviceInfo.Feature.MediaType);
	ND_TRACE("[Feature]PageSize = %d\n", gNAND_IO_DeviceInfo.Feature.PageSize);
	ND_TRACE("[Feature]PpB = %d\n", gNAND_IO_DeviceInfo.Feature.PpB);
	ND_TRACE("[Feature]pucLSBPageTable = %s\n", gNAND_IO_DeviceInfo.Feature.pucLSBPageTable);
	ND_TRACE("[Feature]SpareSize = %d\n", gNAND_IO_DeviceInfo.Feature.SpareSize);
	ND_TRACE("[Feature]ucColCycle = %d\n", gNAND_IO_DeviceInfo.Feature.ucColCycle);
	ND_TRACE("[Feature]ucDDPBit = %d\n", gNAND_IO_DeviceInfo.Feature.ucDDPBit);
	ND_TRACE("[Feature]ucReadAccessTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucReadAccessTime);
	ND_TRACE("[Feature]ucReadCycleTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucReadCycleTime);
	ND_TRACE("[Feature]ucReadHoldTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucReadHoldTime);
	ND_TRACE("[Feature]ucReadPulseWidthTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucReadPulseWidthTime);
	ND_TRACE("[Feature]ucRowCycle = %d\n", gNAND_IO_DeviceInfo.Feature.ucRowCycle);
	ND_TRACE("[Feature]ucWriteCycleTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucWriteCycleTime);
	ND_TRACE("[Feature]ucWriteHoldTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucWriteHoldTime);
	ND_TRACE("[Feature]ucWritePulseWidthTime = %d\n", gNAND_IO_DeviceInfo.Feature.ucWritePulseWidthTime);
	ND_TRACE("[Feature]UsableFunc = %lu\n", gNAND_IO_DeviceInfo.Feature.UsableFunc);

	ND_TRACE("[gpTnftlObj]errLastError = 0x%x\n", gpTnftlObj.errLastError);
	ND_TRACE("[gpTnftlObj]fGoldenInfoReseted = %d\n", gpTnftlObj.fGoldenInfoReseted);
	ND_TRACE("[gpTnftlObj]fReadRetry = %d\n", gpTnftlObj.fReadRetry);
	ND_TRACE("[gpTnftlObj]fRWBlockCollected = %d\n", gpTnftlObj.fRWBlockCollected);
	ND_TRACE("[gpTnftlObj]fRWTempPageBufferLock = %d\n", gpTnftlObj.fRWTempPageBufferLock);
	ND_TRACE("[gpTnftlObj]fUseHotBlock = %d\n", gpTnftlObj.fUseHotBlock);
	ND_TRACE("[gpTnftlObj]fUseLowerPageOnlyForBL1 = %d\n", gpTnftlObj.fUseLowerPageOnlyForBL1);
	ND_TRACE("[gpTnftlObj]pucMLC_LowerPageTable = %s\n", gpTnftlObj.pucMLC_LowerPageTable);
	ND_TRACE("[gpTnftlObj]ucColumnCycle = %d\n", gpTnftlObj.ucColumnCycle);
	ND_TRACE("[gpTnftlObj]ucPlaneCount = %d\n", gpTnftlObj.ucPlaneCount);
	ND_TRACE("[gpTnftlObj]ucPlaneCountShift = %d\n", gpTnftlObj.ucPlaneCountShift);
	ND_TRACE("[gpTnftlObj]ucRowCycle = %d\n", gpTnftlObj.ucRowCycle);
	ND_TRACE("[gpTnftlObj]uiBadBlockCountMax = %d\n", gpTnftlObj.uiBadBlockCountMax);
	ND_TRACE("[gpTnftlObj]uiBFCntMax = %d\n", gpTnftlObj.uiBFCntMax);
	ND_TRACE("[gpTnftlObj]uiBFCntMax = %d\n", gpTnftlObj.uiBL1_MediaType);
	ND_TRACE("[gpTnftlObj]uiBlockCountExceptRWArea = %d\n", gpTnftlObj.uiBlockCountExceptRWArea);
	ND_TRACE("[gpTnftlObj]uiBlocksPerDie = %d\n", gpTnftlObj.uiBlocksPerDie);
	ND_TRACE("[gpTnftlObj]uiBytesPerPage = %d\n", gpTnftlObj.uiBytesPerPage);
	ND_TRACE("[gpTnftlObj]uiBytesPerPageShift = %d\n", gpTnftlObj.uiBytesPerPageShift);
	ND_TRACE("[gpTnftlObj]uiCMTCntMax = %d\n", gpTnftlObj.uiCMTCntMax);
	ND_TRACE("[gpTnftlObj]uiCodeWordSize = %d\n", gpTnftlObj.uiCodeWordSize);
	ND_TRACE("[gpTnftlObj]uiCommCycleRegValue = %d\n", gpTnftlObj.uiCommCycleRegValue);
	ND_TRACE("[gpTnftlObj]uiCurrentDebuginfoBlockAddress = %d\n", gpTnftlObj.uiCurrentDebuginfoBlockAddress);
	ND_TRACE("[gpTnftlObj]uiCurrentDieIndex = %d\n", gpTnftlObj.uiCurrentDieIndex);
	ND_TRACE("[gpTnftlObj]uiCurrentPageBufferIndex = %d\n", gpTnftlObj.uiCurrentPageBufferIndex);
	ND_TRACE("[gpTnftlObj]uiDebuginfoBlockCount = %d\n", gpTnftlObj.uiDebuginfoBlockCount);
	ND_TRACE("[gpTnftlObj]uiDieCount = %d\n", gpTnftlObj.uiDieCount);
	ND_TRACE("[gpTnftlObj]uiDieCountShift = %d\n", gpTnftlObj.uiDieCountShift);
	ND_TRACE("[gpTnftlObj]uiECBCntMax = %d\n", gpTnftlObj.uiECBCntMax);
	ND_TRACE("[gpTnftlObj]uiECDCntMax = %d\n", gpTnftlObj.uiECDCntMax);
	ND_TRACE("[gpTnftlObj]uiEntryCountMax = %d\n", gpTnftlObj.uiEntryCountMax);
	ND_TRACE("[gpTnftlObj]uiLastAllocatedDebugBlock = %d\n", gpTnftlObj.uiLastAllocatedDebugBlock);
	ND_TRACE("[gpTnftlObj]uiLast_convert_version = %d\n", gpTnftlObj.uiLast_convert_version);
	ND_TRACE("[gpTnftlObj]uiMBCntMax = %d\n", gpTnftlObj.uiMBCntMax);
	ND_TRACE("[gpTnftlObj]uiPagesPerBlock = %d\n", gpTnftlObj.uiPagesPerBlock);
	ND_TRACE("[gpTnftlObj]uiPagesPerBlockShift = %d\n", gpTnftlObj.uiPagesPerBlockShift);
	ND_TRACE("[gpTnftlObj]uiPageStatus_SizeInByte = %d\n", gpTnftlObj.uiPageStatus_SizeInByte);
	ND_TRACE("[gpTnftlObj]uiPage_append_index = %d\n", gpTnftlObj.uiPage_append_index);
	ND_TRACE("[gpTnftlObj]uiPMDCntMax = %d\n", gpTnftlObj.uiPMDCntMax);
	ND_TRACE("[gpTnftlObj]uiReadCycleRegValue = %d\n", gpTnftlObj.uiReadCycleRegValue);
	ND_TRACE("[gpTnftlObj]uiRelevantCS = %d\n", gpTnftlObj.uiRelevantCS);
	ND_TRACE("[gpTnftlObj]uiRWAreaBaseBlockAddress = %d\n", gpTnftlObj.uiRWAreaBaseBlockAddress);
	ND_TRACE("[gpTnftlObj]uiRWAreaReservedBlockCount = %d\n", gpTnftlObj.uiRWAreaReservedBlockCount);
	ND_TRACE("[gpTnftlObj]uiRWTempPageBufferPageAddress = %d\n", gpTnftlObj.uiRWTempPageBufferPageAddress);
	ND_TRACE("[gpTnftlObj]uiSafePagesPerBlock = %d\n", gpTnftlObj.uiSafePagesPerBlock);
	ND_TRACE("[gpTnftlObj]uiSectorsPerPage = %d\n", gpTnftlObj.uiSectorsPerPage);
	ND_TRACE("[gpTnftlObj]uiSectorsPerPageShift = %d\n", gpTnftlObj.uiSectorsPerPageShift);
	ND_TRACE("[gpTnftlObj]uiTotalDataBlockCount = %d\n", gpTnftlObj.uiTotalDataBlockCount);
	ND_TRACE("[gpTnftlObj]usBL1_ECCType = %d\n", gpTnftlObj.usBL1_ECCType);
	ND_TRACE("[gpTnftlObj]usECCCodeSize = %d\n", gpTnftlObj.usECCCodeSize);

}
