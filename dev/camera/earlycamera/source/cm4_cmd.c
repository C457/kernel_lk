
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
#include <globals.h>
#include "cm4_cmd.h"
#include "delay.h"

int tcc_mbox_receive_msg(cm_ctrl_msg_t * msg) {
	volatile MAILBOX	* pMbox	= (volatile MAILBOX *)HwCORTEXM4_MAILBOX1_BASE;
	unsigned int		scount	= 0;

	// Data received
	// Wait until data received...
	// If "SEMP" is low, the message has been arrived and than "SCOUNT"
	// indicates the total number of received messages in word unit.
	scount = (unsigned int)pMbox->uMBOX_STATUS.bREG.SCOUNT;
//	printk("[CM] %s - STATUS[0x%08x]: 0x%08x, SCOUNT: %d\n", __func__, (unsigned int)&pMbox->uMBOX_STATUS, (unsigned int)pMbox->uMBOX_STATUS.nREG, scount);
	if(scount != 8) {
		return -1;
	}
	
	// Read Data from FIFO
	// The sequence of read operation can be replaced to burst read to the receive fifo data region
	// After reading done, the empty status of the counter-part goes to "HIGH" means empty
	memcpy((void *)msg, (const void *)&pMbox->uMBOX_RX0, sizeof(cm_ctrl_msg_t));
	return 0;
}

int tcc_mbox_send_msg(/*const */cm_ctrl_msg_t * msg) {
	volatile MAILBOX	* pMbox = (volatile MAILBOX *)HwCORTEXM4_MAILBOX1_BASE;
	volatile int		timeout =  5000;
	// set preamble
	msg->preamble	= MAILBOX_MSG_PREAMBLE;
	
	// Flush transmit FIFO or Wait until empty of transmit FIFO
	// Can be skipped by case
	
	// Set "OEN" to Low
	// To protect the invalid event which can be generated during writing data
	pMbox->uMBOX_CTL_016.bREG.OEN = 0;

	// Write Data to FIFO
	// The sequence of writing operation can be replaced to burst writing to the transmit fifo data region
	dprintf(SPEW, "[CM] Tx: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",      \
		msg->preamble,		\
		msg->data[5],		\
		msg->data[4],		\
		msg->data[3],		\
		msg->data[2],		\
		msg->data[1],		\
		msg->data[0],		\
		msg->cmd);
	memcpy((void *)&pMbox->uMBOX_TX0, (const void *)msg, sizeof(cm_ctrl_msg_t));

	// Set "OEN" to HIGH
	// After this, the event(interrupt) for "SendMessage" will occur to counter-part
	pMbox->uMBOX_CTL_016.bREG.OEN = 1;
	
	// Check "MEMPTY" being true
#if 1
	while(pMbox->uMBOX_STATUS.bREG.MEMP == 0 && timeout > 0) {
		mdelay(1);
		dprintf(SPEW, "[CM] pMbox->uMBOX_STATUS.bREG.MEMP == 0\n");
		timeout--;
	}

	if(timeout <= 0) {
		printk("[CM] tcc_mbox_send_msg - fail(timeout) \n");
		return -1;
	}
#else
	while(pMbox->uMBOX_STATUS.bREG.MEMP == 0)
		dprintf(SPEW, "[CM] pMbox->uMBOX_STATUS.bREG.MEMP == 0\n");
#endif

	return 0;
}

int tcc_mbox_init(void) {
	volatile MAILBOX	* pMbox	 = (volatile MAILBOX	 *)HwCORTEXM4_MAILBOX1_BASE;
	
	// MaliBox setting
	pMbox->uMBOX_CTL_016.bREG.FLUSH	= 1;
	pMbox->uMBOX_CTL_016.bREG.OEN	= 1;
	/* Interrupt isn't used in Cortex-M4 */
	//pMbox->uMBOX_CTL_016.bREG.IEN	= 1;
	//pMbox->uMBOX_CTL_016.bREG.LEVEL	= 3; // full : 8 x 4bytes fifo
	
	return 0;
}

