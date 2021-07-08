#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/dumpParser/dump_interpret.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020,2021
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
import time
import glob
import codecs
import binascii
import struct
import argparse
import textwrap
import datetime
import pickle

##################################### NOTE: DO NOT IMPORT ANT MODULES FROM CTE PATH WHICH USE ECMD LIBRARY/PATH #####################################
#HDCT.bin parser(EKB Mirror File)
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$SBEROOT")+ "/src/import/systems/p10/hdct"))
import createHdctTxtUtils as txtUtils

#Supporting modules from CTE path
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$CTEPATH")+ "/tools/crondump/rel/pymod"))
import dumpConstants
import out

#Dump tool files
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$SBEROOT")+ "/src/tools/utils/dumpParser/"))
import dump_utils as utils
import dump_file_support as fileSupport

#Tool version
toolVersion = 1.2

#Dump Footer
dumpFooter = "DONE"

#OCMB Header
ocmbHdr = "OCMB"

# Create a generic time stamp we can use throughout the program
timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

#Start time
stageStartTime = time.time()

#SBE Clock frequency (500MHz)
sbeClkFreq = 500000000.0

#SBE Supported Dump types
sbeSupportedDumpTypes = ["SCS","MPIPL","CCS","PERF","HB"]

#Dump Collection Clock States
clockStates = ["ON","OFF"]

#Dict of string ID and its hash Values.
strHashDict = dict()

#All HDC.bin decoded entries
allHdctBinEntries = list()

#HDCT.bin entries filtered out based on the requested dump type
reqHdctBinEntries = list()

#Total CPU Cycles
totalCpuCycles = 0

#List of all entries that are parsed by fifo
fifoParseEntriesList = list()

#Store the EKB commit ID from HDCT.bin for comparision with FIFO.bin EKB Commit ID
ekbCommitIDHdctBin = ""
ekbCommitIDFifoBin = ""

#Debug prints - on and off
debugPrints = ""

#Verify FIFO output - on and off
verifyFifoOutput = ""

# Create the filename.
filenames = dict()

#OCMB 1st instance completion flag
ocmbFirstInstCompletionFlag = False

#Fifo Parser Stats and HDCT.bin Stats dict for single scom type
#scomCount = Total scoms performed/found in fifo.bin including with ffdc for a given scom type
#ffdcCount = Total ffdc count in fifo.bin for a given scom type
#entriesCount = Number of HDCT entries found in fifo.bin for a given scom type
#hdctBinEntriesCount = Number of HDCT entries found in HDCT.bin for a given scomtype

stats = {"getscom"       : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "putscom"       : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "getring"       : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "stopclocks"    : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "gettracearray" : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "getfastarray"  : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "getmempba"     : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0},
         "getsram"       : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0}
         }

#Fifo parser stats and HDCT.bin Stats dict for all scoms types
statsAll = {"allscom" : {"scomCount" : 0, "ffdcCount" : 0, "entriesCount" : 0, "hdctBinEntriesCount" : 0}}

############################################################
# Classes - Classes - Classes - Classes - Classes - Classes
############################################################
class dumpHdr():
    """
    Python usable representation of dump header
    """
    def __init__(self, hdr):
        tempDataLayoutVersion = struct.unpack(">B",hdr[:1])

        self.dataLayoutVersion = int(''.join(map(str, tempDataLayoutVersion)))
        #Byte 2,3 and 4 are reserved
        self.reserved = 0x000000
        self.HDCTVersion = binascii.hexlify(bytearray(hdr[4:])).upper() #EKB Commit ID

        utils.fancyPrint(" Dump Header " , "*", "print")
        out.print("Data Layout Version: %s" % hex(self.dataLayoutVersion))
        out.print("EKB Commit ID/HDCT version: %s" % self.HDCTVersion.decode('utf8'))
        utils.fancyPrint("" , "*", "print")

