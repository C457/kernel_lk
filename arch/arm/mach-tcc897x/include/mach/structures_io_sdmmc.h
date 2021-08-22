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


/*
    Not finished block "SD, OVERLAY, AUDIO0/1, NFC, USBOTG"
*/

#ifndef _STRUCTURES_IO_H_
#define _STRUCTURES_IO_H_

/*******************************************************************************
*
*    TCC88x DataSheet PART 5 IO BUS
*
********************************************************************************/

/*******************************************************************************
*   3. SD/SDIO/MMC Host Controller                      (Base Addr = 0x76020000)
********************************************************************************/

typedef struct {
    unsigned SDMA           :32;
} SD_SDMA_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_SDMA_IDX_TYPE        bREG;
} SD_SDMA_TYPE;

typedef struct {
    unsigned BSIZE          :12;
    unsigned SDMABUS        :3;
    unsigned BSIZE2         :1;
} SD_BSIZE_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_BSIZE_IDX_TYPE       bREG;
} SD_BSIZE_TYPE;

typedef struct {
    unsigned BCNT           :16;
} SD_BCNT_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_BCNT_IDX_TYPE        bREG;
} SD_BCNT_TYPE;

typedef struct {
    unsigned ARG            :32;
} SD_ARG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_ARG_IDX_TYPE         bREG;
} SD_ARG_TYPE;

typedef struct {
    unsigned DMAEN          :1;
    unsigned BCNTEN         :1;
    unsigned ACMD12         :1;
    unsigned                :1;
    unsigned DIR            :1;
    unsigned MS             :1;
    unsigned ATACMD         :1;
    unsigned SPI            :1;
    unsigned                :8;
} SD_TMODE_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_TMODE_IDX_TYPE       bREG;
} SD_TMODE_TYPE;

typedef struct {
    unsigned RTYPE          :2;
    unsigned                :1;
    unsigned CRCHK          :1;
    unsigned CICHK          :1;
    unsigned DATSEL         :1;
    unsigned CTYPE          :2;
    unsigned CMDINDEX       :6;
    unsigned                :2;
} SD_CMD_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_CMD_IDX_TYPE         bREG;
} SD_CMD_TYPE;

typedef struct {
    unsigned RESP           :16;
} SD_RESP_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_RESP_IDX_TYPE        bREG;
} SD_RESP_TYPE;

typedef struct {
    unsigned DATA           :16;
} SD_DATA_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_DATA_IDX_TYPE        bREG;
} SD_DATA_TYPE;

typedef struct {
    unsigned NOCMD          :1;
    unsigned NODAT          :1;
    unsigned DATACT         :1;
    unsigned RTREQ          :1;
    unsigned                :4;
    unsigned WRACT          :1;
    unsigned RDACT          :1;
    unsigned WREN           :1;
    unsigned RDEN           :1;
    unsigned                :4;
} SD_STATEL_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_STATEL_IDX_TYPE      bREG;
} SD_STATEL_TYPE;

typedef struct {
    unsigned CDIN           :1;
    unsigned CDST           :1;
    unsigned SDCD           :1;
    unsigned SDWP           :1;
    unsigned DAT            :4;
    unsigned CMD            :1;
    unsigned DAT2           :4;
    unsigned                :3;
} SD_STATEH_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_STATEH_IDX_TYPE      bREG;
} SD_STATEH_TYPE;

typedef struct {
    unsigned LED            :1;
    unsigned SD4            :1;
    unsigned HS             :1;
    unsigned SELDMA         :2;
    unsigned SD8            :1;
    unsigned DETCD          :1;
    unsigned DETSEL         :1;
    unsigned POW            :1;
    unsigned                :3;
    unsigned HRST           :1;
    unsigned                :3;
} SD_CONTL_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_CONTL_IDX_TYPE       bREG;
} SD_CONTL_TYPE;

typedef struct {
    unsigned BGSTOP         :1;
    unsigned CONREQ         :1;
    unsigned RDWAIT         :1;
    unsigned BGINT          :1;
    unsigned SPI            :1;
    unsigned BTEN           :1;
    unsigned ABTEN          :1;
    unsigned                :1;
    unsigned WKINT          :1;
    unsigned WKIN           :1;
    unsigned WKOUT          :1;
    unsigned                :5;
} SD_CONTH_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_CONTH_IDX_TYPE       bREG;
} SD_CONTH_TYPE;

typedef struct {
    unsigned CLKEN          :1;
    unsigned CLKRDY         :1;
    unsigned SCKEN          :1;
    unsigned                :2;
    unsigned CGENSEL        :1;
    unsigned                :2;
    unsigned SDCLKSEL       :8;
} SD_CLK_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_CLK_IDX_TYPE         bREG;
} SD_CLK_TYPE;

typedef struct {
    unsigned TIMEOUT        :4;
    unsigned                :4;
    unsigned RSTALL         :1;
    unsigned RSTCMD         :1;
    unsigned RSTDAT         :1;
    unsigned                :5;
} SD_TIME_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_TIME_IDX_TYPE        bREG;
} SD_TIME_TYPE;

typedef struct {
    unsigned CDONE          :1;
    unsigned TDONE          :1;
    unsigned BLKGAP         :1;
    unsigned DMA            :1;
    unsigned WRRDY          :1;
    unsigned RDRDY          :1;
    unsigned CDIN           :1;
    unsigned CDOUT          :1;
    unsigned CDINT          :1;
    unsigned                :3;
    unsigned RTE            :1;
    unsigned BAR            :1;
    unsigned BEINT          :1;
    unsigned ERR            :1;
} SD_NOR_INT_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_NOR_INT_IDX_TYPE     bREG;
} SD_NOR_INT_TYPE;

