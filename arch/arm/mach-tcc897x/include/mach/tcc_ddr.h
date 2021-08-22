/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#ifndef __TCC_DDR__H__
#define __TCC_DDR__H__

#include <common.h>


#if 1 //defined(CONFIG_DRAM_DDR3) , temporary

/*===========================================================================

                           DDR3 Attributes Type

===========================================================================*/

/*---------------------------------------------------------------------------
 Clock Speed Type
---------------------------------------------------------------------------*/

#define DDR3_800	(800)
#define DDR3_1066	(1066)
#define DDR3_1333	(1333)
#define DDR3_1600	(1600)
#define DDR3_1866	(1866)
#define DDR3_2133	(2133)

/*---------------------------------------------------------------------------
 Additive Latency Type
---------------------------------------------------------------------------*/
enum
{
	AL_DISABLED = 0,
	AL_CL_MINUS_ONE,
	AL_CL_MINUS_TWO
};

/*---------------------------------------------------------------------------
 Burst Length Type
---------------------------------------------------------------------------*/
enum
{
	BL_8 = 0,
	BL_BC4_OR_8,//Not supported by DDR2/DDR3 Controller
	BL_BC4,
};

/*---------------------------------------------------------------------------
 Read Burst Type
---------------------------------------------------------------------------*/
enum
{
	RBT_SEQUENTIAL = 0,
	RBT_INTERLEAVE,
};

/*---------------------------------------------------------------------------
 Write recovery for autoprecharge in MR0
---------------------------------------------------------------------------*/
enum{
	WR_5 = 1,
	WR_6,
	WR_7,
	WR_8,
	WR_10,
	WR_12
};

/*---------------------------------------------------------------------------
 DLL Control for Precharge PD
---------------------------------------------------------------------------*/
enum
{
	SLOW_EXIT = 0,
	FAST_EXIT
};

/*---------------------------------------------------------------------------
 Memory Controller Output Driver Strength
---------------------------------------------------------------------------*/
enum
{
	MEMCTRL_OUTPUT_DRIVER_60_Ohm = 0x8,
	MEMCTRL_OUTPUT_DRIVER_40_Ohm = 0xB,
	MEMCTRL_OUTPUT_DRIVER_34_Ohm = 0xE,
	MEMCTRL_OUTPUT_DRIVER_30_Ohm = 0xF
};

/*---------------------------------------------------------------------------
 Memory Controller ODT(On-die-termination)
---------------------------------------------------------------------------*/
enum
{
	MEMCTRL_ODT_120_Ohm = 0x1,
	MEMCTRL_ODT_60_Ohm  = 0x4, 
	MEMCTRL_ODT_40_Ohm  = 0x6
};

/*---------------------------------------------------------------------------
 DDR3 Output Driver Strength , RZQ=240 Ohm
---------------------------------------------------------------------------*/
enum
{
	DDR3_DIC_RZQ_DIV_6 = 0, //40 Ohm
	DDR3_DIC_RZQ_DIV_7 = 1  //34 Ohm
};

/*---------------------------------------------------------------------------
 DDR3 ODT(On-die-termination) , RZQ=240 Ohm
---------------------------------------------------------------------------*/
enum
{
	DDR3_RTT_DISABLE    = 0,
	DDR3_RTT_RZQ_DIV_4  = 1,    //60 Ohm
	DDR3_RTT_RZQ_DIV_2  = 2, //120 Ohm
	DDR3_RTT_RZQ_DIV_6  = 3, //40 Ohm
	DDR3_RTT_RZQ_DIV_12 = 4, // 20 Ohm
	DDR3_RTT_RZQ_DIV_8  = 5  // 30 Ohm
};

/*---------------------------------------------------------------------------
 DDR3 Driving Strength
---------------------------------------------------------------------------*/
#define MEMCTRL_DDS     MEMCTRL_OUTPUT_DRIVER_30_Ohm
#define MEMCTRL_TERM    MEMCTRL_ODT_60_Ohm
#ifdef CONFIG_CHIP_TCC8960
#define DIC        		DDR3_DIC_RZQ_DIV_7
#define RTT_NOM        DDR3_RTT_RZQ_DIV_6
#else
#define DIC        		DDR3_DIC_RZQ_DIV_7
#define RTT_NOM        DDR3_RTT_RZQ_DIV_6
#endif

