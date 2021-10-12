/*
 * Copyright (c) 2011 Telechips, Inc.
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

#ifndef _STRUCTURES_IO_H_
#define _STRUCTURES_IO_H_

/*******************************************************************************
*
*    TCC897x DataSheet PART 5 IO BUS
*
********************************************************************************/


/*******************************************************************************
*   5. DMA Controller                                   (Base Addr = 0x76030000)
********************************************************************************/

typedef struct {
    unsigned ADDR           :32;
} GDMA_ADDR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_ADDR_IDX_TYPE      bREG;
} GDMA_ADDR_TYPE;

typedef struct {
    unsigned INC            :8;
    unsigned MASK           :24;
} GDMA_PARAM_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_PARAM_IDX_TYPE     bREG;
} GDMA_PARAM_TYPE;

typedef struct {
    unsigned COUNT          :32;
} GDMA_COUNT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_COUNT_IDX_TYPE     bREG;
} GDMA_COUNT_TYPE;

typedef struct {
    unsigned EN             :1;
    unsigned REP            :1;
    unsigned IEN            :1;
    unsigned FLG            :1;
    unsigned WSIZE          :2;
    unsigned BSIZE          :2;
    unsigned TYPE           :2;
    unsigned BST            :1;
    unsigned LOCK           :1;
    unsigned HRD            :1;
    unsigned SYN            :1;
    unsigned DTM            :1;
    unsigned CONT           :1;
    unsigned                :16;
} GDMA_CHCTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_CHCTRL_IDX_TYPE    bREG;
} GDMA_CHCTRL_TYPE;

typedef struct {
    unsigned RPTCNT         :24;
    unsigned                :6;
    unsigned EOT            :1;
    unsigned DRI            :1;
} GDMA_RPTCTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_RPTCTRL_IDX_TYPE   bREG;
} GDMA_RPTCTRL_TYPE;

typedef struct {
    unsigned GPSB3_TX       :1;        // 0
    unsigned GPSB4_TX       :1;
    unsigned GPSB5_TX       :1;
    unsigned TSIF0          :1;
    unsigned GPSB3_RX       :1;
    unsigned GPSB4_RX       :1;        // 5
    unsigned GPSB5_RX       :1;
    unsigned TSIF1          :1;
    unsigned UART2_TX       :1;
    unsigned UART2_RX       :1;
    unsigned UART3_TX       :1;        // 10
    unsigned UART3_RX       :1;
    unsigned SPDIF1_TX_P    :1;
    unsigned SPDIF1_TX_U    :1;
    unsigned DAI1_TX        :1;
    unsigned DAI1_RX        :1;        // 15
    unsigned                :2;
    unsigned NFC            :1;
    unsigned I2C0_RX        :1;
    unsigned SPDIF0_TX_P    :1;        // 20
    unsigned SPDIF0_TX_U    :1;
    unsigned CDIF0_RX       :1;
    unsigned DAI0_TX        :1;
    unsigned DAI0_RX        :1;
    unsigned I2C0_TX        :1;        // 25
    unsigned UART0_TX       :1;
    unsigned UART0_RX       :1;
    unsigned SPDIF0_RX      :1;
    unsigned UART1_TX       :1;
    unsigned UART1_RX       :1;        // 30
    unsigned TSIF2          :1;
} GDMA_EXTREQ_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_EXTREQ_IDX_TYPE    bREG;
} GDMA_EXTREQ_TYPE;

typedef struct {
    unsigned FIX            :1;
    unsigned                :3;
    unsigned PRI            :3;
    unsigned                :1;
    unsigned SWP0           :1;
    unsigned SWP1           :1;
    unsigned SWP2           :1;
    unsigned                :5;
    unsigned MIS0           :1;
    unsigned MIS1           :1;
    unsigned MIS2           :1;
    unsigned                :1;
    unsigned IS0            :1;
    unsigned IS1            :1;
    unsigned IS2            :1;
    unsigned                :9;
} GDMA_CHCONFIG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GDMA_CHCONFIG_IDX_TYPE  bREG;
} GDMA_CHCONFIG_TYPE;

typedef struct _GDMACTRL{
    volatile GDMA_ADDR_TYPE     ST_SADR0;       // 0x000  R/W  0x00000000   Start Address of Source Block
    volatile GDMA_PARAM_TYPE    SPARAM0;        // 0x004  R/W  0x00000000   Parameter of Source Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_SADR0;        // 0x00C  R    0x00000000   Current Address of Source Block
    volatile GDMA_ADDR_TYPE     ST_DADR0;       // 0x010  R/W  0x00000000   Start Address of Destination Block
    volatile GDMA_PARAM_TYPE    DPARAM0;        // 0x014  R/W  0x00000000   Parameter of Destination Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_DADR0;        // 0x01C  R    0x00000000   Current Address of Destination Block
    volatile GDMA_COUNT_TYPE    HCOUNT0;        // 0x020  R/W  0x00000000   Initial and Current Hop count
    volatile GDMA_CHCTRL_TYPE   CHCTRL0;        // 0x024  R/W  0x00000000   Channel Control Register
    volatile GDMA_RPTCTRL_TYPE  RPTCTRL0;       // 0x028  R/W  0x00000000   Repeat Control Register
    volatile GDMA_EXTREQ_TYPE   EXTREQ0;        // 0x02C  R/W  0x00000000   External DMA Request Register
    volatile GDMA_ADDR_TYPE     ST_SADR1;       // 0x030  R/W  0x00000000   Start Address of Source Block
    volatile GDMA_PARAM_TYPE    SPARAM1;        // 0x034  R/W  0x00000000   Parameter of Source Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_SADR1;        // 0x03C  R    0x00000000   Current Address of Source Block
    volatile GDMA_ADDR_TYPE     ST_DADR1;       // 0x040  R/W  0x00000000   Start Address of Destination Block
    volatile GDMA_PARAM_TYPE    DPARAM1;        // 0x044  R/W  0x00000000   Parameter of Destination Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_DADR1;        // 0x04C  R    0x00000000   Current Address of Destination Block
    volatile GDMA_COUNT_TYPE    HCOUNT1;        // 0x050  R/W  0x00000000   Initial and Current Hop count
    volatile GDMA_CHCTRL_TYPE   CHCTRL1;        // 0x054  R/W  0x00000000   Channel Control Register
    volatile GDMA_RPTCTRL_TYPE  RPTCTRL1;       // 0x058  R/W  0x00000000   Repeat Control Register
    volatile GDMA_EXTREQ_TYPE   EXTREQ1;        // 0x05C  R/W  0x00000000   External DMA Request Register
    volatile GDMA_ADDR_TYPE     ST_SADR2;       // 0x060  R/W  0x00000000   Start Address of Source Block
    volatile GDMA_PARAM_TYPE    SPARAM2;        // 0x064  R/W  0x00000000   Parameter of Source Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_SADR2;        // 0x06C  R    0x00000000   Current Address of Source Block
    volatile GDMA_ADDR_TYPE     ST_DADR2;       // 0x070  R/W  0x00000000   Start Address of Destination Block
    volatile GDMA_PARAM_TYPE    DPARAM2;        // 0x074  R/W  0x00000000   Parameter of Destination Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_DADR2;        // 0x07C  R    0x00000000   Current Address of Destination Block
    volatile GDMA_COUNT_TYPE    HCOUNT2;        // 0x080  R/W  0x00000000   Initial and Current Hop count
    volatile GDMA_CHCTRL_TYPE   CHCTRL2;        // 0x084  R/W  0x00000000   Channel Control Register
    volatile GDMA_RPTCTRL_TYPE  RPTCTRL2;       // 0x088  R/W  0x00000000   Repeat Control Register
    volatile GDMA_EXTREQ_TYPE   EXTREQ2;        // 0x08C  R/W  0x00000000   External DMA Request Register
    volatile GDMA_CHCONFIG_TYPE CHCONFIG;       // 0x090  R/W  0x00000000   Channel Configuration Register
} GDMACTRL, *PGDMACTRL;

