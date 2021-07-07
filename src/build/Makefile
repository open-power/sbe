# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2019
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

ifeq ($(img), pibmem)
IMAGE_NAME = $(IMAGE_SBE_NAME)
endif

ifeq ($(img), seeprom)
IMAGE_NAME = $(IMAGE_SEEPROM_NAME)
endif

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
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe_seeprom_*.bin
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe_otprom_*.bin
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

ifeq ($(img),seeprom)
IN_LINK_SCRIPT = linkseeprom.cmd
OUT_LINK_SCRIPT = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptseeprom_$(IMAGE_SUFFIX))
endif

ifeq ($(img),pibmem)
IN_LINK_SCRIPT = linkseeprom.cmd
OUT_LINK_SCRIPT = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptsbe)
endif

pre_sbe_image: xml ppe_trace_bin buildInfo linker_seeprom $(SBE_TOOLS) $(SBE_IPL_TOOLS) $(P9_XIP_TOOL) security
build_sbe_image: $(SUBDIRS) $(SEEPROM_SUBDIRS)

output_sbe_image:
	$(C2) "Generating $(IMG_DIR)/$(IMAGE_NAME).out"
	$(C1)$(LD) -e __system_reset -n -T$(OUT_LINK_SCRIPT) -Map $(IMG_DIR)/$(IMAGE_NAME).map \
		-Bstatic -o $(IMG_DIR)/$(IMAGE_NAME).out $(addprefix $(BOOT_OBJDIR)/, $(BOOT_OBJECTS)) $(BUILDDATA_OBJDIR)/$(IMAGE_BASE_PPE_HEADER).o $(OBJS) $(LIB_DIRS) \
	    --start-group $(LLIBS) --end-group --gc-sections
	$(C1)./parsAndCutElf.py $(IMAGE_SUFFIX) $(IMG_DIR) $(img)
	$(C1)$(OBJDUMP) -S $(IMG_DIR)/$(IMAGE_NAME).out > $(IMG_DIR)/$(IMAGE_NAME).dis

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
linker_seeprom:
	@mkdir -p $(OBJDIR)/build
	$(C1)$(CPP) -I $(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/$(IN_LINK_SCRIPT) -o $(OUT_LINK_SCRIPT)

ifeq ($(img), seeprom)
all:
	$(MAKE) $(OBJDIR)
	$(MAKE) pre_sbe_image
	$(MAKE) build_sbe_image
	$(MAKE) output_sbe_image
	$(MAKE) normalize
	$(MAKE) appendloader
	$(MAKE) compress
	$(MAKE) defaultset
	$(MAKE) add_LoaderAddr
	$(MAKE) symbols tracehash buildtag
	$(MAKE) report

add_LoaderAddr:
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep __l1Loader | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set L2_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_LOADER_NAME).out | grep __l2_loader_setup | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set kernelAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep __pk_boot | cut -f 1 -d " "`
endif

ifeq ($(img), pibmem)
all:
	$(MAKE) $(OBJDIR)
	$(MAKE) pre_sbe_image
	$(MAKE) build_sbe_image
	$(MAKE) output_sbe_image
	$(MAKE) normalize
	$(MAKE) defaultset
	$(MAKE) add_LoaderAddr
	$(MAKE) symbols tracehash buildtag
	$(MAKE) report

add_LoaderAddr:
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep __vectors | cut -f 1 -d " "`
endif

tar:
	@cp -f $(P9_XIP_TOOL) $(IMG_DIR)/
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
		appendoverrides xml security tracehash topfixedheaders \
		tar install

ifeq ($(img), seeprom)
symbols:
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_LOADER_NAME).out >> $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif
ifeq ($(img), pibmem)
symbols:
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif

report: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin report > $(IMG_DIR)/$(IMAGE_NAME).rpt

# Create build Info file
buildInfo:
	./buildInfo.py $(GENFILES_DIR)

buildtag:
	./updateBuildTag.py $(P9_XIP_TOOL) $(IMG_DIR) $(IMAGE_NAME)


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
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/memory_mcs_attributes.xml

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
$(SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $@ -f Makefile
$(SEEPROM_SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $(subst targetseeprom_,, $@) -f Makefile img_mem=seeprom

#Build the SBE XIP Tools
$(SBE_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(TOOLS_IMAGE_DIR) -f Makefile

#Build the IPL Build Tools
$(SBE_IPL_TOOLS): $(P9_XIP_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(IPL_BUILD_DIR) -f Makefile

# Build the P9-XIP Tool
# .PARALLEL = Forces Parallel
# .NOTPARALLEL = forces serial execution of makefile target..
.NOTPARALLEL: $(P9_XIP_TOOL)
$(P9_XIP_TOOL):
	$(MAKE) -I $(P9_XIP_SRCDIR) -C $(P9_XIP_SRCDIR) -f Makefile BINDIR=$(P9_XIP_BINDIR) CXX_FLAGS=-D__PPE__=1

normalize:
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin normalize

compress: $(IMG_DIR)/$(IMAGE_NAME).bin
	$(BOOT_SRCDIR)/sbeCompression.py -l $(IMG_DIR) -i $(IMAGE_NAME).bin -p $(P9_XIP_TOOL)

defaultset:
	$(TOOLS_ATTR_DIR)/ppeSetFixed.pl $(IMG_DIR) $(IMG_DIR)/$(IMAGE_NAME).bin $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

# Build the trace utilities
ppe_trace_bin:
	$(MAKE) -I $(INCLUDES) -C $(PPETRACEPP_DIR) -f Makefile #$(PK trace util)

# generate whitelist and blacklist security algorithm
security: $(OBJDIR)
	$(SECURITY_SRC_DIR)/securityRegListGen.py -f $(SECURITY_LIST) -o $(GENFILES_DIR)

# Build hwp_error_info.H.  If the script fails then print the contents of
# the header and then delete whatever garbage the script left to force it to
# be built again.
#

xml: $(FAPI_RC)

$(FAPI_RC): $(IMPORT_TOOLS_DIR)/parseErrorInfo.pl $(ERROR_XML_FILES)
	$(IMPORT_TOOLS_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=$(GENFILES_DIR) $(ERROR_XML_FILES)

$(IMG_DIR)/fixed.bin: $(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin extract .fixed $(IMG_DIR)/fixed.bin

appendloader:
	$(P9_XIP_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin append .baseloader $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin

# collect all of the trace hash files for this image into a single sbeStringFile
tracehash:
	$(THASH) -c -d $(OBJDIR) -s $(IMG_DIR)/$(SBE_STRINGFILE_NAME)

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(SBE_ROOT_DIR)/obj
	rm -fr $(IMG_ROOT_DIR)

#Add dependencies to header files
ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
