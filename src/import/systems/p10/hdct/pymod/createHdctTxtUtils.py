#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/systems/p10/hdct/pymod/createHdctTxtUtils.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2022
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

# Akhilesh S - created 29/10/2020
# Python module to define HDCT.bin  parse function
# EKB-Mirror-To: hw/ppe

############################################################
# Imports - Imports - Imports - Imports - Imports - Imports
############################################################
# Python provided
import os
import sys
import struct
import binascii
################################################# NOTE: DO NOT IMPORT ANY MODULES WHICH USE ECMD LIBRARY/PATH ############################################
#This module is mirrored into ppe repo which has no ecmd support in simics env where this module is called in a test case
# Add the common modules to the path
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__))))
# Our supporting modules
from dumpConstants import out
# Since this script can be called stand alone, define out if not already defined
if (out == None):
    out = output.out()
import dumpConstants

############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################

#Defined dump types order:(As per HDCT.txt)
#SCS | CSCX | CCR | CCRCLK | CHCR | CHCRCLK | MPIPL | HB | PM | PERF | CCS
#Dict for dump types as per HDCT.txt order
hdctDumpTypes = {"SCS" : 1, "CSCX" : 2, "CCR" : 3, "CCRCLK" : 4,
    "CHCR" : 5, "CHCRCLK" : 6, "MPIPL" : 7, "HB" : 8,
            "PM" : 9, "PERF" : 10, "CCS" : 11
}

############################################################
# Classes - Classes - Classes - Classes - Classes - Classes
############################################################
class HDCTBinEntry():
    """
    Python usable representation of a single HDCT entry from HDCT.bin
    """
    def __init__(self, parseFormat):
        self.parseFormat  = parseFormat # For determining some default values
        if (self.parseFormat == "ekb"):
            defChipUnitType = defChipUnitNum = None
        elif (self.parseFormat == "ppe"):
            defChipUnitNum = 0
            defChipUnitType = "chip"
        else:
            out.critical("Unknown parse format %s " % parseFormat)
            sys.exit(1)

        #Data Header(Common for all command types)
        self.dumpTypes    = None
        self.command      = None
        self.chipType    = None
        self.chipUnitType = defChipUnitType
        self.chipUnitNum  = defChipUnitNum
        self.chipletStart = None #For internal use
        self.chipletEnd = None   #For internal use
        self.address = None   #Address/hash value of string

        #Special case entries like -bor,-band,channel,mode,value etc
        self.funcargs = dict()

        #Additional details added in HDCT.bin which are not a part of HDCT.txt
        #Ring details, fast array details etc
        self.addDetails = dict()

    def printSingleEntry(self):

        out.print(" ")
        out.print("{:<45} {:<15} {:<15} {:<15} {:<15} {:<15}".format('Dump Types','Command Type','Chip Unit Type','Chip Unit Num','Address','Func Args'))
        out.print("{:<45} {:<15} {:<15} {:<15} {:<15} {:<15}".format(str(self.dumpTypes),self.command,str(self.chipUnitType),str(self.chipUnitNum),self.address,str(self.funcargs)))
        out.print(" ")
        out.print("Additional Details: %s " % self.addDetails)
        out.print(" ")

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################