typedef struct _GDMANCTRL{
    volatile GDMA_ADDR_TYPE     ST_SADR;        // 0x000  R/W  0x00000000   Start Address of Source Block
    volatile GDMA_PARAM_TYPE    SPARAM;         // 0x004  R/W  0x00000000   Parameter of Source Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_SADR;         // 0x00C  R    0x00000000   Current Address of Source Block
    volatile GDMA_ADDR_TYPE     ST_DADR;        // 0x010  R/W  0x00000000   Start Address of Destination Block
    volatile GDMA_PARAM_TYPE    DPARAM;         // 0x014  R/W  0x00000000   Parameter of Destination Block
    unsigned :32;
    volatile GDMA_ADDR_TYPE     C_DADR;         // 0x01C  R    0x00000000   Current Address of Destination Block
    volatile GDMA_COUNT_TYPE    HCOUNT;         // 0x020  R/W  0x00000000   Initial and Current Hop count
    volatile GDMA_CHCTRL_TYPE   CHCTRL;         // 0x024  R/W  0x00000000   Channel Control Register
    volatile GDMA_RPTCTRL_TYPE  RPTCTRL;        // 0x028  R/W  0x00000000   Repeat Control Register
    volatile GDMA_EXTREQ_TYPE   EXTREQ;         // 0x02C  R/W  0x00000000   External DMA Request Register
} GDMANCTRL, *PGDMANCTRL;


/*******************************************************************************
*   6. GPSB                                             (Base Addr = 0x76900000)
********************************************************************************/

#if 1
typedef struct {
    unsigned DATA           :32;
} GPSB_PORT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PORT_IDX_TYPE      bREG;
} GPSB_PORT_TYPE;

typedef struct {
    unsigned RTH            :1;
    unsigned WTH            :1;
    unsigned RNE            :1;
    unsigned WE             :1;
    unsigned RF             :1;
    unsigned ROR            :1;
    unsigned WUR            :1;
    unsigned RUR            :1;
    unsigned WOR            :1;
    unsigned                :7;
    unsigned RBVCNT         :4;
    unsigned                :4;
    unsigned WBVCNT         :4;
    unsigned                :4;
} GPSB_STATUS_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_STATUS_IDX_TYPE    bREG;
} GPSB_STATUS_TYPE;

typedef struct {
    unsigned IRQEN          :9;
    unsigned                :6;
    unsigned RC             :1;
    unsigned CFGRTH         :3;
    unsigned                :1;
    unsigned CFGWTH         :3;
    unsigned                :1;
    unsigned SBR            :1;
    unsigned SHR            :1;
    unsigned SBT            :1;
    unsigned SHT            :1;
    unsigned                :2;
    unsigned DR             :1;
    unsigned DW             :1;
} GPSB_INTEN_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_INTEN_IDX_TYPE     bREG;
} GPSB_INTEN_TYPE;

typedef struct {
    unsigned MD             :2;
    unsigned SLV            :1;
    unsigned EN             :1;
    unsigned CTF            :1;
    unsigned SDO            :1;
    unsigned LB             :1;
    unsigned SD             :1;
    unsigned BWS            :5;
    unsigned                :1;
    unsigned CWF            :1;
    unsigned CRF            :1;
    unsigned PCK            :1;
    unsigned PRD            :1;
    unsigned PWD            :1;
    unsigned PCD            :1;
    unsigned PCS            :1;
    unsigned TSU            :1;
    unsigned THL            :1;
    unsigned TRE            :1;
    unsigned DIVLDV         :8;
} GPSB_MODE_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_MODE_IDX_TYPE      bREG;
} GPSB_MODE_TYPE;

typedef struct {
    unsigned PSW            :5;
    unsigned                :2;
    unsigned PLW            :1;
    unsigned RDSTART        :5;
    unsigned                :3;
    unsigned CMDSTART       :5;
    unsigned                :3;
    unsigned CMDEND         :5;
    unsigned                :1;
    unsigned LCR            :1;
    unsigned LCW            :1;
} GPSB_CTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_CTRL_IDX_TYPE      bREG;
} GPSB_CTRL_TYPE;

typedef struct {
    unsigned TXCV           :16;
    unsigned                :11;
    unsigned EXTDPOL        :1;
    unsigned EXTDCHK        :1;
    unsigned EXTEN          :1;
    unsigned TXCREP         :1;
    unsigned TXCRX          :1;
} GPSB_EVTCTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_EVTCTRL_IDX_TYPE   bREG;
} GPSB_EVTCTRL_TYPE;

