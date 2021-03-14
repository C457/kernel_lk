/****************************************************************************
 *   FileName    : vioc_viqe.c
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

#include <reg_physical.h>
#include "vioc_viqe.h"

void VIOC_VIQE_SetImageSize(VIQE *pVIQE, unsigned int width, unsigned int height)
{


	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL_SIZE.nREG, 0xffffffff, (height << 16) |width);	
//	pVIQE->cVIQE_CTRL.nVIQE_CTRL_SIZE.bSIZE.width	= width;
//	pVIQE->cVIQE_CTRL.nVIQE_CTRL_SIZE.bSIZE.height	= height;
	//dprintf(CRITICAL, "DEBUG: VIOC_VIQE_SetImageSize = pVIQE->cVIQE_CTRL.nVIQE_CTRL_SIZE : 0x%x:0x%x \n",&pVIQE->cVIQE_CTRL.nVIQE_CTRL_SIZE,pVIQE->cVIQE_CTRL.nVIQE_CTRL_SIZE); // VIN		
}

void VIOC_VIQE_SetImageY2RMode(VIQE *pVIQE, unsigned int y2r_mode)
{
	pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R.bREG.y2r_mode = y2r_mode;
}

void VIOC_VIQE_SetImageY2REnable(VIQE *pVIQE, unsigned int enable)
{
//	pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R.bVIQE_CTRL_Y2R.y2r_en = enable;
	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R.nREG, (1 << 8), (enable << 8));	
	//dprintf(CRITICAL, "DEBUG: VIOC_VIQE_SetImageY2REnable = pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R : 0x%x:0x%x \n",&pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R.bVIQE_CTRL_Y2R,pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R.bVIQE_CTRL_Y2R); // VIN		
}

void VIOC_VIQE_SetControlMisc(VIQE *pVIQE, unsigned int no_hor_intpl, unsigned int fmt_conv_disable, unsigned int fmt_conv_disable_using_fmt, unsigned int update_disable, unsigned int cfgupd, unsigned int h2h)
{
/*
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.no_hor_intpl				= no_hor_intpl; Hw8
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.fmt_conv_disable			= fmt_conv_disable; Hw16
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.fmt_conv_disable_using_fmt = fmt_conv_disable_using_fmt; Hw17
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.update_disable			= update_disable; Hw20
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.cfgupd					= cfgupd; Hw21
 */


	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL.nREG, 0x00330100, 
								(cfgupd << 21)
								|(update_disable << 20) 
								|(fmt_conv_disable_using_fmt << 17) 
								|(fmt_conv_disable << 16) 
								|(no_hor_intpl << 8));
	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL_Y2R.nREG, 0xFF, h2h);
}

void VIOC_VIQE_SetControlDontUse(VIQE *pVIQE, unsigned int global_en_dont_use, unsigned int top_size_dont_use, unsigned int stream_deintl_info_dont_use)
{
/*
	pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.bVIQE_CTRL_MISC.global_en_dont_use			= global_en_dont_use;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.bVIQE_CTRL_MISC.top_size_dont_use				= top_size_dont_use;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.bVIQE_CTRL_MISC.stream_deintl_info_dont_use	= stream_deintl_info_dont_use;
*/
	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.nREG, 0x07, \
	global_en_dont_use | (top_size_dont_use << 1) | (stream_deintl_info_dont_use) << 2 );	
       //dprintf(CRITICAL, "DEBUG: VIOC_VIQE_SetControlDontUse = pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.uVIQE_CTRL_MISC : 0x%x:0x%x \n",
//	   		&pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.uVIQE_CTRL_MISC,pVIQE->cVIQE_CTRL.nVIQE_CTRL_MISC.uVIQE_CTRL_MISC); // VIN			
}

void VIOC_VIQE_SetControlClockGate(VIQE *pVIQE, unsigned int deintl_dis, unsigned int d3d_dis, unsigned int pm_dis)
{
/*
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.clkgate_deintl_disable	= deintl_dis;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.clkgate_d3d_disable		= d3d_dis;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.clkgate_pm_disable		= pm_dis;
*/

	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL.nREG, 0x07000000, 
								(pm_dis << 26) 
								|(d3d_dis << 25) 
								|(deintl_dis << 24));

}