typedef struct {
    unsigned CMDTIME        :1;
    unsigned CMDCRC         :1;
    unsigned CMDEND         :1;
    unsigned CINDEX         :1;
    unsigned DATTIME        :1;
    unsigned DATCRC         :1;
    unsigned DATEND         :1;
    unsigned                :1;
    unsigned ACMD12         :1;
    unsigned ADMA           :1;
    unsigned                :2;
    unsigned TRERR          :1;
    unsigned CRR            :1;
    unsigned VEND           :2;
} SD_ERR_INT_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_ERR_INT_IDX_TYPE     bREG;
} SD_ERR_INT_TYPE;

typedef struct {
    unsigned NORUN          :1;
    unsigned TIMEOUT        :1;
    unsigned CRC            :1;
    unsigned ENDBIT         :1;
    unsigned INDEX          :1;
    unsigned                :2;
    unsigned NOCMD          :1;
    unsigned                :8;
} SD_CMD12ERR_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_CMD12ERR_IDX_TYPE    bREG;
} SD_CMD12ERR_TYPE;

typedef struct {
    unsigned UHSSEL         :3;
    unsigned                :3;
    unsigned TUN            :1;
    unsigned SCSEL          :1;
    unsigned                :6;
    unsigned AIEN           :1;
    unsigned PREN           :1;
} SD_CONT2_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_CONT2_IDX_TYPE       bREG;
} SD_CONT2_TYPE;

typedef struct {
    unsigned TIMEOUTCLK     :6;
    unsigned                :1;
    unsigned TUNIT          :1;
    unsigned BASECLK        :8;
    unsigned MAXBLK         :2;
    unsigned EXTBUS         :1;
    unsigned ADMA2          :1;
    unsigned                :1;
    unsigned HS             :1;
    unsigned SDMA           :1;
    unsigned RESUME         :1;
    unsigned V33            :1;
    unsigned V30            :1;
    unsigned V18            :1;
    unsigned                :1;
    unsigned BUS64          :1;
    unsigned AIS            :1;
    unsigned SLTP           :2;
} SD_CAP1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_CAP1_IDX_TYPE        bREG;
} SD_CAP1_TYPE;

typedef struct {
    unsigned SDR50          :1;
    unsigned SDR104         :1;
    unsigned DDR50          :1;
    unsigned                :1;
    unsigned DRTA           :1;
    unsigned DRTC           :1;
    unsigned DRTD           :1;
    unsigned                :1;
    unsigned RTTCNT         :4;
    unsigned                :1;
    unsigned SDR50_2        :1;
    unsigned RTMODE         :2;
    unsigned CLKMULT        :8;
    unsigned SPI            :1;
    unsigned SPIBLK         :1;
    unsigned                :6;
} SD_CAP2_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_CAP2_IDX_TYPE        bREG;
} SD_CAP2_TYPE;

typedef struct {
    unsigned MAXCURV33      :8;
    unsigned MAXCURV30      :8;
    unsigned MAXCURV18      :8;
    unsigned                :8;
} SD_CURRENT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_CURRENT_IDX_TYPE     bREG;
} SD_CURRENT_TYPE;

typedef struct {
    unsigned NORUN          :1;
    unsigned TIMEOUT        :1;
    unsigned CRC            :1;
    unsigned ENDBIT         :1;
    unsigned INDEX          :1;
    unsigned                :2;
    unsigned NOCMD          :1;
    unsigned                :8;
    unsigned CMDTIME        :1;
    unsigned CMDCRC         :1;
    unsigned CMDEND         :1;
    unsigned CINDEX         :1;
    unsigned DATTIME        :1;
    unsigned DATCRC         :1;
    unsigned DATEND         :1;
    unsigned                :1;
    unsigned ACMD12         :1;
    unsigned ADMA           :1;
    unsigned                :2;
    unsigned TRERR          :1;
    unsigned CERR           :1;
    unsigned VEND           :2;
} SD_FORCE_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_FORCE_IDX_TYPE       bREG;
} SD_FORCE_TYPE;

typedef struct {
    unsigned ERRSTATE       :2;
    unsigned LEN            :1;
    unsigned                :29;
} SD_ADMAERR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_ADMAERR_IDX_TYPE     bREG;
} SD_ADMAERR_TYPE;

typedef struct {
    unsigned ADDR           :16;
} SD_ADMAADDR_IDX_TYPE;

typedef union {
    unsigned short          nREG;
    SD_ADMAADDR_IDX_TYPE    bREG;
} SD_ADMAADDR_TYPE;

typedef struct {
    unsigned SDCLKFREQ      :10;
    unsigned CG             :1;
    unsigned                :3;
    unsigned DRSTR          :2;
    unsigned SDCLKFREQ2     :10;
    unsigned CG2            :1;
    unsigned                :3;
    unsigned DRSTR2         :2;
} SD_PRESET_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    SD_PRESET_IDX_TYPE      bREG;
} SD_PRESET_TYPE;