typedef struct {
    unsigned TXC            :16;
    unsigned                :15;
    unsigned FSDI           :1;
} GPSB_CCV_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_CCV_IDX_TYPE       bREG;
} GPSB_CCV_TYPE;

typedef struct {
    unsigned TX_BASE        :32;
} GPSB_TX_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_TX_IDX_TYPE        bREG;
} GPSB_TX_TYPE;

typedef struct {
    unsigned RX_BASE        :32;
} GPSB_RX_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_RX_IDX_TYPE        bREG;
} GPSB_RX_TYPE;

typedef struct {
    unsigned SIZE           :13;
    unsigned                :3;
    unsigned COUNT          :13;
    unsigned                :3;
} GPSB_PACKET_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PACKET_IDX_TYPE    bREG;
} GPSB_PACKET_TYPE;

typedef struct {
    unsigned EN             :1;
    unsigned TSIF           :1;
    unsigned PCLR           :1;
    unsigned                :1;
    unsigned MD             :2;
    unsigned                :8;
    unsigned RXAM           :2;
    unsigned TXAM           :2;
    unsigned MS             :1;
    unsigned MP             :1;
    unsigned                :8;
    unsigned END            :1;
    unsigned CT             :1;
    unsigned DRE            :1;
    unsigned DTE            :1;
} GPSB_DMACTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_DMACTRL_IDX_TYPE   bREG;
} GPSB_DMACTRL_TYPE;

typedef struct {
    unsigned TXPCNT         :13;
    unsigned                :4;
    unsigned RXPCNT         :13;
    unsigned                :2;
} GPSB_DMASTAT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_DMASTAT_IDX_TYPE   bREG;
} GPSB_DMASTAT_TYPE;

typedef struct {
    unsigned IRQPCNT        :13;
    unsigned                :3;
    unsigned IEP            :1;
    unsigned IED            :1;
    unsigned                :2;
    unsigned IRQS           :1;
    unsigned                :7;
    unsigned ISP            :1;
    unsigned ISD            :1;
    unsigned                :2;
} GPSB_DMAICR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_DMAICR_IDX_TYPE    bREG;
} GPSB_DMAICR_TYPE;

typedef struct _GPSB{
    volatile GPSB_PORT_TYPE     PORT;           // 0x000  R/W  0x00000000   Data port
    volatile GPSB_STATUS_TYPE   STAT;           // 0x004  R/W  0x00000000   Status register
    volatile GPSB_INTEN_TYPE    INTEN;          // 0x008  R/W  0x00000000   Interrupt enable
    volatile GPSB_MODE_TYPE     MODE;           // 0x00C  R/W  0x00000004   Mode register
    volatile GPSB_CTRL_TYPE     CTRL;           // 0x010  R/W  0x00000000   Control register
    volatile GPSB_EVTCTRL_TYPE  EVTCTRL;        // 0x014  R/W  0x00000000   Counter & Ext. Event Control
    volatile GPSB_CCV_TYPE      CCV;            // 0x018  R    0x00000000   Counter Current Value
    unsigned :32;
    volatile GPSB_TX_TYPE       TXBASE;         // 0x020  R/W  0x00000000   TX base address register
    volatile GPSB_RX_TYPE       RXBASE;         // 0x024  R/W  0x00000000   RX base address register
    volatile GPSB_PACKET_TYPE   PACKET;         // 0x028  R/W  0x00000000   Packet register
    volatile GPSB_DMACTRL_TYPE  DMACTR;         // 0x02C  R/W  0x00000000   DMA control register
    volatile GPSB_DMASTAT_TYPE  DMASTR;         // 0x030  R/W  0x00000000   DMA status register
    volatile GPSB_DMAICR_TYPE   DMAICR;         // 0x034  R/W  0x00000000   DMA interrupt control register
} GPSB, *PGPSB, sHwGPSB;

typedef struct _NONDMA_GPSB{
    volatile GPSB_PORT_TYPE     PORT;           // 0x000  R/W  0x00000000   Data port
    volatile GPSB_STATUS_TYPE   STAT;           // 0x004  R/W  0x00000000   Status register
    volatile GPSB_INTEN_TYPE    INTEN;          // 0x008  R/W  0x00000000   Interrupt enable
    volatile GPSB_MODE_TYPE     MODE;           // 0x00C  R/W  0x00000004   Mode register
    volatile GPSB_CTRL_TYPE     CTRL;           // 0x010  R/W  0x00000000   Control register
    volatile GPSB_EVTCTRL_TYPE  EVTCTRL;        // 0x014  R/W  0x00000000   Counter & Ext. Event Control
    volatile GPSB_CCV_TYPE      CCV;            // 0x018  R    0x00000000   Counter Current Value
} NONDMA_GPSB, *PNONDMA_GPSB;

typedef struct {
    unsigned CH0            :8;
    unsigned CH1            :8;
    unsigned CH2            :8;
    unsigned CH3            :8;
} GPSB_PCFG0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PCFG0_IDX_TYPE     bREG;
} GPSB_PCFG0_TYPE;

typedef struct {
    unsigned CH4            :8;
    unsigned CH5            :8;
    unsigned                :16;
} GPSB_PCFG1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PCFG1_IDX_TYPE     bREG;
} GPSB_PCFG1_TYPE;

typedef struct {
    unsigned ISTC0          :1;
    unsigned ISTD0          :1;
    unsigned ISTC1          :1;
    unsigned ISTD1          :1;
    unsigned ISTC2          :1;
    unsigned ISTD2          :1;
    unsigned ISTC3          :1;
    unsigned                :1;
    unsigned ISTC4          :1;
    unsigned                :1;
    unsigned ISTC5          :1;
    unsigned                :21;
} GPSB_CIRQST_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_CIRQST_IDX_TYPE    bREG;
} GPSB_CIRQST_TYPE;

typedef struct _GPSBPORTCFG{
    volatile GPSB_PCFG0_TYPE    PCFG0;          // 0x800  R/W  0x03020100   Port Configuration Register 0
    volatile GPSB_PCFG1_TYPE    PCFG1;          // 0x800  R/W  0x00000504   Port Configuration Register 1
    unsigned :32;
    volatile GPSB_CIRQST_TYPE   CIRQST;         // 0x80C  R    0x00000000   Channel IRQ Status Register
} GPSBPORTCFG, *PGPSBPORTCFG;

