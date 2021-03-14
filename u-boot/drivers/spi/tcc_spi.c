#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <spi.h>
#include <asm/gpio.h>
#include <asm/arch/tcc_ckc.h>
#include <asm/arch/reg_physical.h>

#define TCC_CS_NUM					1	// Number of Chip Selection
#define TCC_GPSB_BUS_NUM			3	// Number of Bus (or Channel)
#define TCC_GPSB_BASE_OFFSET		(0x10000)
#define TCC_GPSB_PERI_CLK			3000000
#define TCC_GPSB_READ_DIV			7
#define TCC_GPSB_BIT_WIDTH			8

#define SNOR_USE_FAST_READ

//#define SNOR_GPIO	// Not use GPSB: Max Clock < 2MHz
#define SNOR_GPIO_CS		// CS Control GPIO

#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
#if defined(CONFIG_TCC896X)
	#define PORT_CLK	Hw20
	#define PORT_CS		Hw18
	#define PORT_SI		Hw17
	#define PORT_SO		Hw19
#elif defined(CONFIG_TCC897X)
	#if defined(TCC897X_GPA)
	#define PORT_CLK	Hw13
	#define PORT_CS 	Hw14
	#define PORT_SI 	Hw15
	#define PORT_SO 	Hw16
	#else
	#define PORT_CLK	Hw8
	#define PORT_CS 	Hw9
	#define PORT_SI 	Hw7
	#define PORT_SO 	Hw10
	#endif
#else
	#define PORT_CLK	Hw12
	#define PORT_CS		Hw13
	#define PORT_SI		Hw15
	#define PORT_SO		Hw14
#endif
	//volatile unsigned int *SPI_PORT_DATA_REG = NULL;
#endif

struct tcc_spi_slave {
	struct spi_slave slave;
	int base;
	int peri_clk;
	int gpsb_peri_id;
	PGPIO HwGPIO;
	PGPSB HwGPSB;
	PGPSBPORTCFG HwGPSBPORTCFG;
	volatile unsigned int *SPI_PORT_DATA_REG;
};

#define to_tcc_spi_slave(s) container_of(s, struct tcc_spi_slave, slave)

volatile int tcc_spi_snor_nop_count = 0;
#define _ASM_NOP_ { tcc_spi_snor_nop_count++; }
void tcc_spi_delay_us(unsigned int us)
{
	int i;
	while(us--)
	{
		for(i=0 ; i<40; i++)
			_ASM_NOP_ 
	}
}

__attribute__((weak))
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	// Does not support multi bus(channel) and CS.
	return bus < TCC_GPSB_BUS_NUM && cs < TCC_CS_NUM;
}

__attribute__((weak))
void spi_cs_activate(struct spi_slave *slave)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
	BITCLR((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CS);
	tcc_spi_delay_us(1);
#endif
}

__attribute__((weak))
void spi_cs_deactivate(struct spi_slave *slave)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
#if defined(SNOR_GPIO) || defined(SNOR_GPIO_CS)
	BITSET((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CS);
	tcc_spi_delay_us(1);
#endif
}
 
void spi_init(void)
{
}

void spi_set_speed(struct spi_slave *slave, uint hz)
{
	/* altera spi core does not support programmable speed */
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct tcc_spi_slave *tccspi;

	if (!spi_cs_is_valid(bus, cs))
		return NULL;
	
	tccspi = spi_alloc_slave(struct tcc_spi_slave, bus, cs);
	if (!tccspi)
		return NULL;

	tccspi->base = (HwGPSB0_BASE + tccspi->slave.bus*TCC_GPSB_BASE_OFFSET);
	tccspi->peri_clk = TCC_GPSB_PERI_CLK;
	tccspi->HwGPIO = ((PGPIO)HwGPIO_BASE);
	tccspi->HwGPSB = ((PGPSB)(HwGPSB0_BASE + tccspi->slave.bus*TCC_GPSB_BASE_OFFSET));
	tccspi->HwGPSBPORTCFG = ((PGPSBPORTCFG)HwGPSB_PORTCFG_BASE);
	tccspi->gpsb_peri_id = PERI_GPSB0 + tccspi->slave.bus;

	debug("%s: bus:%x cs:%x base:%lx\n", __func__,
		bus, cs, (unsigned long)tccspi->base);

	return &tccspi->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	free(tccspi);
}

