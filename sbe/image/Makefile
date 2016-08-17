# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/image/Makefile $
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

#Pull in object file names for the top directory
include topfiles.mk

SUBDIRS := $(PK_SRCDIR)/$(PPE_TYPE)
LIB_DIRS += -L$(OBJDIR)/pk
PKLIB := $(OBJDIR)/pk/libpk.a
LLIBS += -lpk

SUBDIRS += ../sbefw
LIB_DIRS += -L$(OBJDIR)/sbefw
SBEFWLIB := $(OBJDIR)/sbefw/libsbefw.a
LLIBS += -lsbefw

# FAPI2 library
SUBDIRS += $(PLAT_FAPI2_DIR)/src
LIB_DIRS += -L$(OBJDIR)/fapi2
FAPI2LIB := $(OBJDIR)/fapi2/libfapi2.a
LLIBS += -lfapi2

LIB_DIRS += -L$(GCC-TOOL-PATH)/libgcc
LLIBS += -lmath -leabi -lc

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

SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_SBE_NAME).out
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_SBE_NAME).map
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_SBE_NAME).dis
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_LOADER_NAME).out
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_LOADER_NAME).map
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_LOADER_NAME).dis
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_SEEPROM_NAME).map
SIMICS_PPE_TAR_FILES += $(OBJDIR)/$(IMAGE_SEEPROM_NAME).dis
SIMICS_PPE_TAR_FILES += $(OBJDIR)/sbe.syms
SIMICS_PPE_TAR_FILES += $(OBJDIR)/../test/*.xml
SIMICS_PPE_TAR_FILES += $(OBJDIR)/../test/*.py
SIMICS_PPE_TAR_FILES += $(OBJDIR)/../../tools/ppetracepp/fsp-trace
SIMICS_PPE_TAR_FILES += $(OBJDIR)/sbeStringFile
SIMICS_PPE_TAR_FILES += $(OBJDIR)/ppe2fsp
SIMICS_BB_TAR_FILES += $(OBJDIR)/../build/simics/*
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/cmvc/makefile
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.h
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp_cmd.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/trac_interface.h
SIMICS_PPETRACEPP_TAR_FILES += $(PK_SRCDIR)/trace/pk_trace.h
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_ROOT_DIR)/sbe/sbefw/plugins/sbeUserDataParser.C
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_ROOT_DIR)/sbe/sbefw/sbeFFDCType.H

SBE_TOOLS := $(TOOLS_IMAGE_DIR)/sbe_default_tool

P9_XIP_TOOL := $(P9_XIP_BINDIR)/p9_xip_tool

LINK_OBJS = $(OBJS)

# Define the objects
OBJS := $(addprefix $(OBJDIR)/, $(TOP_OBJECTS))
LINK_OBJS += $(OBJS) $(GCCLIBS)

# Loader Objects
LOADER_OBJS := $(addprefix $(OBJDIR)/, $(BASE_LOADER_OBJECTS))

#Otprom Loader Objects
OTPROM_LOADER_OBJS := $(addprefix $(OBJDIR)/, $(OTPROM_LOADER_OBJECTS))

LINK_SCRIPT_SEEPROM = $(addprefix $(OBJDIR)/, linkscriptseeprom)
LINK_SCRIPT_SBE = $(addprefix $(OBJDIR)/, linkscriptsbe)
LINK_SCRIPT_LOADER = $(addprefix $(OBJDIR)/, linkscriptloader)
LINK_SCRIPT_OTPROM =  $(addprefix $(OBJDIR)/, linkscriptotprom)

# ---- SEEPROM Image  ------
# This removes all unecessary headers from the ELF executable
$(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_SEEPROM_NAME).dis: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
	./parsAndCutElf.py
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_SEEPROM_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_SEEPROM_NAME).out: ppe_trace_bin buildInfo $(SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SEEPROM)
	$(LD) -n -T$(LINK_SCRIPT_SEEPROM) -Map $(OBJDIR)/$(IMAGE_SEEPROM_NAME).map \
		-Bstatic -o $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out  $(OBJS) $(LIB_DIRS) \
	    --start-group $(LLIBS) --end-group  -leabi -lmath -lc

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_SEEPROM): linkseeprom.cmd
	$(CPP) -I. -E -x c++ -P $(DEFS) linkseeprom.cmd -o $(LINK_SCRIPT_SEEPROM)

# ---- PIBMEM Image  ------
#This removes all unecessary headers from the ELF executable
$(OBJDIR)/$(IMAGE_SBE_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).dis: $(OBJDIR)/$(IMAGE_SBE_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_SBE_NAME).bin --pad-to \
	      0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_SBE_NAME).out | grep "A _sbe_end" \
		  | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_SBE_NAME).dis
	$(NM)  $< > $(OBJDIR)/$(IMAGE_SBE_NAME).syms

# create a linked ELF executable
$(OBJDIR)/$(IMAGE_SBE_NAME).out: ppe_trace_bin  buildInfo $(OBJDIR)/base_sbe_fixed.o $(SUBDIRS) $(LINK_OBJS) $(LINK_SCRIPT_SBE)
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SBE) -Map $(OBJDIR)/$(IMAGE_SBE_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_SBE_NAME).out $(LIB_DIRS) $(OBJDIR)/base_sbe_fixed.o  --start-group $(LLIBS) --end-group

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SBE): linksbe.cmd
	$(CPP) -E -x c++ -I. -P $(DEFS) linksbe.cmd -o $(LINK_SCRIPT_SBE)

# ---- Otprom Image  ------
$(OBJDIR)/$(IMAGE_OTPROM_NAME).bin $(OBJDIR)/$(IMAGE_OTPROM_NAME).dis: $(OBJDIR)/$(IMAGE_OTPROM_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_OTPROM_NAME).bin `/usr/bin/nm $(OBJDIR)/$(IMAGE_OTPROM_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_OTPROM_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_OTPROM_NAME).out: $(OTPROM_LOADER_OBJS) $(LINK_SCRIPT_OTPROM)
	$(LD) -e base_loader -T$(LINK_SCRIPT_OTPROM) -Map $(OBJDIR)/$(IMAGE_OTPROM_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_OTPROM_NAME).out $(OTPROM_LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_OTPROM): linkotprom.cmd
	$(CPP) -I. -E -x c++ -P $(DEFS) linkotprom.cmd -o $(LINK_SCRIPT_OTPROM)


# ---- Loader Image  ------
$(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).dis: $(OBJDIR)/$(IMAGE_LOADER_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_LOADER_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_LOADER_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_LOADER_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_LOADER_NAME).out: $(LOADER_OBJS) $(LINK_SCRIPT_LOADER)
	$(LD) -e base_loader -T$(LINK_SCRIPT_LOADER) -Map $(OBJDIR)/$(IMAGE_LOADER_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_LOADER_NAME).out $(LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_LOADER): linkloader.cmd
	$(CPP) -I. -E -x c++ -P $(DEFS) linkloader.cmd -o $(LINK_SCRIPT_LOADER)

all: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).bin \
	 $(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(OBJDIR)/$(IMAGE_OTPROM_NAME).bin \
	 $(SBE_TOOLS) normalize defaultset $(OBJDIR)/fixed.bin appendloader \
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
	@cd  $(OBJDIR)/simics-tar-dir && tar -cvf simics.tar * && mv simics.tar $(OBJDIR) && cd ..
	@rm -rf $(OBJDIR)/simics-tar-dir
	@echo "Generated simics.tar in Sbe Obj Directory"

.PHONY: all normalize defaultset appendloader add_LoaderAddr symbols report \
		appendoverrides xml tracehash topfixedheaders  $(SUBDIRS)

buildinfo: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_date `date +%Y%m%d`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_time `date +%H%M`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_user `id -un`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_host `hostname`

add_LoaderAddr: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set L1_LoaderAddr 0x`nm $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out | grep __pmLoader | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set L2_LoaderAddr 0x`nm $(OBJDIR)/$(IMAGE_LOADER_NAME).out | grep __base_loader_setup | cut -f 1 -d " "`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set kernelAddr 0x`nm $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out | grep __pk_boot | cut -f 1 -d " "`

symbols: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out $(OBJDIR)/$(IMAGE_LOADER_NAME).out $(OBJDIR)/$(IMAGE_SBE_NAME).out
	$(NM) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out > $(OBJDIR)/$(SBE_SYMBOLS_NAME)
	$(NM) $(OBJDIR)/$(IMAGE_LOADER_NAME).out >> $(OBJDIR)/$(SBE_SYMBOLS_NAME)

report: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin report > $(OBJDIR)/$(IMAGE_SEEPROM_NAME).rpt

# Create build Info file
buildInfo:
	python buildInfo.py


#Create an obj directory if needed
$(LINK_OBJS) $(OBJS) $(OBJS:.o=.d) $(OBJDIR)/base_sbe_fixed.o $(OBJDIR)/base_sbe_fixed.d: | $(OBJDIR)

ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pervasive_attributes.xml
# TODO via RTC 142708
# Workaound for ATTR_CHIP_UNIT_POS. Remove Mirror_WA_attributes.xml once fapi
# support is in.
ATTRFILES += $(IMPORT_XML_DIR)/../../../../../sbe/image/Mirror_WA_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_sbe_load_bootloader_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/core_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/nest_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_plat_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_cache_contained_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/chip_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/chip_ec_attributes.xml

$(OBJDIR):
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(TOOLS_ATTR_DIR)/src --inc $(PPE_FAPI2_DIR)/include --src $(PPE_FAPI2_DIR)/src
	$(TOOLS_ATTR_DIR)/ppeCreateIfAttrService.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	mkdir -p $(OBJDIR)

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(TOOLS_ATTR_DIR)/src --inc $(PPE_FAPI2_DIR)/include --src $(PPE_FAPI2_DIR)/src

attrserv:
	$(TOOLS_ATTR_DIR)/ppeCreateIfAttrService.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)



# Build the subdirectories
$(SUBDIRS):
	$(MAKE) -I $(IMAGE_SRCDIR) -C $@ -f Makefile


#Build the SBE XIP Tools
$(SBE_TOOLS):
	$(MAKE) -C $(TOOLS_IMAGE_DIR) -f Makefile

# Build the P9-XIP Tool
$(P9_XIP_TOOL):
	$(MAKE) -I $(P9_XIP_SRCDIR) -C $(P9_XIP_SRCDIR) -f Makefile #$(P9_XIP_TOOL)

normalize: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin normalize

defaultset: $(SBE_TOOLS) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin normalize
	$(TOOLS_ATTR_DIR)/ppeSetFixed.pl $(TOOLS_IMAGE_DIR) $(BASE_OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

# Build the trace utilities
ppe_trace_bin:
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PPETRACEPP_DIR) -f Makefile #$(PK trace util)


# Build hwp_error_info.H.  If the script fails then print the contents of
# the header and then delete whatever garbage the script left to force it to
# be built again.
#

xml: $(FAPI_RC)

$(FAPI_RC): $(TOOLS_ATTR_DIR)/parseErrorInfo.pl $(ERROR_XML_FILES)
	$(TOOLS_ATTR_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=. $(ERROR_XML_FILES)

$(OBJDIR)/fixed.bin: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin extract .fixed $(BASE_OBJDIR)/fixed.bin


appendloader: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin append .baseloader $(OBJDIR)/$(IMAGE_LOADER_NAME).bin

# collect all of the trace hash files for this image into a single sbeStringFile
tracehash:
	mkdir -p $(OBJDIR)
	$(THASH) -c -d $(OBJDIR) -s $(OBJDIR)/sbeStringFile

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)
	rm -f $(TOP-FIXED-HEADERS)
	$(MAKE) -C $(TOOLS_IMAGE_DIR) -f Makefile clean
	$(MAKE) -C $(P9_XIP_SRCDIR) -f Makefile clean
	rm -f collect_reg_ffdc.H hwp_return_codes.H hwp_error_info.H hwp_ffdc_classes.H collect_reg_ffdc.C set_sbe_error.H sbe_build_info.H
	rm -f $(GEN-CREATEATTRGETSETMACROS)
	rm -f $(GEN-PARSEATTRIBUTEINFO)
	rm -f $(GEN-CREATEIFATTRSERVICE)

#Add dependencies to header files
ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