class scomData():
    """
    Python usable representation of single scom operation data
    """

    def __init__(self, dataHdr):

        #Data header
        #1byte of command of status is preReq
        self.preReq = int.from_bytes(dataHdr[:1], "big")

        #1byte of command type
        try:
            self.cmdType = dumpConstants.commandIdToString[dataHdr[1:2]];
        except:
            out.critical("Unknown Command Type Found %s" % dataHdr[1:2])
            sys.exit("Parser error/In-correct FIFO output format")

        #1 byte of chip unit type
        try:
            self.chipUnitType = dumpConstants.chipUnitTypeIdToString[dataHdr[2:3]];
        except:
            out.critical("Unknown Chip Unit Type Found %s" % dataHdr[2:3])
            sys.exit("Parser error/In-correct FIFO output format")

        #1 byte of chip unit num
        self.chipUnitNum = int.from_bytes(dataHdr[3:4], "big")

        #8 bytes of command address
        tempcmdAddress = int.from_bytes(dataHdr[4:12], "big")
        self.cmdAddress = str(format(tempcmdAddress,'08X'))

        #4bytes of data length in bits
        tempdataLen = int.from_bytes(dataHdr[12:], "big")
        #All data that is streamed out is 8byte aligned. If unaligned length is
        #present in case of rings etc align it to 8 bytes.
        self.dataLen = 64 * ((int(tempdataLen / 64)) + (1 if int(tempdataLen % 64) else 0) )
        #Length is in bits, lets convert to bytes
        self.dataLen = int(self.dataLen / 8)

        #For internal use. Not a part of header
        self.hdctEntry = 0

        #Update the fifostats
        statsAll["allscom"]["scomCount"] = statsAll["allscom"]["scomCount"] + 1

        #Lets increament the HDCT and scom count  first time without any check
        if len(fifoParseEntriesList) == 0:
            statsAll["allscom"]["entriesCount"] = statsAll["allscom"]["entriesCount"] + 1
            stats[self.cmdType]["entriesCount"] = stats[self.cmdType]["entriesCount"] + 1

        #Update HDCT entrie count by comparing it with last scom addr and current scom addr
        #Assuming all OCMB scoms are at the end of clock on HDCT entries we can
        #stop  stats updation .The stats[self.cmdType]["scomCount"] will still
        #contain all OCMB scom count for 16 instances .
        if len(fifoParseEntriesList) != 0 and self.cmdAddress != fifoParseEntriesList[-1].cmdAddress and ocmbFirstInstCompletionFlag == False:
            statsAll["allscom"]["entriesCount"] = statsAll["allscom"]["entriesCount"] + 1
            stats[self.cmdType]["entriesCount"] = stats[self.cmdType]["entriesCount"] + 1

        stats[self.cmdType]["scomCount"] = stats[self.cmdType]["scomCount"] + 1
        self.hdctEntry = statsAll["allscom"]["entriesCount"]

        if debugPrints == "on":
            utils.fancyPrint(" SCOM %s : HDCT Entry %s " % (statsAll["allscom"]["scomCount"], statsAll["allscom"]["entriesCount"]) , "*", "print")

    def parseChipOpData(self, scomData):

        #Lets skip data collection for getmempba as data is huge in MB. We will
        #extract out getmempba data into a seperate file.
        self.chipOpData = ""
        #Scom output - length as per data length
        numberOfDoubleWords = int(self.dataLen / 8)
        if self.cmdType != "getmempba":
            for i in range(0,numberOfDoubleWords):
                tempChipOpData = int.from_bytes(scomData[(8*i):((8*i)+8)], "big")
                tempChipOpData = str(format(tempChipOpData,'016X'))
                self.chipOpData = self.chipOpData + tempChipOpData

        #Extract the HB memory dump (getmempba).
        #Will be extracted if dump type is HB and clock state is ON.
        if(self.cmdType == "getmempba" and HBMemDump == "true" and dumpType == "HB" and clockState == "ON"):
            out.print("Extracting HB Mem Dump into %s " % filenames["HBMemDump"])
            file = open(filenames["HBMemDump"], "wb")
            for i in range(0,numberOfDoubleWords):
                file.write(scomData[(8*i):((8*i)+8)])
            file.close()

        #If any ring has A5 as its complete content log that ring info into a
        #error file
        file = open(filenames["RingFailInfo"], "a")
        if(self.cmdType == "getring"):
            hashEqvStr = "NONE"
            if int(self.cmdAddress,16) in strHashDict.keys():
                hashEqvStr = strHashDict[int(self.cmdAddress,16)]

            i = (self.chipOpData+self.chipOpData).find(self.chipOpData, 1, -1)
            if (i == -1):
                out.debug("No repeated characters - A5")
            else:
                out.debug("Repeated character - " + self.chipOpData[:i])
                file.write(" \n")
                file.write("{:<15} {:<15} {:<15} {:<15} {:<20} {:<15} {:<15} \n".format('Pre Req','Command Type','Chip Unit Type','Chip Unit Num','Address/StrEqvHash','HashEqvStr','Data Length'))
                file.write("{:<15} {:<15} {:<15} {:<15} {:<20} {:<15} {:<15} \n".format(self.preReq,self.cmdType,self.chipUnitType,self.chipUnitNum,self.cmdAddress,hashEqvStr,self.dataLen))

        file.close()

    def parseFfdcLength(self, ffdcLen):

        #4 bytes of ffdc length - if 0 => no ffdc
        self.ffdcLen = int.from_bytes(ffdcLen, "big")

        #Delete the HB mem dump collected if FFDC is generated.
        if(self.cmdType == "getmempba" and HBMemDump == "true" and dumpType == "HB" and clockState == "ON"):
            if self.ffdcLen != 0x00:
                out.critical("FFDC generated for getmempba. Deleting HB mem dump collected")
                os.system("rm -f %s" % filenames["HBMemDump"])

    def parseFfdcData(self, ffdcData):

        #update stats
        statsAll["allscom"]["ffdcCount"] = statsAll["allscom"]["ffdcCount"] + 1
        stats[self.cmdType]["ffdcCount"] = stats[self.cmdType]["ffdcCount"] + 1

        self.ffdcData = ""
        #FFDC data - length as per ffdc length
        numberOfWords = int(self.ffdcLen / 4)
        for i in range(0,numberOfWords):
            tempFfdcData = int.from_bytes(ffdcData[(4*i):((4*i)+4)], "big")
            tempFfdcData = str(format(tempFfdcData,'08X'))
            self.ffdcData = self.ffdcData + tempFfdcData

    def parseCpuCycles(self, cpuCycles):

        #8Bytes of CPU cycles
        self.cpuCycles = int.from_bytes(cpuCycles, "big")
        global totalCpuCycles
        totalCpuCycles = totalCpuCycles + self.cpuCycles

    def printSingleFifoEntry(self):

        hashEqvStr = "NONE"
        if int(self.cmdAddress,16) in strHashDict.keys():
            hashEqvStr = strHashDict[int(self.cmdAddress,16)]

        preReq = str(format(self.preReq,'02X')) + " (" + utils.preReqCommandStatus[self.preReq] + ")"

        out.print(" ")
        out.print("{:<20} {:<15} {:<15} {:<15} {:<20} {:<15} {:<15}".format('Pre Req','Command Type','Chip Unit Type','Chip Unit Num','Address/StrEqvHash','HashEqvStr','Data Length'))
        out.print("{:<20} {:<15} {:<15} {:<15} {:<20} {:<15} {:<15}".format(preReq,self.cmdType,self.chipUnitType,self.chipUnitNum,self.cmdAddress,hashEqvStr,self.dataLen))
        out.print("Data: 0X%s" % self.chipOpData)
        out.print("FFDC Length: %s" % self.ffdcLen)
        if self.ffdcLen != 0x00:
            out.print("FFDC Data: 0X%s" % self.ffdcData)
        out.print("CPU Cycles: %s = %.8f Sec" % (self.cpuCycles,(self.cpuCycles/sbeClkFreq)))
        out.print(" ")

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################
def printSingleHDCTEntry(hdctEntry):
    hashEqvStr = "NONE"
    if int(hdctEntry.address,16) in strHashDict.keys():
        hashEqvStr = strHashDict[int(hdctEntry.address,16)]

    out.print(" ")
    out.print("{:<45} {:<15} {:<15} {:<15} {:<18} {:<15} {:<15}".format('Dump Types','Command Type','Chip Unit Type','Chip Unit Num','Addr/StrEqvHash','HashEqvStr','Func Args'))
    out.print("{:<45} {:<15} {:<15} {:<15} {:<18} {:<15} {:<15}".format(str(hdctEntry.dumpTypes),hdctEntry.command,str(hdctEntry.chipUnitType),str(hdctEntry.chipUnitNum),hdctEntry.address,hashEqvStr,str(hdctEntry.funcargs)))
    out.print(" ")
    out.print("Additional Details: %s " % hdctEntry.addDetails)
    out.print(" ")

