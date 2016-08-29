# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/boot/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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

OBJS := $(addprefix $(OBJDIR)/, $(BOOT_OBJECTS))
BOOTOBJS += $(OBJS)
LINK_SCRIPT_LOADER = $(addprefix $(OBJDIR)/, linkscriptloader)
LINK_SCRIPT_OTPROM =  $(addprefix $(OBJDIR)/, linkscriptotprom)

all: $(OBJS) \
     $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin $(IMG_DIR)/$(IMAGE_OTPROM_NAME).bin

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

# Loader Objects
LOADER_OBJS := $(addprefix $(OBJDIR)/, $(BASE_LOADER_OBJECTS))
#Otprom Loader Objects
OTPROM_LOADER_OBJS := $(addprefix $(OBJDIR)/, $(OTPROM_LOADER_OBJECTS))
# ---- Otprom Image  ------
$(IMG_DIR)/$(IMAGE_OTPROM_NAME).bin $(OBJDIR)/$(IMAGE_OTPROM_NAME).dis: $(OBJDIR)/$(IMAGE_OTPROM_NAME).out
	$(OBJCOPY) -O binary  $< $(IMG_DIR)/$(IMAGE_OTPROM_NAME).bin `/usr/bin/nm $(OBJDIR)/$(IMAGE_OTPROM_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_OTPROM_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_OTPROM_NAME).out: $(OTPROM_LOADER_OBJS) $(LINK_SCRIPT_OTPROM)
	$(LD) -e base_loader -T$(LINK_SCRIPT_OTPROM) -Map $(OBJDIR)/$(IMAGE_OTPROM_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_OTPROM_NAME).out $(OTPROM_LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_OTPROM): $(LINKER_DIR)/linkotprom.cmd
	$(CPP) -I$(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/linkotprom.cmd -o $(LINK_SCRIPT_OTPROM)

# ---- Loader Image  ------
$(IMG_DIR)/$(IMAGE_LOADER_NAME).bin $(OBJDIR)/$(IMAGE_LOADER_NAME).dis: $(OBJDIR)/$(IMAGE_LOADER_NAME).out
	$(OBJCOPY) -O binary  $< $(IMG_DIR)/$(IMAGE_LOADER_NAME).bin --pad-to 0x`/usr/bin/nm $(OBJDIR)/$(IMAGE_LOADER_NAME).out | grep "A _loader_end" | cut -d " " -f 1`
	$(OBJDUMP) -S $< > $(OBJDIR)/$(IMAGE_LOADER_NAME).dis

#create a linked ELF executable
$(OBJDIR)/$(IMAGE_LOADER_NAME).out: $(LOADER_OBJS) $(LINK_SCRIPT_LOADER)
	$(LD) -e base_loader -T$(LINK_SCRIPT_LOADER) -Map $(OBJDIR)/$(IMAGE_LOADER_NAME).map -Bstatic -o $(OBJDIR)/$(IMAGE_LOADER_NAME).out $(LOADER_OBJS)

# pass the link command file through the C preprocessor to evaluate macros
# and remove comments
$(LINK_SCRIPT_LOADER): $(LINKER_DIR)/linkloader.cmd
	$(CPP) -I$(INCLUDES) -E -x c++ -P $(DEFS) $(LINKER_DIR)/linkloader.cmd -o $(LINK_SCRIPT_LOADER)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean all

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif


