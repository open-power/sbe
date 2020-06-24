# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testUtil.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2020
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
import time
import conf
from sim_commands import *

simicsObj = simics.SIM_run_command("get-component-list -all proc_p10")

waitItrCount = 10000000;
cyclesPerIter = 20000;

#FIFO1 Constants
US_FIFO_ONE_BASE_ADDR = 0x2400
DS_FIFO_ONE_BASE_ADDR = 0x2440

#FIFO2 Constants
US_FIFO_TWO_BASE_ADDR = 0x2480
DS_FIFO_TWO_BASE_ADDR = 0x24C0

US_FIFO_WRITE  = 0x0000
US_FIFO_STATUS = 0x0004
US_FIFO_EOT    = 0x0008
US_FIFO_RESET  = 0x000C

DS_FIFO_READ   = 0x0000
DS_FIFO_STATUS = 0x0004
DS_FIFO_EOT    = 0x0008
    
def getLbus( node, proc=0):
    #Node is 0 by default
    if(node == 0):
        lbus=SIM_get_object(simicsObj[proc] + ".cfam_cmp.lbus_map")
    #TODO: Simcics does not have object API for different modules.
    #For now i am calling the zero module.
    if(node == 1):
        lbus=SIM_get_object(simicsObj[0] + ".cfam_cmp.lbus_map")
    if(node == 2):
        lbus=SIM_get_object(simicsObj[0] + ".cfam_cmp.lbus_map")
    if(node == 3):
        lbus=SIM_get_object(simicsObj[0] + ".cfam_cmp.lbus_map")
   
    return lbus

#Default parameters are for single node, node 0
def writeUsFifo( data, i_fifoType=0, node=0, proc=0):
    """Main test Loop"""
    lbus = getLbus(node, proc)
    loopCount = len(data)/4;
    address = getUsFifoDataAddrToWrite(i_fifoType)  #Address: 0x2400, 0x2480
    for i in range (loopCount):
        idx = i * 4;
        writeEntry(lbus, address, i_fifoType,\
                   (data[idx], data[idx+1], data[idx+2], data[idx+3]), node, proc )

#Default parameters are for single node, node 0
def readDsFifo(data,  i_fifoType=0, node=0, proc=0):
    """Main test Loop"""
    lbus = getLbus(node, proc)
    loopCount = len(data)/4;
    read_ds_addr = getDsFifoDataAddrToRead(i_fifoType)  #Address: 0x2440, 0x24C0
    for i in range (loopCount):
        idx = i * 4;
        checkEqual(readEntry(lbus, read_ds_addr, 4, i_fifoType, node, proc), (data[idx],\
                                         data[idx+1], data[idx+2], data[idx+3]))

#Default parameters are for single node, node 0
def writeEot(i_fifoType=0, node=0,  proc=0):
    lbus = getLbus(node, proc)
    eot_addr = getUsFiFoEotAddrToWrite(i_fifoType)  #Address: 0x2408, 0x2448
    write(lbus, eot_addr, (0, 0, 0, 1) )

def write(obj, address, value ):
    """ Write to memory space """
    iface = SIM_get_interface(obj, "memory_space")
    iface.write(None, address, value, 0x0)

#Default parameters are for single node, node 0
def readEot(i_fifoType=0, node=0, proc=0):
    """ Read from memory space """
    lbus = getLbus(node, proc)
    eot_addr = getDsFifoStatusAddrToRead(i_fifoType) #Address: 0x2444, 0x2484
    read_addr = getDsFifoDataAddrToRead(i_fifoType)  #Address: 0x2440, 0x2480
    status = read(lbus, eot_addr, 4)
    checkEqual( (status[3] & 0x80), 0x80 );
    read(lbus, read_addr, 4)

#Default parameters are for single node, node 0
def resetFifo(i_fifoType=0, node=0, proc=0):
    lbus = getLbus(node, proc)
    reset_addr = getResetAddr(i_fifoType) #Address:0x240C, 0x248C
    write(lbus, reset_addr, (0, 0, 0, 1))
    return

#Default parameters are for single node, node 0
def readUsFifoStatus(i_fifoType=0, node=0, proc=0):
    lbus = getLbus(node, proc)
    read_addr = getUsFifoStatusAddrToRead(i_fifoType) #Address:0x2404, 0x2484
    status = read(lbus, read_addr, 4)
    return status

#Default parameters are for single node, node 0
def readDsFifoStatus(i_fifoType=0, node=0, proc=0):
    lbus = getLbus(node, proc)
    read_addr = getDsFifoStatusAddrToRead(i_fifoType) #Address:0x2444, 0x24C4
    status = read(lbus, read_addr, 4)
    return status

def waitTillFifoEmpty(func, i_fifoType=0, node=0, proc=0):
    count = 0
    loop = True
    while(loop is True):
        status = func(i_fifoType, node, proc)
        if(status[1] == 0x10):
            loop = False
            break
        else:
            count = count + 1
            runCycles(200000)
            if(count > 10):
                raise Exception('Timed out waiting for FIFO to get flushed')


def waitTillUsFifoEmpty(i_fifoType=0, node=0, proc=0):
    try:
        waitTillFifoEmpty(readUsFifoStatus, i_fifoType, node, proc)
    except:
        raise Exception('US FIFO did not get empty')


def waitTillDsFifoEmpty(i_fifoType=0, node=0, proc=0):
    try:
        waitTillFifoEmpty(readDsFifoStatus, i_fifoType, node, proc)
    except:
        raise Exception('DS FIFO did not get empty')


