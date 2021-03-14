/****************************************************************************
 *  FileName    : structures_display.h
 *  Description :
 ****************************************************************************
 *
 *  Copyright (C) 2014 Telechips, Inc.
 *  All Rights Reserved
 *
 ****************************************************************************/

#ifndef _STRUCTURES_DISPLAY_H_
#define _STRUCTURES_DISPLAY_H_

/*
	Not finished block "VIOC, NTSC/PAL, HDMI"
*/

/*******************************************************************************
*
*    TCC802x DataSheet PART 7 DISPLAY BUS
*
********************************************************************************/

/************************************************************************
*   3. VIOC (Video Input/Output Controller       (Base Addr = 0x12000000)
*************************************************************************/

typedef	struct
{
	unsigned		LEN		: 1;
	unsigned		SWAP	: 3;
	unsigned		Y2R		: 1;
	unsigned		RESERVE0	: 1;
	unsigned		SRST	: 1;
	unsigned		TV		: 1;
	unsigned		NI		: 1;
	unsigned		DP		: 1;
	unsigned		R2Y		: 1;
	unsigned		CLEN	: 1;
	unsigned		IP		: 1;
	unsigned		IH		: 1;
	unsigned		IV		: 1;
	unsigned		ID		: 1;
	unsigned		PXDW	: 4;
	unsigned		Y2RMD	: 2;
	unsigned		SREQ	: 1;
	unsigned		CKG		: 1;
	unsigned		C656	: 1;
	unsigned		RESERVE2	: 1;
	unsigned		ADVI	: 1;
	unsigned		RESERVE3	: 1;
	unsigned		R2YMD	: 2;
	unsigned		EVS		: 1;
	unsigned		EVP		: 1;
}	VIOC_DISP_LCTRL;

typedef	union
{
	unsigned long			nREG;
	VIOC_DISP_LCTRL		bREG;
}	VIOC_DISP_LCTRL_u;

typedef	struct
{
	unsigned 		BG0		: 8;
	unsigned 		BG1		: 8;
	unsigned 		BG2		: 8;
	unsigned 		BG3		: 8;
}	VIOC_DISP_BG;

typedef	union
{
	unsigned long			nREG;
	VIOC_DISP_BG		bREG;
}	VIOC_DISP_BG_u;

typedef	struct
{
	unsigned 		PXCLKDIV	: 8; /* PXCLK = LCLK/(2*PXCLKDIV) LCLK : Display Device Pixel clock form CKC Block, PXCLK : Pixel Clock to the External Display Device */
	unsigned 		RESERVE0	: 8;
	unsigned 		ACDIV   		: 8;
	unsigned 		RESERVE1	: 8;
}	VIOC_DISP_LCLKDIV;

typedef	union
{
	unsigned long			nREG;
	VIOC_DISP_LCLKDIV	bREG;
}	VIOC_DISP_LCLKDIV_u;

typedef	struct
{
	unsigned 		LPC		:14;
	unsigned 		RESERVE0	: 2;
	unsigned 		LPW		: 9;
	unsigned 		RESERVE1	: 7;
}	VIOC_DISP_LHTIME1;

typedef	union
{
	unsigned long			nREG;
	VIOC_DISP_LHTIME1	bREG;
}	VIOC_DISP_LHTIME1_u;

typedef	struct
{
	unsigned		LEWC	: 9;
	unsigned		RESERVE0	: 7;
	unsigned		LSWC	: 9;
	unsigned		RESERVE1	: 7;
}	VIOC_DISP_LHTIME2;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_LHTIME2	bREG;
}	VIOC_DISP_LHTIME2_u;

typedef	struct
{
	unsigned		FLC		:14;
	unsigned		RESERVE0	: 2;
	unsigned		FPW	: 6;
	unsigned		VDF		: 4;
	unsigned		RESERVE1	: 1;
	unsigned		VDB		: 5;
}	VIOC_DISP_LVTIME1;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_LVTIME1	bREG;
}	VIOC_DISP_LVTIME1_u;

typedef	struct
{
	unsigned		FEWC	: 9;
	unsigned		RESERVE0		: 7;
	unsigned		FSWC	: 9;
	unsigned		RESERVE1		: 7;
}	VIOC_DISP_LVTIME2;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_LVTIME2	bREG;
}	VIOC_DISP_LVTIME2_u;

typedef	struct
{
	unsigned 		CTH		: 4;
	unsigned		STH		: 4;
	unsigned 		RESERVED0	: 8;
	unsigned		HDMIVS	: 2;
	unsigned 		HDMIFLD	: 2;
	unsigned		EOFM	: 2;
	unsigned		BM		: 1;
	unsigned		RESERVED1	: 7;
	unsigned		MEN		: 1;
	unsigned		EN		: 1;
}	VIOC_DISP_DEFR;


typedef	union
{
	unsigned long		nREG;
	VIOC_DISP_DEFR	bREG;
}	VIOC_DISP_DEFR_u;

typedef struct
{
	unsigned		FLC		: 14;
	unsigned		RESERVED0	: 2;
	unsigned		FPW		: 6;
	unsigned		RESERVED1	: 9;
	unsigned		MD		: 1;
} 	VIOC_DISP_DVTIME1;

typedef union
{
	unsigned long		nREG;
	VIOC_DISP_DVTIME1	bREG;
}	VIOC_DISP_DVTIME1_u;


typedef	struct
{
	unsigned		XPOS	:14;
	unsigned		RESERVE0   	: 2;
	unsigned		YPOS	:14;
	unsigned		RESERVE1   	: 2;
}	VIOC_DISP_LPOS;

typedef union
{
	unsigned	long		nREG;
	VIOC_DISP_LPOS		bREG;
}	VIOC_DISP_LPOS_u;

typedef struct
{
	unsigned		VALUE	: 24;
	unsigned		RESERVED0	: 7;
	unsigned		BM		: 1;
}	VIOC_DISP_DBLK_VAL;

typedef union
{
	unsigned	long		nREG;
	VIOC_DISP_DBLK_VAL bREG;
}	VIOC_DISP_DBLK_VAL_u;


typedef	struct
{
	unsigned		RESERVE0		:30;
	unsigned		DSEL	: 1;
	unsigned		DEN 	: 1;
}	VIOC_DISP_DITHCTRL;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_DITHCTRL	bREG;
}	VIOC_DISP_DITHCTRL_u;

typedef	struct
{
	unsigned		CLPH			: 8;
	unsigned		RESERVE0		: 8;
	unsigned		CLPL			: 8;
	unsigned		RESERVE1		: 8;
}	VIOC_DISP_CLIP;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_CLIP		bREG;
}	VIOC_DISP_CLIP_u;

typedef	struct
{
	unsigned		HSIZE	: 13;
	unsigned		RESERVE0    	:  3;
	unsigned		VSIZE	: 13;
	unsigned		RESERVE1     	:  3;
}	VIOC_DISP_SIZE;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_SIZE		bREG;
}	VIOC_DISP_SIZE_u;

typedef	struct
{
	unsigned		CONRAST		: 8;
	unsigned		BRIGHTNESS	: 8;
	unsigned		HUE			: 8;
	unsigned		RESERVE0    	: 8;
}	VIOC_DISP_ENH;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_ENH		bREG;
}	VIOC_DISP_ENH_u;

typedef	struct
{
	unsigned		C0	:  4;
	unsigned		C1	:  4;
	unsigned		RESERVE0  	: 24;
}	VIOC_DISP_ADVI;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_ADVI		bREG;
}	VIOC_DISP_ADVI_u;

typedef	struct
{
	#if 0
	unsigned		MAT0	: 16;
	unsigned		MAT1	: 16;
	unsigned		MAT2	: 16;
	unsigned		MAT3	: 16;
	#else
	unsigned	DITH00	: 3;
	unsigned	RESERVE0	: 1;
	unsigned	DITH01	: 3;
	unsigned	RESERVE1	: 1;
	unsigned	DITH02	: 3;
	unsigned	RESERVE2	: 1;
	unsigned	DITH03	: 3;
	unsigned	RESERVE3	: 1;
	unsigned	DITH10	: 3;
	unsigned	RESERVE4	: 1;
	unsigned	DITH11	: 3;
	unsigned	RESERVE5	: 1;
	unsigned	DITH12	: 3;
	unsigned	RESERVE6	: 1;
	unsigned	DITH13	: 3;
	unsigned	RESERVE7	: 1;

	unsigned	DITH20	: 3;
	unsigned	RESERVE8	: 1;
	unsigned	DITH21	: 3;
	unsigned	RESERVE9	: 1;
	unsigned	DITH22	: 3;
	unsigned	RESERVE10	: 1;
	unsigned	DITH23	: 3;
	unsigned	RESERVE11	: 1;
	unsigned	DITH30	: 3;
	unsigned	RESERVE12	: 1;
	unsigned	DITH31	: 3;
	unsigned	RESERVE13	: 1;
	unsigned	DITH32	: 3;
	unsigned	RESERVE14	: 1;
	unsigned	DITH33	: 3;
	unsigned	RESERVE15	: 1;
	#endif
}	VIOC_DISP_DMAT;

typedef	union
{
	unsigned	long		nREG[2];
	VIOC_DISP_DMAT		bREG;
}	VIOC_DISP_DMAT_u;

typedef	struct
{
	unsigned		FU		:  1;	// fifo underrun
	unsigned		VSR		:  1;	// VSYNC rising
	unsigned		VSF		:  1;	// VSYNC falling
	unsigned		RU		:  1;	// Register Update
	unsigned		DD		:  1;	// Disable Done
	unsigned		SREQ	:  1;	// Stop Request
	unsigned		RESERVE0	: 10;
	unsigned		RESERVE1	: 12;
	unsigned		DEOF	:  1;	// Device EOF status
	unsigned		TFIELD	:  1;	// Top field indicator
	unsigned		BUSY_	:  1;	// busy status
	unsigned		VS		:  1;	// VSYNC status
}	VIOC_DISP_STATUS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_STATUS	bREG;
}	VIOC_DISP_STATUS_u;

typedef	struct
{
	unsigned		FU		:  1;	// fifo underrun
	unsigned		VSR		:  1;	// VSYNC rising
	unsigned		VSF		:  1;	// VSYNC falling
	unsigned		RU		:  1;	// Register Update
	unsigned		DD		:  1;	// Disable Done
	unsigned		SREQ	:  1;	// Stop Request
	unsigned		RESERVE0	: 10;
	unsigned		RESERVE1	: 16;
}	VIOC_DISP_IRQMASK;

typedef	union
{
	unsigned	long		nREG;
	VIOC_DISP_IRQMASK	bREG;
}	VIOC_DISP_IRQMASK_u;

#define VIOC_DISP_IREQ_FU_MASK		0x00000001UL /*  fifo underrun */
#define VIOC_DISP_IREQ_VSR_MASK		0x00000002UL /*  VSYNC rising */
#define VIOC_DISP_IREQ_VSF_MASK 		0x00000004UL /* VSYNC falling */
#define VIOC_DISP_IREQ_RU_MASK 		0x00000008UL /* Register Update */
#define VIOC_DISP_IREQ_DD_MASK 		0x00000010UL /* Disable Done */
#define VIOC_DISP_IREQ_SREQ_MASK 		0x00000020UL /* Stop Request */

#define VIOC_DISP_IREQ_DEOF_MASK 		0x10000000UL
#define VIOC_DISP_IREQ_TFIELD_MASK 	0x20000000UL
#define VIOC_DISP_IREQ_BUSY_MASK 		0x40000000UL
#define VIOC_DISP_IREQ_VS_MASK 		0x80000000UL

