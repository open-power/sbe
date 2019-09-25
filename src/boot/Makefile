# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/boot/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2019
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
export SUB_OBJDIR = /boot

include img_defs.mk
include bootfiles.mk

INCLUDES += -I$(BUILD_DIR)

#This will generate L1 Boot *.o objects
#this gets directly appended into Boot Seeprom
L1_BOOT_OBJS := $(addprefix $(OBJDIR)/, $(L1_BOOT_OBJECTS))

#This will generate L1 Measurement *.o objects
#this gets directly appended into measurement seeprom
#when MEASUREMENT_IMAGE_SUPPORTED is set with 1 to enable this.
ifeq ($(MEASUREMENT_IMAGE_SUPPORTED), 1)
L1_MEASUREMENT_OBJS := $(addprefix $(OBJDIR)/, $(L1_MEASUREMENT_OBJECTS))
endif

#L1 loader doesn't need linking since the *.o directly gets appended to seeprom image (boot as well as measurement)
LINK_SCRIPT_LOADER = $(addprefix $(OBJDIR)/, linkscriptloader)
LINK_SCRIPT_OTPROM =  $(addprefix $(OBJDIR)/, linkscriptotprom)

ifneq ($(img), pibmem)
all: $(L1_BOOT_OBJS) $(L1_MEASUREMENT_OBJS) \
     $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin $(IMG_DIR)/$(IMAGE_OTPROM_NAME).bin
endif

$(L1_BOOT_OBJS) $(L1_BOOT_OBJS:.o=.d): | $(OBJDIR)
$(L1_MEASUREMENT_OBJS) $(L1_MEASUREMENT_OBJS:.o=.d): | $(OBJDIR)

# Loader Objects
LOADER_OBJS := $(addprefix $(OBJDIR)/, $(BASE_LOADER_OBJECTS))
#Otprom Loader Objects
OTPROM_LOADER_OBJS := $(addprefix $(OBJDIR)/, $(OTPROM_LOADER_OBJECTS))
# ---- Otprom Image  ------
$(IMG_DIR)/$(IMAGE_OTPROM_NAME).bin $(IMG_DIR)/$(IMAGE_OTPROM_NAME).dis: $(IMG_DIR)/$(IMAGE_OTPROM_NAME).out
	$(OBJCOPY) -O binary  $< $(IMG_DIR)/$(IMAGE_OTPROM_NAME).bin `/usr/bin/nm $(IMG_DIR)/$(IMAGE_OTPROM_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(IMG_DIR)/$(IMAGE_OTPROM_NAME).dis

#create a linked ELF executable
$(IMG_DIR)/$(IMAGE_OTPROM_NAME).out: $(OTPROM_LOADER_OBJS) $(LINK_SCRIPT_OTPROM)
	$(LD) -e l2_loader -T$(LINK_SCRIPT_OTPROM) -Map $(IMG_DIR)/$(IMAGE_OTPROM_NAME).map -Bstatic -o $(IMG_DIR)/$(IMAGE_OTPROM_NAME).out $(OTPROM_LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_OTPROM): $(LINKER_DIR)/linkotprom.cmd
	$(CPP) -I$(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/linkotprom.cmd -o $(LINK_SCRIPT_OTPROM)

# ---- Loader Image for L2 Loader of Boot Seeprom  ------
$(IMG_DIR)/$(IMAGE_LOADER_NAME).bin $(IMG_DIR)/$(IMAGE_LOADER_NAME).dis: $(IMG_DIR)/$(IMAGE_LOADER_NAME).out
	$(OBJCOPY) -O binary  $< $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin --pad-to 0x`/usr/bin/nm $(IMG_DIR)/$(IMAGE_LOADER_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(IMG_DIR)/$(IMAGE_LOADER_NAME).dis

#create a linked ELF executable for L2 Loader of Boot Seeprom
$(IMG_DIR)/$(IMAGE_LOADER_NAME).out: $(LOADER_OBJS) $(LINK_SCRIPT_LOADER)
	$(LD) -e l2_loader -T$(LINK_SCRIPT_LOADER) -Map $(IMG_DIR)/$(IMAGE_LOADER_NAME).map -Bstatic -o $(IMG_DIR)/$(IMAGE_LOADER_NAME).out $(LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_LOADER): $(LINKER_DIR)/linkloader.cmd
	$(CPP) -I$(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/linkloader.cmd -o $(LINK_SCRIPT_LOADER)

PIBMEM_REPAIR_OBJDIR = $(OBJDIR)/pibmem_repair/scom_repair_p10

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(PIBMEM_REPAIR_OBJDIR)

.PHONY: clean all

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(L1_BOOT_OBJS:.o=.d)
include $(L1_MEASUREMENT_OBJS:.o=.d)
endif