typedef struct {
    unsigned PID            :13;
    unsigned                :16;
    unsigned CH0            :1;
    unsigned CH1            :1;
    unsigned CH2            :1;
} GPSB_PIDTBL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PIDTBL_IDX_TYPE    bREG;
} GPSB_PIDTBL_TYPE;

typedef struct _GPSBPIDTABLE{
    volatile GPSB_PIDTBL_TYPE   PIDT[0x80/4];   // 0xF00  R/W  -            PID Table
} GPSBPIDTABLE, *PGPSBPIDTABLE;
#else

typedef struct _GPSB{
    volatile unsigned int   PORT;               // 0x000 R/W 0x0000 Data port
    volatile unsigned int   STAT;               // 0x004 R/W 0x0000 Status register
    volatile unsigned int   INTEN;              // 0x008 R/W 0x0000 Interrupt enable
    volatile unsigned int   MODE;               // 0x00C R/W 0x0004 Mode register
    volatile unsigned int   CTRL;               // 0x010 R/W 0x0000 Control register
    volatile unsigned int   EVTCTRL;            // 0x014 R/W 0x0000 Counter & Ext. Event Control
    volatile unsigned int   CCV;                // 0x018 R 0x0000 Counter Current Value
    volatile unsigned int   NOTDEFINE0;
    volatile unsigned int   TXBASE;             // 0x020 R/W 0x0000 TX base address register
    volatile unsigned int   RXBASE;             // 0x024 R/W 0x0000 RX base address register
    volatile unsigned int   PACKET;             // 0x028 R/W 0x0000 Packet register
    volatile unsigned int   DMACTR;             // 0x02C R/W 0x0000 DMA control register
    volatile unsigned int   DMASTR;             // 0x030 R/W 0x0000 DMA status register
    volatile unsigned int   DMAICR;             // 0x034 R/W 0x0000 DMA interrupt control register
} GPSB, *PGPSB, sHwGPSB;

typedef struct _NONDMA_GPSB{
    volatile unsigned int   PORT;               // 0x000 R/W 0x0000 Data port
    volatile unsigned int   STAT;               // 0x004 R/W 0x0000 Status register
    volatile unsigned int   INTEN;              // 0x008 R/W 0x0000 Interrupt enable
    volatile unsigned int   MODE;               // 0x00C R/W 0x0004 Mode register
    volatile unsigned int   CTRL;               // 0x010 R/W 0x0000 Control register
    volatile unsigned int   EVTCTRL;            // 0x014 R/W 0x0000 Counter & Ext. Event Control
    volatile unsigned int   CCV;                // 0x018 R 0x0000 Counter Current Value
} NONDMA_GPSB, *PNONDMA_GPSB;

typedef struct _GPSBPORTCFG{
    volatile unsigned int   PCFG0;              // 0x800 R/W 0x03020100 Port Configuration Register 0
    volatile unsigned int   PCFG1;              // 0x804 R/W 0x00000504 Port Configuration Port Config Register 1
    volatile unsigned int   CIRQST;             // 0x808 R 0x0000 Channel IRQ Status Register
} GPSBPORTCFG, *PGPSBPORTCFG;

typedef struct _GPSBPIDTABLE{
    volatile unsigned int   PIDT[0x80/4]; // 0xF00 R/W   PID Table
} GPSBPIDTABLE, *PGPSBPIDTABLE;
#endif
/*******************************************************************************
*   etc
********************************************************************************/

typedef struct _NAND_EDI{
    volatile unsigned int   EDI_CTRL;           // 0x00  R/W   0x00000000   EDI Control Register.
    volatile unsigned int   EDI_CSNCFG0;        // 0x04  R/W   0x00543210   EDI CSN Configuration Register 0.
    volatile unsigned int   EDI_CSNCFG1;        // 0x08  R/W   0x00BA9876   EDI CSN Configuration Register 1.
    volatile unsigned int   EDI_WENCFG;         // 0x0C  R/W   0x00000004   EDI WEN Configuration Register
    volatile unsigned int   EDI_OENCFG;         // 0x10  R/W   0x00000004   EDI OEN Configuration Register
    volatile unsigned int   EDI_RDYCFG;         // 0x14  R/W   0x00043210   EDI Ready Configuration Register
    volatile unsigned int   _Reserved0;         // 0x18   -  -  -
    volatile unsigned int   _Reserved1;         // 0x1C   -  -
    volatile unsigned int   EDI_REQOFF;         // 0x20  R/W   0x00000000   EDI Request OFF Flag Register
} NAND_EDI, *NAND_PEDI;