// TODO: Need to support other port (now only support port 1 and 9 with channel 0)
static void tcc_spi_port_config(struct spi_slave *slave, int port_num)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	PGPIO pGPIO = tccspi->HwGPIO;

	debug("%s:\n", __func__);

	// SPI Port and Channel Configuration
	// Only available Port 1 and 9
	if(port_num == 1)
		tccspi->HwGPSBPORTCFG->PCFG0.nREG = 0x03020001;
	else
		tccspi->HwGPSBPORTCFG->PCFG0.nREG = 0x03020109;

	// GPIO Configuration
#ifdef SNOR_GPIO
	#if defined(CONFIG_TCC896X)
	BITCSET(pGPIO->GPEFN2.nREG, 0x000FFFF0, 0x00000000);
	#elif defined(CONFIG_TCC897X)
		#if defined(TCC897X_GPA)
		BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x00000000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000000);
		#else
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x00000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000000);
		#endif
	#else
	BITCSET(pGPIO->GPEFN1.nREG, 0xFFFF0000, 0x00000000);
	#endif
	BITSET(pGPIO->GPEEN.nREG, PORT_CLK|PORT_CS|PORT_SI);
	BITCLR(pGPIO->GPEEN.nREG, PORT_SO);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CLK|PORT_CS|PORT_SI);

	tccspi->SPI_PORT_DATA_REG = (unsigned int *)&pGPIO->GPEDAT.nREG;
#elif defined(SNOR_GPIO_CS)
	#if defined(CONFIG_TCC896X)
	//  SCLK, SI, SO [ E17, E19, E20] Func (6)
	// CS GPIO_E18
	BITCSET(pGPIO->GPEFN2.nREG,0x000FFFF0,0x00066060);
	BITSET(pGPIO->GPEEN.nREG, PORT_CS);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CS);
	tccspi->SPI_PORT_DATA_REG = (unsigned int *)&pGPIO->GPEDAT.nREG;
	#elif defined(CONFIG_TCC897X)
		#if defined(TCC897X_GPA)
		// SCLK, SI, SO [ A13, A15, A16] Func (6)
		// CS GPIO_A14
		BITCSET(pGPIO->GPAFN1.nREG, 0xF0F00000, 0x60600000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000006);
		BITSET(pGPIO->GPAEN.nREG, PORT_CS);
		BITCLR(pGPIO->GPADAT.nREG, PORT_CS);
		tccspi->SPI_PORT_DATA_REG = (unsigned int *)&pGPIO->GPADAT.nREG;
		#else
		// SCLK, SI, SO [ D7, D8, D10] Func (6)
		// CS GPIO_D9
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x60000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000F0F, 0x00000606);
		BITSET(pGPIO->GPDEN.nREG, PORT_CS);
		BITCLR(pGPIO->GPDDAT.nREG, PORT_CS);
		tccspi->SPI_PORT_DATA_REG = (unsigned int *)&pGPIO->GPDDAT.nREG;
		#endif
	#else
	//  SCLK, SI, SO [ E12, E14, E15] Func (4)
	// CS GPIO_E13
	BITCSET(pGPIO->GPEFN1.nREG,0xFFFF0000,0x44040000);
	BITSET(pGPIO->GPEEN.nREG, PORT_CS);
	BITCLR(pGPIO->GPEDAT.nREG, PORT_CS);
	tccspi->SPI_PORT_DATA_REG = (unsigned int *)&pGPIO->GPEDAT.nREG;
	#endif
#else
	#if defined(CONFIG_TCC896X)
	//  SCLK, SI, SO [ E17, E18, E19, E20] Func (6)
	BITCSET(pGPIO->GPEFN2.nREG,0x000FFFF0,0x00066660);
	#elif defined(CONFIG_TCC897X)
		#if defined(TCC897X_GPA)
		// SCLK, SI, SO, CS[ A13, A14, A15, A16] Func (6)
		BITCSET(pGPIO->GPAFN1.nREG, 0xFFF00000, 0x66600000);
		BITCSET(pGPIO->GPAFN2.nREG, 0x0000000F, 0x00000006);
		#else
		// SCLK, SI, SO, CS [ D7, D8, D9, D10] Func (6)
		BITCSET(pGPIO->GPDFN0.nREG, 0xF0000000, 0x60000000);
		BITCSET(pGPIO->GPDFN1.nREG, 0x00000FFF, 0x00000666);
		#endif
	#else
	// /CS, SCLK, SI, SO [ E13, E12, E14, E15] Func (4)
	BITCSET(pGPIO->GPEFN1.nREG,0xFFFF0000,0x44440000);
	#endif
