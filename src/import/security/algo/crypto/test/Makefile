# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/security/algo/crypto/test/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2024
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
MACROS += -DNO_CRYSTALS_R3
MACROS += -DNO_DILITHIUM_R2_3
MACROS += -DNO_DILITHIUM_R2_4

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





# export SUB_OBJDIR = /sbefw/verification
# export VPATH = $(SBE_FW_DIR)/core:$(CRYPTO_DIR)

# OBJS = $(addprefix $(OBJDIR)/, $(VERIFICATION_OBJECTS))
# LIBNAME = libVerification

# include img_defs.mk
# include verificationfiles.mk

# $(LIBNAME).a: $(OBJS)
# 	$(AR) crs $(OBJDIR)/$(LIBNAME).a $(OBJDIR)/*.o

# .PHONY: clean verify
# verify: $(OBJS)

# $(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

# $(OBJDIR):
# 	mkdir -p $(OBJDIR)

# #clean the kernel directory first, then the application level clean
# clean:
# 	rm -fr $(OBJDIR)

# ifneq ($(MAKECMDGOALS),clean)
# 	include $(OBJS:.o=.d)
# endif






# # # Sample Makefile
# CC=gcc
# CFLAGS=-g -Wall
# LIBS=-lm

# # Source files
# SRCS=main.c mlca.c keccak.c
# # Object files
# OBJS=$(SRCS:.c=.o)

# # Executable name
# EXECUTABLE=main

# # Default target
# all: $(EXECUTABLE)

# # Linking step
# $(EXECUTABLE): $(OBJS)
# 	$(CC) $(OBJS) $(LIBS) -o $@

# # Compilation rules
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Clean target
# clean:
# 	rm -f $(OBJS) $(EXECUTABLE)




# VPATH = ../
# TARGET = dilithium

# CC = gcc
# CFLAGS = -g -I. -I../
# OBJ_DIR = build

# # Find all .c files in the current directory and directories specified in VPATH
# SRCS = $(wildcard *.C) $(wildcard ../*.C)

# # Convert source file paths to object file paths in the build directory
# OBJS = $(patsubst %.C, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# # Default target
# all: $(OBJ_DIR) $(TARGET)

# # Create build directory if it doesn't exist
# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# # Linking the object files
# $(TARGET): $(OBJS)
# 	$(CC) -o $@ $^

# # Compiling .c files to .o files
# $(OBJ_DIR)/%.o: %.c
# 	$(CC) $(CFLAGS) -c -lstdc++ $< -o $@

# # Clean up build files
# clean:
# 	rm -rf $(OBJ_DIR) $(TARGET)