#This function will filer out HDCT entries based on clock state and dump type
def getHdctReqDumpEntries(allHdctEntries, reqDumpType):

    #list which contains all HDCT entries based on the requested dump type and clock state
    reqHdctEntries = list()

    #Unset flag when first clock entry is found in HDCT for Clock OFF state.
    clockOffFlag = True

    utils.fancyPrint(" Collecting HDCT Entries for requested dump type: %s and clock state: %s " % (reqDumpType,clockState), "#", "print")

    for entry in allHdctEntries:
        #Filter HDCT entries based on clock state
        if clockState == "ON" and entry.command == "stopclocks":
            break

        if clockState == "OFF" and entry.command == "stopclocks" and clockOffFlag == True:
            clockOffFlag = False
        elif clockState == "OFF" and clockOffFlag == True:
            continue

        #Filter HDCT Entries based on dump type
        if reqDumpType in entry.dumpTypes:

           reqHdctEntries.append(entry)

           #Update stats
           statsAll["allscom"]["hdctBinEntriesCount"] = statsAll["allscom"]["hdctBinEntriesCount"] + 1
           stats[entry.command]["hdctBinEntriesCount"] = stats[entry.command]["hdctBinEntriesCount"] + 1

           if debugPrints == "on":
                #Print the entry
                utils.fancyPrint(" HDCT.bin %s Entry %s " % (reqDumpType,statsAll["allscom"]["hdctBinEntriesCount"]) , "*", "print")
                printSingleHDCTEntry(entry)

    return reqHdctEntries

