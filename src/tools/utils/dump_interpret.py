#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/dump_interpret.py $
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
##################################### NOTE: DO NOT IMPORT ANT MODULES FROM CTE PATH WHICH USE ECMD LIBRARY/PATH #####################################
#Supporting modules from CTE path
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$CTEPATH")+ "/tools/crondump/dev/pymod"))
import dumpConstants
import out
#HDCT.bin parser
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$SBEROOT")+ "/src/import/systems/p10/hdct"))
import createHdctTxtUtils as txtUtils

#Tool version
toolVersion = 1.1

#Dump Footer
dumpFooter = "DONE"

# Create a generic time stamp we can use throughout the program
timestamp = datetime.datetime.now().strftime("%Y/%m/%d-%H:%M:%S")

#Start time
stageStartTime = time.time()

#SBE Supported Dump types
sbeSupportedDumpTypes = ["SCS","MPIPL","CCS","PERF","HB"]

#Dump Collection Clock States
clockStates = ["ON","OFF"]

#All HDC.bin decoded entries
allHdctBinEntries = list()

#HDCT.bin entries filtered out based on the requested dump type
reqHdctBinEntries = list()

#List of all entries that are parsed by fifo
fifoParseEntriesList = list()

#Store the EKB commit ID from HDCT.bin for comparision with FIFO.bin EKB Commit ID
ekbCommitIDHdctBin = ""
ekbCommitIDFifoBin = ""

#Debug prints - on and off
debugPrints = ""

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

        fancyPrint(" Dump Header " , "*", "print")
        out.print("Data Layout Version: %s" % hex(self.dataLayoutVersion))
        out.print("EKB Commit ID/HDCT version: %s" % self.HDCTVersion.decode('utf8'))
        fancyPrint("" , "*", "print")

class scomData():
    """
    Python usable representation of single scom operation data
    """

    def __init__(self, dataHdr):

        #Data header
        #1 byte of command status
        tempcmdStatus = int.from_bytes(dataHdr[:1], "big")
        #last bit of command status is preReq and rest all bits are unused or reserved
        self.preReq = tempcmdStatus >> 7

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
        if len(fifoParseEntriesList) != 0 and self.cmdAddress != fifoParseEntriesList[-1].cmdAddress:
            statsAll["allscom"]["entriesCount"] = statsAll["allscom"]["entriesCount"] + 1
            stats[self.cmdType]["entriesCount"] = stats[self.cmdType]["entriesCount"] + 1

        stats[self.cmdType]["scomCount"] = stats[self.cmdType]["scomCount"] + 1
        self.hdctEntry = statsAll["allscom"]["entriesCount"]

        if debugPrints == "on":
            fancyPrint(" SCOM %s : HDCT Entry %s " % (statsAll["allscom"]["scomCount"], statsAll["allscom"]["entriesCount"]) , "*", "print")

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
            filenames["HBMemDump"] = os.path.join(outputPath, "HBMemDump" + ".bin")
            out.print("Extracting HB Mem Dump into %s " % filenames["HBMemDump"])
            file = open(filenames["HBMemDump"], "wb")
            for i in range(0,numberOfDoubleWords):
                file.write(scomData[(8*i):((8*i)+8)])
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

    def printSingleFifoEntry(self):

        out.print(" ")
        out.print("{:<15} {:<15} {:<15} {:<15} {:<15} {:<15}".format('Pre Req','Command Type','Chip Unit Type','Chip Unit Num','Address','Data Length'))
        out.print("{:<15} {:<15} {:<15} {:<15} {:<15} {:<15}".format(self.preReq,self.cmdType,self.chipUnitType,self.chipUnitNum,self.cmdAddress,self.dataLen))
        out.print("Data: 0X%s" % self.chipOpData)
        out.print("FFDC Length: %s" % self.ffdcLen)
        if self.ffdcLen != 0x00:
            out.print("FFDC Data: 0X%s" % self.ffdcData)
        out.print("CPU Cycles: %s" % self.cpuCycles)
        out.print(" ")

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################

#This function will filer out HDCT entries based on clock state and dump type
def getHdctReqDumpEntries(allHdctEntries, reqDumpType):

    #list which contains all HDCT entries based on the requested dump type and clock state
    reqHdctEntries = list()

    #Unset flag when first clock entry is found in HDCT for Clock OFF state.
    clockOffFlag = True

    fancyPrint(" Collecting HDCT Entries for requested dump type: %s and clock state: %s " % (reqDumpType,clockState), "#", "print")

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
                fancyPrint(" HDCT.bin %s Entry %s " % (reqDumpType,statsAll["allscom"]["hdctBinEntriesCount"]) , "*", "print")
                entry.printSingleEntry()

    return reqHdctEntries

