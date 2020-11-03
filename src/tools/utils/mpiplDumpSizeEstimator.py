#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/mpiplDumpSizeEstimator.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
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
toolVersion = 1.0

# Create a generic time stamp we can use throughout the program
timestamp = datetime.datetime.now().strftime("%Y/%m/%d-%H:%M:%S")

# This tool is only for mpipl dump type
dumpType = "MPIPL"

#Start time
stageStartTime = time.time()

#All HDC.bin decoded entries
allHdctBinEntries = list()

#HDCT.bin entries filtered out based on the requested dump type
reqHdctBinEntries = list()

#Store the EKB commit ID from HDCT.bin for comparision with FIFO.bin EKB Commit ID
ekbCommitIDBin = ""

#hdctBinEntriesCount = Number of HDCT entries found in HDCT.bin for a given scomtype
#All chipUnitType for each command type count is also maintained
stats = {"getscom"       : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0, "eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "putscom"       : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0, "eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "getring"       : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0,"eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "stopclocks"    : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0,  "eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "gettracearray" : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0, "eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "getfastarray"  : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0,"eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "getmempba"     : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0, "eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0},
         "getsram"       : {"hdctBinEntriesCount" : 0, "chip" : 0, "perv" : 0, "iohs" : 0, "nmmu" : 0, "phb" : 0, "pec" : 0, "mc" : 0, "mcc" : 0, "omic" : 0, "pau" : 0, "pauc" : 0, "eq" : 0, "c" : 0, "Estimated Scoms" : 0, "EstimateScomLen" : 0, "totalScomSize" : 0}
         }

#Fifo parser stats and HDCT.bin Stats dict for all scoms types
statsAll = {"allscom" : {"hdctBinEntriesCount" : 0, "Estimated Scoms" : 0, "totalScomSize" : 0}}

#Lets maintain a dic of number of targets available in a single proc for each chipUnitType
#TODO: Update mcc and omic target count with actual count once its implemented in sbe
targetTypeCount = {"chip" : 1, "perv" : 29, "iohs" : 8, "nmmu" : 2, "phb" : 6,
                   "pec" : 2, "mc" : 4, "mcc" : 1, "omic" : 1, "pau" : 6, "pauc" : 4,
                   "eq" : 8, "c" : 32
                  }

#Lets maintain a dict containing single scom that will be streamed out of fifo assuming all scoms have failed with a FFDC length of 128 bytes
#All lenth is mentioned in bytes
#Length of getfastarray, getmempba and getsram are zero as those entries are not found in mpipl hence we can ignore them
singleScomDataLen = { "dataHdr" : 4, "addr" : 8, "dataLen" : 4, "data" : {"getscom" : 8, "putscom" : 0, "stopclocks" : 0, "gettracearray" : 2056 , "getring" : 0, "getfastarray" : 0, "getmempba" : 0, "getsram" : 0}, "ffdcLen" : 4, "ffdcData" : 512, "cpuCycles" : 8 }

#Keep a count of ring length seperately
ringLength = 0

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################

def getHdctReqDumpEntries(allHdctEntries, reqDumpType):

    #list which contains all HDCT entries based on the requested dump type
    reqHdctEntries = list()
    global ringLength

    fancyPrint(" Collecting HDCT Entries for requested dump type: %s " % reqDumpType, "#", "print")

    for entry in allHdctEntries:
        if reqDumpType in entry.dumpTypes:
           reqHdctEntries.append(entry)

           #Update stats
           statsAll["allscom"]["hdctBinEntriesCount"] = statsAll["allscom"]["hdctBinEntriesCount"] + 1
           stats[entry.command]["hdctBinEntriesCount"] = stats[entry.command]["hdctBinEntriesCount"] + 1
           stats[entry.command][entry.chipUnitType] = stats[entry.command][entry.chipUnitType] + 1

           #Update Ring length
           if entry.command == "getring":
               ringLength = ringLength + (entry.addDetails[entry.address]["ringLen"] * targetTypeCount[entry.chipUnitType])

           #Print the entry
           fancyPrint(" HDCT.bin %s Entry %s " % (reqDumpType,statsAll["allscom"]["hdctBinEntriesCount"]) , "*", "print")
           entry.printSingleEntry()

    #Convert the ring length to bytes
    ringLength = int(ringLength / 8)

    return reqHdctEntries

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

