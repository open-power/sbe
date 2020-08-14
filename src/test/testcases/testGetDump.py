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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil

err = False

GETDUMP_TESTDATA = [0, 0, 0, 0x3,
                    0, 0, 0xAA, 0x01,
                    0x40, 0, 0 ,0]

GETDUMP_EXPDATA =  [0xc0, 0xde, 0xAA, 0x01]

def getDump(i_fifoType, expStatus = [0, 0, 0, 0]):
    testUtil.runCycles( 10000000 )

    #Get Dump chip-op test.
    testUtil.writeUsFifo(GETDUMP_TESTDATA, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = GETDUMP_EXPDATA + expStatus

    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True

    #Flush out the expected data from FIFO.
    print("\n Compare expData")
    testUtil.readDsFifo(expData, i_fifoType)

    #Flush out distance.
    print("\n Flush out distance")
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)

print("********************Get Dump******************************\n")

i_fifoType = 0
getDump(i_fifoType, [0, 0, 0, 0])
print("\n Test completed Successfully\n")

#TODO: Enable it for second FIFO.