def parseFifoOutput(dumpFifoOutputFile):

    file = open(dumpFifoOutputFile, "rb")

    fancyPrint(" Parse fifo.bin " , "#", "print")

    #Read dump header
    dumpHeader = dumpHdr(file.read(12))
    global ekbCommitIDFifoBin
    ekbCommitIDFifoBin = dumpHeader.HDCTVersion

    while True:

        #Lets check for dump footer
        footer = file.read(len(dumpFooter))
        footer = binascii.hexlify(bytearray(footer)).lower()
        footer = footer.decode('ascii')

        #Convert dump footer to ascii eqv hex for comparision
        tempDumpFooter = ''.join(str(format(ord(c), 'x')) for c in dumpFooter)
        if footer != tempDumpFooter:

            #Shift file pointer footer size times
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

    fancyPrint(" Chip-op Footer: %s " % codecs.decode(footer, "hex") , "*", "print")

    #Chip op end
    fancyPrint(" Dump Chip Op end " , "*", "print")

    while True:

        chipOpEnd = file.read(4)
        if chipOpEnd != b'':
            chipOpEnd = struct.unpack(">I",chipOpEnd)
            out.print(hex(int(''.join(map(str, chipOpEnd)))))
        else:
            break

    fancyPrint("" , "*", "print", 180)

    file.close()

def printStats():

    fancyPrint(" Parser Stats : Dump Type %s : Clock State %s " % (dumpType,clockState), "#", "print", 180)

    #Print Heading
    fancyPrint("", "-", "print", 180)
    out.print("{:<20} {:<15} {:<15} {:<35} {:<40} {:<35}".format('Scom Type','No: of Scoms','No: of FFDC','No: of HDCT Entries in fifo.bin','No: of %s ' % dumpType + 'HDCT Entries in hdct.bin','No: of HDCT Entries Skipped'))
    fancyPrint("", "-", "print", 180)

    #Print Content
    for key in stats.keys():
        out.print("{:<20} {:<15} {:<15} {:<35} {:<40} {:<35}".format(key.upper(),stats[key]["scomCount"],stats[key]["ffdcCount"],stats[key]["entriesCount"],stats[key]["hdctBinEntriesCount"],(stats[key]["hdctBinEntriesCount"] - stats[key]["entriesCount"])))

    #Print Total
    fancyPrint("", "-", "print", 180)
    out.print("{:<20} {:<15} {:<15} {:<35} {:<40} {:<35}".format("TOTAL",statsAll["allscom"]["scomCount"],statsAll["allscom"]["ffdcCount"],statsAll["allscom"]["entriesCount"],statsAll["allscom"]["hdctBinEntriesCount"],(statsAll["allscom"]["hdctBinEntriesCount"] - statsAll["allscom"]["entriesCount"])))
    fancyPrint("", "-", "print", 180)

def verifyDumpFifoOutput(parsedFifoEntries, parsedHdctEntries):

    fancyPrint(" Verify Fifo dump Content ", "*", "print")

    #Check if EKB commit ID in HDCT.bin is same as in fifo.bin
    if ekbCommitIDHdctBin != ekbCommitIDFifoBin:
        fancyPrint(" EKB Commit ID Mismatch ", "X", "critical")
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
            fancyPrint(" Current Fifo  Entry ", "*", "print")
            parsedFifoEntries[currentFifoEntry].printSingleFifoEntry()
            fancyPrint(" Expected HDCT Entry ", "*", "print")
            parsedHdctEntries[currentHdctEntry].printSingleEntry()

        if((parsedFifoEntries[currentFifoEntry].cmdType == parsedHdctEntries[currentHdctEntry].command) and
            (parsedFifoEntries[currentFifoEntry].cmdAddress == parsedHdctEntries[currentHdctEntry].address)) and parsedFifoEntries[currentFifoEntry].chipUnitType == parsedHdctEntries[currentHdctEntry].chipUnitType:

            if debugPrints == "on":
                out.print("Current Fifo entry equal to Current HDCT entry: Verified")

            #Check if all FIFO entries are parsed.If all fifo entries are parsed print the pending/skipped HDCT entries and exit
            if currentFifoEntry == len(parsedFifoEntries) - 1 :
                fancyPrint(" All FIFO entries verified ", "*", "print")
                currentHdctEntry = currentHdctEntry + 1
                break

            if parsedFifoEntries[currentFifoEntry].hdctEntry == parsedFifoEntries[currentFifoEntry + 1 ].hdctEntry:
                pass
            else:
                currentHdctEntry = currentHdctEntry + 1

            currentFifoEntry = currentFifoEntry + 1
        else:

            if debugPrints == "on":
                fancyPrint(" Current Fifo entry not equal to Current Hdct entry: Skipping below HDCT entry ", "$", "critical")
                parsedHdctEntries[currentHdctEntry].printSingleEntry()

            #Lets compare with next HDCT entry
            currentHdctEntry = currentHdctEntry + 1

    #Lets check if any HDCT entries are left
    if len(parsedHdctEntries) == currentHdctEntry - 1:
        fancyPrint(" All HDCT Entries Parsed ", "*", "print")
    else:
        #print all pending/skipped HDCT entries
        while len(parsedHdctEntries) > currentHdctEntry:
            if debugPrints == "on":
                fancyPrint(" Skipped HDCT entry ", "$", "critical")
                parsedHdctEntries[currentHdctEntry].printSingleEntry()
            currentHdctEntry = currentHdctEntry + 1