def printStats():

    fancyPrint(" MPIPL Parser Stats ", "#", "print", 180)

    #Print Heading
    fancyPrint("", "-", "print", 180)

    out.print("{:<36} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6}".format("Assumed target counts", targetTypeCount["chip"], targetTypeCount["perv"], targetTypeCount["iohs"], targetTypeCount["nmmu"], targetTypeCount["phb"], targetTypeCount["pec"], targetTypeCount["mc"], targetTypeCount["mcc"], targetTypeCount["omic"], targetTypeCount["pau"], targetTypeCount["pauc"],targetTypeCount["eq"], targetTypeCount["c"]))

    fancyPrint("", "-", "print", 180)

    #Print Assumed target type counts
    out.print("{:<15} {:<20} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<18} {:<18} {:<18}".format('Scom Type','HDCT %s ' % dumpType + 'Entries', "chip", "perv", "iohs", "nmmu", "phb", "pec", "mc", "mcc", "omic", "pau", "pauc", "eq", "c", "Estimate ChipOp's","Single ChipOp Len","Total Scom size"))

    fancyPrint("", "-", "print", 180)

    #Print Content
    #Seperate out ring's print as we need to print additional data
    for key in stats.keys():
        if key != "getring":
            out.print("{:<15} {:<20} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<18} {:<18} {:<18}".format(key.upper(),stats[key]["hdctBinEntriesCount"],stats[key]["chip"],stats[key]["perv"],stats[key]["iohs"],stats[key]["nmmu"],stats[key]["phb"],stats[key]["pec"],stats[key]["mc"],stats[key]["mcc"],stats[key]["omic"],stats[key]["pau"],stats[key]["pauc"],stats[key]["eq"],stats[key]["c"],stats[key]["Estimated Scoms"],stats[key]["EstimateScomLen"], stats[key]["totalScomSize"]))
        else:
            out.print("{:<15} {:<20} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<6} {:<18} {:<18} {:<18}".format(key.upper(),stats[key]["hdctBinEntriesCount"],stats[key]["chip"],stats[key]["perv"],stats[key]["iohs"],stats[key]["nmmu"],stats[key]["phb"],stats[key]["pec"],stats[key]["mc"],stats[key]["mcc"],stats[key]["omic"],stats[key]["pau"],stats[key]["pauc"],stats[key]["eq"],stats[key]["c"],stats[key]["Estimated Scoms"],str(stats[key]["EstimateScomLen"]) + " +  Ring Len", stats[key]["totalScomSize"]))

    #Print Total
    fancyPrint("", "-", "print", 180)
    out.print("{:<15} {:<110} {:<38} {:<18}".format("TOTAL",statsAll["allscom"]["hdctBinEntriesCount"],statsAll["allscom"]["Estimated Scoms"], statsAll["allscom"]["totalScomSize"]))
    fancyPrint("", "-", "print", 180)

def printStatsSingleScom():

    #Print Estimate single Chip-op Length
    tempTotal = singleScomDataLen["dataHdr"] + singleScomDataLen["addr"] + singleScomDataLen["dataLen"] + singleScomDataLen["ffdcLen"] + singleScomDataLen["ffdcData"] + singleScomDataLen["cpuCycles"]
    fancyPrint(" Single ChipOp Length(In Bytes) ", "#", "print", 180)
    out.print("Assumed FFDC Data in Bytes: %s" % singleScomDataLen["ffdcData"])

    #Print TOC
    fancyPrint("", "-", "print", 180)
    out.print("{:<15} {:<15} {:<15} {:<15} {:<15} {:<15} {:<15}".format('Data Hdr','Address', "Data Len", "FFDC Len", "FFDC Data", "CPU Cycles", "Total Legth"))

    #Print Content
    fancyPrint("", "-", "print", 180)
    out.print("{:<15} {:<15} {:<15} {:<15} {:<15} {:<15} {:<15}".format(singleScomDataLen["dataHdr"],singleScomDataLen["addr"],singleScomDataLen["dataLen"],singleScomDataLen["ffdcLen"], singleScomDataLen["ffdcData"],singleScomDataLen["cpuCycles"],str(tempTotal)  + " + Data Based on Chip-op Type"))
    fancyPrint("", "-", "print", 180)

    #print the data size based on chip-op
    out.print("")
    out.print("Data Size for Get scom: %s" % singleScomDataLen["data"]["getscom"])
    out.print("Data Size for Put scom: %s" % singleScomDataLen["data"]["putscom"])
    out.print("Data Size for Trace Array: %s" % singleScomDataLen["data"]["gettracearray"])
    out.print("Data Size for Stop Clocks: %s" % singleScomDataLen["data"]["stopclocks"])
    out.print("Data Size for Rings: Ring Length")

################################## Main ###############################################

################################################
# Command line options
# Create the argparser object
# We'll do it in a way so we can create required/optional cmdline arg groups
argparser = argparse.ArgumentParser(description="MPIPL Dump Size Estimator Tool",
                                 add_help=False,
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog=textwrap.dedent('''
Version: ''' + str(toolVersion) + '''

Examples:  > mpiplDumpSizeEstimator.py -s <HDCT.bin_file>
'''))