typedef struct _SDHOST{
    volatile SD_SDMA_TYPE       SDMA;           // 0x000  R/W  0x00000000   SDMA System Address
    volatile SD_BSIZE_TYPE      BSIZE;          // 0x004  R/W  0x0000       Block Size
    volatile SD_BCNT_TYPE       BCNT;           // 0x006  R/W  0x0000       Block Count
    volatile SD_ARG_TYPE        ARG;            // 0x008  R/W  0x00000000   Argument
    volatile SD_TMODE_TYPE      TMODE;          // 0x00C  R/W  0x0000       Transfer Mode
    volatile SD_CMD_TYPE        CMD;            // 0x00E  R/W  0x0000       Command
    volatile SD_RESP_TYPE       RESP0;          // 0x010  R    0x0000       Response0
    volatile SD_RESP_TYPE       RESP1;          // 0x012  R    0x0000       Response1
    volatile SD_RESP_TYPE       RESP2;          // 0x014  R    0x0000       Response2
    volatile SD_RESP_TYPE       RESP3;          // 0x016  R    0x0000       Response3
    volatile SD_RESP_TYPE       RESP4;          // 0x018  R    0x0000       Response4
    volatile SD_RESP_TYPE       RESP5;          // 0x01A  R    0x0000       Response5
    volatile SD_RESP_TYPE       RESP6;          // 0x01C  R    0x0000       Response6
    volatile SD_RESP_TYPE       RESP7;          // 0x01E  R    0x0000       Response7
    volatile SD_DATA_TYPE       DATAL;          // 0x020  R/W  -            Buffer Data Port(Low)
    volatile SD_DATA_TYPE       DATAH;          // 0x022  R/W  -            Buffer Data Port(High)
    volatile SD_STATEL_TYPE     STATEL;         // 0x024  R    0x0000       Present State(Low)
    volatile SD_STATEH_TYPE     STATEH;         // 0x026  R    0x0000       Present State(High)
    volatile SD_CONTL_TYPE      CONTL;          // 0x028  R/W  0x0000       Power Control / Host Control
    volatile SD_CONTH_TYPE      CONTH;          // 0x02A  R/W  0x0000       Wakeup Control / Block Gap Control
    volatile SD_CLK_TYPE        CLK;            // 0x02C  R/W  0x0000       Clock Control
    volatile SD_TIME_TYPE       TIME;           // 0x02E  R/W  0x0000       Software Reset / Timeout Control
    volatile SD_NOR_INT_TYPE    STSL;           // 0x030  R    0x0000       Normal Interrupt Status
    volatile SD_ERR_INT_TYPE    STSH;           // 0x032  R    0x0000       Error Interrupt Status
    volatile SD_NOR_INT_TYPE    STSENL;         // 0x034  R/W  0x0000       Normal Interrupt Status Enable
    volatile SD_ERR_INT_TYPE    STSENH;         // 0x036  R/W  0x0000       Error Interrupt Status Enable
    volatile SD_NOR_INT_TYPE    INTENL;         // 0x038  R/W  0x0000       Normal Interrupt Signal Enable
    volatile SD_ERR_INT_TYPE    INTENH;         // 0x03A  R/W  0x0000       Error Interrupt Signal Enable
    volatile SD_CMD12ERR_TYPE   CMD12ERR;       // 0x03C  R    0x0000       Auto CMD12 Error Status
    volatile SD_CONT2_TYPE      CONT2;          // 0x03E  R/W  0x0000       Host Control2
    volatile SD_CAP1_TYPE       CAP1;           // 0x040  R    0x000030B0   Capabilities1
    volatile SD_CAP2_TYPE       CAP2;           // 0x044  R    0x00000000   Capabilities2
    volatile SD_CURRENT_TYPE    CURR;           // 0x048  R    0x00000000   Maximum Current Capabilities
    unsigned :32;
    volatile SD_FORCE_TYPE      FORCE;          // 0x050  R/W  0x00000000   Force event for AutoCmd12 Error
    volatile SD_ADMAERR_TYPE    ADMAERR;        // 0x054  R/W  0x00000000   AUDIO DMA Error Status
    volatile SD_ADMAADDR_TYPE   ADDR0;          // 0x058  R/W  0x0000       AUDIO DMA Address[15:00]
    volatile SD_ADMAADDR_TYPE   ADDR1;          // 0x05A  R/W  0x0000       AUDIO DMA Address[31:16]
    volatile SD_ADMAADDR_TYPE   ADDR2;          // 0x05C  R/W  0x0000       AUDIO DMA Address[47:32]
    volatile SD_ADMAADDR_TYPE   ADDR3;          // 0x05E  R/W  0x0000       AUDIO DMA Address[63:48]
    volatile SD_PRESET_TYPE     PRESET;         // 0x060  R    0x00000000   Preset Values
    unsigned :32; unsigned :32; unsigned :32;
    volatile TCC_DEF16BIT_TYPE  BTCONTL;        // 0x070  R/W  0x0000       Boot data timeout control(Low)
    volatile TCC_DEF16BIT_TYPE  BTCONTH;        // 0x072  R/W  0x0000       Boot data timeout control(High)
    volatile TCC_DEF16BIT_TYPE  DBGSEL;         // 0x074  R/W  0x0000       Debug Selection
    unsigned :16; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    unsigned :32; unsigned :32; unsigned :32; unsigned :32;
    volatile TCC_DEF16BIT_TYPE  SPIINT;         // 0x0F0  R    0x0000       SPI Interrupt Support
    unsigned :16; unsigned :32; unsigned :32;
    volatile TCC_DEF16BIT_TYPE  SLOT;           // 0x0FC  R    0x0000       Slot Interrupt Status
    volatile TCC_DEF16BIT_TYPE  VERSION;        // 0x0FE  R    0x0002       Host Controller Version
} SDHOST, *PSDHOST;

