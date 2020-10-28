# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testGetDump.py $
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
import sys
import os
import time
import datetime
import textwrap
sys.path.append("targets/p10_standalone/sbeTest")
import testUtil
import testDumpUtils as dumpUtils
import testOut as out

i_fifoType = 0
i_fifoType1 = 1

#Set the consol level and log level.
console_level = out.levels.INFO
log_level = out.levels.DEBUG

#Create a generic time stamp we can use throughout the program
timestamp = datetime.datetime.now().strftime("%Y/%m/%d-%H:%M:%S")

#Start time
stageStartTime = time.time()

#Dict to store all file names
filenames = dict()

# Make sure the path exists
if (not os.path.exists(dumpUtils.outputPath)):
    # Create the output dir
    try:
        os.mkdir(dumpUtils.outputPath)
    except:
        out.critical("The output path does not exist.  Please check the path and re-run")
        out.critical("Problem creating: %s" % dumpUtils.outputPath)
        out.critical("Exception: %s" % sys.exc_info()[0])
        sys.exit(1)
    out.print("Output path is %s" % dumpUtils.outputPath)

filenames["log"] = os.path.join(dumpUtils.outputPath, dumpUtils.filenameBase + ".log")
filenames["console"] = os.path.join(dumpUtils.outputPath, dumpUtils.filenameBase + ".console")

# Setup our console output level
out.setConsoleLevel(console_level)

# Set our log output level
out.setLogLevel(log_level)

# Setup our logging infrastructure
# This has to be done after cmdline args are processed and we know output dirs and suffixes
out.setupLogging(filenames["log"], filenames["console"])

def main():
    out.print("********************Get Dump******************************")

    out.print("*****FIFO 0*****")
    #Invalid test case's
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["OFF"], dumpUtils.sbeSupportedDumpTypes["INVALID"]), i_fifoType, [0, 0x02, 0, 0x40])
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["INVALID"], dumpUtils.sbeSupportedDumpTypes["PERF"]), i_fifoType, [0, 0x02, 0, 0x19])

    #HB Dump test case
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["ON"], dumpUtils.sbeSupportedDumpTypes["HB"]), i_fifoType)
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["OFF"], dumpUtils.sbeSupportedDumpTypes["HB"]), i_fifoType)

    #MPIPL Dump test case
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["ON"], dumpUtils.sbeSupportedDumpTypes["MPIPL"]), i_fifoType)
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["OFF"], dumpUtils.sbeSupportedDumpTypes["MPIPL"]), i_fifoType)

    out.print("*****FIFO 1*****")
    #Invalid test case's
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["OFF"], dumpUtils.sbeSupportedDumpTypes["INVALID"]), i_fifoType1, [0, 0x02, 0, 0x40])
    dumpUtils.getDump(dumpUtils.getDumpValidCase(dumpUtils.dumpClockStates["INVALID"], dumpUtils.sbeSupportedDumpTypes["PERF"]), i_fifoType1, [0, 0x02, 0, 0x19])
    #TODO:Add test case's if needed for FIFO 1 also

###############################################################################################################################################################

    #Lets Run the dump intrepret tool to verify the FIFO.bin content.
    #This is a seperate tool and will run independent of test case in a different shell
    #Tool is written python 3

    dumpUtils.executeDumpInterpretTool("HB","ON")
    dumpUtils.executeDumpInterpretTool("HB","OFF")
    dumpUtils.executeDumpInterpretTool("MPIPL","ON")
    dumpUtils.executeDumpInterpretTool("MPIPL","OFF")

###############################################################################################################################################################

    #End time
    stageEndTime = time.time() - stageStartTime

    out.print("")
    out.print("Date and Time: %s" % timestamp + "    " + "Run Duration: %s" % dumpUtils.formatTime(stageEndTime))
    out.print("For more info all output in: %s/" % dumpUtils.outputPath)
    out.print("Log File: %s" % filenames["log"])
    out.print("Console File: %s" % filenames["console"])

try:
    main()
except:
    out.print ( "Test Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

out.print( "Test Suite completed with no errors" )
