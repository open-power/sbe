#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: modules/udparsers/o3500/parseSbeFfdcBlob.py $
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

# Akhilesh S - created 07/21/2021
# Python utility module to process SBE FFDC Packet in BMC systems.

############################################################
# Imports - Imports - Imports - Imports - Imports - Imports
############################################################
#Python Provided
import sys
import os

############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################
#Check if trace buffer is present in SBE FFDC Chip-op response.
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

#This function will extract sbe_trace.bin,sbe_attr.bin and sbe_hw_data.bin
#from the sbeUserDataBlob/sbeFFDC into 3 sepearte bin files for further usage.
#We will extract all .bin basis trace buffer reference offset.
#For offset details refer:
#/ppe-p10/src/tools/trace/ppe2fsp.py (or)
#/ppe-p10/src/import/chips/p10/common/ppe/ppetrace/pk_trace.h
#For FFDC offset's refer FFDC packed structure
def parseSbeUserDataBlob(sbeRespBinFile):

    file = open(sbeRespBinFile, "rb")
    #First blob of data in FFDC packet is SBE Trace buffer

    #check if PPE trace buffer is present
    componentIdOffset = checkForPpeTraceBuff(sbeRespBinFile)
    buffStartOffset = componentIdOffset - 4
    buffSizeOffset = componentIdOffset - 6
    file.seek(buffSizeOffset)
    #Read 2 bytes of buffer size
    traceBufferSize = file.read(2)
    #TODO:Check for right endians
    traceBufferSize = int.from_bytes(traceBufferSize,"big")

    #Second blob of data in FFDC packet is attr.bin

    attrBinStartOffset = buffStartOffset + traceBufferSize + 4
    attrBinSizeOffset = buffStartOffset + traceBufferSize + 2
    file.seek(attrBinSizeOffset)
    #Read 2 bytes of attr bin size
    attrBinSize = file.read(2)
    #TODO:Check for right endians
    attrBinSize = int.from_bytes(attrBinSize,"big")
    print(attrBinSize)

    #Third blob of data in FFDC packet is hw_data.bin

    hwDataBinStartOffset = attrBinStartOffset + attrBinSize + 4
    hwDataBinSizeOffset = attrBinStartOffset + attrBinSize + 2
    file.seek(hwDataBinSizeOffset)
    #Read 2 bytes of hw data bin size
    hwDataBinSize = file.read(2)
    #TODO:Check for right endians
    hwDataBinSize = int.from_bytes(hwDataBinSize,"big")
    print(hwDataBinSize)

    file.close()

    #Extract out the pk trace buffer
    ppeTraceName = "/tmp/ppeTrace.bin"
    cmd = "dd skip=" + str(buffStartOffset) + " count=" + str(traceBufferSize) + " if=" + sbeRespBinFile  + " of=" + ppeTraceName + " bs=1"
    print(cmd)
    os.system(cmd)

    #Extract out the attr bin
    attrName = "/tmp/attr.bin"
    cmd = "dd skip=" + str(attrBinStartOffset) + " count=" + str(attrBinSize) + " if=" + sbeRespBinFile  + " of=" + attrName + " bs=1"
    print(cmd)
    os.system(cmd)

    #Extract out the hw data bin
    hwDataName = "/tmp/hwData.bin"
    cmd = "dd skip=" + str(hwDataBinStartOffset) + " count=" + str(hwDataBinSize) + " if=" + sbeRespBinFile  + " of=" + hwDataName + " bs=1"
    print(cmd)
    os.system(cmd)

def fetchSbeTraces():

    #Execute ppe2fsp tool to convert ppe trace to fsp trace.
    #TODO:Provide the right path to the tool as per BMC env
    cmd =  "./ppe2fsp.py -i ./ppeTrace.bin -o ./sbetrace.bin -e big"
    print(cmd)
    os.system(cmd)

    #TODO:We can execute the fspTrace tool in same script or seperately
    #TODO:Get the filepath for fsp-trace and ppe2fsp otherwise copy both the files with ppe2fsp.py
    #TODO:Need to be provided by HB Team
#    cmd = "fsp-trace -s sbeStringFile sbetrace.bin > sbeTraceFile"
#    print(cmd)
#    os.system(cmd)

def fetchSbeHwRegData():

    #Execute sbeHwRegData tool to convert hwReg Struct trace to Data.
    #TODO:Provide the right path to the tool as per BMC env
    cmd = "./fetchSbehwRegData.py -i /tmp/hwData.bin > /tmp/sbeHwRegTraceFile"
    print(cmd)
    os.system(cmd)