typedef	struct _VIOC_DISP
{
	volatile VIOC_DISP_LCTRL_u		uCTRL;			// 0x000  R/W  0x30400000  Display Device Register
	volatile VIOC_DISP_BG_u		uBG;				// 0x004  R/W  0x00000000  Display Device Background Register
	volatile VIOC_DISP_LCLKDIV_u	uCLKDIV;			// 0x008  R/W  0x00000000  Display Device Clock Control Register
	volatile VIOC_DISP_LHTIME1_u	uLHTIME1;		// 0x00c  R/W  0x00000000  Display Device Horizontal Timing Register 1
	volatile VIOC_DISP_LHTIME2_u	uLHTIME2;		// 0x010  R/W  0x00000000  Display Device Horizontal Timing Register 2
	volatile VIOC_DISP_LVTIME1_u	uLVTIME1;		// 0x014	 R/W  0x00000000  Display Device Vertical Timing Register 1
	volatile VIOC_DISP_LVTIME2_u	uLVTIME2;		// 0x018	 R/W  0x00000000  Display Device Vertical Timing Register 2
	volatile VIOC_DISP_LVTIME1_u	uLVTIME3;		// 0x01C	 R/W  0x00000000  Display Device Vertical Timing Register 3
	volatile VIOC_DISP_LVTIME2_u	uLVTIME4;		// 0x020	 R/W  0x00000000  Display Device Vertical Timing Register 4
	volatile VIOC_DISP_DEFR_u		uDEFR;			// 0x024	 R/W  0xC00000F1	 Display Emerfency Flag Register
	volatile VIOC_DISP_DVTIME1_u	uDVTIME1_3D;	// 0x028	 R/W  0x00000000	 Display Device Vertical Timing Register 1 in 3D
	volatile VIOC_DISP_LVTIME2_u	uDVTIME2_3D;	// 0x02C  R/W  0x00000000  Display Device Vertical Timing Register 2 in 3D
	volatile VIOC_DISP_LPOS_u		uLPOS;			// 0x030  R/W  0x00000000  Display Device Position Register
	volatile VIOC_DISP_DVTIME1_u	uDVTIME3_3D;	// 0x034	 R/W  0x00000000	 Display Device Vertical Timing Register 3 in 3D
	volatile VIOC_DISP_LVTIME2_u	uDVTIME4_3D;	// 0x038  R/W  0x00000000  Display Device Vertical Timing Register 4 in 3D
	volatile VIOC_DISP_DBLK_VAL_u	uDBLK_VAL;		// 0x03C  R/W  0x00000000  Display Device Blank Value Register
	volatile VIOC_DISP_DITHCTRL_u	uDITHCTRL;			// 0x040  R/W  0x00000000  Display Device Dithering Register
	volatile VIOC_DISP_CLIP_u		uCLIPY;			// 0x044  R/W  0x000000FF  Display Device Luma clipping Register
	volatile VIOC_DISP_CLIP_u		uCLIPC;			// 0x048  R/W  0x000000FF  Display Device chroma Clipping Register
	volatile VIOC_DISP_SIZE_u		uLSIZE;			// 0x04c  R/W  0x00000000  Display Device Size Register
	volatile VIOC_DISP_STATUS_u	uLSTATUS;		// 0x050  R/W  0x00000000  Display Device Status Register
	volatile VIOC_DISP_IRQMASK_u	uLIRQMASK;				// 0x054  R/W  0x00000000  Display Device Interrupt Masking Register
	volatile unsigned			reserved2[6];		// reserved
	volatile VIOC_DISP_ENH_u		uLENH;			// 0x070  R/W  0x00000020  Display Device Color Enhancement Register
	volatile VIOC_DISP_ADVI_u		uADVI;			// 0x074  R/W  0x00000000  Display Device Interlacer Coefficient Register
	volatile VIOC_DISP_DMAT_u		uDMAT;			// 0x078  R/W  0x00000000  Display Device Dithering Matrix Register 0, 1
} VIOC_DISP,*PVIOC_DISP;


typedef struct LCDCDEFAULT
{
	unsigned   evp;    // External VSYNC Polarity,		   [0:Direct Input 1:Inverted Input]
	unsigned   evs;    // External VSYNC Enable,		   [0:Disabled	   1:Enabled]
	unsigned   r2ymd;  // RGB to YCbCr Conversion Option,  [0: 1: 2: 3:]
	unsigned   advi;   // Advanced Interlaced Mode
	unsigned   dtype;  // LCD DMA Type					   [0: AHB DDIC on, 1:AXI DDIC Off]
	unsigned   gen;    // Gamma Correction Enable Bit,	   [0:Disabled 1:Enabled]
	unsigned   ccir656;// CCIR 656 Mode,				   [0:Disable	1:Enable]
	unsigned   ckg;    // Clock Gating Enable for Timing   [0:cannot  1:can]
	unsigned   bpp;    // Bit Per Pixel for STN-LCD 	   [0:1bpp 1:2bpp 2:4bpp 3:RGB332 4:RGB444 5~7:reserved]
	unsigned   pxdw;   // Pixel Data Width, Refer to Datasheet
	unsigned   id;	   // Inverted Data Enable, 		   [0:Active High	1:Active Low]
	unsigned   iv;	   // Inverted Vertical Sync,		   [0:Active High	1:Active Low]
	unsigned   ih;	   // Inverted Horizontal Sync		   [0:Active High	1:Active Low]
	unsigned   ip;	   // Inverted pixel Clock, 		   [0:Rising Edge	1:Falling Edge]
	unsigned   clen;   // clipping Enable				   [0:Disable	1:Enable]
	unsigned   r2y;    // RGB to YCbCr Converter Enable    [0:Disable 1:Converted]
	unsigned   dp;	   // Double Pixel Data,			   [0: 1:]
	unsigned   ni;	   // Non-Interlaced.				   [0:Interlaced	1:non-interlaced]
	unsigned   tv;	   // TV Mode,						   [0: Normal mode	1:TV Mode]
	unsigned   opt;    // LCD DMA Operation mode		   [0:8 burst	1:16 burst]
	unsigned   stn;    // STN LCD mode, 				   [0:Normal	1:STN LCD]
	unsigned   evsel;  // External VSync Select,		   [0: External VSync	1:Internal Generated VSync] Removed
	unsigned   ovp;    // Overlay 0~5
} stLCDCTR;



typedef struct LDCTIMING
{
	// LHTIME1
	unsigned	lpw;	// Line Pulse Width, HSync width
	unsigned	lpc;	// Line Pulse Count, HActive width
	// LHTIME2
	unsigned	lswc;	// Line Start Wait Clock, HFront porch
	unsigned	lewc;	// Line End wait clock, HBack porch
	// LVTIME1
	unsigned	vdb;	// Back Porch Delay
	unsigned	vdf;	// Front Porch Delay
	unsigned	fpw;	// Frame Pulse Width, VSync Width
	unsigned	flc;	// Frame Line Count, VActive width
	// LVTIME2
	unsigned	fswc;	// Frame Start Wait Cycle
	unsigned	fewc;	// Frame End Wait Cycle
	// LVTIME3 [in Interlaced, even field timing, otherwise should be same with LVTIME1]
	unsigned	fpw2;	// Frame Pulse Width,
	unsigned	flc2;	// Frame Line count,
	// LVTIME4 [in Interlaced, even field timing, otherwise should be same with LVTIME2]
	unsigned	fswc2;	 // Frame Start Wait Cycle
	unsigned	fewc2;	 // Frame End Wait Cycle
} stLTIMING;

typedef struct LCDC_PARAM
{
	stLCDCTR	 LCDCTRL;
	stLTIMING	 LCDCTIMING;
} stLCDCPARAM;





/************************************************************************
*   3.6.3   VIDEO IN                         (Base Addr = 0x12004000)
*************************************************************************/

typedef struct
{
	unsigned		enable				:	 1;
	unsigned		conv_en				:	 1;
	unsigned		intl_en				:	 1;
	unsigned		intpl_en				: 	 1;
	unsigned		hsde_connect_en		:	 1;
	unsigned		flush					:	 1;
	unsigned		vs_mask				:	 1;
	unsigned		RESERVE0		:	 1;
	unsigned		pxclk_pol				:	 1;
	unsigned		hs_active_low			:	 1;
	unsigned		vs_active_low			:	 1;
	unsigned		field_bfield_low		:	 1;
	unsigned		de_active_low			:	 1;
	unsigned		gen_field_en			:	 1;
	unsigned		stream_en			:	 1;
	unsigned		RESERVE1		:	 1;
	unsigned		fmt					:	 4;
	unsigned		data_order			:	 3;
	unsigned		RESERVE2		:	 1;
	unsigned		skip					:	 4;
	unsigned		RESERVE3		:	 3;
	unsigned		capture				:	 1;
}	VIN_CTRL;

typedef union
{
	unsigned	long		nREG;
	VIN_CTRL				bVIN_CTRL;
}	VIN_CTRL_u;

typedef struct
{
	unsigned	lut0_en				:	 1;
	unsigned	lut1_en				:	 1;
	unsigned	lut2_en				:	 1;
	unsigned	lut_apbif_en		:	 1;
	unsigned	y2r_en				:	 1;
	unsigned	y2r_mode			:	 2;
	unsigned	RESERVE0			:   25;
}	VIN_MISC;

typedef union
{
	unsigned	long		nREG;
	VIN_MISC				bVIN_MISC;
}	VIN_MISC_u;

typedef struct
{
	unsigned	hblank				:	 4;
	unsigned	vblank				:	 4;
	unsigned	field_pos				:	 4;
	unsigned	RESERVE0			:	 4;
	unsigned	pre_sta_loc			:	 2;
	unsigned	sync_byte			:	 2;
	unsigned	RESERVE1			:	12;
}	VIN_SYNC_M0;

typedef union
{
	unsigned	long		nREG;
	VIN_SYNC_M0		bREG;
}	VIN_SYNC_M0_u;

typedef struct
{
	unsigned	pre_first			:	 8;
	unsigned	pre_second			:	 8;
	unsigned	pre_third			:	 8;
	unsigned	RESERVE0			:	 8;
}	VIN_SYNC_M1;

typedef union
{
	unsigned	long		nREG;
	VIN_SYNC_M1		bREG;
}	VIN_SYNC_M1_u;

typedef	struct
{
	unsigned	width				:	16;
	unsigned	height				:	16;
}	VIN_SIZE;

typedef union
{
	unsigned	long			nREG;
	VIN_SIZE				bVIN_SIZE;
}	VIN_SIZE_u;

typedef	struct
{
	unsigned	offs_width			:	16;
	unsigned	offs_height			:	16;
}	VIN_OFFS;

typedef union
{
	unsigned	long		nREG;
	VIN_OFFS				bVIN_OFFS;
}	VIN_OFFS_u;

typedef	struct
{
	unsigned	update_int				:	 1;
	unsigned	eof_int					:	 1;
	unsigned	vs_int					:	 1;
	unsigned	invs_int					:	 1;
	unsigned	RESERVE0			:	 7;
	unsigned	frm_sq					:	 1;
	unsigned RESERVE1			:	4;
	unsigned	update_int_mask			:	 1;
	unsigned	eof_int_mask				:	 1;
	unsigned	vs_int_mask				: 	 1;
	unsigned	invs_int_mask				: 	 1;
	unsigned	RESERVE2			: 	 11;
	unsigned	int_en					:	 1;
}	VIN_INT;

typedef	union
{
	unsigned	long		nREG;
	VIN_INT				bVIN_INT;
}	VIN_INT_u;

typedef	struct
{
	unsigned	width_cnt			:	16;
	unsigned	height_cnt			:	16;
}	VIN_STAT_CNT;

typedef union
{
	unsigned	long		nREG;
	VIN_STAT_CNT			bREG;
}	VIN_STAT_CNT_u;

typedef	struct
{
	unsigned	value0			:	8;
	unsigned	value1			:	8;
	unsigned	value2			:	8;
	unsigned	value3			:	8;
}	VIN_LUT;

typedef union
{
	unsigned	long 		nREG[54];
	VIN_LUT 				bREG;
}	VIN_LUT_C0_u;

typedef union
{
	unsigned	long 		nREG[54];
	VIN_LUT 				bREG;
}	VIN_LUT_C1_u;

typedef union
{
	unsigned	long 		nREG[54];
	VIN_LUT 				bREG;
}	VIN_LUT_C2_u;

typedef	struct
{
	unsigned	width			:	16;
	unsigned	height			:	16;
}	VIN_CROP_SIZE;

typedef union
{
	unsigned	long				nREG;
	VIN_CROP_SIZE			bREG;
}	VIN_CROP_SIZE_u;

typedef	struct
{
	unsigned	offs_width		:	16;
	unsigned	offs_height		:	16;
}	VIN_CROP_OFFS;

typedef union
{
	unsigned	long				nREG;
	VIN_CROP_OFFS			bREG;
}	VIN_CROP_OFFS_u;


typedef struct _VIOC_VIN
{
	volatile VIN_CTRL_u 			uVIN_CTRL; 			// 	0x000  R/W  0x00000000 VIN Control Register
	volatile VIN_MISC_u 			uVIN_MISC; 			// 	0x004  R/W  0x00000000 VIN Misc Register
	volatile VIN_SYNC_M0_u 		uVIN_SYNC_M0; 		// 	0x008  R/W  0x00030CB9 VIN Sync Misc. 0 Register
	volatile VIN_SYNC_M1_u 		uVIN_SYNC_M1; 		// 	0x00c  R/W  0x000000FF VIN Sync misc. 1 Register
	volatile VIN_SIZE_u 			uVIN_SIZE; 			// 	0x010  R/W  0x00000000 VIN Size Register
	volatile VIN_OFFS_u 			uVIN_OFFS; 			// 	0x014  R/W  0x00000000 VIN Offset Register
	volatile VIN_OFFS_u 			uVIN_OFFS_INTL; 	// 	0x018  R/W  0x00000000 VIN Offset Interlaced Register
	volatile VIN_CROP_SIZE_u 		uVIN_CROP_SIZE; 	// 	0x01c  R/W  0x00000000 VIN Crop Size Register
	volatile VIN_CROP_OFFS_u 		uVIN_CROP_OFFS; 	// 	0x01c  R/W  0x00000000 VIN Crop Offset Register
	unsigned	int 					undef0[15]; 			// 	0x014 ~ 0x5c
	volatile VIN_INT_u 				uVIN_INT; 			// 	0x060  R/W  0x00000000 VIN Iterrupt Register
	unsigned	int 					undef1[534]; 		// 	0x064 ~ 0xFFC
	volatile VIN_LUT_C0_u 			uVIN_LUT_C0;		// 	0x1000 ~ 0x10FC R/W VIN Look-up Table set in Channel 0
	volatile VIN_LUT_C1_u 			uVIN_LUT_C1;		// 	0x1100 ~ 0x11FC R/W VIN Look-up Table set in Channel 1
	volatile VIN_LUT_C2_u 			uVIN_LUT_C2;		// 	0x1200 ~ 0x12FC R/W VIN Look-up Table set in Channel 2
} VIOC_VIN, *PVIOC_VIN;


