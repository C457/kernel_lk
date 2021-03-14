/****************************************************************************
 *   FileName    : vioc_viqe.h
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

#ifndef __VIOC_FIFO_H__
#define	__VIOC_FIFO_H__

#include <reg_physical.h>

extern void VIOC_ASYNC_FIFO_ConfigDMA(VIOC_FIFO * pFIFO, unsigned int nWDMA, unsigned int nRDMA0, unsigned int nRDMA1, unsigned int nRDMA2);
extern void VIOC_ASYNC_FIFO_ConfigEntry(VIOC_FIFO * pFIFO, unsigned int * buf);
extern void VIOC_ASYNC_FIFO_SetEnable(VIOC_FIFO * pFIFO, unsigned int nWDMA, unsigned int nRDMA0, unsigned int nRDMA1, unsigned int nRDMA2);

#endif //__VIOC_FIFO_H__

