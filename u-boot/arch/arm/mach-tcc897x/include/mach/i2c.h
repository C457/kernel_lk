#ifndef __TCC_I2C_H__
#define __TCC_I2C_H__

enum {
	I2C_CH_MASTER0 = 0,
	I2C_CH_MASTER1,
	I2C_CH_MASTER2,
	I2C_CH_MASTER3,
	I2C_CH_SMU,
	I2C_CH_NUM
};

int i2c_xfer(unsigned char slave_addr,
		unsigned char out_len, unsigned char* out_buf,
		unsigned char in_len, unsigned char* in_buf,
		int i2c_name);

void i2c_set_clock(int i2c_name, unsigned int i2c_clk_input_freq_khz, unsigned int speed);

#endif /*__TCC_I2C_H__*/
