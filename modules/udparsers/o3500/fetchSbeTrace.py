#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: modules/udparsers/o3500/fetchSbeTrace.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020,2021
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

# Akhilesh S - created 07/21/2021
# Python module to extract PPE trace's from SBE chip-op response

############################################################
# Imports - Imports - Imports - Imports - Imports - Imports
############################################################
#Python Provided
import sys
import os
import argparse
import textwrap

############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################

#Tool version
toolVersion = 1.0

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################
#Check if trace buffer is present in SBE Chip-op response.
def checkForPpeTraceBuff(sbeRespBinFile):

    #Componenet ID
    bufferName = "SBE_TRACE"
    bufferNameHexString = bytes(bufferName, 'utf-8')

    with open(sbeRespBinFile, 'rb') as rspBin:
        readData = rspBin.read()

    rspBin.close()

    bufferNameOffset = readData.find(bufferNameHexString)
    if bufferNameOffset > -1:  #String found => PPE Trace Buffer found
        return bufferNameOffset
    else:
        sys.exit("PPE Trace Buffer not found\n")

#Find the start offset and size of the trace buffer
#For offset details refer:
#/ppe-p10/src/tools/trace/ppe2fsp.py (or)
#/ppe-p10/src/import/chips/p10/common/ppe/ppetrace/pk_trace.h
def getBuffDetails(sbeRespBinFile, bufferNameOffset):

    pkTraceBuffHdrSize = 56
    buffStartOffset = bufferNameOffset - 4
    buffSizeOffset = bufferNameOffset + 22

    file = open(sbeRespBinFile, "rb")
    file.seek(buffSizeOffset)
    #Read 2 bytes of buffer size
    bufferSize = file.read(2)
    file.close()
    #TODO:Check for right endians
    bufferSize = int.from_bytes(bufferSize,"big")
    totalBuffSize = bufferSize + pkTraceBuffHdrSize

    return(buffStartOffset,totalBuffSize)

################################## Main ###############################################

################################################
def fetchSbeTraces(sbeRespBinFile, sbeTraceFile):
    #check if PPE trace buffer is present
    componentIdOffset = checkForPpeTraceBuff(sbeRespBinFile)

    #Get the start offset and size of the trace buffer
    (startOffset,size) = getBuffDetails(sbeRespBinFile, componentIdOffset)

    #Extract out the pk trace buffer
    cmd = "dd skip=" + str(startOffset) + " count=" + str(size) + " if=" + sbeRespBinFile  + " of=ppeTrace.bin bs=1"
    print(cmd)
    os.system(cmd)

    #Execute ppe2fsp tool to convert ppe trace to fsp trace.
    #TODO:Provide the right path to the tool as per BMC env
    cmd =  os.environ['PATH_PYTHON3'] + " " + "ppe2fsp.py -i ./ppeTrace.bin -o ./sbetrace.bin -e big"
    print(cmd)
    os.system(cmd)

    #TODO:We can execute the fspTrace tool in same script or seperately
    #TODO:Get the filepath for fsp-trace and ppe2fsp otherwise copy both the files with ppe2fsp.py 
#    cmd = "fsp-trace -s sbeStringFile sbetrace.bin > sbeTraceFile"
#    print(cmd)
#    os.system(cmd)
