/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <debug.h>
#include <qgic.h>
#include <qtimer.h>
#include <mmu.h>
#include <arch/arm/mmu.h>
#include <board.h>
#include <boot_stats.h>
#include <lib/heap.h>
#include <reg.h>
#include <plat/cpu.h>
#include <platform/sram_map.h>
#ifdef CONFIG_ARM_TRUSTZONE
#include <platform/smc.h>
#endif

#include <dev/adc.h>
#include <daudio_ver.h>
#include <tcc_lcd.h>

#define ADC_READ_COUNT	5
#define ADC_READ_RETRY	10

#define DMA_START ((unsigned long)&_dma_start)
#define DMA_LEN ((size_t)&_end_of_dma - (size_t)&_dma_start)

extern void clock_init_early(void);
extern void clock_init(void);
extern void timer_init_early(void);
extern void gpio_init_early(void);
extern void i2c_init_early(void);
extern void i2c_init(void);
extern void uart_init_early(void);
extern void uart_init(void);
extern void tcc_extract_chip_revision(void);
extern void platform_init_interrupts(void);
extern void platform_uninit_timer(void);

unsigned int system_rev;
unsigned int __arch_id = TCC897X_ARCH_ID;

#define MB (1024*1024)

/* 64KB Internal SRAM (Shared by Hardware)*/
#define SRAM_BASE		0x10000000
#define SRAM_SIZE		1
#define SRAM_MEMORY		(MMU_MEMORY_TYPE_STRONGLY_ORDERED | \
					MMU_MEMORY_AP_READ_WRITE)

/* LK memory - cacheable, write through */
#define LK_BASE			MEMBASE
#define LK_SIZE			(MEMSIZE/MB)
#define LK_MEMORY		(MMU_MEMORY_TYPE_NORMAL_WRITE_BACK_NO_ALLOCATE | \
					MMU_MEMORY_AP_READ_WRITE)

/* Peripherals - non-shared device */
#define IOMAP_BASE		0x70000000
#define IOMAP_SIZE		(0x10000000/MB)
#define IOMAP_MEMORY		(MMU_MEMORY_TYPE_DEVICE_NON_SHARED | \
					MMU_MEMORY_AP_READ_WRITE)

#ifdef WITH_DMA_ZONE
/* DMA memory - cacheable, write through */
#define DMA_BASE		(((MEMBASE+MEMSIZE+0x100000-1)>>20)<<20)
#define DMA_SIZE		(_DMA_SIZE/MB)
#define DMA_MEMORY		(MMU_MEMORY_TYPE_STRONGLY_ORDERED/*MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH*/ | \
					MMU_MEMORY_AP_READ_WRITE)
#endif

//+[TCCQB] QuickBoot Buffer Scratch Area
#ifdef CONFIG_SNAPSHOT_BOOT
/*	QUICKBOOT SCRATCH memory - cacheable, buffered, write back	*/
#define QB_SCRATCH_BASE		QB_SCRATCH_ADDR
#define QB_SCRATCH_SIZE		2 	/* 	2MB		*/
#define QB_SCRATCH_MEMORY	(MMU_MEMORY_TYPE_NORMAL_WRITE_BACK_NO_ALLOCATE | \
					MMU_MEMORY_AP_READ_WRITE)
#endif
//-[TCCQB]
//

#define SCRATCH_BASE		SCRATCH_ADDR
#define SCRATCH_SIZE		(((TCC_MEM_SIZE*1024*1024UL)-(SCRATCH_ADDR-BASE_ADDR))/MB)
#define SCRATCH_MEMORY		(MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH /* MMU_MEMORY_TYPE_NORMAL_WRITE_BACK_NO_ALLOCATE */ | \
					MMU_MEMORY_AP_READ_WRITE)

#if defined(TSBM_ENABLE)
#define OTP_BASE		0x40000000
#define OTP_SIZE		(0x00100000/MB)
#define OTP_MEMORY		(MMU_MEMORY_TYPE_STRONGLY_ORDERED/*MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH*/ | \
					MMU_MEMORY_AP_READ_WRITE)
#endif


static mmu_section_t mmu_section_table[] = {
	/* Physical addr,	Virtual addr,		Size (in MB),		Flags */
	{ SRAM_BASE,		0x00000000,		SRAM_SIZE,		SRAM_MEMORY	},
	{ SRAM_BASE,		SRAM_BASE,		SRAM_SIZE,		SRAM_MEMORY	},
	{ IOMAP_BASE,		IOMAP_BASE,		IOMAP_SIZE,		IOMAP_MEMORY	},
	{ LK_BASE,		LK_BASE,		LK_SIZE,		LK_MEMORY	},
#ifdef WITH_DMA_ZONE
	{ DMA_BASE,		DMA_BASE,		DMA_SIZE,		DMA_MEMORY	},
#endif
//+[TCCQB] QuickBoot Buffer Scratch Area
#ifdef CONFIG_SNAPSHOT_BOOT
	{ QB_SCRATCH_BASE,	QB_SCRATCH_BASE,	QB_SCRATCH_SIZE,	QB_SCRATCH_MEMORY},
#endif
//-[TCCQB]
//
	{ SCRATCH_BASE,		SCRATCH_BASE,		SCRATCH_SIZE,		SCRATCH_MEMORY	},
#if defined(TSBM_ENABLE)
	{ OTP_BASE,		OTP_BASE,		OTP_SIZE,		OTP_MEMORY	},
#endif
#ifdef CONFIG_ARM_TRUSTZONE
	{ TZ_SECUREOS_BASE,	TZ_SECUREOS_BASE,	((TZ_SECUREOS_SIZE + MB - 1) / MB),	SRAM_MEMORY	},
#endif
};

