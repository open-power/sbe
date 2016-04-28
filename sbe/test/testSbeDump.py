# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/test/testSbeDump.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
import sys
sys.path.append("targets/p9_nimbus/sbeTest")
import testUtil
err = False
#from testWrite import *

TESTDATA = [0, 0, 0, 3,
            0, 0, 0xA1, 0x01,
            0, 0x02, 0x00, 0x2]

EXPDATA = [0xc0, 0xde, 0xa1, 0x01]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main():
    testUtil.runCycles(10000000)
    testUtil.writeUsFifo(TESTDATA)
    testUtil.writeEot()
    testUtil.readDsFifo(EXPDATA)
    #flush out primary and secondary status
    data = testUtil.readDsEntryReturnVal()

    #flush hwp ffdc
    data = testUtil.readDsEntryReturnVal()
    data = testUtil.readDsEntryReturnVal()

    #start processing sbe ffdc
    data = testUtil.readDsEntryReturnVal()
    magicBytes = ((data[0] << 8) | data[1])
    if (magicBytes == 0xFFDC) :
        print ("\nMagic Bytes Match")
    else :
        raise Exception('data mistmach')
    packLen = ((data[2] << 8) | data[3])
    print ("\nFFDC package length = " + str(packLen))

    data = testUtil.readDsEntryReturnVal()
    fapiRc = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\nFAPI rc = " + str(hex(fapiRc)))

    data = testUtil.readDsEntryReturnVal()
    primaryStatus = ((data[0] << 8) | data[1])
    secondaryStatus = ((data[2] << 8) | data[3])
    print ("\nPrimary Status " + str(hex(primaryStatus)) + " Secondary Status "\
                                                    + str(hex(secondaryStatus)))

    data = testUtil.readDsEntryReturnVal()
    header = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\nHeader = " + str(hex(header)))

    for i in range(0, (bin(header).count("1"))):
        #read user data id
        data = testUtil.readDsEntryReturnVal()
        id = (data[0] << 8) | data[1]
        print "User data Id ["+str(hex(id))+"]"
        len = (data[2] << 8) | data[3]
        #if it is trace field SBE_FFDC_TRACE_DUMP
        fileName = ""
        if(id == 0x0002):
            fileName = "trace.bin"
            print ("\nlength of trace dump " + str(len))
        #if it is trace field SBE_FFDC_ATTR_DUMP
        elif(id == 0x0001):
            fileName = "attr.bin"
            print ("\nlength of attr dump " + str(len))
        myBin = open(fileName, 'wb')
        print ("\nwriting "+fileName)
        loopCount = (len ) / 4
        for j in range(0, loopCount):
            data = testUtil.readDsEntryReturnVal()
            myBin.write(bytearray(data))
        print("write to a file Done")
        myBin.close()
    #flush out distance
    data = testUtil.readDsEntryReturnVal()
#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
main()

if err:
    print ("\nTest Suite completed with error(s)")
    #sys.exit(1)
else:
    print ("\nTest Suite completed with no errors")
    #sys.exit(0);