/************************************************************************
*   3.6.6   GRDMA  and VRDMA				(Base Addr = 0x12000400)
*************************************************************************/
typedef	struct
{
	unsigned		FMT		:  5;
	unsigned		RESERVE0   		:  2;
	unsigned		BR  	:  1;
	unsigned		Y2R 	:  1;
	unsigned		Y2RMD	:  2;
	unsigned		AEN  	:  1;
	unsigned		SWAP 	:  3;
	unsigned		PD   	:  1;
	unsigned		UPD  	:  1;
	unsigned		R2Y		:  1;
	unsigned		R2YMD	:  2;
	unsigned		DITS	:  1;
	unsigned		DIT		:  1;
	unsigned		NUVIH	:  1;
	unsigned		UVI  	:  1;
	unsigned		ASEL 	:  1;
	unsigned		DM_3D		:  2;
	unsigned		STRM 	:  1;
	unsigned		IEN  	:  1;
	unsigned		BFIELD	:  1;
	unsigned		BFMD	:  1;
	unsigned		INTL	:  1;
}	VIOC_RDMA_CTRL;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_CTRL		bREG;
} VIOC_RDMA_CTRL_u;

typedef	struct
{
	unsigned	TOP		: 16;
	unsigned	BOT		: 16;
} VIOC_RDMA_PTS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_PTS		bREG;
} VIOC_RDMA_PTS_u;

typedef	struct
{
	unsigned	WIDTH   	: 13;
	unsigned	RESERVE0	:  3;
	unsigned	HEIGHT   	: 13;
	unsigned	RESERVE1	:  3;
} VIOC_RDMA_SIZE;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_SIZE		bREG;
} VIOC_RDMA_SIZE_u;

typedef	struct
{
	unsigned	OFFSET0   	: 16;
	unsigned	OFFSET1   	: 16;
} VIOC_RDMA_OFFS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_OFFS		bREG;
} VIOC_RDMA_OFFS_u;

typedef	struct
{
	unsigned	ALPHA0   	:  8;
	unsigned	RESERVE0     :  8;
	unsigned	ALPHA1   	:  8;
	unsigned	RESERVE1     :  8;
} VIOC_RDMA_ALPHA;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_ALPHA		bREG;
} VIOC_RDMA_ALPHA_u;

typedef	struct
{
	unsigned	CFGDONE	:  1;	// update
	unsigned	EOFRISE		:  1;	// Device EOF Rising
	unsigned	EOFFALL	:  1;	// Device EOF Falling
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	EOFWAITR	:  1;	// EOF Wait Rising - Frame End
	unsigned	TOPRDY		:  1;	// Top Ready
	unsigned	BOTRDY		:  1;	// Bottom Ready
	unsigned	RESERVE0	:  9;
	unsigned	STS_TOPRDY :  1;	// TOP field Ready
	unsigned	STS_BOTRDY :  1;	// BOTTOM field Ready
	unsigned	STS_EOFWAIT:  1;	// RDMA eof-wait status
	unsigned	STS_DEVEOF	:  1;	// Device EOF
	unsigned	STS_BFIELD	:  1;	// Bottom field indicator
	unsigned	RESERVE1	:  7;
	unsigned	STS_FDLY	:  4;	// frame delay
} VIOC_RDMA_STATUS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_STATUS	bREG;
} VIOC_RDMA_STATUS_u;

typedef	struct
{
	unsigned	CFGDONE	:  1;	// update
	unsigned	DEOFR	:  1;	// Device EOF Rising
	unsigned	DEOFF	:  1;	// Device EOF Falling
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	EOFWAITR:  1;	// EOF Wait Rising - Frame End
	unsigned	TOPRDY	:  1;	// Top Ready
	unsigned	BOTRDY	:  1;	// Bottom Ready
	unsigned	RESERVE0		:  9;
	unsigned	RESERVE1		: 16;
} VIOC_RDMA_IRQMSK;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_IRQMSK	bREG;
} VIOC_RDMA_IRQMSK_u;

typedef	struct
{
	unsigned	XSCALE	: 3;
	unsigned	RESERVE0		:13;
	unsigned	YSCALE	: 3;
	unsigned	RESERVE1		:13;
} VIOC_RDMA_SCALE;

typedef	union
{
	unsigned	long		nREG;
	VIOC_RDMA_SCALE		bREG;
} VIOC_RDMA_SCALE_u;

typedef	struct _VIOC_RDMA
{
	volatile VIOC_RDMA_CTRL_u	uCTRL;				// 0x00  R/W  0x00000000  RDMA Control Register
	volatile VIOC_RDMA_PTS_u	uPTS;				// 0x04  R/W  0x00000000  RDMA Image PTS Register
	volatile VIOC_RDMA_SIZE_u	uSIZE;				// 0x08  R/W  0x00000000  RDMA Image Size information Register
	volatile unsigned  int          	nBASE0;			// 0x0C  R/W  0x00000000  RDMA Base0 Address for Each Images Register
	volatile unsigned  int 		nCBASE;			// 0x10  R/W	 0x00000000  RDMA Current Address for Each Images Register
	volatile unsigned  int		nBASE1;			// 0x14  R/W  0x00000000  RDMA Base1 Address for Each Images Register
	volatile unsigned  int		nBASE2;			// 0x18  R/W  0x00000000  RDMA Base2 Address for Each Images Register
	volatile VIOC_RDMA_OFFS_u  	uOFFSET;		// 0x1C  R/W  0x00000000  RDMA Offset Information for Each Images Register
	volatile VIOC_RDMA_SCALE_u	uSCALE;			// 0x20  R/W  0x00000000  RDMA Scale for Each images Register
	volatile VIOC_RDMA_ALPHA_u 	uALPHA;			// 0x24  R/W  0x00000000  RDMA Alpha Information for Each Imgaes Register
	volatile VIOC_RDMA_STATUS_u	uSTATUS;		// 0x28  R/W	 0x00000000  RDMA Status Register
	volatile VIOC_RDMA_IRQMSK_u	uIRQMSK;		// 0x2C  R/W  0x0000007F  RDMA interrupt mask Register
	volatile unsigned  int			nSBASE0;		// 0x30  R/W  0x00000000  RDMA Sync base address
	volatile unsigned  int			nRBASE0;		// 0x34  R/W  0x00000000  RDMA 3D base0 address
	volatile unsigned  int			nRBASE1;		// 0x38  R/W  0x00000000  RDMA 3D base1 address
	volatile unsigned  int			nRBASE2;		// 0x3C  R/W  0x00000000  RDMA 3D base2 address
} VIOC_RDMA, *PVIOC_RDMA;

/************************************************************************
*   3.6.5   GWDMA  and VWDMA				(Base Addr = 0x12002800)
*************************************************************************/

typedef	struct
{
	unsigned	FMT		:  5;
	unsigned	RESERVE0  :  2;
	unsigned	BR  	:  1;
	unsigned	R2Y 	:  1;
	unsigned	R2YMD	:  2;
	unsigned	RESERVE1  :  1;
	unsigned	SWAP 	:  3;
	unsigned	RESERVE2     :  1;
	unsigned	UPD  	:  1;
	unsigned	Y2R 		:  1;
	unsigned	Y2RMD	:  2;	
	unsigned	RESERVE3     : 2;	
	unsigned	SREQ  	:  1;
	unsigned	CONT 	:  1;
	unsigned	DITHEN	:  1;
	unsigned	RESERVE4  :  2;
	unsigned	DITHSEL	:  1;
	unsigned	IEN  	:  1;
	unsigned	FU		:  1;
	unsigned	RESERVE5 :  1;
	unsigned	INTL	:  1;
}	VIOC_WDMA_CTRL;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_CTRL		bREG;
}	VIOC_WDMA_CTRL_u;

typedef	struct
{
	unsigned	SYNCSEL :  8;
	unsigned	SEN		:  1;
	unsigned	SYNCMD_ADDR	:  1;
	unsigned	RESERVE0		:  1;
	unsigned	SYNCMD_SENS	:  1;
	unsigned	RESERVE1		:  4;
	unsigned	MAXRATE		:  8;
	unsigned	RESERVE2		:  7;
	unsigned	REN			:  1;
}	VIOC_WDMA_RATE;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_RATE		bREG;
}	VIOC_WDMA_RATE_u;

typedef	struct
{
	unsigned	WIDTH   	: 13;
	unsigned	RESERVE0	:  3;
	unsigned	HEIGHT   	: 13;
	unsigned	RESERVE1	:  3;
}	VIOC_WDMA_SIZE;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_SIZE		bREG;
}	VIOC_WDMA_SIZE_u;

typedef	struct
{
	unsigned	OFFSET0   	: 16;
	unsigned	OFFSET1   	: 16;
}	VIOC_WDMA_OFFS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_OFFS		bREG;
}VIOC_WDMA_OFFS_u;

typedef	struct
{
	unsigned	BG0		: 8;
	unsigned	BG1		: 8;
	unsigned	BG2		: 8;
	unsigned	BG3		: 8;
}VIOC_WDMA_BG;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_BG		bREG;
}VIOC_WDMA_BG_u;

typedef	struct
{
	unsigned	PTS		: 16;
	unsigned	RESERVE0    	: 16;
}VIOC_WDMA_PTS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_PTS		bREG;
}VIOC_WDMA_PTS_u;

typedef	struct
{

	unsigned	DITH00	: 3;
	unsigned	RESERVE0	: 1;
	unsigned	DITH01	: 3;
	unsigned	RESERVE1	: 1;
	unsigned	DITH02	: 3;
	unsigned	RESERVE2	: 1;
	unsigned	DITH03	: 3;
	unsigned	RESERVE3	: 1;
	unsigned	DITH10	: 3;
	unsigned	RESERVE4	: 1;
	unsigned	DITH11	: 3;
	unsigned	RESERVE5	: 1;
	unsigned	DITH12	: 3;
	unsigned	RESERVE6	: 1;
	unsigned	DITH13	: 3;
	unsigned	RESERVE7	: 1;

	unsigned	DITH20	: 3;
	unsigned	RESERVE8	: 1;
	unsigned	DITH21	: 3;
	unsigned	RESERVE9	: 1;
	unsigned	DITH22	: 3;
	unsigned	RESERVE10	: 1;
	unsigned	DITH23	: 3;
	unsigned	RESERVE11	: 1;
	unsigned	DITH30	: 3;
	unsigned	RESERVE12	: 1;
	unsigned	DITH31	: 3;
	unsigned	RESERVE13	: 1;
	unsigned	DITH32	: 3;
	unsigned	RESERVE14	: 1;
	unsigned	DITH33	: 3;
	unsigned	RESERVE15	: 1;

}VIOC_WDMA_DMAT;

typedef	union
{
	unsigned	long		nREG[2];
	VIOC_WDMA_DMAT	bREG;
}VIOC_WDMA_DMAT_u;

typedef	struct
{
	unsigned	CONTRAST: 8;
	unsigned	BRIGHT	: 8;
	unsigned	HUE		: 8;
	unsigned	RESERVE0  : 8;
}VIOC_WDMA_ENH;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_ENH		bREG;
}VIOC_WDMA_ENH_u;

typedef	struct
{
	unsigned	ROLLCNT   	: 16;
	unsigned	RESERVE0     : 15;
	unsigned	ROLL     	:  1;
}VIOC_WDMA_ROLL;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_ROLL		bREG;
}VIOC_WDMA_ROLL_u;

typedef	struct
{
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	STOPREQ	:  1;	// Stop Request
	unsigned	ROLL 		:  1;	// Roll Interrupt
	unsigned	ENRISE		:  1;	// Frame Synchronized Enable Rising
	unsigned	ENFALL		:  1;	// Frame Synchronized Enable Falling (Disable-Done)
	unsigned	EOFRISE		:  1;	// EOF Rising
	unsigned	EOFFALL	:  1;	// EOF Falling
	unsigned	SEOFRISE	:  1;	// SYNC EOF Rising
	unsigned	SEOFFALL	:  1;	// SYNC EOF Falling
	unsigned	RESERVE0	:  7;
	unsigned	RESERVE1	: 13;
	unsigned	STS_SEN		:  1;	// Synchronized Enable
	unsigned	STS_BFIELD	:  1;	// Bottom field indicator
	unsigned	STS_EOF	:  1;	// EOF
}VIOC_WDMA_STATUS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_STATUS	bREG;
}VIOC_WDMA_STATUS_u;

typedef	struct
{
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	STOPREQ	:  1;	// Stop Request
	unsigned	ROLL 		:  1;	// Roll Interrupt
	unsigned	ENRISE		:  1;	// Synchronized Enable Rising
	unsigned	ENFALL		:  1;	// Synchronized Enable Falling (Disable-Done)
	unsigned	EOFRISE		:  1;	// EOF Rising
	unsigned	EOFFALL	:  1;	// EOF Falling
	unsigned	SEOFRISE	:  1;	// SYNC EOF Rising
	unsigned	SEOFFALL	:  1;	// SYNC EOF Falling
	unsigned	RESERVE0	:  7;
	unsigned	RESERVE1	: 16;
}VIOC_WDMA_IRQMSK;

