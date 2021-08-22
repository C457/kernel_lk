/****************************************************************************
 *   FileName    :disk.c
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#include <asm/arch/fwdn/Disk.h>

/*****************************************************************************
 *	disk device name string array
 *	the order should be matched with DISK_DEVICE type.
 *****************************************************************************/
#ifndef NULL
#define NULL		0
#endif

static struct device_driver_ops *DiskDriver = NULL;

int DISK_FindDisk(int drv_type)
{
	if (!DiskDriver)
		return -1;

	if (DiskDriver->DiskType == (unsigned int)drv_type)
		return 0;

	return -1;
}

int DISK_ReadSector(int drv_type, int lun, unsigned long lba_addr,
			unsigned long nSector, void *buff)
{
	int res;

	res = DISK_FindDisk(drv_type);
	if (res < 0)
		return res;

	if (DiskDriver->ReadSector) {
		while (DiskDriver->LockFlag == DISK_DEVICE_LOCK);

		DiskDriver->LockFlag = DISK_DEVICE_LOCK;
		res = DiskDriver->ReadSector(lun, lba_addr, nSector, buff);
		DiskDriver->LockFlag = DISK_DEVICE_UNLOCK;

		return res;
	}

	return -1;
}

int DISK_WriteSector(int drv_type, int lun, unsigned long lba_addr,
			unsigned long nSector, void *buff)
{
	int res;

	res = DISK_FindDisk(drv_type);
	if (res < 0)
		return res;

	if ( DiskDriver->WriteSector != NULL )
	{
		while (DiskDriver->LockFlag == DISK_DEVICE_LOCK);

		DiskDriver->LockFlag = DISK_DEVICE_LOCK;
		res = DiskDriver->WriteSector(lun, lba_addr, nSector, buff);
		DiskDriver->LockFlag = DISK_DEVICE_UNLOCK;
		return res;
	}

	return -1;
}

int DISK_Ioctl(unsigned int drv_type, int function, void *param)
{
	int		res;

	res = DISK_FindDisk(drv_type);
	if (res < 0)
		return res;

	if (DiskDriver->Ioctl != NULL) {
		if ((function != DEV_GET_POWER) && (function != DEV_GET_INSERTED)) {
			while (DiskDriver->LockFlag == DISK_DEVICE_LOCK);
			DiskDriver->LockFlag = DISK_DEVICE_LOCK;
		}
		res = DiskDriver->Ioctl(function, param);
		if ((function != DEV_GET_POWER) && (function != DEV_GET_INSERTED))
			DiskDriver->LockFlag = DISK_DEVICE_UNLOCK;

		return res;
	}

	return -1;
}

int DISK_Register(struct device_driver_ops *disk)
{
	if (DiskDriver) {
		return -1;
	}
	DiskDriver = disk;
	return 0;
}
