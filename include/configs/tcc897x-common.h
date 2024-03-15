/*
 * (C) Copyright 2015 Telechips
 * Telechips <linux@telechips.com>
 *
 * Configuation settings for the TCC897X EVM board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/***********************************************************
 * Select Boot Device
 ***********************************************************/
#if defined(CONFIG_TCC_BOOT_EMMC)
#define USE_EMMC_BOOT		1
#elif defined(CONFIG_TCC_BOOT_NAND)
#define USE_NAND_BOOT		1
#elif defined(CONFIG_TCC_BOOT_SNOR)
#define USE_SNOR_BOOT		1
#endif

#define CONFIG_USB_UPDATE

#define KERNEL_DEBUG		/* for kernel debugging (remove "quiet" option in the cmdline) */

#ifdef KERNEL_DEBUG
#define CMDLINE_QUIET
#else
#define CMDLINE_QUIET				" quiet "
#endif

#define CONFIG_OF_LIBFDT
#define CONFIG_ANDROID_BOOT_IMAGE
#define CONFIG_SYS_BOOTM_LEN (60<<20)

#if defined(CONFIG_USE_IRQ)
#define CONFIG_STACKSIZE_IRQ   		(4*1024)        /* IRQ stack */
#define CONFIG_STACKSIZE_FIQ   		(4*1024)        /* FIQ stack */
#endif

#define CONFIG_SYS_CACHELINE_SIZE       64

#ifdef CONFIG_USB_UPDATE
/*
 * System recovery for usb update Configuration
 */
#define UPDATE_FILE_NAME				"update.fai"		/* Just support the only "SD Data.fai" type file*/
#define UPDATE_BUFFER_ADDR				"F0000000"
#define UPDATE_BUFFER_SIZE				(16*1024*1024)
#define UPDATE_BOOTLOADER_NAME          "lk.rom"
#define UPDATE_BOOTLOADER_DEFAULT_SIZE  (10*1024*1024)
#if defined(CONFIG_FBCON)
#define UPDATE_DEBUG_LOG_FBCON
#endif /* CONFIG_FBCON */
#endif

#define CONFIG_BOOTLOADER_PARTITION_NAME       "bootloader"

/***********************************************************
 * FB CONSOLE
 ***********************************************************/
#if defined(CONFIG_FBCON)
/*
 * from lk boot log
 * lcdc_set_logo fb_cfg base:0xa7a00000 xres:1920 yres:720 bpp:32
 * fb_cfg base:0xa7c00000 xres:1280 yres:720 bpp:32
 */
#define SZ_1MB				(1024*1024)
#define ARRAY_MBYTE(x)            	((((x) + (SZ_1MB-1))>> 20) << 20)
#define LK_MEM_BASE			0xA8000000

#if defined(CONFIG_USE_LCD_1920x720)
#define DISP_WIDTH	1920
#define DISP_HEIGHT	720
#else
#define DISP_WIDTH	1280
#define DISP_HEIGHT	720
#endif

#define FB_BPP		32
#define FB_BASE_ADDR	(LK_MEM_BASE - ARRAY_MBYTE(DISP_WIDTH*DISP_HEIGHT*2*2))
#endif /* CONFIG_FBCON */

/***********************************************************
 * Telechips Chip Specific Configurations
 ***********************************************************/
#define CONFIG_TELECHIPS			1	/* in a Telechips Core */
#define CONFIG_TCC					1	/* which is in a TCC Family */
#define CONFIG_TCC897X				1	/* which is in a TCC897X */
#define CONFIG_TCC897X_EVM			1	/* working with TCC897X */
#define CONFIG_ARCH_TCC897X
#if 0
#define CONFIG_TCC897X_REV_XX
#endif
/**********************************************************
 * Define board revision
 * CONFIG_UBOOT_TCC8970 : TCC8971_EVB_D3164_2CS_V0.1
 **********************************************************/
#define CONFIG_UBOOT_TCC8970

#if defined(CONFIG_UBOOT_TCC8970)
	#define CONFIG_CHIP_TCC8970		1
#elif defined(CONFIG_UBOOT_TCC8971)
	#define CONFIG_CHIP_TCC8971		1
#else
	#error "Not defined BOARD_REV"
#endif

/*
 * Kernel Root Device Type.
 * initramfs or block devices (e.g. USB and MMC devices).
 */
#define CONFIG_INITRAMFS

/*
 * QuickBoot Option
 */

#if defined(CONFIG_CMD_TCC_SNAPSHOT)
//#define CONFIG_TCC_CLEANUP
#endif

/*
 * Check Booting Time
 */

#ifdef CONFIG_BOOTTIME
#define CONFIG_BOOTSTAGE
#define CONFIG_BOOTSTAGE_REPORT
#define CONFIG_BOOTSTAGE_INIT_F
#define CONFIG_BOOTSTAGE_INIT_R
#define KERNEL_EXT_CMDLINE CMDLINE_QUIET
#else
#define KERNEL_EXT_CMDLINE CMDLINE_QUIET
#endif


/*
 * Telechips Debug UART Configurations
 */
#define CONFIG_TCC_SERIAL
#define CONFIG_TCC_SERIAL_CLK		48000000
#define CONFIG_BAUDRATE				115200

#define CONFIG_SYS_BAUDRATE_TABLE   \
    {9600, 14400, 19200, 38400, 57600, 115200, 230400}

#define CONFIG_DEBUG_UART_CH		0

#define CONFIG_TCC_SERIAL0_PORT 	22
 #define CONFIG_TCC_SERIAL0_BAUDRATE CONFIG_BAUDRATE

/*
 * Telechips Drivers for U-Boot
 */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_SPEED        100000
#define CONFIG_SYS_I2C_TCC
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_INIT_BOARD

/*
 * Network Driver
 */
#ifndef CONFIG_USB_UPDATE
#define CONFIG_GMAC_BASE			0x71600000
#define CONFIG_MAC_ADDR				{0x00, 0x12, 0x34, 0x56, 0x78, 0x01}
#define CONFIG_TCC_GMAC_RGMII_MODE

#define CONFIG_MII
#define CONFIG_DESIGNWARE_ETH
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_DW_SEARCH_PHY
#define CONFIG_DW0_PHY				1
#define CONFIG_PHY_RESET_DELAY		10000       /* in usec */
#define CONFIG_DW_AUTONEG
#define CONFIG_PHY_GIGE				/* Include GbE speed/duplex detection */
#endif
/*
 * Telechips Drivers for U-Boot
 */
#define CONFIG_TCC_GPIO /* GPIO Driver */
#define CONFIG_FWDN_V7
#define TCC_CHIP_REV				1
#define CONFIG_USB_DWC_OTG
#define FWDN_V7						1

#define HAVE_BLOCK_DEVICE


/*
 * Fastboot
 */
#if defined(CONFIG_USB_FUNCTION_FASTBOOT)
#define CONFIG_FASTBOOT_BUF_ADDR	CONFIG_SYS_LOAD_ADDR
#define CONFIG_FASTBOOT_BUF_SIZE 	0x2000000

#define CONFIG_FASTBOOT_FLASH

#if defined(USE_EMMC_BOOT)
#define CONFIG_FASTBOOT_FLASH_MMC_DEV   0
#else
/*#define CONFIG_FASTBOOT_FLASH_NAND_DEV    0*/
#endif

#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_DWC2_OTG
#define CONFIG_USB_GADGET_DOWNLOAD

#define CONFIG_G_DNL_VENDOR_NUM		0x18D1
#define CONFIG_G_DNL_PRODUCT_NUM	0xD00D

#define CONFIG_G_DNL_UMS_PRODUCT_NUM
#define CONFIG_G_DNL_MANUFACTURER       "Telechips"
#define CONFIG_USB_GADGET_VBUS_DRAW	0

#endif

/*
 * SD/MMC
 */
#define CONFIG_TCC_SDMMC
#if defined(CONFIG_TCC_SDMMC)
#define CONFIG_EFI_PARTITION
#define KERNEL_RECOVERY_CMD			"update kernel"
#endif
#if defined(USE_EMMC_BOOT)
#define BOOTSD_INCLUDE
#define TODO_EMMC
#define _EMMC_BOOT
#define FEATURE_SDMMC_MMC43_BOOT	/* Support eMMC Boot (eMMC Boot Only, Not SD/MMC) */
#endif

#ifndef CONFIG_DAUDIO_KK
#define CONFIG_TCC_SDHCI3	/* boot slot GPIO_D22~31=SD3, GPIO_B14=CD */
#else
#define CONFIG_TCC_SDHCI2
#endif
#if defined(CONFIG_TCC_SDHCI2) || defined(CONFIG_TCC_SDHCI3)
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_SDHCI
#define CONFIG_TCC_SDHCI
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_SUPPORT_VFAT
#define CONFIG_CMD_EXT2
#define CONFIG_DOS_PARTITION
#define CONFIG_MMC_SDMA
#define CONFIG_PARTITION_TYPE_GUID
#endif

/*
 * USB Host Driver
 */
/*
#ifdef CONFIG_USB_DWC2
#define CONFIG_USB_STORAGE
#define CONFIG_DOS_PARTITION
#define CONFIG_FS_FAT
#define CONFIG_CMD_FAT
#define CONFIG_CMD_USB
#endif
*/
#define CONFIG_USB_DWC2_REG_ADDR		0x71B00000

/***********************************************************
 * Configurations for U-Boot
 ***********************************************************/
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/***********************************************************
 * Configurations for U-Boot
 ***********************************************************/
/*
 * Miscellaneous configurable options
 */
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_BOOTDELAY			(3)
#define CONFIG_SYS_LONGHELP			/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser	*/
#define CONFIG_SYS_PROMPT			"TCC # "
#define CONFIG_SYS_CBSIZE			256	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE			384	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS			16	/* max number of command args */
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

/*
 * Enable support for ARM PL310 L2 cache controller
 */
#ifndef CONFIG_SYS_L2CACHE_OFF
#endif


#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR 	0x300 /* address 0x60000 */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS      	0x200 /* 256 KB */
#define CONFIG_SYS_MMC_SD_FAT_BOOT_PARTITION    	1

/*
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH				1


/*
 * U-Boot Display configuration
 */

/***********************************************************
 * VIOC Manager definition
 ***********************************************************/
#if defined(CONFIG_TCC_VIOCMG_SUPPORT)
#define CONFIG_MAIN_DISPLAY_ID 			0
#define CONFIG_MAIN_DISPLAY_SWAP 		1
#else
#define CONFIG_MAIN_DISPLAY_ID 			1
#endif
#define CONFIG_MAIN_DISPLAY_OVP 		24

#define CONFIG_NO_LCD_CONSOLE
#define CONFIG_SPLASH_IN_EMMC
#if defined(CONFIG_SPLASH_IN_EMMC)
#define CONFIG_SPLAH_NAME   "bootlogo_565"
#else
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_BMP_32BPP
#endif
#define CONFIG_VIDEO_BMP_GZIP
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE  (2 << 20)


#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_PREBOOT		"setenv splashpos m,m"

/*
 * U-Boot Gpio Expender configuration
 */
#ifndef CONFIG_DAUDIO_KK
#define CONFIG_PCA950X
#define CONFIG_SYS_I2C_PCA953X_U3 		0x74
#define CONFIG_SYS_I2C_PCA953X_U5 		0x75
#define CONFIG_SYS_I2C_PCA953X_U2 		0x77
#define CONFIG_SYS_I2C_PCA953X_U11 		0x76
#define CONFIG_SYS_I2C_PCA953X_WIDTH	{ {CONFIG_SYS_I2C_PCA953X_U3, 16}, {CONFIG_SYS_I2C_PCA953X_U5, 16}, {CONFIG_SYS_I2C_PCA953X_U2, 16}, {CONFIG_SYS_I2C_PCA953X_U11, 16} }
#endif
/*
 * U-Boot Environment configuration
 */
#ifdef CONFIG_USB_UPDATE
#define CONFIG_ENV_IS_NOWHERE
#else
#define CONFIG_ENV_IS_IN_MMC	 		1
#endif
#define CONFIG_SYS_MMC_ENV_DEV 			0
#define CONFIG_ENV_SIZE					0x100000	/* 1MB */
#define CONFIG_ENV_OFFSET				0x4000
#define CONFIG_SYS_MALLOC_LEN			(CONFIG_ENV_SIZE + (4 << 20)) /* Size of malloc() pool */
#define CONFIG_ENV_OVERWRITE			/* allow to overwrite serial and ethaddr */


#if defined(CONFIG_NAND_TCC)
/* NAND support */
#define NAND_INCLUDE				1
#define TCC8970
#define TCC897X
#define NFC_VER_300
#define CONFIG_CMD_NAND
#define EDI_MODE_NAND				0
#define CONFIG_MTD_DEBUG
#define CONFIG_MTD_DEBUG_VERBOSE	3
#define CONFIG_SYS_NAND_MASK_ALE	(1 << 21)
#define CONFIG_SYS_NAND_MASK_CLE	(1 << 22)

#define CONFIG_SYS_MAX_NAND_DEVICE 	1
#define CONFIG_SYS_NAND_BASE		0x76600010

#define CONFIG_SYS_NAND_DBW_8
#define CONFIG_SYS_NAND_READY_PIN
#define CONFIG_SYS_NAND_USE_FLASH_BBT

#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#endif

#if defined(USE_SNOR_BOOT)
#define SNOR_BOOT_INCLUDE
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_TCC_SPI
#define CONFIG_SPI_FLASH_MACRONIX
#endif

/*
 * Enable ATAGs for Linux Kernel
 */
#ifdef CONFIG_USB_UPDATE
#define CONFIG_BOOTARGS "root=/dev/ram rw iuser_debug=255, console=ttyS0,115200n8"
#endif
#define CONFIG_PANEL_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG

#ifndef CONFIG_USB_UPDATE
#if defined(USE_EMMC_BOOT)
#ifdef CONFIG_INITRAMFS
#define KERNEL_ZIMAGE_SIZE	"0xD00000"
#define PARTITION_INFO		" partition.boot=/dev/mmcblk0p2" \
							" partition.system=/dev/mmcblk0p3" \
							" partition.kpanic=/dev/mmcblk0p7" \
							" partition.data=/dev/mmcblk0p8"
#define NORMAL_BOOTARGS 	"iuser_debug=255, console=ttyS0,115200n8" \
							PARTITION_INFO KERNEL_EXT_CMDLINE
#else
#define KERNEL_ZIMAGE_SIZE	"0x400000"
#define PARTITION_INFO		" partition.boot=/dev/mmcblk0p2" \
							" partition.system=/dev/mmcblk0p3" \
							" partition.kpanic=/dev/mmcblk0p7" \
							" partition.rootfs=/dev/mmcblk0p8" \
							" partition.data=/dev/mmcblk0p9"
#define NORMAL_BOOTARGS 	"root=/dev/mmcblk0p8 rootwait rw iuser_debug=255, console=ttyS0,115200n8" \
							" resume=/dev/mmcblk0p5" \
							PARTITION_INFO KERNEL_EXT_CMDLINE
#endif


#if defined(CONFIG_CMD_TCC_SNAPSHOT)
#define CONFIG_BOOTARGS 	NORMAL_BOOTARGS \
							" resume=/dev/mmcblk0p5 snapshot_boot"
#define SNAPSHOT_PARTITION	"mmc 0:5"
#else
#define CONFIG_BOOTARGS 	NORMAL_BOOTARGS
#endif


#elif defined(USE_NAND_BOOT)
#define CONFIG_BOOTARGS "root=/dev/ram rw iuser_debug=255, console=ttyS0,115200n8"
#endif
#endif
/*
 * U-Boot Default Boot Command after CONFIG_BOOTDELAY
 */
#ifdef CONFIG_USB_UPDATE
#define CONFIG_BOOTCOMMAND		"reset"
#else
#if defined(USE_EMMC_BOOT)
#define COMMAND_LOAD_BOOT_IMAGE		"tcc_load kernel mmc 0:boot 0x80008000; tcc_load dtb mmc 0:dtb 0x83000000; "

#define PARTS_DEFAULT \
        "uuid_disk=${uuid_gpt_disk};" \
        "name=env,size=1MiB,uuid=data;" \
		"name=boot,size=20MiB,uuid=data;" \
		"name=recovery,size=20MiB,uuid=data;" \
		"name=system,size=1024MiB,uuid=linux;" \
		"name=misc,size=2MiB,uuid=data;" \
		"name=snapshot,size=400MiB,uuid=data;" \
		"name=splash,size=40MiB,uuid=data;" \
		"name=home,size=512MiB,uuid=linux;" \
		"name=dtb,size=2MiB,uuid=data\0" \


#if defined(CONFIG_CMD_TCC_SNAPSHOT)
#define COMMAND_SNAPSHOT_BOOT_IMAGE	"dcache on; tcc_snapshot " SNAPSHOT_PARTITION ";"
#define CONFIG_BOOTCOMMAND			COMMAND_SNAPSHOT_BOOT_IMAGE\
									COMMAND_LOAD_BOOT_IMAGE\
									"bootz 0x80008000 - 0x83000000;"