#define SLV_DLY_WIDTH 				6
#define HALF_RATE_MODE 			1
#define HALF_RATE_SUPPORT 			1
#define SWAP_PHASE 					0
#ifdef CONFIG_DRAM_16BIT_USED
# define DRAM_MEM_WIDTH                 2
#else
# define DRAM_MEM_WIDTH                 0
#endif
#define MEM_STRB_WIDTH 			4
#define WIDTH_REDUCTION 			(DRAM_MEM_WIDTH==2?2:(DRAM_MEM_WIDTH==1?4:1))
#define SCL_LANES 					((1<<(MEM_STRB_WIDTH/WIDTH_REDUCTION))-1)
#define MEM_CLOCKS 					2
#define MIN_CMDACPT_WIDTH 		8
#define NUM_AGENTS 					2
#define ADDRESS_MAPPING_TYPE 		0
#define ECC_DLY 						0
#define C2D 							2
#define MAX_LATENCY 				4
#define NATIVE_BURST8 				((HALF_RATE_MODE)?0:1)
#define BL 							(NATIVE_BURST8?4:2)
#define AUTO_ZQC_ENABLE 			0
#define ZQC_SHORT 					0
#define ZQC_INTERVAL 				2
#define MEM_CHIP_SELECTS 			2
#define BIG_ENDIAN 					0
#define CMD_MRS                	 		(0xFFF001)
#define CMD_ZQC                 			(0xFFF601)
#define CKE_LOW                 			(0xFFEFFE)
#define IDLE_NOP                			(0xFFFFFF)
#define PREAMBLE_DLY				2
#define EXTRA_ONE_CLK				0
#define RTT_WR						1
#define SCL_WAIT_LATENCY			5
#define ANALOG_DLL_FOR_SCL			0
#define MAIN_CLK_DLY				7
#define CAPTURE_CLK_DLY			6	
#define BITLVL_DLY_WIDTH			6
#define CTR_START_VAL				10
#define MASTER_DLL					(CTR_START_VAL<<28)|0x1000
#define DLLS_TRIM_ADRCTRL_MA_UPD 	1
#define DLLS_TRIM_ADRCTRL_MA		-4
#define DLLS_TRIM_ADRCTRL_UPD 		1
#define DLLS_TRIM_ADRCTRL			5
#define DLLS_TRIM					10
#define DLLS_TRIM_UPD				1
#define TRIM1						-5
#define TRIM3						-9
#define IP_DQS_TRIM_UPD			0
#define IP_DQ_TRIM_UPD				0
#define OP_DQS_TRIM_UPD			0
#define OP_DQ_TRIM_UPD				0
#define IP_DQS_BITWISE_TRIM		-31
#define OP_DQS_BITWISE_TRIM		-31
#define OP_DM_BITWISE_TRIM		0
#define IP_DQ_BITWISE_TRIM			0
#define OP_DQ_BITWISE_TRIM			0

#if defined(CONFIG_DDR3_DUAL_CHANNEL)
	#if defined(CONFIG_DDR3_INTLV_USE)
	#define INTLV_SIZE			0x28000000 // 128MB ~ TOTAL DRAM SIZE/2
	#define INTLV_BASE			(0x20000000 + ((TCC_MEM_SIZE*0x100000)/2)) - INTLV_SIZE
	#define INTLV_MODE			0	// page size - 0 : 4kb , 1 : 8kb , 2 : 16kb, 3 : 32kb
	#endif
#endif

#if defined(CONFIG_DDR3_MODULE_1024MB)
/*---------------------------------------------------------------------------
 DDR3 Configuation
---------------------------------------------------------------------------*/
#if defined(CONFIG_DDR3_IF_32) // IF x32
// not yet
#elif defined(CONFIG_DDR3_IF_16) // IF x16
#define DDR3_ROWBITS			16
#define DDR3_COLBITS			10
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#elif defined(CONFIG_DDR3_IF_8) // IF x8
#define DDR3_ROWBITS			16
#define DDR3_COLBITS			11
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#elif defined(CONFIG_DDR3_IF_4) // IF x4
#define DDR3_ROWBITS			16
#define DDR3_COLBITS			12
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#else
	#error "not selected ddr IF(x32,x16,x8,x4).."