#endif

#if defined(CONFIG_TCC896X)
	//  SCLK, SI, SO [ E17, E19, E20] Func (6)
	// CS GPIO_E18
	BITCSET(pGPIO->GPECD1.nREG,0x000003FC,0x00000154);
#elif defined(CONFIG_TCC897X)
	#if defined(TCC897X_GPA)
	BITCSET(pGPIO->GPACD0.nREG,0xFC000000,0x54000000);
	BITCSET(pGPIO->GPACD1.nREG,0x00000003,0x00000001);
	#else
	BITCSET(pGPIO->GPDCD0.nREG,0x003FC000,0x00154000);
	#endif
#else
	BITCSET(pGPIO->GPECD0.nREG,0xFF000000,0x55000000);
#endif

#if defined(CONFIG_TCC896X)
	// WP = 1 [A21]
	BITCSET(pGPIO->GPAFN2.nREG,0x00F00000,0x00000000);
	BITSET(pGPIO->GPAEN.nREG,Hw21);
	BITSET(pGPIO->GPADAT.nREG,Hw21);
#elif defined(CONFIG_TCC897X)
	// WP = 1 [A5]
	BITCSET(pGPIO->GPAFN0.nREG,0x00F00000,0x00000000);
	BITSET(pGPIO->GPAEN.nREG,Hw5);
	BITSET(pGPIO->GPADAT.nREG,Hw5);
#else
	// WP = 1 [E19]
	BITCSET(pGPIO->GPEFN2.nREG,0x0000F000,0x00000000);
	BITSET(pGPIO->GPEEN.nREG,Hw19);
	BITSET(pGPIO->GPEDAT.nREG,Hw19);
#endif

#if defined(CONFIG_TCC896X)
	// HOLD = 1 [A22]
	BITCSET(pGPIO->GPAFN2.nREG,0x0F000000,0x00000000);
	BITSET(pGPIO->GPAEN.nREG,Hw22);
	BITSET(pGPIO->GPADAT.nREG,Hw22);
#elif defined(CONFIG_TCC897X)
	// HOLD = 1 [A6]
	BITCSET(pGPIO->GPAFN0.nREG,0x0F000000,0x00000000);
	BITSET(pGPIO->GPAEN.nREG,Hw6);
	BITSET(pGPIO->GPADAT.nREG,Hw6);
#else
	// HOLD = 1 [E18]
	BITCSET(pGPIO->GPEFN2.nREG,0x00000F00,0x00000000);
	BITSET(pGPIO->GPEEN.nREG,Hw18);
	BITSET(pGPIO->GPEDAT.nREG,Hw18);
#endif

}

static void tcc_spi_flush_buffer(struct spi_slave *slave)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	BITSET(tccspi->HwGPSB->MODE.nREG, Hw14|Hw15);
	BITCLR(tccspi->HwGPSB->MODE.nREG, Hw14|Hw15);
}

