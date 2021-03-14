
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
#include <platform/iomap.h>
#include <platform/reg_physical.h>
#include <platform/tcc_ckc.h>
#include <platform/CM4_earlycam.h>
#include <bitmap.h>
#include <dev/camera/camera.h>
#include <dev/camera/sensor_if.h>
#ifdef EARLY_CAM_MODULE_TW9921
#include <dev/camera/tw9921.h>
#endif
#ifdef EARLY_CAM_MODULE_TW9990
#include <dev/camera/tw9990.h>
#endif
#include <splash/splashimg.h>

#include <clock.h>
#include <platform/timer.h>
#include <kernel/thread.h>

#define SYNC_BASE_SIZE	0x1000
#define SYNC_BASE_ADDR	(HwCORTEXM4_DATA_MEM_BASE + HwCORTEXM4_DATA_MEM_SIZE - SYNC_BASE_SIZE)

static int DEBUG_EARLY_CAM = 0;

#include <tcc_lcd.h>
extern pmap_info_t gPmap[PMAP_MAX];

void tcc_cif_vioc_register_dump(void) {
	struct reg_test {
			unsigned int * reg;
			unsigned int cnt;
	};
	struct reg_test regList[] = {
//					{ (unsigned int *)HwVIOC_RDMA16,		12 },
					{ (unsigned int *)HwVIOC_VIN00, 		16 },
					{ (unsigned int *)HwVIOC_SC0,			 8 },
					{ (unsigned int *)HwVIOC_WMIX5, 		28 },
					{ (unsigned int *)HwVIOC_WDMA05,		18 },
					{ (unsigned int *)HwVIOC_RDMA05,		12 },
					{ (unsigned int *)HwVIOC_WMIX1, 		28 },
					{ (unsigned int *)HwVIOC_WMIX0, 		28 },
   };
	unsigned int * addr;
	unsigned int reg, idxLoop, nReg, idxReg;

	dprintf(INFO,"\n\n");
	for(idxLoop=0; idxLoop<sizeof(regList)/sizeof(regList[0]); idxLoop++) {
			addr	= regList[idxLoop].reg;
			nReg	= regList[idxLoop].cnt;
			for(idxReg=0; idxReg<nReg; idxReg++) {
					if((idxReg%4) == 0)
							dprintf(INFO,"\n%08x: ", (unsigned int)(addr + idxReg));

//					REGREAD((unsigned int)(addr + idxReg), reg);
					reg = *((volatile unsigned int *) (addr + idxReg));
					dprintf(INFO,"%08x ", reg);
			}
			dprintf(INFO,"\n");
	}
	dprintf(INFO,"\n\n");
}

void tcc_cif_delay(int ms) {
	volatile unsigned int msec = ms / 10; //10msec unit
	
	if(!msec)	mdelay(1);
	else		mdelay(msec);
}

void tcc_mbox_flush_buffers(void) {
	volatile MAILBOX	* pMbox;
	
	// flush mbox0
	pMbox = (volatile MAILBOX *)HwCORTEXM4_MAILBOX0_BASE;
	memset((void *)&pMbox->uMBOX_TX0, 0, sizeof(unsigned int) * 8);
	
	// flush mbox0
	pMbox = (volatile MAILBOX *)HwCORTEXM4_MAILBOX1_BASE;
	memset((void *)&pMbox->uMBOX_TX0, 0, sizeof(unsigned int) * 8);
}

int tcc_mbox_receive_msg(cm_ctrl_msg_t * msg) {
	volatile MAILBOX	* pMbox	= (volatile MAILBOX *)HwCORTEXM4_MAILBOX0_BASE;
	unsigned int		scount	= 0;
	
	// Data received
	// Wait until data received...
	// If "SEMP" is low, the message has been arrived and than "SCOUNT"
	// indicates the total number of received messages in word unit.
//	scount = (unsigned int)pMbox->uMBOX_STATUS.bREG.SCOUNT;
	scount = (unsigned int)((pMbox->uMBOX_CTL_017.nREG >> 20) & 0xF);
//	printf("[LK] %s - STATUS[0x%08x]: 0x%08x, SCOUNT: %d\n", __func__, (unsigned int)&pMbox->uMBOX_STATUS, (unsigned int)pMbox->uMBOX_STATUS.nREG, scount);
//	printf("[LK] %s - STATUS[0x%08x]: 0x%08x, SCOUNT: %d\n", __func__, (unsigned int)&pMbox->uMBOX_CTL_017, (unsigned int)pMbox->uMBOX_CTL_017.nREG, scount);
	if(scount != 8) {
		return -1;
	}
	
	// Read Data from FIFO
	// The sequence of read operation can be replaced to burst read to the receive fifo data region
	// After reading done, the empty status of the counter-part goes to "HIGH" means empty
	memcpy((void *)msg, (const void *)&pMbox->uMBOX_RX0, sizeof(cm_ctrl_msg_t));
	
	return 0;
}