# Create our group of required cmdline args
reqgroup = argparser.add_argument_group('Required Arguments')
reqgroup.add_argument('-s', '--hdctBinFile', required=True, help="The HDCT.bin file with which MPIPL dump size needs to be estimated")

# Create our group of optional cmdline args
optgroup = argparser.add_argument_group('Optional Arguments')
optgroup.add_argument('-h', '--help', action="help", help="Show this help message and exit")
optgroup.add_argument('-o', '--output', help="Directory to place output")
optgroup.add_argument('-l', '--log-level', default=out.levels.INFO, type=out.levels.argparse, choices=list(out.levels),
                      help="The output level to send to the log.  INFO is the default")
optgroup.add_argument('-c', '--console-level', default=out.levels.BASE, type=out.levels.argparse, choices=list(out.levels),
                      help="The output level to send to the console.  BASE is the default")

# cmdline loaded up, now parse for it and handle what is found
args = argparser.parse_args()

#Get the HDCT.bin file
hdctBinFile = args.hdctBinFile

# Grab your output location and level args right away so we can setup output and logging
# Setup our output directory
# If the user gave us one, use that
# If not, create one in current dir
if (args.output == None):
    # If we don't have one, use the default
    outputPath = os.path.join("./")
else:
    outputPath = args.output

# Create the filename.
filenameBase = "mpiplDumpSizeEstimator"
filenames = dict()
outputPath = outputPath + "/" + filenameBase
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
(ekbCommitIDBin, allHdctBinEntries) = txtUtils.createHDCTTxt(hdctBinFile)

#NOTE:Print all HDCT decoded entries if required for debug
totalHdctBinEntries = 0
for entry in allHdctBinEntries:
    totalHdctBinEntries = totalHdctBinEntries + 1
    fancyPrint(" HDCT  Entry %s " % totalHdctBinEntries , "*", "print")
    entry.printSingleEntry()

#Filter out HDCT.bin parsed entries based on the requested dump type
reqHdctBinEntries = getHdctReqDumpEntries(allHdctBinEntries, dumpType)

#calculate the Estimated scom's,estimate length of each scom and total scom length
for key in stats.keys():
    stats[key]["Estimated Scoms"] = (stats[key]["chip"] * targetTypeCount["chip"]) + (stats[key]["perv"] * targetTypeCount["perv"]) + (stats[key]["iohs"] * targetTypeCount["iohs"]) + (stats[key]["nmmu"] * targetTypeCount["nmmu"]) + (stats[key]["phb"] * targetTypeCount["phb"]) + (stats[key]["pec"] * targetTypeCount["pec"]) +(stats[key]["mc"] * targetTypeCount["mc"]) + (stats[key]["mcc"] * targetTypeCount["mcc"]) + (stats[key]["omic"] * targetTypeCount["omic"]) + (stats[key]["pau"] * targetTypeCount["pau"]) + (stats[key]["pauc"] * targetTypeCount["pauc"]) + (stats[key]["eq"] * targetTypeCount["eq"]) + (stats[key]["c"] * targetTypeCount["c"])

    stats[key]["EstimateScomLen"] = singleScomDataLen["dataHdr"] + singleScomDataLen["addr"] + singleScomDataLen["dataLen"] + singleScomDataLen["data"][key] + singleScomDataLen[ "ffdcLen"] + singleScomDataLen["ffdcData"] + singleScomDataLen["cpuCycles"]

    stats[key]["totalScomSize"] = stats[key]["Estimated Scoms"] * stats[key]["EstimateScomLen"]

    #Add the ring length for rings
    if key == "getring":
        stats[key]["totalScomSize"] = stats[key]["totalScomSize"] + ringLength

    #Total stats counts
    statsAll["allscom"]["Estimated Scoms"] = statsAll["allscom"]["Estimated Scoms"] + stats[key]["Estimated Scoms"]
    statsAll["allscom"]["totalScomSize"] =  statsAll["allscom"]["totalScomSize"] + stats[key]["totalScomSize"] #Totalsize that needs to be allocated to a proc


#Print the single chip-op length stats
printStatsSingleScom()

#Print the fifo parser output stats
out.print(" ")
printStats()

#Print total estimate size that has to be allocated to a proc
out.print("")
totalSizeInMB =  ( statsAll["allscom"]["totalScomSize"] / 1024 ) / 1024
out.print("Total estimate size to be allocated for a single proc for MPIPL dump is: %sMb " % totalSizeInMB)

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
