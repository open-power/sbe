#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: modules/udparsers/o3500/fetchSbehwRegData.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020,2022
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

# Python module to convert HwReg dump to bmc error ffdc format

############################################################
# Imports - Imports - Imports - Imports - Imports - Imports
############################################################
#Python Provided
import sys
import os
import argparse
import textwrap
import struct

############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################

#Tool version
toolVersion = 1.0

#Size of word/uint32_t
sizeofWord = 4

#Verbose mode
verbose = False

# Structure indicating the contents of FFDC package for
# collect reg ffdc dump data. More info for SbeFfdcData_t
# src/sbefw/core/sbeFFDC.H and src/sbefw/core/sbeFFDCType.H
#eachHwRegRowSize is 3 words data == 12 bytes
#HwReg data sent seems to be register[1 word]/addresss[2 words] info

eachHwRegRowSize = 3 * sizeofWord

#20 entries limits the size of SbeFfdcData_t under 256 bytes
maxInputRowSize = eachHwRegRowSize * 20

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################
#Parse the sbe hw reg data and extract addr and scom data info
def parseHwRegBuff(sbeHwRegBinFile, sbeHwRegTraceFile):

    file = open(sbeHwRegBinFile, "rb")
    fileWrite = open(sbeHwRegTraceFile, "w")

    #First word of data in bin packet is FFDC RC buffer
    rcNumber = file.read(4)
    rcNumber = int.from_bytes(rcNumber,"big")
    fileWrite.write("  SBE FFDC RC Number                       :  %s \n"  % hex(rcNumber))

    #Second word of data in size of HwReg Number in Bytes
    countHwReg = file.read(4)
    countHwReg = int.from_bytes(countHwReg,"big")

    #Third blob of data in FFDC packet is hw_data.bin
    count = countHwReg // eachHwRegRowSize
    if count >= maxInputRowSize:
        fileWrite.write("    Input sbe hw reg data exceeds max size of %s bytes \n" % maxInputRowSize)
        return

    fileWrite.write("  Count of SbeHwReg Data rows in FFDC blob :  %s \n" % hex(count))
    while(count):
        count -= 1
        #Read 4 bytes of hw reg address from bin
        hwRegAddr = file.read(4)
        hwRegAddr = int.from_bytes(hwRegAddr,"big")

        #Read 8 bytes of hw reg address data from bin
        hwRegData = file.read(8)
        hwRegData = int.from_bytes(hwRegData,"big")
        fileWrite.write("    Scom Address[%s] "  %hex(hwRegAddr) )
        fileWrite.write("    Hw Reg Data [%s] \n"  %hex(hwRegData))

    file.close()
    fileWrite.close()