unsigned int tcc_mbox_wait_receive_msg(cm_ctrl_msg_t * msg_to_check) {
	unsigned int	ack_to_check	= msg_to_check->cmd | MAILBOX_MSG_ACK;
	cm_ctrl_msg_t	msg_to_receive;
	volatile int	timeout = 100000;
	int		ret;
	
	do {
		ret = tcc_mbox_receive_msg(&msg_to_receive);
		if(!ret) {
			if(DEBUG_EARLY_CAM) {
				printf("[LK] Rx: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",	\
					msg_to_receive.preamble,	\
					msg_to_receive.data[5],		\
					msg_to_receive.data[4],		\
					msg_to_receive.data[3], 	\
					msg_to_receive.data[2],		\
					msg_to_receive.data[1],		\
					msg_to_receive.data[0],		\
					msg_to_receive.cmd);
			}
			if(msg_to_receive.cmd == ack_to_check)
				return 0;
		}
		mdelay(1);
	} while(0 < --timeout);
	return -1;
}

int tcc_mbox_send_msg(/*const */cm_ctrl_msg_t * msg) {
	volatile MAILBOX	* pMbox	 = (volatile MAILBOX *)HwCORTEXM4_MAILBOX0_BASE;
	volatile int		timeout =  100000;

	// set preamble
	msg->preamble	= MAILBOX_MSG_PREAMBLE;
	
	// Flush transmit FIFO or Wait until empty of transmit FIFO
	// Can be skipped by case
	
	// Set "OEN" to Low
	// To protect the invalid event which can be generated during writing data
	pMbox->uMBOX_CTL_016.bREG.OEN = 0;
	
	// Write Data to FIFO
	// The sequence of writing operation can be replaced to burst writing to the transmit fifo data region
	if(DEBUG_EARLY_CAM) {
		printf("[LK] Tx: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",      \
			msg->preamble,		\
			msg->data[5],		\
			msg->data[4],		\
			msg->data[3],		\
			msg->data[2],		\
			msg->data[1],		\
			msg->data[0],		\
			msg->cmd);
	}
	memcpy((void *)&pMbox->uMBOX_TX0, (const void *)msg, sizeof(cm_ctrl_msg_t));
	
	// Set "OEN" to HIGH
	// After this, the event(interrupt) for "SendMessage" will occur to counter-part
	pMbox->uMBOX_CTL_016.bREG.OEN = 1;
	
	// Check "MEMPTY" being true
#if 1
	while(((pMbox->uMBOX_CTL_017.nREG & 0x1) == 0) && timeout > 0) {
		mdelay(1);
//		if(DEBUG_EARLY_CAM) printf( "[LK] - pMbox->uMBOX_STATUS.bREG.MEMP == 0\n");
		timeout--;
	}

	if(timeout <= 0) {
		dprintf(INFO, "!@#---- %s - fail(timeout) \n", __func__);
		return -1;
	}
#else
	while(((pMbox->uMBOX_CTL_017.nREG & 0x1) == 0))
		dprintf(SPEW, "[CM] pMbox->uMBOX_STATUS.bREG.MEMP == 0\n");
#endif

	return 0;
}

void CM_MailBox_Configure(void) {
	volatile PMAILBOX pMbox_M	= (volatile PMAILBOX)HwCORTEXM4_MAILBOX0_BASE;
	volatile PMAILBOX pMbox_S	= (volatile PMAILBOX)HwCORTEXM4_MAILBOX1_BASE;
	volatile PCM_TSD_CFG pTSDCfg	= (volatile PCM_TSD_CFG)HwCORTEXM4_TSD_CFG_BASE;
	
	tcc_mbox_flush_buffers();
	/* Interrupt isn't used in Cortex-M4 */
//	BITSET(pMbox_M->uMBOX_CTL_016.nREG, Hw6|Hw5|Hw4|Hw1|Hw0);
//	BITSET(pMbox_S->uMBOX_CTL_016.nREG, Hw6|Hw5|Hw4|Hw1|Hw0);
	BITSET(pMbox_M->uMBOX_CTL_016.nREG, Hw6|Hw5);
	BITSET(pMbox_S->uMBOX_CTL_016.nREG, Hw6|Hw5);
	BITSET(pTSDCfg->IRQ_MASK_POL.nREG, Hw16|Hw22);
}

