#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/sbe-debug.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2017
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
err = False

baseAddr = 0xfffe8000
syms = {};
SBE_TOOLS_PATH = os.getcwd()
if 'SBE_TOOLS_PATH' in os.environ:
    SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];

# preference is SBE_TOOLS_PATH, current woring directory and then system path
PATH = SBE_TOOLS_PATH+os.pathsep+os.getcwd()+os.pathsep+os.environ['PATH']

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

def fillSymTable(target, ddsuffix ):
    if (target == 'AWAN'):
        symFile = getFilePath("sim.sbe.syms")
    else:
        symFile = getFilePath("sbe_"+ddsuffix+".syms")
    f = open( symFile, 'r' )
    for line in f:
        words = line.split()
        if( len( words ) == 4 ):
            syms[words[3]] = [words[0], words[1]]

def getOffset( symbol ):
    symAddr = syms[ symbol][0]
    print "\n symAddress :", symAddr
    offset = int(symAddr, base = 16) - baseAddr;
    return hex(offset)

def createPibmemDumpFile( file_path, offset, length ):
    fileHandle = open(file_path)
    fileHandle.seek(int(offset, 16))
    fileData = fileHandle.read(int(length, 16))
    tempFileHandle = open("DumpPIBMEM", 'w')
    tempFileHandle.write(fileData)
    tempFileHandle.close()
    fileHandle.close()

def getSymbolVal(target, node, proc, symbol, file_path):
    if(symbol in syms.keys()):
        offset = getOffset(symbol)
        length = '0x'+syms[symbol][1]
        if(target == 'FILE'):
            createPibmemDumpFile(file_path, offset, length)
        else:
            cmd1 = (getFilePath("p9_pibmem_dump_wrap.exe")+
                            " -quiet -start_byte "+str(offset)+
                            " -num_of_byte " + length +
                            " -n" + str(node) + " -p" + str(proc))
            rc = os.system(cmd1)
            if ( rc ):
                print "ERROR running %s: %d " % ( cmd1, rc )
                return 1
        cmd2 = ("hexdump -C DumpPIBMEM")
        rc = os.system(cmd2)
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd2, rc )
            return 1
    else:
        print "symbol not found"