def parseFifoOutput(dumpFifoOutputFile):

    file = open(dumpFifoOutputFile, "rb")

    utils.fancyPrint(" Parse fifo.bin " , "#", "print")

    #Read dump header
    dumpHeader = dumpHdr(file.read(12))
    global ekbCommitIDFifoBin
    global ocmbFirstInstCompletionFlag
    ekbCommitIDFifoBin = dumpHeader.HDCTVersion

    while True:

        #Lets check for dump footer and OCMB Header.
        #Lets use footer size as both header and footer are same length
        footerOrHdr = file.read(len(dumpFooter))
        footerOrHdr = binascii.hexlify(bytearray(footerOrHdr)).lower()
        footerOrHdr = footerOrHdr.decode('ascii')

        #Convert dump footer and OCMB Header to ascii eqv hex for comparision
        tempDumpFooter = ''.join(str(format(ord(c), 'x')) for c in dumpFooter)
        tempOCMBHeader = ''.join(str(format(ord(c), 'x')) for c in ocmbHdr)

        if footerOrHdr != tempDumpFooter:

            if footerOrHdr == tempOCMBHeader:
                #If OCMB Header is found then next 4 bytes is OCMB Instance ID.
                ocmbInstance = file.read(4)
                utils.fancyPrint(" OCMB Instance: %s " % ocmbInstance.decode('UTF-8') , "*", "print")
                #Set a flag to stop updating stats after 1st OCMB instance is
                #completed
                if ocmbInstance.decode('UTF-8') == "   1":
                    ocmbFirstInstCompletionFlag = True

            #Shift file pointer footer size times
            if footerOrHdr != tempOCMBHeader:
                file.seek(file.tell() - len(dumpFooter))

            #Parser scom data header
            singleScomData = scomData(file.read(16))

            #Parse scom data
            singleScomData.parseChipOpData(file.read(singleScomData.dataLen))

            #Parse FFDC Length
            singleScomData.parseFfdcLength(file.read(4))

            #If FFDC Length is not equal to zero parse FFDC data
            if singleScomData.ffdcLen != 0x00:
                singleScomData.parseFfdcData(file.read(singleScomData.ffdcLen))

            #Parse CPU Cycles
            singleScomData.parseCpuCycles(file.read(8))

            #Update all fifo parsed entries list
            fifoParseEntriesList.append(singleScomData)

            if debugPrints == "on":
                #Print Single fifo entry
                singleScomData.printSingleFifoEntry()

        else:
            break

    utils.fancyPrint(" Chip-op Footer: %s " % codecs.decode(footerOrHdr, "hex") , "*", "print")

    #Chip op end
    utils.fancyPrint(" Dump Chip Op end " , "*", "print")

    while True:

        chipOpEnd = file.read(4)
        if chipOpEnd != b'':
            chipOpEnd = struct.unpack(">I",chipOpEnd)
            out.print(hex(int(''.join(map(str, chipOpEnd)))))
        else:
            break

    utils.fancyPrint("" , "*", "print", 180)

    file.close()

