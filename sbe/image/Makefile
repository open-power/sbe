
#remove this once we have a real compiler
export P2P_ENABLE = 1

#remove this once we have a real compiler
export SBEFW_ENABLE = 1

#Pull in the definitions that affect all makefiles for this image
include img_defs.mk

#Pull in object file names for the top directory
include topfiles.mk

ifdef P2P_ENABLE
include $(P2P_SRCDIR)/p2pfiles.mk
endif

#Pull in PPE complier libraries .  
# Sets up the following variables:
#   $(GCCLIBS)  ->  Libary objects to be linked
#   $(LIB_DIRS) ->  Include directories 
include ../../include/ppe_compiler.mk


PK_MAKE_DIR := $(PK_SRCDIR)/$(PPE_TYPE)
LIB_DIRS += -L$(OBJDIR)/pk
PKLIB := $(OBJDIR)/pk/libpk.a
LLIBS += -lpk

SBEFW_MAKE_DIR := ../sbefw
LIB_DIRS += -L$(OBJDIR)/sbefw
SBEFWLIB := $(OBJDIR)/sbefw/libsbefw.a
#LLIBS += -lsbefw

SAMPLE_MAKE_DIR := ../sample
LIB_DIRS += -L$(OBJDIR)/sample
SAMPLELIB := $(OBJDIR)/sample/libsample.a
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
endif

# HWP LIB library
HWPLIB_MAKE_DIR := $(HWPLIB_SRCDIR)
LIB_DIRS += -L$(OBJDIR)/lib
HWPLIB := $(OBJDIR)/libcommon.a
LLIBS += -lcommon

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

SBE_TOOLS := $(TOOLS_IMAGE_DIR)/sbe_xip_tool $(TOOLS_IMAGE_DIR)/sbe_default_tool

LINK_OBJS = $(OBJS) $(SBEFWLIB) $(PKLIB) $(FAPI2LIB) $(CACHELIB) $(CORELIB) $(HWPLIB) $(P2PLIB) $(PERVLIB)

# Define the objects
OBJS := $(addprefix $(OBJDIR)/, $(TOP_OBJECTS))
LINK_OBJS += $(OBJS) $(GCCLIBS)

LINK_SCRIPT_SEEPROM = $(addprefix $(OBJDIR)/, linkscriptseeprom)
LINK_SCRIPT_SBE = $(addprefix $(OBJDIR)/, linkscriptsbe)
LINK_SCRIPT_LOADER = $(addprefix $(OBJDIR)/, linkscriptloader)


#default target is to make a binary application image
#This removes all unecessary headers from the ELF executable
$(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_SEEPROM_NAME).dis: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out | grep "A _seeprom_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_SEEPROM_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_SEEPROM_NAME).out: $(LINK_OBJS) $(LINK_SCRIPT_SEEPROM)
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SEEPROM) -Map $(OBJDIR)/$(IMAGE_SEEPROM_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out $(LIB_DIRS) $(OBJS)  --start-group  $(LLIBS) --end-group

#pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SEEPROM): linkseeprom.cmd
	$(CPP) -E -x c++ -P $(DEFS) linkseeprom.cmd -o $(LINK_SCRIPT_SEEPROM)



#default target is to make a binary pibmem image
#This removes all unecessary headers from the ELF executable
$(OBJDIR)/$(IMAGE_SBE_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).dis: $(OBJDIR)/$(IMAGE_SBE_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_SBE_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_SBE_NAME).out | grep "A _sbe_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_SBE_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_SBE_NAME).out: $(OBJDIR)/base_sbe_fixed.o $(LINK_OBJS) $(LINK_SCRIPT_SBE)
	$(LD) -e __system_reset  -T$(LINK_SCRIPT_SBE) -Map $(OBJDIR)/$(IMAGE_SBE_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_SBE_NAME).out $(LIB_DIRS) $(OBJDIR)/base_sbe_fixed.o  --start-group  $(SBEFWLIB) $(LLIBS) --end-group

#pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_SBE): linksbe.cmd
	$(CPP) -E -x c++ -P $(DEFS) linksbe.cmd -o $(LINK_SCRIPT_SBE)




$(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).dis: $(OBJDIR)/$(IMAGE_LOADER_NAME).out
	$(OBJCOPY) -O binary  $< $(OBJDIR)/$(IMAGE_LOADER_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_LOADER_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_LOADER_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_LOADER_NAME).out: $(OBJDIR)/base_loader.o $(LINK_OBJS) $(LINK_SCRIPT_LOADER)
	$(LD) -e base_loader -T$(LINK_SCRIPT_LOADER) -Map $(OBJDIR)/$(IMAGE_LOADER_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_LOADER_NAME).out $(LIB_DIRS) $(OBJDIR)/base_loader.o  --start-group  $(SBEFWLIB) $(LLIBS) --end-group

#pass the link command file through the C preprocessor to evaluate macros and remove comments
$(LINK_SCRIPT_LOADER): linkloader.cmd
	$(CPP) -E -x c++ -P $(DEFS) linkloader.cmd -o $(LINK_SCRIPT_LOADER)




all: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).bin $(SBE_TOOLS) normalize defaultset $(OBJDIR)/fixed.bin appendbase appendloader

