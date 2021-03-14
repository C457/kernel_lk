/****************************************************************************
 *   FileName    : vioc_vin.c
 *   Description : 
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
/******************************************************************************
*	yyyy/mm/dd		ver		descriptions		  Author
*	----------		---		------------		--------
*	2011/10/05		0.1		     created		C3-G1-3T
*******************************************************************************/
#include <reg_physical.h>
#include "vioc_vin.h"


#define VIOC_VIN_IREQ_UPD_MASK		0x00000001UL
#define VIOC_VIN_IREQ_EOF_MASK		0x00000002UL
#define VIOC_VIN_IREQ_VS_MASK		0x00000004UL
#define VIOC_VIN_IREQ_INVS_MASK		0x00000008UL

#define	Hw31								0x80000000
#define	Hw30								0x40000000
#define	Hw29								0x20000000
#define	Hw28								0x10000000
#define	Hw27								0x08000000
#define	Hw26								0x04000000
#define	Hw25								0x02000000
#define	Hw24								0x01000000
#define	Hw23								0x00800000
#define	Hw22								0x00400000
#define	Hw21								0x00200000
#define	Hw20								0x00100000
#define	Hw19								0x00080000
#define	Hw18								0x00040000
#define	Hw17								0x00020000
#define	Hw16								0x00010000
#define	Hw15								0x00008000
#define	Hw14								0x00004000
#define	Hw13								0x00002000
#define	Hw12								0x00001000
#define	Hw11								0x00000800
#define	Hw10								0x00000400
#define	Hw9									0x00000200
#define	Hw8									0x00000100
#define	Hw7									0x00000080
#define	Hw6									0x00000040
#define	Hw5									0x00000020
#define	Hw4									0x00000010
#define	Hw3									0x00000008
#define	Hw2									0x00000004
#define	Hw1									0x00000002
#define	Hw0									0x00000001


void VIOC_VIN_SetSyncPolarity(VIOC_VIN *pVIN, unsigned int hs_active_low, unsigned int vs_active_low, unsigned int field_bfield_low, unsigned int de_active_low, unsigned int gen_field_en, unsigned int pxclk_pol)
{

/*
	pVIN->uVIN_CTRL.bREG.hs_active_low		= hs_active_low;
	pVIN->uVIN_CTRL.bREG.vs_active_low		= vs_active_low;
	pVIN->uVIN_CTRL.bREG.field_bfield_low	= field_bfield_low;
	pVIN->uVIN_CTRL.bREG.de_active_low		= de_active_low;
	pVIN->uVIN_CTRL.bREG.gen_field_en		= gen_field_en;
	pVIN->uVIN_CTRL.bREG.pxclk_pol			= pxclk_pol;
*/
	BITCSET(pVIN->uVIN_CTRL.nREG, 0x00003F00, (hs_active_low << 9) |(vs_active_low << 10)
		|(field_bfield_low << 11) | (de_active_low << 12) | (gen_field_en << 13) | (pxclk_pol << 8) );	

	
}

/* VIN Configuration 1 */
void VIOC_VIN_SetCtrl(VIOC_VIN *pVIN, unsigned int conv_en, unsigned int hsde_connect_en, unsigned int vs_mask, unsigned int fmt, unsigned int data_order)
{
/*
	pVIN->uVIN_CTRL.bREG.conv_en			= conv_en;
	pVIN->uVIN_CTRL.bREG.hsde_connect_en	= hsde_connect_en;
	pVIN->uVIN_CTRL.bREG.vs_mask			= vs_mask;
	pVIN->uVIN_CTRL.bREG.fmt				= fmt;
	pVIN->uVIN_CTRL.bREG.data_order		= data_order;
*/
	BITCSET(pVIN->uVIN_CTRL.nREG, 0x00FF0051, (conv_en << 1) |(hsde_connect_en << 4)
		|(vs_mask << 6) | (fmt << 16) | (data_order << 20));	

	
	//dprintf(CRITICAL, "DEBUG: VIOC_VIN_SetCtrl = pVIN->uVIN_CTRL : 0x%x:0x%x \n",&pVIN->uVIN_CTRL.nREG, pVIN->uVIN_CTRL.nREG); // VIN	
}


/* Interlace mode setting */
void VIOC_VIN_SetInterlaceMode(VIOC_VIN *pVIN, unsigned int intl_en, unsigned int intpl_en)
{

	pVIN->uVIN_CTRL.bVIN_CTRL.intl_en	= intl_en;
//	pVIN->uVIN_CTRL.bREG.intpl_en	= intpl_en;

	BITCSET(pVIN->uVIN_CTRL.nREG, 0x0000000C , (intl_en << 2) |(intpl_en << 3) );	

	//dprintf(CRITICAL, "DEBUG: VIOC_VIN_SetCtrl = pVIN->uVIN_CTRL : 0x%x:0x%x \n",&pVIN->uVIN_CTRL.nREG, pVIN->uVIN_CTRL.nREG); // VIN	
}

/* Image size setting */
void VIOC_VIN_SetImageSize(VIOC_VIN *pVIN, unsigned int width, unsigned int height)
{	/* pkjin20 : Chip Bug... To Prevent Read Operation */
	//pVIN->uVIN_SIZE.bVIN_SIZE.width	 	= width;	//(height << 16) | width;
	//pVIN->uVIN_SIZE.bVIN_SIZE.height	= height;

	BITCSET(pVIN->uVIN_SIZE.nREG, 0xFFFFFFFF, (height << 16) | width);	
}

/* Image offset setting */
void VIOC_VIN_SetImageOffset(VIOC_VIN *pVIN, unsigned int offs_width, unsigned int offs_height, unsigned int offs_height_intl)
{
	//pVIN->uVIN_OFFS.bVIN_OFFS.offs_width		= offs_width;
	//pVIN->uVIN_OFFS.bVIN_OFFS.offs_height		= offs_height;
	//pVIN->uVIN_OFFS_INTL.bVIN_OFFS.offs_height	= offs_height_intl;

	BITCSET(pVIN->uVIN_OFFS.nREG, 0xFFFFFFFF, (offs_height << 16) | (offs_width) );
	BITCSET(pVIN->uVIN_OFFS_INTL.nREG, 0xFFFF0000, (offs_height_intl << 16) );
}

/* VIN Enable/Disable */
void VIOC_VIN_SetEnable(VIOC_VIN *pVIN, unsigned int vin_en)
{
//	pVIN->uVIN_CTRL.bREG.enable	= vin_en;
	BITCSET(pVIN->uVIN_CTRL.nREG, 1  , vin_en );	
	//dprintf(CRITICAL, "DEBUG: VIOC_VIN_SetEnable = pVIN->uVIN_CTRL : 0x%x:0x%x \n",&pVIN->uVIN_CTRL,pVIN->uVIN_CTRL); // VIN		
}

void VIOC_VIN_SetImageCropSize(VIOC_VIN *pVIN, unsigned int width, unsigned int height)
{
	BITCSET(pVIN->uVIN_CROP_SIZE.nREG, 0xFFFFFFFF, ((height<<16)|(width)));
}

void VIOC_VIN_SetImageCropOffset(VIOC_VIN *pVIN, unsigned int offs_width, unsigned int offs_height)
{
	BITCSET(pVIN->uVIN_CROP_OFFS.nREG, 0xFFFFFFFF, ((offs_height<<16)|(offs_width)));
}

/* Y2R conversion mode setting */
void VIOC_VIN_SetY2RMode(VIOC_VIN *pVIN, unsigned int y2r_mode)
{
	BITCSET(pVIN->uVIN_MISC.nREG, 0x00000060, (y2r_mode << 5) );
}

/* Y2R conversion Enable/Disable */
void VIOC_VIN_SetY2REnable(VIOC_VIN *pVIN, unsigned int y2r_en)
{
	BITCSET(pVIN->uVIN_MISC.nREG, 0x00000010, (y2r_en << 4) );
}


void VIOC_VIN_SetLUT(VIOC_VIN *pVIN, unsigned int *pLUT)
{
	unsigned int *pLUT0, *pLUT1, *pLUT2, uiCount;
	
	pLUT0 = (unsigned int *)(pLUT  +     0);
	pLUT1 = (unsigned int *)(pLUT0 + 256/4);
	pLUT2 = (unsigned int *)(pLUT1 + 256/4);
	
//	pVIN->uVIN_MISC.bVIN_MISC.lut_apbif_en = 1;	/* Access Look-Up Table Using Slave Port */

	BITCSET(pVIN->uVIN_MISC.nREG, Hw3, 1 << 3 );	

	for (uiCount = 0 ; uiCount < 256 ; uiCount = uiCount + 4)
	{	/* Initialize Look-up Table */
		*pLUT0++ = ((uiCount + 3) << 24) |
		           ((uiCount + 2) << 16) |
		           ((uiCount + 1) <<  8) |
		           ((uiCount + 0) <<  0) ;
		*pLUT1++ = ((uiCount + 3) << 24) |
		           ((uiCount + 2) << 16) |
		           ((uiCount + 1) <<  8) |
		           ((uiCount + 0) <<  0) ;
		*pLUT2++ = ((uiCount + 3) << 24) |
		           ((uiCount + 2) << 16) |
		           ((uiCount + 1) <<  8) |
		           ((uiCount + 0) <<  0) ;
	}

//	pVIN->uVIN_MISC.bVIN_MISC.lut_apbif_en = 0;	/* Access Look-Up Table Using VIN Module */
	BITCSET(pVIN->uVIN_MISC.nREG, Hw3, 0 << 3 );	
}

/* LUT Enable/Disable */
void VIOC_VIN_SetLUTEnable(VIOC_VIN *pVIN, unsigned int lut0_en, unsigned int lut1_en, unsigned int lut2_en)
{
	BITCSET(pVIN->uVIN_MISC.nREG, 0x00000007, (lut2_en << 2) | (lut1_en << 1) | (lut0_en));
}

void VIOC_VIN_SetDemuxPort(VIOC_VIN_DEMUX *pVINDEMUX, unsigned int p0, unsigned int p1, unsigned int p2, unsigned int p3)
{
	pVINDEMUX->uVIN_DEMUX_CTRL.bVIN_DEMUX_CTRL.sel0 = p0;
	pVINDEMUX->uVIN_DEMUX_CTRL.bVIN_DEMUX_CTRL.sel1 = p1;
	pVINDEMUX->uVIN_DEMUX_CTRL.bVIN_DEMUX_CTRL.sel2 = p2;
	pVINDEMUX->uVIN_DEMUX_CTRL.bVIN_DEMUX_CTRL.sel3 = p3;
}

void VIOC_VIN_SetDemuxClock(VIOC_VIN_DEMUX *pVINDEMUX, unsigned int mode)
{
	pVINDEMUX->uVIN_DEMUX_CTRL.bVIN_DEMUX_CTRL.clk_mode	= mode;
}

void VIOC_VIN_SetDemuxEnable(VIOC_VIN_DEMUX *pVINDEMUX, unsigned int enable)
{
	pVINDEMUX->uVIN_DEMUX_CTRL.bVIN_DEMUX_CTRL.enable = enable;
}

/*EOF*/
