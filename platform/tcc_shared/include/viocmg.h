#ifndef __VIOCMG_H__
#define __VIOCMG_H__

struct viocmg_info {
        // fbdisplay
        unsigned int main_display_id;
        unsigned int main_display_port;
        unsigned int main_display_ovp;  // default ovp

        // feature!
        unsigned int feature_early_view_enable;
        unsigned int feature_early_view_use_viqe;
        unsigned int feature_early_view_viqe_mode;      //0: VIOC_VIQE_DEINTL_MODE_BYPASS, 1: VIOC_VIQE_DEINTL_MODE_2D 2: VIOC_VIQE_DEINTL_MODE_3D
        unsigned int feature_early_view_use_parking_line;

        // camera hw
        unsigned int early_cam_cifport;
        unsigned int early_cam_vin_vin;
        unsigned int early_cam_vin_rdma;
        unsigned int early_cam_vin_wmix;
        unsigned int early_cam_vin_wdma;
        unsigned int early_cam_vin_scaler;
        unsigned int early_cam_display_rdma;
        unsigned int early_cam_gear_port;
        unsigned int early_cam_gear_port_active_level;
        
        // rear cam 
        unsigned int early_cam_mode; //0: normal mode, 1: rear cam mode   further use
        unsigned int early_cam_ovp;

        // rear cam preview
        unsigned int early_cam_preview_x;
        unsigned int early_cam_preview_y;

	 unsigned int early_cam_preview_crop_x;
	 unsigned int early_cam_preview_crop_y;
	 
        unsigned int early_cam_preview_width;
        unsigned int early_cam_preview_height;
        unsigned int early_cam_preview_format;
       
        unsigned int early_cam_preview_additional_width;
        unsigned int early_cam_preview_additional_height;

        // rear cam parking line
        unsigned int early_cam_parking_line_x;
        unsigned int early_cam_parking_line_y;
        unsigned int early_cam_parking_line_width;
        unsigned int early_cam_parking_line_height;
        unsigned int early_cam_parking_line_format;
};

extern struct viocmg_info viocmg_info;

extern void viocmg_dump(void);

#endif