static unsigned char tcc_spi_set_bitwidth(struct spi_slave *slave, unsigned char bitWidth)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	unsigned char prevBitWidth = (unsigned char)((tccspi->HwGPSB->MODE.nREG & (0x1F<<8)) >> 8) + 1;
	BITCSET(tccspi->HwGPSB->MODE.nREG,0x1F<<8,(bitWidth-1)<<8);
	debug("%s: bitwidth : %02x\n", __func__,(unsigned char)((tccspi->HwGPSB->MODE.nREG & (0x1F<<8)) >> 8) + 1);
	return prevBitWidth;
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);

	debug("%s:\n", __func__);

	// SPI Port and Channel Configuration
	tcc_spi_port_config(slave,0);
	
	// Clock Configuration
	tcc_set_peri(tccspi->gpsb_peri_id,ENABLE,tccspi->peri_clk);
	tccspi->peri_clk = tcc_get_peri(tccspi->gpsb_peri_id);
	
	// SPI Configuration
	/* Configure MODE */
	tccspi->HwGPSB->MODE.nREG =
           ( (0x0) << 0 ) |	// (2) operation mode ; 00:SPI compatible
           ( (0x0) << 2 ) |	// (1) Slave mode configuration ; 0:master mode
           ( (31)  << 8 ) |	// (5) Bit width selection (BWS+1)
           ( (5)   << 24) |	// (8) clock divider load value

           //Hw23 | 		//Master recovery time 
							//tRECV = (TRE + 1) * (SCKO period) 

           //Hw22 | 		//Master hold time 
							//tHOLD = (THL + 1) * (SCKO period) 

           //Hw21 | 		//Master setup time 
							//tSETUP = (TSU + 1) * (SCKO period) 

           //Hw20 | 		//Polarity control for CS(FRM) - Master Only 
							//¡®0¡¯ for active low (default) / must be ¡®0¡¯ for SPI 
							//¡®1¡¯ for active high 

           //Hw19 | 		//Polarity control for CMD(FRM) - Master only
							//¡®0¡¯ for active high / must be ¡®0¡¯ for SSP 
							//¡®1¡¯ for active low 

           //Hw18 |			//Polarity control for transmitting data - Master Only
							//¡®0¡¯ for falling edge of SCK 
							//¡®1¡¯ for rising edge of SCK / must be ¡®1¡¯ for SSP

           Hw17 |			//Polarity control for receiving data - Master only 
							//¡®0¡¯ for rising edge of SCK 
							//¡®1¡¯ for falling edge of SCK / must be ¡®1¡¯ for SSP 
           ( (0) );

	/* Configure Interrupt */
	tccspi->HwGPSB->INTEN.nREG =
	           ( (0x1) << 24 ) |	// RX byte swap in half-word
	           ( (0x1) << 25 ) |	// RX half-word swap in word
	           ( (0x1) << 26 ) |	// TX byte swap in half-word
	           ( (0x1) << 27 ) |	// TX half-word swap in word
	           ( (0) );
	
	BITSET(tccspi->HwGPSB->MODE.nREG,Hw3);	//operation enable

	// Flush Buffer
	tcc_spi_flush_buffer(slave);

	tcc_spi_delay_us(1000);

	// CLK Set
	BITCSET(tccspi->HwGPSB->MODE.nREG,0xFF000000, ((unsigned int)TCC_GPSB_READ_DIV<<24));
	//d = readl(tccspi->base+TCC_GPSB_MODE);
	//writel((d | (unsigned int)(7 << 24)), tccspi->base+TCC_GPSB_MODE);

	// Set SPI Bit Width
	tcc_spi_set_bitwidth(slave, TCC_GPSB_BIT_WIDTH);
	//d = readl(tccspi->base+TCC_GPSB_MODE);
	//writel((d | (unsigned int)(7<<8)), tccspi->base+TCC_GPSB_MODE);

	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
}

static void tcc_spi_set_ctf_mode(struct spi_slave *slave, unsigned int set)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	if(set)
		BITSET(tccspi->HwGPSB->MODE.nREG,Hw4); // set continuous mode
	else
		BITCLR(tccspi->HwGPSB->MODE.nREG,Hw4);	// release continuous mode

}

