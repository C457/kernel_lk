
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

#include <debug.h>
#include <string.h>

#include "reg_physical.h"
#include "cm4_cmd.h"
#include "camera.h"
#include "gpio.h"
#include "sensor_if.h"
#include "lcdc.h"
#include "delay.h"

void HWDMX_InitSystem(void) {
	volatile unsigned int nReg = 0;

	tcc_mbox_init();

	// Interrupt Enable
	REGREAD(0xe000e100, nReg);	nReg;	// enable
	REGREAD(0xe000e104, nReg);	nReg;
	REGREAD(0xe000e108, nReg);	nReg;

	REGREAD(0xe000e200, nReg);	nReg;	// pending
	REGREAD(0xe000e204, nReg);	nReg;
	REGREAD(0xe000e208, nReg);	nReg;

	REGSET(0xe000e180, 0xffffffff); // clear irq enable 0~31
	REGSET(0xe000e184, 0xffffffff); // clear irq enable
	REGSET(0xe000e188, 0xffffffff); // clear irq enable

	REGSET(0xe000e280, 0xffffffff); // clear irq pending 0~31
	REGSET(0xe000e284, 0xffffffff); // clear irq pending
	REGSET(0xe000e288, 0xffffffff); // clear irq pending

	REGSET(0xe000e100, 0x000007ff); // 0~31 , Not enable FIQ, IRQ
	
	REGREAD(0xe000e100, nReg);	nReg;
}

int main(void) {
	cm_ctrl_msg_t	msg;
	
	int		recovery_check_enabled = 0;
	int		isGearOn, video_signal, preGearStatus = 0;
	int		ret = -1;
	
	HWDMX_InitSystem();
	
	FUNCTION_IN
	
	tcc_cif_sync_parameter();
	
	sensor_if_init(cm4_get_camera_type());
//	sensor_if_open(cm4_get_camera_type(), CAM_ENC_DEFAULT);
	
	tcc_cif_start_stream();
	tcc_cif_ie_lut();
	
#if 0//defined(PLATFORM_TCC802X)
	// notify that EarlyCamera is ready to run
	memset((void *)&msg, 0, sizeof(cm_ctrl_msg_t));
	msg.preamble	= MAILBOX_MSG_PREAMBLE;
	msg.cmd		= MAILBOX_MSG_EARLYCAMERA_READY | MAILBOX_MSG_ACK;
	tcc_mbox_send_msg(&msg);
#endif//defined(PLATFORM_TCC802X)
	
	while(1) {
		isGearOn = R_Gear_Status_Check();
//		printk("R-Gear Status(%d). \n", isGearOn);
		if(isGearOn != preGearStatus) {
			preGearStatus = isGearOn;
			printk("R-Gear Status(%d). \n", isGearOn);
		}

//		tcc_cif_vioc_register_dump();

		if(isGearOn) {
			if(sensor_if_video_check() == 0)
				video_signal = tcc_cif_get_video();
			else
				video_signal = 0;

			if(video_signal)
				tcc_rear_camera_display(1);
			else {
	//			printk("video_signal Status(%d). \n", video_signal);
				tcc_rear_camera_display(0);
			}
		} else {
			tcc_rear_camera_display(0);
		}

		ret = tcc_mbox_receive_msg(&msg);
		if(!ret) {
			switch(msg.cmd) {
			case MAILBOX_MSG_EARLYCAMERA_STOP:
				//printk("MAILBOX_MSG_EARLYCAMERA_STOP\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_STOP\n");

				tcc_rear_camera_display(OFF);
				tcc_rear_camera_wmix_channel_prioty();
				tcc_cif_stop_stream();
				sensor_if_close();
				
				msg.data[0]	= Viocmg_info->early_cam_mode;
				dprintf(SPEW, "!@#---- EarlyCamera - gear_status: 0x%08x\n", msg.data[0]);
				msg.cmd		= (MAILBOX_MSG_EARLYCAMERA_STOP | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);
				mdelay(100);
				
				return 0;
				
			case MAILBOX_MSG_EARLYCAMERA_EXIT:
				//printk("MAILBOX_MSG_EARLYCAMERA_EXIT\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_EXIT\n");

				msg.data[0]	= Viocmg_info->early_cam_mode;
				dprintf(SPEW, "!@#---- EarlyCamera - gear_status: 0x%08x\n", msg.data[0]);
				msg.cmd		= (MAILBOX_MSG_EARLYCAMERA_EXIT | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);
				mdelay(100);
				
				return 0;
				
			case MAILBOX_MSG_EARLYCAMERA_DISABLE_RECOVERY:
				//printk("MAILBOX_MSG_EARLYCAMERA_DISABLE_RECOVERY\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_DISABLE_RECOVERY\n");

				recovery_check_enabled = 0;
				msg.cmd = (MAILBOX_MSG_EARLYCAMERA_DISABLE_RECOVERY | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);
				
				break;
				
			case MAILBOX_MSG_EARLYCAMERA_ENABLE_RECOVERY:
				//printk("MAILBOX_MSG_EARLYCAMERA_ENABLE_RECOVERY\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_ENABLE_RECOVERY\n");

				recovery_check_enabled = 1;
				msg.cmd = (MAILBOX_MSG_EARLYCAMERA_ENABLE_RECOVERY | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);
				
				break;
				
			case MAILBOX_MSG_EARLYCAMERA_PGL:
				//printk("MAILBOX_MSG_EARLYCAMERA_PGL\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_PGL\n");
				
				if(!pgl_enable) {
					pgl_enable = 1;
					tcc_cif_set_pgl();
					
					dprintf(SPEW, "PGL enable!\n");
				}
#if 0
				msg.cmd = (MAILBOX_MSG_EARLYCAMERA_PGL | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);
#endif
				break;

			case MAILBOX_MSG_EARLYCAMERA_PMAP:
				//printk("MAILBOX_MSG_EARLYCAMERA_PMAP\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_PMAP\n");

				tcc_rear_camera_display(OFF);
				tcc_cif_stop_stream();
//				mdelay(20);
				tcc_cif_start_stream();

				msg.cmd = (MAILBOX_MSG_EARLYCAMERA_PMAP | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);

				break;

			case MAILBOX_MSG_EARLYCAMERA_KNOCK:
				//printk("MAILBOX_MSG_EARLYCAMERA_KNOCK\n");
				dprintf(SPEW, "MAILBOX_MSG_EARLYCAMERA_KNOCK\n");
				
				msg.cmd = (MAILBOX_MSG_EARLYCAMERA_KNOCK | MAILBOX_MSG_ACK);
				tcc_mbox_send_msg(&msg);
				
				break;
				
			default:
				//printk("The message(0x%08x) from Mailbox is wrong.\n", msg.cmd);
				dprintf(SPEW, "The message(0x%08x) from Mailbox is wrong.\n", msg.cmd);
				break;
			}
#if 0
			// Check WDMA current address and if it doesn't change, recovery early camera
			if(!tcc_cif_wdma_counter()) {
				log("Early camera recovery!! \n");
				tcc_cif_stop_stream();
				mdelay(20);
				sensor_if_open();
				tcc_cif_start_stream();
			}
#endif
		}
//		mdelay(100);
	}
	FUNCTION_OUT
	return 0;
}

