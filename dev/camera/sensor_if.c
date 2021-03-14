
#include <debug.h>
#include <dev/camera/sensor_if.h>
#include <platform/gpio.h>
#include <dev/camera/camera.h>

extern struct tcc_cif_parameters	atv_parameters_data;
extern struct tcc_cif_parameters	lvds_parameters_data;
struct tcc_cif_parameters *		parameters_data = &atv_parameters_data;

SENSOR_FUNC_TYPE			sensor_func;

extern int sensor_if_connect_api(SENSOR_FUNC_TYPE * sensor_func);
extern int lvds_if_connect_api(SENSOR_FUNC_TYPE * sensor_func);

int sensor_if_close(void) {
	dprintf(INFO, "!@#---- %s\n", __func__);
	
	return sensor_func.close();
}

int sensor_if_open(void) {
	int		camera_type	= CAM_TYPE_DEFAULT;
	int		camera_encode	= CAM_ENC_DEFAULT;

	unsigned int loop=0;
	unsigned int mode0 = 0, mode1 = 0, mode2 = 0;
	unsigned int mode_value[3];
	
	//dprintf(INFO, "!@#---- %s\n", __func__);
	
	for(loop = 0; loop < 3; loop++) {
		mode0 = gpio_get( GPIO_PORTB|22 );
		mode1 = gpio_get( GPIO_PORTB|19 );      //M101_CAMERA_VARI2, PB[12]
		mode2 = gpio_get( GPIO_PORTB|23 );      //M100_CAMERA_VARI1, PD[12]
		camera_type = ((mode0<<2)|(mode1<<1)|mode2);
		mode_value[loop] = camera_type;

		mdelay(10);
	}

	if( mode_value[0] == mode_value[1] && mode_value[1] == mode_value[2] ) {
		dprintf(INFO, "%s : gpio read value pass.................. \n", __func__);
		if(camera_type == 4 || camera_type == 6) {
			parameters_data = &lvds_parameters_data;
			parameters_data->Camera_type = CAM_TYPE_LVDS;	// LVDS
			lvds_if_connect_api(&sensor_func);
		} else {
			parameters_data = &atv_parameters_data;

			switch(camera_type) {
				case 1:
					parameters_data->Camera_type = CAM_TYPE_SVIDEO;		// avm_svideo
					sensor_if_connect_api(&sensor_func);
					break;
				case 2:
					parameters_data->Camera_type = CAM_TYPE_COMPONENT;	// avm-component
					sensor_if_connect_api(&sensor_func);
					break;
				/*case 3:
					  parameters_data->Camera_type = CAM_TYPE_UPDATE;		// update
					  break;*/
				default:
					parameters_data->Camera_type = CAM_TYPE_CVBS;		// CVBS
					sensor_if_connect_api(&sensor_func);
					break;
			}
		}
	}
	dprintf(INFO, "%s : gpio read value = %d \n", __func__, camera_type);

	return sensor_func.open(camera_type, camera_encode);
}

int sensor_if_tune(int camera_type, int camera_encode) {
	dprintf(INFO, "!@#---- %s - type = 0x%x, encode = 0x%x\n", __func__, camera_type, camera_encode);
	
	return sensor_func.tune(camera_type, camera_encode);
}

