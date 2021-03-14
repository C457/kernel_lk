/*
 * Copyright (c) 2013 Telechips, Inc.
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


//#include<debug.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/irqs.h>
#include <asm/arch/structures_smu_pmu.h>

#ifdef CONFIG_USE_IRQ
#define GIC_CPU_BASE		HwARMPERI_GIC_BASE
#define GIC_DIST_BASE           HwARMPERI_INT_DIST_BASE

#define GIC_CPU_REG(off)	(GIC_CPU_BASE + (off))
#define GIC_DIST_REG(off)	(GIC_DIST_BASE + (off))

#define GIC_CPU_CTRL		GIC_CPU_REG(0x00)
#define GIC_CPU_PRIMASK		GIC_CPU_REG(0x04)
#define GIC_CPU_BINPOINT	GIC_CPU_REG(0x08)
#define GIC_CPU_INTACK		GIC_CPU_REG(0x0c)
#define GIC_CPU_EOI		GIC_CPU_REG(0x10)
#define GIC_CPU_RUNNINGPRI	GIC_CPU_REG(0x14)
#define GIC_CPU_HIGHPRI		GIC_CPU_REG(0x18)

#define GIC_DIST_CTRL		GIC_DIST_REG(0x000)
#define GIC_DIST_CTR		GIC_DIST_REG(0x004)
#define GIC_DIST_ENABLE_SET	GIC_DIST_REG(0x100)
#define GIC_DIST_ENABLE_CLEAR	GIC_DIST_REG(0x180)
#define GIC_DIST_PENDING_SET	GIC_DIST_REG(0x200)
#define GIC_DIST_PENDING_CLEAR	GIC_DIST_REG(0x280)
#define GIC_DIST_ACTIVE_BIT	GIC_DIST_REG(0x300)
#define GIC_DIST_PRI		GIC_DIST_REG(0x400)
#define GIC_DIST_TARGET		GIC_DIST_REG(0x800)
#define GIC_DIST_CONFIG		GIC_DIST_REG(0xc00)
#define GIC_DIST_SOFTINT	GIC_DIST_REG(0xf00)


struct _irq_handler {
	void                *m_data;
	void (*m_func)( void *data);
};


static struct _irq_handler IRQ_HANDLER[NR_IRQS] __attribute__ ((section(".data"))) ;



static void platform_gic_dist_init(void)
{
	int i;
	unsigned gic_irqs = 0;

	/* Disable GIC */
	writel(0x0, GIC_DIST_CTRL);

	/*
	 * Find out how many interrupts are supported.
	 * The GIC only supports up to 1020 interrupt sources.
	 */
	gic_irqs = readl(GIC_DIST_CTR) & 0x1f;
	gic_irqs = (gic_irqs + 1) * 32;
	if (gic_irqs > 1020)
		gic_irqs = 1020;

	/*
	 * Set all global interrupts to be level triggered, active low.
	 */
	for (i = 32; i < gic_irqs; i += 16)
		writel(0/*0xffffffff*/, GIC_DIST_CONFIG + i * 4 / 16);

	/*
	 * Set all global interrupts to this CPU only.
	 */
	for (i = 32; i < gic_irqs; i += 4)
		writel(0x01010101, GIC_DIST_TARGET + i * 4 / 4);

 	/*
	 * Set priority on all global interrupts.
	 */
	for (i = 32; i < gic_irqs; i += 4)
		writel(0xa0a0a0a0, GIC_DIST_PRI + i * 4 / 4);

	/*
	 * Disable all interrupts.  Leave the PPI and SGIs alone
	 * as these enables are banked registers.
	 */
	for (i = 32; i < gic_irqs; i += 32)
		writel(0xffffffff, GIC_DIST_ENABLE_CLEAR + i * 4 / 32);

	//writel(0x0000ffff, GIC_DIST_ENABLE_SET);

	/* Enable GIC */
	writel(0x1, GIC_DIST_CTRL);
}

