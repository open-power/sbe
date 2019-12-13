#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/sbe-debug.py $
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
import os
import subprocess
import re
import random
import getopt
import sys
import binascii
import struct

from ctypes import *
import ctypes

err = False

baseAddr = 0xfffe8000
syms = {};
SBE_TOOLS_PATH = os.getcwd()
if 'SBE_TOOLS_PATH' in os.environ:
    SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];

# preference is SBE_TOOLS_PATH, current woring directory and then system path
PATH = SBE_TOOLS_PATH+os.pathsep+os.getcwd()+os.pathsep+os.environ['PATH']

target = 'HW'
node = 0
proc = 0
ddsuffix = 'DD2'
file_path = ""
output_path = os.getcwd()+"/"

'''
-----------------------------------------------------------------------------
                        Internal helper functions
-----------------------------------------------------------------------------
'''
def getFilePath(filename):
    for dir in PATH.split(os.pathsep):
        retPath = os.path.join(dir, filename)
        if(os.path.exists(retPath)):
            return os.path.abspath(retPath)
    print "ERROR: file " + filename +" not found"
    return 1

def getTraceFilePath():
    fspTrace = getFilePath("fsp-trace")
    if(fspTrace == None):
        fspTrace = "fsp-trace"
    return fspTrace

def fillSymTable():
    try:
        if (target == 'AWAN'):
            symFile = getFilePath("sim.sbe.syms")
        else:
            symFile = getFilePath("sbe_"+ddsuffix+".syms")
        f = open( symFile, 'r' )
        for line in f:
            words = line.split()
            if( len( words ) == 4 ):
                syms[words[3]] = [words[0], words[1]]
    except:
        print "Symbol file not found, limited commands avaliable"

def getSymbolInfo( symbol ):
    symAddr = ''
    length = 0
    for key, val in syms.items():
        if(re.search(symbol, key)!= None):
            symAddr = val[0]
            length = val[1]

    if(ddsuffix == 'AXONE'):
        baseAddr = 0xfffc8000
    else:
        baseAddr = 0xfffe8000
    
    print "\n symAddress :", symAddr
    print "\n baseAddress :", baseAddr
    offset = int(symAddr, base = 16) - baseAddr;
    return (hex(offset), length)

def createPibmemDumpFile( offset, length, input_file=None):
    if input_file is None:
        input_file = file_path
    fileHandle = open(input_file)
    fileHandle.seek(int(offset, 16))
    fileData = fileHandle.read(int(length, 16))
    fileHandle.close()
    fileHandle = open(output_path +"DumpPIBMEM", 'w')
    fileHandle.write(fileData)
    fileHandle.close()

def createPibmemDump(offset, length):
    invokeOsCmd( getFilePath("p9_pibmem_dump_wrap.exe")+
            " -quiet -start_byte "+str(offset)+
            " -num_of_byte " + length +
            " -n" + str(node) + " -p" + str(proc)+ " -path "+output_path +
            " -ecc_enable")


def getSymbolVal(symbol, hexdump=False):
    (offset, length) = getSymbolInfo( symbol )
    if(length != 0):
        if(target == 'FILE'):
            createPibmemDumpFile(offset, length)
        else:
            createPibmemDump( offset, length )
        if hexdump:
            invokeOsCmd( "hexdump -C "+ output_path +"DumpPIBMEM" )
    else:
        print "symbol not found"

def invokeOsCmd(cmd):
    print "Running command [" + cmd + "]"
    rc = os.system( cmd )
    if ( rc ):
       print "ERROR running command: %d " % ( rc )
       return 1

'''
--------------------------------------------------------------------------------
                Command functions
--------------------------------------------------------------------------------
'''
def collectTrace():
    getSymbolVal('g_pk_trace_buf' )

    invokeOsCmd( getFilePath("ppe2fsp")+ " " + output_path + "DumpPIBMEM "+\
            output_path +"sbetrace.bin " )
    invokeOsCmd( getTraceFilePath() + " -s " +
            getFilePath("sbeStringFile_"+ddsuffix)+ " "+ output_path +"sbetrace.bin > "+\
            output_path+"sbe_"+str(node)+"_"+str(proc)+"_tracMERG" )
    invokeOsCmd( "mv " + output_path +"DumpPIBMEM "+\
            output_path +"dumpPibMem_trace" )