void VIOC_VIQE_SetControlEnable(VIQE *pVIQE, unsigned int his_cdf_or_lut_en, unsigned int his_en, unsigned int gamut_en, unsigned int denoise3d_en, unsigned int deintl_en)
{
/*
	pVIQE->cDEINTL_DMA.nDEINTL_CTRL.bDEINTL_CTRL.enable 			= deintl_en;
	pVIQE->cDEINTL_DECOMP0.nVIQE_FC_CTRL.bVIQE_FC_CTRL.fc_enable	= deintl_en;
	pVIQE->cDEINTL_DECOMP1.nVIQE_FC_CTRL.bVIQE_FC_CTRL.fc_enable	= deintl_en;
	pVIQE->cDEINTL_DECOMP2.nVIQE_FC_CTRL.bVIQE_FC_CTRL.fc_enable	= deintl_en;
	pVIQE->cDEINTL_COMP.nVIQE_FC_CTRL.bVIQE_FC_CTRL.fc_enable 		= deintl_en;

	pVIQE->cD3D_DMA.nD3D_CTRL.bD3D_CTRL.enable						= denoise3d_en;
	pVIQE->cD3D_DECOMP.nVIQE_FC_CTRL.bVIQE_FC_CTRL.fc_enable		= denoise3d_en;
	pVIQE->cD3D_COMP.nVIQE_FC_CTRL.bVIQE_FC_CTRL.fc_enable 			= denoise3d_en;

	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.his_cdf_or_lut_en		= his_cdf_or_lut_en;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.his_en					= his_en;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.gamut_en				= gamut_en;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.denoise3d_en			= denoise3d_en;
	pVIQE->cVIQE_CTRL.nVIQE_CTRL.bVIQE_CTRL.deintl_en				= deintl_en;
*/

	BITCSET(pVIQE->cDEINTL_DECOMP0.nVIQE_FC_CTRL.nREG, (0x1<<31)  ,( deintl_en << 31 ) );			
	BITCSET(pVIQE->cDEINTL_DECOMP1.nVIQE_FC_CTRL.nREG, (0x1<<31)  ,( deintl_en << 31 ) );			
	BITCSET(pVIQE->cDEINTL_DECOMP2.nVIQE_FC_CTRL.nREG, (0x1<<31)  ,( deintl_en << 31 ) );			
	BITCSET(pVIQE->cDEINTL_COMP.nVIQE_FC_CTRL.nREG, (0x1<<31)  ,( deintl_en << 31 ) );			


	BITCSET(pVIQE->cVIQE_CTRL.nVIQE_CTRL.nREG,0x1F, 
								(deintl_en << 4)
								|(0 << 3) 
								|(0 << 2) 
								|(0 << 1) 
								|(0 << 0));	


}


void VIOC_VIQE_SetControlRegister(VIQE *pVIQE, unsigned int width, unsigned int height, unsigned int fmt)
{
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);
	VIOC_VIQE_SetImageSize(pVIQE, width, height);
	VIOC_VIQE_SetControlMisc(pVIQE, OFF, OFF, ON, OFF, ON, 0x16);	/* All of variables are the recommended value */
	VIOC_VIQE_SetControlDontUse(pVIQE, OFF, OFF, OFF);				/* All of variables are the recommended value */
	VIOC_VIQE_SetControlClockGate(pVIQE, OFF, OFF, OFF);			/* All of variables are the recommended value */
}

