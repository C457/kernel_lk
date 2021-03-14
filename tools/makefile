#Makefile to generate lk.rom

TOOLS_DIR  := target/$(TARGET)/tools
KEYS_DIR := ../../../vendor/telechips/tools/tcsb/sign_key
SIGNTOOLS_DIR := ../../../vendor/telechips/tools/tcsb
TZOS := ../../../vendor/telechips/proprietary/$(PLATFORM)/tztee/tzos/tzos.rom

# For Secure Boot Start
SIGNTOOL :=$(SIGNTOOLS_DIR)/tcsb_tool_v2
KEYPAIR :=$(KEYS_DIR)/tcsb_keypair.der
WRAPKEY:=$(KEYS_DIR)/tcsb_sbcr.bin
ROLLBACK_ID_FILE := $(SECURE_DIR)/sign_key/rollbackid
# For Secure Boot End
DRAMINIT := $(BUILDDIR)/dram.rom

ifeq ($(TSBM_INCLUDE),1)
ifneq ($(wildcard $(KEYPAIR)),)
BOOTLOADER_FILE := tcsb-lk.rom
else
BOOTLOADER_FILE := $(BUILDDIR)/lk.rom
endif
else
BOOTLOADER_FILE := $(BUILDDIR)/lk.rom
endif

ifneq ($(wildcard $(ROLLBACK_ID_FILE)),)
ROLLBACK_ID=$(shell cat $(ROLLBACK_ID_FILE) | \grep BOOTLOADER | \sed -e "s,BOOTLOADER=,"",g" | tr -d "\r")
ifneq ($(ROLLBACK_ID), )
ifneq ($(ROLLBACK_ID), 0)
RID_CMD=-r $(ROLLBACK_ID)
endif
endif
endif

APPSBOOTHEADER: $(BOOTLOADER_FILE)

tcsb-lk.rom: $(OUTROM)
	$(NOECHO)echo signinging $@
	$(NOECHO)$(SIGNTOOL) -b $^ -o $(BUILDDIR)/$@ -s $(KEYPAIR) -e $(WRAPKEY) -c $(BUILDDIR)/tcsb-cfg.rom $(RID_CMD)

$(OUTROM): $(OUTBIN) $(BUILDDIR)/tcc_crc $(BUILDDIR)/tcsb_mkblimg
ifeq ($(TRUSTZONE_ENABLE), 1)
	$(NOECHO)cat $(TZOS) >> $(OUTBIN)
	$(NOECHO)$(BUILDDIR)/tcsb_mkblimg tzmend $(OUTBIN) $(TZOS)
endif
#	$(BUILDDIR)/lk.rom: $(OUTBIN) $(BUILDDIR)/tcc_crc $(BUILDDIR)/mkimg
	$(NOECHO)echo generating $@
#	$(NOECHO)$(BUILDDIR)/mkimg $(OUTBIN) $(DRAMINIT) $(BUILDDIR)/emmc_header.rom
	$(NOECHO)$(BUILDDIR)/tcc_crc -v __BL -o $@ $(OUTBIN)