# Can be used if proper sbe symbol files are not available
def forcedCollectTrace():
    # Collect entire PIBMEM
    offset = "0x00" # PIBMEM BASE
    if(ddsuffix == 'AXONE'):
        length = "0x37e00"
    else:
        length = "0x16400"
    if(target == 'FILE'):
        createPibmemDumpFile( offset, length );
    else:
        createPibmemDump( offset, length )

    # find offset of trace buffer in PIBMEM dump
    # version = 0x00 0x02
    # rsvd = 0x00 0x00
    # image_str = "sbe_seeprom_DD1\0"
    image_str = "SBE_TRACE"
    trace_pattern = [0, 2, 0, 0] + [ord(a) for a in image_str] + [0]
    data_read     = [0] * len(trace_pattern)
    trace_index = 0
    #print trace_pattern
    with open(output_path+"DumpPIBMEM", "r") as f:
        byte = 'x'
        while byte != "":
            byte = f.read(1)
            data_read = data_read[1:]+[ord(byte)]
            #print [chr(a) for a in data_read]
            if data_read == trace_pattern:
                trace_index -= len(trace_pattern)-1
                break
            trace_index += 1
        f.close()

    offset = "0x%x" % (trace_index)
    length = "0x1000"
    createPibmemDumpFile( offset, length, output_path+"DumpPIBMEM");
    invokeOsCmd( getFilePath("ppe2fsp")+ " " + output_path + "DumpPIBMEM "+\
            output_path +"sbetrace.bin " )

    invokeOsCmd( getTraceFilePath() + " -s " +
            getFilePath("sbeStringFile_"+ddsuffix)+" "+ output_path +"sbetrace.bin > "+\
            output_path+"sbe_"+str(node)+"_"+str(proc)+"_tracMERG" )
    invokeOsCmd( "mv "+ output_path +"DumpPIBMEM "+\
            output_path +"dumpPibMem_trace" )

def collectAttr():
    if (target == 'AWAN'):
        sbeImgFile = "p9n_10.sim.sbe_seeprom.bin"
    else:
        sbeImgFile = "sbe_seeprom_"+ddsuffix+".bin"
    getSymbolVal( 'G_sbe_attrs' )
    invokeOsCmd( "mv "+ output_path + "DumpPIBMEM " +\
            output_path +"sbeAttr.bin" )
    invokeOsCmd( getFilePath("p9_xip_tool")+" "+getFilePath(sbeImgFile) + " -ifs attrdump "+ output_path +"sbeAttr.bin > "+\
             output_path+"sbe_"+str(node)+"_"+str(proc)+"_attrs")

def collectStackUsage():
    threads = ('sbeSyncCommandProcessor_stack',
               'sbeCommandReceiver_stack',
               'sbe_Kernel_NCInt_stack',
               'sbeAsyncCommandProcessor_stack')
    for thread in threads:
        getSymbolVal( thread )

        # Dump stack memory to binary file
        invokeOsCmd( "cat "+ output_path +"DumpPIBMEM >"+ output_path + thread )

    print "==================================Stack usage==================================="
    print "Thread".ljust(40)+"Least Available[bytes]".ljust(30)+"Max usage[%]"
    for thread in threads:
        with open(output_path+thread, "rb") as f:
            word = struct.unpack('I', f.read(4))[0]
            leastAvailable = 0
            while (1):
                if (word == int("0xEFCDAB03", 16)):
                    leastAvailable += 4
                    try:
                        word = struct.unpack('I', f.read(4))[0]
                    except:
                        break
                else:
                    break
            print str("["+thread+"]").ljust(40) + str(leastAvailable).ljust(30) + str("%.2f" % (100 * (1 - (leastAvailable/float(int("0x"+syms[thread][1], 16))))))