typedef struct _SDCHCTRL{
    volatile TCC_DEF32BIT_TYPE  SDCTRL;         // 0x000  R/W  0x00000000   Host Controller Control Register
    volatile TCC_DEF32BIT_TYPE  SDRESERVED;     // 0x004  R/W  0x00000000   Reserved Register
    volatile TCC_DEF32BIT_TYPE  SD0CMDDAT;      // 0x008  R/W  0x00000000   SD/MMC0 output delay control register
    volatile TCC_DEF32BIT_TYPE  SD1CMDDAT;      // 0x00C  R/W  0x00000000   SD/MMC1 output delay control register
    volatile TCC_DEF32BIT_TYPE  SD2CMDDAT;      // 0x010  R/W  0x00000000   SD/MMC2 output delay control register
    volatile TCC_DEF32BIT_TYPE  SD3CMDDAT;      // 0x014  R/W  0x00000000   SD/MMC3 output delay control register
    volatile TCC_DEF32BIT_TYPE  SD0CAPREG0;     // 0x018  R/W  0x00000000   SD/MMC0 Capabilities 0 register
    volatile TCC_DEF32BIT_TYPE  SD0CAPREG1;     // 0x01C  R/W  0x00000000   SD/MMC0 Capabilities 1 register
    volatile TCC_DEF32BIT_TYPE  SD0INITSPD;     // 0x020  R/W  0x00000000   SD/MMC0 Initialization & Default Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD0HIGHSPD;     // 0x024  R/W  0x00000000   SD/MMC0 High Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD0PRESET5;     // 0x028  R/W  0x00000000   SD/MMC0 Preset Register5
    volatile TCC_DEF32BIT_TYPE  SD0PRESET6;     // 0x02C  R/W  0x00000000   SD/MMC0 Preset Register6
    volatile TCC_DEF32BIT_TYPE  SD1CAPREG0;     // 0x030  R/W  0x00000000   SD/MMC1 Capabilities 0 register
    volatile TCC_DEF32BIT_TYPE  SD1CAPREG1;     // 0x034  R/W  0x00000000   SD/MMC1 Capabilities 1 register
    volatile TCC_DEF32BIT_TYPE  SD1INITSPD;     // 0x038  R/W  0x00000000   SD/MMC1 Initialization & Default Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD1HIGHSPD;     // 0x03C  R/W  0x00000000   SD/MMC1 High Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD1PRESET5;     // 0x040  R/W  0x00000000   SD/MMC1 Preset Register5
    volatile TCC_DEF32BIT_TYPE  SD1PRESET6;     // 0x044  R/W  0x00000000   SD/MMC1 Preset Register6
    volatile TCC_DEF32BIT_TYPE  SD2CAPREG0;     // 0x048  R/W  0x00000000   SD/MMC2 Capabilities 0 register
    volatile TCC_DEF32BIT_TYPE  SD2CAPREG1;     // 0x04C  R/W  0x00000000   SD/MMC2 Capabilities 1 register
    volatile TCC_DEF32BIT_TYPE  SD2INITSPD;     // 0x050  R/W  0x00000000   SD/MMC2 Initialization & Default Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD2HIGHSPD;     // 0x054  R/W  0x00000000   SD/MMC2 High Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD2PRESET5;     // 0x058  R/W  0x00000000   SD/MMC2 Preset Register5
    volatile TCC_DEF32BIT_TYPE  SD2PRESET6;     // 0x05C  R/W  0x00000000   SD/MMC2 Preset Register6
    volatile TCC_DEF32BIT_TYPE  SD3CAPREG0;     // 0x060  R/W  0x00000000   SD/MMC3 Capabilities 0 register
    volatile TCC_DEF32BIT_TYPE  SD3CAPREG1;     // 0x064  R/W  0x00000000   SD/MMC3 Capabilities 1 register
    volatile TCC_DEF32BIT_TYPE  SD3INITSPD;     // 0x068  R/W  0x00000000   SD/MMC3 Initialization & Default Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD3HIGHSPD;     // 0x06C  R/W  0x00000000   SD/MMC3 High Speed Config register
    volatile TCC_DEF32BIT_TYPE  SD3PRESET5;     // 0x070  R/W  0x00000000   SD/MMC3 Preset Register5
    volatile TCC_DEF32BIT_TYPE  SD3PRESET6;     // 0x074  R/W  0x00000000   SD/MMC3 Preset Register6
    volatile TCC_DEF32BIT_TYPE  SD01DELAY;      // 0x078  R/W  0x00000000   SD/MMC0/1 Clock delay controller
    volatile TCC_DEF32BIT_TYPE  SD23DELAY;      // 0x07C  R/W  0x00000000   SD/MMC2/3 Clock delay controller
} SDCHCTRL, *PSDCHCTRL;


/*******************************************************************************
*   4. EHI                                              (Base Addr = 0x76000000)
********************************************************************************/

typedef struct {
    unsigned ST             :7;
    unsigned RDY            :1;
    unsigned FOR            :1;
    unsigned FUR            :1;
    unsigned WFF            :1;
    unsigned WFNE           :1;
    unsigned RFNE           :1;
    unsigned                :19;
} EHI_STATUS_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_STATUS_IDX_TYPE     bREG;
} EHI_STATUS_TYPE;