#Create an obj directory if needed
$(LINK_OBJS) $(OBJS) $(OBJS:.o=.d) $(OBJDIR)/base_sbe_fixed.o $(OBJDIR)/base_sbe_fixed.d: | $(OBJDIR)

ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/perv_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/proc_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/ex_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/eq_attributes.xml
ATTRFILES += $(IMPORT_XML_DIR)/attribute_info/core_attributes.xml

$(OBJDIR):
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES)
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl -path $(TOOLS_ATTR_DIR)/src -inc $(PPE_FAPI2_DIR)/include -src $(PPE_FAPI2_DIR)/src
	$(TOOLS_ATTR_DIR)/ppeCreateIfAttrService.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES)
	mkdir -p $(OBJDIR)

.PHONY: clean topfixedheaders $(PKLIB) $(P2PLIB) $(PPELIB) $(FAPI2LIB) $(CACHELIB) $(CORELIB) $(PERVLIB) $(HWPLIB)

topfixedheaders:
	$(TOOLS_ATTR_DIR)/ppeParseProcSbeFixed.pl . $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES)

attrids:
	$(TOOLS_ATTR_DIR)/ppeParseAttributeInfo.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES)

platattr:
	$(TOOLS_ATTR_DIR)/ppeCreateAttrGetSetMacros.pl --path $(TOOLS_ATTR_DIR)/src --inc $(PPE_FAPI2_DIR)/include --src $(PPE_FAPI2_DIR)/src

attrserv:
	$(TOOLS_ATTR_DIR)/ppeCreateIfAttrService.pl $(PPE_FAPI2_DIR)/include $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES)


#Build macro-specific kernel code
$(PKLIB):
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PK_MAKE_DIR) -f Makefile

#Build macro-specific kernel code
$(SBEFWLIB):
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(SBEFW_MAKE_DIR) -f Makefile

sbefw:
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

#Build the comming HWP lib procedures
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

$(SBE_TOOLS):
	$(MAKE) -C $(TOOLS_IMAGE_DIR) -f Makefile

normalize: $(SBE_TOOLS) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(TOOLS_IMAGE_DIR)/sbe_xip_tool $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin normalize

defaultset: $(SBE_TOOLS) $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin normalize 
	$(TOOLS_IMAGE_DIR)/ppeSetFixed.pl $(TOOLS_IMAGE_DIR) $(BASE_OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(IMPORT_XML_DIR)/p9_ppe_attributes.xml $(ATTRFILES) 


$(OBJDIR)/fixed.bin: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin
	$(TOOLS_IMAGE_DIR)/sbe_xip_tool $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin extract .fixed $(BASE_OBJDIR)/fixed.bin


appendbase: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_SBE_NAME).bin
	$(TOOLS_IMAGE_DIR)/sbe_xip_tool $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin append .base $(OBJDIR)/$(IMAGE_SBE_NAME).bin



appendloader: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).bin
	$(TOOLS_IMAGE_DIR)/sbe_xip_tool $(OBJDIR)/$(IMAGE_SEEPROM_NAME).bin append .baseloader $(OBJDIR)/$(IMAGE_LOADER_NAME).bin

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
	rm -fr $(TOOLS_IMAGE_DIR)/bin/*
	rm -f *.dump


dump:
	objdump -s $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out > $(IMAGE_SEEPROM_NAME).dump
	objdump -s $(OBJDIR)/$(IMAGE_SBE_NAME).out > $(IMAGE_SBE_NAME).dump

# load and run the SBE image in a GPE simics environment
runseeprom: $(OBJDIR)/$(IMAGE_SEEPROM_NAME).out
	$(SIMICS_WS)/simics \
		-e '$$occ_gpe0_binary_to_load=$(OBJDIR)/$(IMAGE_SEEPROM_NAME).out' modelsetup.simics



# load and run the SBE image in a GPE simics environment
runsbe: $(OBJDIR)/$(IMAGE_SBE_NAME).out
	$(SIMICS_WS)/simics \
		-e '$$occ_gpe0_binary_to_load=$(OBJDIR)/$(IMAGE_SBE_NAME).out' modelsetup.simics



#Add dependencies to header files
ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif
