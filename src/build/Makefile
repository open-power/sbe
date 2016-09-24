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

include $(HWPERR_SRCDIR)/hwpErrors.mk

# Common Utils library
SUBDIRS += $(UTILS_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/utils
UTILSLIB := $(OBJDIR)/utils/libutils.a
LLIBS += -lutils

SUBDIRS += $(ISTEP2_INFRA_DIR)
LIB_DIRS += -L$(OBJDIR-ISTEP2)
ISTEP2LIB := $(OBJDIR-ISTEP2)/libistep2.a
LLIBS += -listep2

SUBDIRS += $(ISTEP3_INFRA_DIR)
LIB_DIRS += -L$(OBJDIR-ISTEP3)
ISTEP3LIB := $(OBJDIR-ISTEP3)/libistep3.a
LLIBS += -listep3

SUBDIRS += $(ISTEP4_INFRA_DIR)
LIB_DIRS += -L$(OBJDIR-ISTEP4)
ISTEP4LIB := $(OBJDIR-ISTEP4)/libistep4.a
LLIBS += -listep4

SUBDIRS += $(ISTEP5_INFRA_DIR)
LIB_DIRS += -L$(OBJDIR-ISTEP5)
ISTEP5LIB := $(OBJDIR-ISTEP5)/libistep5.a
LLIBS += -listep5

SUBDIRS += $(ISTEPMPIPL_INFRA_DIR)
LIB_DIRS += -L$(OBJDIR-ISTEPMPIPL)
ISTEPMPIPL := $(OBJDIR-ISTEPMPIPL)/libistepmpipl.a
LLIBS += -listepmpipl

SUBDIRS += $(ISTEPCOMMON_INFRA_DIR)
LIB_DIRS += -L$(OBJDIR-ISTEPCOMMON)
ISTEPCOMMON := $(OBJDIR-ISTEPCOMMON)/libistepcommon.a
LLIBS += -listepcommon

SIMICS_PPE_TAR_FILES += $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out
SIMICS_PPE_TAR_FILES += $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).map
SIMICS_PPE_TAR_FILES += $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).dis
ifeq ($(img), seeprom)
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).map
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).dis
endif
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe.syms
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.xml
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.py
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbeStringFile
SIMICS_PPE_TAR_FILES += $(PPETRACEPP_BIN_DIR)/ppe2fsp
SIMICS_BB_TAR_FILES += $(SIMICSTOOLS_SRCDIR)/*
SIMICS_BB_TAR_FILES += $(DEBUGTOOLS_SRCDIR)/simics-debug-framework.py
SIMICS_BB_TAR_FILES += $(DEBUGTOOLS_SRCDIR)/sbe-debug.py
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/cmvc/makefile
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.h
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp_cmd.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/trac_interface.h
SIMICS_PPETRACEPP_TAR_FILES += $(PK_SRCDIR)/trace/pk_trace.h
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/plugins/sbeUserDataParser.C
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/sbeFFDCType.H

SBE_TOOLS := $(IMG_DIR)/sbe_default_tool
SBE_IPL_TOOLS := $(IMG_DIR)/p9_ipl_build

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

ifeq ($(img), pibmem)
# ---- PIBMEM Image  ------
#This removes all unecessary headers from the ELF executable
$(IMG_DIR)/$(IMAGE_SBE_NAME).bin $(IMG_DIR)/$(IMAGE_SBE_NAME).dis: $(IMG_DIR)/$(IMAGE_SBE_NAME).out
	$(OBJCOPY) -O binary  $< $(IMG_DIR)/$(IMAGE_SBE_NAME).bin --pad-to \
	      0x`/usr/bin/nm $(IMG_DIR)/$(IMAGE_SBE_NAME).out | grep "A _sbe_end" \
		  | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(IMG_DIR)/$(IMAGE_SBE_NAME).dis

# create a linked ELF executable
$(IMG_DIR)/$(IMAGE_SBE_NAME).out: ppe_trace_bin  buildInfo $(SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SBE)
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SBE) -Map $(IMG_DIR)/$(IMAGE_SBE_NAME).map -Bstatic -o $(IMG_DIR)/$(IMAGE_SBE_NAME).out $(LIB_DIRS) $(BUILDDATA_OBJDIR)/base_sbe_fixed.o  --start-group $(LLIBS) --end-group

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SBE): $(LINKER_DIR)/linksbe.cmd
	$(CPP) -E -x c++ -I $(INCLUDES) -P $(DEFS) $(LINKER_DIR)/linksbe.cmd -o $(LINK_SCRIPT_SBE)
endif

ifeq ($(img), pibmem)
all: $(OBJDIR) xml \
	$(IMG_DIR)/$(IMAGE_SBE_NAME).bin \
	 symbols tracehash
endif
ifeq ($(img), seeprom)
all: $(OBJDIR) xml $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin \
	 $(SBE_TOOLS) $(SBE_IPL_TOOLS) normalize defaultset $(IMG_DIR)/fixed.bin \
	 appendloader appendoverrides add_LoaderAddr symbols tracehash buildinfo \
	 report
endif

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

ifeq ($(img), seeprom)
symbols: $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out
	$(NM) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
	$(NM) $(BOOT_OBJDIR)/$(IMAGE_LOADER_NAME).out >> $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif
ifeq ($(img), pibmem)
symbols: $(IMG_DIR)/$(IMAGE_SBE_NAME).out
	$(NM) $(IMG_DIR)/$(IMAGE_SBE_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif

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
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(PLAT_FAPI2_DIR)/src/plat --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PLAT_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(PLAT_FAPI2_DIR)/src/plat --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

# Build the subdirectories
$(SUBDIRS):
	$(MAKE) -I $(INCLUDES) -C $@ -f Makefile


#Build the SBE XIP Tools
$(SBE_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(TOOLS_IMAGE_DIR) -f Makefile

#Build the IPL Build Tools
$(SBE_IPL_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(IPL_BUILD_DIR) -f Makefile

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

$(FAPI_RC): $(IMPORT_TOOLS_DIR)/parseErrorInfo.pl $(ERROR_XML_FILES)
	$(IMPORT_TOOLS_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=$(GENFILES_DIR) $(ERROR_XML_FILES)

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