def collectTrace( target, node, proc, ddsuffix, file_path ):
    offset = getOffset( 'g_pk_trace_buf' );
    length = "0x" + syms['g_pk_trace_buf'][1];
    print "\ncollecting trace with commands -\n"
    if(target == 'FILE'):
        createPibmemDumpFile(file_path, offset, length);
    else:
        cmd1 = (getFilePath("p9_pibmem_dump_wrap.exe")+" -quiet -start_byte " +
                str(offset) +\
                 " -num_of_byte " + length + " "
                 " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1
    cmd2 = getFilePath("ppe2fsp")+" DumpPIBMEM sbetrace.bin "
    cmd3 = (getTraceFilePath() + " -s " +
            getFilePath("sbeStringFile_"+ddsuffix)+" sbetrace.bin > "+
                         "sbe_"+str(proc)+"_tracMERG")
    cmd4 = "mv DumpPIBMEM dumpPibMem_trace"
    print "cmd2:", cmd2
    rc = os.system( cmd2 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd2, rc )
       return 1

    print "cmd3:", cmd3
    rc = os.system( cmd3 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd3, rc )
       return 1

    print "cmd4:", cmd4
    rc = os.system( cmd4 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd4, rc )
       return 1

# Can be used if proper sbe symbol files are not available
def forcedCollectTrace( target, node, proc, ddsuffix, file_path ):
    # Collect entire PIBMEM
    offset = "0x00" # PIBMEM BASE
    length = "0x16400"
    print "\ncollecting trace with commands -\n"
    if(target == 'FILE'):
        createPibmemDumpFile(file_path, offset, length);
    else:
        cmd1 = (getFilePath("p9_pibmem_dump_wrap.exe")+" -quiet -start_byte " +
                str(offset) +\
                 " -num_of_byte " + length + " "
                 " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

    # find offset of trace buffer in PIBMEM dump
    # version = 0x00 0x02
    # rsvd = 0x00 0x00
    # image_str = "sbe_seeprom_DD1\0"
    image_str = "sbe_seeprom_" + ddsuffix
    trace_pattern = [0, 2, 0, 0] + [ord(a) for a in image_str] + [0]
    data_read     = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    trace_index = 0
    #print trace_pattern
    with open("DumpPIBMEM", "r") as f:
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
    createPibmemDumpFile("DumpPIBMEM", offset, length);
    cmd2 = getFilePath("ppe2fsp")+" DumpPIBMEM sbetrace.bin "

    cmd3 = (getTraceFilePath() + " -s " +
            getFilePath("sbeStringFile_"+ddsuffix)+" sbetrace.bin > "+\
                         "sbe_"+str(proc)+"_tracMERG")
    cmd4 = "mv DumpPIBMEM dumpPibMem_trace"
    print "cmd2:", cmd2
    rc = os.system( cmd2 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd2, rc )
       return 1

    print "cmd3:", cmd3
    rc = os.system( cmd3 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd3, rc )
       return 1

    print "cmd4:", cmd4
    rc = os.system( cmd4 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd4, rc )
       return 1

def collectAttr( target, node, proc, ddsuffix, file_path ):
    if (target == 'AWAN'):
        sbeImgFile = "p9n_10.sim.sbe_seeprom.bin"
    else:
        sbeImgFile = "sbe_seeprom_"+ddsuffix+".bin"
    offset = getOffset( 'G_sbe_attrs' );
    length = "0x" + syms['G_sbe_attrs'][1];
    print "\ncollecting attributes with commands -\n"
    if(target == 'FILE'):
        createPibmemDumpFile(file_path, offset, length);
    else:
        cmd1 = (getFilePath("p9_pibmem_dump_wrap.exe")+" -quiet -start_byte "+
                str(offset) +\
                " -num_of_byte " + length + " "
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

    cmd2 = "mv DumpPIBMEM sbeAttr.bin"
    # TODO via RTC 158861
    # For multi-node system we need to convert node/proc to absolute
    # proc number.
    cmd3 = ( getFilePath("p9_xip_tool")+" "+getFilePath(sbeImgFile) + " -ifs attrdump sbeAttr.bin > "+\
             "sbe_"+str(proc)+"_attrs")

    print "cmd2:", cmd2
    rc = os.system( cmd2 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd2, rc )
       return 1

    print "cmd3:", cmd3
    rc = os.system( cmd3 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd3, rc )
       return 1

def collectStackUsage (node, proc ):
    threads = ('sbeSyncCommandProcessor_stack',
               'sbeCommandReceiver_stack',
               'sbe_Kernel_NCInt_stack',
               'sbeAsyncCommandProcessor_stack')
    for thread in threads:
        offset = getOffset( thread );
        length = "0x" + syms[thread][1];
        cmd1 = (getFilePath("p9_pibmem_dump_wrap.exe")+" -quiet -start_byte " +
                str(offset) +\
                " -num_of_byte " + length + " "
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

        # Dump stack memory to binary file
        cmd2 = "cat DumpPIBMEM >>"+thread
        print "cmd2:", cmd2
        rc = os.system( cmd2 )
        if (rc):
            print "ERROR running %s: %d " % ( cmd2, rc )
            return 1

    print "==================================Stack usage==================================="
    print "Thread".ljust(40)+"Least Available[bytes]".ljust(30)+"Max usage[%]"
    for thread in threads:
        with open(thread, "rb") as f:
            word = struct.unpack('I', f.read(4))[0]
            leastAvailable = 0
            while (1):
                if (word == int("0xEFCDAB03", 16)):
                    leastAvailable += 4
                    word = struct.unpack('I', f.read(4))[0]
                else:
                    break
            print str("["+thread+"]").ljust(40) + str(leastAvailable).ljust(30) + str("%.2f" % (100 * (1 - (leastAvailable/float(int("0x"+syms[thread][1], 16))))))


def ppeState( target, node, proc, file_path ):
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
        cmd1 = (getFilePath("p9_ppe_state_wrap.exe")+" -verbose -sbe -snapshot"+
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

def sbeLocalRegister( target, node, proc, file_path ):
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
        cmd1 = (getFilePath("p9_sbe_localreg_dump_wrap.exe")+" -quiet -sbe -snapshot"+
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

def sbeState( target, node, proc, file_path ):
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
        cmd1 = (getFilePath("p9_pibms_reg_dump_wrap.exe")+" -verbose" +\
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

def sbeStatus( target, node, proc ):
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

def usage():
    print "usage: testarg.py [-h] [-l {trace,attr,ppestate,sbestate,sbestatus,sbelocalregister, sym}]\n\
\t\t\t\t[-t {AWAN,HW,FILE}] [-n NODE] [-p PROC] [-s SYMBOL] [-f FILE_PATH]\n\
\n\
SBE Dump Parser\n\
\n\
optional arguments:\n\
  -h, --help            show this help message and exitn\n\
  -l {trace,attr,stack,ppestate,sbestate,sbestatus,sbelocalregister,sym}, --level {trace,attr,stack,ppestate,sbestate,sbestatus,sbelocalregisteri,sym}\n\
                        Parser level\n\
  -t {AWAN,HW,FILE}, --target {AWAN,HW,FILE}\n\
                        Target type\n\
  -n NODE, --node NODE  Node Number\n\
  -p PROC, --proc PROC  Proc Number\n\
  -d DDLEVEL, --ddlevel DD Specific image identifier\n\
  -s SYMBOL, --symbol  Get value of symbol from PIBMEM for level 'sym'\n\
  -f FILE_PATH, --file_path FILE_PATH Path of the file if Target is FILE"

def main( argv ):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "l:t:n:p:d:s:f:h", ['level=', 'target=', 'node=', 'proc=', 'ddlevel=', 'symbol=', 'file_path=', 'help'])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        exit(1)

    # Default values
    level = 'trace'
    target = 'HW'
    node = 0
    proc = 0
    ddsuffix = 'DD1'
    file_path = ""
    symbol = ''

    # Parse the command line arguments
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            exit(1)
        elif opt in ('-l', '--level'):
            if arg in ('trace', 'forced-trace','attr','stack','ppestate','sbestate','sbestatus','sbelocalregister', 'sym'):
                level = arg
            else:
                print "level should be one of {trace,attr,stack,ppestate,sbestate,sbestatus,sbelocalregister, sym}"
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
            if arg in ('DD1', 'DD2'):
                ddsuffix = arg
            else:
                print "target should be one of {DD1, DD2}"
                exit(1)
        elif opt in ('-f', '--file_path'):
            try:
                file_path = arg
                assert os.path.exists(arg), "Did not find the file at,"+str(arg)
            except:
                print "file_path should a string path"
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
        cmdFifoOff = "setconfig USE_SBE_FIFO off"
        rc = os.system( cmdFifoOff )
        if ( rc ):
           print "ERROR running %s: %d " % ( cmdFifoOff, rc )
           return 1

    if(target == 'FILE'):
        # check if the file path exist or not
        assert os.path.exists(file_path), "Did not find the file at, "+str(arg)

    if ( level == 'all' ):
        print "Parsing everything"
        fillSymTable(target, ddsuffix)
        collectTrace(target, node, proc, ddsuffix, file_path )
        collectAttr(target, node, proc, ddsuffix, file_path )
        sbeStatus( target, node, proc )
        ppeState( target, node, proc, file_path )
        sbeState(  target, node, proc, file_path )
        sbeLocalRegister( target, node, proc, file_path )
    elif ( level == 'trace' ):
        fillSymTable(target, ddsuffix)
        collectTrace(target, node, proc, ddsuffix, file_path )
    elif ( level == 'forced-trace' ):
        fillSymTable(target, ddsuffix)
        forcedCollectTrace( target, node, proc, ddsuffix, file_path )
    elif ( level == 'attr' ):
        fillSymTable(target, ddsuffix)
        collectAttr( target, node, proc, ddsuffix, file_path )
    elif ( level == 'ppestate' ):
        ppeState( target, node, proc, file_path )
    elif ( level == 'sbestate' ):
        sbeState(  target, node, proc, file_path )
    elif ( level == 'sbestatus' ):
        sbeStatus( target, node, proc )
    elif ( level == 'sbelocalregister' ):
        sbeLocalRegister( target, node, proc, file_path )
    elif ( level == 'stack' ):
        fillSymTable(target, ddsuffix)
        collectStackUsage( node, proc )
    elif ( level == 'sym' ):
        fillSymTable(target, ddsuffix)
        getSymbolVal(target, node, proc, symbol, file_path)

    if(target != 'FILE'):
        # On cronus, set the FIFO mode to previous state
        cmdFifoLastState = "setconfig USE_SBE_FIFO " + cmdFifoLastState
        rc = os.system( cmdFifoLastState )
        if ( rc ):
           print "ERROR running %s: %d " % ( cmdFifoLastState, rc )
           return 1

if __name__ == "__main__":
    main( sys.argv )