def getSbeCommit():
    getSymbolVal( 'SBEGlobalsSingleton.*fwCommitId' )

    with open(output_path+"DumpPIBMEM", "rb") as f:
        # Big Endian word
        word = struct.unpack('>I', f.read(4))[0]
        print "SBE commit:", hex(word)

def ppeStateFfdc():
    # Keep this structure in sync with src/sbefw/core/sbeirqregistersave.H
    class registersave_t(BigEndianStructure):
        _fields_ = [
                ("version", c_uint16),
                ("magicbyte", c_uint8),
                ("validbyte", c_uint8),
                ("register_SRR0", c_uint32),
                ("register_SRR1", c_uint32),
                ("register_ISR", c_uint32),
                ("register_FI2C_CONFIG_LOWER_32BITS", c_uint32),
                ("register_FI2C_CONFIG_UPPER_32BITS", c_uint32),
                ("register_FI2C_STAT_LOWER_32BITS", c_uint32),
                ("register_FI2C_STAT_UPPER_32BITS", c_uint32),
                ("register_LR", c_uint32),
                ]
    ppestateffdc = registersave_t()

    getSymbolVal( '__g_register_ffdc' )
    with open(output_path+"DumpPIBMEM", "rb") as f:
        data = f.read()
        ctypes.memmove(ctypes.addressof(ppestateffdc), data, ctypes.sizeof(ppestateffdc))

        print '''
---------------------------------------------------------------------------------
Please verify sbe_DD2.syms is valid for this dump, before depending on the values
---------------------------------------------------------------------------------'''
        if ppestateffdc.validbyte == 0x1 and ppestateffdc.magicbyte == 0xA5:
            print "ppestateffdc.version                             0x%04X" % ppestateffdc.version
            print "ppestateffdc.magicbyte                           0x%02X" % ppestateffdc.magicbyte
            print "ppestateffdc.validbyte                           0x%02X" % ppestateffdc.validbyte
            print "ppestateffdc.register_SRR0                       0x%08X" % ppestateffdc.register_SRR0
            print "ppestateffdc.register_SRR1                       0x%08X" % ppestateffdc.register_SRR0
            print "ppestateffdc.register_ISR                        0x%08X" % ppestateffdc.register_ISR
            print "ppestateffdc.register_FI2C_CONFIG_LOWER_32BITS   0x%08X" % ppestateffdc.register_FI2C_CONFIG_LOWER_32BITS
            print "ppestateffdc.register_FI2C_CONFIG_UPPER_32BITS   0x%08X" % ppestateffdc.register_FI2C_CONFIG_UPPER_32BITS
            print "ppestateffdc.register_FI2C_STAT_LOWER_32BITS     0x%08X" % ppestateffdc.register_FI2C_STAT_LOWER_32BITS
            print "ppestateffdc.register_FI2C_STAT_UPPER_32BITS     0x%08X" % ppestateffdc.register_FI2C_STAT_UPPER_32BITS
            print "ppestateffdc.register_LR                         0x%08X" % ppestateffdc.register_LR
        else:
            print "Register ffdc is not valid, probably SBE has not hit any internal halt condition"
        print '''---------------------------------------------------------------------------------'''

