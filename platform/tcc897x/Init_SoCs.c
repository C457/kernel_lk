#include <reg.h>
#include <platform/iomap.h>
#include <platform/sram_map.h>

extern void *memcpy(void *dest, const void *src, size_t count);
extern void SRAM_Boot(void);
extern void mon_vector(void);

typedef void (*FuncPtr)(void);

static void local_coherency(int en)
{
	unsigned int val;
	__asm__ volatile ("mrc  p15, 0, %0, c1, c0, 1" : "=r" (val));
	if (en)
		val |= (1<<6);	/* ACTLR.SMP=1 */
	else
		val &= ~(1<<6);	/* ACTLR.SMP=0 */
	__asm__ volatile ("mcr  p15, 0, %0, c1, c0, 1" :: "r" (val));
}

void Init_SoCs(void)
{
	void __iomem *pmu_reg = (void __iomem *)TCC_PMU_BASE;
	void __iomem *sram_reg = (void __iomem *)SRAM_BOOT_ADDR;
	FuncPtr pFunc = (FuncPtr)(RESET_ARM_TIEMR_ADDR);

	/* Clear Type 2 Indicator for remap */
	unsigned type2 = readl(pmu_reg+0x14/*PMU_CONFIG*/) & ~(1<<25 /*type2 Indicator*/);
	writel(type2, pmu_reg+0x14 /*PMU_CONFIG*/);
	/* Clear Type2 Indicator end*/

	unsigned val = readl(pmu_reg+0x14/*PMU_CONFIG*/) & ~(3<<28);
	/* REMAP 0x00000000 to SRAM */
	writel(val|(1<<28), pmu_reg+0x14/*PMU_CONFIG*/);

	/* copy sram_boot & reset_arm_timer codes */
	memcpy(sram_reg, (const void *)SRAM_Boot, SRAM_BOOT_SIZE);
	memcpy((void *)RESET_ARM_TIEMR_ADDR, (const void *)mon_vector, RESET_ARM_TIEMR_SIZE);

	/* if boot by T32 Debugger then return */
	if ((readl(sram_reg+0x8000) == 0x32123321) || check_fwdn_mode()){
		local_coherency(1);
		return;
	}

	pFunc();

	local_coherency(1);
}

