# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/security/algo/crypto/test/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2024
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

export VPATH = ../

# Define Target
TARGET = dilithium

# Define the compiler and flags
LIBS = -lm
CC = gcc
CFLAGS = -g -I. -I../ # -Wall
# MACROS = -D __PPE__

MACROS =  -DNO_CRYSTALS_CIP
MACROS += -DNO_CRYSTALS_KEX
MACROS += -DNO_DILITHIUM_SIGN
MACROS += -DNO_MLDSA_SIGN
MACROS += -DNO_DILITHIUM_VERIFY

# Define the directories
BUILD_DIR = build

# Define the target executable
default: $(TARGET)

# List of specific source files
SRC_C_FILES =  main.C
SRC_C_FILES += ../mlca.C
SRC_C_FILES += ../keccak.C

# Generate corresponding object files
OBJECTS = $(patsubst %.C, $(BUILD_DIR)/%.o, $(notdir $(SRC_C_FILES)))
HEADERS = $(wildcard ../*.h) $(wildcard ../*.H)

# Linking the object files
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

# Compiling .c files to .o files
$(BUILD_DIR)/%.o: %.C $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(MACROS) -c -lstdc++ $< -o $@

# .PRECIOUS directive to keep the TARGET and OBJ_FILES
.PRECIOUS: $(TARGET) $(OBJECTS)

# Clean rule
.PHONY: default all clean
clean:
	rm -rf $(TARGET)
	rm -rf $(BUILD_DIR)