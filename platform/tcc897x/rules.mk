LOCAL_DIR := $(GET_LOCAL_DIR)

ARCH    := arm
ARM_CPU := cortex-a7
CPU     := generic

DEFINES += ARM_CPU_CORE_A7
DEFINES += TCC897X=1

DEFINES += \
	_LINUX_ \
	NAND_BOOT_REV \
	FWDN_V7

MMC_SLOT         := 1

################################################
## SDMMC BOOT
################################################
ifeq ($(EMMC_BOOT),1)
DEFINES += FEATURE_SDMMC_MMC43_BOOT
endif
################################################

DEFINES += PERIPH_BLK_BLSP=1
#DEFINES += WITH_CPU_EARLY_INIT=0 WITH_CPU_WARM_BOOT=0 \
#	   MMC_SLOT=$(MMC_SLOT) SSD_ENABLE
DEFINES += WITH_CPU_EARLY_INIT=0 WITH_CPU_WARM_BOOT=0 \
	   MMC_SLOT=$(MMC_SLOT)
DEFINES += TZ_SAVE_KERNEL_HASH

INCLUDES += -I$(LOCAL_DIR)/include -I$(LK_TOP_DIR)/platform/tcc_shared/include

################################################
## KEYBOX INCLUDE
################################################
ifeq ($(HIDDEN_ENABLE), 1)
DEFINES += SECURE_KEYBOX_INCLUDE
endif

ifeq ($(GPHD_ENABLE), 1)
DEFINES += GPH_AREA_INCLUDE
endif

################################################
## SECURE BOOT
################################################
ifeq ($(TSBM_INCLUDE),1)
LIBS += $(LOCAL_DIR)/libs/atsb_lib.a
OBJS += $(LOCAL_DIR)/tcc_security.o
endif


LIBS += $(LOCAL_DIR)/libs/libjpu.a

DEVS += fbcon
MODULES += dev/fbcon
MODULES += dev/keys
		
OBJS += \
	$(LOCAL_DIR)/tcc_ddr.Ao \
	$(LOCAL_DIR)/tcc_ckc.o \
	$(LOCAL_DIR)/gpio.o \
	$(LOCAL_DIR)/uart.o \
	$(LOCAL_DIR)/i2c.o \
	$(LOCAL_DIR)/interrupts.o

OBJS += \
	$(LOCAL_DIR)/lcdc.o \
	$(LOCAL_DIR)/tcc_lcd_interface.o \
	$(LOCAL_DIR)/vioc/vioc_rdma.o \
	$(LOCAL_DIR)/vioc/vioc_vin.o \
	$(LOCAL_DIR)/vioc/vioc_viqe.o \
	$(LOCAL_DIR)/vioc/vioc_wdma.o \
	$(LOCAL_DIR)/vioc/vioc_wmix.o \
	$(LOCAL_DIR)/vioc/vioc_outcfg.o \
	$(LOCAL_DIR)/vioc/vioc_disp.o \
	$(LOCAL_DIR)/vioc/vioc_scaler.o \
	$(LOCAL_DIR)/vioc/vioc_config.o

OBJS += \
	$(LOCAL_DIR)/platform.o\
	$(LOCAL_DIR)/reboot.o

ifeq ($(TCC_HDMI_USE), 1)
  OBJS += \
	$(LOCAL_DIR)/hdmi_v1_4.o
endif

OBJS += \
	$(LOCAL_DIR)/sram_asm.Ao \
	$(LOCAL_DIR)/mon_vector.Ao \
	$(LOCAL_DIR)/Init_SoCs.o

LINKER_SCRIPT += $(BUILDDIR)/system-onesegment.ld

#LIBS += \
#		$(LOCAL_DIR)/libs/libtnftl_v8_TCC897X_BL.lo


## TRUST ZONE ENABLE ##
ifeq ($(TRUSTZONE_ENABLE), 1)
DEFINES += CONFIG_ARM_TRUSTZONE=1
else
ifeq ($(TZOW_ENABLE), 1)
#DEFINES += OTP_UID_INCLUDE
DEFINES += TZOW_INCLUDE
#LIBS += ../../../vendor/telechips/proprietary/$(PLATFORM)/tztee/lib/tzow_lib.a
endif
ifeq ($(CMOTP_ENABLE), 1)
DEFINES += TZOW_INCLUDE
endif
endif

include platform/tcc_shared/rules.mk