typedef struct {
    unsigned IRQ            :1;
    unsigned IRQ_ST         :7;
    unsigned                :24;
} EHI_INT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_INT_IDX_TYPE        bREG;
} EHI_INT_TYPE;

typedef struct {
    unsigned                :2;
    unsigned EHA            :30;
} EHI_ADDR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_ADDR_IDX_TYPE       bREG;
} EHI_ADDR_TYPE;

typedef struct {
    unsigned EHD            :32;
} EHI_DATA_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_DATA_IDX_TYPE       bREG;
} EHI_DATA_TYPE;

typedef struct {
    unsigned FLG            :2;
    unsigned ST             :6;
    unsigned                :24;
} EHI_SEMAPHORE_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_SEMAPHORE_IDX_TYPE  bREG;
} EHI_SEMAPHORE_TYPE;

typedef struct {
    unsigned MD             :1;
    unsigned BW             :2;
    unsigned RDYE           :1;
    unsigned RDYP           :1;
    unsigned                :1;
    unsigned CSIRQ          :1;
    unsigned WIRQ           :1;
    unsigned                :24;
} EHI_CONFIG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_CONFIG_IDX_TYPE     bREG;
} EHI_CONFIG_TYPE;

typedef struct {
    unsigned EHIND          :8;
    unsigned                :24;
} EHI_INDEX_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_INDEX_IDX_TYPE      bREG;
} EHI_INDEX_TYPE;

typedef struct {
    unsigned BSIZE          :4;
    unsigned RW             :2;
    unsigned LK_            :1;
    unsigned AI             :1;
    unsigned                :24;
} EHI_RWCONTROL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EHI_RWCONTROL_IDX_TYPE  bREG;
} EHI_RWCONTROL_TYPE;

typedef struct _EHI{
    volatile EHI_STATUS_TYPE    EHST;           // 0x000  R/W  0x00000080   Status register
    volatile EHI_INT_TYPE       EHIINT;         // 0x004  R/W  0x00000000   Internal interrupt control register
    volatile EHI_INT_TYPE       EHEINT;         // 0x008  R/W  0x00000000   External interrupt control register
    volatile EHI_ADDR_TYPE      EHA;            // 0x00C  R    0x00000000   Address register
    volatile EHI_ADDR_TYPE      EHAM;           // 0x010  R/W  0x00000000   Address masking register
    volatile EHI_DATA_TYPE      EHD;            // 0x014  R/W  0x00000000   Data register
    volatile EHI_SEMAPHORE_TYPE EHSEM;          // 0x018  R/W  0x00000000   Semaphore register
    volatile EHI_CONFIG_TYPE    EHCFG;          // 0x01C  R/W  0x00000000   Configuration registers
    volatile EHI_INDEX_TYPE     EHIND;          // 0x020  R    0x00000000   Index register
    volatile EHI_RWCONTROL_TYPE EHRWCS;         // 0x024  R    0x00000000   Read/Write Control/Status register
} EHI, *PEHI;


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
    volatile unsigned int       NFC_CMD;        // 0x00   W                 NAND Flash Command Register
    volatile unsigned int       NFC_LADDR;      // 0x04   W                 NAND Flash Linear Address Register
    volatile unsigned int       NFC_SADDR;      // 0x08   W                 NAND Flash Signal Address Register
    volatile unsigned int       NFC_SDATA;      // 0x0C   R/W               NAND Flash Single Data Register
    volatile unsigned int       NFC_WDATA;      // 0x1x   R/W               NAND Flash Word Data Register
    volatile unsigned int       NOTDEFINE0[3];
    volatile unsigned int       NFC_LDATA;      // 0x2x/3x R/W      NAND Flash Linear Data Register
    volatile unsigned int       NOTDEFINE1[7];  // 0x01x  R/W       NAND Flash Word Data Register
    volatile unsigned int       NFC_MDATA;      // 0x40     R/W     NAND Flash Multiple Data Register
    volatile unsigned int       NFC_CACYC;      // 0x44     R/W     NAND Flash Command & Address Cycle Register
    volatile unsigned int       NFC_WRCYC;      // 0x48     R/W     NAND Flash Write Cycle Register
    volatile unsigned int       NFC_RECYC;      // 0x4C     R/W     NAND Flash Read Cycle Register
    volatile unsigned int       NFC_CTRL;       // 0x50     R/W     NFC Control Register
    volatile unsigned int       NFC_DSIZE;      // 0x54     R/W     NFC Data Size Register
    volatile unsigned int       NFC_SPARE;      // 0x58     R/W     NFC Spare Burst Transfer Control Register
    volatile unsigned int       NFC_PRSTART;    // 0x5C     R/W     NFC Burst Program/Read Start
    volatile unsigned int       NFC_RST;        // 0X60     R/W     NFC Reset Register
    volatile unsigned int       NFC_IRQCFG;     // 0x64     R/W     NFC Interrupt Request Control Register
    volatile unsigned int       NFC_IRQ;        // 0x68     R/W     NFC Interrupt Request Register
    volatile unsigned int       NFC_STATUS;     // 0x6C     R/W     NFC Stutus Register
    volatile unsigned int       NFC_RFWBASE;    // 0x70     R/W     NFC Read Flag Wait Base Word Register
	volatile unsigned int	NFC_RAND_CTRL;			// 0x74		R/W		NFC Randomizer Control Register
	volatile unsigned int	NFC_RAND_STATUS;		// 0x78		R/W		NFC Randomizer Status Register
	volatile unsigned int	NFC_DDR_CTRL;			// 0x7C		R/W		NFC DDR NAND Control Register
	volatile unsigned int	NFC_DDR_CYCLE;			// 0x80		R/W		NFC DDR NAND Cycle Register
	volatile unsigned int   NOTDEFINE2[31];         // 0x84~0xFC
    volatile unsigned int       NDMA_ADDR;      // 0X100    R/W     NFC DMA Source/Destination Register
    volatile unsigned int       NDMA_PARAM;     // 0X104    R/W     NFC DMA Parameter Register
    volatile unsigned int       NDMA_CADDR;     // 0X108    R       NFC DMA Current Address Register
    volatile unsigned int       NDMA_CTRL;      // 0x10C    R/W     NFC DMA Controll Register
    volatile unsigned int       NDMA_SPCTRL;    // 0x110    R/W     NFC DMA SUBPAGE Control Regiter
    volatile unsigned int       NDMA_STATUS;    // 0x114    R/W     NFC DMA Stutus Register
    volatile unsigned int       NOTDEFINE3[2];  // 0x118~011C
	volatile unsigned int   SP_CFG[32];             // 0x120    R/W     NFC Sub Page Configuration Register 
} NFC, *PNFC;