void VIOC_VIQE_SetDeintlBase(VIQE *pVIQE, unsigned int frmnum, unsigned int base0, unsigned int base1, unsigned int base2, unsigned int base3)
{
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);
	if (frmnum == 0)
	{
	/*
		pVIQE->cDEINTL_DMA.nDEINTL_BASE0.nREG	= base0;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE1.nREG	= base1;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE2.nREG	= base2;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE3.nREG	= base3;
	*/
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE0.nREG, 0xFFFFFFFF, base0);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE1.nREG, 0xFFFFFFFF, base1);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE2.nREG, 0xFFFFFFFF, base2);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE3.nREG, 0xFFFFFFFF, base3);	
	}
	else if (frmnum == 1)
	{
	/*
		pVIQE->cDEINTL_DMA.nDEINTL_BASE0A.nREG	= base0;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE1A.nREG	= base1;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE2A.nREG	= base2;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE3A.nREG	= base3;
	*/
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE0A.nREG, 0xFFFFFFFF, base0);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE1A.nREG, 0xFFFFFFFF, base1);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE2A.nREG, 0xFFFFFFFF, base2);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE3A.nREG, 0xFFFFFFFF, base3);	
	}
	else if (frmnum == 2)
	{
	/*
		pVIQE->cDEINTL_DMA.nDEINTL_BASE0B.nREG	= base0;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE1B.nREG	= base1;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE2B.nREG	= base2;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE3B.nREG	= base3;
	*/
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE0B.nREG, 0xFFFFFFFF, base0);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE1B.nREG, 0xFFFFFFFF, base1);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE2B.nREG, 0xFFFFFFFF, base2);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE3B.nREG, 0xFFFFFFFF, base3);	
	}
	else
	{
	/*
		pVIQE->cDEINTL_DMA.nDEINTL_BASE0C.nREG	= base0;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE1C.nREG	= base1;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE2C.nREG	= base2;
		pVIQE->cDEINTL_DMA.nDEINTL_BASE3C.nREG	= base3;
	*/
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE0C.nREG, 0xFFFFFFFF, base0);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE1C.nREG, 0xFFFFFFFF, base1);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE2C.nREG, 0xFFFFFFFF, base2);
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_BASE3C.nREG, 0xFFFFFFFF, base3);	
	}
	
}

void VIOC_VIQE_SetDeintlSize(VIQE *pVIQE, unsigned int width, unsigned int height)
{
	BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_SIZE.nREG, 0x07FF07FF, ((height>>1) << 16)|(width << 0));
}


void VIOC_VIQE_SetDeintlMisc(VIQE *pVIQE, unsigned int uvintpl, unsigned int cfgupd, unsigned int dma_enable, unsigned int h2h, unsigned int top_size_dont_use)
{
	BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_CTRL.nREG, 0xFF030003, 
								(h2h << 24) 
								|(cfgupd << 17) 
								|(dma_enable << 16) 			//DI DMA enable
								|(uvintpl << 2) 
								|(top_size_dont_use << 0));
}


void VIOC_VIQE_SetDeintlControl(VIQE *pVIQE, unsigned int fmt, unsigned int eof_control_ready, unsigned int dec_divisor, unsigned int ac_k0_limit, unsigned int ac_k1_limit, unsigned int ac_k2_limit)
{

	BITCSET(pVIQE->cDEINTL_DECOMP0.nVIQE_FC_MISC.nREG, 0x304 ,(eof_control_ready << 2) |(dec_divisor << 8) );		
	BITCSET(pVIQE->cDEINTL_DECOMP1.nVIQE_FC_MISC.nREG, 0x304 ,(eof_control_ready << 2) |(dec_divisor << 8) );		
	BITCSET(pVIQE->cDEINTL_DECOMP2.nVIQE_FC_MISC.nREG, 0x304 ,(eof_control_ready << 2) |(dec_divisor << 8) );		

	BITCSET(pVIQE->cDEINTL_COMP.nVIQE_FC_MISC.nREG,0xF000 ,(fmt << 12) );		

	BITCSET(pVIQE->cDEINTL_COMP.nVIQE_FC_LIMIT.nREG ,0x003F3F3F ,(ac_k0_limit ) | (ac_k1_limit << 8) | (ac_k2_limit <<  16) );		
}

