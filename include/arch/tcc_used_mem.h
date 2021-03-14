/****************************************************************************
 *	 TCC Version 0.6
 *	 Copyright (c) telechips, Inc.
 *	 ALL RIGHTS RESERVED
 *
****************************************************************************/

#ifndef SZ_1K
#define SZ_1K		(1024)
#endif

#ifndef SZ_1M
#define SZ_1M		(1024*1024)
#endif

#define MEM_PHYS_OFFSET						(BASE_ADDR)

#define ARRAY_MBYTE(x)		((((x) + (SZ_1M-1))>> 20) << 20)

#define TOTAL_SDRAM_SIZE					(TCC_MEM_SIZE * SZ_1M)

#include "tcc_used_mem_tcc897x.h"