def printStats():

    utils.fancyPrint(" Parser Stats : Dump Type %s : Clock State %s " % (dumpType,clockState), "#", "print", 180)

    #Print Heading
    utils.fancyPrint("", "-", "print", 180)
    out.print("{:<20} {:<15} {:<15} {:<35} {:<40} {:<35}".format('Scom Type','No: of Scoms','No: of FFDC','No: of HDCT Entries in fifo.bin','No: of %s ' % dumpType + 'HDCT Entries in hdct.bin','No: of HDCT Entries Skipped'))
    utils.fancyPrint("", "-", "print", 180)

    #Print Content
    for key in stats.keys():
        out.print("{:<20} {:<15} {:<15} {:<35} {:<40} {:<35}".format(key.upper(),stats[key]["scomCount"],stats[key]["ffdcCount"],stats[key]["entriesCount"],stats[key]["hdctBinEntriesCount"],(stats[key]["hdctBinEntriesCount"] - stats[key]["entriesCount"])))

    #Print Total
    utils.fancyPrint("", "-", "print", 180)
    out.print("{:<20} {:<15} {:<15} {:<35} {:<40} {:<35}".format("TOTAL",statsAll["allscom"]["scomCount"],statsAll["allscom"]["ffdcCount"],statsAll["allscom"]["entriesCount"],statsAll["allscom"]["hdctBinEntriesCount"],(statsAll["allscom"]["hdctBinEntriesCount"] - statsAll["allscom"]["entriesCount"])))
    utils.fancyPrint("", "-", "print", 180)

def verifyDumpFifoOutput(parsedFifoEntries, parsedHdctEntries):

    utils.fancyPrint(" Verify Fifo dump Content ", "*", "print")

    #Check if EKB commit ID in HDCT.bin is same as in fifo.bin
    if ekbCommitIDHdctBin != ekbCommitIDFifoBin:
        utils.fancyPrint(" EKB Commit ID Mismatch ", "X", "critical")
        out.print("EKB Commit ID in HDCT.bin: %s" % ekbCommitIDHdctBin)
        out.print("EKB Commit ID in FIFO.bin: %s" % ekbCommitIDFifoBin)
        out.critical("Cannot verify fifo.bin. Pass HDCT.bin with same EKB commit ID used during project build")
        out.critical("Skipping Verification of FIFO.bin")
        #Lets skip the verification part if EKB commit ID are different
        return

    #Lets keep a track of the current HDCT entry and fifo entry which we are comparing the result with
    currentHdctEntry = 0
    currentFifoEntry = 0

    #Return if there are no FIFO entries to to parsed
    if len(parsedFifoEntries) == 0:
        out.critical("No FIFO entries found all HDCT entries skipped")
        return

    while len(parsedHdctEntries) > currentHdctEntry:

        if debugPrints == "on":
            utils.fancyPrint(" Current Fifo  Entry ", "*", "print")
            parsedFifoEntries[currentFifoEntry].printSingleFifoEntry()
            utils.fancyPrint(" Expected HDCT Entry ", "*", "print")
            parsedHdctEntries[currentHdctEntry].printSingleEntry()

        if((parsedFifoEntries[currentFifoEntry].cmdType == parsedHdctEntries[currentHdctEntry].command) and
            (parsedFifoEntries[currentFifoEntry].cmdAddress == parsedHdctEntries[currentHdctEntry].address)) and parsedFifoEntries[currentFifoEntry].chipUnitType == parsedHdctEntries[currentHdctEntry].chipUnitType:

            if debugPrints == "on":
                out.print("Current Fifo entry equal to Current HDCT entry: Verified")

            #Check if all FIFO entries are parsed.If all fifo entries are parsed print the pending/skipped HDCT entries and exit
            if currentFifoEntry == len(parsedFifoEntries) - 1 :
                utils.fancyPrint(" All FIFO entries verified ", "*", "print")
                currentHdctEntry = currentHdctEntry + 1
                break

            if parsedFifoEntries[currentFifoEntry].hdctEntry == parsedFifoEntries[currentFifoEntry + 1 ].hdctEntry:
                pass
            else:
                currentHdctEntry = currentHdctEntry + 1

            currentFifoEntry = currentFifoEntry + 1
        else:

            if debugPrints == "on":
                utils.fancyPrint(" Current Fifo entry not equal to Current Hdct entry: Skipping below HDCT entry ", "$", "critical")
                parsedHdctEntries[currentHdctEntry].printSingleEntry()

            #Lets compare with next HDCT entry
            currentHdctEntry = currentHdctEntry + 1

    #Lets check if any HDCT entries are left
    if len(parsedHdctEntries) == currentHdctEntry - 1:
        utils.fancyPrint(" All HDCT Entries Parsed ", "*", "print")
    else:
        #print all pending/skipped HDCT entries
        while len(parsedHdctEntries) > currentHdctEntry:
            if debugPrints == "on":
                utils.fancyPrint(" Skipped HDCT entry ", "$", "critical")
                parsedHdctEntries[currentHdctEntry].printSingleEntry()
            currentHdctEntry = currentHdctEntry + 1

