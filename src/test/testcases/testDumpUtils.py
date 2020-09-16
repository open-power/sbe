# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testDumpUtils.py $
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

from __future__ import print_function
import os
import sys
import datetime
sys.path.append("targets/p10_standalone/sbeTest" )
import testOut as out
import testUtil

#Lets maintain a dict of all SBE supported dump types
sbeSupportedDumpTypes = {"SCS"     : 0x01,
                         "MPIPL"   : 0x02,
                         "PERF"    : 0x03,
                         "CCS"     : 0x04,
                         "HB"      : 0x05,
                         "INVALID" : 0xFF
                         }

#Lets maintain a dict of all SBE supported Clock states
dumpClockStates = {"ON"      : 0x01,
                   "OFF"     : 0x02,
                   "INVALID" : 0xFF
                  }

#output path of all files
#Dont use export paths as it might not be available in rain and denali simics
outputPath = "./dump_test_output"

# Create the filename.
filenameBase = "dump"

def getDumpValidCase(clockState, dumpType):

    GETDUMP_TESTDATA = [0, 0, 0, 0x3,
                        0, 0, 0xAA, 0x01,
                        0x00, 0, clockState ,dumpType]

    return GETDUMP_TESTDATA

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

def getDump(data, i_fifoType, expStatus = [0, 0, 0, 0], dumpOutputFilePath = outputPath + "/" + filenameBase):
    testUtil.runCycles( 100000 )

    expData = [0xc0, 0xde, 0xAA, 0x01] + expStatus

    #Get Dump chip-op
    testUtil.writeUsFifo(data, i_fifoType)
    testUtil.writeEot(i_fifoType)

    #Lets verify the fifo output only if a file path is not passed
    #If file path is not mentioned it is a must to pass expStatus for test to pass
    #Else we will redirect all output to a file
    if expStatus != [0, 0, 0, 0]:

        #Flush out the expected data from FIFO.
        out.print(" Compare expData ")
        testUtil.readDsFifo(expData, i_fifoType)

        #Flush out distance.
        print(" Flush out distance ")
        testUtil.readDsEntryReturnVal(i_fifoType)
        testUtil.readEot(i_fifoType)

        return None

    #Invert the sbeSupportedDumpTypes keys for adding dump type in file name
    invertKeysDumpType = {v: k for k, v in sbeSupportedDumpTypes.items()}

    #Invert the dumpClockStates keys for adding clock state in file name
    invertKeysClkState = {v: k for k, v in dumpClockStates.items()}

    dumpOutputFile = os.path.join(dumpOutputFilePath + "_Fifo" + str(i_fifoType) + "_" + invertKeysDumpType[data[11]] + "_clk_" + invertKeysClkState[data[10]] + ".bin")
    #Lets open a .bin file and redirect all fifo output into it
    dumpBinFile = open(dumpOutputFile, "wb")

    out.print("Redirecting all fifo: %s " % i_fifoType + "output to %s" % dumpOutputFile)

    while not testUtil.checkIfEot(i_fifoType):
        data = testUtil.readDsEntryReturnVal(i_fifoType)
        dumpBinFile.write(bytearray(data))
        out.print(str(data))

    dumpBinFile.close()

