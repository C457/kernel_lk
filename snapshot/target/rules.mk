# //+[TCCQB] QuickBoot Option

LOCAL_DIR := $(GET_LOCAL_DIR)
#INCLUDES += -I$(LOCAL_DIR)/include -I$(LK_TOP_DIR)/platform/tcc_shared

OBJS += $(LOCAL_DIR)/init.o

#==================================================================
# Snapshot Boot Option
#==================================================================
TCC_QUICKBOOT_USE := 1


ifeq ($(TCC_QUICKBOOT_USE),1)
	DEFINES += CONFIG_SNAPSHOT_BOOT
	DEFINES += CONFIG_SNAPSHOT_CHECKSUM
	DEFINES += CONFIG_QB_WATCHDOG_ENABLE

TCC_SNAPSHOT_USE_LZ4 := 1

CONFIG_WATCHDOG_TIMEOUT := 10                # sec
DEFINES += CONFIG_WATCHDOG_TIMEOUT=$(CONFIG_WATCHDOG_TIMEOUT)
endif
# //-[TCCQB]
# //