void VIOC_VIQE_InitDeintlCoreSpatial(VIQE *pVIQE)
{	/* Deinterlacing By Using Just Input Frame */

	BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0xFFFFFFFF, 0x00030a31);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE0, 0xFFFFFFFF, 0x02040408);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE1, 0xFFFFFFFF, 0x2812050f);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE2, 0xFFFFFFFF, 0x00800410);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE3, 0xFFFFFFFF, 0x01002000);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE4, 0xFFFFFFFF, 0x12462582);
	BITCSET(pVIQE->cDEINTL.nPD_THRES0, 0xFFFFFFFF, 0x010085f4);
	BITCSET(pVIQE->cDEINTL.nPD_THRES1, 0xFFFFFFFF, 0x001e140f);
	BITCSET(pVIQE->cDEINTL.nPD_JUDDER, 0xFFFFFFFF, 0x6f408805);
	BITCSET(pVIQE->cDEINTL.nPD_JUDDER_M, 0xFFFFFFFF, 0x00095800);
	BITCSET(pVIQE->cDEINTL.nDI_MISCC, 0xFFFFFFFF, 0x00000000);
	
	BITCSET(pVIQE->cDEINTL.nDI_REGION0, 0xFFFFFFFF, 0x00000000);
	BITCSET(pVIQE->cDEINTL.nDI_REGION1, 0xFFFFFFFF, 0x00000000);
	BITCSET(pVIQE->cDEINTL.nDI_INT, 0xFFFFFFFF, 0x00000000);
	
	BITCSET(pVIQE->cDEINTL.nPD_SAW, 0xFFFFFFFF, 0x0008050a);
}
	
void VIOC_VIQE_SetDeintlCore(VIQE *pVIQE, unsigned int width, unsigned int height, VIOC_VIQE_FMT_TYPE fmt, unsigned int bypass, unsigned int top_size_dont_use)
{
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);

	if(bypass)
		BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0x00000100, (0<<8));						//0x280

	BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0x20000000, (bypass<<29));						//0x280
	BITCSET(pVIQE->cDEINTL.nDI_CSIZE, 0x07FF07FF, (height << 16) | (width << 0));			//0x2E4  
	BITCSET(pVIQE->cDEINTL.nDI_FMT, 0x00010001, (top_size_dont_use << 16) | (fmt << 0));	//0x2E8
}

void VIOC_VIQE_InitDeintlCoreBypass(VIQE *pVIQE)
{	/* Bypass Through Deinterlacer Core */
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);
	/*
	pVIQE->cDEINTL.nDI_CTRL 		= 0x02010a31;			//0x280  
	pVIQE->cDEINTL.nDI_ENGINE0 	= 0x02040408;			//0x284  
	pVIQE->cDEINTL.nDI_ENGINE1 	= 0x7f32040f;			//0x288  
	pVIQE->cDEINTL.nDI_ENGINE2 	= 0x00800410;			//0x28C  
	pVIQE->cDEINTL.nDI_ENGINE3 	= 0x01002000;			//0x290  
	pVIQE->cDEINTL.nDI_ENGINE4 	= 0x12462582;			//0x294  
	pVIQE->cDEINTL.nPD_THRES0 	= 0x010085f4;			//0x298  
	pVIQE->cDEINTL.nPD_THRES1 	= 0x001e140f;			//0x29C  
	pVIQE->cDEINTL.nPD_JUDDER 	= 0x6f40881e;			//0x2A0  
	pVIQE->cDEINTL.nPD_JUDDER_M 	= 0x00095800;			//0x2A4  
	pVIQE->cDEINTL.nDI_MISCC	 	= 0x00000000;			//0x2A8  
	
	pVIQE->cDEINTL.nDI_REGION0 	= 0x00000000;			//0x2B4  
	pVIQE->cDEINTL.nDI_REGION1 	= 0x00000000;			//0x2B8  
	pVIQE->cDEINTL.nDI_INT		= 0x00000000;			//0x2BC  
	
	pVIQE->cDEINTL.nPD_SAW 		= 0x0008050a;			//0x2E0  
//	pVIQE->cDEINTL.nDI_CSIZE 		= 0x04380780;			//0x2E4  
//	pVIQE->cDEINTL.nDI_FMT 		= 0x00000001;			//0x2E8  
	*/
	BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0xFFFFFFFF, 0x00010b31);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE0, 0xFFFFFFFF, 0x02040408);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE1, 0xFFFFFFFF, 0x7f32040f);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE2, 0xFFFFFFFF, 0x00800410);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE3, 0xFFFFFFFF, 0x01002000);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE4, 0xFFFFFFFF, 0x12462582);
	BITCSET(pVIQE->cDEINTL.nPD_THRES0, 0xFFFFFFFF, 0x010085f4);
	BITCSET(pVIQE->cDEINTL.nPD_THRES1, 0xFFFFFFFF, 0x001e140f);
	BITCSET(pVIQE->cDEINTL.nPD_JUDDER, 0xFFFFFFFF, 0x6f40881e);
	BITCSET(pVIQE->cDEINTL.nPD_JUDDER_M, 0xFFFFFFFF, 0x00095800);
	BITCSET(pVIQE->cDEINTL.nDI_MISCC, 0xFFFFFFFF, 0x00000000);
	
	BITCSET(pVIQE->cDEINTL.nDI_REGION0, 0xFFFFFFFF, 0x00000000);
	BITCSET(pVIQE->cDEINTL.nDI_REGION1, 0xFFFFFFFF, 0x00000000);
	BITCSET(pVIQE->cDEINTL.nDI_INT, 0xFFFFFFFF, 0x00000000);
	
	BITCSET(pVIQE->cDEINTL.nPD_SAW, 0xFFFFFFFF, 0x0008050a);

}

