# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2018
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
############################################################################
# Image settings
# pass argument img=seeprom/pibmem
# By default seeprom image is built
ifndef img
export img = seeprom
endif

# pass argument project=power/power_axone/zthemis
# By default power image is built
ifndef project
export project = power
endif
###########################################################################

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

SUBDIRS += ../sbefw/core
LIB_DIRS += -L$(OBJDIR)/sbefw/core_pibmem
SBECORELIB := $(OBJDIR)/sbefw/core_pibmem/libsbecorepibmem.a
LLIBS += -lsbecorepibmem

SEEPROM_SUBDIRS += targetseeprom_../sbefw/core
LIB_DIRS += -L$(OBJDIR)/sbefw/core_seeprom
SBECORESEEPROMLIB := $(OBJDIR)/sbefw/core_seeprom/libsbecoreseeprom.a
LLIBS += -lsbecoreseeprom

SUBDIRS += ../sbefw/app/$(PROJECT_APP_DIR)
LIB_DIRS += -L$(OBJDIR)/sbefw/app/$(PROJECT_APP_DIR)_pibmem
SBEAPPLIB := $(OBJDIR)/sbefw/app/$(PROJECT_APP_DIR)_pibmem/lib$(PROJECT_APP_PIBMEM_LIB).a
LLIBS += -l$(PROJECT_APP_PIBMEM_LIB)

SEEPROM_SUBDIRS += targetseeprom_../sbefw/app/$(PROJECT_APP_DIR)
LIB_DIRS += -L$(OBJDIR)/sbefw/app/$(PROJECT_APP_DIR)_seeprom
SBECORESEEPROMLIB := $(OBJDIR)/sbefw/app/$(PROJECT_APP_DIR)_seeprom/lib$(PROJECT_APP_SEEPROM_LIB).a
LLIBS += -l$(PROJECT_APP_SEEPROM_LIB)

SUBDIRS += ../sbefw/app/common
LIB_DIRS += -L$(OBJDIR)/sbefw/app/common_pibmem
SBEAPPLIB := $(OBJDIR)/sbefw/app/common_pibmem/libsbeappcommonpibmem.a
LLIBS += -lsbeappcommonpibmem

SEEPROM_SUBDIRS += targetseeprom_../sbefw/app/common
LIB_DIRS += -L$(OBJDIR)/sbefw/app/common_seeprom
SBECORESEEPROMLIB := $(OBJDIR)/sbefw/app/common_seeprom/libsbeappcommonseeprom.a
LLIBS += -lsbeappcommonseeprom

# FAPI2 library
SUBDIRS += $(PLAT_FAPI2_DIR)
LIB_DIRS += -L$(OBJDIR)/fapi2
FAPI2LIB := $(OBJDIR)/fapi2/libfapi2.a
LLIBS += -lfapi2

SEEPROM_SUBDIRS += targetseeprom_$(PLAT_FAPI2_DIR)
LIB_DIRS += -L$(OBJDIR)/fapi2_seeprom
FAPI2LIB := $(OBJDIR)/fapi2/libfapi2seeprom.a
LLIBS += -lfapi2seeprom

include $(HWPERR_SRCDIR)/hwpErrors.mk

# Common Utils library
SUBDIRS += $(UTILS_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/utils
UTILSLIB := $(OBJDIR)/utils/libutils.a
LLIBS += -lutils

SUBDIRS += $(PROJ_SUBDIRS)
LIB_DIRS += $(PROJ_LIB_DIRS)
LLIBS += $(PROJ_LLIBS)

SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_LOADER_NAME).dis
ifeq ($(img), seeprom)
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe_seeprom_*.dis
endif
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe_*.syms
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.xml
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.py
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbeStringFile*
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
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/core/plugins/sbeUserDataParser.C
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/core/sbeFFDCType.H

SBE_TOOLS := $(IMG_DIR)/sbe_default_tool
SBE_IPL_TOOLS := $(IMG_DIR)/p9_ipl_build

P9_XIP_TOOL := $(P9_XIP_BINDIR)/p9_xip_tool

LINK_OBJS = $(OBJS)

# Define the objects
OBJS := $(addprefix $(OBJDIR)/, $(TOP_OBJECTS))
LINK_OBJS += $(OBJS) $(GCCLIBS)

LINK_SCRIPT_SEEPROM = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptseeprom_$(IMAGE_SUFFIX))
LINK_SCRIPT_SBE = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptsbe)

# ---- SEEPROM Image  ------
# This removes all unecessary headers from the ELF executable
$(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).dis: $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out
	./parsAndCutElf.py $(IMAGE_SUFFIX) $(IMG_DIR)
	$(OBJDUMP) -S $< > $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).dis

#create a linked ELF executable
$(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out: ppe_trace_bin buildInfo $(SUBDIRS) $(SEEPROM_SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SEEPROM)
	$(LD) -n -T$(LINK_SCRIPT_SEEPROM) -Map $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).map \
		-Bstatic -o $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out $(addprefix $(BOOT_OBJDIR)/, $(BOOT_OBJECTS)) $(BUILDDATA_OBJDIR)/$(IMAGE_BASE_PPE_HEADER).o $(OBJS) $(LIB_DIRS) \
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
$(IMG_DIR)/$(IMAGE_SBE_NAME).out: ppe_trace_bin  buildInfo $(SUBDIRS) $(SEEPROM_SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SBE)
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SBE) -Map $(IMG_DIR)/$(IMAGE_SBE_NAME).map -Bstatic -o $(IMG_DIR)/$(IMAGE_SBE_NAME).out $(LIB_DIRS) $(BUILDDATA_OBJDIR)/base_sbe_fixed.o  --start-group $(LLIBS) --end-group

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SBE): $(LINKER_DIR)/linksbe.cmd
	$(CPP) -E -x c++ -I $(INCLUDES) -P $(DEFS) $(LINKER_DIR)/linksbe.cmd -o $(LINK_SCRIPT_SBE)