# This function will only read the entry but will not compare it
# with anything. This can be used to flush out enteries.
#Default parameters are for single node, node 0
def readDsEntry(entryCount, i_fifoType=0, node=0, proc=0):
    lbus = getLbus(node, proc)
    read_addr = getDsFifoDataAddrToRead(i_fifoType) #Address:0x2440, 0x24C0
    for i in range (entryCount):
        readEntry(lbus, read_addr, 4, i_fifoType, node, proc)

#Default parameters are for single node, node 0
def writeEntry(obj, address, i_fifoType, value, node=0, proc=0):
    lbus = getLbus(node, proc)
    loop = 1;
    count = 0;
    status_addr = getUsFifoStatusAddrToRead(i_fifoType)
    while( loop ):
        status = read(lbus, status_addr, 4)  # Address 0x2404, 0x2484

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

#Default parameters are for single node, node 0
def readDsEntryReturnVal(i_fifoType, node=0, proc=0):
    lbus = getLbus(node, proc)
    read_addr = getDsFifoDataAddrToRead(i_fifoType) #Address:0x2440, 0x24C0
    data = readEntry(lbus, read_addr, 4, i_fifoType, node, proc)
    runCycles(200000)
    return data

#Default parameters are for single node, node 0
def readEntry(obj, address, size, i_fifoType=0, node=0, proc=0):
    """ Read from memory space """
    lbus = getLbus(node, proc)
    loop = 1;
    count = 0;
    value = (0,0,0,0)
    read_addr = getDsFifoStatusAddrToRead(i_fifoType)
    while( loop ):
        status = read(lbus, read_addr, 4)  # Address 0x2444, 0x24C4: Downstream Fifo Status

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

def extractHWPFFDC(i_fifoType=0, dumpToFile = False, readData = None, node=0, proc=0):
    '''Header extraction'''
    if(readData != None):
        data = readData[:4]
        readData = readData[4:]
    else:
        data = readDsEntryReturnVal(i_fifoType, node, proc)
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
        data = readDsEntryReturnVal(i_fifoType, node, proc)
    seqId = ((data[0] << 24) | (data[1] << 16))
    cmdClass = data[2]
    cmd = data[3]
    print ("\n SeqId ["+str(seqId)+"] CmdClass ["+str(cmdClass)+"] Cmd ["+str(cmd)+"]")

    if(readData != None):
        data = readData[:4]
        readData = readData[4:]
    else:
        data = readDsEntryReturnVal(i_fifoType, node, proc)
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
            data = readDsEntryReturnVal(i_fifoType, node, proc)
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
            print("simics ERROR running %s: %d "%( syscmd, rc ))

def checkEqual( data, expdata ):
    """ Throw exception if data is not equal """
    if( cmp(data, expdata )):
        print("Eqality check failed")
        print("Data:", data)
        print("Expected Data", expdata)
        raise Exception('data mistmach');

def collectFFDC():
        simics.SIM_run_command('sbe-trace 0')
        simics.SIM_run_command('sbe-stack 0')
        simics.SIM_run_command('sbe-regffdc 0')
        simics.SIM_run_command(simicsObj[0] + '.pib_cmp.sbe_ppe->ppe_state')
        simics.SIM_run_command(simicsObj[0] + '.cfam_cmp.sbe_fifo->upstream_hw_fifo')
        simics.SIM_run_command(simicsObj[0] + '.cfam_cmp.sbe_fifo->downstream_hw_fifo')

def getUsFifoDataAddrToWrite(i_fifoType):
    if i_fifoType == 0:
        address = US_FIFO_ONE_BASE_ADDR + US_FIFO_WRITE
    if i_fifoType == 1:
        address = US_FIFO_TWO_BASE_ADDR + US_FIFO_WRITE
    return address

def getDsFifoDataAddrToRead(i_fifoType):
    if i_fifoType == 0:
        address = DS_FIFO_ONE_BASE_ADDR + DS_FIFO_READ
    if i_fifoType == 1:
        address = DS_FIFO_TWO_BASE_ADDR + DS_FIFO_READ
    return address

def getUsFiFoEotAddrToWrite(i_fifoType):
    if i_fifoType == 0:
        address = US_FIFO_ONE_BASE_ADDR + US_FIFO_EOT
    if i_fifoType == 1:
        address = US_FIFO_TWO_BASE_ADDR + US_FIFO_EOT
    return address

def getResetAddr(i_fifoType):
    if i_fifoType == 0:
        address = US_FIFO_ONE_BASE_ADDR + US_FIFO_RESET
    if i_fifoType == 1:
        address = US_FIFO_TWO_BASE_ADDR + US_FIFO_RESET
    return address

def getUsFifoStatusAddrToRead(i_fifoType):
    if i_fifoType == 0:
        address = US_FIFO_ONE_BASE_ADDR + US_FIFO_STATUS
    if i_fifoType == 1:
        address =  US_FIFO_TWO_BASE_ADDR + US_FIFO_STATUS
    return address

def getDsFifoStatusAddrToRead(i_fifoType):
    if i_fifoType == 0:
        address = DS_FIFO_ONE_BASE_ADDR + DS_FIFO_STATUS
    if i_fifoType == 1:
        address = DS_FIFO_TWO_BASE_ADDR + DS_FIFO_STATUS
    return address