typedef	union
{
	unsigned	long		nREG;
	VIOC_WDMA_IRQMSK	bREG;
}VIOC_WDMA_IRQMSK_u;

typedef	struct _VIOC_WDMA
{
	volatile VIOC_WDMA_CTRL_u 		uCTRL; 			// 0x00  R/W  0x00000400 WMDA Control Register
	volatile VIOC_WDMA_RATE_u 		uRATE; 			// 0x04  R/W  0x00000000 WDMA Rate Control Register
	volatile VIOC_WDMA_SIZE_u 		uSIZE; 			// 0x08  R/W  0x00000000 WDMA Size Register
	volatile unsigned  int 				uBASE0; 		// 0x0C  R/W  0x00000000 WDMA Base Address 0 Register
	volatile unsigned  int 				uCBASE; 		// 0x10  R/W  0x00000000 WDMA Current Address Register
	volatile unsigned  int 				uBASE1; 		// 0x14  R/W  0x00000000 WDMA Base Address 1 Register
	volatile unsigned  int 				uBASE2; 		// 0x18  R/W  0x00000000 WDMA Base Address 2 Register
	volatile VIOC_WDMA_OFFS_u 		uOFFS; 			// 0x1C  R/W  0x00000000 WDMA Offset Register
	volatile unsigned  int 				reserved0; 		// 0x20
	volatile VIOC_WDMA_BG_u 			uBG; 			// 0x24  R/W  0x00000000 WDMA Back Ground Color Register
	volatile VIOC_WDMA_PTS_u 			uPTS; 			// 0x28  R/W  0x00000000 WDMA PTS Register
	volatile VIOC_WDMA_DMAT_u 		uDMAT; 			// 0x2C  R/W  0x00000000 WDMA Dither Matrix 0,1 Register
	volatile VIOC_WDMA_ENH_u 		uENH; 			// 0x34  R/W  0x00000000 WDMA Color Enhancement Register
	volatile VIOC_WDMA_ROLL_u 		uROLL; 			// 0x38  R/W  0x00000000 WDMA Rolling Control Register
	volatile unsigned  int 				uSBASE; 		// 0x3C  R/W  0x00000000 WMA Synchronized Base Address Register
	volatile VIOC_WDMA_STATUS_u 		uIRQSTS; 		// 0x40  R/W  0x00000000 WDMA Interrupt Status Register
	volatile VIOC_WDMA_IRQMSK_u 		uIRQMSK; 		// 0x44  R/W  0x000001FF WDMA Interrupt Mask Register
}VIOC_WDMA, *PVIOC_WDMA;


/************************************************************************
*   3.6.6   Scaler				(Base Addr = 0x12002000)
*************************************************************************/

typedef	struct
{
	unsigned	BP		:  1;
	unsigned	RESERVE0: 15;
	unsigned	UPD 	:  1;
	unsigned	RESERVE1     : 15;
} VIOC_SC_CTRL;

typedef	union
{
	unsigned	long			nREG;
	VIOC_SC_CTRL		bREG;
} VIOC_SC_CTRL_u;

typedef	struct
{
	unsigned	WIDTH   	: 16;
	unsigned	HEIGHT   	: 16;
} VIOC_SC_SIZE;

typedef	union
{
	unsigned	long			nREG;
	VIOC_SC_SIZE		bREG;
} VIOC_SC_SIZE_u;

typedef	struct
{
	unsigned	XPOS    	: 16;
	unsigned	YPOS     	: 16;
} VIOC_SC_POS;

typedef	union
{
	unsigned	long			nREG;
	VIOC_SC_POS			bREG;
} VIOC_SC_POS_u;

typedef	struct
{
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	EOFRISE	:  1;	// EOF Rising
	unsigned	EOFFALL	:  1;	// EOF Falling
	unsigned	ERR		:  1;		// Error
	unsigned	RESERVE0		: 12;
	unsigned	RESERVE1		: 16;
} VIOC_SC_STATUS;

typedef	union
{
	unsigned	long		nREG;
	VIOC_SC_STATUS	bREG;
} VIOC_SC_STATUS_u;

typedef	struct
{
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	EOFRISE	:  1;	// EOF Rising
	unsigned	EOFFALL	:  1;	// EOF Falling
	unsigned	ERR	:  1;	// Error
	unsigned	RESERVE0		: 12;
	unsigned	RESERVE1		: 16;
} VIOC_SC_IRQMSK;

typedef	union
{
	unsigned	long			nREG;
	VIOC_SC_IRQMSK	bREG;
} VIOC_SC_IRQMSK_u;

#define VIOC_SC_INT_MASK_UPDDONE 		0x00000001UL /*Status of Register Updated*/
#define VIOC_SC_INT_MASK_EOFRISE 		0x00000002UL /*Rising EOF Interrupt Masked*/
#define VIOC_SC_INT_MASK_EOFFALL 		0x00000004UL /*Falling EOF Interrupt Masked*/
#define VIOC_SC_INT_MASK_ERROR  		0x00000008UL /*Scaler Error Interrupt Masked*/
#define VIOC_SC_INT_MASK_ALL 			0x0000000FUL /*ALL*/

#define VIOC_SC_IREQ_UPDDONE_MASK 		0x00000001UL
#define VIOC_SC_IREQ_EOFRISE_MASK 		0x00000002UL
#define VIOC_SC_IREQ_EOFFALL_MASK  		0x00000004UL
#define VIOC_SC_IREQ_ERROR_MASK  		0x00000008UL

typedef	struct _VIOC_SC
{
	volatile VIOC_SC_CTRL_u		uCTRL;				// 0x00  R/W  0x00000000 Scaler Control Register
	volatile unsigned int			reserved0;			// 0x04
	volatile VIOC_SC_SIZE_u		uSRCSIZE;			// 0x08  R       -              Scaler Source Size Register
	volatile VIOC_SC_SIZE_u		uDSTSIZE;			// 0x0C  R/W  0x00000000 Scaler Destination Size Register
	volatile VIOC_SC_POS_u 		uOUTPOS;			// 0x10  R/W  0x00000000 Scaler Output Position Register
	volatile VIOC_SC_SIZE_u		uOUTSIZE;			// 0x14  R/W  0x00000000 Scaler Output Size Register
	volatile VIOC_SC_STATUS_u		uSTATUS;			// 0x18  R/W  0x00000000 Scaler Interrupt Status Register
	volatile VIOC_SC_IRQMSK_u		uIRQMSK;			// 0x1C  R/W  0x0000000F Scaler Interrupt Mask Register
} VIOC_SC,*PVIOC_SC;



/************************************************************************
*   3.6.10   WMIX				(Base Addr = 0x12001800)
*************************************************************************/


typedef	struct
{
	unsigned		OVP		:  5;
	unsigned		RESERVED0 	:  1;
	unsigned		STR		:  2;
	unsigned		EN0_3D	:  1;
	unsigned		EN1_3D	:  1;
	unsigned		EN2_3D	:  1;
	unsigned		EN3_3D	:  1;
	unsigned		RESERVED	:  4;
	unsigned		UPD 	:  1;
	unsigned		MD0_3D	:  3;
	unsigned		MD1_3D	:  3;
	unsigned		MD2_3D	:  3;
	unsigned		MD3_3D	:  3;
	unsigned		RESERVED1	: 3;
} VIOC_WMIX_CTRL;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_CTRL		bREG;
} VIOC_WMIX_CTRL_u;

typedef	struct
{
	unsigned	BG0		: 8;
	unsigned	BG1		: 8;
	unsigned	BG2		: 8;
	unsigned	BG3		: 8;
} VIOC_WMIX_BG;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_BG		bREG;
} VIOC_WMIX_BG_u;

typedef	struct
{
	unsigned	WIDTH    	: 13;
	unsigned 	RESERVED0	:  3;
	unsigned	HEIGHT     	: 13;
	unsigned 	RESERVED1	:  3;
} VIOC_WMIX_SIZE;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_SIZE		bREG;
} VIOC_WMIX_SIZE_u;

typedef	struct
{
	unsigned	XPOS    	: 13;
	unsigned 	RESERVED0:  3;
	unsigned	YPOS     	: 13;
	unsigned 	RESERVED1:  3;
} VIOC_WMIX_POS;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_POS		bREG;
} VIOC_WMIX_POS_u;

typedef	struct
{
	unsigned	KEYB    	:  8;
	unsigned	KEYG		:  8;
	unsigned	KEYR     	:  8;
	unsigned RESERVED0	:  7;
	unsigned	KEYEN    	:  1;
	unsigned	KEYMB    	:  8;
	unsigned	KEYMG		:  8;
	unsigned	KEYMR     	:  8;
	unsigned 	RESERVED1	:  8;
} VIOC_WMIX_KEY;

typedef	union
{
	unsigned	long			nREG[2];
	VIOC_WMIX_KEY		bREG;
} VIOC_WMIX_KEY_u;

typedef	struct
{
	unsigned	UPDDONE		:  1;	// Register Update Done
	unsigned	EOFWAITFALL	:  1;	// EOF Rising
	unsigned	EOFWAITRISE	:  1;	// EOF Falling
	unsigned	EOFRISE		:  1;	// Error
	unsigned	EOFFALL		:  1;	// Error
	unsigned	RESERVE0			: 11;
	unsigned	RESERVE1			:  8;
	unsigned	UP_EOF    	:  1;	// Source
	unsigned	UP_ENABLE 	:  1;	// Source
	unsigned	UP_UPDATE 	:  1;	// Source
	unsigned	UP_INTL   	:  1;	// Source
	unsigned	DN_EOFWAIT	:  1;	// Source
	unsigned	DN_ENABLE	:  1;	// Source
	unsigned	DN_BFIELD	:  1;	// Source
	unsigned	DN_INTL		:  1;	// Source
} VIOC_WMIX_STATUS;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_STATUS	bREG;
} VIOC_WMIX_STATUS_u;

typedef	struct
{
	unsigned	UPDDONE		:  1;	// Register Update Done
	unsigned	EOFWAITFALL	:  1;	// EOF Rising
	unsigned	EOFWAITRISE	:  1;	// EOF Falling
	unsigned	EOFRISE		:  1;	// Error
	unsigned	EOFFALL		:  1;	// Error
	unsigned	RESERVE0		: 11;
	unsigned	RESERVE1		: 16;
} VIOC_WMIX_IRQMSK;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_IRQMSK	bREG;
} VIOC_WMIX_IRQMSK_u;

/*WMIX ALPHA CONTROL*/
typedef	struct
{
	unsigned	ACON0_00		:  3;
	unsigned	RESERVE0   		:  1;
	unsigned	ACON1_00		:  3;
	unsigned	RESERVE1   		:  1;
	unsigned	ACON0_01		:  3;
	unsigned	RESERVE2   		:  1;
	unsigned	ACON1_01		:  3;
	unsigned	RESERVE3   		:  1;
	unsigned	ACON0_10		:  3;
	unsigned	RESERVE4   		:  1;
	unsigned	ACON1_10		:  3;
	unsigned	RESERVE5   		:  1;
	unsigned	ACON0_11		:  3;
	unsigned	RESERVE6   		:  1;
	unsigned	ACON1_11		:  3;
	unsigned	RESERVE7   		:  1;
} VIOC_WMIX_MACON;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_MACON		bREG;
} VIOC_WMIX_MACON_u;

typedef	struct
{
	unsigned	CCON0_00		:  4;
	unsigned	CCON1_00		:  4;
	unsigned	CCON0_01		:  4;
	unsigned	CCON1_01		:  4;
	unsigned	CCON0_10		:  4;
	unsigned	CCON1_10		:  4;
	unsigned	CCON0_11		:  4;
	unsigned	CCON1_11		:  4;
} VIOC_WMIX_MCCON;

typedef	union
{
	unsigned	long				nREG;
	VIOC_WMIX_MCCON		bREG;
} VIOC_WMIX_MCCON_u;

typedef	struct
{
	unsigned	ROPMODE		:  5;
	unsigned	RESERVE1		:  9;
	unsigned	ASEL			:  2;
	unsigned	ALPHA0			:  8;
	unsigned	ALPHA1			:  8;
} VIOC_WMIX_MROPC;

typedef	union
{
	unsigned	long				nREG;
	VIOC_WMIX_MROPC		bREG;
} VIOC_WMIX_MROPC_u;

typedef	struct
{
	unsigned	BLUE		:  8;
	unsigned	GREEN		:  8;
	unsigned	RED			:  8;
	unsigned	RESERVE0	:  8;
} VIOC_WMIX_MPAT;

typedef	union
{
	unsigned	long			nREG;
	VIOC_WMIX_MPAT		bREG;
} VIOC_WMIX_MPAT_u;