endif

ifeq ($(img), pibmem)
all: $(OBJDIR) xml security\
	$(IMG_DIR)/$(IMAGE_SBE_NAME).bin \
	 symbols tracehash
endif
ifeq ($(img), seeprom)
all: $(OBJDIR) xml security $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin \
	 $(SBE_TOOLS) $(SBE_IPL_TOOLS) normalize compress defaultset $(IMG_DIR)/fixed.bin \
	 appendloader appendoverrides add_LoaderAddr symbols tracehash buildinfo \
	 report
endif

install: all tar

tar:
	@cp $(IMG_DIR)/$(SBE_STRINGFILE_NAME) $(IMG_DIR)/sbeStringFile || true
	@echo "Installing simics.tar"
	@rm -rf $(TAR_OBJDIR)
	@mkdir $(TAR_OBJDIR)
	@mkdir $(TAR_OBJDIR)/ppetracepp
	@mkdir $(TAR_OBJDIR)/parserplugins
	@cp  $(SIMICS_PPE_TAR_FILES) $(TAR_OBJDIR)
	@cp  $(SIMICS_BB_TAR_FILES) $(TAR_OBJDIR)
	@cp  $(SIMICS_PPETRACEPP_TAR_FILES) $(TAR_OBJDIR)/ppetracepp
	@cp  $(SIMICS_PARSERPLUGINS_TAR_FILES) $(TAR_OBJDIR)/parserplugins
	@cd  $(TAR_OBJDIR) && tar -cvf simics.tar * && mv simics.tar $(IMG_DIR) && cd ..
	@rm -rf $(TAR_OBJDIR)
	@echo "Generated simics.tar in Sbe images Directory"

.PHONY: all normalize compress defaultset appendloader add_LoaderAddr symbols report \
		appendoverrides xml security tracehash topfixedheaders  $(SUBDIRS) $(SEEPROM_SUBDIRS) \
		tar install

buildinfo: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	if [ -z "$(REPRODUCIBLE)" ]; then ./updateBuildTag.py $(P9_XIP_TOOL) $(IMG_DIR) $(IMAGE_SEEPROM_NAME); fi

add_LoaderAddr: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out | grep __l1Loader | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set L2_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_LOADER_NAME).out | grep __l2_loader_setup | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin set kernelAddr 0x`nm $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out | grep __pk_boot | cut -f 1 -d " "`

ifeq ($(img), seeprom)
symbols: $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out $(IMG_DIR)/$(IMAGE_LOADER_NAME).out
	$(NM) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
	$(NM) $(IMG_DIR)/$(IMAGE_LOADER_NAME).out >> $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif
ifeq ($(img), pibmem)
symbols: $(IMG_DIR)/$(IMAGE_SBE_NAME).out
	$(NM) $(IMG_DIR)/$(IMAGE_SBE_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif

report: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin report > $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).rpt

# Create build Info file
buildInfo:
	python buildInfo.py $(GENFILES_DIR)


#Create an obj directory if needed
$(LINK_OBJS) $(OBJS) $(OBJS:.o=.d)

ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pervasive_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/system_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/unit_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_sbe_load_bootloader_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/core_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/nest_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_plat_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_hwp_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_cache_contained_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/chip_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/chip_ec_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_hcode_image_build_attributes.xml

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(IMG_DIR)
	mkdir -p $(GENFILES_DIR)
	mkdir -p $(IMPORT_OBJDIR)
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl $(GENFILES_DIR) $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(GENFILES_DIR) $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(PLAT_FAPI2_DIR) --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PLAT_FAPI2_DIR) $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(PLAT_FAPI2_DIR) --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

# Build the subdirectories
$(SUBDIRS):
	$(MAKE) -I $(INCLUDES) -C $@ -f Makefile
$(SEEPROM_SUBDIRS):
	$(MAKE) -I $(INCLUDES) -C $(subst targetseeprom_,, $@) -f Makefile img_mem=seeprom

#Build the SBE XIP Tools
$(SBE_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(TOOLS_IMAGE_DIR) -f Makefile

#Build the IPL Build Tools
$(SBE_IPL_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(IPL_BUILD_DIR) -f Makefile

# Build the P9-XIP Tool
$(P9_XIP_TOOL):
	$(MAKE) -I $(P9_XIP_SRCDIR) -C $(P9_XIP_SRCDIR) -f Makefile BINDIR=$(P9_XIP_BINDIR) CXX_FLAGS=-D__PPE__=1
	cp $(P9_XIP_TOOL) $(IMG_DIR)/

normalize: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin normalize

compress:
	$(BOOT_SRCDIR)/sbeCompression.py -l $(IMG_DIR) -i $(IMAGE_SEEPROM_NAME).bin

defaultset:$(SBE_TOOLS) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin normalize
	$(TOOLS_ATTR_DIR)/ppeSetFixed.pl $(IMG_DIR) $(IMG_DIR)/$(IMAGE_SEEPROM_NAME).bin $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

# Build the trace utilities
ppe_trace_bin:
	$(MAKE) -I $(INCLUDES) -C $(PPETRACEPP_DIR) -f Makefile #$(PK trace util)

# generate whitelist and blacklist security algorithm
security:
	python $(SECURITY_SRC_DIR)/securityRegListGen.py -f $(SECURITY_LIST) -o $(GENFILES_DIR)

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
	$(THASH) -c -d $(OBJDIR) -s $(IMG_DIR)/$(SBE_STRINGFILE_NAME)

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(SBE_ROOT_DIR)/obj
	rm -fr $(IMG_ROOT_DIR)

#Add dependencies to header files
ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
