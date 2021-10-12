/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <exports.h>
#include <asm/io.h>
#include <asm/arch/globals.h>
#include <asm/arch/reg_physical.h>
#include <asm/arch/clock.h>
#include <asm/arch/tcc_ckc.h>
#include <asm/arch/i2c.h>

DECLARE_GLOBAL_DATA_PTR;

#define read_reg(a)			(*(volatile unsigned long *)a)
#define write_reg(v, a)		(*(volatile unsigned long *)a = v)

/* read/write bit */
#define I2C_WR	0
#define I2C_RD	1

#define WAIT_ACK_TIMEOUT	100000
#define WAIT_TIP_TIMEOUT	10000
extern  	void i2c_set_clock(int i2c_name, unsigned int i2c_clk_input_freq_khz, unsigned int speed);
/* 
 * I2C register structure
 */
struct tcc_i2c_regs {
	volatile unsigned long PRES, CTRL, TXR, CMD, RXR, SR, TR;
};

/* 
 * register structure
 */
struct tcc_i2c {
	unsigned int clk;
	unsigned long IRQSTR;
	volatile struct tcc_i2c_regs *regs;
	unsigned int peri_name;
	unsigned int iobus_name;
};

static struct tcc_i2c i2c[I2C_CH_NUM] = {
	[I2C_CH_MASTER0] = {
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER0_BASE,
		.peri_name = PERI_I2C0,
		.iobus_name = IOBUS_I2C_M0,
	},
	[I2C_CH_MASTER1] = {
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER1_BASE,
		.peri_name = PERI_I2C1,
		.iobus_name = IOBUS_I2C_M1,
	},
	[I2C_CH_MASTER2] = {
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER2_BASE,
		.peri_name = PERI_I2C2,
		.iobus_name = IOBUS_I2C_M2,
	},
	[I2C_CH_MASTER3] = {
		.IRQSTR = HwI2C_PORTCFG_BASE + 0x0C,
		.regs = (volatile struct tcc_i2c_regs *)HwI2C_MASTER3_BASE,
		.peri_name = PERI_I2C3,
		.iobus_name = IOBUS_I2C_M3,
	},
	/* SMU_I2C Controller */
	[I2C_CH_SMU] = {
		.IRQSTR = HwSMUI2C_BASE + 0x80,	/* SMU_I2C ICLK register address */
		.regs = (volatile struct tcc_i2c_regs *)HwSMUI2C_BASE,
	},
};

#if 0
static int wait_busy(int i2c_name)
{
	volatile unsigned long cnt = 0;

	while (1) {
		cnt++;
		if (!(i2c[i2c_name].regs->SR & Hw1)) break;
		if (cnt > WAIT_TIP_TIMEOUT) {
			debug("i2c-tcc: tip time out!\n");
			return -1;
		}
	}

	return 0;
}

static int wait_ack(int i2c_name)
{
	volatile unsigned long cnt = 0;

	if (wait_busy(i2c_name) < 0)
		return -1;

	while (1) {
		cnt++;
		if (!(i2c[i2c_name].regs->SR & Hw7)) break;
		if (cnt > WAIT_TIP_TIMEOUT) {
			debug("i2c-tcc: ack time out!\n");
			return -1;
		}
	}

	return 0;
}

int i2c_xfer(unsigned char slave_addr, 
		unsigned char out_len, unsigned char* out_buf, 
		unsigned char in_len, unsigned char* in_buf, 
		int i2c_name)
{
	int ret;
	int i = 0;

	/* 
	 * WRITE 
	 */
	if (out_len > 0) {
		i2c[i2c_name].regs->TXR = slave_addr | I2C_WR;
		i2c[i2c_name].regs->CMD = Hw7 | Hw4;

		ret = wait_ack(i2c_name);
		if (ret != 0) {
			i2c[i2c_name].regs->CMD = Hw6; //stop enable
			if (wait_busy(i2c_name) != 0)
				printf("Start Wait end\n");
			return ret;
		}

		for (i = 0; i < out_len; i++) {
			i2c[i2c_name].regs->TXR = out_buf[i];
			i2c[i2c_name].regs->CMD = Hw4;

			ret = wait_ack(i2c_name);
			if (ret != 0) {
				i2c[i2c_name].regs->CMD = Hw6; //stop enable
				if (wait_busy(i2c_name) != 0)
					printf("Start Wait end\n");
				return ret;
			}
		}

		if(in_len <= 0)
		{
			i2c[i2c_name].regs->CMD = Hw6;

			ret = wait_busy(i2c_name);
			if (ret != 0) return ret;
		}
	}

	/* 
	 * READ
	 */
	if (in_len > 0) {
		i2c[i2c_name].regs->TXR = slave_addr | I2C_RD;
		i2c[i2c_name].regs->CMD = Hw7 | Hw4;

		ret = wait_ack(i2c_name);
		if (ret != 0) {
			i2c[i2c_name].regs->CMD = Hw6; //stop enable
			if (wait_busy(i2c_name) != 0)
				printf("Start Wait end\n");
			return ret;
		}


		for (i = 0; i < in_len; i++) {
			if (i == (in_len - 1)) 
    			i2c[i2c_name].regs->CMD = Hw5 | Hw3;
    		else
    			i2c[i2c_name].regs->CMD = Hw5;

			if (wait_busy(i2c_name) != 0) {
				printf("Start Wait end\n");
				return ret;
			}

			in_buf[i] =(unsigned char)i2c[i2c_name].regs->RXR;
		}

		i2c[i2c_name].regs->CMD = Hw6; //stop enable
		if (wait_busy(i2c_name) != 0)
			printf("Start Wait end\n");
		return ret;
	}

	return 0;
}
#else
static int wait_intr(int i2c_name)
{
	volatile unsigned long cnt = 0;

	if (i2c_name < I2C_CH_SMU) {
		while (!(read_reg(i2c[i2c_name].IRQSTR) & (1<<i2c_name))) {
			cnt++;
			if (cnt > WAIT_ACK_TIMEOUT) {
				printf("i2c-tcc: time out!\n");
				return -1;
			}
		}
	} else {
		/* SMU_I2C wait */
		while (1) {
			cnt++;
			if (!(i2c[i2c_name].regs->SR & Hw1)) break;
			if (cnt > WAIT_ACK_TIMEOUT) {
				printf("smu-i2c-tcc: time out!\n");
				return -1;
			}
		}
		for (cnt = 0; cnt <15; cnt++);
	}

	return 0;
}