typedef	struct _VIOC_WMIX
{
	volatile VIOC_WMIX_CTRL_u 	uCTRL; 			// 0x00  R/W  0x00000005 WMIX Control Register
	volatile VIOC_WMIX_BG_u 		uBG; 			// 0x04  R/W  0x00000000 WMIX Background Color Register
	volatile VIOC_WMIX_SIZE_u 	uSIZE; 				// 0x08  R/W  0x00000000 WMIx Size Register
	volatile unsigned  int 			reserved0; 		// 0x0C
	volatile VIOC_WMIX_POS_u 		uPOS0; 			// 0x10  R/W  0x00000000 WMIX Position 0 Register
	volatile VIOC_WMIX_POS_u 		uPOS1; 			// 0x14  R/W  0x00000000 WMIX Position 1 Register
	volatile VIOC_WMIX_POS_u 		uPOS2; 			// 0x18  R/W  0x00000000 WMIX Position 2 Register
	volatile VIOC_WMIX_POS_u 		uPOS3; 			// 0x1C  R/W  0x00000000 WMIX Position 3 Register
	volatile VIOC_WMIX_KEY_u 		uKEY0; 			// 0x20  R/W  0x00000000 WMIX Chroma-Key 0,1 Image 0 Register
	volatile VIOC_WMIX_KEY_u 		uKEY1; 			// 0x28  R/W  0x00000000 WMIX Chroma-key 0,1 Image 1 Register
	volatile VIOC_WMIX_KEY_u 		uKEY2; 			// 0x30  R/W  0x00000000 WMIX Chroma-key 0,1 Image 2 Register
	volatile VIOC_WMIX_STATUS_u 	uSTATUS; 		// 0x38  R/W  0x00000000 WMIX Status Register
	volatile VIOC_WMIX_IRQMSK_u 	uIRQMSK; 		// 0x3C  R/W  0x0000001F WMIX Interrupt Masked Register

	volatile VIOC_WMIX_MACON_u 	uACON0; 		// 0x40  R/W  0x10622662 WMIX Alpha Control Register for 0'th Layer
	volatile VIOC_WMIX_MCCON_u 	uCCON0; 		// 0x44  R/W  0x20C44CC4 WMIX Color Control Register for 0'th Layer
	volatile VIOC_WMIX_MROPC_u 	uROPC0; 		// 0x48  R/W  0x0000C018 WMIX ROP Control Register for 0'th Layer
	volatile VIOC_WMIX_MPAT_u 	uPAT0; 			// 0x4C  R/W  0x00000000 WMIX Patterm Register for 0'th Layer

	volatile VIOC_WMIX_MACON_u 	uACON1; 		// 0x50  R/W  0x10622662 WMIX Alpha Control Register for 1'th Layer
	volatile VIOC_WMIX_MCCON_u 	uCCON1; 		// 0x54  R/W  0x20C44CC4 WMIX Color Control Register for 1'th Layer
	volatile VIOC_WMIX_MROPC_u 	uROPC1; 		// 0x58  R/W  0x0000C018 WMIX ROP Control Register for 1'th Layer
	volatile VIOC_WMIX_MPAT_u 	uPAT1; 			// 0x5C  R/W  0x00000000 WMIX Patterm Register for 1'th Layer

	volatile VIOC_WMIX_MACON_u 	uACON2; 		// 0x60  R/W  0x10622662 WMIX Alpha Control Register for 2'th Layer
	volatile VIOC_WMIX_MCCON_u 	uCCON2; 		// 0x64  R/W  0x20C44CC4 WMIX Color Control Register for 2'th Layer
	volatile VIOC_WMIX_MROPC_u 	uROPC2; 		// 0x68  R/W  0x0000C018 WMIX ROP Control Register for 2'th Layer
	volatile VIOC_WMIX_MPAT_u 	uPAT2; 			// 0x6C  R/W  0x00000000 WMIX Patterm Register for 2'th Layer
} VIOC_WMIX,*PVIOC_WMIX;


/************************************************************************
*   3.6.10   VIQE (Video Quality Enhancemnet)			(Base Addr = 0x1200D000)
*************************************************************************/

typedef	struct
{
	unsigned	his_cdf_or_lut_en			:	 1;
	unsigned	his_en						:	 1;
	unsigned	gamut_en					:	 1;
	unsigned	denoise3d_en				:	 1;
	unsigned	deintl_en					: 	 1;
	unsigned	RESERVE0					:	 3; // bit  7
	unsigned	no_hor_intpl				:	 1;
	unsigned	RESERVE1					:	 7; // bit 15
	unsigned	fmt_conv_disable			:	 1;
	unsigned	fmt_conv_disable_using_fmt	:	 1;
	unsigned	fmt_conv_flush				:	 1;
	unsigned	RESERVE2					:	 1;
	unsigned	update_disable				:	 1;
	unsigned	cfgupd						:	 1;
	unsigned	RESERVE3					:	 2;	// bit 23
	unsigned	clkgate_deintl_disable		:	 1;
	unsigned	clkgate_d3d_disable			:	 1;
	unsigned	clkgate_pm_disable			:	 1;
	unsigned	RESERVE4					:	 5; // bit 31
}	VIQE_CTRL;

typedef	union
{
	unsigned	long			nREG;
	VIQE_CTRL			bREG;
}	VIQE_CTRL_u;

typedef	struct
{
	unsigned	width					:	16;
	unsigned	height					:	16;
}	VIQE_SIZE;

typedef	union
{
	unsigned	long			nREG;
	VIQE_SIZE			bREG;
}	VIQE_SIZE_u;

typedef	struct
{
	unsigned	h2h						:	 8;
	unsigned	y2r_en					:	 1;
	unsigned	y2r_mode				:	 2;
	unsigned	RESERVE0				:	21;
}	VIQE_CTRL_Y2R;

typedef	union
{
	unsigned	long			nREG;
	VIQE_CTRL_Y2R		bREG;
}	VIQE_CTRL_Y2R_u;

typedef	struct
{
	unsigned	deintl_IRQ				:	1;
	unsigned	denoise_IRQ				:	1;
	unsigned	pm_IRQ					:	1;
	unsigned	RESERVE0				:	5;
	unsigned	deintl_IRQ_mask			:	1;
	unsigned	denoise_IRQ_mask		:	1;
	unsigned	pm_IRQ_mask				:	1;
	unsigned							:  21;
}	VIQE_IRQ;

typedef	union
{
	unsigned	long			nREG;
	VIQE_IRQ			bREG;
}	VIQE_IRQ_u;

typedef	struct
{
	unsigned	global_en_dont_use			:	 1;
	unsigned	top_size_dont_use			:	 1;
	unsigned	stream_deintl_info_dont_use :	 1;
	unsigned	RESERVE0					:	29;
}	VIQE_CTRL_MISC;

typedef union
{
	unsigned	long			nREG;
	VIQE_CTRL_MISC	bREG;
}	VIQE_CTRL_MISC_u;

typedef	struct
{
	unsigned	base					:	32;
}	VIQE_BASE;

typedef	union
{
	unsigned	long			nREG;
	VIQE_BASE			bREG;
}	VIQE_BASE_u;

typedef	struct
{
	unsigned	offs0					:	16;
	unsigned	offs1					:	16;
}	VIQE_OFFS;

typedef	union
{
	unsigned	long			nREG;
	VIQE_OFFS			bREG;
}	VIQE_OFFS_u;

typedef	struct
{
	unsigned	top_size_dont_use			:	 1;
	unsigned	stream_deintl_info_dont_use	:	 1;
	unsigned	uvintpl						:	 1;
	//unsigned								:	13;
	unsigned	RESERVE0					:	 5;
	unsigned 	frmnum						: 	 2;
	unsigned	RESERVE1					:	 6;
	unsigned	enable						:	 1;
	unsigned	cfgupd						:	 1;
	unsigned	frame_rate_disable			:	 1;
	unsigned	RESERVE2					:	 5;
	unsigned	h2h							:	 8;
}	DEINTL_CTRL;

typedef	union
{
	unsigned	long			nREG;
	DEINTL_CTRL		bREG;
}	DEINTL_CTRL_u;

typedef	struct
{
	unsigned	detect_end				:	 1;
	unsigned	flush					:	 1;
	unsigned	eof_control_ready		:	 1;
	unsigned	size_fix				:	 1;
	unsigned	RESERVE0				:	 4;
	unsigned	divisor					:	 2;
	unsigned	RESERVE1				:	 2;
	unsigned	fmt						:	 5;
	unsigned	RESERVE2				:	15;
}	VIQE_FC_MISC;

typedef	union
{
	unsigned	long			nREG;
	VIQE_FC_MISC		bREG;
}	VIQE_FC_MISC_u;

typedef struct
{
	unsigned	width_dwalign			:	16;
	unsigned	full					:	 4;
	unsigned	empty					:	 4;
	unsigned	eof_in					:	 1;
	unsigned	eof_out					:	 1;
	unsigned	RESERVE0				:	 1;
	unsigned	decoder_err				:	 1;
	unsigned	decoder_state			:	 4;
}	VIQE_FC_STATUS;

typedef union
{
	unsigned	long			nREG;
	VIQE_FC_STATUS	bREG;
}	VIQE_FC_STATUS_u;

typedef	struct
{
	unsigned	fc_select				:	 8;
	unsigned	err_check				:	 1;
//   need to add here_en
	unsigned	RESERVE0				:	 6;
	unsigned	fc_stat					:	 8;
	unsigned	RESERVE1				:	 7;
	unsigned	fc_enable				:	 1;
}	VIQE_FC_CTRL;

typedef union
{
	unsigned	long			nREG;
	VIQE_FC_CTRL		bREG;
}	VIQE_FC_CTRL_u;

typedef	struct
{
	unsigned	k0_ac_length_limit		:	 6;
	unsigned	RESERVE0				:	 2;
	unsigned	k1_ac_length_limit		:	 6;
	unsigned	RESERVE1				:	 2;
	unsigned	k2_ac_length_limit		:	 6;
	unsigned	RESERVE2				:	 2;
	unsigned	RESERVE3				:	 8;
}	VIQE_FC_LIMIT;

typedef	union
{
	unsigned	long			nREG;
	VIQE_FC_LIMIT		bREG;
}	VIQE_FC_LIMIT_u;

typedef	struct
{
	unsigned	top_size_dont_use		:	 1;
	unsigned	RESERVE0				: 	 1;
	unsigned	uvintpl					:	 1;
//	unsigned	RESERVE0				:	13;
	unsigned	RESERVE1				:	 5;
	unsigned	frmnum					:	 2;
	unsigned	RESERVE2				:	 6;
	unsigned	enable					:	 1;
	unsigned	cfgupd					:	 1;
	unsigned	RESERVE3				:	 1;
	unsigned	RESERVE4				:	 5;
	unsigned	h2h						:	 8;
}	D3D_CTRL;

typedef	union
{
	unsigned	long			nREG;
	D3D_CTRL			bREG;
}	D3D_CTRL_u;

typedef	struct
{
	unsigned	pcnt_y					:	11;
	unsigned	RESERVE0				:	 5;
	unsigned	lcnt_y					:	11;
	unsigned	RESERVE1				:	 2;
	unsigned	int_mask				:	 1;
	unsigned	int_busy				:	 1;
	unsigned	busy_stat				:	 1;
}	D3D_COUNT;

typedef union
{
	unsigned	long				nREG;
	D3D_COUNT			bREG;
}	D3D_COUNT_u;

typedef struct
{
	unsigned	h2h						:	 8;
	unsigned	d3in					:	 1;
	unsigned	cfgupd					:	 1;
	unsigned	frmupd_disable			:	 1;
	unsigned	lut_init				:	 1;
	unsigned	RESERVE0				:	 4;
	unsigned	top_size_dont_use		:	 1;
	//unsigned							:	15;
	unsigned	RESERVE1				:	 7;
	unsigned	frmnum					:	 2;
	unsigned	RESERVE2				:	 6;
}	D3D_MISC;

typedef union
{
	unsigned	long			nREG;
	D3D_MISC			bREG;
}	D3D_MISC_u;

typedef struct
{
	unsigned	div_pos					:	16;
	unsigned	div_toggle				:	 1;
	unsigned	div_en					:	 1;
	unsigned	RESERVE0				:	14;
}	D3D_DIV;

typedef union
{
	unsigned	long			nREG;
	D3D_DIV			bREG;
}	D3D_DIV_u;

typedef	struct
{
	unsigned	bypass					:	 1;
	unsigned	RESERVE0				:	29;
	unsigned	flush					:	 2;
}	D3D_MISC2;

typedef	union
{
	unsigned	long			nREG;
	D3D_MISC2			bREG;
}	D3D_MISC2_u;

typedef	struct _VIOC_VIQE_CTRL
{
	volatile VIQE_CTRL_u				nVIQE_CTRL;				//  0x000
	volatile VIQE_SIZE_u				nVIQE_CTRL_SIZE;		//	0x004
	volatile VIQE_CTRL_Y2R_u			nVIQE_CTRL_Y2R;			//	0x008
	volatile VIQE_IRQ_u				nVIQE_IRQ;				//	0x00c
	volatile VIQE_CTRL_MISC_u		nVIQE_CTRL_MISC;		//	0x010
	unsigned	int		nHIDDEN;				//	0x014
	unsigned	int		undef_0x018[26];		// 	0x018 ~ 0x07C
}	VIOC_VIQE_CTRL,*PVIOC_VIQE_CTRL;

