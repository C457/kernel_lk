/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <debug.h>
#include <smem.h>
//#include <msm_panel.h>
//#include <pm8x41.h>
//#include <pm8x41_wled.h>
#include <board.h>
//#include <mdp5.h>
#include <platform/gpio.h>
#include <target/display.h>
#include <dev/fbcon.h>
#include "../../app/aboot/recovery.h"
#include <daudio_settings.h>
#include <splash/splashimg.h>
#include <daudio_ver.h>
//static struct msm_fb_panel_data panel;
static uint8_t display_enable;

static struct fbcon_config *fb_config;

static void display_bootlogo(struct fbcon_config *fb_cfg)
{
	int snapshot_header = 0;
	int ret = -1;

	dprintf(INFO ,"display_bootlogo \n");

	VARIANT_CODE_INFO_t variant_code_info;

	snapshot_header = snapshot_boot_header();
	dprintf(INFO ,"snapshot_boot_header %d \n", snapshot_header);

	ret = read_variant_setting(&variant_code_info);
	if (ret == 0) {
		printf("[DEBUG] variant code -------------------------  \r\n");
		printf("[DEBUG] id : %x %x %x %x  \r\n",variant_code_info.id[0],variant_code_info.id[1],variant_code_info.id[2],variant_code_info.id[3]);
		printf("[DEBUG] version : %d  \r\n",variant_code_info.version);
		printf("[DEBUG] size : %d  \r\n",variant_code_info.size);
		printf("[DEBUG] variant code [0] : %d  \r\n",variant_code_info.variant_code[0]);
		printf("[DEBUG] variant code [1] : %d  \r\n",variant_code_info.variant_code[1]);
		printf("[DEBUG] --------------------------------------  \r\n");
	}
#if 1
//always display oem bootlogo(not making quickboot image)
	splash_image_load("bootlogo",fb_cfg);
#else 
	if (snapshot_header == -1) {
		struct recovery_message Msg;
		if(get_recovery_message(&Msg) == 0) {
			if(0 == strcmp("boot-recovery", Msg.command)) {
				dprintf(INFO ,"Not valid snapshot header, But recovery update \n");
				if ( (ret == 0)
						&& (variant_code_info.version == VARIANT_STRUCT_VERSION)
						&& (strncmp(variant_code_info.id,VARIANT_STRUCT_ID,4) == 0)) {
					if (variant_code_info.variant_code[0] == 1) {
						splash_image_load("bluelink",fb_cfg);
					} else {
						splash_image_load("bootlogo",fb_cfg);
					}
				} else {
					splash_image_load("bluelink",fb_cfg);
				}
			} else {
				dprintf(INFO ,"Not valid snapshot header \n");
				splash_image_load("quickboot",fb_cfg);
			}
		} else {
			dprintf(INFO ,"Not valid snapshot header \n");
			splash_image_load("quickboot",fb_cfg);
		}
	} else {
		dprintf(INFO ,"Valid snapshot header \n");

		if (ret == 0) {
			if ((variant_code_info.version == VARIANT_STRUCT_VERSION)
					&& (strncmp(variant_code_info.id,VARIANT_STRUCT_ID,4) == 0)) {
				if (variant_code_info.variant_code[0] == 1) {
					splash_image_load("bluelink",fb_cfg);
				} else {
					splash_image_load("bootlogo",fb_cfg);
				}
			} else {
				// not valid variant code data
				splash_image_load("bootlogo",fb_cfg);
			}
		} else {
			dprintf(INFO,"read_variant_setting error (%d) \r\n",ret);
			// not read emmc data
			splash_image_load("bootlogo",fb_cfg);
		}
	}
#endif
}

void display_init(void)
{
	uint32_t hw_id = 0 ;//board_hardware_id();
//	uint32_t soc_ver = board_soc_version();
	
	unsigned char lcd_ver = get_daudio_lcd_ver();
	
	dprintf(INFO, "display_init(),target_id=%d.\n", hw_id);

	switch(lcd_ver)
	{
		case 0:
		case 2:
		case 3:
		case 4:
		case 5:
		case 7:
			fb_config = lcdc_init_hdmi();
			break;
		case 6:
			fb_config = lcdc_init();
			break;
		case 10:
			if(gpio_get(TCC_GPB(24)))//OE hdmi no monitor
			{
				fb_config = lcdc_init_hdmi();
			}
			else//PIO lvds
			{
				fb_config = lcdc_init();
			}
			break;
			
		default:
			dprintf(INFO,"ADC value is wrong number : %d\n",lcd_ver);
			break;
	}	

	ASSERT(fb_config);
	fbcon_setup(fb_config);

#if !defined(DISPLAY_SPLASH_SCREEN_DIRECT)
	dprintf(INFO, "Splash Display int: start\n");
	//splash_image_load("bootlogo", fb_config);
#if defined(DEFAULT_DISPLAY_LCD)
	display_bootlogo(fb_config);
#endif
	display_splash_logo(fb_config);
	dprintf(INFO, "Splash Display int: end\n");
#endif

	display_enable = 1;
}

void display_shutdown(void)
{
	//if (display_enable)
	//	msm_display_off();
}
