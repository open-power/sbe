# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/dilithiumalgotest/hwtestcase/Makefile $
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
# Define Target
TARGET = hw_dilithium

# Define the compiler and flags
LIBS = -lm
CC = gcc
CFLAGS = -g # -Wall
# MACROS = -D __PPE__

# Define the directories
BUILD_DIR = build

# List of specific source files
SRC_FILES =  main.c

# Define the target executable
default: $(TARGET)

# Generate corresponding object files
OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Rule to build the target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

# Rule to build the object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(MACROS) -c -lstdc++ $< -o $@

# .PRECIOUS directive to keep the TARGET and OBJ_FILES
.PRECIOUS: $(TARGET) $(OBJECTS)

# Clean rule
.PHONY: default all clean
clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)






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

