#include <power.h>
#include <string.h>
#include <debug.h>
#include <i2c.h>

#ifdef AXP192_PMIC
#include <dev/axp192.h>
#endif

#ifdef TCC270_PMIC
#include <dev/tcc270.h>
#endif

#ifdef RT8088_DISCR
#include <dev/rt8088.h>
#endif

#ifdef DA9062_PMIC
#include <dev/da9062.h>
#endif

#ifdef RT5746_DISCR
#include <dev/rt5746.h>
#endif

static struct tcc_pwr_src *power_src = NULL;
static int power_initialized = 0;

struct tcc_pwr_src power_control = {
    #ifdef DA9062_PMIC
    .pwr_set_voltage = &da9062_set_voltage,
    .pwr_set_power = &da9062_set_power,
    .pwr_init = &da9062_init,
    .pwr_get_voltage = &da9062_get_voltage,
    #endif
    #ifdef RT8088_DISCR
   .pwr_set_voltage = &rt8088_set_voltage,
   .pwr_set_power = &rt8088_set_power,
   .pwr_init = &rt8088_init,
    #endif
    #ifdef AXP192_PMIC
   .pwr_set_voltage = &axp192_set_voltage,
   .pwr_set_power = &axp192_set_power,
   .pwr_init = &axp192_init,
    #endif
   #ifdef TCC270_PMIC
   .pwr_set_voltage = &tcc270_set_voltage,
   .pwr_set_power = &tcc270_set_power,
   .pwr_init = &tcc270_init,
   #endif
};

void tcc_pwr_init(void)
{
    power_src = &power_control;

    if (power_src)
        power_src->pwr_init();

#ifdef RT5746_DISCR
    rt5746_init();
#endif

    power_initialized = 1;
}

void tcc_set_voltage(int type, int onoff, unsigned int voltage)
{
    if(!power_initialized)
        tcc_pwr_init();

    if (power_src)
    {
#ifdef RT5746_DISCR
	if(type == PWR_VB){
	     rt5746_set_voltage(0, voltage);
	     rt5746_set_power(0, onoff);	
	}
	else{
#endif
            power_src->pwr_set_voltage(type, voltage);
            power_src->pwr_set_power(type, onoff);
#ifdef RT5746_DISCR
	}
#endif
    }
}

void tcc_get_voltage(int type, int onoff)
{
    unsigned int value;

    if(!power_initialized)
        tcc_pwr_init();

    if (power_src)
    {
        value = power_src->pwr_get_voltage(type);
        dprintf(INFO,"%d Voltage set is %d\n", type, value);
    }
}
