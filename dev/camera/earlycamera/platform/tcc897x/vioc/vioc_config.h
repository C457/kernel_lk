
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

#ifndef __VIOC_CONFIG_H__
#define	__VIOC_CONFIG_H__

#include <reg_physical.h>


#define	VIOC_SC0						(0)
#define	VIOC_SC1						(1)
#define	VIOC_SC2						(2)
#define	VIOC_SC3						(3)
		#define	VIOC_SC_RDMA_00			(00)
		#define	VIOC_SC_RDMA_01			(01)
		#define	VIOC_SC_RDMA_02			(02)
		#define	VIOC_SC_RDMA_03			(03)
		#define	VIOC_SC_RDMA_04			(04)
		#define	VIOC_SC_RDMA_05			(05)
		#define	VIOC_SC_RDMA_06			(06)
		#define	VIOC_SC_RDMA_07			(07)
#define	VIOC_VIQE						(4)
#define	VIOC_DEINTLS					(5)

#define	VIOC_DEVICE_INVALID     		(-2)
#define	VIOC_DEVICE_BUSY				(-1)
#define	VIOC_DEVICE_CONNECTED			( 0)

#define VIOC_PATH_DISCONNECTED			(0)
#define VIOC_PATH_CONNECTING  			(1)
#define VIOC_PATH_CONNECTED   			(2)
#define VIOC_PATH_DISCONNECTING			(3)

typedef enum {
	WMIX00 = 0,
	WMIX03,
	WMIX10,
	WMIX13,
	WMIX30,
	WMIX40,
	WMIX50,
	WMIX60,
	WMIX_MAX
} VIOC_CONFIG_WMIX_PATH;

/* Interface APIs */
extern int VIOC_CONFIG_PlugIn(unsigned int nType, unsigned int nValue);
extern int VIOC_CONFIG_PlugOut(unsigned int nType);
extern void VIOC_CONFIG_WMIXPath(unsigned int Path, unsigned int Mode);
#endif



