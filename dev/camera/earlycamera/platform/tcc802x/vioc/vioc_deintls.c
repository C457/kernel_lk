
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.

  This software is licensed under the terms of the GNU General Public
  License version 2, as published by the Free Software Foundation, and
  may be copied, distributed, and modified under those terms.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
****************************************************************************/

#include <debug.h>
#include "vioc_deintls.h"

void VIOC_DEINTLS_SetDeIntlMode(unsigned int * pDeintls, unsigned int mode) {
//	printk("%s() - DeInterlacing mode = 0x%x\n", __func__, mode);
	BITCSET(*pDeintls, 0x0000000F, mode);
}