typedef struct _ECC{
    volatile unsigned int       ECC_CTRL;               // 0x00     W       NAND Flash Command Register
	volatile unsigned int   ECC_ERRSTATUS;			// 0x04     W       NAND Flash Linear Address Register
	volatile unsigned int   ECC_BASE;				// 0x08			 Reserved
    volatile unsigned int       ECC_MASK;                // 0x0C             Reserved 
    volatile unsigned int       ECC_CLEAR;
    volatile unsigned int       ECC_RSTART;
    volatile unsigned int       ECC_STLDCTL;
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
*   15. I2C Controller                                  (Base Addr = 0x76300000)
********************************************************************************/

typedef struct {
    unsigned PRES           :16;
    unsigned                :16;
} I2C_PRES_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2C_PRES_IDX_TYPE       bREG;
} I2C_PRES_TYPE;

typedef struct {
    unsigned                :5;
    unsigned MOD            :1;
    unsigned IEN            :1;
    unsigned EN             :1;
    unsigned                :24;
} I2C_CTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2C_CTRL_IDX_TYPE       bREG;
} I2C_CTRL_TYPE;

typedef struct {
    unsigned DATA           :16;
    unsigned                :16;
} I2C_TRANS_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2C_TRANS_IDX_TYPE      bREG;
} I2C_TRANS_TYPE;

typedef struct {
    unsigned IACK           :1;
    unsigned                :2;
    unsigned ACK            :1;
    unsigned WR             :1;
    unsigned RD             :1;
    unsigned STO            :1;
    unsigned STA            :1;
    unsigned                :24;
} I2C_CMD_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2C_CMD_IDX_TYPE        bREG;
} I2C_CMD_TYPE;

typedef struct {
    unsigned IF             :1;
    unsigned TIP            :1;
    unsigned                :3;
    unsigned AL             :1;
    unsigned BUSY           :1;
    unsigned RxACK          :1;
    unsigned                :24;
} I2C_SR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2C_SR_IDX_TYPE         bREG;
} I2C_SR_TYPE;

typedef struct {
    unsigned FC             :5;
    unsigned CKSEL          :1;
    unsigned                :2;
    unsigned RC             :8;
    unsigned                :16;
} I2C_TR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2C_TR_IDX_TYPE         bREG;
} I2C_TR_TYPE;

typedef struct _I2CMASTER{
    volatile I2C_PRES_TYPE      PRES;           // 0x000  R/W  0x0000FFFF   Clock Prescale register
    volatile I2C_CTRL_TYPE      CTRL;           // 0x004  R/W  0x00000000   Control Register
    volatile I2C_TRANS_TYPE     TXR;            // 0x008  W    0x00000000   Transmit Register
    volatile I2C_CMD_TYPE       CMD;            // 0x00C  W    0x00000000   Command Register
    volatile I2C_TRANS_TYPE     RXR;            // 0x010  R    0x00000000   Receive Register
    volatile I2C_SR_TYPE        SR;             // 0x014  R    0x00000000   Status register
    volatile I2C_TR_TYPE        TIME;           // 0x018  R/W  0x00000000   Timing Control Register
} I2CMASTER, *PI2CMASTER;

typedef struct {
    unsigned PORT           :8;
    unsigned                :8;
    unsigned RXVC           :3;
    unsigned                :5;
    unsigned TXVC           :3;
    unsigned                :5;
} I2CS_DPORT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_DPORT_IDX_TYPE     bREG;
} I2CS_DPORT_TYPE;

typedef struct {
    unsigned EN             :1;
    unsigned                :1;
    unsigned CLR            :1;
    unsigned SDA            :1;
    unsigned WS             :1;
    unsigned RCLR           :1;
    unsigned                :2;
    unsigned RXTH           :2;
    unsigned                :2;
    unsigned TXTH           :2;
    unsigned                :2;
    unsigned DRQEN          :4;
    unsigned FC             :5;
    unsigned                :5;
    unsigned SLV            :2;
} I2CS_CTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_CTRL_IDX_TYPE      bREG;
} I2CS_CTRL_TYPE;