################################## Main ###############################################

################################################
# Command line options
# Create the argparser object
# We'll do it in a way so we can create required/optional cmdline arg groups
argparser = argparse.ArgumentParser(description="Dump FIFO output interpreter tool",
                                 add_help=False,
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog=textwrap.dedent('''
Version: ''' + str(toolVersion) + '''

Examples:  > dump_interpret.py -b <dumpFifoOutputFile> -d <Dump_type> -s <HDCT.bin_file>
           > dump_interpret.py -b <dumpFifoOutputFile -d <Dump_type> -o <output_dir> -s <HDCT.bin_file>
'''))

# Create our group of required cmdline args
reqgroup = argparser.add_argument_group('Required Arguments')
reqgroup.add_argument('-b', '--dumpFifoBinFile', required=True, help="The dump fifo output binary file to be parsed and verified")
reqgroup.add_argument('-d', '--dumpType', required=True, help="The dump type", choices=sbeSupportedDumpTypes)
reqgroup.add_argument('-k', '--clockState', required=True, help="Dump collection Clock State", choices=clockStates)

# Create our group of optional cmdline args
optgroup = argparser.add_argument_group('Optional Arguments')
optgroup.add_argument('-h', '--help', action="help", help="Show this help message and exit")
optgroup.add_argument('-o', '--output', help="Directory to place output")
optgroup.add_argument('-v', '--verifyOutput', help="Verify FIFO Output with HDCT.bin and print skipped entries." , choices=["on","off"],default = "off")
optgroup.add_argument('-f', '--parserPrints', help="Print the Parsed output on to consol", choices=["on","off"],default = "on" )
optgroup.add_argument('-t', '--hdctLookup', help="HDCT Lookup File Path")
optgroup.add_argument('-s', '--hdctBinFile', help="The HDCT.bin file with which fifo output need to be verified")
optgroup.add_argument('-m', '--HBMemDump', help="Extract the HB memory dump into a .bin file(getmempba). Applicable only for HB dump in clock ON state", choices=["true","false"],default = "false" )
optgroup.add_argument('-l', '--log-level', default=out.levels.INFO, type=out.levels.argparse, choices=list(out.levels),
                      help="The output level to send to the log.  INFO is the default")
optgroup.add_argument('-c', '--console-level', default=out.levels.BASE, type=out.levels.argparse, choices=list(out.levels),
                      help="The output level to send to the console.  BASE is the default")

# cmdline loaded up, now parse for it and handle what is found
args = argparser.parse_args()

# Get the dump fifo output file path
dumpFifoOutput = args.dumpFifoBinFile

# Get the dump type
dumpType = args.dumpType

#Get the clock state
clockState = args.clockState

#Get the HDCT.bin file
hdctBinFile = args.hdctBinFile

#Grab the debug prints on or off
debugPrints = args.parserPrints

#Grab if verification of FIFO content is needed or not .
verifyFifoOutput = args.verifyOutput

#Grabe the host boot mem dump option
HBMemDump = args.HBMemDump

#Fetch the HDCT.lookup
filenames["hdctLookupFilePath"] = args.hdctLookup

# Grab your output location and level args right away so we can setup output and logging
# Setup our output directory
# If the user gave us one, use that
# If not, create one in tmp dir
if (args.output == None):
    # If we don't have one, use the default
    outputPath = os.path.join("/tmp/")
else:
    outputPath = args.output

