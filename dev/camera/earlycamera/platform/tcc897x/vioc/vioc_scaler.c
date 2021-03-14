
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

#include "vioc_scaler.h"

void VIOC_SC_SetBypass(PVIOC_SC pSCALER, unsigned int nOnOff)
{
	//pSCALER->uCTRL.bREG.BP  = nOnOff;
	BITCSET(pSCALER->uCTRL.nREG, 0x1, nOnOff);
}

void VIOC_SC_SetUpdate(PVIOC_SC pSCALER)
{
	//pSCALER->uCTRL.bREG.UPD  = 1;
	BITCSET(pSCALER->uCTRL.nREG, (0x1<<16), (0x1<<16));
}

void VIOC_SC_SetSrcSize(PVIOC_SC pSCALER, unsigned int nWidth, unsigned int nHeight)
{
	//pSCALER->uSRCSIZE.bREG.WIDTH    = nWidth;
	//pSCALER->uSRCSIZE.bREG.HEIGHT   = nHeight;
	BITCSET(pSCALER->uSRCSIZE.nREG, (0x1FFF<<16), (nHeight<<16));
	BITCSET(pSCALER->uSRCSIZE.nREG, 0x1FFF, nWidth);
}

void VIOC_SC_SetDstSize(PVIOC_SC pSCALER, unsigned int nWidth, unsigned int nHeight)
{
	//pSCALER->uDSTSIZE.bREG.WIDTH    = nWidth;
	//pSCALER->uDSTSIZE.bREG.HEIGHT   = nHeight;
	BITCSET(pSCALER->uDSTSIZE.nREG, (0x1FFF<<16), (nHeight<<16));
	BITCSET(pSCALER->uDSTSIZE.nREG, 0x1FFF, nWidth);
}

void VIOC_SC_SetOutSize(PVIOC_SC pSCALER, unsigned int nWidth, unsigned int nHeight)
{
	//pSCALER->uOUTSIZE.bREG.WIDTH    = nWidth;
	//pSCALER->uOUTSIZE.bREG.HEIGHT   = nHeight;
	BITCSET(pSCALER->uOUTSIZE.nREG, (0x1FFF<<16), (nHeight<<16));
	BITCSET(pSCALER->uOUTSIZE.nREG, 0x1FFF, nWidth);
}

void VIOC_SC_SetOutPosition(PVIOC_SC pSCALER, unsigned int nXpos, unsigned int nYpos)
{
	//pSCALER->uOUTPOS.bREG.XPOS  = nXpos;
	//pSCALER->uOUTPOS.bREG.YPOS  = nYpos;
	BITCSET(pSCALER->uOUTPOS.nREG, (0x1FFF<<16), (nYpos<<16));
	BITCSET(pSCALER->uOUTPOS.nREG, 0x1FFF, nXpos);
}