void CM_UnloadBinary(void) {
	volatile unsigned int * pCodeMem = (volatile unsigned int *)HwCORTEXM4_CODE_MEM_BASE;
	volatile PCM_TSD_CFG    pTSDCfg  = (volatile PCM_TSD_CFG   )HwCORTEXM4_TSD_CFG_BASE;
	
	// turn off
	BITSET(pTSDCfg->CM_RESET.nREG, Hw2|Hw1);
	
	// clear all code memory
	memset((void *)pCodeMem, 0, HwCORTEXM4_CODE_MEM_SIZE);	// CODE_MEM_SIZE == 64KB on CM4
}

void CM_LoadBinary(unsigned char * fw_data, unsigned int fw_size) {
	volatile unsigned int * pCodeMem = (volatile unsigned int *)HwCORTEXM4_CODE_MEM_BASE;
	volatile PCM_TSD_CFG    pTSDCfg  = (volatile PCM_TSD_CFG   )HwCORTEXM4_TSD_CFG_BASE;
	
	// unload and clear all code memory
	CM_UnloadBinary();
	
	//dprintf(INFO, "!@#---- %s - DST: 0x%08x, SRC: 0x%08x, SIZE: %d\n", __func__,	\
		(unsigned)pCodeMem, (unsigned)fw_data, (unsigned)fw_size);
	if((fw_data != NULL) && (0 < fw_size)) {
		// clear all memory
		memset((void *)pCodeMem, 0, (HwCORTEXM4_CODE_MEM_SIZE + HwCORTEXM4_DATA_MEM_SIZE) - SYNC_BASE_SIZE);
		// copy image to run
		memcpy((void *)pCodeMem, (void *)fw_data, fw_size);
	} else {
		dprintf(INFO,"Using previous loading the firmware\n");
	}
	
	// turn on
	BITCLR(pTSDCfg->CM_RESET.nREG, Hw2|Hw1);
}

extern struct tcc_cif_parameters * parameters_data;

void tcc_sync_parameters(void) {
	struct lcd_panel *panel_info;
	panel_info = tccfb_get_panel();

	// calculate pmap address
	parameters_data->Lcdc_address0  = gPmap[PMAP_EARLYCAM_PREVIEW].pbase;
	parameters_data->PGL_addr       = gPmap[PMAP_EARLYCAM_PGL].pbase;
	parameters_data->Viqe_area      = gPmap[PMAP_EARLYCAM_VIQE].pbase;

	// set width, height size
	parameters_data->viocmg_info.early_cam_preview_width    = panel_info->xres;
	parameters_data->viocmg_info.early_cam_preview_height   = panel_info->yres;

	parameters_data->viocmg_info.early_cam_parking_line_width   = panel_info->xres;
	parameters_data->viocmg_info.early_cam_parking_line_height  = panel_info->yres;

	dprintf(INFO, "parameters_data->size : %d \n", sizeof(struct tcc_cif_parameters));
	dprintf(INFO, "earlycam PGL base addr : 0x%x 0x%x \n", \
				parameters_data->PGL_addr,  gPmap[PMAP_EARLYCAM_PGL].pbase);
	dprintf(INFO, "earlycam PREVIEW base addr : 0x%x 0x%x \n", \
				parameters_data->Lcdc_address0, gPmap[PMAP_EARLYCAM_PREVIEW].pbase);
	dprintf(INFO, "earlycam VIQE base addr : 0x%x 0x%x \n", \
				parameters_data->Viqe_area, gPmap[PMAP_EARLYCAM_VIQE].pbase);
	dprintf(INFO, "early view width : %d, early view height : %d \n", \
	            parameters_data->viocmg_info.early_cam_preview_width, parameters_data->viocmg_info.early_cam_preview_height);

	memset((void *)SYNC_BASE_ADDR, 0, SYNC_BASE_SIZE);
	memcpy((void *)SYNC_BASE_ADDR, parameters_data, sizeof(struct tcc_cif_parameters));
}

void parking_guide_clear(void) {
	unsigned int parking_guide = parameters_data->PGL_addr;
	memset((void *)parking_guide, 0xff, (parameters_data->viocmg_info.early_cam_parking_line_width * parameters_data->viocmg_info.early_cam_parking_line_height * 4));
}

int set_parking_guide_loading_status(int onOff) {
	cm_ctrl_msg_t	msg;

	if(onOff) {
		// wati until receiving stop msg
		msg.cmd 	= MAILBOX_MSG_EARLYCAMERA_PGL;

		tcc_mbox_send_msg(&msg);
//		tcc_mbox_wait_receive_msg(&msg);
	}

	return 0;
}