void VIOC_VIQE_InitDeintlCoreTemporal(VIQE *pVIQE)
{	/* Deinterlacing By Using 4-Field Reference Frame */
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);
	/*	
	pVIQE->cDEINTL.nDI_CTRL 		= 0x00010a31;			//0x280  
	pVIQE->cDEINTL.nDI_ENGINE0 	= 0x02040408;			//0x284  
	pVIQE->cDEINTL.nDI_ENGINE1 	= 0x7f32050f;			//0x288  
	pVIQE->cDEINTL.nDI_ENGINE2 	= 0x00800410;			//0x28C  
	pVIQE->cDEINTL.nDI_ENGINE3 	= 0x01002000;			//0x290  
	pVIQE->cDEINTL.nDI_ENGINE4 	= 0x12462582;			//0x294  
	pVIQE->cDEINTL.nPD_THRES0 	= 0x010085f4;			//0x298  
	pVIQE->cDEINTL.nPD_THRES1 	= 0x001e140f;			//0x29C  
	pVIQE->cDEINTL.nPD_JUDDER 	= 0x6f408805;			//0x2A0  
	pVIQE->cDEINTL.nPD_JUDDER_M 	= 0x00095800;			//0x2A4  
	pVIQE->cDEINTL.nDI_MISCC	 	= 0x00000000;			//0x2A8  
	
	pVIQE->cDEINTL.nDI_REGION0 	= 0x00000000;			//0x2B4  
	pVIQE->cDEINTL.nDI_REGION1 	= 0x00000000;			//0x2B8  
	pVIQE->cDEINTL.nDI_INT		= 0x00000000;			//0x2BC  
	
	pVIQE->cDEINTL.nPD_SAW 		= 0x0008050a;			//0x2E0  
//	pVIQE->cDEINTL.nDI_CSIZE 		= 0x01200160;			//0x2E4  
//	pVIQE->cDEINTL.nDI_FMT 		= 0x00000000;			//0x2E8  
	*/
	BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0xFFFFFFFF, 0x00010a31);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE0, 0xFFFFFFFF, 0x02040408);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE1, 0xFFFFFFFF, 0x7f32050f);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE2, 0xFFFFFFFF, 0x00800410);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE3, 0xFFFFFFFF, 0x01002000);
	BITCSET(pVIQE->cDEINTL.nDI_ENGINE4, 0xFFFFFFFF, 0x12462582);
	BITCSET(pVIQE->cDEINTL.nPD_THRES0, 0xFFFFFFFF, 0x010085f4);
	BITCSET(pVIQE->cDEINTL.nPD_THRES1, 0xFFFFFFFF, 0x001e140f);
	BITCSET(pVIQE->cDEINTL.nPD_JUDDER, 0xFFFFFFFF, 0x6f408805);
	BITCSET(pVIQE->cDEINTL.nPD_JUDDER_M, 0xFFFFFFFF, 0x00095800);
	BITCSET(pVIQE->cDEINTL.nDI_MISCC, 0xFFFFFFFF, 0x00000000);
	
	BITCSET(pVIQE->cDEINTL.nDI_REGION0, 0xFFFFFFFF, 0x00000000);
	BITCSET(pVIQE->cDEINTL.nDI_REGION1, 0xFFFFFFFF, 0x00000000);
	BITCSET(pVIQE->cDEINTL.nDI_INT, 0xFFFFFFFF, 0x00000000);
	
	BITCSET(pVIQE->cDEINTL.nPD_SAW, 0xFFFFFFFF, 0x0008050a);
}