#else	/* not CONFIG_CMD_TCC_SNAPSHOT */
#define CONFIG_BOOTCOMMAND			COMMAND_LOAD_BOOT_IMAGE \
									"bootz 0x80008000 - 0x83000000;"
#endif 	/* CONFIG_CMD_TCC_SNAPSHOT */

#define NORMAL_BOOTCMD				COMMAND_LOAD_BOOT_IMAGE \
									"bootz 0x80008000 - 0x83000000;"

#define CONFIG_EXTRA_ENV_SETTINGS	"fdt_high=0xffffffff\0" \
									"normal_bootargs=" NORMAL_BOOTARGS "\0" \
									"normal_bootcmd=" NORMAL_BOOTCMD "\0" \
									"normalboot=setenv bootargs $normal_bootargs; run normal_bootcmd;\0" \
									"mk_snapshot=run normal_bootcmd;\0" \
									"partitions="PARTS_DEFAULT

#elif defined(USE_NAND_BOOT)
#ifdef CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT				"nand0=tcc-nand"

#define MTDPARTS_DEFAULT			"mtdparts=tcc-nand:2M(Bootloader),2M(Bootloader_backup)," \
									"20M(kernel),20M(kernel_backup)," \
									"450M(Ubifs_system)," \
									"1M(misc),1M(cache),-(user_data)"

#define COMMAND_LOAD_BOOT_IMAGE			"mtdparts; nboot 84000000 kernel;"
#define CREATE_UBI_VOLUME			"ubi part Ubifs_system; ubi create app_system;" \
						"ubi part user_data; ubi create user_data;"

#define	CONFIG_EXTRA_ENV_SETTINGS 		"mtdparts="MTDPARTS_DEFAULT "\0" \
						"bootcmd_nand="COMMAND_LOAD_BOOT_IMAGE "bootm\0" \
						"mk_ubi_vol="CREATE_UBI_VOLUME

/*
	"bootargs_nand=rootfstype=ubifs ubi.mtd=3 root=ubi0:rootfs rw\0" \
*/
#endif	/* CONFIG_CMD_MTDPARTS */

#define CONFIG_BOOTCOMMAND		COMMAND_LOAD_BOOT_IMAGE \
								"bootm;"

#endif	/* USE_EMMC_BOOT */
#endif	/* CONFIG_USB_UPDATE */

/***********************************************************
 * Early_camera definition
 ***********************************************************/
#define CONFIG_TCC_TVDEC_TVP5150
#if defined(CONFIG_TCC_PARKING_GUIDE_LINE)
#define CONFIG_TCC_PARKING_GUIDE_LINE_NAME "parkingline_888"
#endif


/***********************************************************
 * Command definition
 ***********************************************************/
#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_NFS

#define CONFIG_CMD_PING
#define CONFIG_CMD_MII

#define CONFIG_CMD_CACHE
#ifndef CONFIG_USB_UPDATE
#define CONFIG_CMD_TCC_LOAD
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_GPT
#define CONFIG_CMD_PART
#define CONFIG_RANDOM_UUID
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_BOOTI
#define CONFIG_CMD_FASTBOOT
#endif

/***********************************************************
 * USE Audio PLL  definition
 ***********************************************************/
#define CONFIG_AUDIO_PLL_USE

/***********************************************************
 * CPU macro definition
 ***********************************************************/
#ifdef CONFIG_CHIP_TCC8930
#define cpu_is_tcc8930()		(1)
#else
#define cpu_is_tcc8930()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8960
#define cpu_is_tcc8960()		(1)
#else
#define cpu_is_tcc8960()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8963
#define cpu_is_tcc8963()		(1)
#else
#define cpu_is_tcc8963()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8933
#define cpu_is_tcc8933()		(1)
#else
#define cpu_is_tcc8933()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8935
#define cpu_is_tcc8935()		(1)
#else
#define cpu_is_tcc8935()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8933S
#define cpu_is_tcc8933s()		(1)
#else
#define cpu_is_tcc8933s()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8935S
#define cpu_is_tcc8935s()		(1)
#else
#define cpu_is_tcc8935s()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8937S
#define cpu_is_tcc8937s()		(1)
#else
#define cpu_is_tcc8937s()		(0)
#endif

#ifdef CONFIG_CHIP_TCC8970
#define cpu_is_tcc8970()		(1)
#else
#define cpu_is_tcc8970()		(0)
#endif

#endif	/* __CONFIG_H */

