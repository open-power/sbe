# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testSbeDump.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2019
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False
import testScomUtil
#from testWrite import *

TESTDATA = [0, 0, 0, 2,
            0, 0, 0xA8, 0x01]

EXPDATA = [0xc0, 0xde, 0xa8, 0x01,
           0x0, 0x0, 0x0, 0x0,
           0x0, 0x0, 0x0, 0x3]

i_fifoType = 0

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main():
    testUtil.runCycles(10000000)

    # Generate FSPI rc
    testScomUtil.getscom(0x0A000027, i_fifoType, [0x00, 0xFE, 0x00, 0x11], True)

    testUtil.writeUsFifo(TESTDATA, i_fifoType)
    testUtil.writeEot(i_fifoType)

    print ("\n HWP internal ffdc")
    testUtil.extractHWPFFDC(i_fifoType, True )

    print ("\n SBE internal ffdc")
    data = testUtil.readDsEntryReturnVal(i_fifoType)
    magicBytes = ((data[0] << 8) | data[1])
    if (magicBytes == 0xFFDC) :
        print ("\nMagic Bytes Match")
    else :
        raise Exception('data mistmach')
    packLen = ((data[2] << 8) | data[3])
    print ("\nFFDC package length = " + str(packLen))
    # extract Sequence ID, Command class and command
    data = testUtil.readDsEntryReturnVal(i_fifoType)
    seqId = ((data[0] << 24) | (data[1] << 16))
    cmdClass = data[2]
    cmd = data[3]
    print ("\n SeqId ["+str(seqId)+"] CmdClass ["+str(cmdClass)+"] Cmd ["+str(cmd)+"]")

    data = testUtil.readDsEntryReturnVal(i_fifoType)
    fapiRc = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\nFAPI rc = " + str(hex(fapiRc)))

    data = testUtil.readDsEntryReturnVal(i_fifoType)
    primaryStatus = ((data[0] << 8) | data[1])
    secondaryStatus = ((data[2] << 8) | data[3])
    print ("\nPrimary Status " + str(hex(primaryStatus)) + " Secondary Status "\
                                                    + str(hex(secondaryStatus)))

    data = testUtil.readDsEntryReturnVal(i_fifoType)
    commitID = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\ncommitID = " + str(hex(commitID)))

    data = testUtil.readDsEntryReturnVal(i_fifoType)
    ddLevel = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\nddLevel = " + str(hex(ddLevel)))

    data = testUtil.readDsEntryReturnVal(i_fifoType)
    header = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\nHeader = " + str(hex(header)))

    for i in range(0, (bin(header).count("1"))):
        #read user data id
        data = testUtil.readDsEntryReturnVal(i_fifoType)
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
            data = testUtil.readDsEntryReturnVal(i_fifoType)
            myBin.write(bytearray(data))
        print("write to a file Done")
        myBin.close()

    print ("Read the Expected data")
    testUtil.readDsFifo( EXPDATA, i_fifoType )
    print ("Read Eot")
    testUtil.readEot(i_fifoType)

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
try:
    main()
except:
    print ( "\nTest Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

print ( "\nTest Suite completed with no errors" )