#endif
/*---------------------------------------------------------------------------
 DDR3 Access Timing Parameters
---------------------------------------------------------------------------*/
#define DDR3_tRFC_ps			350000
#define DDR3_tREFI_ps			7800000
#elif defined(CONFIG_DDR3_MODULE_512MB)
/*---------------------------------------------------------------------------
 DDR3 Configuation
---------------------------------------------------------------------------*/
#if defined(CONFIG_DDR3_IF_32) // IF x32
// not yet
#elif defined(CONFIG_DDR3_IF_16) // IF x16
#define DDR3_ROWBITS			15
#define DDR3_COLBITS			10
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#elif defined(CONFIG_DDR3_IF_8) // IF x8
#define DDR3_ROWBITS			16
#define DDR3_COLBITS			10
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#elif defined(CONFIG_DDR3_IF_4) // IF x4
#define DDR3_ROWBITS			16
#define DDR3_COLBITS			11
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#else
	#error "not selected ddr IF(x32,x16,x8,x4).."
#endif

/*---------------------------------------------------------------------------
 DDR3 Access Timing Parameters
---------------------------------------------------------------------------*/
#define DDR3_tRFC_ps			260000
#define DDR3_tREFI_ps			7800000
#elif defined(CONFIG_DDR3_MODULE_256MB)
/*---------------------------------------------------------------------------
 DDR3 Configuation
---------------------------------------------------------------------------*/
#if defined(CONFIG_DDR3_IF_32) // IF x32
// not yet
#elif defined(CONFIG_DDR3_IF_16) // IF x16
#define DDR3_ROWBITS			14
#define DDR3_COLBITS			10
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#elif defined(CONFIG_DDR3_IF_8) // IF x8
#define DDR3_ROWBITS			15
#define DDR3_COLBITS			10
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#elif defined(CONFIG_DDR3_IF_4) // IF x4
#define DDR3_ROWBITS			15
#define DDR3_COLBITS			11
#define DDR3_BANKBITS			3
#define DDR3_APBIT				10
#else
	#error "not selected ddr IF(x32,x16,x8,x4).."
#endif

/*---------------------------------------------------------------------------
 DDR3 Access Timing Parameters
---------------------------------------------------------------------------*/
#define DDR3_tRFC_ps			160000
#define DDR3_tREFI_ps			7800000
#else
	#error "do not support 128MB dram module"
#endif

#if defined(CONFIG_DDR3_2133)
#define DDR3_tRAS_ps			33000
#define DDR3_tRC_ps				46130
#define DDR3_tRCD_ps			13090
#define DDR3_tRP_ps				13090
#define DDR3_tXP_ps				6000
#define DDR3_tCKE_ps			5000
#elif defined(CONFIG_DDR3_1866)
#define DDR3_tRAS_ps			34000
#define DDR3_tRC_ps				47910
#define DDR3_tRCD_ps			13910
#define DDR3_tRP_ps				13910
#define DDR3_tXP_ps				6000
#define DDR3_tCKE_ps			5000
#elif defined(CONFIG_DDR3_1600)
#define DDR3_tRAS_ps			35000
#define DDR3_tRC_ps				48750
#define DDR3_tRCD_ps			13750
#define DDR3_tRP_ps				13750
#define DDR3_tXP_ps				6000
#define DDR3_tCKE_ps			5000
#elif defined(CONFIG_DDR3_1333H)
#define DDR3_tRAS_ps			36000
#define DDR3_tRC_ps				49500
#define DDR3_tRCD_ps			13500
#define DDR3_tRP_ps				13500
#define DDR3_tXP_ps				6000
#define DDR3_tCKE_ps			5625
#elif defined(CONFIG_DDR3_1333J)
#define DDR3_tRAS_ps			36000
#define DDR3_tRC_ps				51000
#define DDR3_tRCD_ps			15000
#define DDR3_tRP_ps				15000
#define DDR3_tXP_ps				6000
#define DDR3_tCKE_ps			5625
#elif defined(CONFIG_DDR3_1066F)
#define DDR3_tRAS_ps			37500
#define DDR3_tRC_ps				50625
#define DDR3_tRCD_ps			13125
#define DDR3_tRP_ps				13125
#define DDR3_tXP_ps				7500
#define DDR3_tCKE_ps			5625
#elif defined(CONFIG_DDR3_1066G)
#define DDR3_tRAS_ps			37500
#define DDR3_tRC_ps				52500
#define DDR3_tRCD_ps			15000
#define DDR3_tRP_ps				15000
#define DDR3_tXP_ps				7500
#define DDR3_tCKE_ps			5625
#elif defined(CONFIG_DDR3_800D)
#define DDR3_tRAS_ps			37500
#define DDR3_tRC_ps				50000
#define DDR3_tRCD_ps			12500
#define DDR3_tRP_ps				12500
#define DDR3_tXP_ps				7500
#define DDR3_tCKE_ps			7500
#elif defined(CONFIG_DDR3_800E)
#define DDR3_tRAS_ps			37500
#define DDR3_tRC_ps				52500
#define DDR3_tRCD_ps			15000
#define DDR3_tRP_ps				15000
#define DDR3_tXP_ps				7500
#define DDR3_tCKE_ps			7500
#endif

