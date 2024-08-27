#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/hwtestsuiteverificationimg/testxls2json.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2024
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

import argparse
import os
import pandas as pd
import json


def checkNA(var: str) -> bool:
    """
    Check Not applicable

    Args:
        var: Not applicable string

    Returns:
        Return True if string is "NA" or "na" otherwise False

    """
    if var == "NA" or var == "na":
        return True
    else:
        return False


def safeHex2Int(hexString: str) -> int:
    """
    Safely converts a hexadecimal string to an integer

    Args:
        hexString: The hexadecimal string to convert

    Returns:
        The integer value of the hexadecimal string, or 0 if the data is 0, "NA", or "na" or "Na
  """

    if hexString in [0, "NA", "na", "Na", "nan", ""]:
        return 0
    try:
        return int(hexString, 16)
    except ValueError:
        return None


def parsescratch(lookuptable: str, loadVStatus: "str", tpmStatus: "str", hbblShStatus: "str", fwShStatus: "str", msgReg: str) -> "list[dict]":
    """
    Parse scratch data and combine data

    Args:
        lookuptable    : Lookup table
        loadVStatus    :load verification status value
        tpmStatus      :tpm status value
        hbblShStatus   :hbbl secure header status value
        fwShStatus     :firmware secure header status value
        msgReg         :msg register

    Returns:
        Dict
        [
            {
                "address": "0x50009",
                "strvalue": "SBE_CODE_VERIFICATION_PIBMEM_MAIN_MSG",
                "value": 0x0A
            },
            {
                "address": "0x50182"
                "byte0": "0",                   # loadVStatus
                "byte1": "0",                   # tpmStatus
                "byte2": "MAGIC_NUMBER_TEST",   # hbblShStatus
                "byte3": "0",                   # fwShStatus
                "strvalue": "0" | "0" | "MAGIC_NUMBER_TEST" | "0",
                "value": 0x2000                 # byte0 << 24 | byte1 << 16 | byte2 << 8 | byte3
            }
        ]
    """

    dataDict = []
    scomData = {}

    # Loading lookup table and loading to json
    with open(lookuptable, 'r') as f:
        lookup = json.load(f)

        # Check Not applicable flag before assigning
        if not checkNA(msgReg):
            l_msgReg = safeHex2Int(lookup["0x50009"][msgReg])

            # Creating dict for reg 0x50009
            scomData["address"]  = "0x50009"
            scomData["strvalue"] = msgReg
            scomData["value"]    = l_msgReg

            # Append to list
            dataDict.append(scomData.copy())

            # clear dict to assign new data
            scomData.clear()

        # Creating dict for reg 0x50182
        l_loadVStatus  = safeHex2Int(0 if loadVStatus  == 0 else lookup["0x50182"][loadVStatus])
        l_tpmStatus    = safeHex2Int(0 if tpmStatus    == 0 else lookup["0x50182"][tpmStatus])
        l_hbblShStatus = safeHex2Int(0 if hbblShStatus == 0 else lookup["0x50182"][hbblShStatus])
        l_fwShStatus   = safeHex2Int(0 if fwShStatus   == 0 else lookup["0x50182"][fwShStatus])

        scomData["address"]  = "0x50182"
        scomData["byte0"]    = loadVStatus
        scomData["byte1"]    = tpmStatus
        scomData["byte2"]    = hbblShStatus
        scomData["byte3"]    = fwShStatus
        scomData["value"]    = l_loadVStatus << 24 | l_tpmStatus << 16 | l_hbblShStatus << 8 | l_fwShStatus

        # Append to list
        dataDict.append(scomData.copy())

        # clear dict to assign new data
        scomData.clear()

    # Return data dict
    return dataDict



