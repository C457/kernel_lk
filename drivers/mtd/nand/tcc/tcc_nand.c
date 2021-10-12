#include <common.h>
#include <asm/arch/gpio.h>
#include <asm/arch/globals.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/structures_smu_pmu.h>
#include <asm/io.h>
#include <asm/arch/fwdn/Disk.h>

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

//#define NFC_VER_300
//#define TCC8930
//#define TCC8930_88M_BOARD

#define CONFIG_NAND_BASE 0x76600010
#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST { CONFIG_NAND_BASE }
#endif

struct tcc_nand_chip {
	unsigned int offset;	/* start of MTD block */
	unsigned int end;	/* end of MTD block */
	unsigned int uipartnum;
	unsigned int cached_page;

	struct {
		uint8_t databuf[16384];
		uint8_t oobbuf[512];
	} buffers;
};

extern int edi_init(unsigned int mode);

static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
static ulong base_addr[CONFIG_SYS_MAX_NAND_DEVICE] = CONFIG_SYS_NAND_BASE_LIST;

extern NAND_IO_DEVINFO		gNAND_IO_DeviceInfo;
extern TNFTL_OBJECT_T		gpTnftlObj;


static PGPIO s_pNAND_BD_GPIO;

struct mtd_partition tcc_partition_info[] = {
    /* kernel */
    {
        .name       = "kernel",
        .offset     = MTDPART_OFS_APPEND,
        .size       = 20 * 1024 * 1024,
        .mask_flags = MTD_WRITEABLE,
    },
    /* Kernel backup*/
    {
        .name       = "kernel_backup",
        .offset     = MTDPART_OFS_APPEND,
        .size       = 20 * 1024 * 1024,
        .mask_flags = MTD_WRITEABLE,
    },
    
    {
        .name       = "ubifs_system",
        .offset     = MTDPART_OFS_APPEND,
        .size       = 450 * 1024 * 1024,
        .mask_flags = 0,
    },
		
    {
        .name       = "misc",
        .offset     = MTDPART_OFS_APPEND,
        .size       = 1024 * 1024,
        .mask_flags = 0,
    },
    
    {
        .name       = "cache",
        .offset     = MTDPART_OFS_APPEND,
        .size       = 1024 * 1024,
        .mask_flags = 0,
    }

};


void tcc_nand_command(struct mtd_info *mtd, unsigned int command,
	int column, int page_addr)
{
	struct nand_chip *chip = mtd->priv;
	//struct nand_drv *info;
	NAND_IO_DEVID nDeviceCode;
	unsigned int i;

	/*
	 * Write out the command to the device.
	 *
	 * Only command NAND_CMD_RESET or NAND_CMD_READID will come
	 * here before mtd->writesize is initialized.
	 */

	/* Emulate NAND_CMD_READOOB */
	
	if (command == NAND_CMD_READOOB) {
		assert(mtd->writesize != 0);
		column += mtd->writesize;
		command = NAND_CMD_READ0;
	}

	/* Adjust columns for 16 bit bus-width */
	if (column != -1 && (chip->options & NAND_BUSWIDTH_16))
		column >>= 1;
	//printk(KERN_INFO "[TCC NAND] CMD = %d, column = %d, page_addr = 0x%x\n", command, column, page_addr);
	switch (command) {
	case NAND_CMD_READID:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_READID!\n");
		NAND_IO_ReadID(0, &nDeviceCode, 0 );
		for (i=0; i<6; i++)
			ND_DEBUG("0x%x ", nDeviceCode.Code[i]);
		ND_DEBUG("\n");
		break;
	case NAND_CMD_PARAM:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_PARAM!\n");
		break;
	case NAND_CMD_READ0:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_READ0!\n");		
		break;
	case NAND_CMD_SEQIN:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_SEQIN!\n");
		break;
	case NAND_CMD_PAGEPROG:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_PAGEPROG!\n");
		return;
	case NAND_CMD_ERASE1:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_ERASE1!\n");
		break;
	case NAND_CMD_ERASE2:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_ERASE2!\n");
		return;
	case NAND_CMD_STATUS:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_STATUS!\n");
		break;
	case NAND_CMD_RESET:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_RESET!\n");
		NAND_IO_Reset( 0, 0);
		break;
	case NAND_CMD_RNDOUT:
		ND_DEBUG("[TCC NAND] CMD : NAND_CMD_RNDOUT!\n");
		break;
	default:
		ND_DEBUG("[TCC NAND] %s: Unsupported command 0x%x\n", __func__, command);
		break;
	}

}
/*
static int tcc_mtd_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, uint8_t *buf)
{
	struct mtd_oob_ops ops;
	int ret;

	//nand_get_device(chip, mtd, FL_READING);
	ops.len = len;
	ops.datbuf = buf;
	ops.oobbuf = NULL;
	ops.mode = MTD_OPS_PLACE_OOB;
	ret = tcc_mtd_read_oob(mtd, from, &ops);
	*retlen = ops.retlen;
	//nand_release_device(mtd);
	return ret;
}
*/
/*
static int tcc_mtd_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	struct mtd_oob_ops ops;
	int ret;
	int oob_buff[20];

	printk(KERN_INFO "[TCC NAND] tcc_mtd_write!!\n");
	//nand_get_device(chip, mtd, FL_WRITING);
	ops.len = len;
	ops.datbuf = (uint8_t *) buf;
	ops.oobbuf = (uint8_t *) oob_buff;//NULL;
	ops.mode = MTD_OPS_PLACE_OOB;

	ret =  tcc_mtd_write_oob(mtd, to, &ops);
	*retlen = ops.retlen;
	//nand_release_device(mtd);
	return ret;
}
*/