def ppeState():
    if(target == 'FILE'):
        regNameMapSPR = {
                         # SPRs and XIR
                         9 : ["CTR", False],
                         316 : ["DACR", False],
                         308 : ["DBCR", False],
                         22 : ["DEC", False],
                         61 : ["EDR", False],
                         63 : ["IVPR", False],
                         62 : ["ISR", False],
                         8 : ["LR", False],
                         286 : ["PIR", False],
                         287 : ["PVR", False],
                         272 : ["SPRG0", False],
                         26 : ["SRR0", False],
                         27 : ["SRR1", False],
                         340 : ["TCR", False],
                         336 : ["TSR", False],
                         1 : ["XER", False],
                         4200 : ["XSR", False],
                         2 : ["IAR", False],
                         3 : ["IR", False],
                         42 : ["MSR", False],
                         420 : ["CR", False],
                        0x800:["FI2C_CFG", False],
                        0x820:["FI2C_STAT", False],
                        0x860:["FI2C_SCFG0", False],
                        0x880:["FI2C_SCFG1", False],
                        0x8A0:["FI2C_SCFG2", False],
                        0x8C0:["FI2C_SCFG3", False],
                        0x1000:["SBE_SCRATCH0", False],
                        0x1020:["SBE_SCRATCH1", False],
                        0x1040:["SBE_SCRATCH2", False],
                        0x1060:["SBE_SCRATCH3", False],
                        0x2000:["SBE_MISC", False],
                        0x0000:["SBE_EISR", False],
                        0x0020:["SBE_EIMR", False],
                        0x0040:["SBE_EIPR", False],
                        0x0060:["SBE_EITR", False],
                        0x0080:["SBE_EISTR", False],
                        0x00A0:["SBE_EINR", False],
                        0x0100:["SBE_TSEL", False],
                        0x0120:["SBE_DBG", False],
                        0x0140:["SBE_TBR", False],
                        0x0160:["SBE_IVPR", False],
                         }

        regNameMapGPR = {
                         # GPRs
                         0 : ["R0", False] ,
                         1 : ["R1", False] ,
                         2 : ["R2", False] ,
                         3 : ["R3", False] ,
                         4 : ["R4", False] ,
                         5 : ["R5", False] ,
                         6 : ["R6", False] ,
                         7 : ["R7", False] ,
                         8 : ["R8", False] ,
                         9 : ["R9", False] ,
                         10 : ["R10", False],
                         13 : ["R13", False],
                         28 : ["R28", False],
                         29 : ["R29", False],
                         30 : ["R30", False],
                         31 : ["R31", False]
                     }
        print "File path: ", file_path
        fileHandle = open(file_path)
        l_cnt = 0
        print '********************************************************************'
        print 'Reg'.ljust(15),'Reg Value'.ljust(20)
        print '--------------------------------------------------------------------'
        while(l_cnt < os.path.getsize(file_path)):
            regNum = int(binascii.hexlify(fileHandle.read(2)), 16)
            str1 = ''
            try:
                if((regNum in regNameMapSPR.keys())
                    and (regNameMapSPR[regNum][1] == False)):
                    str1 = regNameMapSPR[regNum][0]
                    regNameMapSPR[regNum][1] = True
                else:
                    str1 = regNameMapGPR[regNum][0]
                    regNameMapGPR[regNum][1] = True
            except:
                str1 = hex(regNum)
            str3 = binascii.hexlify(fileHandle.read(4))
            print str(str1).ljust(15),str(str3).ljust(20)
            l_cnt = l_cnt + 6;

        print '********************************************************************'
        fileHandle.close()
    else:
        invokeOsCmd(getFilePath("p9_ppe_state_wrap.exe")+" -verbose -sbe -snapshot"+
                " -n" + str(node) + " -p" + str(proc))

def sbeLocalRegister():
    if(target == 'FILE'):
        print "File path: ", file_path
        fileHandle = open(file_path)
        l_cnt = 0
        print '********************************************************************'
        print 'Reg Number  Reg Value            Reg String'
        while(l_cnt < os.path.getsize(file_path)):
            str1 = binascii.hexlify(fileHandle.read(2))
            str2 = fileHandle.read(32)
            str3 = binascii.hexlify(fileHandle.read(8))
            print str(str1).ljust(11),str(str3).ljust(20),str2.ljust(40)
            l_cnt = l_cnt + 42;

        print '********************************************************************'
        fileHandle.close()
    else:
        invokeOsCmd(getFilePath("p9_sbe_localreg_dump_wrap.exe")+" -verbose -halt"+
                " -n" + str(node) + " -p" + str(proc))

