/****************************************************************************
 *   FileName    : vioc_fifo.c
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
#include "vioc_fifo.h"

void VIOC_ASYNC_FIFO_ConfigDMA(VIOC_FIFO * pFIFO, unsigned int nWDMA, unsigned int nRDMA0, unsigned int nRDMA1, unsigned int nRDMA2) {
//	log("pFIFO = 0x%x, WDMA = %d, RDMA0 = %d, RDMA1 = %d, RDMA2 = %d\n", pFIFO, nWDMA, nRDMA0, nRDMA1, nRDMA2);
	BITCSET(pFIFO->uCH0_CTRL1.nREG, 0x000FFFF, ((nRDMA2<<12)|(nRDMA1<<8)|(nRDMA0<<4)|(nWDMA<<0)));
}

void VIOC_ASYNC_FIFO_ConfigEntry(VIOC_FIFO * pFIFO, unsigned int * buf) {
	unsigned int EEMPTY	= 0; // emergency empty
	unsigned int EFULL	= 0; // emergency full
	unsigned int WMT	= 0; // wdma mode - time
	unsigned int NENTRY	= 4; // frame memory number  ->  max. frame count is 4.
	unsigned int RMT	= 0; // rdma mode - time
	unsigned int idxBuf;

	for(idxBuf=0; idxBuf < NENTRY; idxBuf++) {
//		log("buf[%d]: 0x%08x\n", idxBuf, buf[idxBuf]);
		BITCSET(pFIFO->nCH0_BASE[idxBuf], 0xFFFFFFFF, buf[idxBuf]);
	}
	BITCSET(pFIFO->uCH0_CTRL0.nREG, 0x0003FFFF, ((EEMPTY<<18)|(EFULL<<16)|(WMT<<14)|(NENTRY<<8)|(RMT<<4)));
}

void VIOC_ASYNC_FIFO_SetEnable(VIOC_FIFO * pFIFO, unsigned int nWDMA, unsigned int nRDMA0, unsigned int nRDMA1, unsigned int nRDMA2) {
//	log("pFIFO = 0x%x, WDMA = %d, RDMA0 = %d, RDMA1 = %d, RDMA2 = %d\n", pFIFO, nWDMA, nRDMA0, nRDMA1, nRDMA2);
	BITCSET(pFIFO->uCH0_CTRL0.nREG, 0x0000000F, (((nRDMA2&1)<<3)|((nRDMA1&1)<<2)|((nRDMA0&1)<<1)|(nWDMA&1)));
}

