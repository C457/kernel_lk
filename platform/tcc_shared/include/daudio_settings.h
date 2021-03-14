#ifndef __DAUDIO_SETTINGS__
#define __DAUDIO_SETTINGS__

#include "../../../app/aboot/libsnapshot.h"

#define SETTINGS_PARTITION		"settings"

/**
 * SETTING PARTITION STRUCTURE.
 *
 * ENGINEER MODE			0	
 **/
#define OFFSET_EM_SETTINGS					0
#define OFFSET_VARIANT_SETTINGS				OFFSET_EM_SETTINGS + 512
#define OFFSET_BOOT_ACTIVE_SETTINGS			OFFSET_VARIANT_SETTINGS + 512
#define OFFSET_IE_SETTINGS					OFFSET_BOOT_ACTIVE_SETTINGS + 512
#define VARIANT_STRUCT_VERSION				001
#define VARIANT_STRUCT_ID					"VARI"

struct ie_setting_info
{
	unsigned int id;
	unsigned int version;

	unsigned int tw8836_brightness;
	unsigned int tw8836_contrast;
	unsigned int tw8836_hue;
	unsigned int tw8836_saturation;

	unsigned int twxxxx_cam_brightness;
	unsigned int twxxxx_cam_contrast;
	unsigned int twxxxx_cam_hue;
	unsigned int twxxxx_cam_saturation;

	unsigned int tcc_brightness;
	unsigned int tcc_contrast;
	unsigned int tcc_hue;
	unsigned int tcc_saturation;

	unsigned int tcc_video_brightness;
	unsigned int tcc_video_contrast;
	unsigned int tcc_video_gamma;
	unsigned int tcc_video_saturation;

	unsigned int tcc_video_brightness2;
	unsigned int tcc_video_contrast2;
	unsigned int tcc_video_gamma2;
	unsigned int tcc_video_saturation2;

	/*GT system*/
	unsigned int tcc_dmb_brightness;
	unsigned int tcc_dmb_contrast;
	unsigned int tcc_dmb_gamma;
	unsigned int tcc_dmb_saturation;

	unsigned int tcc_cmmb_brightness;
	unsigned int tcc_cmmb_contrast;
	unsigned int tcc_cmmb_gamma;
	unsigned int tcc_cmmb_saturation;

	unsigned int tcc_usb_brightness;
	unsigned int tcc_usb_contrast;
	unsigned int tcc_usb_gamma;
	unsigned int tcc_usb_saturation;

	unsigned int temp_1;
	unsigned int temp_2;
	unsigned int temp_3;
	unsigned int temp_4;
};

typedef struct variant_code_info {
	unsigned char id[4];
	int version;
	int size;
	int reserved;
	int variant_code[8];
	// [0] - telematic , value : '0' - not telematic, '1' - telematic 
	// [1] ~ [7] - not used
} VARIANT_CODE_INFO_t;

#define EM_SETTING_ID		0x45444F4D5F4D45
#define EM_MODE_ENABLE		0x1
#define EM_MODE_DISABLE		0x0

struct em_setting_info {
	unsigned long long id;
	unsigned int mode;
	int temp;
};

struct quickboot_info {
	char sig[QB_SIG_SIZE];
	unsigned int addr;
};

struct board_adc_info {
	int hw_adc;
	int main_adc;
	int bt_adc;
	int gps_adc;
};

enum video_type {
	CVBS,
	S_VIDEO,
	COMPONENT,
	LVDS,
}VIDEO_TYPE_t;

enum cam_type {
	DIRECT_CAM,
	STEERING_CAM,
	SVM_CAM,
	PGS_CAM,
}CAM_TYPE_t;

/* Boot active partition */
struct boot_partition {
	char active_partition[32];
};

int read_ie_setting(struct ie_setting_info *info);
int write_ie_setting(struct ie_setting_info *info);

int read_em_setting(struct em_setting_info *info);
int read_variant_setting(VARIANT_CODE_INFO_t *variant_info);
int read_active_partition_setting(struct boot_partition *active_partition);
#endif
