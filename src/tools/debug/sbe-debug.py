#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/sbe-debug.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2021
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
import subprocess
import re
import random
import getopt
import sys
import binascii
import struct
import gzip
import shutil

from ctypes import *
import ctypes

err = False

pibmemBaseAddr = 0xfff80000
syms = {};
SBE_TOOLS_PATH = os.getcwd()
if 'SBE_TOOLS_PATH' in os.environ:
    SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];

# preference is SBE_TOOLS_PATH, current woring directory and then system path
PATH = SBE_TOOLS_PATH+os.pathsep+os.getcwd()+os.pathsep+os.environ['PATH']

target = 'HW'
image_type = 'SBE'
node = 0
proc = 0
ddsuffix = 'DD1'
file_path = ""
sbe_string_file = ""
sbe_tracMERG_file = ""
sbe_syms_file = ""
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
    print("ERROR: file " + filename +" not found")
    return 1

def getTraceFilePath():
    fspTrace = getFilePath("fsp-trace")
    print("\n fsp-trace: [" + fspTrace + "]")
    if(fspTrace == None):
        fspTrace = "fsp-trace"
    return fspTrace

def fillSymTable(file_name):
    try:
        symFile = getFilePath(file_name)
        print("\n Symbol File: [" + symFile + "]")
        f = open( symFile, 'r' )
        for line in f:
            words = line.split()
            if( len( words ) == 4 ):
                syms[words[3]] = [words[0], words[1]]
    except:
        print("Symbol file not found, limited commands avaliable")

