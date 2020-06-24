# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testGetTIInfo.py $
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
import testScomUtil
import testMemUtil

err = False

i_fifoType = 0

GETTIINFO_TESTDATA = [0, 0, 0, 0x2,
                      0, 0, 0xA9, 0x04]

GETTIINFO_EXPDATA =  [0xc0, 0xde, 0xA9, 0x04]
GETTIINFO_EXPLEN  =  [0, 0, 0x4, 0]

#As of now TI length is 8 bytes.
TI_LEN = 1024
SCRATCH_0 = 0x20028486

def main(expStatus = [0, 0, 0, 0]):
    testUtil.runCycles( 10000000 )

    print("Read the scratch 0 register")
    i_fifoType = 0
    tiLocation = testScomUtil.getscom(SCRATCH_0, i_fifoType)
    print("TI data location is " + hex(tiLocation))

    print("Write TI data to TI location")
    tiData = os.urandom(TI_LEN)
    tiData = [ord(c) for c in tiData]
    testMemUtil.putmem(tiLocation, tiData, 0x02)

    #Get TI Info test.
    testUtil.writeUsFifo(GETTIINFO_TESTDATA, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = GETTIINFO_EXPDATA + expStatus

    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True

    length = TI_LEN
    data = [0]*0
    while length > 0: 
        data += testUtil.readDsEntryReturnVal(i_fifoType)
        length = length - 4

    #Read the length from FIFO.
    testUtil.readDsFifo( GETTIINFO_EXPLEN )

    #Flush out the expected data from FIFO.
    testUtil.readDsFifo(expData, i_fifoType)

    #Flush out distance.
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)

    if(tiData == data):
        print("Success")
    else:
        print(tiData)
        print(data)
        raise Exception('data mistmach')


#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
try:
    main()
except:
    print( "\nTest Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

print("\nTest completed Successfully\n")
