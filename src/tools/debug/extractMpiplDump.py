#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/extractMpiplDump.py $
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
import textwrap
import argparse

##################################### NOTE: DO NOT IMPORT ANT MODULES FROM CTE PATH WHICH USE ECMD LIBRARY/PATH #####################################
#Supporting modules from CTE path
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$CTEPATH")+ "/tools/crondump/dev/pymod"))
import parseMpipl

################################## Main ###############################################

################################################
# Command line options
# Create the argparser object
# We'll do it in a way so we can create required/optional cmdline arg groups
argparser = argparse.ArgumentParser(description="Get MPIPL HW data offset for all procs and nodes for a given Dump",
                                 add_help=False,
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog=textwrap.dedent('''

Examples:  > extractMpiplDump.py -d <mpiplDumpFifoOutputFile>
           > extractMpiplDump.py -d <mpiplDumpFifoOutputFile> -o <outputFilePath>
'''))

# Create our group of required cmdline args
reqgroup = argparser.add_argument_group('Required Arguments')
reqgroup.add_argument('-d', '--mpiplDumpFile', required=True, help="The MPIPL Dump file from which HW data offsets need to be extracted")

# Create our group of optional cmdline args
optgroup = argparser.add_argument_group('Optional Arguments')
optgroup.add_argument('-h', '--help', action="help", help="Show this help message and exit")
optgroup.add_argument('-o', '--output', help="Directory to place output")

# cmdline loaded up, now parse for it and handle what is found
args = argparser.parse_args()

#MPIPL Dump File
mpiplDumpFile = args.mpiplDumpFile

# Create the empty bytearray we'll add all our data to
contents = bytearray()

if (args.output == None):
    # If we don't have one, use the default
    outputPath = os.path.join("./mpipl_dump/")
else:
    outputPath = args.output + "/mpipl_dump/"

if (not os.path.exists(outputPath)):
    # Create the output dir
    try:
        os.mkdir(outputPath)
    except:
        print("The output path does not exist.  Please check the path and re-run")
        print("Problem creating: %s" % outputPath)
        print("Exception: %s" % sys.exc_info()[0])
        sys.exit(1)
print("Output path is %s" % outputPath)

file = open(mpiplDumpFile,"rb")

# Assign the file contents over the variable all code uses
contents = file.read()

stream = parseMpipl.getMpiplHWDataOffset(contents)

if not stream:
    print("HW data not found.")
    print("MPIPL Dump extraction failed")
    sys.exit(1)

for idx in stream:

    # Create a "filename" from the node/pos
    # After the original node/pos, they broke the data into multiple sections
    # That section name is in the 3rd arg.  This supports both.
    if (len(idx) == 3):
        streamFileName = outputPath + "mpipl.%s.node%d.proc%d" % (idx[2], idx[0], idx[1])
    else:
        streamFileName = outputPath + "mpipl.hwdata.node%d.proc%d" % (idx[0], idx[1])

    cmd = "dd skip=" + str(stream[idx]["start"]) + " count=" + str(stream[idx]["end"] - stream[idx]["start"]) + " if=" + mpiplDumpFile  + " of=" + streamFileName + " bs=1"
    os.system(cmd)
    print(streamFileName)

print("Program Ends")
sys.exit(0)
