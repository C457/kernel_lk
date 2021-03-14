
#include <config.h>

#include <dev/gpio.h>
#include <platform/gpio.h>
#include <platform/iomap.h>
#include <platform/reg_physical.h>
#include <platform/adc.h>
#include <stdlib.h>
#include <reg.h>
#include <platform/tcc_ckc.h>
//#include <platform/globals.h>

#ifndef BITSET
#define BITSET(X, MASK)			((X) |= (unsigned int)(MASK))
#endif
#ifndef BITCSET
#define BITCSET(X, CMASK, SMASK)	((X) = ((((unsigned int)(X)) & ~((unsigned int)(CMASK))) | ((unsigned int)(SMASK))) )
#endif
#ifndef BITCLR
#define BITCLR(X, MASK)			((X) &= ~((unsigned int)(MASK)) )
#endif

#define adc_writel	writel
#define adc_readl	readl
#define PMU_TSADC   0x0c
#define PMU_ISOL    0x9c

//#define ADC_DELAY			HwADCDLY_SEL
#define MASK_ADCDATA10(n)   ( (n)&0x3FF )
#define MASK_ADCDATA12(n)   ( (n)&0xFFF )


volatile PTSADC pTSADC;
unsigned short uADC_DATA[8];

#define ASM_NOP {					\
	__asm__ __volatile__ ("nop");	\
	__asm__ __volatile__ ("nop");	\
	__asm__ __volatile__ ("nop");	\
	__asm__ __volatile__ ("nop");	\
	__asm__ __volatile__ ("nop");	\
	__asm__ __volatile__ ("nop");	\
}
static void adc_delay(int delay)
{
	int i;
	for (i=0 ; i<delay ; i++)
		ASM_NOP;
}

static unsigned int adc_powerdown(void)
{
	unsigned int ret = 0;

//    pTSADC->CLR_INT_EOC.bREG.CLR = Hw0;
    pTSADC->ADCUPDN.bREG.DOWN = Hw0;
    pTSADC->ADCTSC.nREG |= ADCTSC_WAIT_PENDOWN;

    pTSADC->ADCCON.bREG.STBY = 1;

	return ret;
}

unsigned int adc_power_down(void)
{
	return adc_powerdown();
}
EXPORT_SYMBOL(adc_power_down);

static unsigned int adc_powerup(void)
{
	unsigned int ret = 0;

	pTSADC->ADCCON.bREG.STBY = 0;

	return ret;
}

unsigned int adc_power_up(void)
{
	return adc_powerup();
}
EXPORT_SYMBOL(adc_power_up);

unsigned long adc_read(int channel);
void adc_init_early(void)
{
	unsigned int reg;
	
	pTSADC	= (PTSADC) TCC_TSADC_BASE;
	//printf("adc_init_early\n");

	tcc_set_peri(PERI_ADC, ENABLE, 1200000); //set clock

	reg |= (0x1<<16); //pPMU->PMU_TSADC.bREG.PWREN = 1;
	reg &= ~(0x1<<0); //pPMU->PMU_TSADC.bREG.STOP = 0;
	adc_writel(reg, (TCC_PMU_BASE+PMU_TSADC));
	adc_writel(reg & ~(0x1<<7), (TCC_PMU_BASE+PMU_ISOL)); //pPMU->PMU_ISOL.bREG.TSADC


   	pTSADC->ADCDLY.nREG = ADC_DELAY(50);
    pTSADC->ADCCON.nREG = RESSEL_12BIT | PRESCALER_EN | PRESCALER_VAL(11)|Hw2;
#if defined(TCC_ADC_NO_TOUCHSCREEN)
    pTSADC->ADCTSC.nREG = ADCTSC_SET_GENERAL; //set touch channel to use general purpose 
#else
    pTSADC->ADCTSC.nREG = ADCTSC_WAIT_PENDOWN;
#endif
    pTSADC->CLR_INT_EOC.nREG = CLEAR_ADC_INT;
    pTSADC->ADCUPDN.nREG = CLEAR_ADCWK_INT;
	//printf("[ADC] ADCTSC = 0x%x\n",  pTSADC->ADCTSC.nREG);
	//BITCLR(HwIOBUSCFG->HCLKEN0, HwIOBUSCFG_TSADC);
	adc_powerup();
}

void adc_init(void)
{
}

#include <debug.h>

unsigned long adc_read(int channel)
{
	unsigned long	ret = 0;
	unsigned int	uiCh = 0;

	int test_routine;

    switch(channel)
    {
#if 0 //Ground
        case ADC_CHANNEL0:
            uiCh = SEL_MUX_AIN0;
            break;

        case ADC_CHANNEL1:
            uiCh = SEL_MUX_AIN1;
            break;
#endif

        case ADC_CHANNEL2:
            uiCh = SEL_MUX_AIN2;
            break;

        case ADC_CHANNEL3:
            uiCh = SEL_MUX_AIN3;
            break;

        case ADC_CHANNEL4:
            uiCh = SEL_MUX_AIN4;
            break;

        case ADC_CHANNEL5:
            uiCh = SEL_MUX_AIN5;
            break;

#if defined(TCC_ADC_NO_TOUCHSCREEN)
        case ADC_CHANNEL6:
            uiCh = SEL_MUX_YM;
            break;

        case ADC_CHANNEL7:
            uiCh = SEL_MUX_YP;
            break;

        case ADC_CHANNEL8:
            uiCh = SEL_MUX_XM;
            break;

        case ADC_CHANNEL9:
            uiCh = SEL_MUX_XP;
            break;
#endif
    }

	BITCLR(pTSADC->ADCCON.nREG, Hw2);//wakeup


#if defined(TCC_ADC_NO_TOUCHSCREEN)
	if(channel <= ADC_CHANNEL9)
#else
	if(channel <= ADC_CHANNEL5)
#endif
	{
		BITCLR(pTSADC->ADCCON.nREG ,SEL_MUX_MASK );
		adc_delay(1);
	}


	BITSET(pTSADC->ADCCON.nREG ,(uiCh|ENABLE_START_EN) );
    while (pTSADC->ADCCON.nREG & ENABLE_START_EN)
    {	// Wait for Start Bit Cleared
		adc_delay(1);
    }
    while (!(pTSADC->ADCCON.nREG & ECFLG_END))
    {	// Wait for ADC Conversion Ended
		adc_delay(1);
   }

    ret = MASK_ADCDATA12( (pTSADC->ADCDAT0.nREG) );

    //BITCSET(pTSADC->ADCCON.nREG, SEL_MUX_MASK ,ENABLE_START_EN);
    BITCLR(pTSADC->ADCCON.nREG, ENABLE_START_EN);
    BITSET(pTSADC->ADCCON.nREG, Hw2);

    return ret;
}

