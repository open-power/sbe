#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: modules/udparsers/o3500/o3500.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021,2022
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
import json
import sys
import os
import argparse
import textwrap
import udparsers.o3500.parseSbeFfdcBlob

############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################

#Tool version
toolVersion = 1.0

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################

def parseUDToJson(subType, ver, data):

    #Parse the FFDC blob and fetch ppeTrace.bin,attr.bin and hwData.bin
    udparsers.o3500.parseSbeFfdcBlob.parseSbeUserDataBlob(data)

    #Parse ppeTrace.bin file to fetch SBE Traces
    traceDataString = udparsers.o3500.parseSbeFfdcBlob.fetchSbeTraces()

    # Split the data string into a list for "pretty printing"
    traces = traceDataString.split("\n")
    d = dict()
    d["Data"] = traces

    #Parse hwData.bin file to fetch SBE HwRegData
    udparsers.o3500.parseSbeFfdcBlob.fetchSbeHwRegData()
    with open("/tmp/sbeHwRegTraceFile", 'r') as rspHwBin:
        readData = rspHwBin.read()
    sbeHwRegTraces = readData.split("\n")
    d["HwRegData"] = sbeHwRegTraces

    # TODO: Convert all the scripts to work on data stream.
    # Delete all the temp files created.
    if os.path.exists("/tmp/sbeffdcBin"):
        os.remove("/tmp/sbeffdcBin")
    if os.path.exists("/tmp/ppeTrace.bin"):
        os.remove("/tmp/ppeTrace.bin")
    if os.path.exists("/tmp/hwData.bin"):
        os.remove("/tmp/hwData.bin")
    if os.path.exists("/tmp/attr.bin"):
        os.remove("/tmp/attr.bin")
    if os.path.exists("/tmp/sbetrace.bin"):
        os.remove("/tmp/sbetrace.bin")

    jsonStr = json.dumps(d)
    return jsonStr

############################################ Main ###################################