typedef struct {
    unsigned ADDR           :8;
    unsigned                :24;
} I2CS_ADDR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_ADDR_IDX_TYPE      bREG;
} I2CS_ADDR_TYPE;

typedef struct {
    unsigned IRQEN          :12;
    unsigned                :4;
    unsigned IRQSTAT        :12;
    unsigned                :4;
} I2CS_INT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_INT_IDX_TYPE       bREG;
} I2CS_INT_TYPE;

typedef struct {
    unsigned SADR           :8;
    unsigned                :15;
    unsigned DDIR           :1;
    unsigned                :8;
} I2CS_STAT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_STAT_IDX_TYPE      bREG;
} I2CS_STAT_TYPE;

typedef struct {
    unsigned MBFR           :8;
    unsigned                :8;
    unsigned MBFT           :8;
    unsigned                :8;
} I2CS_MBF_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_MBF_IDX_TYPE       bREG;
} I2CS_MBF_TYPE;

typedef struct {
    unsigned DATA0          :8;
    unsigned DATA1          :8;
    unsigned DATA2          :8;
    unsigned DATA3          :8;
} I2CS_DATA0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_DATA0_IDX_TYPE     bREG;
} I2CS_DATA0_TYPE;

typedef struct {
    unsigned DATA4          :8;
    unsigned DATA5          :8;
    unsigned DATA6          :8;
    unsigned DATA7          :8;
} I2CS_DATA1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CS_DATA1_IDX_TYPE     bREG;
} I2CS_DATA1_TYPE;

typedef struct _I2CSLAVE{
    volatile I2CS_DPORT_TYPE    PORT;           // 0x000  R/W  -            Data Access port (TX/RX FIFO)
    volatile I2CS_CTRL_TYPE     CTL;            // 0x004  R/W  0x00000000   Control register
    volatile I2CS_ADDR_TYPE     ADDR;           // 0x008  W    0x00000000   Address register
    volatile I2CS_INT_TYPE      INT;            // 0x00C  W    0x00000000   Interrupt Enable Register
    volatile I2CS_STAT_TYPE     STAT;           // 0x010  R    0x00000000   Status Register
    unsigned :32; unsigned :32;
    volatile I2CS_MBF_TYPE      MBF;            // 0x01C  R/W  0x00000000   Buffer Valid Flag
    volatile I2CS_DATA0_TYPE    MB0;            // 0x020  R/W  0x00000000   Data Buffer (Byte 7 ~ 0)
    volatile I2CS_DATA1_TYPE    MB1;            // 0x024  R/W  0x00000000   Data Buffer (Byte 7 ~ 0)
} I2CSLAVE, *PI2CSLAVE;

typedef struct {
    unsigned MASTER0        :8;
    unsigned MASTER1        :8;
    unsigned MASTER2        :8;
    unsigned MASTER3        :8;
} I2CP_CFG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CP_CFG_IDX_TYPE       bREG;
} I2CP_CFG_TYPE;

typedef struct {
    unsigned SLAVE0         :8;
    unsigned SLAVE1         :8;
    unsigned                :16;
} I2CPS_CFG_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CPS_CFG_IDX_TYPE      bREG;
} I2CPS_CFG_TYPE;

typedef struct {
    unsigned M0             :1;
    unsigned M1             :1;
    unsigned M2             :1;
    unsigned M3             :1;
    unsigned S0             :1;
    unsigned S1             :1;
    unsigned                :26;
} I2CP_IRQ_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    I2CP_IRQ_IDX_TYPE       bREG;
} I2CP_IRQ_TYPE;

typedef struct _I2CPORTCFG{
    volatile I2CP_CFG_TYPE     PCFG0;           // 0x000  R/W  0x03020100   I2C Master Port Configuration
    volatile I2CPS_CFG_TYPE    PCFG1;           // 0x004  R/W  0x00000504   I2C Slave Port Configuration
    unsigned :32;
    volatile I2CP_IRQ_TYPE     ISTS;            // 0x00C  R    0x00000000   IRQ Status
} I2CPORTCFG, *PI2CPORTCFG;

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

/*******************************************************************************
*   23. IOBUS Configuration Register                    (Base Addr = 0x76066000)
********************************************************************************/

typedef struct {
    unsigned SEL            :32;
} IO_DMAREQSEL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    IO_DMAREQSEL_IDX_TYPE   bREG;
} IO_DMAREQSEL_TYPE;

typedef struct {
    unsigned EHI            :1;        // 00
    unsigned MPEFEC         :1;
    unsigned SDMMC0         :1;
    unsigned SDMMC1         :1;
    unsigned SDMMC2         :1;
    unsigned SDMMC3         :1;        // 05
    unsigned SDMMC_CTRL     :1;
    unsigned DMAC0          :1;
    unsigned DMAC1          :1;
    unsigned DMAC2          :1;
    unsigned DMAC_CH        :1;        // 10
    unsigned OVERLAY        :1;
    unsigned PWM            :1;
    unsigned SMC            :1;
    unsigned                :1;
    unsigned RTC            :1;        // 15
    unsigned REMOTE         :1;
    unsigned TSADC          :1;
    unsigned EDI            :1;
    unsigned EDI_CTRL       :1;
    unsigned SHOULD_BE_1    :1;        // 20
    unsigned ADMA0          :1;
    unsigned DAI0           :1;
    unsigned SPDIF0         :1;
    unsigned AUDIO0         :1;
    unsigned ADMA1          :1;        // 25
    unsigned DAI1           :1;
    unsigned SPDIF1         :1;
    unsigned AUDIO1         :1;
    unsigned I2CM0          :1;
    unsigned I2CM1          :1;        // 30
    unsigned I2CM2          :1;
} IO_HCLK0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    IO_HCLK0_IDX_TYPE       bREG;
} IO_HCLK0_TYPE;