int tcc_nand_chip_init(struct mtd_info *mtd, struct nand_chip *nand, int devnum, ulong base_addr)
{
	unsigned int uiTotalSize=0;
	int ret;

	ND_DEBUG("[TCC NAND] tcc_nand_chip_init - devnum=%d, base_addr=%lu\n", devnum, base_addr);
	mtd->priv = nand;
	nand->IO_ADDR_R = nand->IO_ADDR_W = (void  __iomem *)base_addr;

#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
	nand->bbt_options	  |= NAND_BBT_USE_FLASH;
#endif
	nand->cmdfunc = tcc_nand_command;
#ifdef CONFIG_SYS_NAND_READY_PIN
	nand->dev_ready = tcc_nand_ready;
#endif
	nand->chip_delay = 0;//20;

	uiTotalSize = ((gNAND_IO_DeviceInfo.Feature.PageSize >> 10) * gNAND_IO_DeviceInfo.Feature.PpB * gNAND_IO_DeviceInfo.Feature.BpD);

	mtd->type = MTD_NANDFLASH;
	mtd->flags = MTD_CAP_NANDFLASH;
	mtd->size = (uint64_t)uiTotalSize << 10;//0x40000000;
	mtd->name = "TCC_MTD\0";
	mtd->writesize = gNAND_IO_DeviceInfo.Feature.PageSize;
	mtd->erasesize = (gNAND_IO_DeviceInfo.Feature.PpB * gNAND_IO_DeviceInfo.Feature.PageSize);
	mtd->oobsize = gNAND_IO_DeviceInfo.Feature.SpareSize;
	ND_DEBUG("[TCC NAND] total = %llu, writesize = %d, erasesize = %d, oobsize = %d, codeword=%d ECC size=%d, ECC total=%d\n", 
		mtd->size, mtd->writesize, mtd->erasesize, mtd->oobsize, gNAND_IO_DeviceInfo.CodewordSize, gNAND_IO_DeviceInfo.EccCodeSize, gNAND_IO_DeviceInfo.EccCodeSizeTotalInDword);

	nand->bbt_erase_shift = ffs(mtd->erasesize);
	nand->phys_erase_shift = ffs(mtd->erasesize);
	nand->ecc.mode = NAND_ECC_NONE;//NAND_ECC_HW_SYNDROME;
	nand->ecc.size = gNAND_IO_DeviceInfo.EccCodeSize;
	nand->ecc.bytes	= 8;
	nand->chipsize = mtd->size;
	nand->page_shift = ffs((int)mtd->writesize) - 1;
	nand->pagemask = (nand->chipsize >> nand->page_shift) - 1;
	if (nand->chipsize & 0xffffffff)
		nand->chip_shift = ffs((unsigned)nand->chipsize) - 1;
	else {
		nand->chip_shift = ffs((unsigned)(nand->chipsize >> 32));
		nand->chip_shift += 32 - 1;
	}
	ND_DEBUG("chipsize = %llu, page_shift = %d, pagemask = 0x%x\n", nand->chipsize, nand->page_shift, nand->pagemask);

	/* methods */

	nand->select_chip = tcc_nand_select_chip;
	nand->scan_bbt = nand_default_bbt;

	ret = nand_scan_tail(mtd);
	
	if (!ret)
	{
		printk("[TCC NAND] nand_register!!\n");
		nand_register(devnum);
		//add_mtd_device(mtd);
	}
	else
	{
		ND_TRACE(KERN_INFO "[TCC NAND] nand_scan_tail : ret= %d\n", ret);
	}

	ND_DEBUG("PageSize = %d, PpB = %d, BpD = %d, Total size=%dKB\n", gNAND_IO_DeviceInfo.Feature.PageSize, 
		gNAND_IO_DeviceInfo.Feature.PpB, gNAND_IO_DeviceInfo.Feature.BpD, uiTotalSize );

	nand->block_markbad = tcc_nand_markbad;
	nand->erase = tcc_nand_erase_block;
	nand->ecc.read_page = tcc_nand_read_page;
	nand->ecc.read_oob = tcc_nand_read_oob;
	nand->write_page = tcc_nand_write_page;
	nand->read_byte = tcc_nand_read_byte;
	nand->read_word= tcc_nand_read_word;

	mtd->_erase = tcc_mtd_erase;
	//mtd->_read = tcc_mtd_read;
	//mtd->_write = tcc_mtd_write;
	mtd->_read_oob = tcc_mtd_read_oob;
	mtd->_write_oob = tcc_mtd_write_oob;
	mtd->_block_isbad = tcc_nand_isbad;
	mtd->_block_markbad= tcc_nand_markbad;

	gpTnftlObj.uiCurrentDieIndex = 0;

	/* Register the partitions */
	//add_mtd_partitions(mtd, tcc_partition_info, ARRAY_SIZE(tcc_partition_info));
	return ret;
}