def fancyPrint(text, specialCharacter, printLevel, length = 170):

    # Keep length less than the number of characters the screen size can
    # accmodate in a single line -- max = 180

    #TODO: Can be done with setattr by passing attribute as function param
    if(printLevel == "print"):
        out.print(text.center(length, specialCharacter))
    elif(printLevel == "info"):
        out.info(text.center(length, specialCharacter))
    elif(printLevel == "warn"):
        out.warn(text.center(length, specialCharacter))
    elif(printLevel == "debug"):
        out.debug(text.center(length, specialCharacter))
    elif(printLevel == "critical"):
        out.critical(text.center(length, specialCharacter))
    elif(printLevel == "error"):
        out.error(text.center(length, specialCharacter))
    else:
        out.critical("Unknown print level passed")
        sys.exit(1)

def formatTime(timePassed, fraction=True):
    """
    Handles time formatting in common function
    """
    # The time comes out as 0:00:45.3482..
    # We find the break from the full seconds to the fractional seconds
    timeString = str(datetime.timedelta(seconds=timePassed))
    decIdx = timeString.find(".")

    if (fraction):
        # The second half of this is a bit of a mess
        # Convert the decimal string to a float, then round it to two places, then turn it back into a string
        # It has to be a formatted string conversion, a simple str() would turn .10 into .1.  Then remove the "0."
        return timeString[0:decIdx] + ("%.2f" % round(float(timeString[decIdx:]), 2))[1:]
    else:
        return timeString[0:decIdx]

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
reqgroup.add_argument('-s', '--hdctBinFile', required=True, help="The HDCT.bin file with which fifo output need to be verified")

# Create our group of optional cmdline args
optgroup = argparser.add_argument_group('Optional Arguments')
optgroup.add_argument('-h', '--help', action="help", help="Show this help message and exit")
optgroup.add_argument('-o', '--output', help="Directory to place output")
optgroup.add_argument('-f', '--parserPrints', help="Print the Parsed output on to consol", choices=["on","off"],default = "off" )
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

#Grabe the host boot mem dump option
HBMemDump = args.HBMemDump

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
filenames = dict()
outputPath = outputPath + "/" + filenameBase + "_" + dumpType + "_" + clockState
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

# Setup our console output level
out.setConsoleLevel(args.console_level)

# Set our log output level
out.setLogLevel(args.log_level)

# Setup our logging infrastructure
# This has to be done after cmdline args are processed and we know output dirs and suffixes
out.setupLogging(filenames["log"], filenames["console"])

#Parse the hdct.bin file
fancyPrint(" Parse HDCT.bin " , "#", "print")
(ekbCommitIDHdctBin, allHdctBinEntries) = txtUtils.createHDCTTxt(hdctBinFile)

#NOTE:Print all HDCT decoded entries if required for debug
totalHdctBinEntries = 0
for entry in allHdctBinEntries:
    totalHdctBinEntries = totalHdctBinEntries + 1
    if debugPrints == "on":
        fancyPrint(" HDCT  Entry %s " % totalHdctBinEntries , "*", "print")
        entry.printSingleEntry()

#Filter out HDCT.bin parsed entries based on the requested dump type
reqHdctBinEntries = getHdctReqDumpEntries(allHdctBinEntries, dumpType)

#Parse the fifo output
parseFifoOutput(dumpFifoOutput)

#Verify the dump fifo output
verifyDumpFifoOutput(fifoParseEntriesList, reqHdctBinEntries)

#Print the fifo parser output stats
out.print("")
printStats()

#End time
stageEndTime = time.time() - stageStartTime

#Print files path
out.print(" ")
fancyPrint(" INFO " , "#", "print")
out.print("Date and Time: %s" % timestamp + "    " + "Run Duration: %s" % formatTime(stageEndTime))
out.print("For more info all output in: %s/ " % outputPath)
out.print("Log File: %s" % filenames["log"])
out.print("Console File: %s" % filenames["console"])
out.print(" ")
out.print("Program Ends")