def dictCheckNaAndRemove(data: dict) -> dict:
    """
    Check all dict values, if values is if "NA" or "na" remove key from dict

    Args:
        data: Dict data

    Return:
        Return the trimmed dict
    """

    delKeysList = []

    # Verify value have a not applicable keyword
    for key, value in data.items():
       if value in [0, "NA", "na", "Na", "nan", ""]:
          delKeysList.append(key)

    # Removing key, value pair from given dict if not applicable
    for key in delKeysList:
       data.pop(key)

    return data



def convertExcelToJson(args):
    fullDict = {}

    # Read excel using pandas lib
    df = pd.read_excel(args.file, engine='openpyxl')

    # Extract columns
    tids            = df.get('Tid')
    priorities      = df.get('Priority')
    groups          = df.get('Group')
    header          = df.get('Header')
    fieldname       = df.get('Field name')
    descriptions    = df.get('Description')
    filenames       = df.get('Filename')
    sizes           = df.get('Size')
    section         = df.get('Section')
    sbMode          = df.get('sb mode')
    loadVStatus     = df.get('loadVStatus - Scratch 11 (0x50182)')
    tpmStatus       = df.get('tpmStatus - Scratch 11 (0x50182)')
    hbblShStatus    = df.get('hbblSecureHdrStatus - Scratch 11 (0x50182)')
    fwShStatus      = df.get('sbeFWSecureHdrStatus - Scratch 11 (0x50182)')
    fwMeasurement   = df.get('VERIFICATION MEAS FIRMWARE 0x10018-x1001B (otprom register 8-11)')
    hbblMeasurement = df.get('VERIFICATION MEAS HBBL 0xx1001C-x1001F (otprom register 12-15)')
    msgReg          = df.get('MESSAGING REG 0x50009')
    ppestates       = df.get('PPE State')
    tesTtype        = df.get('Test type')

    nanCount = 0
    # Extract ROW and creating dict
    for row in range(0, len(tids)-1):
        if checkNA(tids[row]) == True:
            nanCount = nanCount + 1
            if (nanCount >=5 ):
                break
            continue

        verifyData = parsescratch( args.lookup, loadVStatus[row],
                                   int(tpmStatus[row]), hbblShStatus[row],
                                   fwShStatus[row], msgReg[row] )

        # Parse required elements
        tid         = int(tids[row])
        priority    = int(priorities[row])
        grp         = str(groups[row])
        hdr         = str(header[row])
        fieldn      = str(fieldname[row])
        desc        = str(descriptions[row])
        fname       = str(filenames[row])
        sizze       = str(sizes[row])
        ssection    = str(section[row])
        mode        = int(sbMode[row])
        ppestate    = str(ppestates[row])
        testtype    = str(tesTtype[row])
        fwMeas      = str(fwMeasurement[row])
        hbblMeas    = str(hbblMeasurement[row])

        # Creating dict for each row
        dictTestData = {
           "priority"   : priority,
           "group"      : grp,
           "header"     : hdr,
           "fieldname"  : fieldn,
           "desc"       : desc,
           "filename"   : fname,
           "size"       : sizze,
           "section"    : ssection,
           "sb_mode"    : mode,
           "PPE_state"  : ppestate,
           "Test type"  : testtype,
           "fw_measurement"  : fwMeas,
           "hbbl_measurement": hbblMeas,
           "data"       : {
              "verify"  : verifyData
           }
        }

        # Remove not applicable data from dict
        dictTestData = dictCheckNaAndRemove(dictTestData)

        # generate dict with key test ID
        fullDict[str(tid)] = dictTestData

    with open(args.output, "w") as outfile:
        json.dump(fullDict, outfile, indent=4)

    return fullDict


parser = argparse.ArgumentParser(description="Tool to convert Test case (excel file) to Json")
parser.add_argument('-f', '--file',    required=True,           help='Excel file')
parser.add_argument('-l', '--lookup',  required=True,           help='Look up file which have the mapping of Error and status code')
parser.add_argument("-o","--output",   default='testcase.json', help="Output json file")

parser.set_defaults ( func=convertExcelToJson )
args = parser.parse_args()

args.func(args)
print ("Convert excel to json - successful")