def getSymbolInfo( symbol ):
    symAddr = ''
    length = 0
    for key, val in syms.items():
        if(re.search(symbol, key)!= None):
            symAddr = val[0]
            length = val[1]
    print("\n Trace buffer symbol addr: [" + symAddr + "] Trace Buffer Length: [" + length + "]")
    offset = int(symAddr, base = 16) - pibmemBaseAddr;
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
    invokeOsCmd( getFilePath("p10_pibmem_dump_wrap.exe")+
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
        print("symbol not found")

def invokeOsCmd(cmd):
    print("Running command [" + cmd + "]")
    rc = os.system( cmd )
    if ( rc ):
       print("ERROR running command: %d " % ( rc ))
       return 1

def updateHwTraceFiles(sbe_tracMERG_file,sbe_string_file,sbe_syms_file):
    if(target == 'HW'):
        cmd = ("getcfam pu 2809" +\
               " -n" + str(node) + " -p" + str(proc))
        print("cmd:", cmd)
        output = os.popen(cmd).read()
        output = output.split()
        expected_out = 'k0:n%1d:s0:p%02d'%(node,proc)
        if (expected_out not in output):
            print("Error while getting the status register")
            print(' '.join(output))
        else:
            # Read opcode in SB_MSG Register
            # SBE_CODE_MEASURMENT_PIBMEM_START_MSG 0x05
            tempVal = bin(int(output[output.index(expected_out)+1], 16))
            print("opcode in SB_MSG Register[28:31] : %s" %(tempVal))
            if( 0x05 == tempVal[30:34] ):
                sbe_tracMERG_file = "sbe_measurement_seeprom_" + str(proc) + "_tracMERG"
                sbe_string_file = stringFile = "sbeMeasurementStringFile"
                sbe_syms_file = "sbe_measurement_seeprom.syms"


'''
--------------------------------------------------------------------------------
                Command functions
--------------------------------------------------------------------------------
'''
def collectTrace(string_file,tracMERG_file):
    getSymbolVal('g_pk_trace_buf' )
    stringFile = getFilePath(string_file)
    print("\n String File: [" + stringFile + "]")
    invokeOsCmd( getFilePath("ppe2fsp")+ " " + output_path + "DumpPIBMEM "+\
            output_path +"sbetrace.bin " )
    invokeOsCmd( getTraceFilePath() + " -s " + stringFile + " "+ output_path +"sbetrace.bin > "+\
            output_path + tracMERG_file )
    invokeOsCmd( "mv " + output_path +"DumpPIBMEM "+\
            output_path +"dumpPibMem_trace" )

# Can be used if proper sbe symbol files are not available
def forcedCollectTrace(string_file,tracMERG_file):
    stringFile = getFilePath(string_file)
    print("\n String File: [" + stringFile + "]")
    # Collect entire PIBMEM
    offset = "0x00" # PIBMEM BASE
    length = "0x7D400"
    print("\n Forced Trace, Pibmem Offset: [" + offset + "] Pibmem Length: [" + length + "]")
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
    #print(trace_pattern)
    with open(output_path+"DumpPIBMEM", "r") as f:
        byte = 'x'
        while byte != "":
            byte = f.read(1)
            data_read = data_read[1:]+[ord(byte)]
            #print([chr(a) for a in data_read])
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

    invokeOsCmd( getTraceFilePath() + " -s " + stringFile +" "+ output_path +"sbetrace.bin > "+\
            output_path + tracMERG_file )
    invokeOsCmd( "mv "+ output_path +"DumpPIBMEM "+\
            output_path +"dumpPibMem_trace" )

def collectStdAttr():
    sbeImgFile = "sbe_seeprom_"+ddsuffix+".bin"
    getSymbolVal( 'G_sbe_attrs' )
    invokeOsCmd( getFilePath("ipl_image_tool")+" "+getFilePath(sbeImgFile) +\
                 " extract .fixed "+ output_path +"sbeStdAttr.bin ")
    invokeOsCmd( getFilePath("ipl_image_tool")+" "+getFilePath(sbeImgFile) +\
                 " attrdump "+ output_path +"sbeStdAttr.bin > " +\
                 output_path+"sbe_"+str(node)+"_"+str(proc)+"_attrs")

def collectAttr():
    sbeImgFile = "sbe_seeprom_"+ddsuffix+".bin"
    getSymbolVal( 'G_sbe_attrs' )
    invokeOsCmd( "mv "+ output_path + "DumpPIBMEM " +\
            output_path +"sbeAttr.bin" )
    invokeOsCmd( getFilePath("ipl_image_tool")+" "+getFilePath(sbeImgFile) + " -ifs attrdump "+ output_path +"sbeAttr.bin > "+\
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

    print("==================================Stack usage===================================")
    print("Thread".ljust(40)+"Least Available[bytes]".ljust(30)+"Max usage[%]")
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
            print(str("["+thread+"]").ljust(40) + str(leastAvailable).ljust(30) + str("%.2f" % (100 * (1 - (leastAvailable/float(int("0x"+syms[thread][1], 16)))))))

def getSbeCommit():
    getSymbolVal( 'SBEGlobalsSingleton.*fwCommitId' )

    with open(output_path+"DumpPIBMEM", "rb") as f:
        # Big Endian word
        word = struct.unpack('>I', f.read(4))[0]
        print("SBE commit:", hex(word))

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

        print('''
---------------------------------------------------------------------------------
Please verify sbe_DD1.syms is valid for this dump, before depending on the values
---------------------------------------------------------------------------------''')
        if ppestateffdc.validbyte == 0x1 and ppestateffdc.magicbyte == 0xA5:
            print("ppestateffdc.version                             0x%04X" % ppestateffdc.version)
            print("ppestateffdc.magicbyte                           0x%02X" % ppestateffdc.magicbyte)
            print("ppestateffdc.validbyte                           0x%02X" % ppestateffdc.validbyte)
            print("ppestateffdc.register_SRR0                       0x%08X" % ppestateffdc.register_SRR0)
            print("ppestateffdc.register_SRR1                       0x%08X" % ppestateffdc.register_SRR0)
            print("ppestateffdc.register_ISR                        0x%08X" % ppestateffdc.register_ISR)
            print("ppestateffdc.register_FI2C_CONFIG_LOWER_32BITS   0x%08X" % ppestateffdc.register_FI2C_CONFIG_LOWER_32BITS)
            print("ppestateffdc.register_FI2C_CONFIG_UPPER_32BITS   0x%08X" % ppestateffdc.register_FI2C_CONFIG_UPPER_32BITS)
            print("ppestateffdc.register_FI2C_STAT_LOWER_32BITS     0x%08X" % ppestateffdc.register_FI2C_STAT_LOWER_32BITS)
            print("ppestateffdc.register_FI2C_STAT_UPPER_32BITS     0x%08X" % ppestateffdc.register_FI2C_STAT_UPPER_32BITS)
            print("ppestateffdc.register_LR                         0x%08X" % ppestateffdc.register_LR)
        else:
            print("Register ffdc is not valid, probably SBE has not hit any internal halt condition")
        print('''---------------------------------------------------------------------------------''')

def ppeState():
    if(target == 'FILE'):
        regNameMapSPR = {
                         # SPRs and XIR
                         9 : ["CTR", False],
                         316 : ["DACR", False],
                         308 : ["DBCR", False],
                         22 : ["DEC", False],
                         61 : ["EDR", False],
                         3  : ["IR", False],
                         62 : ["ISR", False],
                         63 : ["IVPR", False],
                         8 : ["LR", False],
                         286 : ["PIR", False],
                         287 : ["PVR", False],
                         272 : ["SPRG0", False],
                         26 : ["SRR0", False],
                         27 : ["SRR1", False],
                         340 : ["TCR", False],
                         336 : ["TSR", False],
                         1 : ["XER", False],
                         42 : ["MSR", False],
                         420 : ["CR", False],
                         1003 : ["XSR", False],
                         1000 : ["IAR", False],
                         1001 : ["IR", False],
                         1002 : ["XCR", False],
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
        print("File path: ", file_path)
        fileHandle = open(file_path)
        l_cnt = 0
        print('********************************************************************')
        print('Reg'.ljust(15),'Reg Value'.ljust(20))
        print('--------------------------------------------------------------------')
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
            print(str(str1).ljust(15),str(str3).ljust(20))
            str4 = fileHandle.read(4)
            #2Bytes(SPR/GPR number) + 4Bytes(Value) + 4Bytes(Name)
            l_cnt = l_cnt + 10; #

        print('********************************************************************')
        fileHandle.close()
    else:
        invokeOsCmd(getFilePath("p10_ppe_state_wrap.exe")+" -verbose -sbe -snapshot"+
                " -n" + str(node) + " -p" + str(proc))

def sbeLocalRegister():
    if(target == 'FILE'):
        print("File path: ", file_path)
        fileHandle = open(file_path)
        l_cnt = 0
        print('********************************************************************')
        print('Reg Number  Reg Value            Reg String')
        while(l_cnt < os.path.getsize(file_path)):
            str1 = binascii.hexlify(fileHandle.read(2))
            str2 = fileHandle.read(32)
            str3 = binascii.hexlify(fileHandle.read(8))
            print(str(str1).ljust(11),str(str3).ljust(20),str2.ljust(40))
            l_cnt = l_cnt + 42;

        print('********************************************************************')
        fileHandle.close()
    else:
        invokeOsCmd(getFilePath("p10_sbe_localreg_dump_wrap.exe")+" -verbose -halt"+
                " -n" + str(node) + " -p" + str(proc))

def bootSeepromDump():
    invokeOsCmd(getFilePath("p10_sbe_seepromDump_wrap.exe")+" -seeprom0 -ecc"+
                " -n" + str(node) + " -p" + str(proc) + " -TARGET p10 -length 71C70 -scom -dump_file bseeprom.txt")
    print("Primary Boot SEEPROM content is dumped to bseeprom.txt")

def measSeepromDump():
    invokeOsCmd(getFilePath("p10_sbe_seepromDump_wrap.exe")+" -seeprom3 -ecc"+
                " -n" + str(node) + " -p" + str(proc) + " -TARGET p10 -length 2AAA8 -scom -dump_file mseeprom.txt")
    print("Primary Mesurement SEEPROM content is dumped to mseeprom.txt")

def secBootSeepromDump():
    invokeOsCmd(getFilePath("p10_sbe_seepromDump_wrap.exe")+" -seeprom1 -ecc"+
                " -n" + str(node) + " -p" + str(proc) + " -TARGET p10 -length 71C70 -scom -dump_file secbseeprom.txt")
    print("Secondary Boot SEEPROM content is dumped to secbseeprom.txt")

def secMeasSeepromDump():
    invokeOsCmd(getFilePath("p10_sbe_seepromDump_wrap.exe")+" -seeprom2 -ecc"+
                " -n" + str(node) + " -p" + str(proc) + " -TARGET p10 -length 2AAA8 -scom -dump_file secmseeprom.txt")
    print("Secondary Measurement SEEPROM content is dumped to secmseeprom.txt")

def sbeState():
    if(target == 'FILE'):
        print("File path: ", file_path)
        fileHandle = open(file_path)
        l_cnt = 0
        print('********************************************************************')
        print('Reg Number  Reg Value            Reg String')
        while(l_cnt < os.path.getsize(file_path)):
            str1 = binascii.hexlify(fileHandle.read(8))
            str2 = fileHandle.read(32)
            str3 = binascii.hexlify(fileHandle.read(4))
            str4 = binascii.hexlify(fileHandle.read(8))
            print(str(str1).ljust(11),str(str3).ljust(20),str(str3).ljust(20),str2.ljust(40))
            l_cnt = l_cnt + 44;

        print('********************************************************************')
        fileHandle.close()
    else:
        invokeOsCmd(getFilePath("p10_pibms_reg_dump_wrap.exe")+" -verbose" +\
                " -n" + str(node) + " -p" + str(proc) + " -TARGET p10")

def sbeStatus():
    if(target == 'FILE'):
        print("Error - not supported for FILE Target")
    else:
        cmd = ("getcfam pu 2809" +\
                " -n" + str(node) + " -p" + str(proc))
        print("cmd:", cmd)
        output = os.popen(cmd).read()
        output = output.split()
        expected_out = 'k0:n%1d:s0:p%02d'%(node,proc)
        if (expected_out not in output):
            print("Error while getting the status register")
            print(' '.join(output))
        else:
            parsevalue(bin(int(output[output.index(expected_out)+1], 16)))

def parsevalue(iValue):
    sbeStates = {'0000' : 'SBE_STATE_UNKNOWN' , '0001' : 'SBE_STATE_IPLING' ,
                 '0010' : 'SBE_STATE_ISTEP', '0011' : 'SBE_STATE_MPIPL' ,
                 '0100' : 'SBE_STATE_RUNTIME' , '0101' : 'SBE_STATE_DMT' ,
                 '0110' : 'SBE_STATE_DUMP' , '0111' : 'SBE_STATE_FAILURE' ,
                 '1000' : 'SBE_STATE_QUIESCE' , '1001' : 'SBE_STATE_QUIESCE' ,
                 '1111' : 'SBE_INVALID_STATE'}

    progressCode = {'0001' : 'OTPROM' ,
                    '0010' : 'MEASUREMENT SEEPROM loader',
                    '0011' : 'MEASUREMENT SEEPROM Pibmem Repair' ,
                    '0100' : 'MEASUREMENT SEEPROM Pib Init',
                    '0101' : 'MEASUREMENT TPM RESET',
                    '0110' : 'MEASUREMENT PIBMEM',
                    '0111' : 'MEASUREMENT PIBMEM SHA COMPUTE',
                    '1000' : 'MEASUREMENT PIBMEM TPM INIT',
                    '1001' : 'BOOT SEEPROM loader L1',
                    '1010' : 'BOOT PIBMEM Loader L2',
                    '1011' : 'BOOT PIBMEM Main Flow'}

    tempVal = iValue[2:3]
    tempVal = 'True' if tempVal == '1' else 'False'
    print("SBE Booted           : %s" %(tempVal))

    tempVal = iValue[3:4]
    tempVal = 'True' if tempVal == '1' else 'False'
    print("Async FFDC           : %s" %(tempVal))

    tempVal = iValue[4:6]
    print("Reserver Bit [2:3]   : %s" %(tempVal))

    tempVal = iValue[6:10]
    print("SBE Previous State   : %s (%s)" %(sbeStates[tempVal], tempVal))

    tempVal = iValue[10:14]
    print("SBE Current State    : %s (%s)" %(sbeStates[tempVal], tempVal))

    tempVal = iValue[14:22]
    print("Istep Major          : %s" %(int(tempVal, 2)))

    tempVal = iValue[22:28]
    print("Istep Minor          : %s" %(int(tempVal, 2)))

    tempVal = iValue[28:30]
    print("Reserved Bit [26:27] : %s" %(tempVal))

    tempVal = iValue[30:34]
    print("SBE Progress Code [28:31] : Code reached to %s (%s)" %(progressCode[tempVal],tempVal))


def extractSbeDump(dumpFile):
    print("Extract dump File")

    # If the file is compressed, unzip it and use the unzipped file elsewhere
    if (dumpFile.endswith(".gz")):
        print("gunzip on file: %s" % dumpFile)
        fileTmp=gzip.open(dumpFile, 'rb')
        fileObj=open(dumpFile[:-3], 'wb+')
        #Copy the unzipped file content into a new file
        shutil.copyfileobj(fileTmp, fileObj)
        fileTmp.close()
        #Use the unzipped file else where
        dumpFile=dumpFile[:-3]
    else:
        #Open the file in binary mode.
        fileObj=open(dumpFile, 'rb')

    print("Parsing the Dump header")
    dumpSummaryOffset = 0xD0
    missingSecIndicatorOffset = 0x83

    # If the missing section indicator is on, then adjust the dump summary location
    fileObj.seek(missingSecIndicatorOffset, 0)
    oneByteChunk = fileObj.read(1)
    missingSecIndicator = struct.unpack('b', oneByteChunk)[0]
    print("Missing section indicator is %s" %hex(missingSecIndicator))
    if (hex(missingSecIndicator) == 0x01):
        print("Adjusting the dump summary location")
        dumpSummaryOffset -= 0x30

    #Read the dump content type
    dumpContentOffset = dumpSummaryOffset + 0x260
    fileObj.seek(dumpContentOffset, 0)
    fourByteChunk = fileObj.read(4)
    dumpContentType = struct.unpack('>i', fourByteChunk)[0]
    print("Dump Content Type is %s" %hex(dumpContentType))
    if(dumpContentType == 0x40000 or dumpContentType == 0x80000000):
        print("Extract MPIPL Dump")
        fileObj.close()
        cmd = os.environ['PATH_PYTHON3'] + " " + os.path.expandvars("$SBEROOT") + "/src/tools/debug/extractMpiplDump.py" + " -d " + dumpFile
        print(cmd)
        os.system(cmd)
        sys.exit(0)

    hwDataLenOffest = 0x40 + 0x30 + 0x18 #FileHeaderLength + SumarrySectionLen + HWSectionLenOffset
    hwDataOffest = 0x4D0

    #Read the sysdatalength.
    fileObj.seek(hwDataLenOffest, 0)
    fourByteChunk = fileObj.read(4)
    sysDataSize = struct.unpack('>i', fourByteChunk)[0]
    print("Sys data size is %s" %hex(sysDataSize));

    #Read the hwdatalength.
    fileObj.seek(hwDataLenOffest + 4, 0)
    fourByteChunk = fileObj.read(4)
    hwDataSize = struct.unpack('>i', fourByteChunk)[0]
    print("HW data size is %s" %hex(hwDataSize));

    length = hwDataSize - sysDataSize
    print("Length of tar is %s" %hex(length));

    cmd = "dd skip=" + str(hwDataOffest) + " count=" + str(length) + " if=" + dumpFile  + " of=output.binary bs=1"
    print("Command is %s" %cmd)
    os.system(cmd)

    #Untar the HW data.
    cmd = "tar -xvf output.binary"
    print("Command is %s" %cmd)
    os.system(cmd)

    #Close the fileObj.
    fileObj.close();

'''
-----------------------------------------------------------------------------
                        Main Application
-----------------------------------------------------------------------------
'''
LEVELS_ARRAY = ('all', 'trace', 'forced-trace','attr','stack','ppestate','sbestate',
                'sbestatus','sbelocalregister', 'sym', 'sbecommit', 'ppestateffdc', 'bseepromdump', 'mseepromdump', 'secbseepromdump', 'secmseepromdump')

def usage():
    print(
    '''
usage: testarg.py [-h] [-l '''+str(LEVELS_ARRAY)+''']
[-t {AWAN,HW,FILE}] [-i {SBE,MSBE,VSBE}] [-n NODE] [-p PROC] [-s SYMBOL] [-f FILE_PATH]

SBE Dump Parser

optional arguments:
  -h, --help            show this help message and exit
  -l, --level           Parser level
                        '''+str(LEVELS_ARRAY)+'''
  -t, --target          Target type
                        {AWAN,HW,FILE}
  -i, --image_type      Seeprom Image type
                        {SBE,MSBE,VSBE}
  -n, --node NODE       Node Number
  -p, --proc PROC       Proc Number
  -d, --ddlevel         DD Specific image identifier
  -s, --symbol          Get value of symbol from PIBMEM for level 'sym'
  -f, --file_path       Path of the file if Target is FILE
  -o, --output_path     Path to put the outfiles
  -e, --extract         Extract Sbe Dump.
  ''')

def main( argv ):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "l:t:i:n:p:d:s:f:o:e:h", ['level=', 'target=', 'image_type=', 'node=', 'proc=', 'ddlevel=', 'symbol=', 'file_path=', 'output_path=', 'extract=', 'help'])
    except getopt.GetoptError as err:
        print(str(err))
        usage()
        exit(1)

    # Default values
    global target, image_type, node, proc, ddsuffix, file_path, output_path
    level = 'trace'
    symbol = ''
    dumpFile = ''
    # Parse the command line arguments
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            exit(1)
        elif opt in ('-l', '--level'):
            if arg in LEVELS_ARRAY:
                level = arg
            else:
                print("level should be one of " + str(LEVELS_ARRAY))
                exit(1)
        elif opt in ('-t', '--target'):
            if arg in ('AWAN', 'HW', 'FILE'):
                target = arg
            else:
                print("target should be one of {AWAN,HW,FILE}")
                exit(1)
        elif opt in ('-i', '--image_type'):
            if arg in ('SBE', 'MSBE', 'VSBE'):
                image_type = arg
            else:
                print("Image should be one of {SBE,MSBE,VSBE}")
                exit(1)
        elif opt in ('-n', '--node'):
            try:
                node = int(arg)
            except:
                print("node should be an integer number")
                exit(1)
        elif opt in ('-p', '--proc'):
            try:
                proc = int(arg)
            except:
                print("proc should be an integer number")
                exit(1)
        elif opt in ('-d', '--ddlevel'):
            if arg in ('DD1', 'AXONE'):
                ddsuffix = arg
            else:
                print("target should be DD1")
                exit(1)
        elif opt in ('-f', '--file_path'):
            try:
                file_path = arg
                assert os.path.exists(arg), "Did not find the file at,"+str(arg)
            except:
                print("file_path should a string path")
                exit(1)
        elif opt in ('-o', '--output_path'):
            try:
                output_path = arg+"/"
                assert os.path.exists(arg)
            except:
                print("output_path should a string path")
                exit(1)
        elif opt in ('-e', '--extract'):
            try:
                dumpFile = arg
                assert os.path.exists(arg), "Did not find the dump file at,"+str(arg)
            except:
                print("dump file path should be a string path")
                exit(1)
        elif opt in ('-s', '--symbol'):
            symbol = arg

    if (dumpFile != '' ):
        extractSbeDump(dumpFile)
        exit()

    if(target != 'FILE'):
        # On cronus, save the existing FIFO mode
        cmdFifoLastState = subprocess.Popen(['getconfig','USE_SBE_FIFO'], stdout=subprocess.PIPE).communicate()[0]
        # Example output: 'CNFG FILE GLOBAL_DEBUG: 8.VI.B\n\tk0                 \nUSE_SBE_FIFO = off\n/afs/awd.austin.ibm.com/projects/eng/tools/cronus/p9/exe/dev/p9sim_dev_x86_64.exe getconfig USE_SBE_FIFO \n'
        regEx = re.search('USE_SBE_FIFO.*=(.*)', cmdFifoLastState)
        if (regEx == None):
           print("ERROR in getting USE_SBE_FIFO config")
           return 1
        cmdFifoLastState = regEx.group(1).split()[0]
        # On cronus, disabe FIFO mode
        invokeOsCmd("setconfig USE_SBE_FIFO off")

    if(target == 'FILE'):
        # check if the file path exist or not
        assert os.path.exists(file_path), "Did not find the file at, "+str(arg)

    if ( image_type == 'MSBE' ):
        sbe_tracMERG_file = "sbe_measurement_seeprom_" + str(proc) + "_tracMERG"
        sbe_string_file = stringFile = "sbeMeasurementStringFile"
        sbe_syms_file = "sbe_measurement_seeprom.syms"
    elif ( image_type == 'VSBE' ):
        sbe_tracMERG_file = "sbe_verification_" + str(proc) + "_tracMERG"
        sbe_string_file = stringFile = "sbeVerificationStringFile"
        sbe_syms_file = "sbe_verification.syms"
    else:
        sbe_tracMERG_file = "sbe_"+str(node)+"_"+str(proc)+"_tracMERG"
        sbe_string_file = "sbeStringFile_"+ddsuffix
        sbe_syms_file = "sbe_"+ddsuffix+".syms"

    #updateHwTraceFiles(sbe_tracMERG_file,sbe_string_file,sbe_syms_file)
    #print("\n ****symFile :", sbe_syms_file)
    fillSymTable(sbe_syms_file)

    if ( level == 'all' ):
        print("Parsing everything")
        collectTrace(sbe_string_file,sbe_tracMERG_file)
        collectAttr()
        sbeStatus()
        ppeState()
        sbeState()
        sbeLocalRegister()
        bootSeepromDump()
        measSeepromDump()
        secBootSeepromDump()
        secMeasSeepromDump()
    elif ( level == 'trace' ):
        collectTrace(sbe_string_file,sbe_tracMERG_file)
    elif ( level == 'forced-trace' ):
        forcedCollectTrace(sbe_string_file,sbe_tracMERG_file)
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
    elif ( level == 'bseepromdump' ):
        bootSeepromDump()
    elif ( level == 'mseepromdump' ):
        measSeepromDump()
    elif ( level == 'secbseepromdump' ):
        secBootSeepromDump()
    elif ( level == 'secmseepromdump' ):
        secMeasSeepromDump()

    if(target != 'FILE'):
        # On cronus, set the FIFO mode to previous state
        invokeOsCmd("setconfig USE_SBE_FIFO " + cmdFifoLastState)

if __name__ == "__main__":
    main( sys.argv )