typedef	struct _VIQE_DEINTL_DMA
{
	volatile VIQE_BASE_u				nDEINTL_BASE0;			//	0x080
	volatile VIQE_BASE_u				nDEINTL_BASE1;			//	0x084
	volatile VIQE_BASE_u				nDEINTL_BASE2;			//	0x088
	volatile VIQE_BASE_u				nDEINTL_BASE3;			//	0x08c
	volatile VIQE_SIZE_u				nDEINTL_SIZE;			//  0x090
	volatile VIQE_OFFS_u				nDEINTL_OFFS;			//	0x094
	volatile DEINTL_CTRL_u			nDEINTL_CTRL;			//	0x098
	unsigned	int			undef_0x09c[1];			//  0x09c
	volatile VIQE_BASE_u				nDEINTL_BASE0A;			//  0x0A0
	volatile VIQE_BASE_u				nDEINTL_BASE1A;			//  0x0A4
	volatile VIQE_BASE_u				nDEINTL_BASE2A;			//  0x0A8
	volatile VIQE_BASE_u				nDEINTL_BASE3A;			//	0x0AC
	volatile VIQE_BASE_u				nDEINTL_BASE0B;			//  0x0B0
	volatile VIQE_BASE_u				nDEINTL_BASE1B;			//  0x0B4
	volatile VIQE_BASE_u				nDEINTL_BASE2B;			//  0x0B8
	volatile VIQE_BASE_u				nDEINTL_BASE3B;			//	0x0BC
	volatile VIQE_BASE_u				nDEINTL_BASE0C;			//  0x0C0
	volatile VIQE_BASE_u				nDEINTL_BASE1C;			//  0x0C4
	volatile VIQE_BASE_u				nDEINTL_BASE2C;			//  0x0C8
	volatile VIQE_BASE_u				nDEINTL_BASE3C;			//	0x0CC
	unsigned	int		nDEINTL_CUR_BASE0;		//	0x0D0
	unsigned	int		nDEINTL_CUR_BASE1;		//	0x0D4
	unsigned	int		nDEINTL_CUR_BASE2;		//	0x0D8
	unsigned	int		nDEINTL_CUR_BASE3;		//	0x0Dc
	unsigned	int		nDEINTL_CUR_WDMA;		//	0x0E0
	unsigned	int		nDEINTL_CUR_RDMA;		//	0x0E4
	unsigned	int		undef_0x0b8[6];			//	0x0E8 ~ 0x0FC
}	VIQE_DEINTL_DMA,*PVIQE_DEINTL_DMA;

typedef	struct
{
	volatile VIQE_FC_MISC_u			nVIQE_FC_MISC;			// 0x100
	volatile VIQE_SIZE_u				nVIQE_FC_SIZE;			// 0x104
	volatile VIQE_FC_STATUS_u			nVIQE_FC_STATUS;		// 0x108
	volatile VIQE_FC_CTRL_u			nVIQE_FC_CTRL;			// 0x10c
	unsigned	int		undef0[4];		// 0x110~0x11C
}	VIQE_FC;

typedef	struct
{
	volatile VIQE_FC_MISC_u			nVIQE_FC_MISC;			// 0x160
	unsigned int undef_0x164;			// 0x164
	volatile VIQE_FC_STATUS_u			nVIQE_FC_STATUS;		// 0x168
	volatile VIQE_FC_CTRL_u			nVIQE_FC_CTRL;			// 0x16c
	volatile VIQE_FC_LIMIT_u			nVIQE_FC_LIMIT;			// 0x170
	unsigned	int		undef0[3];		// 0x174~0x17C
}	VIQE_FC_COMP;


typedef struct
{
	volatile unsigned int 		nDI_CTRL;			//0x280
	volatile unsigned int 		nDI_ENGINE0;		//0x284
	volatile unsigned int 		nDI_ENGINE1;		//0x288
	volatile unsigned int 		nDI_ENGINE2;		//0x28C
	volatile unsigned int 		nDI_ENGINE3;		//0x290
	volatile unsigned int 		nDI_ENGINE4;		//0x294
	volatile unsigned int 		nPD_THRES0;			//0x298 
	volatile unsigned int 		nPD_THRES1;			//0x29C
	volatile unsigned int 		nPD_JUDDER;			//0x2A0
	volatile unsigned int 		nPD_JUDDER_M;		//0x2A4
	volatile unsigned int 		nDI_MISCC;			//0x2A8
	volatile unsigned int 		nDI_STATUS;			//0x2AC
	volatile unsigned int 		nPD_STATUS;			//0x2B0
	volatile unsigned int 		nDI_REGION0;		//0x2B4
	volatile unsigned int 		nDI_REGION1;		//0x2B8
	volatile unsigned int 		nDI_INT;				//0x2BC
	volatile unsigned			undef_0[8];			//  0x2C0 ~ 0x2DC (deintl)
	volatile unsigned int 		nPD_SAW;			//0x2E0
	volatile unsigned int 		nDI_CSIZE;			//0x2E4
	volatile unsigned int 		nDI_FMT;			//0x2E8
	volatile unsigned			undef_1[5];			//  0x2EC ~ 0x2FC (deintl)

}	VIQE_DEINTL;

typedef	struct _VIOC_VIQE
{
	volatile VIOC_VIQE_CTRL				cVIQE_CTRL;				//	0x000 ~ 0x07c
	volatile VIQE_DEINTL_DMA			cDEINTL_DMA;			//	0x080 ~ 0x0fc
	volatile VIQE_FC					cDEINTL_DECOMP0;		//  0x100 ~ 0x11C
	volatile VIQE_FC					cDEINTL_DECOMP1;		//	0x120 ~ 0x13c
	volatile VIQE_FC					cDEINTL_DECOMP2;		//	0x140 ~ 0x15c
	volatile VIQE_FC_COMP			cDEINTL_COMP;			//	0x160 ~ 0x17c
	unsigned	int		undef_0[64];			//	0x180 ~ 0x27c
	volatile VIQE_DEINTL				cDEINTL;				//  0x280 ~ 0x2fc (deintl)
}	VIQE,*PVIQE;


/************************************************************************
*   3.6.13   Asynchronous Frame FIFO 			(Base Addr = 0x12003900)
*************************************************************************/

typedef	struct {
	unsigned				WEN			:  1;
	unsigned				REN0		:  1;
	unsigned				REN1		:  1;
	unsigned				REN2		:  1;
	unsigned				RMODE_TIME	:  1;
	unsigned 				   			:  3;
	unsigned				NENTRY    	:  6;
	unsigned				WMODE_TIME	:  1;
	unsigned				WMODE_FULL	:  1;
	unsigned				EFULL    	:  2;
	unsigned				EEMPTY   	:  2;
	unsigned 				   			:  4;
	unsigned				IE_FULL		:  1;
	unsigned				IE_EFULL	:  1;
	unsigned				IE_EMPTY	:  1;
	unsigned				IE_EEMPTY	:  1;
	unsigned				IE_WEOF		:  1;
	unsigned				IE_REOF0	:  1;
	unsigned				IE_REOF1	:  1;
	unsigned				IE_REOF2	:  1;
}	VIOC_FIFO_CTRL0;

typedef	union {
	unsigned	long			nREG;
	VIOC_FIFO_CTRL0		bREG;
}	VIOC_FIFO_CTRL0_u;

typedef	struct {
	unsigned				WDMA		:  4;
	unsigned				RDMA0		:  4;
	unsigned				RDMA1		:  4;
	unsigned				RDMA2		:  4;
	unsigned				FAST_RDMA	:  2;
	unsigned 				   			: 14;
}	VIOC_FIFO_CTRL1;

typedef	union {
	unsigned	long			nREG;
	VIOC_FIFO_CTRL1		bREG;
}	VIOC_FIFO_CTRL1_u;

typedef	struct {
	unsigned				IS_FULL		:  1;
	unsigned				IS_EFULL	:  1;
	unsigned				IS_EMPTY	:  1;
	unsigned				IS_EEMPTY	:  1;
	unsigned				IS_WEOF		:  1;
	unsigned				IS_REOF0	:  1;
	unsigned				IS_REOF1	:  1;
	unsigned				IS_REOF2	:  1;
	unsigned 								: 24;
}	VIOC_FIFO_IRQSTAT;

typedef	union {
	unsigned	long			nREG;
	VIOC_FIFO_IRQSTAT	bREG;
}	VIOC_FIFO_IRQSTAT_u;

typedef	struct {
	unsigned				FULL		:  1;
	unsigned				EFULL		:  1;
	unsigned				EMPTY		:  1;
	unsigned				EEMPTY		:  1;
	unsigned				WEOF		:  1;
	unsigned				REOF0		:  1;
	unsigned				REOF1		:  1;
	unsigned				REOF2		:  1;
	unsigned 							: 16;
	unsigned				FILLED		:  8;
}	VIOC_FIFO_FIFOSTAT;

typedef	union {
	unsigned	long		nREG;
	VIOC_FIFO_FIFOSTAT	bREG;
}	VIOC_FIFO_FIFOSTAT_u;

typedef	struct	_VIOC_FIFO
{
	volatile VIOC_FIFO_CTRL0_u			uCH0_CTRL0;				// 0x000
	volatile VIOC_FIFO_CTRL1_u			uCH0_CTRL1;				// 0x004
	volatile VIOC_FIFO_IRQSTAT_u		uCH0_IRQSTAT;			// 0x008
	volatile VIOC_FIFO_FIFOSTAT_u		uCH0_FIFOSTAT;			// 0x00C
	volatile VIOC_FIFO_CTRL0_u			uCH1_CTRL0;				// 0x010
	volatile VIOC_FIFO_CTRL1_u			uCH1_CTRL1;				// 0x014
	volatile VIOC_FIFO_IRQSTAT_u		uCH1_IRQSTAT;			// 0x018
	volatile VIOC_FIFO_FIFOSTAT_u		uCH1_FIFOSTAT;			// 0x01C
	unsigned	int				nCH0_BASE[16];		// 0x020~0x05C
	unsigned	int				nCH1_BASE[16];		// 0x060~0x09C
}	VIOC_FIFO,*PVIOC_FIFO;


/************************************************************************
*   3.6.16   VIN_DEMUX				(Base Addr = 0x1200A800)
*************************************************************************/

typedef struct
{
	unsigned	enable				:	 1;
	unsigned					:	 3;
	unsigned	byte_select			:	 2;
	unsigned					:	 2;
	unsigned	clk_mode			:	 3;
	unsigned					:	 5;
	unsigned	sel0					:	 3;
	unsigned					:	 1;
	unsigned	sel1					:	 3;
	unsigned 					:	 1;
	unsigned	sel2					: 	 3;
	unsigned 					:	 1;
	unsigned	sel3					:	 3;
	unsigned 					:	 1;
}	VIN_DEMUX_CTRL;

typedef	union
{
	unsigned	long			nREG;
	VIN_DEMUX_CTRL		bVIN_DEMUX_CTRL;
}	VIN_DEMUX_CTRL_u;

typedef	struct
{
	unsigned	hb					:	 4;
	unsigned	vb					:	 4;
	unsigned	fp					:	 4;
	unsigned					:	 4;
	unsigned	psl					:	 2;
	unsigned	sync_byte			:	 2;
	unsigned					:	12;
}	VIN_DEMUX_BLANK0;

typedef union
{
	unsigned	long		nREG;
	VIN_DEMUX_BLANK0		bREG;
}	VIN_DEMUX_BLANK0_u;

typedef struct
{
	unsigned	pf					:	 8;
	unsigned	ps					:	 8;
	unsigned	pt					:	 8;
	unsigned	RESERVE0			:	 8;
}	VIN_DEMUX_BLANK1;

typedef union
{
	unsigned	long		nREG;
	VIN_DEMUX_BLANK1		bREG;
}	VIN_DEMUX_BLANK1_u;

typedef struct
{
	unsigned	dsel0				:	 4;
	unsigned	dsel1				:	 4;
	unsigned	dsel2				:	 4;
	unsigned	dsel3				:	 4;
	unsigned	RESERVE0			:	16;
}	VIN_DEMUX_MISC;

typedef union
{
	unsigned	long		nREG;
	VIN_DEMUX_MISC		bREG;
}	VIN_DEMUX_MISC_u;

typedef struct
{
	unsigned	idx0				:	 4;
	unsigned	idx1				:	 4;
	unsigned	idx2				:	 4;
	unsigned	idx3				:	 4;
	unsigned	RESERVE0			:	16;
}	VIN_DEMUX_STS;

typedef	union
{
	unsigned	long		nREG;
	VIN_DEMUX_STS			bREG;
}	VIN_DEMUX_STS_u;


