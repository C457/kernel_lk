
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

#include "delay.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  General defines
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Internal Function Define
**---------------------------------------------------------------------------
*/



/*
**---------------------------------------------------------------------------
**  External Function Define
**---------------------------------------------------------------------------
*/

void delay_1ms(int count) {
	int i=1;
	int time;
	
	while(count--) {
		time = 3500;
		while(time--) {
			i *= time;
		}
	}
}

void mdelay(unsigned long ms) {
	volatile unsigned long __us = ms * 7500;
	do {
		__us--;
	} while(__us > 0);
}

void tcc_cif_delay(int ms) {
	volatile unsigned int msec = ms / 10; //10msec unit

	if(!msec)	mdelay(1);
	else		mdelay(msec);
}