typedef struct {
    unsigned I2CM3          :1;     // 0
    unsigned I2CS0          :1;
    unsigned I2CS1          :1;
    unsigned I2C_CTRL       :1;
    unsigned UART0          :1;
    unsigned UART1          :1;     // 5
    unsigned UART2          :1;
    unsigned UART3          :1;
    unsigned UART4          :1;
    unsigned UART5          :1;
    unsigned UART6          :1;     // 10
    unsigned UART7          :1;
    unsigned UART_CTRL      :1;
    unsigned IDE            :1;
    unsigned NFC            :1;
    unsigned TSIF0          :1;     // 15
    unsigned TSIF1          :1;
    unsigned TSIF2          :1;
    unsigned TSIF_CTRL      :1;
    unsigned GPSB0          :1;
    unsigned GPSB1          :1;     // 20
    unsigned GPSB2          :1;
    unsigned GPSB3          :1;
    unsigned GPSB4          :1;
    unsigned GPSB5          :1;
    unsigned GPSB_CTRL      :1;     // 25
    unsigned OTG            :1;
    unsigned                :5;
} IO_HCLK1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    IO_HCLK1_IDX_TYPE       bREG;
} IO_HCLK1_TYPE;

typedef struct {
    unsigned SG             :1;
    unsigned SR             :1;
    unsigned                :2;
    unsigned NS             :1;
    unsigned NP             :1;
    unsigned OS             :1;
    unsigned OP             :1;
    unsigned EP             :1;
    unsigned ES             :1;
    unsigned GS             :1;
    unsigned GP             :1;
    unsigned                :20;
} IO_MEMPWR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    IO_MEMPWR_IDX_TYPE      bREG;
} IO_MEMPWR_TYPE;

typedef struct {
    unsigned RTCWAIT        :3;
    unsigned                :29;
} IO_RTCWAIT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    IO_RTCWAIT_IDX_TYPE     bREG;
} IO_RTCWAIT_TYPE;

typedef struct {
    unsigned BW0            :1;
    unsigned A2XMOD0        :3;
    unsigned BW1            :1;
    unsigned A2XMOD1        :3;
    unsigned BW2            :1;
    unsigned A2XMOD2        :3;
    unsigned                :4;
    unsigned                :16;
} IO_A2X_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    IO_A2X_IDX_TYPE         bREG;
} IO_A2X_TYPE;

typedef struct _IOBUSCFG{
    volatile TCC_DEF32BIT_TYPE  USBOTG;         // 0x000  R/W  -            Refer to OTGCR in "14.2 Register Description for USB 2.0 OTG Controller"
    volatile IO_DMAREQSEL_TYPE  DMAREQSEL0;     // 0x004  R/W  -            DMA Request Selector for GDMA0 Controller0
    volatile IO_DMAREQSEL_TYPE  DMAREQSEL1;     // 0x008  R/W  -            DMA Request Selector for GDMA0 Controller1
    volatile IO_DMAREQSEL_TYPE  DMAREQSEL2;     // 0x00C  R/W  -            DMA Request Selector for GDMA0 Controller2
    volatile IO_HCLK0_TYPE      HCLKEN0;        // 0x010  R/W  0xFFFFFFFF   IOBUS AHB Clock Enable Register
    volatile IO_HCLK1_TYPE      HCLKEN1;        // 0x014  R/W  0xFFFFFFFF   IOBUS AHB Clock Enable Register
    volatile IO_HCLK0_TYPE      HRSTEN0;        // 0x018  R/W  0xFFFFFFFF   IOBUS AHB HRESET Control Register 0
    volatile IO_HCLK1_TYPE      HRSTEN1;        // 0x01C  R/W  0xFFFFFFFF   IOBUS AHB HRESET Control Register 1
    volatile IO_MEMPWR_TYPE     MEMPWR;         // 0x020  R/W  -            Memory Power Controll
    volatile IO_RTCWAIT_TYPE    RTCWAIT;        // 0x024  R/W  0x0000       RTC Wait Count
    unsigned : 32;     unsigned : 32;    unsigned : 32;    unsigned : 32;
    volatile IO_A2X_TYPE        IO_A2X;         // 0x038  R/W  0x00000077   IOBUS AHB2AXI Control Register
    unsigned : 32;     unsigned : 32;    unsigned : 32;    unsigned : 32;
    unsigned : 32;     unsigned : 32;    unsigned : 32;    unsigned : 32;
    unsigned : 32;
    volatile IO_DMAREQSEL_TYPE  DMAREQSEL3;     // 0x060  R/W  -            DMA Request Selector for GDMA1 Controller0
    volatile IO_DMAREQSEL_TYPE  DMAREQSEL4;     // 0x064  R/W  -            DMA Request Selector for GDMA1 Controller1

} IOBUSCFG, *PIOBUSCFG;
