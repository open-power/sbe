# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2021
# [+] International Business Machines Corp.
# [+] Justin.Ginn@ibm.com
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
# pass argument img=seeprom/pibmem. When seeprom is used both the boot and
# measurement images are built only when set MEASUREMENT_IMAGE_SUPPORTED to 1.
# verification image is built only when set VERIFICATION_IMAGE_SUPPORTED to 1.
# By default seeprom image is built without measurement image

ifndef img
export img = seeprom
endif

# pass argument project=power/zthemis
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
else
IMAGE_NAME = $(IMAGE_SEEPROM_NAME)
endif

#Mesurement SEEPROM version: Any change in measurement seeprom code the below version field
#needs to be updated.
#Example: 3.4 = \x00\03\x00\x04
#Version = 4Bytes = 2.0 = "\x00\02\x00\x00"
MEASUREMENT_SEEPROM_VER="\x00\x02\x00\x00"
#4bytes reserved to keep the xip section 8 byte alligned
MEASUREMENT_SEEPROM_RESERVED="\x00\x00\x00\x00"

SUBDIRS := $(PK_BASELIB_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/pkbaselib
#PKLIB := $(OBJDIR)/p/libpk.a
LLIBS += -lpkbase

SUBDIRS += $(PK_SRCDIR)/../boltonlib/standard
LIB_DIRS += -L$(OBJDIR)/pk
#PKLIB := $(OBJDIR)/pk/libpk.a
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

include $(P9_HWPERR_SRCDIR)/hwpErrors.mk
include $(HWPERR_SRCDIR)/hwpErrors.mk
include $(GENERR_SRCDIR)/hwpErrors.mk
include $(OCMBERR_SRCDIR)/hwpErrors.mk

# Common Utils library
SUBDIRS += $(UTILS_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/utils
UTILSLIB := $(OBJDIR)/utils/libutils.a
LLIBS += -lutils

SUBDIRS += $(PROJ_SUBDIRS)
LIB_DIRS += $(PROJ_LIB_DIRS)
LLIBS += $(PROJ_LLIBS)

# PK Lib info
MEASUREMENT_LIB_DIRS += -L$(OBJDIR)/pkbaselib
MEASUREMENT_LLIBS += -lpkbase

MEASUREMENT_LIB_DIRS += -L$(OBJDIR)/pk
MEASUREMENT_LLIBS += -lpk

VERIFICATION_LIB_DIRS += -L$(OBJDIR)/pkbaselib
VERIFICATION_LLIBS += -lpkbase

VERIFICATION_LIB_DIRS += -L$(OBJDIR)/pk
VERIFICATION_LLIBS += -lpk

# Measurment PLAT/FAPI lib
SBEM_FAPI_SUBDIRS += sbemfapi_$(PLAT_FAPI2_DIR)

MEASUREMENT_SUBDIRS += $(MEASUREMENT_PROJ_SUBDIRS)
MEASUREMENT_LIB_DIRS += $(MEASUREMENT_PROJ_LIB_DIRS)
MEASUREMENT_LLIBS += $(MEASUREMENT_PROJ_LLIBS)

VERIFICATION_SUBDIRS += $(VERIFICATION_PROJ_SUBDIRS)
VERIFICATION_LIB_DIRS += $(VERIFICATION_PROJ_LIB_DIRS)
VERIFICATION_LLIBS += $(VERIFICATION_PROJ_LLIBS)

# TODO Add Pibmem dis-assembly as well as measurement dis-assembly as part of
# the simics.tar
# add seeprom .dis only for img is "seeprom"
ifeq ($(img), seeprom)
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe*seeprom*.dis
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe*seeprom*.bin
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe_otprom_*.bin
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/$(IMAGE_LOADER_NAME).dis
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbeMeasurementStringFile
endif

# tar content is excluded for DFT
ifneq ($(project), $(filter $(project), power_dft))
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbe_*.syms
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.xml
SIMICS_PPE_TAR_FILES += $(TEST_SRCDIR)/*.py
SIMICS_PPE_TAR_FILES += $(IMG_DIR)/sbeStringFile*
SIMICS_PPE_TAR_FILES += $(PPETRACEPP_BIN_DIR)/ppe2fsp
SIMICS_PPE_TAR_FILES += $(P10_IMAGE_TOOL_BINDIR)/ipl_image_tool
SIMICS_BB_TAR_FILES += $(SIMICSTOOLS_SRCDIR)/*
SIMICS_BB_TAR_FILES += $(SBE_SRC_DIR)/tools/scripts/getSbeTrace
SIMICS_BB_TAR_FILES += $(DEBUGTOOLS_SRCDIR)/simics-debug-framework.py
SIMICS_BB_TAR_FILES += $(DEBUGTOOLS_SRCDIR)/sbe-debug.py
SIMICS_BB_TAR_FILES += $(DEBUGTOOLS_SRCDIR)/sbeModifyPGvalue.py
SIMICS_BB_TAR_FILES += $(SBE_SRC_DIR)/tools/utils/ecc
SIMICS_BB_TAR_FILES += $(SBE_SRC_DIR)/tools/scripts/signSbeImage
SIMICS_BB_TAR_FILES += $(P10_IMAGE_TOOL_BINDIR)/ipl_image_tool
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/cmvc/makefile
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.h
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/ppe2fsp_cmd.c
SIMICS_PPETRACEPP_TAR_FILES += $(PPETRACEPP_DIR)/trac_interface.h
SIMICS_PPETRACEPP_TAR_FILES += $(PK_SRCDIR)/../ppetrace/pk_trace.h
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/core/plugins/sbeUserDataParser.C
SIMICS_PARSERPLUGINS_TAR_FILES += $(SBE_FW_DIR)/core/sbeFFDCType.H
endif

SBE_TOOLS := $(IMG_DIR)/sbe_default_tool

IPL_IMAGE_TOOL := $(P10_IMAGE_TOOL_BINDIR)/ipl_image_tool

ifeq ($(img), seeprom)
#boot seeprom image linker
IN_LINK_SCRIPT = linkseeprom.cmd
OUT_LINK_SCRIPT = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptseeprom_$(IMAGE_SUFFIX))
#measurement seeprom image linker
IN_MEASUREMENT_LINK_SCRIPT = linkmeasure.cmd
OUT_MEASUREMENT_LINK_SCRIPT = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptmeasure_$(IMAGE_SUFFIX))
else
# same linker is used for both boot_seeprom and pibmem
IN_LINK_SCRIPT = linkseeprom.cmd
OUT_LINK_SCRIPT = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptsbe)
endif

#verification seeprom image linker
IN_VERIFICATION_LINK_SCRIPT = linkverification.cmd
OUT_VERIFICATION_LINK_SCRIPT = $(addprefix $(OBJDIR)/build/linkerscripts, linkscriptverify_$(IMAGE_SUFFIX))

pre_sbe_image: xml ppe_trace_bin buildInfo linker_seeprom $(SBE_TOOLS) $(IPL_IMAGE_TOOL) security
build_sbe_image: $(SUBDIRS) $(SEEPROM_SUBDIRS)

output_sbe_image:
	$(C2) "Generating $(IMG_DIR)/$(IMAGE_NAME).out"
	$(C1)$(LD) -e __system_reset -n -T$(OUT_LINK_SCRIPT) -Map $(IMG_DIR)/$(IMAGE_NAME).map \
		-Bstatic -o $(IMG_DIR)/$(IMAGE_NAME).out $(addprefix $(BOOT_OBJDIR)/, $(L1_BOOT_OBJECTS)) $(BUILDDATA_OBJDIR)/$(IMAGE_BASE_PPE_HEADER).o $(OBJS) $(LIB_DIRS) \
		--start-group $(LLIBS) --end-group --gc-sections
ifeq ($(project), $(filter $(project), power_dft))
	$(C1)./parsAndCutElfDFT.py $(IMG_DIR) $(IMAGE_NAME)
else
	$(C1)./parsAndCutElf.py $(IMAGE_SUFFIX) $(IMG_DIR) $(img)
endif
	$(C1)$(OBJDUMP) -S $(IMG_DIR)/$(IMAGE_NAME).out > $(IMG_DIR)/$(IMAGE_NAME).dis

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
linker_seeprom:
	@mkdir -p $(OBJDIR)/build
	$(C1)$(CPP) -I $(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/$(IN_LINK_SCRIPT) -o $(OUT_LINK_SCRIPT)

## TODO:: Look into all pre linked info for measurement image
pre_measure_image: measure_xml ppe_trace_bin buildInfo linker_measure

# Build the measurement subdirectories files
build_measure_image: $(MEASUREMENT_SUBDIRS) $(SBEM_FAPI_SUBDIRS)

$(MEASUREMENT_SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $@ -f Makefile

$(SBEM_FAPI_SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $(subst sbemfapi_,, $@) -f Makefile img_mem=measurement

linker_measure:
	@mkdir -p $(OBJDIR)/build
	$(C1)$(CPP) -I $(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/$(IN_MEASUREMENT_LINK_SCRIPT) -o $(OUT_MEASUREMENT_LINK_SCRIPT)

output_measure_image:
	$(C2) "Generating $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).out"
	$(C1)$(LD) -e __system_reset -n -T$(OUT_MEASUREMENT_LINK_SCRIPT) -Map $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).map \
  -Bstatic -o $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).out $(addprefix $(BOOT_OBJDIR)/, $(L1_MEASUREMENT_OBJECTS)) \
	 $(BUILDDATA_OBJDIR)/$(IMAGE_BASE_MEASUREMENT_HEADER).o $(MEASUREMENT_LIB_DIRS) \
					   --start-group $(MEASUREMENT_LLIBS) --end-group --gc-sections
	$(C1)./parsAndCutElf.py $(IMAGE_SUFFIX) $(IMG_DIR) measurement_seeprom
	$(C1)$(OBJDUMP) -S $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).out > $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).dis

measure_normalize:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin normalize
	echo -n -e $(MEASUREMENT_SEEPROM_VER)$(MEASUREMENT_SEEPROM_RESERVED) > $(IMG_DIR)/temp_version.bin
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin append .sb_settings $(IMG_DIR)/temp_version.bin
	rm -f $(IMG_DIR)/temp_version.bin

measure_symbols:
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).out > $(IMG_DIR)/$(MEASUREMENT_SYMBOLS_NAME)

measure_report: $(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin report > $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).rpt

measure_buildtag:
	./updateBuildTag.py $(IPL_IMAGE_TOOL) $(IMG_DIR) $(IMAGE_MEASUREMENT_NAME)

# collect all of the trace hash files for measurement image into a single sbeMeasureStringFile
measure_tracehash:
	$(THASH) -c -d $(OBJDIR) -s $(IMG_DIR)/$(SBE_MEASUREMENT_STRINGFILE_NAME)

measure_xml: $(M_FAPI_RC)

$(M_FAPI_RC): $(IMPORT_TOOLS_DIR)/parseErrorInfo.pl $(HWPERR_SRCDIR)/p10_sbe_spi_cmd_errors.xml
	$(M_FAPI_TOOLS_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=$(GENFILES_DIR) $(HWPERR_SRCDIR)/p10_sbe_spi_cmd_errors.xml

### TODO :: Bellow L1_LoaderAddr will update once we get right L1 Loder File
add_MeasureLoaderAddr:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).out | grep __l1Loader | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin set kernelAddr 0x`nm $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).out | grep __pk_boot | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin delete .fixed
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin delete .toc
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin delete .fixedtoc
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_MEASUREMENT_NAME).bin delete .strings

ifeq ($(img), seeprom)
all:
	$(MAKE) boot_branch
ifeq ($(MEASUREMENT_IMAGE_SUPPORTED), 1)
	$(MAKE) measurement_branch
endif
ifeq ($(VERIFICATION_IMAGE_SUPPORTED), 1)
	$(MAKE) verification_branch
endif
endif
## TODO:: Look into all pre linked info for verification image
pre_verify_image: verify_xml ppe_trace_bin buildInfo linker_verify

# Build the verification subdirectories files
build_verify_image: $(VERIFICATION_SUBDIRS)

$(VERIFICATION_SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $@ -f Makefile

linker_verify:
	@mkdir -p $(OBJDIR)/build
	$(C1)$(CPP) -I $(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/$(IN_VERIFICATION_LINK_SCRIPT) -o $(OUT_VERIFICATION_LINK_SCRIPT)

VERIFICATION_LIB_DIRS += -L$(OBJDIR)/fapi2_measurement
VERIFICATION_LLIBS += -lfapi2measurement

VERIFICATION_LIB_DIRS += -L$(OBJDIR)/import_hwp_mk/sbem_hwp
VERIFICATION_LLIBS += -lsbem_hwp

output_verify_image:
	$(C2) "Generating $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).out"
	$(C1)$(LD) -e __system_reset -n -T$(OUT_VERIFICATION_LINK_SCRIPT) -Map $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).map \
  -Bstatic -o $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).out $(BUILDDATA_OBJDIR)/$(IMAGE_BASE_VERIFICATION_HEADER).o  $(VERIFICATION_LIB_DIRS) \
					   --start-group $(VERIFICATION_LLIBS) --end-group --gc-sections
	$(C1)./parsAndCutElf.py $(IMAGE_SUFFIX) $(IMG_DIR) verification
	$(C1)$(OBJDUMP) -S $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).out > $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).dis

verify_normalize:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin normalize

verify_symbols:
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).out > $(IMG_DIR)/$(VERIFICATION_SYMBOLS_NAME)

verify_report: $(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin report > $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).rpt

verify_buildtag:
	./updateBuildTag.py $(IPL_IMAGE_TOOL) $(IMG_DIR) $(IMAGE_VERIFICATION_NAME)

# collect all of the trace hash files for verification image into a single sbeVerifyStringFile
verify_tracehash:
	$(THASH) -c -d $(OBJDIR) -s $(IMG_DIR)/$(SBE_VERIFICATION_STRINGFILE_NAME)

verify_xml: $(M_FAPI_RC)

$(M_FAPI_RC): $(IMPORT_TOOLS_DIR)/parseErrorInfo.pl $(HWPERR_SRCDIR)/p10_sbe_spi_cmd_errors.xml
	$(M_FAPI_TOOLS_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=$(GENFILES_DIR) $(HWPERR_SRCDIR)/p10_sbe_spi_cmd_errors.xml

### TODO :: Bellow L1_LoaderAddr will update once we get right L1 Loder File
add_VerifyLoaderAddr:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).out | grep _base_origin | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin set kernelAddr 0x`nm $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).out | grep __pk_boot | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin delete .fixed
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin delete .toc
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin delete .fixedtoc
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin delete .strings

boot_branch:
	$(MAKE) $(OBJDIR)
	$(MAKE) attribute_parse
	$(MAKE) pre_sbe_image
	$(MAKE) build_sbe_image
	$(MAKE) output_sbe_image
	$(MAKE) normalize
	$(MAKE) appendloader
	$(MAKE) compress
	$(MAKE) defaultset
	$(MAKE) add_BootLoaderAddr
	$(MAKE) symbols tracehash buildtag

add_BootLoaderAddr:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep __l1Loader | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set L2_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_LOADER_NAME).out | grep __l2_loader_setup | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set kernelAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep __pk_boot | cut -f 1 -d " "`
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set dataAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep _data_origin | cut -f 1 -d " "`

ifeq ($(img), pibmem)
all:
	$(MAKE) $(OBJDIR)
	$(MAKE) attribute_parse
	$(MAKE) pre_sbe_image
	$(MAKE) build_sbe_image
	$(MAKE) output_sbe_image
	$(MAKE) normalize
	$(MAKE) defaultset
	$(MAKE) add_LoaderAddr
	$(MAKE) symbols tracehash buildtag
	$(MAKE) report

add_LoaderAddr:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin set L1_LoaderAddr 0x`nm $(IMG_DIR)/$(IMAGE_NAME).out | grep __vectors | cut -f 1 -d " "`
endif

measurement_branch:
	$(MAKE) $(OBJDIR)
	$(MAKE) pre_measure_image
	$(MAKE) build_measure_image
	$(MAKE) output_measure_image
	$(MAKE) measure_normalize
	$(MAKE) add_MeasureLoaderAddr
#	$(MAKE) measure_symbols measure_buildtag
	$(MAKE) measure_symbols
	$(MAKE) measure_tracehash
	$(MAKE) measure_report

verification_branch:
	$(MAKE) $(OBJDIR)
	$(MAKE) pre_verify_image
	$(MAKE) build_verify_image
	$(MAKE) output_verify_image
	$(MAKE) verify_normalize
	$(MAKE) verify_compress
	$(MAKE) add_VerifyLoaderAddr
#	$(MAKE) verify_symbols verify_buildtag
	$(MAKE) verify_symbols
	$(MAKE) verify_tracehash
	$(MAKE) verify_report
	$(MAKE) appendverification
	$(MAKE) report

ifneq ($(project), $(filter $(project), power_dft))
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
endif

.PHONY: all normalize compress verify_compress defaultset appendloader appendverification add_BootLoaderAddr \
  add_MeasureLoaderAddr add_LoaderAddr add_VerifyLoaderAddr symbols report \
		appendoverrides xml security tracehash topfixedheaders \
		tar install
#TODO add symbols for all images (pibmem/boot_seeprom/measurement_seeprom)
ifeq ($(img), $(filter $(img), seeprom boot))
symbols:
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_LOADER_NAME).out >> $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif
ifeq ($(img), pibmem)
symbols:
	$(C1)$(NM) $(IMG_DIR)/$(IMAGE_NAME).out > $(IMG_DIR)/$(SBE_SYMBOLS_NAME)
endif

report: $(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin report > $(IMG_DIR)/$(IMAGE_NAME).rpt

# Create build Info file
buildInfo:
	python buildInfo.py $(GENFILES_DIR)

buildtag:
	./updateBuildTag.py $(IPL_IMAGE_TOOL) $(IMG_DIR) $(IMAGE_NAME)


#Create an obj directory if needed
$(LINK_OBJS) $(OBJS) $(OBJS:.o=.d)

ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_pervasive_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/system_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/unit_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_ipl_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_sbe_load_bootloader_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_clock_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_freq_attributes.xml
#ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/core_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_nest_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_plat_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_hwp_attributes.xml
ATTRFILES += $(BASE_FAPI2_DIR)/xml/attribute_info/chip_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_chip_ec_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_hcode_image_build_attributes.xml
#ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/memory_mcs_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_bars_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_memory_bars_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_runn_attributes.xml

# Support for additional attributes for DFT, without affecting mainline collection
ifeq ($(project), $(filter $(project), power_dft))
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_dft_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p10_contained_attributes.xml
SBEATTRFILE = $(IMPORT_XML_DIR)/attribute_info/p10_dft_sbe_attributes.xml
else
SBEATTRFILE = $(IMPORT_XML_DIR)/attribute_info/p10_sbe_attributes.xml
endif

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(IMG_DIR)
	mkdir -p $(GENFILES_DIR)
	mkdir -p $(IMPORT_OBJDIR)

attribute_parse:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl $(GENFILES_DIR) $(SBEATTRFILE) $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(GENFILES_DIR) $(SBEATTRFILE) $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(PLAT_FAPI2_DIR) --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(SBEATTRFILE) $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PLAT_FAPI2_DIR) $(SBEATTRFILE) $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(PLAT_FAPI2_DIR) --inc $(GENFILES_DIR) --src $(GENFILES_DIR)

# Build the subdirectories
$(SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $@ -f Makefile

$(SEEPROM_SUBDIRS): ppe_trace_bin
	$(C1)$(MAKE) -I $(INCLUDES) -C $(subst targetseeprom_,, $@) -f Makefile img_mem=seeprom

#Build the SBE XIP Tools
$(SBE_TOOLS): $(IPL_IMAGE_TOOL)
	$(MAKE) -I $(INCLUDES) -C $(TOOLS_IMAGE_DIR) -f Makefile

# Build the P9-XIP Tool
# .PARALLEL = Forces Parallel
# .NOTPARALLEL = forces serial execution of makefile target..
#.NOTPARALLEL: $(IPL_IMAGE_TOOL)
$(IPL_IMAGE_TOOL):
	$(MAKE) -I $(INCLUDES) -C $(IPL_IMAGE_BUILD_DIR) -f Makefile BINDIR=$(P10_IPL_IMAGE_BINDIR) CXX_FLAGS=-D__PPE__=1
#$(MAKE) -I $(IMAGEPROCS_TOOL_DIR) -C $(IPL_IMAGE_BUILD_DIR) -f Makefile BINDIR=$(P10_IPL_IMAGE_BINDIR) CXX_FLAGS=-D__PPE__=1
	cp -f $(IPL_IMAGE_TOOL) $(IMG_DIR)/
	cp -f $(SBE_SRC_DIR)/tools/scripts/signSbeImage $(IMG_DIR)/
	cp -f $(SBE_SRC_DIR)/tools/scripts/signSbeImage_cronusWrap $(IMG_DIR)/

normalize:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin normalize

compress: $(IMG_DIR)/$(IMAGE_NAME).bin
	$(BOOT_SRCDIR)/sbeCompression.py -l $(IMG_DIR) -i $(IMAGE_NAME).bin -s "base"
	$(BOOT_SRCDIR)/sbeCompression.py -l $(IMG_DIR) -i $(IMAGE_NAME).bin -s "data"

verify_compress: $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin
	$(BOOT_SRCDIR)/sbeCompression.py -l $(IMG_DIR) -i $(IMAGE_VERIFICATION_NAME).bin -s "base"

defaultset:
	$(TOOLS_ATTR_DIR)/ppeSetFixed.pl $(IMG_DIR) $(IMG_DIR)/$(IMAGE_NAME).bin $(SBEATTRFILE) $(ATTRFILES)

# Build the trace utilities
ppe_trace_bin:
	$(MAKE) -I $(INCLUDES) -C $(PPETRACEPP_DIR) -f Makefile #$(PK trace util)

# generate whitelist and blacklist security algorithm
security: $(OBJDIR)
	python2 $(SECURITY_SRC_DIR)/securityRegListGen.py -f $(SECURITY_LIST) -o $(GENFILES_DIR)

# Build hwp_error_info.H.  If the script fails then print the contents of
# the header and then delete whatever garbage the script left to force it to
# be built again.
#

xml: $(FAPI_RC)

$(FAPI_RC): $(IMPORT_TOOLS_DIR)/parseErrorInfo.pl $(ERROR_XML_FILES)
	$(IMPORT_TOOLS_DIR)/parseErrorInfo.pl --local-ffdc --output-dir=$(GENFILES_DIR) $(ERROR_XML_FILES)

$(IMG_DIR)/fixed.bin: $(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin extract .fixed $(IMG_DIR)/fixed.bin

appendloader:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin append .baseloader $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin

appendverification:
	$(IPL_IMAGE_TOOL) $(IMG_DIR)/$(IMAGE_NAME).bin append .sb_verification $(IMG_DIR)/$(IMAGE_VERIFICATION_NAME).bin

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