void nand_board_set ( void )
{

	ND_DEBUG("[TCC NAND] Set NAND Pin\n");
	//RDY
	BITCSET(s_pNAND_BD_GPIO->GPAFN2.nREG, 0x0000000F, 0x00000001);

	//WE OE ALE CLE
	BITCSET(s_pNAND_BD_GPIO->GPAFN1.nREG, 0x0000FFFF, 0x00001111);

	//WP
	BITCLR(s_pNAND_BD_GPIO->GPFFN3.nREG,0xF0000000);
	BITSET(s_pNAND_BD_GPIO->GPFEN.nREG,Hw31);

	//Power Pin
	BITSET( s_pNAND_BD_GPIO->GPGPE.nREG, Hw19);
	BITSET( s_pNAND_BD_GPIO->GPGPS.nREG, Hw19);
	BITCLR( s_pNAND_BD_GPIO->GPGEN.nREG, Hw19);    // Enable Register Setting.

	// Maximize the IO strength
	BITSET(s_pNAND_BD_GPIO->GPACD0.nREG, 0xFFFFFFFF);
	BITSET(s_pNAND_BD_GPIO->GPACD1.nREG, 0x00000003);
	
}

void tcc_nand_init(void)
{
	struct mtd_info *mtd = &nand_info[0];
	struct nand_chip *nand = (struct nand_chip *)nand_chip;

	ND_TRACE("[TCC NAND] board_nand_init\n");
	edi_init(EDI_MODE_NAND);

	if (NAND_DRV_GetFeatureOfNAND_MTD(&gNAND_IO_DeviceInfo) != TRUE)
	{
		ND_TRACE("[TCC NAND] Failed to get device info\n");
	}	

	if (tcc_nand_chip_init(mtd, nand, 0, base_addr[0]))
		ND_TRACE("[TCC NAND] Fail to initialize #%d chip", 0);

//	tcc_print_info();
}
struct mtd_info * tcc_get_mtd(void)
{
	struct mtd_info *mtd = &nand_info[0];
	return mtd;
}

uint64_t Get_MTD_size( void )
{
	unsigned int i = 0;
	unsigned int uiPartCnt			= 0;
	unsigned long long uiMTDBlkSize	= 0;
	/* It will be replaced getting partition info from mtdparts definition(in tcc893x_evm.h) */
	
	uiPartCnt = ARRAY_SIZE(tcc_partition_info);
	for (i = 0; i < uiPartCnt; i++)
	{
		ND_DEBUG("%d part name =%s, size = %llu\n", i, tcc_partition_info[i].name, tcc_partition_info[i].size);
		uiMTDBlkSize = uiMTDBlkSize + tcc_partition_info[i].size;
	}
	ND_DEBUG("PartCnt = %d, MTDAreaSize = %llu\n", uiPartCnt, uiMTDBlkSize);
	return uiMTDBlkSize;
	
}