/*******************************************************************************
*    7. DMA Controller Register Define (Base Addr = 0x76030000)
********************************************************************************/
typedef struct _NAND_GDMACTRL{
    volatile unsigned int  ST_SADR0;                // 0x00 R/W 0x00000000 Start Address of Source Block
    volatile unsigned int  SPARAM0;                 // 0x04 R/W 0x00000000 Parameter of Source Block
    volatile unsigned int  NOTDEFINE0;              // 0x08
    volatile unsigned int  C_SADR0;                 // 0x0C R   0x00000000 Current Address of Source Block
    volatile unsigned int  ST_DADR0;                // 0x10 R/W 0x00000000 Start Address of Destination Block
    volatile unsigned int  DPARAM0;                 // 0x14 R/W 0x00000000 Parameter of Destination Block
    volatile unsigned int  NOTDEFINE1;              // 0x18
    volatile unsigned int  C_DADR0;                 // 0x1C R   0x00000000 Current Address of Destination Block
    volatile unsigned int  HCOUNT0;                 // 0x20 R/W 0x00000000 Initial and Current Hop count
    volatile unsigned int  CHCTRL0;                 // 0x24 R/W 0x00000000 Channel Control Register
    volatile unsigned int  RPTCTRL0;                // 0x28 R/W 0x00000000 Repeat Control Register
    volatile unsigned int  EXTREQ0;                 // 0x2C R/W 0x00000000 External DMA Request Register
    volatile unsigned int  ST_SADR1;                // 0x30 R/W 0x00000000 Start Address of Source Block
    volatile unsigned int  SPARAM1;                 // 0x34 R/W 0x00000000 Parameter of Source Block
    volatile unsigned int  NOTDEFINE2;              // 0x38
    volatile unsigned int  C_SADR1;                 // 0x3C R   0x00000000 Current Address of Source Block
    volatile unsigned int  ST_DADR1;                // 0x40 R/W 0x00000000 Start Address of Destination Block
    volatile unsigned int  DPARAM1;                 // 0x44 R/W 0x00000000 Parameter of Destination Block
    volatile unsigned int  NOTDEFINE3;              // 0x48
    volatile unsigned int  C_DADR1;                 // 0x4C R   0x00000000 Current Address of Destination Block
    volatile unsigned int  HCOUNT1;                 // 0x50 R/W 0x00000000 Initial and Current Hop count
    volatile unsigned int  CHCTRL1;                 // 0x54 R/W 0x00000000 Channel Control Register
    volatile unsigned int  RPTCTRL1;                // 0x58 R/W 0x00000000 Repeat Control Register
    volatile unsigned int  EXTREQ1;                 // 0x5C R/W 0x00000000 External DMA Request Register
    volatile unsigned int  ST_SADR2;                // 0x60 R/W 0x00000000 Start Address of Source Block
    volatile unsigned int  SPARAM2;                 // 0x64 R/W 0x00000000 Parameter of Source Block
    volatile unsigned int  NOTDEFINE4;              // 0x68
    volatile unsigned int  C_SADR2;                 // 0x6C R   0x00000000 Current Address of Source Block
    volatile unsigned int  ST_DADR2;                // 0x70 R/W 0x00000000 Start Address of Destination Block
    volatile unsigned int  DPARAM2;                 // 0x74 R/W 0x00000000 Parameter of Destination Block
    volatile unsigned int  NOTDEFINE5;              // 0x78
    volatile unsigned int  C_DADR2;                 // 0x7C R   0x00000000 Current Address of Destination Block
    volatile unsigned int  HCOUNT2;                 // 0x80 R/W 0x00000000 Initial and Current Hop count
    volatile unsigned int  CHCTRL2;                 // 0x84 R/W 0x00000000 Channel Control Register
    volatile unsigned int  RPTCTRL2;                // 0x88 R/W 0x00000000 Repeat Control Register
    volatile unsigned int  EXTREQ2;                 // 0x8C R/W 0x00000000 External DMA Request Register
    volatile unsigned int  CHCONFIG;                // 0x90 R/W 0x00000000 Channel Configuration Register
} NAND_GDMACTRL, *NAND_PGDMACTRL;

typedef struct _NAND_GDMANCTRL{
    volatile unsigned int   ST_SADR;                // 0x000  R/W   Start Address of Source Block
    volatile unsigned int   SPARAM[2];              // 0x004~ R/W   Parameter of Source Block
    volatile unsigned int   C_SADR;                 // 0x00C  R     Current Address of Source Block
    volatile unsigned int   ST_DADR;                // 0x010  R/W   Start Address of Destination Block
    volatile unsigned int   DPARAM[2];              // 0x014~ R/W   Parameter of Destination Block
    volatile unsigned int   C_DADR;                 // 0x01C  R     Current Address of Destination Block
    volatile unsigned int   HCOUNT;                 // 0x020  R/W   Initial and Current Hop count
    volatile unsigned int   CHCTRL;                 // 0x024  R/W   Channel Control Register
    volatile unsigned int   RPTCTRL;                // 0x028  R/W   Repeat Control Register
    volatile unsigned int   EXTREQ;                 // 0x02C  R/W   External DMA Request Register
} NAND_GDMANCTRL, *NAND_PGDMANCTRL;

/*******************************************************************************
*   10. NFC                                             (Base Addr = 0x76600000)
********************************************************************************/
typedef struct _NFC{
   volatile unsigned int   NFC_CMD;                // 0x00     W       NAND Flash Command Register
	volatile unsigned int   NFC_LADDR;              // 0x04     W       NAND Flash Linear Address Register
	volatile unsigned int   NFC_SADDR;              // 0x08     W       NAND Flash Signal Address Register
	volatile unsigned int   NFC_SDATA;              // 0x0C     R/W     NAND Flash Single Data Register
	volatile unsigned int   NFC_WDATA;           	// 0x1x     R/W     NAND Flash Word Data Register
	volatile unsigned int	NOTDEFINE0[3];
	volatile unsigned int   NFC_LDATA;           	// 0x2x/3x R/W      NAND Flash Linear Data Register
	volatile unsigned int	NOTDEFINE1[7];			// 0x01x  R/W	NAND Flash Word Data Register
	volatile unsigned int   NFC_MDATA;              // 0x40     R/W     NAND Flash Multiple Data Register
	volatile unsigned int   NFC_CACYC;              // 0x44     R/W     NAND Flash Command & Address Cycle Register
	volatile unsigned int   NFC_WRCYC;              // 0x48     R/W     NAND Flash Write Cycle Register
	volatile unsigned int   NFC_RECYC;              // 0x4C     R/W     NAND Flash Read Cycle Register
	volatile unsigned int   NFC_CTRL;               // 0x50     R/W     NFC Control Register
	volatile unsigned int   NFC_DSIZE;              // 0x54     R/W     NFC Data Size Register
	volatile unsigned int   NFC_SPARE;              // 0x58     R/W     NFC Spare Burst Transfer Control Register
	volatile unsigned int   NFC_PRSTART;            // 0x5C     R/W     NFC Burst Program/Read Start
	volatile unsigned int   NFC_RST;                // 0X60     R/W     NFC Reset Register
	volatile unsigned int   NFC_IRQCFG;             // 0x64     R/W     NFC Interrupt Request Control Register
	volatile unsigned int   NFC_IRQ;                // 0x68     R/W     NFC Interrupt Request Register
	volatile unsigned int   NFC_STATUS;             // 0x6C     R/W     NFC Stutus Register
	volatile unsigned int   NFC_RFWBASE;            // 0x70     R/W     NFC Read Flag Wait Base Word Register
	volatile unsigned int	NFC_RAND_CTRL;			// 0x74		R/W		NFC Randomizer Control Register
	volatile unsigned int	NFC_RAND_STATUS;		// 0x78		R/W		NFC Randomizer Status Register
	volatile unsigned int	NFC_DDR_CTRL;			// 0x7C		R/W		NFC DDR NAND Control Register
	volatile unsigned int	NFC_DDR_CYCLE;			// 0x80		R/W		NFC DDR NAND Cycle Register
	volatile unsigned int   NOTDEFINE2[31];         // 0x84~0xFC
	volatile unsigned int   NDMA_ADDR;              // 0X100    R/W     NFC DMA Source/Destination Register
	volatile unsigned int   NDMA_PARAM;             // 0X104    R/W     NFC DMA Parameter Register
	volatile unsigned int   NDMA_CADDR;             // 0X108    R       NFC DMA Current Address Register
	volatile unsigned int   NDMA_CTRL;              // 0x10C    R/W     NFC DMA Controll Register
	volatile unsigned int   NDMA_SPCTRL;            // 0x110    R/W     NFC DMA SUBPAGE Control Regiter
	volatile unsigned int   NDMA_STATUS;            // 0x114    R/W     NFC DMA Stutus Register
	volatile unsigned int   NOTDEFINE3[2];          // 0x118~011C
	volatile unsigned int   SP_CFG[32];             // 0x120    R/W     NFC Sub Page Configuration Register 
} NFC, *PNFC;

