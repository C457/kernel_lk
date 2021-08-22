/***************************************************************************************
*	FileName    : update.c
*	Description : firmware update 
****************************************************************************************
*
*	TCC Board Support Package
*	Copyright (c) Telechips, Inc.
*	ALL RIGHTS RESERVED
*
****************************************************************************************/

//#include <common.h>
#include <debug.h>
#include <err.h>
#include <reg.h>
#include <string.h>
//#include <platform.h>
//#include <def_tcc.h>
#include <i2c.h>
//#include <platform/reg_physical.h>
#include <platform/tcc_ckc.h>

#ifndef _TODO_
//#include <i2c.h>
//#include <clock.h>
#include <asm/arch/fwdn/Disk.h>
#include <asm/arch/fwdn/file.h>
#include <asm/arch/fwdn/FSAPP.h>
#include <sdmmc/sd_memory.h>

int gSDUpgrading = 0;

extern unsigned char pca9539u3_data[2];
extern int FWUG_MainFunc(int hFile, int iFileSize);
extern int VerifyROMFile(unsigned int *pBuffer,unsigned int size );

void sd_deinit(void)
{
	unsigned int i2c_ch = I2C_CH_SMU;
	unsigned char DestAddress;
	unsigned char i2cData_mode[3] = {0,0,0};
	unsigned char i2cData_data[3] = {0,0,0};
	//PPIC pPIC = (PIC *)&HwPIC_BASE;

	/* disable clock & io bus */
	//tcc_set_peri(PERI_SDMMC0, DISABLE, 12000000);
	//tcc_set_iobus_pwdn(IOBUS_SDMMC, 1);

	/* reset peri block */
	//tca_ckc_setioswreset(IOBUS_SDMMC0, 1);
	//tca_ckc_setioswreset(IOBUS_SDMMC0, 0);
	
	/* slot0,1 power down */
	DestAddress = 0xE8;
	i2cData_mode[0] = 6;
	i2cData_data[0] = 2;
	//BITCLR(pca9539u3_data[0], Hw6|Hw7);
	i2cData_data[1] = pca9539u3_data[0];
	i2cData_data[2] = pca9539u3_data[1];
	i2c_xfer(DestAddress, 3, i2cData_mode, 0, 0, i2c_ch);
	i2c_xfer(DestAddress, 3, i2cData_data, 0, 0, i2c_ch);

	/* clear interrupt */
	//BITSET(pPIC->CLR1, Hw12|Hw13);
}
#endif

/************* end of file *************************************************************/