def sbeState():
    if(target == 'FILE'):
        print "File path: ", file_path
        fileHandle = open(file_path)
        l_cnt = 0
        print '********************************************************************'
        print 'Reg Number  Reg Value            Reg String'
        while(l_cnt < os.path.getsize(file_path)):
            str1 = binascii.hexlify(fileHandle.read(4))
            str2 = fileHandle.read(32)
            str3 = binascii.hexlify(fileHandle.read(8))
            print str(str1).ljust(11),str(str3).ljust(20),str2.ljust(40)
            l_cnt = l_cnt + 44;

        print '********************************************************************'
        fileHandle.close()
    else:
        invokeOsCmd(getFilePath("p9_pibms_reg_dump_wrap.exe")+" -verbose" +\
                " -n" + str(node) + " -p" + str(proc))

def sbeStatus():
    if(target == 'FILE'):
        print "Error - not supported for FILE Target"
    else:
        cmd = ("getcfam pu 2809" +\
                " -n" + str(node) + " -p" + str(proc))
        print "cmd:", cmd
        output = os.popen(cmd).read()
        output = output.split()
        expected_out = 'k0:n%1d:s0:p%02d'%(node,proc)
        if (expected_out not in output):
            print "Error while getting the status register"
            print ' '.join(output)
        else:
            parsevalue(bin(int(output[output.index(expected_out)+1], 16)))

def parsevalue(iValue):
    sbeStates = {'0000' : 'SBE_STATE_UNKNOWN' , '0001' : 'SBE_STATE_IPLING' ,
                 '0010' : 'SBE_STATE_ISTEP', '0011' : 'SBE_STATE_MPIPL' ,
                 '0100' : 'SBE_STATE_RUNTIME' , '0101' : 'SBE_STATE_DMT' ,
                 '0110' : 'SBE_STATE_DUMP' , '0111' : 'SBE_STATE_FAILURE' ,
                 '1000' : 'SBE_STATE_QUIESCE' , '1001' : 'SBE_STATE_QUIESCE' ,
                 '1111' : 'SBE_INVALID_STATE'}

    tempVal = iValue[2:3]
    tempVal = 'True' if tempVal == '1' else 'False'
    print "SBE Booted           : %s" %(tempVal)

    tempVal = iValue[3:4]
    tempVal = 'True' if tempVal == '1' else 'False'
    print "Async FFDC           : %s" %(tempVal)

    tempVal = iValue[4:6]
    print "Reserver Bit [2:3]   : %s" %(tempVal)

    tempVal = iValue[6:10]
    print "SBE Previous State   : %s (%s)" %(sbeStates[tempVal], tempVal)


    tempVal = iValue[10:14]
    print "SBE Current State    : %s (%s)" %(sbeStates[tempVal], tempVal)

    tempVal = iValue[14:22]
    print "Istep Major          : %s" %(int(tempVal, 2))

    tempVal = iValue[22:28]
    print "Istep Minor          : %s" %(int(tempVal, 2))

    tempVal = iValue[28:34]
    print "Reserved Bit [26:31] : %s" %(tempVal)


'''
-----------------------------------------------------------------------------
                        Main Application
-----------------------------------------------------------------------------
'''
LEVELS_ARRAY = ('all', 'trace', 'forced-trace','attr','stack','ppestate','sbestate',
                'sbestatus','sbelocalregister', 'sym', 'sbecommit', 'ppestateffdc')

def usage():
    print(
    '''
usage: testarg.py [-h] [-l '''+str(LEVELS_ARRAY)+''']
[-t {AWAN,HW,FILE}] [-n NODE] [-p PROC] [-s SYMBOL] [-f FILE_PATH]

SBE Dump Parser

optional arguments:
  -h, --help            show this help message and exit
  -l, --level           Parser level
                        '''+str(LEVELS_ARRAY)+'''
  -t, --target          Target type
                        {AWAN,HW,FILE}
  -n, --node NODE       Node Number
  -p, --proc PROC       Proc Number
  -d, --ddlevel         DD Specific image identifier
  -s, --symbol          Get value of symbol from PIBMEM for level 'sym'
  -f, --file_path       Path of the file if Target is FILE
  -o, --output_path     Path to put the outfiles
  ''')