void VIOC_VIQE_SetDeintlMode(VIQE *pVIQE, VIOC_VIQE_DEINTL_MODE mode)
{
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);
	
	if(mode == VIOC_VIQE_DEINTL_MODE_BYPASS)
	{
		BITCSET(pVIQE->cDEINTL.nDI_CTRL , 0xFFFFFFFF, 0x20010b31);
		BITCSET(pVIQE->cDEINTL.nDI_ENGINE1, 0xFFFFFFFF, 0x7f32040f);
		BITCSET(pVIQE->cDEINTL.nPD_JUDDER, 0xFFFFFFFF, 0x6f40881e);
		
		BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0x20000000, (1<<29));						//bypass
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_CTRL.nREG, (0x1 << 16), (1 << 16)); 		//DI DMA enable
	}
	else if(mode == VIOC_VIQE_DEINTL_MODE_2D)
	{
		BITCSET(pVIQE->cDEINTL.nDI_CTRL , 0xFFFFFFFF, 0x00020a31);
		BITCSET(pVIQE->cDEINTL.nDI_ENGINE1, 0xFFFFFFFF, 0x2812050f);
		BITCSET(pVIQE->cDEINTL.nPD_JUDDER, 0xFFFFFFFF, 0x6f408805);

		BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0x20000000, (0<<29));						//bypass
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_CTRL.nREG, (0x1 << 16), (1 << 16)); 		//DI DMA enable
	}
	else if(mode == VIOC_VIQE_DEINTL_MODE_3D)	//Temporal Mode - using 4-field frames.
	{
		BITCSET(pVIQE->cDEINTL.nDI_CTRL , 0xFFFFFFFF, 0x00010a31);
		BITCSET(pVIQE->cDEINTL.nDI_ENGINE1, 0xFFFFFFFF, 0x7f32050f);
		BITCSET(pVIQE->cDEINTL.nPD_JUDDER, 0xFFFFFFFF, 0x6f408805);

		BITCSET(pVIQE->cDEINTL.nDI_CTRL, 0x20000000, (0<<29));						//bypass
		BITCSET(pVIQE->cDEINTL_DMA.nDEINTL_CTRL.nREG, (0x1 << 16), (1 << 16)); 		//DI DMA enable
	}
	
}	


void VIOC_VIQE_SetDeintlRegister(VIQE *pVIQE, unsigned int fmt, unsigned int top_size_dont_use, unsigned int width, unsigned int height, 
	VIOC_VIQE_DEINTL_MODE mode, unsigned int base0, unsigned int base1, unsigned int base2, unsigned int base3)
{
	int bypass =0;
	int dma_enable =1;
	//dprintk(KERN_INFO "%s\n", __FUNCTION__);
	
	if(mode == VIOC_VIQE_DEINTL_MODE_BYPASS)
	{
		bypass =1;
		VIOC_VIQE_InitDeintlCoreBypass(pVIQE);
	}	
	else if(mode == VIOC_VIQE_DEINTL_MODE_2D)
	{
		VIOC_VIQE_InitDeintlCoreSpatial(pVIQE);
	}
	else			//VIOC_VIQE_DEINTL_MODE_3D
	{
		dma_enable =1;
		VIOC_VIQE_InitDeintlCoreTemporal(pVIQE);
	}

	VIOC_VIQE_SetDeintlBase(pVIQE, 0, base0, base1, base2, base3); 
	VIOC_VIQE_SetDeintlSize(pVIQE, width, height);
	VIOC_VIQE_SetDeintlMisc(pVIQE, OFF, ON, dma_enable, 0x16, OFF);					/* All of variables are the recommended value */
	VIOC_VIQE_SetDeintlControl(pVIQE, fmt, ON, 0x3, 0x31, 0x2a, 0x23);	/* All of variables are the recommended value */
	VIOC_VIQE_SetDeintlCore(pVIQE, width, height, fmt, bypass, OFF);
}