typedef struct _ECC{
    volatile unsigned int   ECC_CTRL;				// 0x00     W       NAND Flash Command Register
	volatile unsigned int   ECC_ERRSTATUS;			// 0x04     W       NAND Flash Linear Address Register
	volatile unsigned int   ECC_BASE;				// 0x08			 Reserved
	volatile unsigned int   ECC_MASK;				// 0x0C			 Reserved
	volatile unsigned int   ECC_CLEAR;
	volatile unsigned int   ECC_RSTART;
	volatile unsigned int   ECC_STLDCTL;
	volatile unsigned int   ECC_CSTATE0;
	volatile unsigned int   ECC_CSTATE1;
	volatile unsigned int	NOTDEFINE1[3];
	volatile unsigned int	ECC_CODE0;				// 0x230  R/W	1st  ECC Code Register
	volatile unsigned int	ECC_CODE1;				// 0x234  R/W	2nd  ECC Code Register
	volatile unsigned int	ECC_CODE2;				// 0x238  R/W	3rd  ECC Code Register
	volatile unsigned int	ECC_CODE3;				// 0x23C  R/W	4th  ECC Code Register
	volatile unsigned int	ECC_CODE4;				// 0x240  R/W	5th  ECC Code Register
	volatile unsigned int	ECC_CODE5;				// 0x244  R/W	6th  ECC Code Register
	volatile unsigned int	ECC_CODE6;				// 0x248  R/W	7th  ECC Code Register
	volatile unsigned int	ECC_CODE7;				// 0x24C  R/W	8th  ECC Code Register
	volatile unsigned int	ECC_CODE8;				// 0x250  R/W	9th  ECC Code Register
	volatile unsigned int	ECC_CODE9;				// 0x254  R/W	10th  ECC Code Register
	volatile unsigned int	ECC_CODE10; 			// 0x258  R/W	11th  ECC Code Register
	volatile unsigned int	ECC_CODE11; 			// 0x25C  R/W	12th  ECC Code Register
	volatile unsigned int	ECC_CODE12; 			// 0x260  R/W	13th  ECC Code Register
	volatile unsigned int	ECC_CODE13; 			// 0x264  R/W	14th  ECC Code Register
	volatile unsigned int	ECC_CODE14; 			// 0x268  R/W	15th  ECC Code Register
	volatile unsigned int	ECC_CODE15; 			// 0x26C  R/W	16th  ECC Code register
	volatile unsigned int	ECC_CODE16; 			// 0x270  R/W	11th  ECC Code Register
	volatile unsigned int	ECC_CODE17; 			// 0x274  R/W	12th  ECC Code Register
	volatile unsigned int	ECC_CODE18; 			// 0x278  R/W	13th  ECC Code Register
	volatile unsigned int	ECC_CODE19; 			// 0x27C  R/W	14th  ECC Code Register
	volatile unsigned int	ECC_CODE20; 			// 0x280  R/W	15th  ECC Code Register
	volatile unsigned int	ECC_CODE21; 			// 0x284  R/W	16th  ECC Code register
	volatile unsigned int	ECC_CODE22; 			// 0x288  R/W	11th  ECC Code Register
	volatile unsigned int	ECC_CODE23; 			// 0x28C  R/W	12th  ECC Code Register
	volatile unsigned int	ECC_CODE24; 			// 0x290  R/W	13th  ECC Code Register
	volatile unsigned int	ECC_CODE25; 			// 0x294  R/W	14th  ECC Code Register
	volatile unsigned int	ECC_CODE26; 			// 0x298  R/W	15th  ECC Code Register
	volatile unsigned int	NOTDEFINE2[1];				// 0x29C  NULL
	volatile unsigned int	ECC_EADDR[60]; 			// 0x2A0 ~ 0x390 R  	ECC Error Address Register
	volatile unsigned int	NOTDEFINE3[(0x400-0x390)/4];			// 0x400  ~  0x13FF 
	volatile unsigned int	NFC_MEMORY[1024];		// 0x1000 ~ 0x17FF NFC MEMORY for Spare Area [NDMA Designation Memory]
} ECC, *PECC;

#endif /* _STRUCTURES_IO_H_ */


/*******************************************************************************
*   12. EDI                                             (Base Addr = 0x76065000)
********************************************************************************/

typedef struct {
    unsigned PRIORITY       :5;
    unsigned                :3;
    unsigned AM             :1;
    unsigned                :23;
} EDI_CTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_CTRL_IDX_TYPE       bREG;
} EDI_CTRL_TYPE;

typedef struct {
    unsigned CFGCS0         :4;
    unsigned CFGCS1         :4;
    unsigned CFGCS2         :4;
    unsigned CFGCS3         :4;
    unsigned CFGCS4         :4;
    unsigned CFGCS5         :4;
    unsigned                :8;
} EDI_CSNCFG0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_CSNCFG0_IDX_TYPE    bREG;
} EDI_CSNCFG0_TYPE;