unsigned int load_parking_guide(void) {
	ulong splah_width, splah_height;
	unsigned int ret;
	struct fbcon_config pgl_cfg;

	pgl_cfg.base = parameters_data->PGL_addr;

	printf("early_cam change parking line %d\r\n", parameters_data->viocmg_info.feature_early_view_use_parking_line);

	if(parameters_data->viocmg_info.feature_early_view_use_parking_line) {
		ret = splash_image_load(CONFIG_TCC_PARKING_GUIDE_LINE_NAME, &pgl_cfg);
		if(ret != -1) {
			if(pgl_cfg.width <= parameters_data->viocmg_info.early_cam_parking_line_width || pgl_cfg.height <= parameters_data->viocmg_info.early_cam_parking_line_height) {
				parameters_data->viocmg_info.early_cam_parking_line_width = pgl_cfg.width;
				parameters_data->viocmg_info.early_cam_parking_line_height = pgl_cfg.height;

				if(pgl_cfg.width < parameters_data->viocmg_info.early_cam_preview_width) {
					parameters_data->viocmg_info.early_cam_parking_line_x = (parameters_data->viocmg_info.early_cam_preview_width - pgl_cfg.width) >> 1;
				}
				if(pgl_cfg.height < parameters_data->viocmg_info.early_cam_preview_height) {
					parameters_data->viocmg_info.early_cam_parking_line_y = (parameters_data->viocmg_info.early_cam_preview_height - splah_height) >> 1;
				}

				((struct tcc_cif_parameters *)SYNC_BASE_ADDR)->viocmg_info.early_cam_parking_line_width = pgl_cfg.width;
				((struct tcc_cif_parameters *)SYNC_BASE_ADDR)->viocmg_info.early_cam_parking_line_height = pgl_cfg.height;
				((struct tcc_cif_parameters *)SYNC_BASE_ADDR)->viocmg_info.early_cam_parking_line_x = parameters_data->viocmg_info.early_cam_parking_line_x;
				((struct tcc_cif_parameters *)SYNC_BASE_ADDR)->viocmg_info.early_cam_parking_line_y = parameters_data->viocmg_info.early_cam_parking_line_y;

				set_parking_guide_loading_status(1);

				if(DEBUG_EARLY_CAM) printf("early_cam change parking line size to %dx%d\r\n",
											parameters_data->viocmg_info.early_cam_parking_line_width,
											parameters_data->viocmg_info.early_cam_parking_line_height);
			}
			else {
				((struct tcc_cif_parameters *)SYNC_BASE_ADDR)->viocmg_info.feature_early_view_use_parking_line = 0;
				parameters_data->viocmg_info.feature_early_view_use_parking_line = 0;
				printf("PGL is larger than lcd panel!! PGL is disabled...\n");
			}
		}
	}
}

int startEarlyCamera(void) {
	cm_ctrl_msg_t	msg;

	// decoder start
	sensor_if_open();
	tcc_cif_delay(10);
	
	// enable cm clk
	tcc_set_clkctrl(FBUS_CMBUS, ENABLE, 100000000);

	// syncronize parameters
	tcc_sync_parameters();
	
	// set mbox
	CM_MailBox_Configure();
	
	// load EarlyCamera image
	CM_LoadBinary(CM4_earlycam, sizeof(CM4_earlycam));
#if 0	
	// wati until receiving ready msg
	msg.cmd		= MAILBOX_MSG_EARLYCAMERA_READY;
	tcc_mbox_wait_receive_msg(&msg);
#endif	
	return 0;
}

int stopEarlyCamera(void) {
	cm_ctrl_msg_t	msg;
	
	// wati until receiving stop msg
	msg.cmd		= MAILBOX_MSG_EARLYCAMERA_STOP;
	tcc_mbox_send_msg(&msg);
	tcc_mbox_wait_receive_msg(&msg);
	
	// unload EarlyCamera image
	CM_UnloadBinary();
	
	// disable cm clk
//	tcc_set_clkctrl(FBUS_CMBUS, DISABLE, 1000000);
//	tcc_cif_delay(10);
	
	// decoder start
	sensor_if_close();
	
	return 0;
}

static int cm4_clk = 0;

long tcc_cm4_ctrl_ioctl(unsigned int cmd, unsigned long arg) {
#if defined(DAUDIO_KK_3HW_GPIO)
	switch (cmd) {
	case IOCTL_CM4_CTRL_OFF:
		if(cm4_clk)
		{
			stopEarlyCamera();
		}
		break;
			
	case IOCTL_CM4_CTRL_ON:
		//dprintf(INFO, "EarlyCamera - Start\n");	

		startEarlyCamera(); 
			
		cm4_clk = 1;
		break;
	}
#endif
	return 0;
}