void tcc_spi_byte_read(struct spi_slave *slave, uchar *rxp_byte)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
#ifdef SNOR_GPIO
	int i = 0;
	unsigned char rData = 0;

	//==================================
	// Receive 1 byte
	//==================================
	for ( i = 0; i < 8; i++)
	{
		BITSET((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CLK);
		rData = (0xFF)&(rData << 1);
		if((*(unsigned int*)tccspi->SPI_PORT_DATA_REG) & PORT_SO)
			rData |= Hw0;

		BITCLR((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CLK);
	}
	*rxp_byte = rData;
#else
	int flagContinue = 1;
	if(flagContinue != 0)
	{
		tcc_spi_set_ctf_mode(slave,flagContinue);
	}	

	tccspi->HwGPSB->PORT.nREG = 0;

	while(ISZERO(tccspi->HwGPSB->STAT.nREG,Hw2));	// check receive FIFO not empty flag

	*rxp_byte = (unsigned char)((unsigned int)(tccspi->HwGPSB->PORT.nREG) >> 24);
	debug("%s bytes: 0x%02x\n", __func__, *rxp_byte);
	
	if(flagContinue == 0)
	{
		tcc_spi_set_ctf_mode(slave,flagContinue);
	}
#endif

}

void tcc_spi_byte_write(struct spi_slave *slave, const uchar txp_byte)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
#ifdef SNOR_GPIO
	int i = 0;
	int nData[8];

	//==================================
	// Data Serialize
	//==================================
	for ( i = 0; i < 8; i++)
	{
		nData[i] = PORT_CLK;
		if(txp_byte & Hw0)
			nData[i] |= PORT_SI;

		txp_byte = txp_byte >> 1;
	}

	//==================================
	// Send 1 byte
	//==================================
	for (i = 8; i >= 0; --i)
	{
		BITCSET((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_SI|PORT_CLK, nData[i]);
		BITCLR((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_SI|PORT_CLK);
	}
#else
	unsigned int dummy;
	int flagContinue = 1;
	unsigned char tx_data = txp_byte;
	if(flagContinue != 0)
	{
		tcc_spi_set_ctf_mode(slave,flagContinue);
	}

	tccspi->HwGPSB->PORT.nREG = ((unsigned int)tx_data)<<24;

	while(ISZERO(tccspi->HwGPSB->STAT.nREG,Hw2));	// check receive FIFO not empty flag

	// Flush data port register
	// TODO
	//dummy = tccspi->HwGPSB->PORT.nREG;
	dummy = readl(tccspi->HwGPSB);
	debug("%s : remainder: %08x\n",__func__,dummy);

	if(flagContinue == 0)
	{
		tcc_spi_set_ctf_mode(slave,flagContinue);
	}
#endif

}

void tcc_spi_read(struct spi_slave *slave, unsigned int bytes, uchar *din)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	int i = 0;
	uchar *rxp = din;

	BITCLR((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CLK);
	debug("%s bytes: %d\n", __func__, bytes);

	//==================================
	// Receive Multiple bytes
	//==================================
	for (i = 0; i < bytes ; i++)
	{
		tcc_spi_byte_read(slave,rxp++);
		debug("[0x%02x]\n",din[i] );
	}
	
	spi_cs_deactivate(slave);	// TODO : 
}

void tcc_spi_write(struct spi_slave *slave, unsigned int bytes, const uchar *dout)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	int i = 0;
	const uchar *txp = dout;

	BITCLR((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CLK);
	debug("%s\n", __func__);
	spi_cs_activate(slave);	// TODO : 

	//==================================
	// Send Multiple bytes
	//==================================
	for (i = 0; i < bytes ; i++)
	{
		tcc_spi_byte_write(slave,txp[i]);
	}
}

void tcc_spi_txrx(struct spi_slave *slave, unsigned int bytes, const uchar *dout, uchar *din)
{
	struct tcc_spi_slave *tccspi = to_tcc_spi_slave(slave);
	int i;
	const uchar *txp = dout;
	uchar *rxp = din;

	BITCLR((*(unsigned int*)tccspi->SPI_PORT_DATA_REG), PORT_CLK);
	debug("%s\n", __func__);

	for(i=0;i<bytes;i++)
	{
		debug("%s\n", __func__);
		
		tcc_spi_byte_write(slave,txp[i]);
		tcc_spi_byte_read(slave,rxp++);
	}
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	/* assume spi core configured to do 8 bit transfers */
	uint bytes = bitlen / 8;

	debug("%s: bus:%i cs:%i bitlen:%i bytes:%i flags:%lx\n", __func__,
		slave->bus, slave->cs, bitlen, bytes, flags);
	if (bitlen == 0)
		goto done;

	if (bitlen % 8) {
		flags |= SPI_XFER_END;
		goto done;
	}
	//if (flags & SPI_XFER_BEGIN)
	//spi_cs_activate(slave);
	// TODO : Control cs activation

	// Send and Read Data
	if(dout != NULL && din != NULL)
	{
		tcc_spi_txrx(slave, bytes, (const uchar*)dout, (uchar *)din);
	}
	// Send Data
	else if(dout != NULL)
	{
		tcc_spi_write(slave, bytes, (const uchar*)dout);
	}
	// Read Data
	else if(din != NULL)
	{
		tcc_spi_read(slave, bytes, (uchar *)din);
	}
 done:
	//if (flags & SPI_XFER_END)
	//	spi_cs_deactivate(slave);
	return 0;
}
