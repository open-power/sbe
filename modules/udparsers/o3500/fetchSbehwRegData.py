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
def parseHwRegBuff(sbeHwRegBinFile):

    file = open(sbeHwRegBinFile, "rb")

    #First word of data in bin packet is FFDC RC buffer
    rcNumber = file.read(4)
    rcNumber = int.from_bytes(rcNumber,"big")
    print("  SBE FFDC RC Number                       :  %s "  % hex(rcNumber), end ="\n")

    #Second word of data in size of HwReg Number in Bytes
    countHwReg = file.read(4)
    countHwReg = int.from_bytes(countHwReg,"big")

    #Third blob of data in FFDC packet is hw_data.bin
    count = countHwReg // eachHwRegRowSize
    print("  Count of SbeHwReg Data rows in FFDC blob :  %s " % hex(count), end ="\n")

    assert count <= maxInputRowSize, "Input sbe hw reg data exceeds max size of %s bytes" % maxInputRowSize
    while(count):
        count -= 1
        #Read 4 bytes of hw reg address from bin
        hwRegAddr = file.read(4)
        hwRegAddr = int.from_bytes(hwRegAddr,"big")

        #Read 8 bytes of hw reg address data from bin
        hwRegData = file.read(8)
        hwRegData = int.from_bytes(hwRegData,"big")
        print("    Scom Address[%s]"  %hex(hwRegAddr) )
        print("    Hw Reg Data [%s]"  %hex(hwRegData), end ="\n")
    file.close()

################################## Main ########################################
# Command line options
# Create the argparser object
# We'll do it in a way so we can create required/optional cmdline arg groups
argparser = argparse.ArgumentParser(
                 description="Tool To convert HwReg Data into BMC trace format",
                 add_help=False,
                 formatter_class=argparse.RawDescriptionHelpFormatter,
                 epilog=textwrap.dedent('''
Version: ''' + str(toolVersion) + '''
Examples:  > fetchSbehwRegData.py -i <sbeHwRegFfdcBinFile> > <sbeHwRegFfdcTraceFile>

'''))

# Create our group of required cmdline args
reqgroup = argparser.add_argument_group('Required Arguments')
reqgroup.add_argument('-i', '--sbeHwRegFfdcBinFile', required=True, help="Sbe HwReg Data File which needs to be converted to BMC trace format")

# Create our group of optional cmdline args
optgroup = argparser.add_argument_group('Optional Arguments')
optgroup.add_argument('-h', '--help', action="help", help="Show this help message and exit")
optgroup.add_argument('-v', '--verbose', action='store_true', help="Show all traces")
optgroup.add_argument('-e', '--endianFormat', help="Endian format of FSP trace", choices=["big","little","auto"], default = "auto" )

# cmdline loaded up, now parse for it and handle what is found
args = argparser.parse_args()

# Get the FFDC HwReg Data Bin file path
sbeHwRegFfdcBinFile = args.sbeHwRegFfdcBinFile

# Get the verbose flag
verbose = args.verbose

# Get the fsp trace bin file endian format.
# By default this is always auto, based on machine script will internally
# find endian format and create the fsp trace bin.
# But an option has been provided to create fsp trace bin file as per user choice.
fspTraceEndianFormat = args.endianFormat

#If endian format is auto lets find out the endian format based on the machine
if fspTraceEndianFormat == "auto":
    fspTraceEndianFormat = sys.byteorder

#Create the FSP trace bin file
parseHwRegBuff(sbeHwRegFfdcBinFile)