static void platform_gic_cpu_init(void)
{
	int i;

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
	 * PPI interrupts, ensure all SGI interrupts are enabled.
	 */
	writel(0xffff0000, GIC_DIST_ENABLE_CLEAR);
	writel(0x0000ffff, GIC_DIST_ENABLE_SET);

	/*
	 * Set priority on PPI and SGI interrupts
	 */
	for (i = 0; i < 32; i += 4)
		writel(0xa0a0a0a0, GIC_DIST_PRI + i * 4 / 4);

	writel(0xf0, GIC_CPU_PRIMASK);

	writel(0x01, GIC_CPU_CTRL);
}

int arch_interrupt_init (void)
{
	int i;

	platform_gic_dist_init();
	platform_gic_cpu_init();


	return (0);
}


void do_irq (struct pt_regs *pt_regs)
{
	unsigned irq;
	
	irq= readl(GIC_CPU_INTACK) & 0x3ff;

	if (irq >= NR_IRQS)  return;

	IRQ_HANDLER[irq].m_func(IRQ_HANDLER[irq].m_data);

	/* clear Timer32 */
	writel(((irq + 1) % 32), GIC_DIST_PENDING_CLEAR + (irq / 32) * 4);

	writel(irq, GIC_CPU_EOI);
}

void irq_install_handler (int irq, interrupt_handler_t handle_irq, void *data)
{
	if (irq >= NR_IRQS || !handle_irq)
		return;

	IRQ_HANDLER[irq].m_data = data;
	IRQ_HANDLER[irq].m_func = handle_irq;

	//printf("irq_install_handler irq[0x%x] = 0x%x\r\n", irq, handle_irq);
}

void irq_free_handler (int irq)
{
	if (irq >= NR_IRQS)
		return;
	
	IRQ_HANDLER[irq].m_data = NULL;
	IRQ_HANDLER[irq].m_func = NULL;
}


int irq_mask(unsigned int irq)
{
	unsigned mask = 1 << (irq & 31);

	writel(mask, GIC_DIST_ENABLE_CLEAR + (irq / 32) * 4);
	return 0;
}

int irq_unmask(unsigned int irq)
{
	unsigned mask = 1 << (irq & 31);

	writel(mask, GIC_DIST_ENABLE_SET + (irq / 32) * 4);
	return 0;
}

unsigned int irq_external_select(unsigned int external_irq, unsigned int external_select)
{
	unsigned int  bResult = -1;
	PGPIO pGPIO = (PGPIO)HwGPIO_BASE;
	volatile unsigned int * external_select_reg = NULL;
	unsigned int value, select_offset;
	
	if(external_select < IRQSEL_MAX && external_irq >= IRQ_EI0 && external_irq <= IRQ_EI11)
	{
		switch(external_irq)
		{
			case IRQ_EI0:
			case IRQ_EI1:
			case IRQ_EI2:
			case IRQ_EI3:
				external_select_reg = (volatile unsigned int * )&pGPIO->EINTSEL0;
				select_offset =  ((external_irq - IRQ_EI0) << 3);
				break;
			case IRQ_EI4:
			case IRQ_EI5:
			case IRQ_EI6:
			case IRQ_EI7:
				external_select_reg = (volatile unsigned int * )&pGPIO->EINTSEL1;
				select_offset =  ((external_irq - IRQ_EI4) << 3);
				break;
			case IRQ_EI8:
			case IRQ_EI9:
			case IRQ_EI10:
			case IRQ_EI11:
				external_select_reg = (volatile unsigned int * )&pGPIO->EINTSEL2;
				select_offset =  ((external_irq - IRQ_EI8) << 3);
				break;		
		}

	
		if(external_select_reg)
		{
			value = *external_select_reg;
			BITCSET(value, 0xFF << select_offset, external_select << select_offset);
			*external_select_reg = value;
			
			bResult = TRUE;
		}
	}
	return bResult;
}



unsigned int irq_polarity(unsigned int irq, unsigned int source_is_low_polarity)
{
	unsigned int bResult = -1;
	PPIC pPIC = (PPIC)HwPIC_BASE;

	if(irq >= IRQ_TC0 && irq < IRQ_SD3)
	{
		if(source_is_low_polarity)
		{
			BITSET(pPIC->POL0.nREG, 1 << ((irq - IRQ_TC0)));
		}
		else
		{
			BITCLR(pPIC->POL0.nREG, 1 << ((irq - IRQ_TC0)));
		}
		bResult = 0;
	}
	else if(irq >= IRQ_SD3 && irq < NR_IRQS)
	{	
		if(source_is_low_polarity)
		{
			BITSET(pPIC->POL1.nREG, 1 << ((irq - IRQ_SD3)));
		}
		else
		{
			BITCLR(pPIC->POL1.nREG, 1 << ((irq - IRQ_SD3)));
		}
		bResult = 0;
	}
	
	return bResult;
}


unsigned int irq_set_type(unsigned int irq, unsigned int type)
{
	unsigned int bResult = -1;
	unsigned int irq_enabled = 0;
	unsigned int value, enable_value;

	unsigned int enable_bit = 1 << (irq % 32);
	unsigned int icdiser_index = (irq >> 5);
	unsigned int icdicfr_bit = (2 << ((irq % 16) << 1)); // b'11 : Edge b'01 : Level
	unsigned int icdicfr_index = (irq >> 4);

    /* for TCC893x */
    if (irq > NR_IRQS)
    {
        goto END_PROCESS;
	}

	/* Interrupt configuration for SGIs can't be changed */
	if (irq < 16)
    {
        goto END_PROCESS;
	}

	if (type <= IRQ_TYPE_UNKNOWN && type >= IRQ_TYPE_MAX)
    {
        goto END_PROCESS;
	}
	
	if(type == IRQ_TYPE_LEVEL_LOW || type == IRQ_TYPE_EDGE_FALLING)
		irq_polarity(irq, 1);
	else
		irq_polarity(irq, 0);

	value = readl(GIC_DIST_CONFIG+(icdicfr_index << 2));
	if (type == IRQ_TYPE_LEVEL_HIGH || type == IRQ_TYPE_LEVEL_LOW)
		value &= ~icdicfr_bit;
	else if (type == IRQ_TYPE_EDGE_RISING || type == IRQ_TYPE_EDGE_FALLING)
		value |= icdicfr_bit;

	writel(value, GIC_DIST_CONFIG+(icdicfr_index << 2));
	
	 // As recommended by the spec, disable the interrupt before changing the configuration
	enable_value = readl(GIC_DIST_ENABLE_SET+(icdiser_index << 2));
	if (enable_value & enable_bit) 
	{
		writel(enable_value, GIC_DIST_ENABLE_CLEAR+(icdiser_index << 2));
		irq_enabled = 1;
	}	
	if (irq_enabled)
		writel(enable_value, GIC_DIST_ENABLE_SET+(icdiser_index << 2));

	bResult = 0;
	
END_PROCESS:
	return bResult;
}


#if defined(CONFIG_IRQ_GPIO_TEST)

#include <asm/gpio.h>
#define TEST_GPIO TCC_GPB(29)
#define TEST_ERQSEL IRQSEL_GPIO_B_29
static void button_handler(void *arg)
{
	printf("button_handler BUTTON=%d\r\n", gpio_get_value(TEST_GPIO));
}

void gpio_irq_type(unsigned int type)
{
	gpio_config(TEST_GPIO, GPIO_FN0|GPIO_INPUT|GPIO_PULLUP);
	
	irq_external_select(IRQ_EI2, TEST_ERQSEL);
	irq_set_type(IRQ_EI2, type);

	irq_install_handler(IRQ_EI2, &button_handler, NULL);
	irq_unmask(IRQ_EI2);

	#if defined(CONFIG_IRQ_LOOP_TEST)
	while(1) ;
	
	irq_mask(IRQ_EI2);
	irq_free_handler(IRQ_EI2);
	#endif
}

void gpio_irq_test(void)
{
	printf("Start GPIO TEST\r\n");
	enable_interrupts();
	//gpio_irq_type(IRQ_TYPE_LEVEL_HIGH);
	//gpio_irq_type(IRQ_TYPE_LEVEL_LOW);
	//gpio_irq_type(IRQ_TYPE_EDGE_RISING);
	gpio_irq_type(IRQ_TYPE_EDGE_FALLING);
	#if defined(CONFIG_IRQ_LOOP_TEST)
	disable_interrupts();
	#endif
}
#endif
#endif
