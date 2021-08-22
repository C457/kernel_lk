/****************************************************************************
 *   FileName    : tcc_nand_drv.h
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
#include "nand_io_v8.h"

#define CONFIG_SYS_NAND_USE_FLASH_BBT
#define CONFIG_SYS_NAND_READY_PIN

//*****************************************************************************
//*
//*
//*                       [ EXTERNAL FUCTIONS DEFINE ]
//*
//*
//*****************************************************************************

//void edi_init( void );
int NAND_DRV_GetFeatureOfNAND_MTD( NAND_IO_DEVINFO *NAND_DRV_deviceInfo);
int tcc_nand_ready( struct mtd_info *mtd );
int tcc_nand_isbad(struct mtd_info *mtd, loff_t ofs);
int tcc_nand_markbad(struct mtd_info *mtd, loff_t ofs);
void tcc_nand_select_chip(struct mtd_info *mtd, int chip);
int tcc_mtd_erase(struct mtd_info *mtd, struct erase_info *instr);
int tcc_nand_erase_block(struct mtd_info *mtd, int page);
int tcc_nand_read_page(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int oob_required, int page);
int tcc_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offset, int data_len, const uint8_t *buf, 
			int oob_required, int page,	int cached, int raw);
int tcc_nand_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page);
int tcc_mtd_read_oob (struct mtd_info *mtd, loff_t from, struct mtd_oob_ops *ops);
int tcc_mtd_write_oob(struct mtd_info *mtd, loff_t to, struct mtd_oob_ops *ops);
uint8_t tcc_nand_read_byte(struct mtd_info *mtd);
u16 tcc_nand_read_word(struct mtd_info *mtd);
void tcc_print_info( void );