def main( argv ):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "l:t:n:p:d:s:f:o:h", ['level=', 'target=', 'node=', 'proc=', 'ddlevel=', 'symbol=', 'file_path=', 'output_path=','help'])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        exit(1)

    # Default values
    global target, node, proc, ddsuffix, file_path, output_path
    level = 'trace'
    symbol = ''

    # Parse the command line arguments
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            exit(1)
        elif opt in ('-l', '--level'):
            if arg in LEVELS_ARRAY:
                level = arg
            else:
                print "level should be one of " + str(LEVELS_ARRAY)
                exit(1)
        elif opt in ('-t', '--target'):
            if arg in ('AWAN', 'HW', 'FILE'):
                target = arg
            else:
                print "target should be one of {AWAN,HW,FILE}"
                exit(1)
        elif opt in ('-n', '--node'):
            try:
                node = int(arg)
            except:
                print "node should be an integer number"
                exit(1)
        elif opt in ('-p', '--proc'):
            try:
                proc = int(arg)
            except:
                print "proc should be an integer number"
                exit(1)
        elif opt in ('-d', '--ddlevel'):
            if arg in ('DD1', 'DD2', 'AXONE'):
                ddsuffix = arg
            else:
                print "target should be one of {DD1, DD2, AXONE}"
                exit(1)
        elif opt in ('-f', '--file_path'):
            try:
                file_path = arg
                assert os.path.exists(arg), "Did not find the file at,"+str(arg)
            except:
                print "file_path should a string path"
                exit(1)
        elif opt in ('-o', '--output_path'):
            try:
                output_path = arg+"/"
                assert os.path.exists(arg)
            except:
                print "output_path should a string path"
                exit(1)
        elif opt in ('-s', '--symbol'):
            symbol = arg

    if(target != 'FILE'):
        # On cronus, save the existing FIFO mode
        cmdFifoLastState = subprocess.Popen(['getconfig','USE_SBE_FIFO'], stdout=subprocess.PIPE).communicate()[0]
        # Example output: 'CNFG FILE GLOBAL_DEBUG: 8.VI.B\n\tk0                 \nUSE_SBE_FIFO = off\n/afs/awd.austin.ibm.com/projects/eng/tools/cronus/p9/exe/dev/p9sim_dev_x86_64.exe getconfig USE_SBE_FIFO \n'
        regEx = re.search('USE_SBE_FIFO.*=(.*)', cmdFifoLastState)
        if (regEx == None):
           print "ERROR in getting USE_SBE_FIFO config"
           return 1
        cmdFifoLastState = regEx.group(1).split()[0]
        # On cronus, disabe FIFO mode
        invokeOsCmd("setconfig USE_SBE_FIFO off")

    if(target == 'FILE'):
        # check if the file path exist or not
        assert os.path.exists(file_path), "Did not find the file at, "+str(arg)

    fillSymTable()

    if ( level == 'all' ):
        print "Parsing everything"
        collectTrace()
        collectAttr()
        sbeStatus()
        ppeState()
        sbeState()
        sbeLocalRegister()
    elif ( level == 'trace' ):
        collectTrace()
    elif ( level == 'forced-trace' ):
        forcedCollectTrace()
    elif ( level == 'attr' ):
        collectAttr()
    elif ( level == 'ppestate' ):
        ppeState()
    elif ( level == 'sbestate' ):
        sbeState()
    elif ( level == 'sbestatus' ):
        sbeStatus()
    elif ( level == 'sbelocalregister' ):
        sbeLocalRegister()
    elif ( level == 'stack' ):
        collectStackUsage()
    elif ( level == 'sym' ):
        getSymbolVal(symbol, True)
    elif ( level == 'sbecommit' ):
        getSbeCommit()
    elif ( level == 'ppestateffdc' ):
        ppeStateFfdc()

    if(target != 'FILE'):
        # On cronus, set the FIFO mode to previous state
        invokeOsCmd("setconfig USE_SBE_FIFO " + cmdFifoLastState)

if __name__ == "__main__":
    main( sys.argv )
