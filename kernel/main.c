/*
 * Copyright (c) 2008 Travis Geiselbrecht
 *
 * Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <compiler.h>
#include <debug.h>
#include <string.h>
#include <app.h>
#include <arch.h>
#include <platform.h>
#include <target.h>
#include <lib/heap.h>
#include <kernel/thread.h>
#include <kernel/timer.h>
#include <kernel/dpc.h>
#include <boot_stats.h>
#include <platform/gpio.h>

extern void *__ctor_list;
extern void *__ctor_end;
extern int __bss_start;
extern int _end;

#include <daudio_ver.h>

#define LCDC_NUM       1

static char *daudio_hw_versions[] =
{
	"DAUDIOKK_HW_1ST",
	"DAUDIOKK_HW_2ND",
	"DAUDIOKK_HW_3RD",
	"DAUDIOKK_HW_4TH",
	"DAUDIOKK_HW_5TH",
	"DAUDIOKK_HW_6TH",
	"DAUDIOKK_HW_7TH",
	"DAUDIOKK_HW_8TH",
	"DAUDIOKK_HW_9TH",
};


//see also platform/tcc893x/include/daudio_ver.h - typedef enum daudio_ver_data
static char *daudio_board_versions[] =
{

	"DAUDIOKK_PLATFORM_WS4",
	"DAUDIOKK_PLATFORM_WS5",
	"DAUDIOKK_PLATFORM_WS3",
	"DAUDIOKK_PLATFORM_WS6",
	"DAUDIOKK_PLATFORM_WS7",
	"DAUDIOKK_PLATFORM_WS8",
	"DAUDIOKK_PLATFORM_WS9",
	"DAUDIOKK_PLATFORM_WS10",
	"DAUDIOKK_PLATFORM_WS11",
};

//see also platform/tcc893x/include/daudio_ver.h - typedef enum daudio_ver_bt
static char *daudio_bt_versions[] =
{
	"DAUDIOKK_BT_VER_1",
	"DAUDIOKK_BT_VER_2",
	"DAUDIOKK_BT_VER_3",
	"DAUDIOKK_BT_VER_4",
	"DAUDIOKK_BT_VER_5",
	"DAUDIOKK_BT_VER_6",
	"DAUDIOKK_BT_VER_7",
	"DAUDIOKK_BT_VER_8",
	"DAUDIOKK_BT_VER_9",
	"DAUDIOKK_BT_RESERVED",
};

static char *daudio_lcd_versions_oe_int[11] =
{
	"DAUDIOKK_LCD_OI_10_25_1920_720_INCELL_Si",
	"DAUDIOKK_LCD_OI_10_25_1920_720_OGS_TEMP",
	"DAUDIOKK_LCD_OI_RESERVED1",
	"DAUDIOKK_LCD_OI_RESERVED2",
	"DAUDIOKK_LCD_OI_RESERVED3",
	"DAUDIOKK_LCD_OI_RESERVED4",
	"DAUDIOKK_LCD_OI_RESERVED5",
	"DAUDIOKK_LCD_OI_10_25_1920_720_INCELL_LTPS",
	"DAUDIOKK_LCD_OI_08_00_1280_720_OGS_Si",
	"DAUDIOKK_LCD_OI_RESERVED6",
	"DAUDIOKK_LCD_OI_DISCONNECTED",
};

static char *daudio_lcd_versions_pio_int[11] =
{
	"DAUDIOKK_LCD_PI_RESERVED1",
	"DAUDIOKK_LCD_PI_RESERVED2",
	"DAUDIOKK_LCD_PI_RESERVED3",
	"DAUDIOKK_LCD_PI_RESERVED4",
	"DAUDIOKK_LCD_PI_RESERVED5",
	"DAUDIOKK_LCD_PI_10_25_1920_720_PIO_AUO",
	"DAUDIOKK_LCD_PI_08_00_800_400_PIO_TRULY",
	"DAUDIOKK_LCD_PI_RESERVED6",
	"DAUDIOKK_LCD_PI_RESERVED7",
	"DAUDIOKK_LCD_PI_RESERVED8",
	"DAUDIOKK_LCD_PI_DISCONNECTED",
};


static char *daudio_lcd_versions_oe_de[11] =
{
	"DAUDIOKK_LCD_OD_RESERVED1",
	"DAUDIOKK_LCD_OD_RESERVED2",
	"DAUDIOKK_LCD_OD_RESERVED3",
	"DAUDIOKK_LCD_OD_10_25_1920_720_INCELL_Si",
	"DAUDIOKK_LCD_OD_12_30_1920_720_INCELL_Si",
	"DAUDIOKK_LCD_OD_10_25_1920_720_INCELL_LTPS",
	"DAUDIOKK_LCD_OD_08_00_1280_720_OGS_Si",
	"DAUDIOKK_LCD_OD_RESERVED4",
	"DAUDIOKK_LCD_OD_RESERVED5",
	"DAUDIOKK_LCD_OD_RESERVED6",
	"DAUDIOKK_LCD_OD_DISCONNECTED",
};

static char *daudio_lcd_versions_pio_de[11] =
{
	"DAUDIOKK_LCD_PD_RESERVED1",
	"DAUDIOKK_LCD_PD_RESERVED2",
	"DAUDIOKK_LCD_PD_RESERVED3",
	"DAUDIOKK_LCD_PD_RESERVED4",
	"DAUDIOKK_LCD_PD_RESERVED5",
	"DAUDIOKK_LCD_PD_RESERVED6",
	"DAUDIOKK_LCD_PD_RESERVED7",
	"DAUDIOKK_LCD_PD_RESERVED8",
	"DAUDIOKK_LCD_PD_RESERVED9",
	"DAUDIOKK_LCD_PD_RESERVED10",
	"DAUDIOKK_LCD_PD_DISCONNECTED",
};


extern unsigned long daudio_borad_revs[3];
static int bootstrap2(void *arg);
static char **daudio_lcd_versions;

static int lcd_version_check(int oem, int mon)
{
	if((oem == 1) && (mon == 1))
		return daudio_lcd_versions_oe_int;
	else if((oem == 0) && (mon == 1))
		return daudio_lcd_versions_pio_int;
	else if((oem == 1) && (mon == 0))
		return daudio_lcd_versions_oe_de;
	else if((oem == 0) && (mon == 0))
		return daudio_lcd_versions_pio_de;
}

static void call_constructors(void)
{
	void **ctor;

	ctor = &__ctor_list;
	while(ctor != &__ctor_end) {
		void (*func)(void);

		func = (void (*)())*ctor;

		func();
		ctor++;
	}
}

#ifdef CONFIG_ARM_TRUSTZONE
int lk_in_normalworld = 0;  // lk runs on SW when FWDN mode.
void _get_ns(void)
{
	unsigned int cpsr = 0, nsacr = 0;
	__asm__ volatile(
			"mrc	p15, 0, %[nsacr], cr1, cr1, 2\n"
			"mrs %[cpsr], cpsr\n" :
			[nsacr]"=r" (nsacr),
			[cpsr]"=r"(cpsr));
	if (nsacr == 0 || ((cpsr & 0x1F) == 0b10110))
		lk_in_normalworld = 0;
	else
		lk_in_normalworld = 1;
}

#define TZOS_MAGIC_1	0x21458E6A
#define TZOS_MAGIC_2	0x94C6289B
#define TZOS_MAGIC_3	0xFA89ED03
#define TZOS_MAGIC_4	0x9968728F
#define TZOS_MAGIC_M	0xA372B85C

unsigned int tzos_magic[4] =
{TZOS_MAGIC_1, TZOS_MAGIC_2, TZOS_MAGIC_3, TZOS_MAGIC_4};

extern void _secure_vector(void);
unsigned int tzos_load(void)
{
	if((tzos_magic[0] == TZOS_MAGIC_M)&&(check_fwdn_mode()==0))
	{
		memcpy((void *)tzos_magic[1], (void *)(MEMBASE+tzos_magic[2]), tzos_magic[3]);
		return tzos_magic[1];
	}
	else
		return 0;
}
#endif

/* called from crt0.S */
void kmain(void) __NO_RETURN __EXTERNALLY_VISIBLE;
void kmain(void)
{
	long lcd_rev_voltage[3] = {0, };
	unsigned char daudio_vers[DAUDIO_VER_MAX] = {0, };
	int i;
	int oem, mon;


#ifdef CONFIG_ARM_TRUSTZONE
	_get_ns();
#endif

	// get us into some sort of thread context
	thread_init_early();

	// early arch stuff
	arch_early_init();

	// do any super early platform initialization
	platform_early_init();

	// do any super early target initialization
	target_early_init();
	
	oem = gpio_get(TCC_GPB(24));
	mon = gpio_get(TCC_GPB(13));

	daudio_lcd_versions = lcd_version_check(oem,mon);
	dprintf(INFO, "***** BOOTLOADER BSP VERSION: %d *****\n", BSP_LK_VERSION);

	daudio_vers[DAUDIO_VER_HW] = get_daudio_hw_ver();
	daudio_vers[DAUDIO_VER_MAIN] = get_daudio_main_ver();
	daudio_vers[DAUDIO_VER_BT] = get_daudio_bt_ver();
	daudio_vers[DAUDIO_VER_LCD] = get_daudio_lcd_ver();
	dprintf(INFO, "***** D-AudioKK HW ver: %s, adc: %d mV, Platform ver : %s, adc %d mV, OEM : %d MON : %d LCD Ver : %s, adc %d mV BT VER : %s, adc %d mv*****\n",
			daudio_hw_versions[daudio_vers[DAUDIO_VER_HW]-1], get_daudio_hw_adc(), daudio_board_versions[daudio_vers[DAUDIO_VER_MAIN]-1], get_daudio_main_adc(), oem, mon, daudio_lcd_versions[daudio_vers[DAUDIO_VER_LCD]], get_daudio_lcd_adc(),daudio_bt_versions[daudio_vers[DAUDIO_VER_BT]], get_daudio_bt_adc());

#ifdef CONFIG_ARM_TRUSTZONE
	dprintf(INFO, "welcome to lk (ns = %d)!\n\n", lk_in_normalworld);
#else
	//	dprintf(INFO, "welcome to lk\n\n");
#endif
	bs_set_timestamp(BS_BL_START);

	// deal with any static constructors
	//	dprintf(SPEW, "calling constructors\n");
	call_constructors();

	// bring up the kernel heap
	//	dprintf(SPEW, "initializing heap\n");
	heap_init();

	// initialize the threading system
	//	dprintf(SPEW, "initializing threads\n");
	thread_init();

	// initialize the dpc system
	//	dprintf(SPEW, "initializing dpc\n");
	dpc_init();

	// initialize kernel timers
	//	dprintf(SPEW, "initializing timers\n");
	timer_init();

	// create a thread to complete system initialization
	//	dprintf(SPEW, "creating bootstrap completion thread\n");
	thread_resume(thread_create("bootstrap2", &bootstrap2, NULL, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE));

	// enable interrupts
	exit_critical_section();

	// become the idle thread
	thread_become_idle();
}

int main(void);

static int bootstrap2(void *arg)
{
	unsigned char c = 0;
	//	dprintf(SPEW, "top of bootstrap2()\n");

	arch_init();

	// XXX put this somewhere else

	// initialize the rest of the platform
	platform_init();

	// initialize the target
	target_init();
	apps_init();

	return 0;
}

#ifdef OTP_UID_INCLUDE
#include "otp_uid_include.h"
void copy_secure_bootloader(void)
{
	memcpy((void*)TARGET_ADDR, (void*)(MEMBASE+NORMAL_LK_SIZE), SECURE_LK_SIZE);
}
#endif
