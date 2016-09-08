# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2016
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG


#Pull in the definitions that affect all makefiles for this image
include img_defs.mk

include $(BOOT_SRCDIR)/bootfiles.mk
include $(BUILDDATA_SRCDIR)/utils.mk

SUBDIRS := $(PK_SRCDIR)/$(PPE_TYPE)
LIB_DIRS += -L$(OBJDIR)/pk
PKLIB := $(OBJDIR)/pk/libpk.a
LLIBS += -lpk

SUBDIRS += $(BOOT_SRCDIR)
SUBDIRS += $(BUILDDATA_SRCDIR)

SUBDIRS += ../sbefw
LIB_DIRS += -L$(OBJDIR)/sbefw
SBEFWLIB := $(OBJDIR)/sbefw/libsbefw.a
LLIBS += -lsbefw

# FAPI2 library
SUBDIRS += $(PLAT_FAPI2_DIR)/src
LIB_DIRS += -L$(OBJDIR)/fapi2
FAPI2LIB := $(OBJDIR)/fapi2/libfapi2.a
LLIBS += -lfapi2

# HWP LIB library
SUBDIRS += $(HWPLIB_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/lib
HWPLIB := $(OBJDIR)/libcommon.a
LLIBS += -lcommon
include $(HWPERR_SRCDIR)/hwpErrors.mk

# Common Utils library
SUBDIRS += $(UTILS_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/utils
UTILSLIB := $(OBJDIR)/utils/libutils.a
LLIBS += -lutils

# Common Cache HWP Exit library
SUBDIRS += $(CACHE_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/cache
CACHELIB := $(OBJDIR)/cache/libcache.a
LLIBS += -lcache

# Common Core libraries
SUBDIRS += $(CORE_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/core
CORELIB := $(OBJDIR)/core/libcore.a
LLIBS += -lcore

# Common Perv libraries
SUBDIRS += $(PERV_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/perv
PERVLIB := $(OBJDIR)/perv/libperv.a
LLIBS += -lperv

# Common Nest libraries
SUBDIRS += $(NEST_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/nest
NESTLIB := $(OBJDIR)/nest/libnest.a
LLIBS += -lnest

# Initfiles library
SUBDIRS += $(INITFILES_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/initfiles
INITFILESLIB := $(OBJDIR)/initfiles/libinitfiles.a
LLIBS += -linitfiles

# Common Pm libraries
SUBDIRS += $(PM_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/pm
PMLIB := $(OBJDIR)/pm/libpm.a
LLIBS += -lpm

SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SBE_NAME).out
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SBE_NAME).map
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SBE_NAME).dis
SIMICS_PPE_TAR_FILES += $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out
SIMICS_PPE_TAR_FILES += $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).map
SIMICS_PPE_TAR_FILES += $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).dis
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).map
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).dis
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe.syms
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.xml
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.py
SIMICS_PPE_TAR_FILES += $(PPETRACEPP_DIR)/fsp-trace
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbeStringFile
SIMICS_PPE_TAR_FILES += $(PPETRACEPP_BIN_DIR)/ppe2fsp
SIMICS_BB_TAR_FILES += $(SIMICSTOOLS_SRCDIR)/*
SIMICS_BB_TAR_FILES += $(DEBUGTOOLS_SRCDIR)/simics-debug-framework.py
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/cmvc/makefile
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.h
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp_cmd.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/trac_interface.h
SIMICS_PPETRACEPP_TAR_FILES += $(PK_SRCDIR)/trace/pk_trace.h
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/plugins/sbeUserDataParser.C
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/sbeFFDCType.H

SBE_TOOLS := $(IMG_DIR)/sbe_default_tool

P9_XIP_TOOL := $(P9_XIP_BINDIR)/p9_xip_tool

LINK_OBJS = $(OBJS)

# Define the objects
OBJS := $(addprefix $(OBJDIR)/, $(TOP_OBJECTS))
LINK_OBJS += $(OBJS) $(GCCLIBS)

LINK_SCRIPT_SEEPROM = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptseeprom)
LINK_SCRIPT_SBE = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptsbe)

# ---- SEEPROM Image  ------
# This removes all unecessary headers from the ELF executable
$(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).dis: $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out
	./parsAndCutElf.py
	$(OBJDUMP) -S $< > $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).dis

#create a linked ELF executable
$(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out: ppe_trace_bin buildInfo $(SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SEEPROM)
	$(LD) -n -T$(LINK_SCRIPT_SEEPROM) -Map $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).map \
		-Bstatic -o $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out $(BOOT_OBJDIR)/$(BOOT_OBJECTS) $(BUILDDATA_OBJDIR)/base_ppe_header.o $(OBJS) $(LIB_DIRS) \
	    --start-group $(LLIBS) --end-group

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_SEEPROM): $(LINKER_DIR)/linkseeprom.cmd
	$(CPP) -I $(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/linkseeprom.cmd -o $(LINK_SCRIPT_SEEPROM)

# ---- PIBMEM Image  ------
#This removes all unecessary headers from the ELF executable
$(IMG_DIR)/$(IMAGE_SBE_NAME).bin $(IMG_DIR)/$(IMAGE_SBE_NAME).dis: $(IMG_DIR)/$(IMAGE_SBE_NAME).out
	$(OBJCOPY) -O binary  $< $(IMG_DIR)/$(IMAGE_SBE_NAME).bin --pad-to \
	      0x`/usr/bin/nm $(IMG_DIR)/$(IMAGE_SBE_NAME).out | grep "A _sbe_end" \
		  | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(IMG_DIR)/$(IMAGE_SBE_NAME).dis
	$(NM)  $< > $(IMG_DIR)/$(IMAGE_SBE_NAME).syms

# create a linked ELF executable
$(IMG_DIR)/$(IMAGE_SBE_NAME).out: ppe_trace_bin  buildInfo $(SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SBE)
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SBE) -Map $(IMG_DIR)/$(IMAGE_SBE_NAME).map -Bstatic -o $(IMG_DIR)/$(IMAGE_SBE_NAME).out $(LIB_DIRS) $(BUILDDATA_OBJDIR)/base_sbe_fixed.o  --start-group $(LLIBS) --end-group

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SBE): $(LINKER_DIR)/linksbe.cmd
	$(CPP) -E -x c++ -I $(INCLUDES) -P $(DEFS) $(LINKER_DIR)/linksbe.cmd -o $(LINK_SCRIPT_SBE)

all: $(OBJDIR) xml $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin $(IMG_DIR)/$(IMAGE_SBE_NAME).bin \
	 $(SBE_TOOLS) normalize defaultset $(IMG_DIR)/fixed.bin appendloader \
	 appendoverrides add_LoaderAddr symbols tracehash buildinfo report

install: all
	@echo "Installing simics.tar"
	@rm -rf $(OBJDIR)/simics-tar-dir
	@mkdir $(OBJDIR)/simics-tar-dir
	@mkdir $(OBJDIR)/simics-tar-dir/ppetracepp
	@mkdir $(OBJDIR)/simics-tar-dir/parserplugins
	@cp  $(SIMICS_PPE_TAR_FILES) $(OBJDIR)/simics-tar-dir
	@cp  $(SIMICS_BB_TAR_FILES) $(OBJDIR)/simics-tar-dir
	@cp  $(SIMICS_PPETRACEPP_TAR_FILES) $(OBJDIR)/simics-tar-dir/ppetracepp
	@cp  $(SIMICS_PARSERPLUGINS_TAR_FILES) $(OBJDIR)/simics-tar-dir/parserplugins
	@cd  $(OBJDIR)/simics-tar-dir && tar -cvf simics.tar * && mv simics.tar $(IMG_DIR) && cd ..
	@rm -rf $(OBJDIR)/simics-tar-dir
	@echo "Generated simics.tar in Sbe images Directory"

.PHONY: all normalize defaultset appendloader add_LoaderAddr symbols report \
		appendoverrides xml tracehash topfixedheaders  $(SUBDIRS)

buildinfo: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set build_date `date +%Y%m%d`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set build_time `date +%H%M`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set build_user `id -un`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set build_host `hostname`

add_LoaderAddr: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out | grep __l1Loader | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set L2_LoaderAddr 0x`nm $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out | grep __l2_loader_setup | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set kernelAddr 0x`nm $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out | grep __pk_boot | cut -f 1 -d " "`

symbols: $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out $(IMG_DIR)/$(IMAGE_SBE_NAME).out
	$(NM) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
	$(NM) $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out >> $(IMG_DIR)/$(SBE_SYMBOLS_NAME)

report: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin report > $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).rpt

# Create build Info file
buildInfo:
	python buildInfo.py


#Create an obj directory if needed
$(LINK_OBJS) $(OBJS) $(OBJS:.o=.d)

ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pervasive_attributes.xml
# TODO via RTC 142708
# Workaound for ATTR_CHIP_UNIT_POS. Remove Mirror_WA_attributes.xml once fapi
# support is in.
ATTRFILES += $(BUILD_DIR)/Mirror_WA_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_sbe_load_bootloader_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/core_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/nest_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_plat_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_cache_contained_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/chip_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/chip_ec_attributes.xml

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(IMG_DIR)
	mkdir -p $(OBJDIR)/genfiles
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl $(GENFILES_DIR) $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(GENFILES_DIR) $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(TOOLS_ATTR_DIR)/src --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PLAT_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(TOOLS_ATTR_DIR)/src --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

# Build the subdirectories
$(SUBDIRS):
	$(MAKE) -I $(INCLUDES) -C $@ -f Makefile


#Build the SBE XIP Tools
$(SBE_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(TOOLS_IMAGE_DIR) -f Makefile

# Build the P9-XIP Tool
$(P9_XIP_TOOL):
	$(MAKE) -I $(P9_XIP_SRCDIR) -C $(P9_XIP_SRCDIR) -f Makefile BINDIR=$(P9_XIP_BINDIR)
	cp $(P9_XIP_TOOL) $(IMG_DIR)/

normalize: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin normalize

defaultset:$(SBE_TOOLS) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin normalize
	$(TOOLS_ATTR_DIR)/ppeSetFixed.pl $(IMG_DIR) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

# Build the trace utilities
ppe_trace_bin:
	$(MAKE) -I $(INCLUDES) -C $(PPETRACEPP_DIR) -f Makefile #$(PK trace util)


# Build hwp_error_info.H.  If the script fails then print the contents of
# the header and then delete whatever garbage the script left to force it to
# be built again.
#

xml: $(FAPI_RC)

$(FAPI_RC): $(TOOLS_ATTR_DIR)/parseErrorInfo.pl $(ERROR_XML_FILES)
	$(TOOLS_ATTR_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=$(GENFILES_DIR) $(ERROR_XML_FILES)

$(IMG_DIR)/fixed.bin: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin extract .fixed $(IMG_DIR)/fixed.bin


appendloader: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin append .baseloader $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin

# collect all of the trace hash files for this image into a single sbeStringFile
tracehash:
	mkdir -p $(OBJDIR)
	$(THASH) -c -d $(OBJDIR) -s $(IMG_DIR)/sbeStringFile

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)
	rm -fr $(IMG_DIR)

#Add dependencies to header files
ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