typedef struct {
    unsigned CFGCS6         :4;
    unsigned CFGCS7         :4;
    unsigned CFGCS8         :4;
    unsigned CFGCS9         :4;
    unsigned CFGCS10        :4;
    unsigned CFGCS11        :4;
    unsigned                :8;
} EDI_CSNCFG1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_CSNCFG1_IDX_TYPE    bREG;
} EDI_CSNCFG1_TYPE;

typedef struct {
    unsigned CFGOEN0        :2;
    unsigned CFGOEN1        :2;
    unsigned                :28;
} EDI_OENCFG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_OENCFG_IDX_TYPE     bREG;
} EDI_OENCFG_TYPE;

typedef struct {
    unsigned CFGWEN0        :2;
    unsigned CFGWEN1        :2;
    unsigned                :28;
} EDI_WENCFG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_WENCFG_IDX_TYPE     bREG;
} EDI_WENCFG_TYPE;

typedef struct {
    unsigned CFGRDY0        :3;
    unsigned                :1;
    unsigned CFGRDY1        :3;
    unsigned                :1;
    unsigned CFGRDY2        :3;
    unsigned                :1;
    unsigned CFGRDY3        :3;
    unsigned                :1;
    unsigned CFGRDY4        :3;
    unsigned                :13;
} EDI_RDYCFG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_RDYCFG_IDX_TYPE     bREG;
} EDI_RDYCFG_TYPE;

typedef struct {
    unsigned REQOFF         :2;
    unsigned                :30;
} EDI_REQOFF_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EDI_REQOFF_IDX_TYPE     bREG;
} EDI_REQOFF_TYPE;

typedef struct _EDI{
    volatile EDI_CTRL_TYPE      EDI_CTRL;       // 0x000  R/W  0x00000000   EDI Control Register.
    volatile EDI_CSNCFG0_TYPE   EDI_CSNCFG0;    // 0x004  R/W  0x00543210   EDI CSN Configuration Register 0.
    volatile EDI_CSNCFG1_TYPE   EDI_CSNCFG1;    // 0x008  R/W  0x00BA9876   EDI CSN Configuration Register 1.
    volatile EDI_OENCFG_TYPE    EDI_OENCFG;     // 0x00C  R/W  0x00000004   EDI OEN Configuration Register
    volatile EDI_WENCFG_TYPE    EDI_WENCFG;     // 0x010  R/W  0x00000004   EDI WEN Configuration Register
    volatile EDI_RDYCFG_TYPE    EDI_RDYCFG;     // 0x014  R/W  0x00043210   EDI Ready Configuration Register
    unsigned :32; unsigned :32;
    volatile EDI_REQOFF_TYPE    EDI_REQOFF;     // 0x020  R/W  0x00000000   EDI Request OFF Flag Register
} EDI, *PEDI;

/*******************************************************************************
*   20. UART                                            (Base Addr = 0x76370000)
********************************************************************************/

typedef struct {
    unsigned DATA           :8;
    unsigned                :24;
} UART_PBR_IDX_TYPE;

typedef struct {
    unsigned DATA           :8;
    unsigned                :24;
} UART_THR_IDX_TYPE;

typedef struct {
    unsigned DIV            :8;
    unsigned                :24;
} UART_DLL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_PBR_IDX_TYPE       RBR;                // 0x000  R    Unknown      Receiver Buffer Register(DLAB = 0)
    UART_THR_IDX_TYPE       THR;                // 0x000  W    0x00000000   Transmitter Holding Register (DLAB=0)
    UART_DLL_IDX_TYPE       DLL;                // 0x000  R/W  0x00000000   Divisor Latch (LSB) (DLAB=1)
    volatile unsigned int  UTRXD;                 
    volatile unsigned int  UTTXD;                 
} UART_REG1_TYPE;

typedef struct {
    unsigned DIV            :8;
    unsigned                :24;
} UART_DLM_IDX_TYPE;

typedef struct {
    unsigned ERXI           :1;
    unsigned ETXI           :1;
    unsigned ELSI           :1;
    unsigned EMSI           :1;
    unsigned                :28;
} UART_IER_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_IER_IDX_TYPE       IER;                // 0x004  R/W  0x00000000   Interrupt Enable Register (DLAB=0)
    UART_DLM_IDX_TYPE       DLM;                // 0x004  R/W  0x00000000   Divisor Latch (MSB) (DLAB=1)
} UART_REG2_TYPE;

typedef struct {
    unsigned IPF            :1;
    unsigned IID            :3;
    unsigned                :23;
    unsigned STF            :1;
    unsigned                :4;
} UART_IIR_IDX_TYPE;

typedef struct {
    unsigned FE             :1;
    unsigned RXFR           :1;
    unsigned TXFR           :1;
    unsigned DRTE           :1;
    unsigned TXT            :2;
    unsigned RXT            :2;
    unsigned                :24;
} UART_FCR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_IIR_IDX_TYPE       IIR;                // 0x008  R    Unknown      Interrupt Ident. Register (DLAB=0)
    UART_FCR_IDX_TYPE       FCR;                // 0x008  W    0x000000C0   FIFO Control Register (DLAB=1)
} UART_REG3_TYPE;

typedef struct {
    unsigned WLS            :2;
    unsigned STB            :1;
    unsigned PEN            :1;
    unsigned EPS            :1;
    unsigned SP             :1;
    unsigned SB             :1;
    unsigned DLAB           :1;
    unsigned                :24;
} UART_LCR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_LCR_IDX_TYPE       bREG;
} UART_LCR_TYPE;

typedef struct {
    unsigned                :1;
    unsigned RTS            :1;
    unsigned                :2;
    unsigned LOOP           :1;
    unsigned AFE            :1;
    unsigned RS             :1;
    unsigned                :25;
} UART_MCR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_MCR_IDX_TYPE       bREG;
} UART_MCR_TYPE;

typedef struct {
    unsigned DR             :1;
    unsigned OE             :1;
    unsigned PE             :1;
    unsigned FE             :1;
    unsigned BI             :1;
    unsigned THRE           :1;
    unsigned TEMT           :1;
    unsigned ERF            :1;
    unsigned                :24;
} UART_LSR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_LSR_IDX_TYPE       bREG;
} UART_LSR_TYPE;

