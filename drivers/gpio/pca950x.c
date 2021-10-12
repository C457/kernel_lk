/*
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include <common.h>
#include <i2c.h>
#include <pca950x.h>
#include <asm/arch/gpio.h>
#include <malloc.h>

#define PCA950_I2C_ADDR	0x20

#define PCA9506_INPUT_REG 0x00
#define PCA9506_OUTPUT_REG 0x08
#define PCA9506_POLARITY_REG 0x10
#define PCA9506_IOCFG_REG 0x18
#define PCA9506_INT_REG 0x20
#define PCA9506_MAX_BANK 5

struct ext_gpio {
    unsigned i2c_ch;
    unsigned addr;
    unsigned gpio_id;
    int (*config)(struct ext_gpio *ext, unsigned nr, unsigned flags);
    void (*set)(struct ext_gpio *ext, unsigned nr, unsigned on);
    int (*get)(struct ext_gpio *ext, unsigned nr);
};

extern int register_ext_gpio(unsigned gpio_id, struct ext_gpio *gpios);
extern int i2c_xfer(unsigned char slave_addr,
		unsigned char out_len, unsigned char* out_buf,
		unsigned char in_len, unsigned char* in_buf,
		int i2c_name);

int pca950x_config(struct ext_gpio *ext, unsigned n, unsigned flags)
{
	unsigned port = n&GPIO_BITMASK;
	unsigned bank = port/8;
	unsigned char mode[2] = {PCA9506_IOCFG_REG + bank ,0};
	unsigned char data[2] = {PCA9506_OUTPUT_REG + bank ,0};
	i2c_xfer(ext->addr, 1, &data[0], 1, &data[1], ext->i2c_ch);
	i2c_xfer(ext->addr, 1, &mode[0], 1, &mode[1], ext->i2c_ch);

	if (bank >= PCA9506_MAX_BANK)
		return -1;

	port %= 8;
	if (flags & GPIO_OUTPUT) {
		if (flags & GPIO_HIGH)
			data[1] |= 1<<port;
		if (flags & GPIO_LOW)
			data[1] &= ~(1<<port);
		mode[1] &= ~(1<<port);
	} else
		mode[1] |= 1<<port;

	i2c_xfer(ext->addr, 2, data, 0, 0, ext->i2c_ch);
	i2c_xfer(ext->addr, 2, mode, 0, 0, ext->i2c_ch);

	return 0;
}

void pca950x_set(struct ext_gpio *ext, unsigned n, unsigned on)
{
	unsigned port = n&GPIO_BITMASK;
	unsigned bank = port/8;
	unsigned char data[2] = {PCA9506_OUTPUT_REG + bank , 0};

	int ret = 0;

	debug("\x1b[1;31m[%s:%d]\x1b[0mbank: %d, addr : 0x%x\n", __func__, __LINE__,bank,  ext->addr);
	
	ret = i2c_xfer(ext->addr, 1, &data[0], 1, &data[1], ext->i2c_ch);

	debug("i2c ret : %d \n" , ret);

	if (bank >= PCA9506_MAX_BANK)
		return;

	port %= 8;
	on ? (data[1] |= 1<<port) : (data[1] &= ~(1<<port));

	debug("\x1b[1;33m[%s:%d]\x1b[0m0x%x, 0x%x\n", __func__, __LINE__, data[0], data[1]);
	
	i2c_xfer(ext->addr, 2, data, 0, 0, ext->i2c_ch);
}

int pca950x_get(struct ext_gpio *ext, unsigned n)
{
	unsigned port = n&GPIO_BITMASK;
	unsigned bank = port/8;
	unsigned char data[2] = {PCA9506_INPUT_REG + bank , 0};
	i2c_xfer(ext->addr, 1, &data[0], 1, &data[1], ext->i2c_ch);

	if (bank >= PCA9506_MAX_BANK)
		return 0;

	port %= 8;
	return (data[1] & (1<<port)) ? 1 : 0;
}

/*
 * Initialize PCA950X device
 * i2c_ch: i2c controller channel
 * id    : pca950x address id.
 *         ex) 00,01,02,...
 * bank0, bank1, bank2, bank3, bank4
 * input, output, Polarity, IO, mask
 */
void pca950x_init(unsigned gpio_id, unsigned i2c_ch, unsigned id, unsigned char bank, unsigned char direct, unsigned char level)
{
	struct ext_gpio *gpios;

	unsigned char DestAddress;
	unsigned char data[2] = {0,0};	/*{REG,DATA,}*/

	DestAddress = (PCA950_I2C_ADDR+(id&0x3))<<1;
	
	debug("\x1b[1;31m[%s:%d]\x1b[0mid: %d, bank: %d, direct: 0x%x, level: 0x%x, DestAddress: 0x%x\n", __func__, __LINE__, id, bank, direct, level, DestAddress);
	
	data[0] = PCA9506_IOCFG_REG + bank;
	data[1] = direct;
	i2c_xfer(DestAddress, 2, data, 0, 0, i2c_ch);

	data[0] = PCA9506_OUTPUT_REG + bank;
	data[1] = level;
	
	debug("\x1b[1;33m[%s:%d]\x1b[0m0x%x, 0x%x\n", __func__, __LINE__, data[0], data[1]);
	i2c_xfer(DestAddress, 2, data, 0, 0, i2c_ch);

	gpios = (struct ext_gpio *)malloc(sizeof(struct ext_gpio));
	if (gpios) {
		gpios->addr = DestAddress;
		gpios->i2c_ch = i2c_ch;
		gpios->gpio_id = gpio_id;
		gpios->config = &pca950x_config;
		gpios->set = &pca950x_set;
		gpios->get = &pca950x_get;
		if (register_ext_gpio(gpio_id, gpios))
			free(gpios);
	}
}
