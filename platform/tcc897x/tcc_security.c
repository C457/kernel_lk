#include <arch/ops.h>
#include <platform/iomap.h>
#include <debug.h>
#include <string.h>

#define TCSB_BASE       0x18000000
#define TCSB_SIZE       0x20000

#define BOOT_MODE       *((unsigned int*)0x40000000)

#define BUILD_DATE      *((unsigned int*)(TCSB_BASE + 0x20))

#define TZOW_API ((int (*)(unsigned int, unsigned char*, unsigned int, void*))(TCSB_BASE))

///////////////////////////////////////////////////////////////////////////////////////////////////
// Secure Boot
///////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(TSBM_ENABLE)
bool target_use_signed_kernel(void)
{
#if 0
	if (BOOT_MODE != 0)
		return 1;
#endif
	return 0;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// TZOW
///////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(TZOW_INCLUDE)
#if !defined(OTP_UID_INCLUDE)
static int TZOW_PREPARED = 0;
#endif

void tzow_init(unsigned char *oBuff, unsigned int oSize)
{
#if !defined(OTP_UID_INCLUDE)
	if (TZOW_PREPARED == 0)
	{
		memcpy((void*)TCSB_BASE, oBuff, oSize);
		TZOW_PREPARED = 1;
		dprintf(INFO, "[OTP] v%x\n", BUILD_DATE);
	}
#endif
}

int tzow_run(unsigned char* oBuff, unsigned int oSize)
{
	volatile unsigned int *CMB_CKC = ((volatile unsigned int*)(TCC_CKC_BASE+0x2C));
	unsigned int ulTempCKC = *CMB_CKC;
	int iRet = 0;

#if !defined(OTP_UID_INCLUDE)
	if (TZOW_PREPARED == 1)
#endif
	{
		// ENABLE CMBUS CLOCK
		if ((ulTempCKC & (1 << 21)) == 0)
		{
			dprintf(INFO, "[OTP] CM Bus Clock Enable\n");
			*CMB_CKC |= (1 << 21);
			while (*CMB_CKC & (1 << 29));
		}

#if defined(OTP_UID_INCLUDE)
		iRet = tzow_api(0, oBuff, oSize, (void*)printf);
		dprintf(INFO, "[OTP] Finish (ERR=%d)\n", iRet);
#else
		typedef int (*pFunc)(unsigned int nMHz, unsigned char *pucData, unsigned int owsize, void* dbg);
		dprintf(INFO, "[OTP] Start OTP Write\n");
		iRet = TZOW_API(0, oBuff, oSize, (void*)printf);
		dprintf(INFO, "[OTP] Finish (SKIP=%d, ERR=%d)\n", (iRet >> 8) & 0xff, iRet & 0xff);
#endif

		// DISABLE CMBUS CLOCK
		if ((ulTempCKC & (1 << 21)) == 0)
		{
			dprintf(INFO, "[OTP] CM Bus Clock Disable\n");
			*CMB_CKC = ulTempCKC;
		}
	}
	return iRet & 0xff;
}

void tzow_deinit(void)
{
#if !defined(OTP_UID_INCLUDE)
	if (TZOW_PREPARED == 1)
	{
		dprintf(INFO, "[OTP] TZOW Deinit\n");
		memset((void*)TCSB_BASE, 0x0, TCSB_SIZE); // Clear SRAM1
		TZOW_PREPARED = 0;
	}
#endif
}
#endif

