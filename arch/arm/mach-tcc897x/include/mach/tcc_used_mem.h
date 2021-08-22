/****************************************************************************
 *   FileName    : tcc_used_mem.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall constitute any express or implied warranty of any kind, 
including without limitation, any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. No warranty is made, express or implied, regarding the information's accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement between Telechips and Company.
*
****************************************************************************/

#include <common.h>
#include <asm/arch/tcc_used_mem_tcc893x.h>

#ifndef SZ_1K
#define SZ_1K		(1024)
#endif

#ifndef SZ_1M
#define SZ_1M		(1024u*1024u)
#endif

#define TCC_MEM_SIZE		(PHYS_DDR_SIZE)

#define MEM_PHYS_OFFSET		CONFIG_SYS_SDRAM_BASE

#define ARRAY_MBYTE(x)		((((x) + (SZ_1M-1))>> 20) << 20)

#define TOTAL_SDRAM_SIZE	(TCC_MEM_SIZE * SZ_1M)


