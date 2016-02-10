

#Use this to switch between the new pp42 compiler
#  and the 405 compiler with P2P backend
#  Enable for P2P backend
#export P2P_ENABLE = 1

#remove this once we have a real compiler
export SBEFW_ENABLE = 1

# Force a non-interrupt variant of PK for FAPI2 testing.  This allows the test to run
# on any of the PPE SIMICS models.
# This must be before the inclusion of img_defs.mk.
ifdef FAPI2_TEST
PPE_TYPE := ppe
endif

#Pull in the definitions that affect all makefiles for this image
include img_defs.mk

#Pull in object file names for the top directory
include topfiles.mk

ifdef P2P_ENABLE
include $(P2P_SRCDIR)/p2pfiles.mk

#Pull in PPE complier libraries .
# Sets up the following variables:
#   $(GCCLIBS)  ->  Libary objects to be linked
#   $(LIB_DIRS) ->  Include directories
include ../../include/ppe_compiler.mk
endif

PK_MAKE_DIR := $(PK_SRCDIR)/$(PPE_TYPE)
LIB_DIRS += -L$(OBJDIR)/pk
PKLIB := $(OBJDIR)/pk/libpk.a
LLIBS += -lpk

SBEFW_MAKE_DIR := ../sbefw
LIB_DIRS += -L$(OBJDIR)/sbefw
SBEFWLIB := $(OBJDIR)/sbefw/libsbefw.a
LLIBS += -lsbefw

#SAMPLE_MAKE_DIR := ../sample
#LIB_DIRS += -L$(OBJDIR)/sample
#SAMPLELIB := $(OBJDIR)/sample/libsample.a
#LLIBS += -lsample

# FAPI2 library
FAPI2_MAKE_DIR := $(PLAT_FAPI2_DIR)
LIB_DIRS += -L$(OBJDIR)/fapi2
FAPI2LIB := $(OBJDIR)/fapi2/libfapi2.a
LLIBS += -lfapi2

ifdef P2P_ENABLE
LIB_DIRS += -L$(OBJDIR)/p2p
P2PLIB := $(OBJDIR)/p2p/libp2p.a
LINK_OBJS += $(P2PLIB)
LLIBS += -lp2p
else
LIB_DIRS += -L$(GCC-TOOL-PATH)/libgcc
LLIBS += -lmath -leabi -lc
endif

