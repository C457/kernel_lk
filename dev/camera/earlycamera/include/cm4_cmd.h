
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

#ifndef __TCC_MBOX_CTRL_H__
#define __TCC_MBOX_CTRL_H__

#define	MAILBOX_MSG_PREAMBLE				((unsigned int)0x0043414D)

#define MAILBOX_MSG_EARLYCAMERA_READY			((unsigned int)0x00000831)
#define MAILBOX_MSG_EARLYCAMERA_DISABLE_RECOVERY	((unsigned int)0x00000040)
#define MAILBOX_MSG_EARLYCAMERA_ENABLE_RECOVERY		((unsigned int)0x00000041)
#define MAILBOX_MSG_EARLYCAMERA_STOP			((unsigned int)0x00000051)
#define MAILBOX_MSG_EARLYCAMERA_EXIT			((unsigned int)0x00000052)
#define MAILBOX_MSG_EARLYCAMERA_PGL 			((unsigned int)0x00000053)
#define MAILBOX_MSG_EARLYCAMERA_PMAP			((unsigned int)0x00000054)
#define MAILBOX_MSG_EARLYCAMERA_KNOCK			((unsigned int)0x000000FF)
#define MAILBOX_MSG_ACK					((unsigned int)0x10000000)

typedef struct cm_ctrl_msg_t {
	unsigned int cmd;
	unsigned int data[6];
	unsigned int preamble;
} cm_ctrl_msg_t;

extern int tcc_mbox_init(void);
extern int tcc_mbox_receive_msg(cm_ctrl_msg_t * msg);
extern int tcc_mbox_send_msg(/*const */cm_ctrl_msg_t * msg);

#endif //__TCC_MBOX_CTRL_H__

