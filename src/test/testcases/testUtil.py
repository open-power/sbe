# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testUtil.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2017
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
import time
import conf
from sim_commands import *

waitItrCount = 10000000;
cyclesPerIter = 20000;
#err = False
lbus = conf.p9Proc0.proc_lbus_map
def writeUsFifo( data):
    """Main test Loop"""
    loopCount = len(data)/4;
    for i in range (loopCount):
        idx = i * 4;
        writeEntry(lbus, 0x2400, (data[idx], data[idx+1], data[idx+2], data[idx+3]) )

def readDsFifo(data):
    """Main test Loop"""
    loopCount = len(data)/4;
    for i in range (loopCount):
        idx = i * 4;
        checkEqual(readEntry(lbus, 0x2440, 4), (data[idx], data[idx+1], data[idx+2], data[idx+3]))

def writeEot():
    write(lbus, 0x2408, (0, 0, 0, 1) )

def write(obj, address, value ):
    """ Write to memory space """
    iface = SIM_get_interface(obj, "memory_space")
    iface.write(None, address, value, 0x0)

def readEot():
    """ Read from memory space """
    status = read(lbus, 0x2444, 4)
    checkEqual( (status[3] & 0x80), 0x80 );
    read(lbus, 0x2440, 4)

def resetFifo():
    write(lbus, 0x240C, (0, 0, 0, 1))
    return

def readUsFifoStatus():
    status = read(lbus, 0x2404, 4)
    return status

def readDsFifoStatus():
    status = read(lbus, 0x2444, 4)
    return status

def waitTillFifoEmpty(func):
    count = 0
    loop = True
    while(loop is True):
        status = func()
        if(status[1] == 0x10):
            loop = False
            break
        else:
            count = count + 1
            runCycles(200000)
            if(count > 10):
                raise Exception('Timed out waiting for FIFO to get flushed')


def waitTillUsFifoEmpty():
    try:
        waitTillFifoEmpty(readUsFifoStatus)
    except:
        raise Exception('US FIFO did not get empty')


def waitTillDsFifoEmpty():
    try:
        waitTillFifoEmpty(readDsFifoStatus)
    except:
        raise Exception('DS FIFO did not get empty')


# This function will only read the entry but will not compare it
# with anything. This can be used to flush out enteries.
def readDsEntry(entryCount):
    for i in range (entryCount):
        readEntry(lbus, 0x2440, 4)

def writeEntry(obj, address, value ):

    loop = 1;
    count = 0;
    while( loop ):
        status = read(lbus, 0x2404, 4)  # Address 0x2404: Upstream Fifo Status

        if( status[2] & 0x02):
            count = count + 1
            runCycles(cyclesPerIter)
            # This will cause  test to fail
            if(count > waitItrCount):
                raise Exception('Timeout. FIFO FULL');
        else:
            # write entry
            write(obj, address, value)
            loop = 0

    return value
def readDsEntryReturnVal():
    data = readEntry(lbus, 0x2440, 4)
    runCycles(200000)
    return data
def readEntry(obj, address, size):

    """ Read from memory space """
    loop = 1;
    count = 0;
    value = (0,0,0,0)
    while( loop ):
        status = read(lbus, 0x2444, 4)  # Address 0x2444: Downstream Fifo Status

        if( status[1] & 0x0F):
            # read entry
            value = read(lbus, address, size)
            loop = 0
        else:
            count = count + 1
            runCycles(cyclesPerIter)
            # This will cause  test to fail
            if(count > waitItrCount):
                raise Exception('Timeout. Empty FIFO');

    return value

def extractHWPFFDC(dumpToFile = False, readData = None):
    '''Header extraction'''
    if(readData != None):
        data = readData[:4]
        readData = readData[4:]
    else:
        data = readDsEntryReturnVal()
    magicBytes = ((data[0] << 8) | data[1])
    if (magicBytes == 0xFFDC) :
        print ("\nMagic Bytes Match")
    else :
        raise Exception('data mistmach')
    packLen = ((data[2] << 8) | data[3])
    print ("\nFFDC package length = " + str(packLen))
    # extract Sequence ID, Command class and command
    if(readData != None):
        data = readData[:4]
        readData = readData[4:]
    else:
        data = readDsEntryReturnVal()
    seqId = ((data[0] << 24) | (data[1] << 16))
    cmdClass = data[2]
    cmd = data[3]
    print ("\n SeqId ["+str(seqId)+"] CmdClass ["+str(cmdClass)+"] Cmd ["+str(cmd)+"]")

    if(readData != None):
        data = readData[:4]
        readData = readData[4:]
    else:
        data = readDsEntryReturnVal()
    fapiRc = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3])
    print ("\nFAPI rc = " + str(hex(fapiRc)))

    if(dumpToFile):
        myBin = open('hwp_ffdc.bin', 'wb')
        print ("\nwriting "+'hwp_ffdc.bin')
    for i in range(0, packLen-3):
        if(readData != None):
            data = readData[:4]
            readData = readData[4:]
        else:
            data = readDsEntryReturnVal()
        if(dumpToFile):
            myBin.write(bytearray(data))
    if(dumpToFile):
        print("write to a file Done")
        myBin.close()
    return readData

def read(obj, address, size):
    """ Read from memory space """
    iface = SIM_get_interface(obj, "memory_space")
    value = iface.read(None, address, size, 0x0)
    return value

def runCycles( cycles ):
    if (not SIM_simics_is_running()):
        syscmd   =   "run-cycles %d"%(cycles)
        ( rc, out )  =   quiet_run_command( syscmd, output_modes.regular )
        if ( rc ):
            print "simics ERROR running %s: %d "%( syscmd, rc )

def checkEqual( data, expdata ):
    """ Throw exception if data is not equal """
    if( cmp(data, expdata )):
        print "Eqality check failed"
        print "Data:", data
        print "Expected Data", expdata
        raise Exception('data mistmach');