typedef struct {
    unsigned DCTS           :1;
    unsigned                :3;
    unsigned CTS            :1;
    unsigned                :27;
} UART_MSR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_MSR_IDX_TYPE       bREG;
} UART_MSR_TYPE;

typedef struct {
    unsigned DATA           :8;
    unsigned                :24;
} UART_SCR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_SCR_IDX_TYPE       bREG;
} UART_SCR_TYPE;

typedef struct {
    unsigned DTL            :4;
    unsigned ATL            :4;
    unsigned                :24;
} UART_AFT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_AFT_IDX_TYPE       bREG;
} UART_AFT_TYPE;

typedef struct {
    unsigned TxDE           :1;
    unsigned RxDE           :1;
    unsigned TWA            :1;
    unsigned RWA            :1;
    unsigned                :28;
} UART_UCR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_UCR_IDX_TYPE       bREG;
} UART_UCR_TYPE;

typedef struct {
    unsigned RxD            :8;
    unsigned                :24;
} UART_SRBR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_SRBR_IDX_TYPE      bREG;
} UART_SRBR_TYPE;

typedef union {
    unsigned long           nREG;
    UART_THR_IDX_TYPE       bREG;
} UART_STHR_TYPE;

typedef union {
    unsigned long           nREG;
    UART_DLL_IDX_TYPE       bREG;
} UART_SDLL_TYPE;

typedef union {
    unsigned long           nREG;
    UART_DLM_IDX_TYPE       bREG;
} UART_SDLM_TYPE;

typedef union {
    unsigned long           nREG;
    UART_IER_IDX_TYPE       bREG;
} UART_SIER_TYPE;

typedef struct {
    unsigned DIV            :16;
    unsigned                :10;
    unsigned STE            :1;
    unsigned STF            :1;
    unsigned P              :1;
    unsigned DEN            :1;
    unsigned STEN           :1;
    unsigned SEN            :1;
} UART_SCCR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_SCCR_IDX_TYPE      bREG;
} UART_SCCR_TYPE;

typedef struct {
    unsigned S_CNT          :16;
    unsigned C_CNT          :16;
} UART_STC_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_STC_IDX_TYPE       bREG;
} UART_STC_TYPE;

typedef struct {
    unsigned TXE            :1;
    unsigned RXE            :1;
    unsigned TXP            :1;
    unsigned RXP            :1;
    unsigned                :28;
} UART_IRCFG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_IRCFG_IDX_TYPE     bREG;
} UART_IRCFG_TYPE;

typedef struct _UART{
    volatile UART_REG1_TYPE     REG1;           // 0x000  PBR(R) / THR(W) / DLL(R/W)
    volatile UART_REG2_TYPE     REG2;           // 0x004  IER(R/W) / DLM(R/W)
    volatile UART_REG3_TYPE     REG3;           // 0x008  IIR(R) / FCR(W)
    volatile UART_LCR_TYPE      LCR;            // 0x00C  R/W  0x00000003   Line Control Register
    volatile UART_MCR_TYPE      MCR;            // 0x010  R/W  0x00000040   MODEM Control Register
    volatile UART_LSR_TYPE      LSR;            // 0x014  R    Unknown      Line Status Register
    volatile UART_MSR_TYPE      MSR;            // 0x018  R    Unknown      MODEM Status Register
    volatile UART_SCR_TYPE      SCR;            // 0x01C  R/W  0x00000000   Scratch Register
    volatile UART_AFT_TYPE      AFT;            // 0x020  R/W  0x00000000   AFC Trigger Level Register
    volatile UART_UCR_TYPE      UCR;            // 0x024  R/W  0x00000000   UART Control register
    unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    volatile UART_SRBR_TYPE     SRBR;           // 0x040  R    Unknown      Rx Buffer Register
    volatile UART_STHR_TYPE     STHR;           // 0x044  W    0x00000000   Transmitter Holding Register
    volatile UART_SDLL_TYPE     SDLL;           // 0x048  R/W  0x00000000   Divisor Latch (LSB)
    volatile UART_SDLM_TYPE     SDLM;           // 0x04C  R/W  0x00000000   Divisor Latch (MSB)
    volatile UART_SIER_TYPE     SIER;           // 0x050  R/W  0x00000000   Interrupt Enable register
    unsigned :32; unsigned :32; unsigned :32;
    volatile UART_SCCR_TYPE     SCCR;           // 0x060  R/W  0x00000000   Smart Card Control Register
    volatile UART_STC_TYPE      STC;            // 0x064  R/W  0x00000000   Smart Card TX Count register
    unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    volatile UART_IRCFG_TYPE    IRCFG;          // 0x080  R/W  0x00000000   IRDA Configuration Register
} UART, *PUART;

typedef struct {
    unsigned UART0          :8;
    unsigned UART1          :8;
    unsigned UART2          :8;
    unsigned UART3          :8;
} UART_PORTCFG0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_PORTCFG0_IDX_TYPE  bREG;
} UART_PORTCFG0_TYPE;

typedef struct {
    unsigned UART4          :8;
    unsigned UART5          :8;
    unsigned UART6          :8;
    unsigned UART7          :8;
} UART_PORTCFG1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_PORTCFG1_IDX_TYPE  bREG;
} UART_PORTCFG1_TYPE;

typedef struct {
    unsigned U0             :1;
    unsigned U1             :1;
    unsigned U2             :1;
    unsigned U3             :1;
    unsigned U4             :1;
    unsigned U5             :1;
    unsigned U6             :1;
    unsigned U7             :1;
    unsigned                :24;
} UART_ISTS_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    UART_ISTS_IDX_TYPE      bREG;
} UART_ISTS_TYPE;

typedef struct _UARTPORTCFG{
    volatile UART_PORTCFG0_TYPE PCFG0;          // 0x000  R/W  0x03020100   Port Configuration 0
    volatile UART_PORTCFG1_TYPE PCFG1;          // 0x004  R/W  0x07060504   Port Configuration 1
    unsigned :32;
    volatile UART_ISTS_TYPE     ISTS;           // 0x00C  R    0x00000000   IRQ Status
} UARTPORTCFG, *PUARTPORTCFG;