# Create the filename.
filenameBase = "verifyDumpFifoOut"
outputPath = outputPath + "/" + filenameBase + "_" + dumpType + "_" + clockState + "_" + timestamp
# Make sure the path exists
if (not os.path.exists(outputPath)):
    # Create the output dir
    try:
        os.mkdir(outputPath)
    except:
        out.critical("The output path does not exist.  Please check the path and re-run")
        out.critical("Problem creating: %s" % outputPath)
        out.critical("Exception: %s" % sys.exc_info()[0])
        sys.exit(1)
    out.debug("Output path is %s" % outputPath)

filenames["log"] = os.path.join(outputPath, filenameBase + ".log")
filenames["console"] = os.path.join(outputPath, filenameBase + ".console")

#If any ring has A5 as its complete content log that ring info into a
#error file
filenames["RingFailInfo"] = os.path.join(outputPath, "ringFailInfo" + ".txt")
#Delete old file if it exists as we are writing in append mode.
if (os.path.exists(filenames["RingFailInfo"])):
    os.system("rm -f %s" % filenames["RingFailInfo"])

#Extract the HB memory dump (getmempba).
#Will be extracted if dump type is HB and clock state is ON.
filenames["HBMemDump"] = os.path.join(outputPath, "HBMemDump" + ".bin")

# Setup our console output level
out.setConsoleLevel(args.console_level)

# Set our log output level
out.setLogLevel(args.log_level)

# Setup our logging infrastructure
# This has to be done after cmdline args are processed and we know output dirs and suffixes
out.setupLogging(filenames["log"], filenames["console"])

# Log the args the program was called with
out.print("Program Args: " + ' '.join(sys.argv), logOnly=True)
out.print("")

#Grab files if user has not passed on a HDCT.bin.
#This will grab both DD1 and DD2 files internally.
if(hdctBinFile == None):
    out.print("Fetch HDCT.bin from latest BB HW image")
    hdctBinFileTmp = fileSupport.getHdctBinFile(outputPath)
    #Lets Parse only DD2 file.
    hdctBinFile = hdctBinFileTmp["DD2HDCT"]

#Parse the hdct.bin file
utils.fancyPrint(" Parse HDCT.bin " , "#", "print")
(ekbCommitIDHdctBin, allHdctBinEntries) = txtUtils.createHDCTTxt(hdctBinFile)

#Fetch the complete path of HDCT.lookup based on EKB Commit ID
if filenames["hdctLookupFilePath"] == None:
    filenames["hdctLookupFilePath"] = fileSupport.getHdctLookup(ekbCommitIDHdctBin, filenames["hdctLookupFilePath"])
    if filenames["hdctLookupFilePath"] == None:
        out.critical("Unable to find look up file. Please enter -t option")
        sys.exit(1)

out.print("HDCT lookup file path: %s" % filenames["hdctLookupFilePath"])

#Load data from the pickle file which contains string equivalent hash
strHashDict = pickle.load(open(filenames["hdctLookupFilePath"],"rb"))

#NOTE:Print all HDCT decoded entries if required for debug
totalHdctBinEntries = 0
for entry in allHdctBinEntries:
    totalHdctBinEntries = totalHdctBinEntries + 1
    if debugPrints == "on":
        utils.fancyPrint(" HDCT  Entry %s " % totalHdctBinEntries , "*", "print")
        printSingleHDCTEntry(entry)

#Filter out HDCT.bin parsed entries based on the requested dump type
reqHdctBinEntries = getHdctReqDumpEntries(allHdctBinEntries, dumpType)

#Parse the fifo output
parseFifoOutput(dumpFifoOutput)

#Verify the dump fifo output
if verifyFifoOutput == "on":
    verifyDumpFifoOutput(fifoParseEntriesList, reqHdctBinEntries)

#Print the fifo parser output stats
out.print("")
printStats()

#End time
stageEndTime = time.time() - stageStartTime

#Print files path
out.print(" ")
utils.fancyPrint(" INFO " , "#", "print")
out.print("Date and Time: %s" % timestamp + "    " + "Run Duration: %s" % utils.formatTime(stageEndTime))
out.print("For more info all output in: %s/ " % outputPath)
out.print("Log File: %s" % filenames["log"])
out.print("Console File: %s" % filenames["console"])
out.print("Ring Failures File: %s" % filenames["RingFailInfo"])
out.print("HB Mem dump bin file: %s" % filenames["HBMemDump"])
out.print(" ")
out.print("Total CPU Cycles: %s = %.8f Sec" % (totalCpuCycles,(totalCpuCycles/sbeClkFreq)))
out.print(" ")
out.print("Program Ends")