#if defined(CONFIG_DDR3_IF_16) // IF x16 // 2KB page size
	#if defined(CONFIG_DDR3_2133)
	#define DDR3_tRRD_ps                  6000	
	#define DDR3_tFAW_ps                 35000	
	#elif defined(CONFIG_DDR3_1866)
	#define DDR3_tRRD_ps                  6000	
	#define DDR3_tFAW_ps                 35000	
	#elif defined(CONFIG_DDR3_1600)
	#define DDR3_tRRD_ps                  7500	
	#define DDR3_tFAW_ps                 40000	
	#elif defined(CONFIG_DDR3_1333H)
	#define DDR3_tRRD_ps                  7500	
	#define DDR3_tFAW_ps                 45000	
	#elif defined(CONFIG_DDR3_1333J)
	#define DDR3_tRRD_ps                  7500	
	#define DDR3_tFAW_ps                 45000	
	#else
	#define DDR3_tRRD_ps                  10000	
	#define DDR3_tFAW_ps                 50000	
	#endif
#else // IF x8,x4 // 1KB page size
	#if defined(CONFIG_DDR3_2133) 
	#define DDR3_tRRD_ps                  5000	
	#define DDR3_tFAW_ps                 25000	
	#elif defined(CONFIG_DDR3_1866)
	#define DDR3_tRRD_ps                  5000	
	#define DDR3_tFAW_ps                 25000	
	#elif defined(CONFIG_DDR3_1600)
	#define DDR3_tRRD_ps                  6000	
	#define DDR3_tFAW_ps                 30000	
	#elif defined(CONFIG_DDR3_1333H)
	#define DDR3_tRRD_ps                  6000	
	#define DDR3_tFAW_ps                 30000	
	#elif defined(CONFIG_DDR3_1333J)
	#define DDR3_tRRD_ps                  6000	
	#define DDR3_tFAW_ps                 30000	
	#elif defined(CONFIG_DDR3_1066F)	
	#define DDR3_tRRD_ps                  7500	
	#define DDR3_tFAW_ps                 37500	
	#elif defined(CONFIG_DDR3_1066G)	
	#define DDR3_tRRD_ps                  7500	
	#define DDR3_tFAW_ps                 37500	
	#else
	#define DDR3_tRRD_ps                  10000
	#define DDR3_tFAW_ps                 40000	
	#endif
#endif

// common 

#define DDR3_tRCD_ck                     1
#define DDR3_tRP_ck                      1
#define DDR3_tRC_ck                      1
#define DDR3_tRAS_ck                     1
#define DDR3_tRTP_ps                  7500
#define DDR3_tRTP_ck                     4
#define DDR3_tWTR_ps                  7500
#define DDR3_tWTR_ck                     4
#define DDR3_tWR_ps                  15000
#define DDR3_tWR_ck                      1
#define DDR3_tRRD_ck                     4
#define DDR3_tFAW_ck                     1
#define DDR3_tXS_ps    (DDR3_tRFC_ps+10000)
#define DDR3_tXS_ck                      5
#define DDR3_tXP_ck                      3
#define DDR3_tCKE_ck                     3
#define DDR3_tMRD_ck                     4
#define DDR3_tCCD_ck                     4
#define DDR3_tRAS_MAX_ps  (DDR3_tREFI_ps*9)
#define DDR3_tMOD_ps                 15000
#define DDR3_tMOD_ck                    12
#define DDR3_tDLLK_ck                  512
#define DDR3_tXPDLL_ps               24000
#define DDR3_tXPDLL_ck                  10
#define DDR3_tXSDLL_ck       DDR3_tDLLK_ck
#define DDR3_tCKSRX_ps               10000
#define DDR3_tCKSRX_ck                   5
#define DDR3_tCKSRE_ps               10000
#define DDR3_tCKSRE_ck                   5
#define DDR3_tZQCS_ck                   64
#define DDR3_tZQINIT_ck                512
#define DDR3_tZQOPER_ck                256
#define DDR3_tCKESR_ck			4
#define DDR3_tXPR_ps			270000
#define DDR3_tXPR_ck			5


#endif /* CONFIG_DRAM_DDR3 */

#endif	/* __TCC_DDR__H__ */