int i2c_xfer(unsigned char slave_addr,
		unsigned char out_len, unsigned char* out_buf,
		unsigned char in_len, unsigned char* in_buf,
		int i2c_name)
{
	int ret;
	int i = 0;

	/*
	 * WRITE
	 */
	if (out_len > 0) {
		i2c[i2c_name].regs->TXR = slave_addr | I2C_WR;
		i2c[i2c_name].regs->CMD = Hw7 | Hw4;

		ret = wait_intr(i2c_name);
		if (ret != 0) return ret;
		BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt

		for (i = 0; i < out_len; i++) {
			i2c[i2c_name].regs->TXR = out_buf[i];
			i2c[i2c_name].regs->CMD = Hw4;

			ret = wait_intr(i2c_name);
			if (ret != 0) return ret;
			BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt
		}

		if(in_len <= 0)
		{
			i2c[i2c_name].regs->CMD = Hw6;

			ret = wait_intr(i2c_name);
			if (ret != 0) return ret;
		}

		BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt
	}

	/*
	 * READ
	 */
	if (in_len > 0) {
		i2c[i2c_name].regs->TXR = slave_addr | I2C_RD;
		i2c[i2c_name].regs->CMD = Hw7 | Hw4;

		ret = wait_intr(i2c_name);
		if (ret != 0) return ret;
		BITSET(i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt

		for (i = 0; i < in_len; i++) {
			//i2c[i2c_name].regs->CMD = Hw5 | Hw3;
			if (i == (in_len - 1))
				i2c[i2c_name].regs->CMD = Hw5 | Hw3;
			else
				i2c[i2c_name].regs->CMD = Hw5;

			ret = wait_intr(i2c_name);
			if (ret != 0) return ret;

			BITSET( i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt

			in_buf[i] =(unsigned char)i2c[i2c_name].regs->RXR;
		}

		i2c[i2c_name].regs->CMD = Hw6;

		ret = wait_intr(i2c_name);
		if (ret != 0) return ret;
		BITSET( i2c[i2c_name].regs->CMD, Hw0); //Clear a pending interrupt
	}

	return 0;
}
#endif

static void i2c_reset(int i2c_name)
{
	#if defined(CONFIG_TCC897X)
	tcc_set_iobus_swreset(i2c[i2c_name].iobus_name, 1);
	tcc_set_iobus_swreset(i2c[i2c_name].iobus_name, 0);
	#else
	tca_ckc_setioswreset(i2c[i2c_name].iobus_name, 1);
	tca_ckc_setioswreset(i2c[i2c_name].iobus_name, 0);
	#endif
}

void i2c_set_clock(int i2c_name, unsigned int i2c_clk_input_freq_khz, unsigned int speed)
{
	unsigned int prescale;

	prescale = i2c_clk_input_freq_khz / ((speed/1000) * 5) - 1;
	i2c[i2c_name].regs->PRES = prescale;
	i2c[i2c_name].regs->CTRL = Hw7 | Hw6 | HwZERO;  /* start enable, stop enable, 8bit mode */
	BITSET(i2c[i2c_name].regs->CMD, Hw0);           /* clear pending interrupt */

	debug("i2c_clk_input_freq_khz: %d, speed : %d, prescale : %d\n", 
			i2c_clk_input_freq_khz, speed, prescale);
}


static void i2c_set_speed(int i2c_name, unsigned int speed)
{
	int input_freq;

	if (i2c_name == I2C_CH_SMU) {
		/* SMU_I2C */
		write_reg(0x80000000, i2c[I2C_CH_SMU].IRQSTR);
		i2c[I2C_CH_SMU].regs->CTRL = 0;
		i2c[I2C_CH_SMU].regs->PRES = i2c[I2C_CH_SMU].clk;
		BITSET(i2c[I2C_CH_SMU].regs->CTRL, Hw7|Hw6);
	} else {
		#if defined(CONFIG_TCC897X)		
		tcc_set_peri(i2c[i2c_name].peri_name, ENABLE, 6000000);	/* I2C peri bus enable */
		tcc_set_iobus_pwdn(i2c[i2c_name].iobus_name, 0);				/* I2C io bus enable */
		input_freq = tcc_get_peri(i2c[i2c_name].peri_name);		/* get I2C bus clock */
		i2c_set_clock(i2c_name, (input_freq / 10), speed);
		#else
		tca_ckc_setperi(i2c[i2c_name].peri_name, ENABLE, 60000);	/* I2C peri bus enable */
		tca_ckc_setiopwdn(i2c[i2c_name].iobus_name, 0);				/* I2C io bus enable */
		input_freq = tca_ckc_getperi(i2c[i2c_name].peri_name);		/* get I2C bus clock */
		i2c_set_clock(i2c_name, (input_freq / 10), speed);
		#endif
	}
}

static unsigned int tcc_i2c_set_bus_speed(struct i2c_adapter *adap,
			unsigned int speed)
{
	i2c_set_speed(adap->hwadapnr, speed);
	return 0;
}


void tcc_i2c_init(struct i2c_adapter *adap, int speed, int slaveaddr)
{
	#if 0
	/* No i2c support prior to relocation */
	if (!(gd->flags & GD_FLG_RELOC))
		return;
	#endif

	i2c_reset(adap->hwadapnr);
	i2c_set_bus_speed(speed);
}

/*
 * i2c_read: - Read multiple bytes from an i2c device
 *
 * The higher level routines take into account that this function is only
 * called with len < page length of the device (see configuration file)
 *
 * @chip:   address of the chip which is to be read
 * @addr:   i2c data address within the chip
 * @alen:   length of the i2c data address (1..2 bytes)
 * @buffer: where to write the data
 * @len:    how much byte do we want to read
 * @return: 0 in case of success
 */
int tcc_i2c_read(struct i2c_adapter *adap, uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int ret;

	ret = i2c_xfer(chip << 1, alen, (unsigned char*)&addr, len, buffer, adap->hwadapnr);

	return ret;
}

/*
 * i2c_write: -  Write multiple bytes to an i2c device
 *
 * The higher level routines take into account that this function is only
 * called with len < page length of the device (see configuration file)
 *
 * @chip:   address of the chip which is to be written
 * @addr:   i2c data address within the chip
 * @alen:   length of the i2c data address (1..2 bytes)
 * @buffer: where to find the data to be written
 * @len:    how much byte do we want to read
 * @return: 0 in case of success
 */
int tcc_i2c_write(struct i2c_adapter *adap, uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	unsigned char *tmp_buf;
	int ret;

	tmp_buf = malloc(alen + len);

	memcpy(tmp_buf, &addr, alen);
	memcpy(tmp_buf+alen, buffer, len);
	
	ret = i2c_xfer(chip << 1, alen+len, tmp_buf,  (unsigned char)0, NULL, adap->hwadapnr);

	free(tmp_buf);

	return ret;
}

/*
 * i2c_probe: - Test if a chip answers for a given i2c address
 *
 * @chip:   address of the chip which is searched for
 * @return: 0 if a chip was found, -1 otherwhise
 */
int tcc_i2c_probe(struct i2c_adapter *adap, uchar chip)
{
	uchar tmp;
	return tcc_i2c_read(adap, chip, 0, 0, &tmp, 1);
}


/*
 * Register soft i2c adapters
 */
U_BOOT_I2C_ADAP_COMPLETE(tcc_i2c0, tcc_i2c_init, tcc_i2c_probe,
			 tcc_i2c_read, tcc_i2c_write,
			 tcc_i2c_set_bus_speed, 400000, 0, 0)
U_BOOT_I2C_ADAP_COMPLETE(tcc_i2c1, tcc_i2c_init, tcc_i2c_probe,
			 tcc_i2c_read, tcc_i2c_write,
			 tcc_i2c_set_bus_speed, 100000, 0, 1)
U_BOOT_I2C_ADAP_COMPLETE(tcc_i2c2, tcc_i2c_init, tcc_i2c_probe,
			 tcc_i2c_read, tcc_i2c_write,
			 tcc_i2c_set_bus_speed, 100000, 0, 2)
U_BOOT_I2C_ADAP_COMPLETE(tcc_i2c3, tcc_i2c_init, tcc_i2c_probe,
			 tcc_i2c_read, tcc_i2c_write,
			 tcc_i2c_set_bus_speed, 100000, 0, 3)
U_BOOT_I2C_ADAP_COMPLETE(tcc_i2c4, tcc_i2c_init, tcc_i2c_probe,
			 tcc_i2c_read, tcc_i2c_write,
			 tcc_i2c_set_bus_speed, 100000, 0, 4)
