
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
#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H

#ifndef __cplusplus

#define false 0
#define true 1

#endif

#include <stddef.h>
#include <limits.h>
#include <stdint.h>

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned int	u_int;
typedef unsigned long	u_long;

typedef int		status_t;

typedef uintptr_t	addr_t;
typedef uintptr_t	vaddr_t;
typedef uintptr_t	paddr_t;

#endif