void init_daudio_adc_check(void)
{
	int i;
	unsigned long adc = 0;

	adc_init_early();

	for (i = 2; i < 10; i++) {
		int retry = ADC_READ_RETRY;
		while (retry-- > 0) {
			adc += adc_read(i);
		}
		adc = adc / ADC_READ_RETRY;
		init_daudio_adc(adc, i);
		adc = 0;
	}
	adc_power_down();
}

void init_daudio_rev(void)
{
	int i;
	int valid;
	unsigned long daudio_board_revs[3] = {0, };
	unsigned long adc = 0;

	adc_init_early();

	for (i = 0; i < DAUDIO_VER_MAX; i++) {          //ADC_AIN[3], ADC_AIN[6], ADC_AIN[9], ADC_AIN[7]
		int retry = ADC_READ_RETRY;
		valid = 0;

		while (retry-- > 0) {
			int count = ADC_READ_COUNT;
			int init_ver;

			if(i == DAUDIO_VER_BT) //DAUDIO_VER_BT : 3
				adc = adc_read(DAUDIO_VER_BT + 4);
			else
				adc = adc_read(3 * (i + 1));

			init_ver = init_version(i, adc);

			if(init_ver < 0) continue;

			while (count-- > 0) {
				adc = adc_read(3 * (i + 1));
				valid = adc_valid_check(adc, i, init_ver);
				if(!valid)	break;
			}

			if(valid)
				break;
		}
		daudio_board_revs[i] = valid ? adc : -1;
	}
	adc_power_down();

	init_daudio_ver(daudio_board_revs);
}

void platform_early_init(void)
{
	system_rev = HW_REV;
	clock_init_early();
	gpio_init_early();
	platform_init_interrupts();
	timer_init_early();
	uart_init_early();
	i2c_init_early();
	pwm_init_early();
	init_daudio_rev();
}

void platform_init(void)
{
	//dprintf(INFO, "platform_init()\n");
	clock_init();
	uart_init();
#if 1	//show DRAM register log
	sdram_test();
#endif
	copy_dram_init();
}

void platform_uninit(void)
{
	platform_uninit_timer();
}

int platform_use_identity_mmu_mappings(void)
{
	/* Use only the mappings specified in this file. */
	return 1;
}

/* Setup memory for this platform */
void platform_init_mmu_mappings(void)
{
	uint32_t i;
	uint32_t sections;
	uint32_t alloc_pmap_size = 0;
	uint32_t table_size = ARRAY_SIZE(mmu_section_table);

	pmap_info_t gPmap[PMAP_MAX];

	/* Configure the MMU page entries for memory read from the
		mmu_section_table */
	for (i = 0; i < table_size; i++) {
		sections = mmu_section_table[i].num_of_sections;

		while (sections--) {
			arm_mmu_map_section(mmu_section_table[i].paddress +
								sections * MB,
								mmu_section_table[i].vaddress +
								sections * MB,
								mmu_section_table[i].flags);
		}
	}

	/* Configure the MMU page entries for memory read from the
	Display area */

	alloc_pmap_size = tc_get_pmap_info(&gPmap);

	{
		uint32_t mem_base, mem_size_before_base_addr;

		mem_base = BASE_ADDR;
		mem_size_before_base_addr = (MEMBASE - BASE_ADDR - alloc_pmap_size);

		sections = mem_size_before_base_addr / MB;
		while (sections--) {
			arm_mmu_map_section(mem_base + sections * MB,
								mem_base + sections * MB, LK_MEMORY);
		}
	}

	if((gPmap[PMAP_FB_WMIXER].size != 0) && (gPmap[PMAP_FB_WMIXER].pbase != 0)) {
		sections = (gPmap[PMAP_FB_WMIXER].size) / MB;
		while (sections--) {
			arm_mmu_map_section(gPmap[PMAP_FB_WMIXER].pbase +
								sections * MB,
								gPmap[PMAP_FB_WMIXER].pbase +
								sections * MB,
								(MMU_MEMORY_TYPE_STRONGLY_ORDERED | MMU_MEMORY_AP_READ_WRITE));
		}
	}

	if((gPmap[PMAP_FB_VIDEO].size != 0) && (gPmap[PMAP_FB_VIDEO].pbase != 0)) {
		sections = (gPmap[PMAP_FB_VIDEO].size) / MB;
		while (sections--) {
			arm_mmu_map_section(gPmap[PMAP_FB_VIDEO].pbase +
								sections * MB,
								gPmap[PMAP_FB_VIDEO].pbase +
								sections * MB,
								(MMU_MEMORY_TYPE_STRONGLY_ORDERED | MMU_MEMORY_AP_READ_WRITE));
		}
	}
}

