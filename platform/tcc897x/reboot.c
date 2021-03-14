/*
 * Copyright (C) 2010 Telechips, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <reg.h>
#include <platform/iomap.h>
#include <debug.h>

#include "../../../app/aboot/recovery.h"
#include <daudio_settings.h>

#define IOBUSCFG_HCLKEN0        (TCC_IOBUSCFG_BASE + 0x10)
#define IOBUSCFG_HCLKEN1        (TCC_IOBUSCFG_BASE + 0x14)
#define PMU_WATCHDOG            (TCC_PMU_BASE + 0x08)
#define PMU_CONFIG1             (TCC_PMU_BASE + 0x14)
#define PMU_USSTATUS            (TCC_PMU_BASE + 0x1C)

void reboot(unsigned reboot_reason)
{
	unsigned int usts;

	if (reboot_reason == 0x77665500) {
		usts = 1;
	} else {
		usts = 0;
	}

	writel(usts, PMU_USSTATUS);

	writel((readl(PMU_CONFIG1) & 0xCFFFFFFF), PMU_CONFIG1);

	writel(0xFFFFFFFF, IOBUSCFG_HCLKEN0);
	writel(0xFFFFFFFF, IOBUSCFG_HCLKEN1);

	while (1) {
		writel((1 << 31) | 1, PMU_WATCHDOG);
	}
}

unsigned check_active_partition(void)
{
	static bool sbSet_=false;
	unsigned int mode = 0;				// default value

	if( false==sbSet_)
	{
		struct boot_partition active_p;

		if( read_active_partition_setting(&active_p) == 1 )
		{
			active_p.active_partition[sizeof(active_p.active_partition)-1]=0;

			if( 0==strcmp("boot", active_p.active_partition) )
			{
				mode = 0;
				dprintf(INFO, "read_active_partition_setting active_partition boot\n");
			}
			else if( 0==strcmp("boot_mirror", active_p.active_partition) )
			{
				mode = 1;
				dprintf(INFO, "read_active_partition_setting active_partition boot_mirror\n");
			}
			else
			{
				mode = 0;
				dprintf(INFO, "read_active_partition_setting NOT SET!!! active_partition boot\n");
			}
		}
		else
		{
			dprintf(INFO, "[fail]check_active_partition\n");
		}

		sbSet_=true;
	}
	return mode;
}

unsigned check_reboot_mode(void)
{
	static bool sbSet=false;
	unsigned int usts;
	unsigned int mode;

	if( false==sbSet)
	{
		struct recovery_message Msg;

		if( get_recovery_message(&Msg) == 0 )
		{
			Msg.command[sizeof(Msg.command)-1]=0;

			if( 0==strcmp("boot-bootloader", Msg.command) )
			{
				writel(1, PMU_USSTATUS);
				memset(&Msg, 0, sizeof(Msg));
				if( 0!=get_recovery_message(&Msg) ) // clear message.
					dprintf(INFO, "[fail]emmc_set_recovery_msg\n");
			}
			else if( 0==strcmp("boot-recovery", Msg.command) )
			{
				writel(2, PMU_USSTATUS);
			}
			else if( 0==strcmp("boot-force_normal", Msg.command) )
			{
				writel(3, PMU_USSTATUS);
			}
			else
			{
				writel(0, PMU_USSTATUS);
			}

		}
		else
		{
			dprintf(INFO, "[fail]emmc_get_recovery_msg\n");
		}

		sbSet=true;
	}
	/* XXX: convert reboot mode value because USTS register
	 * hold only 8-bit value
	 */
	usts = readl(PMU_USSTATUS);
	usts &= 0xF;

	printf("usts(0x%08X)\n", usts);
	switch (usts) {
	case 1:
		mode = 0x77665500;	/* fastboot mode */
		break;

	case 2:
		mode = 0x77665502;	/* recovery mode */
		break;

//+[TCCQB] For QuickBoot Booting Option
	case 3:
		mode = 0x77665503;      /* force normal mode : skip quickboot */
		break;
//-[TCCQB]
//

	default:
		mode = 0x77665501;
		break;
	}

	dprintf(SPEW, "reboot mode = 0x%x\n", mode);
	return mode;
}

unsigned is_reboot_mode(void)
{
	return check_reboot_mode();
}

//+[TCCQB] Send QB LK Boot Time to kernel
void set_lk_boottime(unsigned int usts)
{
	/*       Send LK Total Time to Kernel    
	 *       usts : mili second time		*/
	
	writel(usts, PMU_USSTATUS);
}
//-[TCCQB]
//

unsigned get_restart_reason(void)
{
	    return readl(PMU_USSTATUS);
}
