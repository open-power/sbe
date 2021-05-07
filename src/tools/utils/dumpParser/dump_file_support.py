#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/dumpParser/dump_file_support.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021
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

#Python Provided
import os
import sys
import glob

##################################### NOTE: DO NOT IMPORT ANT MODULES FROM CTE PATH WHICH USE ECMD LIBRARY/PATH #####################################
#Supporting modules from CTE path
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$CTEPATH")+ "/tools/crondump/rel/pymod"))
import out

def getHdctBinFile(outputPath):
    hwImagePath = os.path.expandvars("$BACKING_BUILD") + "/src/engd/href/p10.hw_ref_image.bin"
    iplImageToolPath = os.path.expandvars("$BACKING_BUILD") + "/obj/ppc/sbei/sbfw/simics/ipl_image_tool"
    filePathDict = {"DD1HDCT":outputPath + "/hdctDD1.bin",
                    "DD2HDCT":outputPath + "/hdctDD2.bin"
                    }

    #Copy the HWREF image locally.
    cmd = "cp " + hwImagePath + " /tmp/"
    out.print(cmd)
    os.system(cmd)

    #Extract SBE section from HWREF image.
    cmd = iplImageToolPath + " /tmp/p10.hw_ref_image.bin extract .sbe /tmp/sbe.bin"
    out.print(cmd)
    os.system(cmd)

    #Extract HDCT section from SBE section of HWREF image.DD1
    out.print("\nExtracting DD1 HDCT.bin")
    cmd = iplImageToolPath + " /tmp/sbe.bin extract .hdct 10 " + filePathDict["DD1HDCT"]
    out.print(cmd)
    os.system(cmd)

    #Extract HDCT section from SBE section of HWREF image.DD2
    out.print("\nExtracting DD2 HDCT.bin")
    cmd = iplImageToolPath + " /tmp/sbe.bin extract .hdct 20 " + filePathDict["DD2HDCT"]
    out.print(cmd)
    os.system(cmd)

    return filePathDict

def getHdctLookup(ekbCommitID, hdctLookupFilePath):

    #Path of HDCT.lookup file(Default search path via EKB commit ID)
    hdctLookupFilePath = os.path.join("/gsa/rchgsa/projects/s/smartdump/hdct-save/p10/ekb/")

    hdctLookupFilePath = hdctLookupFilePath + ekbCommitID.lower().decode('utf8')[0] + "/"
    hdctLookupFilePath = glob.glob(hdctLookupFilePath + "*" + ekbCommitID.lower().decode('utf8') + "*")
    if not hdctLookupFilePath:
        out.critical("No dir found with EKB commit ID: %s" % ekbCommitID)
        hdctLookupFilePath = None
        out.critical("Checking if user has entered lookup file path")
    else:
        hdctLookupFilePath = hdctLookupFilePath[0] + "/lookup"

    return hdctLookupFilePath