typedef	struct _VOIC_VIN_DEMUX
{
	volatile VIN_DEMUX_CTRL_u		uVIN_DEMUX_CTRL;	//	0x000 R/W 0x32100000 VIN DEMUX Control register
	volatile VIN_DEMUX_BLANK0_u		uVIN_DEMUX_BLANK0;	//	0x004 R/W 0x00030CB9 VIN DEMUX Blank Set 0 Register
	volatile VIN_DEMUX_BLANK1_u		uVIN_DEMUX_BLANK1;	//	0x008 R/W 0x000000FF VIN DEMUX Black Set 1 Register
	volatile VIN_DEMUX_MISC_u		uVIN_DEMUX_MISC;	//	0x00C R/W 0x00004444 VIN DEMUX Miscellaneous Register
	volatile VIN_DEMUX_STS_u		uVIN_DEMUX_STS;		//	0x010 R     -                VIN DEMUX Status Register
}	VIOC_VIN_DEMUX,*PVIOC_VIN_DEMUX;



/************************************************************************
*   3.6.18   Configuration & Interrupt			(Base Addr = 0x1200A000)
*************************************************************************/

typedef	struct
{
	unsigned				DEV0	: 1;
	unsigned				DEV1	: 1;
	unsigned				DEV2	: 1;
	unsigned				TIMER	: 1;
	unsigned				RDMA00	: 1;
	unsigned				RDMA01	: 1;
	unsigned				RDMA02	: 1;
	unsigned				RDMA03	: 1;
	unsigned				RDMA04	: 1;
	unsigned				RDMA05	: 1;
	unsigned				RDMA06	: 1;
	unsigned				RDMA07	: 1;
	unsigned				RDMA08	: 1;
	unsigned				RDMA09	: 1;
	unsigned				RDMA10	: 1;
	unsigned				RDMA11	: 1;
	unsigned				RDMA12	: 1;
	unsigned				RDMA13	: 1;
	unsigned				RDMA14	: 1;
	unsigned				RDMA15	: 1;
	unsigned				RDMA16	: 1;
	unsigned				RDMA17	: 1;
	unsigned 						: 2;
	unsigned				MMU		: 1;
	unsigned 						: 3;
	unsigned				FIFO0	: 1;
	unsigned				FIFO1	: 1;
	unsigned 						: 2;
	unsigned				WDMA00	: 1;
	unsigned				WDMA01	: 1;
	unsigned				WDMA02	: 1;
	unsigned				WDMA03	: 1;
	unsigned				WDMA04	: 1;
	unsigned				WDMA05	: 1;
	unsigned				WDMA06	: 1;
	unsigned				WDMA07	: 1;
	unsigned				WDMA08	: 1;
	unsigned 						: 7;
	unsigned				WMIX0	: 1;
	unsigned				WMIX1	: 1;
	unsigned				WMIX2	: 1;
	unsigned				WMIX3	: 1;
	unsigned				WMIX4	: 1;
	unsigned				WMIX5	: 1;
	unsigned 						: 5;
	unsigned				VIQE	: 1;
	unsigned				SC0		: 1;
	unsigned				SC1		: 1;
	unsigned				SC2		: 1;
	unsigned				SC3		: 1;
}	VIOC_IREQ_IREQ;

typedef	union {
	unsigned	long			nREG[2];
	VIOC_IREQ_IREQ	bREG;
}	VIOC_IREQ_IREQ_u;

typedef struct {
	unsigned 				INDEX	: 6;
	unsigned 		 			: 10;
	unsigned 					: 15;
	unsigned 				IVALID : 1 ;
}VIOC_IREQ_VECTORID;

typedef union
{
	unsigned long			nREG;
	VIOC_IREQ_VECTORID	bREG;
}VIOC_IREQ_VECTORID_u;

typedef struct
{
	unsigned 				L0EVSEL	:  3;				// TCC8910 | TCC8010
	unsigned 						:  1;
	unsigned 				L1EVSEL	:  3;				// TCC8910
	unsigned 						:  1;
	unsigned 				L2EVSEL	:  3;				// TCC8910
	unsigned 						:  1;
	unsigned 						:  4;
	unsigned 				WMIX0_0	:  1;	// 16		// TCC8910 | TCC8010
	unsigned 				WMIX0_1	:  1;	// 17		// TCC8910 | TCC8010
	unsigned 				WMIX1_0	:  1;	// 18		// TCC8910 | TCC8010
	unsigned 				WMIX1_1	:  1;	// 19		// TCC8910 | TCC8010
	unsigned 				WMIX2_0	:  1;	// 20		// TCC8910 | TCC8010
	unsigned 				WMIX2_1	:  1;	// 21		// TCC8910 | TCC8010
	unsigned 				WMIX3_0	:  1;	// 22		// TCC8910
	unsigned 				WMIX3_1	:  1;	// 23		// TCC8910
	unsigned 				WMIX4_0	:  1;	// 24		// TCC8910
	unsigned 				WMIX4_1	:  1;	// 25		// TCC8910
	unsigned 				WMIX5_0	:  1;	// 26		// TCC8910
	unsigned 				WMIX5_1	:  1;	// 27		// TCC8910
	unsigned 				WMIX6_0	:  1;	// 28		// TCC8910
	unsigned 				WMIX6_1	:  1;	// 29		// TCC8910
	unsigned 				RDMA12	:  1;	// 30
	unsigned 				RDMA14	:  1;	// 31
}VIOC_CONFIG_ETC;

typedef	union {
	unsigned long				nREG;
	VIOC_CONFIG_ETC		bREG;
}	VIOC_CONFIG_ETC_u;

typedef struct {
	unsigned 				SELECT    	:  8;
	unsigned 							:  8;
	unsigned 				STATUS     	:  2;
	unsigned 				ERR      	:  1;
	unsigned 							:  5;
	unsigned 							:  7;
	unsigned 				EN		     	:  1;
}VIOC_CONFIG_PATH;

typedef	union {
	unsigned long				nREG;
	VIOC_CONFIG_PATH		bREG;
}VIOC_CONFIG_PATH_u;


typedef	struct {
	unsigned				RDMA 	: 1;
	unsigned				SCALER	: 1;
	unsigned				MIXER	: 1;
	unsigned				WDMA	: 1;
	unsigned				FDLY	: 1;
	unsigned				VIQE	: 1;
	unsigned				DEINTLS	: 1;
	unsigned				FILT2D	: 1;
	unsigned				FCENC	: 1;
	unsigned				FCDEC	: 1;
	unsigned				CPUIF	: 1;
	unsigned				LCD2AHB	: 1;
	unsigned				VIDEOIN	: 1;
	unsigned				DEBLOCK	: 1;
	unsigned						: 18;
}	VIOC_POWER_AUTOPD;

typedef	union {
	unsigned	long			nREG;
	VIOC_POWER_AUTOPD	bREG;
}	VIOC_POWER_AUTOPD_u;

typedef	struct {
	unsigned				MIN			: 4;
	unsigned							: 4;
	unsigned				EN			: 1;
	unsigned				PROF_EN		: 1;
	unsigned							: 5;
	unsigned							: 1;
	unsigned							: 16;
}	VIOC_POWER_CLKCTRL;

typedef	union {
	unsigned	long			nREG;
	VIOC_POWER_CLKCTRL	bREG;
}	VIOC_POWER_CLKCTRL_u;

typedef	struct {
	unsigned				RDMA00		: 1;
	unsigned				RDMA01		: 1;
	unsigned				RDMA02		: 1;
	unsigned				RDMA03		: 1;
	unsigned				RDMA04		: 1;
	unsigned				RDMA05		: 1;
	unsigned				RDMA06		: 1;
	unsigned				RDMA07		: 1;
	unsigned				RDMA08		: 1;
	unsigned				RDMA09		: 1;
	unsigned				RDMA10		: 1;
	unsigned				RDMA11		: 1;
	unsigned				RDMA12		: 1;
	unsigned				RDMA13		: 1;
	unsigned				RDMA14		: 1;
	unsigned				RDMA15		: 1;
	unsigned				RDMA16		: 1;
	unsigned				RDMA17		: 1;
	unsigned				FCDEC0		: 1;
	unsigned				FCDEC1		: 1;
	unsigned				FCDEC2		: 1;
	unsigned				FCDEC3		: 1;
	unsigned				FCENC0		: 1;
	unsigned				FCENC1		: 1;
	unsigned				VIDEOIN0	: 1;
	unsigned				VIDEOIN1	: 1;
	unsigned							: 1;
	unsigned				FRAMEFIFO	: 1;
	unsigned				SCALER0		: 1;
	unsigned				SCALER1		: 1;
	unsigned				SCALER2		: 1;
	unsigned				SCALER3		: 1;		// 32
	unsigned				WDMA00		: 1;
	unsigned				WDMA01		: 1;
	unsigned				WDMA02		: 1;
	unsigned				WDMA03		: 1;
	unsigned				WDMA04		: 1;
	unsigned				WDMA05		: 1;
	unsigned				WDMA06		: 1;
	unsigned				WDMA07		: 1;
	unsigned				WDMA08		: 1;
	unsigned							: 1;
	unsigned				WMIX0 		: 1;
	unsigned				WMIX1 		: 1;
	unsigned				WMIX2 		: 1;
	unsigned				WMIX3 		: 1;
	unsigned				WMIX4 		: 1;
	unsigned				WMIX5 		: 1;
	unsigned				VIQE 		: 1;
	unsigned				DEINTLS		: 1;
	unsigned							: 2;
	unsigned				DEV0 		: 1;
	unsigned				DEV1 		: 1;
	unsigned				DEV2 		: 1;
	unsigned				MMU  		: 1;
	unsigned				FILT2D 		: 1;
	unsigned				DEBLOCK		: 1;
	unsigned				FDLY0 		: 1;
	unsigned				FDLY1 		: 1;
	unsigned							: 2;
	unsigned							: 2;
}	VIOC_POWER_BLOCKS;

typedef	union {
	unsigned	long			nREG[2];
	VIOC_POWER_BLOCKS	bREG;
}	VIOC_POWER_BLOCKS_u;

typedef	struct	_VIOC_IREQ_CONFIG
{
	volatile VIOC_IREQ_IREQ_u 			uRAWSTATUS;		// 0x00~0x04
	volatile VIOC_IREQ_IREQ_u 			uSYNCSTATUS;		// 0x08~0x0C
	volatile VIOC_IREQ_IREQ_u 			uIREQSELECT;		// 0x10~0x14
	volatile VIOC_IREQ_IREQ_u 			nIRQMASKSET;		// 0x18~0x1C
	volatile VIOC_IREQ_IREQ_u 			nIRQMASKCLR;		// 0x20~0x24
	volatile VIOC_IREQ_VECTORID_u 	nVECTORID;			// 0x28
	unsigned	int 						reserved0[5];		// 0x2C~0x3C
	volatile VIOC_CONFIG_ETC_u 		uMISC;				// 0x40
	volatile VIOC_CONFIG_PATH_u 		uSC0;				// 0x44
	volatile VIOC_CONFIG_PATH_u 		uSC1;				// 0x48
	volatile VIOC_CONFIG_PATH_u 		uSC2;				// 0x4C
	volatile VIOC_CONFIG_PATH_u 		uSC3;				// 0x50
	volatile VIOC_CONFIG_PATH_u 		uVIQE;				// 0x54
	volatile VIOC_CONFIG_PATH_u 		uDEINTLS;			// 0x58
	unsigned	int 						reserved1[7];		// 0x5C~0x74
	volatile VIOC_CONFIG_PATH_u 		uFDELAY0;			// 0x78
	#if defined(CONFIG_ARCH_TCC893X) 					//20121120 ysseung   add to MISC1 config register..
	unsigned int 						reserved2[2];		// 0x7C~0x80
	volatile VIOC_CONFIG_ETC_u 		uMISC1;				// 0x84
	unsigned int 						reserved3[14];		// 0x88~0xBC
	#else
	unsigned int 						reserved2[17]; 		// 0x7C~0xBC
	#endif
	unsigned int						nARID;				// 0xC0
	unsigned int						nAWID;				// 0xC4
	volatile VIOC_POWER_AUTOPD_u 	uAUTOPD;			// 0xC8
	volatile VIOC_POWER_CLKCTRL_u 	uCLKCTRL;			// 0xCC
	volatile VIOC_POWER_BLOCKS_u 	uPOWERDOWN;		// 0xD0~0xD4
	volatile VIOC_POWER_BLOCKS_u 	uSOFTRESET;		// 0xD8~0xDC
}	VIOC_IREQ_CONFIG,*PVIOC_IREQ_CONFIG;




/************************************************************************
*   8.3	DDI_CONFIG                                (Base Addr = 0x72380000)
*************************************************************************/


typedef struct {
	unsigned VIOC		:  1;
	unsigned NTSCPAL		:  1;
	unsigned HDMI		:  1;
	unsigned	G2D			:  1;
	unsigned	RESERVED0	:28;
} DDI_CONFIG_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_CONFIG_IDX_TYPE		bREG;
} DDI_CONFIG_TYPE;

typedef struct {
	unsigned CIF0			:3;
	unsigned				:1;
	unsigned CIF1			:3;
	unsigned				:1;
	unsigned CIF2			:3;
	unsigned				:1;
	unsigned CIF3			:3;
	unsigned				:1;
	unsigned DEMUX			:3;
	unsigned				:13;
} DDI_CIFPORT_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_CIFPORT_IDX_TYPE	bREG;
} DDI_CIFPORT_TYPE;

