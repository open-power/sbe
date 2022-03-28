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
import json

from udparsers.helpers.miscUtils import getLid
from udparsers.helpers.hostfw_trace import get_binary_trace_data_as_string
############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################
SBE_STRING_LID_FILE = "81E0068A.lid"

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################
#Check if trace buffer is present in SBE FFDC Chip-op response.

def checkForPpeTraceBuff():

    #Componenet ID
    bufferName = "SBE_TRACE"
    bufferNameHexString = bytes(bufferName, 'utf-8')

    with open("/tmp/sbeffdcBin", 'rb') as rspBin:
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
def parseSbeUserDataBlob(data):

    with open("/tmp/sbeffdcBin", "wb") as sbeRespBinFile:
        sbeRespBinFile.write(data)

    file = open("/tmp/sbeffdcBin", "rb")

    #check if PPE trace buffer is present
    componentIdOffset = checkForPpeTraceBuff()

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

    #Extract out the pk trace buffer
    ppeTraceName = "/tmp/ppeTrace.bin"
    cmd = "dd skip=" + str(buffStartOffset) + " count=" + str(traceBufferSize) + " if=" + "/tmp/sbeffdcBin"  + " of=" + ppeTraceName + " bs=1"
    print(cmd)
    os.system(cmd)

    #Extract out the attr bin
    attrName = "/tmp/attr.bin"
    cmd = "dd skip=" + str(attrBinStartOffset) + " count=" + str(attrBinSize) + " if=" + "/tmp/sbeffdcBin"  + " of=" + attrName + " bs=1"
    print(cmd)
    os.system(cmd)

    #Extract out the hw data bin
    hwDataName = "/tmp/hwData.bin"
    cmd = "dd skip=" + str(hwDataBinStartOffset) + " count=" + str(hwDataBinSize) + " if=" + "/tmp/sbeffdcBin"  + " of=" + hwDataName + " bs=1"
    print(cmd)
    os.system(cmd)

    file.close()


def fetchSbeTraces():

    #Execute ppe2fsp tool to convert ppe trace to fsp trace.
    #TODO:Provide the right path to the tool as per BMC env
    cmd =  "/usr/lib/python3.9/site-packages/udparsers/o3500/ppe2fsp.py -i /tmp/ppeTrace.bin -o /tmp/sbetrace.bin -e big"
    print(cmd)
    os.system(cmd)

    # Create an stream from sbetrace.bin
    with open('/tmp/sbetrace.bin', 'rb') as rspBin:
        readData = rspBin.read()

    # Create a dictionary to hold the trace output
    d = dict()

    # Get the LID file for the HB string file
    stringFile = getLid(SBE_STRING_LID_FILE)


    if stringFile == "":
        d["File not found"]=SBE_STRING_LID_FILE
        jsonStr = json.dumps(d)
        return jsonStr

    startingPosition = 0
    printNumberOfTraces = -1 # -1 means to get all traces
    (retVal, traceDataString) = get_binary_trace_data_as_string(readData, startingPosition, printNumberOfTraces, stringFile)

    return traceDataString

def fetchSbeHwRegData():
    #Execute sbeHwRegData tool to convert hwReg Struct trace to Data.
    #TODO:Provide the right path to the tool as per BMC env
    cmd = "/usr/lib/python3.9/site-packages/udparsers/o3500/fetchSbehwRegData.py -i /tmp/hwData.bin > /tmp/sbeHwRegTraceFile"
    print(cmd)
    os.system(cmd)

