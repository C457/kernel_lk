#ifndef __POWER_H_
#define __POWER_H_

enum {
    PWR_CPU,
    PWR_CPU0,
    PWR_CPU1,
    PWR_CORE,
    PWR_MEM,
    PWR_SDIO,
    PWR_IOD0,
    PWR_IOD1,
    PWR_IOD2,
    PWR_GV,
    PWR_VB,
    PWR_IO,
    PWR_HDMI_12D,
    PWR_HDMI_33D,
    PWR_BOOST_5V,
};

struct tcc_pwr_src {
    int (*pwr_set_voltage)(int type, unsigned int voltage);
    int (*pwr_set_power)(int type, int onoff);
    unsigned int (*pwr_get_voltage)(int type);
    void (*pwr_init)(void);
};

extern void tcc_set_voltage(int type, int onoff, unsigned int voltage);
extern void tcc_get_voltage(int type, int onoff);
extern void tcc_pwr_init(void);

#endif