typedef struct {
	unsigned 		RESET		:  4;
	unsigned 		SPDIF 		:  2;
	unsigned		RESERVED0	:  2;
	unsigned		CLK			:  2;
	unsigned		PAD			:  1;
	unsigned		REF			:  1;
	unsigned		RESERVED1	:  2;
	unsigned		OP			:  1;
	unsigned 		EN			:  1;
	unsigned		RESEVERD2	:  16;
} DDI_HDMICTRL_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_HDMICTRL_IDX_TYPE	bREG;
} DDI_HDMICTRL_TYPE;

typedef struct {
	unsigned VLD			:1;
	unsigned				:31;
} DDI_HDMIAES_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_HDMIAES_IDX_TYPE	bREG;
} DDI_HDMIAES_TYPE;

typedef struct {
	unsigned DATA			:32;
} DDI_HDMIAESD0_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_HDMIAESD0_IDX_TYPE	bREG;
} DDI_HDMIAESD0_TYPE;

typedef struct {
	unsigned DATA			:1;
	unsigned				:31;
} DDI_HDMIAESD1_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_HDMIAESD1_IDX_TYPE	bREG;
} DDI_HDMIAESD1_TYPE;

typedef struct {
	unsigned HW				:32;
} DDI_HDMIAESHW_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_HDMIAESHW_IDX_TYPE	bREG;
} DDI_HDMIAESHW_TYPE;

typedef struct {
	unsigned HW				:31;
	unsigned				:1;
} DDI_HDMIAESHW2_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_HDMIAESHW2_IDX_TYPE	bREG;
} DDI_HDMIAESHW2_TYPE;

typedef struct {
	unsigned SEL			:1;
	unsigned				:31;
} DDI_NTSCPALEN_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_NTSCPALEN_IDX_TYPE	bREG;
} DDI_NTSCPALEN_TYPE;

typedef struct {
	unsigned 					:1;
	unsigned RST				:1;
	unsigned EN				:1;
	unsigned OC				:1;
	unsigned VSEL			:1;
	unsigned S				:3;
	unsigned M				:7;
	unsigned P				:6;
	unsigned TC				:3;
	unsigned					:6;
	unsigned	SEL				:2;
} DDI_LVDSCTRL_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSCTRL_IDX_TYPE	bREG;
} DDI_LVDSCTRL_TYPE;

typedef struct {
	unsigned SEL_00			:8;
	unsigned SEL_01			:8;
	unsigned SEL_02			:8;
	unsigned SEL_03			:8;
} DDI_LVDSTXSEL0_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL0_IDX_TYPE	bREG;
} DDI_LVDSTXSEL0_TYPE;

typedef struct {
	unsigned SEL_04			:8;
	unsigned SEL_05			:8;
	unsigned SEL_06			:8;
	unsigned SEL_07			:8;
} DDI_LVDSTXSEL1_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL1_IDX_TYPE	bREG;
} DDI_LVDSTXSEL1_TYPE;

typedef struct {
	unsigned SEL_08			:8;
	unsigned SEL_09			:8;
	unsigned SEL_10			:8;
	unsigned SEL_11			:8;
} DDI_LVDSTXSEL2_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL2_IDX_TYPE	bREG;
} DDI_LVDSTXSEL2_TYPE;

typedef struct {
	unsigned SEL_12			:8;
	unsigned SEL_13			:8;
	unsigned SEL_14			:8;
	unsigned SEL_15			:8;
} DDI_LVDSTXSEL3_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL3_IDX_TYPE	bREG;
} DDI_LVDSTXSEL3_TYPE;

typedef struct {
	unsigned SEL_16			:8;
	unsigned SEL_17			:8;
	unsigned SEL_18			:8;
	unsigned SEL_19			:8;
} DDI_LVDSTXSEL4_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL4_IDX_TYPE	bREG;
} DDI_LVDSTXSEL4_TYPE;

typedef struct {
	unsigned SEL_20			:8;
	unsigned SEL_21			:8;
	unsigned SEL_22			:8;
	unsigned SEL_23			:8;
} DDI_LVDSTXSEL5_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL5_IDX_TYPE	bREG;
} DDI_LVDSTXSEL5_TYPE;

typedef struct {
	unsigned SEL_24			:8;
	unsigned SEL_25			:8;
	unsigned SEL_26			:8;
	unsigned SEL_27			:8;
} DDI_LVDSTXSEL6_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL6_IDX_TYPE	bREG;
} DDI_LVDSTXSEL6_TYPE;

typedef struct {
	unsigned SEL_28			:8;
	unsigned SEL_29			:8;
	unsigned SEL_30			:8;
	unsigned SEL_31			:8;
} DDI_LVDSTXSEL7_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL7_IDX_TYPE	bREG;
} DDI_LVDSTXSEL7_TYPE;

typedef struct {
	unsigned SEL_32			:8;
	unsigned SEL_33			:8;
	unsigned SEL_34			:8;
	unsigned				:8;
} DDI_LVDSTXSEL8_IDX_TYPE;

typedef union {
	unsigned long			nREG;
	DDI_LVDSTXSEL8_IDX_TYPE	bREG;
} DDI_LVDSTXSEL8_TYPE;

typedef struct {
    unsigned    INTTYPE     :   1; // 0: egde, 1:level
    unsigned                :   15;
    unsigned    RESETN      :   1;
    unsigned                :   15;
} MIPICFG;
typedef union {
    unsigned long        nReg;
    MIPICFG     bReg;
} MIPICFG_U;

typedef struct
{
	unsigned		AWCACHM0_SEL	:	4;
	unsigned		AWCHCHEM0		:	4;
	unsigned		ARCACHEM0_SEL	:	4;
	unsigned 		ARCACHEM0		:	4;
	unsigned		AWCACHM1_SEL	:	4;
	unsigned		AWCHCHEM1		:	4;
	unsigned		ARCACHEM1_SEL	:	4;
	unsigned 		ARCACHEM1		:	4;
} DDI_LVDS_CACHECTRL;

typedef union
{
	unsigned 	long			nREG;
	DDI_LVDS_CACHECTRL	bREG;
}	DDI_LVDS_CACHECTRL_u;

typedef struct
{
	unsigned		VF	:	1;
	unsigned		AS	:	1;
	unsigned		DSK_CNTS	:	12;
	unsigned		PPMS	:	6;
	unsigned		SK_CUR	:	2;
	unsigned		SDA		:	1;
	unsigned		C_TDY	:	1;
	unsigned		SKEH	:	1;
	unsigned		SKIN		:	1;
	unsigned		SK_BIAS	:	4;
	unsigned		ADS		:	1;
	unsigned		CPOL 	:	1;
}	DDI_LVDS_MISC0;

typedef union
{
	unsigned	long		nREG;
	DDI_LVDS_MISC0	bREG;
}	DDI_LVDS_MISC0_u;

typedef struct
{
	unsigned		FC 		:	3;
	unsigned		CPH		:	8;
	unsigned		CVH		:	8;
	unsigned		ST		:	1;
	unsigned		VHS		:	1;
	unsigned		LC		:	1;
	unsigned		CC		:	2;
	unsigned		CMS		:	1;
	unsigned		VOC		:	1;
	unsigned		FLT_CNT		:	1;

	unsigned		RESERVED0	:	3;
	unsigned		TCM		:	1;
	unsigned		AM		:	1;
}	DDI_LVDS_MISC1;

typedef union
{
	unsigned	long		nREG;
	DDI_LVDS_MISC1	bREG;
}	DDI_LVDS_MISC1_u;

typedef struct
{
	unsigned		SKCCK	:	3;
	unsigned		SKC0	:	3;
	unsigned		SKC1	:	3;
	unsigned		SKC2	:	3;
	unsigned		SKC3	:	3;
	unsigned		SKC4	:	3;
	unsigned		TXD		:	3;
	unsigned		ITDX	:	3;
	unsigned		RESERVED	:	8;
}	DDI_LVDS_MISC2;

typedef union
{
	unsigned	long		nREG;
	DDI_LVDS_MISC2	bREG;
}	DDI_LVDS_MISC2_u;

typedef	struct
{
	unsigned		BE 		:	1;
	unsigned		BR 		: 	1;
	unsigned		BCS		:	3;
	unsigned		BDI		:	2;
	unsigned		BCI 		: 	2;
	unsigned		BSC		:	6;
	unsigned		BFE		:	1;
	unsigned		BUP		:	7;
	unsigned		BPS		:	2;
	unsigned		DB		:	1;
	unsigned		SB		:	1;
	unsigned		RESERVED0	:	1;
	unsigned		ON_3D	:	1;
	unsigned		DLYS_BST	:	1;
	unsigned		RESERVED1	:	2;
}	DDI_LVDS_MISC3;

typedef union
{
	unsigned	long		nREG;
	DDI_LVDS_MISC3	bREG;
}	DDI_LVDS_MISC3_u;


typedef struct _DDICONFIG{
	volatile DDI_CONFIG_TYPE		PWDN;			// 0x000  R/W  0x00000000	Power Down
	volatile DDI_CONFIG_TYPE		SWRESET;		// 0x004  R/W  0x00000000	Soft Reset
	unsigned :32;
	volatile DDI_CIFPORT_TYPE		CIFPORT;		// 0x00C  R/W  0x00043210	CIF select
	volatile DDI_HDMICTRL_TYPE		HDMI_CTRL;		// 0x010  R/W  0x00000000	HDMI Control
	volatile DDI_HDMIAES_TYPE		HDMI_AES;		// 0x014  R/W  0x00000000	HDMI AES
	volatile DDI_HDMIAESD0_TYPE		HDMI_AES_DATA0;	// 0x018  R/W  0x00000000	HDMI AES DATA #0
	volatile DDI_HDMIAESD1_TYPE		HDMI_AES_DATA1;	// 0x01C  R/W  0x00000000	HDMI AES DATA #1
	volatile DDI_HDMIAESHW_TYPE		HDMI_AES_HW0;	// 0x020  R/W  0x00000000	HDMI AES HW #0
	volatile DDI_HDMIAESHW_TYPE		HDMI_AES_HW1;	// 0x024  R/W  0x00000000	HDMI AES HW #1
	volatile DDI_HDMIAESHW2_TYPE		HDMI_AES_HW2;	// 0x028  R/W  0x00000000	HDMI AES HW #2
	unsigned :32;
	volatile DDI_NTSCPALEN_TYPE		NTSCPAL_EN;		// 0x030  R/W  0x00000001	NTSCPAL Encoder Enable
	unsigned :32;
	volatile MIPICFG_U           uMIPICFG       ; // 14
	unsigned :32;
	volatile DDI_LVDSCTRL_TYPE		LVDS_CTRL;		// 0x040  R/W  0x00850A01	LVDS Control register
	volatile DDI_LVDSTXSEL0_TYPE	LVDS_TXO_SEL0;	// 0x044  R/W  0x03020100	LVDS TXOUT select #0
	volatile DDI_LVDSTXSEL1_TYPE	LVDS_TXO_SEL1;	// 0x048  R/W  0x09080504	LVDS TXOUT select #1
	volatile DDI_LVDSTXSEL2_TYPE	LVDS_TXO_SEL2;	// 0x04C  R/W  0x0D0C0B0A	LVDS TXOUT select #2
	volatile DDI_LVDSTXSEL3_TYPE	LVDS_TXO_SEL3;	// 0x050  R/W  0x13121110	LVDS TXOUT select #3
	volatile DDI_LVDSTXSEL4_TYPE	LVDS_TXO_SEL4;	// 0x054  R/W  0x1A191514	LVDS TXOUT select #4
	volatile DDI_LVDSTXSEL5_TYPE	LVDS_TXO_SEL5;	// 0x058  R/W  0x0E070618	LVDS TXOUT select #5
	volatile DDI_LVDSTXSEL6_TYPE	LVDS_TXO_SEL6;	// 0x05C  R/W  0x1B17160F	LVDS TXOUT select #6
	volatile DDI_LVDSTXSEL7_TYPE	LVDS_TXO_SEL7;	// 0x060  R/W  0x1F1E1F1E	LVDS TXOUT select #7
	volatile DDI_LVDSTXSEL8_TYPE	LVDS_TXO_SEL8;	// 0x064  R/W  0x001E1F1E	LVDS TXOUT select #8
	volatile DDI_LVDS_CACHECTRL	LVDS_CACHE_CTRL	; // 0x068  R/W  0x22222222  DMA CACHE Control
	unsigned :32;									  // 0x06C RESERVED
	volatile DDI_LVDS_MISC0_u		LVDS_MISC0;		  // 0x070  R/W  0x00000000 LVDS Miscellaneous 0 Register
	volatile DDI_LVDS_MISC1_u		LVDS_MISC1;		  // 0x074  R/W  0x00000000 LVDS Miscellaneous 1 Register

	volatile DDI_LVDS_MISC2_u		LVDS_MISC2;		  // 0x074  R/W  0x00000000 LVDS Miscellaneous 2 Register

	volatile DDI_LVDS_MISC3_u		LVDS_MISC3;		  // 0x074  R/W  0x00000000 LVDS Miscellaneous 3Register

}DDICONFIG, *PDDICONFIG;

#endif /* _STRUCTURES_DISPLAY_H_ */
