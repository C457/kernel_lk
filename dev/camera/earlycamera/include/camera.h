
/****************************************************************************
  Copyright (C) 2014 Telechips, Inc.

  This software is licensed under the terms of the GNU General Public
  License version 2, as published by the Free Software Foundation, and
  may be copied, distributed, and modified under those terms.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
****************************************************************************/
/****************************************************************************
   Written by S.W.Hwang (Platform Group BSP Team, Telechips Inc.)
****************************************************************************/

#ifndef __CAMERA_H__
#define	__CAMERA_H__

#define ON                              1
#define OFF                             0

#define ENABLE                          1
#define DISABLE                         0

#define ACT_HIGH                        1
#define ACT_LOW                         0

#define NEGATIVE_EDGE                   1
#define POSITIVE_EDGE                   0

struct viocmg_info
{
	// fbdisplay
	unsigned int main_display_id;
	unsigned int main_display_port;
	unsigned int main_display_ovp;	// default ovp

	// feature!
	unsigned int feature_early_view_enable;
	unsigned int feature_early_view_use_viqe;
	unsigned int feature_early_view_viqe_mode;		//0: VIOC_VIQE_DEINTL_MODE_BYPASS, 1: VIOC_VIQE_DEINTL_MODE_2D 2: VIOC_VIQE_DEINTL_MODE_3D
	unsigned int feature_early_view_use_parking_line;

	// VIOC Components
	unsigned int early_cam_cifport;
	unsigned int early_cam_vin_vin;
	unsigned int early_cam_vin_rdma;
	unsigned int early_cam_vin_wmix;
	unsigned int early_cam_vin_wdma;
	unsigned int early_cam_vin_scaler;
	unsigned int early_cam_display_rdma;

	// R-Gear
	unsigned int early_cam_gear_port;
	unsigned int early_cam_gear_port_active_level;

	// Early cam status
	unsigned int early_cam_mode; //0: normal mode, 1: rear cam mode   further use
	unsigned int early_cam_ovp;

	// Early cam preview
	unsigned int early_cam_preview_x;
	unsigned int early_cam_preview_y;

	unsigned int early_cam_preview_crop_x;
	unsigned int early_cam_preview_crop_y;

	unsigned int early_cam_preview_width;
	unsigned int early_cam_preview_height;
	unsigned int early_cam_preview_format;

	unsigned int early_cam_preview_additional_width;
	unsigned int early_cam_preview_additional_height;

	// Early cam parking line
	unsigned int early_cam_parking_line_x;
	unsigned int early_cam_parking_line_y;
	unsigned int early_cam_parking_line_width;
	unsigned int early_cam_parking_line_height;
	unsigned int early_cam_parking_line_format;
};


struct tcc_cif_parameters
{
	//Camera Parameters
	int Cam_p_clock_pol;
	int Cam_v_sync_pol;
	int Cam_h_sync_pol;
	int Cam_de_pol;
	int Cam_field_bfield_low;
	int Cam_gen_field_en;
	int Cam_conv_en;
	int Cam_hsde_connect_en;
	int Cam_vs_mask;
	int Cam_input_fmt;
	int Cam_data_order;
	int Cam_intl_en;
	int Cam_intpl_en;
	int Cam_format;
	int Cam_preview_w;
	int Cam_preview_h;

	//LCD Parameters
//	int Lcdc_Frame_width;
//	int Lcdc_Frame_height;
//	int Lcdc_Image_width;
//	int Lcdc_Image_height;
//	int Lcdc_offset_x;
//	int Lcdc_offset_y;
	unsigned int Lcdc_address0;
	unsigned int Lcdc_address1;
	unsigned int Lcdc_address2;
//	int Lcdc_format;

	int CIF_Port_num;
	
	unsigned int Viqe_area;
	unsigned int PGL_addr;
	
	unsigned int Camera_type;

	struct viocmg_info viocmg_info;

	unsigned char Cam_brightness;
	unsigned char Cam_contrast;
	unsigned char Cam_saturation;
};

extern void tcc_cif_start_stream(void);
extern void tcc_cif_stop_stream(void);
extern int R_Gear_Status_Check(void);
extern int tcc_cif_get_video(void);
extern char tcc_video_check(void);
extern unsigned int cm4_get_camera_type(void);
extern void tcc_cif_sync_parameter(void);
extern void tcc_cif_set_pgl(void);

extern struct viocmg_info * Viocmg_info;
extern int pgl_enable;

#endif

