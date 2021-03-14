#Makefile to generate mkseimg

SECURE_DIR  := platform/tcc_shared/tcsb
COMPILER := gcc

SECURE_SRC     := $(SECURE_DIR)/tcsb_mkblimg.c

ifeq ($(EMMC_BOOT),1)
DEFINE_SE += -DEMMC_USED
else
DEFINE_SE += -DNAND_USED
ifeq ($(TNFTL_V8_USE),1)
DEFINE_SE += -DTNFTL_V8_INCLUDE
endif
endif

ifeq ($(TCC893X),1)
DEFINE_SE += -DTCC893X
endif
ifeq ($(TCC896X),1)
DEFINE_SE += -DTCC896X
endif
ifeq ($(TCC897X),1)
DEFINE_SE += -DTCC897X
endif

ifeq ($(TCC_MEM_SIZE),512)
DEFINE_SE += -DDRAM512MB
endif
ifeq ($(TCC_MEM_SIZE),1024)
DEFINE_SE += -DDRAM1GB
endif
ifeq ($(TCC_MEM_SIZE),2048)
DEFINE_SE += -DDRAM2GB
endif
ifeq ($(TCC_MEM_SIZE),3072)
DEFINE_SE += -DDRAM3GB
endif

ifeq ($(TRUSTZONE_ENABLE),1)
DEFINE_SE += -DCONFIG_ARM_TRUSTZONE
endif

$(BUILDDIR)/tcsb_mkblimg: $(SECURE_SRC)
	$(COMPILER) -Iplatform/$(PLATFORM)/include -o $(BUILDDIR)/tcsb_mkblimg $(SECURE_SRC) $(DEFINE_SE)
