#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: modules/udparsers/o3500/o3500.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021
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
import json
import sys
import os
import argparse
import textwrap
import fetchSbeTrace
############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################

#Tool version
toolVersion = 1.0

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################
#import fetchSbeTrace

def parseUDToJson(subType, ver, data):
    # Create a binary file with the SBE data.

    with open('sbeffdcFile', "wb") as ffdcFile:
        ffdcFile.write(data)

    ffdcFile.close();

    # Parse and populate data into dictionary
    fetchSbeTrace.fetchSbeTraces("sbeffdcFile", "sbeTraceFile")

    #with open("sbeTraceFile", 'rb') as rspBin:
    with open("sbeTraceFile", 'r') as rspBin:
        readData = rspBin.read()

    jsonStr = json.dumps({"Data": readData})
    return jsonStr

############################################ Main ###################################
# Command line options
# Create the argparser object
# We'll do it in a way so we can create required/optional cmdline arg groups

#argparser = argparse.ArgumentParser(description="Tool to parse SBE traces from SBE FDDC",
#                                    add_help=False,
#                                    formatter_class=argparse.RawDescriptionHelpFormatter,
#                                    epilog=textwrap.dedent('''
#Version: ''' + str(toolVersion) + '''

#Examples:  > o3500.py -i <sbeFFDCFile> -o <ppeTraceFile>

#'''))

# Create our group of required cmdline args
#reqgroup = argparser.add_argument_group('Required Arguments')
#reqgroup.add_argument('-i', '--respBinFile', required=True, help="SBE FFDC response in a file")
#reqgroup.add_argument('-o', '--ppeTraceFile', required=True, help="Output file containing the PPE trace")

# Create our group of optional cmdline args
#optgroup = argparser.add_argument_group('Optional Arguments')
#optgroup.add_argument('-h', '--help', action="help", help="Show this help message and exit")
#optgroup.add_argument('-v', '--verbose', action='store_true', help="Show all traces")

# cmdline loaded up, now parse for it and handle what is found
#args = argparser.parse_args()

#Get the response bin file
#sbeRespBinFile = args.respBinFile

# Get the PPE Trace Bin file path
#ppeTraceFile = args.ppeTraceFile

# Read the file and store the content in buffer.
#with open(sbeRespBinFile, 'rb') as rspBin:
#        readData = rspBin.read()

# Execute parseUDToJson
#print(parseUDToJson(0x3500, 1, readData))
