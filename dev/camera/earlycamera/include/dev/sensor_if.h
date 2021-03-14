#ifndef __SENSOR_IF_H__
#define __SENSOR_IF_H__

#ifndef NULL
#define NULL	0
#endif

//#define PROGRESSIVE_MODE

enum camera_type {
	CAM_TYPE_DEFAULT	= 0,
	CAM_TYPE_CVBS		= 0,
	CAM_TYPE_SVIDEO,
	CAM_TYPE_COMPONENT,
	CAM_TYPE_AUX,
	CAM_TYPE_CMMB,
	CAM_TYPE_LVDS,
	CAM_TYPE_MAX
};

enum camera_enc {
	CAM_ENC_DEFAULT		= 0,
	CAM_ENC_NTSC		= 0,
	CAM_ENC_PAL,
	CAM_ENC_MAX
};

typedef struct {
	int (* close)(void);
	int (* open)(int camera_type, int camera_encode);
	int (* tune)(int camera_type, int camera_encode);
	int (* video_check)(void);
} SENSOR_FUNC_TYPE;

struct sensor_gpio {
	int pwr_port;	// gpio for power
	int pwd_port;	// gpio for power-down
	int rst_port;	// gpio for reset

	int hsync;	// gpio for Horizontal Sync
	int vsync;	// gpio for Vertical Sync
	int pclk;	// gpio for Pixel Clock
	
	int data[25];	// gpio for data
	int data_func;	// gpio function
};

struct sensor_reg {
	unsigned short reg;
	unsigned short val;
};

extern int sensor_if_init(int camera_type);
extern int sensor_if_close(void);
extern int sensor_if_open(int camera_type, int camera_encode);
extern int sensor_if_tune(int camera_type, int camera_encode);
extern int sensor_if_video_check(void);

#endif//__SENSOR_IF_H__

