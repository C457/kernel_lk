#include <nand_drv_v8.h>

int NAND_GetSerialNumber( unsigned char *pucSN, unsigned int uiSize )
{
	return 0;
}

int NAND_Ioctl( int function, void *param )
{
	return 0;
}

int NAND_ReadPhysicalBlock( unsigned int uiBlockAddress, unsigned int uiReadSize, void *pvBuffer )
{
	return 0;
}

int NAND_WriteFirmware( unsigned int uiDrvNum, unsigned char *pucRomBuffer, unsigned int uiFWSize )
{
	return 0;
}

int NAND_WriteSector(  unsigned int uiDrvNum, unsigned int uiSectorAddress, unsigned int uiSectorCount, void *pvBuffer )
{
	return 0;
}

unsigned int NAND_Area_GetTotalSectorCount( unsigned int uiDrvNum, unsigned int uiAreaIndex )
{
	return 0;
}

int NAND_Area_ReadSector( unsigned int uiDrvNum, unsigned int uiAreaIndex, unsigned int uiSectorAddress, unsigned int uiSectorCount, void *pvBuffer )
{
	return 0;
}

int NAND_Area_WriteSector( unsigned int uiDrvNum, unsigned int uiAreaIndex, unsigned int uiSectorAddress, unsigned int uiSectorCount, void *pvBuffer )
{
	return 0;
}

int NAND_RO_Area_Flush_WriteCache( unsigned int uiDrvNum )
{
	return 0;
}

unsigned int NAND_DumpExtArea( unsigned char *buf )
{
	return 0;
}

unsigned int NAND_Area_GetCount( unsigned int uiDrvNum )
{
	return 0;
}

NAND_ERROR_T NAND_Init( unsigned int uiDrvNum, NAND_INIT_TYPE_T eType, const NAND_ProgressHandler fnProgressHandler )
{
	return NAND_SUCCESS;
}

unsigned int FWUG_CalcCrc8(unsigned char *base, unsigned int length)
{
	return 0;
}

unsigned int FWUG_CalcCrc8I(unsigned uCRCIN, unsigned char *base, unsigned int length)
{
	return 0;
}