# HWP LIB library
HWPLIB_MAKE_DIR := $(HWPLIB_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/lib
HWPLIB := $(OBJDIR)/libcommon.a
LLIBS += -lcommon
include $(HWPERR_SRCDIR)/hwpErrors.mk

# Common Cache HWP Exit library
CACHE_MAKE_DIR := $(CACHE_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/cache
CACHELIB := $(OBJDIR)/cache/libcache.a
LLIBS += -lcache

# Common Core libraries
CORE_MAKE_DIR := $(CORE_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/core
CORELIB := $(OBJDIR)/core/libcore.a
LLIBS += -lcore

# Common Perv libraries
PERV_MAKE_DIR := $(PERV_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/perv
PERVLIB := $(OBJDIR)/perv/libperv.a
LLIBS += -lperv

# Common Nest libraries
NEST_MAKE_DIR := $(NEST_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/nest
NESTLIB := $(OBJDIR)/nest/libnest.a
LLIBS += -lnest

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
SIMICS_PPE_TAR_FILES += $(OBJDIR)/../../tools/ppetracepp/ppe2fsp
SIMICS_PPE_TAR_FILES += $(OBJDIR)/trexStringFile
SIMICS_BB_TAR_FILES += $(OBJDIR)/../build/simics/*

# Common Pm libraries
PM_MAKE_DIR := $(PM_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/pm
PMLIB := $(OBJDIR)/pm/libpm.a
LLIBS += -lpm

SBE_TOOLS := $(TOOLS_IMAGE_DIR)/sbe_default_tool

P9_XIP_TOOL := $(P9_XIP_BINDIR)/p9_xip_tool

LINK_OBJS = $(OBJS) $(SBEFWLIB) $(PKLIB) $(FAPI2LIB) $(CACHELIB) $(CORELIB) $(HWPLIB) $(P2PLIB) $(PERVLIB) $(NESTLIB) $(PMLIB)

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


ifndef FAPI2_TEST
# This is the production build of the SEEPROM
#create a linked ELF executable
$(OBJDIR)/$(IMAGE_SEEPROM_NAME).out: buildInfo $(LINK_OBJS) $(LINK_SCRIPT_SEEPROM)
	@echo ">>>>> Production Build <<<<<<"
	$(LD) -n -T$(LINK_SCRIPT_SEEPROM) -Map $(OBJDIR)/$(IMAGE_SEEPROM_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out  $(OBJS) $(LIB_DIRS)  --start-group $(LLIBS) --end-group  -leabi -lmath -lc
else
# This a special build of the SEEPROM image that includes the PK to allow for test threads to be used.
# create a linked ELF executable
$(OBJDIR)/$(IMAGE_SEEPROM_NAME).out: buildInfo $(LINK_OBJS) $(LINK_SCRIPT_SEEPROM)
	@echo ">>>>> Special FAPI2 Test Build <<<<<<"
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SEEPROM) -Map $(OBJDIR)/$(IMAGE_SEEPROM_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out $(LIB_DIRS) $(OBJS) --start-group $(LLIBS) --end-group

endif

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SEEPROM): linkseeprom.cmd
	$(CPP) -I. -E -x c++ -P $(DEFS) linkseeprom.cmd -o $(LINK_SCRIPT_SEEPROM)

# ---- PIBMEM Image  ------
#This removes all unecessary headers from the ELF executable
$(OBJDIR)/$(IMAGE_SBE_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).dis: $(OBJDIR)/$(IMAGE_SBE_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_SBE_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_SBE_NAME).out | grep "A _sbe_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_SBE_NAME).dis
	$(NM)  $< > $(OBJDIR)/$(IMAGE_SBE_NAME).syms

# create a linked ELF executable
$(OBJDIR)/$(IMAGE_SBE_NAME).out: buildInfo $(OBJDIR)/base_sbe_fixed.o $(LINK_OBJS) $(LINK_SCRIPT_SBE)
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

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_OTPROM): linkotprom.cmd
	$(CPP) -I. -E -x c++ -P $(DEFS) linkotprom.cmd -o $(LINK_SCRIPT_OTPROM)


# ---- Loader Image  ------
$(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).dis: $(OBJDIR)/$(IMAGE_LOADER_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_LOADER_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_LOADER_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_LOADER_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_LOADER_NAME).out: $(LOADER_OBJS) $(LINK_SCRIPT_LOADER)
	$(LD) -e base_loader -T$(LINK_SCRIPT_LOADER) -Map $(OBJDIR)/$(IMAGE_LOADER_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_LOADER_NAME).out $(LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_LOADER): linkloader.cmd
	$(CPP) -I. -E -x c++ -P $(DEFS) linkloader.cmd -o $(LINK_SCRIPT_LOADER)

# Remove the "all" rule if FAPI2_TEST as the SBE FW is incompatible with the
# option of PK enabled
ifndef FAPI2_TEST
all: seeprom $(OBJDIR)/$(IMAGE_SBE_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(SBE_TOOLS) appendloader
install: all
	@echo "Installing simics.tar"
	@rm -rf $(OBJDIR)/simics-tar-dir
	@mkdir $(OBJDIR)/simics-tar-dir
	@cp  $(SIMICS_PPE_TAR_FILES) $(OBJDIR)/simics-tar-dir
	@cp  $(SIMICS_BB_TAR_FILES) $(OBJDIR)/simics-tar-dir
	@cd  $(OBJDIR)/simics-tar-dir && tar -cvf simics.tar * && mv simics.tar $(OBJDIR) && cd ..
	@rm -rf $(OBJDIR)/simics-tar-dir
	@echo "Generated simics.tar in Sbe Obj Directory"
else
all:
	@echo "Makefile option error:  ALL is not compatible with FAPI2_test"
endif

.PHONY: all normalize defaultset appendloader add_L1_LoaderAddr add_L2_LoaderAddr add_kernelAddr symbols appendoverrides report

all: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(OBJDIR)/$(IMAGE_OTPROM_NAME).bin $(SBE_TOOLS) normalize defaultset $(OBJDIR)/fixed.bin appendloader appendoverrides add_L1_LoaderAddr add_L2_LoaderAddr add_kernelAddr symbols tracehash buildinfo report


buildinfo: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_date `date +%Y%m%d`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_time `date +%H%M`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_user `id -un`
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set build_host `hostname`

add_L1_LoaderAddr: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set L1_LoaderAddr 0x`nm $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out | grep __pmLoader | cut -f 1 -d " "`

add_L2_LoaderAddr: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set L2_LoaderAddr 0x`nm $(OBJDIR)/$(IMAGE_LOADER_NAME).out | grep __base_loader_setup | cut -f 1 -d " "`

add_kernelAddr: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_LOADER_NAME).out
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin set kernelAddr 0x`nm $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out | grep __pk_boot | cut -f 1 -d " "`

symbols: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out $(OBJDIR)/$(IMAGE_LOADER_NAME).out $(OBJDIR)/$(IMAGE_SBE_NAME).out
	$(NM) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out > $(OBJDIR)/$(SBE_SYMBOLS_NAME)
	$(NM) $(OBJDIR)/$(IMAGE_LOADER_NAME).out >> $(OBJDIR)/$(SBE_SYMBOLS_NAME)

# this target is broken, since the image report should be created
# when the xip image has been finalised
seeprom: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(SBE_TOOLS) normalize defaultset $(OBJDIR)/fixed.bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin report > $(OBJDIR)/$(IMAGE_SEEPROM_NAME).rpt

report: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin report > $(OBJDIR)/$(IMAGE_SEEPROM_NAME).rpt

# Create build Info file
buildInfo:
	python buildInfo.py


#Create an obj directory if needed
$(LINK_OBJS) $(OBJS) $(OBJS:.o=.d) $(OBJDIR)/base_sbe_fixed.o $(OBJDIR)/base_sbe_fixed.d: | $(OBJDIR)

ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pg_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pervasive_attributes.xml
# TODO via RTC 142708
# Workaound for ATTR_CHIP_UNIT_POS. Remove Mirror_WA_attributes.xml once fapi
# support is in.
ATTRFILES += $(IMPORT_XML_DIR)/../../../../../sbe/image/Mirror_WA_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/p9_sbe_load_bootloader_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/core_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/nest_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/pm_plat_attributes.xml

$(OBJDIR):
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl -path $(TOOLS_ATTR_DIR)/src -inc $(PPE_FAPI2_DIR)/include -src $(PPE_FAPI2_DIR)/src
	$(TOOLS_ATTR_DIR)/ppeCreateIfAttrService.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)
	mkdir -p $(OBJDIR)

.PHONY: clean topfixedheaders $(PKLIB) $(P2PLIB) $(PPELIB) $(FAPI2LIB) $(CACHELIB) $(CORELIB) $(PERVLIB) $(NESTLIB) $(PMLIB) $(HWPLIB) buildInfo

#errxml: topfixedheaders attrids platattr attrserv

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(TOOLS_ATTR_DIR)/src --inc $(PPE_FAPI2_DIR)/include --src $(PPE_FAPI2_DIR)/src

attrserv:
	$(TOOLS_ATTR_DIR)/ppeCreateIfAttrService.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)


#Build macro-specific kernel code
$(PKLIB):
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PK_MAKE_DIR) -f Makefile

#Build SBE firmware
$(SBEFWLIB):
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(SBEFW_MAKE_DIR) -f Makefile

#Build the code that is common for all processors (PPEs and 405)
$(PPELIB):
	@echo "Processing ppelibmakefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(LIB_MAKE_DIR) -f Makefile

#Build the cache exit code shared with the SBE
$(CACHELIB):
	@echo "Processing cache makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(CACHE_MAKE_DIR) -f Makefile

#Build the core exit code shared with the SBE
$(CORELIB):
	@echo "Processing core makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(CORE_MAKE_DIR)  -f Makefile

#Build the perv code
$(PERVLIB):
	@echo "Processing perv makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PERV_MAKE_DIR) -f Makefile

#Build the nest code
$(NESTLIB):
	@echo "Processing nest makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(NEST_MAKE_DIR) -f Makefile

#Build the pm code
$(PMLIB):
	@echo "Processing pm makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PM_MAKE_DIR) -f Makefile

#Build the HWP lib procedures
$(HWPLIB):
	@echo "Processing HWP lib makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(HWPLIB_MAKE_DIR) -f Makefile

#Build the FAPI2 library
$(FAPI2LIB):
	@echo "Processing fapi2 makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(FAPI2_MAKE_DIR)/src -f Makefile

ifdef P2P_ENABLE
$(P2PLIB):
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(P2P_SRCDIR) -f Makefile
endif

#Build the SBE XIP Tools
$(SBE_TOOLS):
	$(MAKE) -C $(TOOLS_IMAGE_DIR) -f Makefile

# Build the P9-XIP Tool
$(P9_XIP_TOOL):
	$(MAKE) -I $(P9_XIP_SRCDIR) -C $(P9_XIP_SRCDIR) -f Makefile #$(P9_XIP_TOOL)

normalize: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin normalize

defaultset: $(SBE_TOOLS) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin normalize
	$(TOOLS_IMAGE_DIR)/ppeSetFixed.pl $(TOOLS_IMAGE_DIR) $(BASE_OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(IMPORT_XML_DIR)/attribute_info/p9_sbe_attributes.xml $(ATTRFILES)

# Build hwp_error_info.H.  If the script fails then print the contents of
# the header and then delete whatever garbage the script left to force it to
# be built again.
#
# -*- HACK -*- Use local version of parseErrorInfo.pl until FW team
# updates the standard version to work in assembly.  The source for this
# script is : $(FAPI)/capi/scripts/parseErrorInfo.pl

.PHONY : xml

xml: $(FAPI_RC)

$(FAPI_RC): $(TOOLS_ATTR_DIR)/parseErrorInfo.pl $(ERROR_XML_FILES)
	$(TOOLS_ATTR_DIR)/parseErrorInfo.pl --empty-ffdc-classes --output-dir=. $(ERROR_XML_FILES)
# 	if [ $$? -ne 0 ]; then \
# 	echo "Build of hwp_error_info.H failed; Current contents :"; \
# 	cat hwp_error_info.H; \
# 	rm -f hwp_error_info.H; \
# 	exit 1; fi
# The above is commented out as the current version of make produces errors
# This was taken from the P8 SBE Makefile which worked.

$(OBJDIR)/fixed.bin: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin extract .fixed $(BASE_OBJDIR)/fixed.bin


appendloader: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).bin
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin append .baseloader $(OBJDIR)/$(IMAGE_LOADER_NAME).bin

appendoverrides: $(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	dd if=/dev/zero of=$(OBJDIR)/overrides.bin count=1 bs=2048
	$(P9_XIP_TOOL) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin append .overrides $(OBJDIR)/overrides.bin

#/afs/bb/proj/cte/tools/ppetools/prod/powerpc-eabi/bin/objcopy -O binary --only-section=.fixed $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out $(OBJDIR)/fixed.bin

# collect all of the trace hash files for this image into a single trexStringFile
.PHONY : tracehash
tracehash:
	mkdir -p $(OBJDIR)
	$(THASH) -c -d $(OBJDIR) -s $(OBJDIR)/trexStringFile

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)
	rm -f $(TOP-FIXED-HEADERS)
	$(MAKE) -C $(TOOLS_IMAGE_DIR) -f Makefile clean
	$(MAKE) -C $(P9_XIP_SRCDIR) -f Makefile clean
	rm -f *.dump
	rm -f hwp_return_codes.H hwp_error_info.H hwp_ffdc_classes.H collect_reg_ffdc.C set_sbe_error.H sbe_build_info.H
	rm -f $(GEN-CREATEATTRGETSETMACROS)
	rm -f $(GEN-PARSEATTRIBUTEINFO)
	rm -f $(GEN-CREATEIFATTRSERVICE)

dump:
	objdump -s $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out > $(IMAGE_SEEPROM_NAME).dump
	objdump -s $(OBJDIR)/$(IMAGE_SBE_NAME).out > $(IMAGE_SBE_NAME).dump

.PHONY : run_spgpe
# load and run the SBE SeeProm image in a GPE simics environment
run_spgpe: seeprom
	$(SIMICS_WS)/simics \
		-e '$$occ_gpe0_binary_to_load=$(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin;$$occ_gpe0_sym_tbl_file=$(OBJDIR)/$(IMAGE_SEEPROM_NAME).out' \
                modelsetup.simics


#Add dependencies to header files
ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