def createHDCTTxt(HDCTBinFileName, parserUsage = "ppe", stringHashDict = None):

    file = open(HDCTBinFileName,"rb" )

    #Read and process the ekb commit ID from HDCT.bin
    byte = file.read(8)
    ekbCommitIDBin = binascii.hexlify(bytearray(byte)).upper()
    out.debug("EKB Commit ID from HDCT.bin: %s" % ekbCommitIDBin.decode('utf8'))

    #Our return type.Will contain all the HDCT.txt decode entries from HDCT.bin.
    hdctTxtEntries = list()

    while True:

        genericVal = file.read(4)

        if((genericVal != b'')):

             entry = HDCTBinEntry(parserUsage)

             #decodeGenericVal
             data = int.from_bytes(genericVal, "big")
             out.debug("genericVal: %s" % hex(data))

                #decodeContent in genericVal
             content = data & 0x000003FF
             out.debug("content: %s" % content)

             dumpType = []
             for i in range(10):
                 if(((content & (1 << i)) >> i) == 0x01):
                     try:
                        dumpType.append(dumpConstants.dumpTypeIDToString["ebmc"][struct.pack("B",i)])
                     except:
                        out.critical("Unknown Dump Type Found %s" % i)
                        sys.exit(-1)
             #Rearrange the dump types as per HDCT.txt order
             entry.dumpTypes = sorted(dumpType, key=lambda x: hdctDumpTypes[x])

                #decodeCmd in genericVal
             cmd = (data & 0x00003C00) >> 10
             out.debug("cmd: %s" % cmd)

             try:
                cmdTxt = dumpConstants.commandIdToString[struct.pack("B", cmd)];
             except:
                out.critical("Unknown Command Type Found %s" % cmd)
                sys.exit(-1)

             entry.command = cmdTxt

                #decodeChipInfo in genericVal
             chipInfo = (data & 0xFFFFC000) >> 14
             out.debug("Chip Info: %s" % chipInfo)

             chipUnitType = (chipInfo & 0x3F000) >> 12
             try:
                chipUnitTypeTxt = dumpConstants.chipUnitTypeIdToString[struct.pack("B",chipUnitType)];
             except:
                out.critical("Unknown Chip Unit Type Found %s" % chipUnitType)
                sys.exit(-1)
             if chipUnitType != 0x00:
                entry.chipUnitType = chipUnitTypeTxt

             if ( (entry.command != "getmempba") and (entry.chipUnitType != "ocmb") ):
                entry.chipType = "pu"

                #decodeStartEnd in genericVal
             data = int.from_bytes(genericVal, "big")
             chipInfo = (data & 0xFFFFC000) >> 14

             entry.chipletStart = start = (chipInfo & 0x00FC0) >> 6
             out.debug("fst chiplet: %s" % start)

             entry.chipletEnd = end = (chipInfo & 0x0003F)
             out.debug("lst chiplet: %s" % end)

             if((start != 0x00) and (end != 0x00)):
                 entry.chipUnitNum = str(start) + ".." + str(end)
             elif((start != 0x00) and (end == 0x00)):
                 entry.chipUnitNum = str(start)

             # decodeAddrID
             if(cmdTxt == 'gettracearray') or (cmdTxt == 'getring') or (cmdTxt == 'stopclocks') or (cmdTxt == 'getfastarray'):
                addrID = file.read(4)
                val = int.from_bytes(addrID, "big")
                #String values are not required in ppe/sbe test cse
                if parserUsage != "ppe":
                    commandArg = stringHashDict[val]
                    entry.address = commandArg
                else:
                    entry.address = str(format(val,'08X'))
             else:
                addrID = file.read(4)
                commandArg = int.from_bytes(addrID, "big")
                entry.address = str(format(commandArg,'08X'))

             #decode control_set in case of fast array and write into dictionary
             #Will not be validated as it is not a part of HDCT.txt
             if(entry.command == "getfastarray"):
                entry.addDetails.update({entry.address:int.from_bytes(file.read(4), "big")})

             #Decode ring address and length in case of getring and write into dictionary.
             #Will not be validated as it is not part of HDCT.txt
             if(entry.command == "getring"):
                entry.addDetails.update({entry.address:{"ringAddr":int.from_bytes(file.read(4),"big"), "ringLen":int.from_bytes(file.read(4), "big")}})

             #Decode trace array ID and write into dict
             #Will not be validated as it is not part of HDCT.txt
             if(entry.command == "gettracearray"):
                entry.addDetails.update({entry.address:int.from_bytes(file.read(4),"big")})

             #Decode target type in case of stop clocks and write into dict
             #Will not be validated as it is not part of HDCT.txt
             if(entry.command == "stopclocks"):
                entry.addDetails.update({entry.address:int.from_bytes(file.read(4),"big")})

             #decodeFunArg
             args = {}

             if cmdTxt == "putscom":
                funArg = file.read(8)
                functArg = int.from_bytes(funArg , "big")
                args["i_data"] = str(format(functArg,'016X'))
             elif(cmdTxt == 'getsram') or (cmdTxt == 'getmempba'):
                funArg = file.read(8)
                functArg = int.from_bytes(funArg , "big")
                args["i_bytes"] = str(format(functArg,'016'))
             entry.funcargs = args

             #Decode -bor/-band if present for putscom
             if(entry.command == "putscom"):
                 bit_modifier = int.from_bytes(file.read(4),"big")
                 if bit_modifier == 0x01:
                     entry.funcargs["i_band"] = True
                 elif bit_modifier == 0x02:
                     entry.funcargs["i_bor"] = True

             #Decode mode/channel if present for getsram
             if (entry.command == "getsram"):
                 entry.funcargs["i_channel"] = str(int.from_bytes(file.read(4),"big"))

             hdctTxtEntries.append(entry)
        else:
            break
    file.close()

    return (ekbCommitIDBin, hdctTxtEntries)
