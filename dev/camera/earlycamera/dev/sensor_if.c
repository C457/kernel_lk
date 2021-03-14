
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.
****************************************************************************/

#include <debug.h>
#include <sensor_if.h>
#include <gpio.h>

SENSOR_FUNC_TYPE	sensor_func;

extern int sensor_connect_api(SENSOR_FUNC_TYPE * sensor_func);
extern int lvds_sensor_connect_api(SENSOR_FUNC_TYPE * sensor_func);

int sensor_if_init(int camera_type) {
	dprintf(SPEW,"camera_type : %d \n", camera_type);

	if(camera_type == CAM_TYPE_LVDS)
		lvds_sensor_connect_api(&sensor_func);
	else
		sensor_connect_api(&sensor_func);

	return 0;
}

int sensor_if_close(void) {
	return sensor_func.close();
}

int sensor_if_open(int camera_type, int camera_encode) {
	return sensor_func.open(camera_type, camera_encode);
}

int sensor_if_tune(int camera_type, int camera_encode) {
	return sensor_func.tune(camera_type, camera_encode);
}

int sensor_if_video_check(void) {
	return sensor_func.video_check();
}

