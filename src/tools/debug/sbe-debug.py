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
err = False

syms = {};
if 'SBE_TOOLS_PATH' in os.environ:
    SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];
else:
    print "SBE_TOOLS_PATH not defined"
    exit(1)

baseAddr = 0xfffe8000

def fillSymTable(sbeObjDir, target, ddsuffix ):
    if (target == 'AWAN'):
        symFile = sbeObjDir + "/sim.sbe.syms"
    else:
        symFile = sbeObjDir + "/sbe_"+ddsuffix+".syms"
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

def createPibmemDumpFile( file_path, offset, len ):
    fileHandle = open(file_path)
    fileHandle.seek(int(offset, 16))
    fileData = fileHandle.read(int(len, 16))
    tempFileHandle = open("DumpPIBMEM", 'w')
    tempFileHandle.write(fileData)
    tempFileHandle.close()
    fileHandle.close()

def collectTrace( sbeObjDir, target, node, proc, ddsuffix, file_path ):
    offset = getOffset( 'g_pk_trace_buf' );
    len = "0x" + syms['g_pk_trace_buf'][1];
    print "\ncollecting trace with commands -\n"
    if(target == 'FILE'):
        createPibmemDumpFile(file_path, offset, len);
    else:
        cmd1 = ("p9_pibmem_dump_wrap.exe -quiet -start_byte " + \
                str(offset) +\
                 " -num_of_byte " + len + " "
                 " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1
    cmd2 = sbeObjDir + "/ppe2fsp DumpPIBMEM sbetrace.bin "
    cmd3 = (sbeObjDir + "/fsp-trace -s " + sbeObjDir +\
                         "/sbeStringFile_"+ddsuffix+" sbetrace.bin > "+\
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
def forcedCollectTrace( sbeObjDir, target, node, proc, ddsuffix, file_path ):
    # Collect entire PIBMEM
    offset = "0x00" # PIBMEM BASE
    len = "0x16400"
    print "\ncollecting trace with commands -\n"
    if(target == 'FILE'):
        createPibmemDumpFile(file_path, offset, len);
    else:
        cmd1 = ("p9_pibmem_dump_wrap.exe -quiet -start_byte " + \
                str(offset) +\
                 " -num_of_byte " + len + " "
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
                trace_index -= 20
                break
            trace_index += 1
        f.close()

    offset = "0x%x" % trace_index
    len = "0x1000"
    createPibmemDumpFile("DumpPIBMEM", offset, len);
    cmd2 = sbeObjDir + "/ppe2fsp DumpPIBMEM sbetrace.bin "
    cmd3 = (sbeObjDir + "/fsp-trace -s " + sbeObjDir +\
                         "/sbeStringFile_"+ddsuffix+" sbetrace.bin > "+\
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

def collectAttr( sbeObjDir, target, node, proc, ddsuffix, file_path ):
    if (target == 'AWAN'):
        sbeImgFile = "p9n_10.sim.sbe_seeprom.bin"
    else:
        sbeImgFile = "sbe_seeprom_"+ddsuffix+".bin"
    offset = getOffset( 'G_sbe_attrs' );
    len = "0x" + syms['G_sbe_attrs'][1];
    print "\ncollecting attributes with commands -\n"
    if(target == 'FILE'):
        createPibmemDumpFile(file_path, offset, len);
    else:
        cmd1 = ("p9_pibmem_dump_wrap.exe -quiet -start_byte " + \
                str(offset) +\
                " -num_of_byte " + len + " "
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
    cmd3 = ( sbeObjDir + "/p9_xip_tool " +\
             sbeObjDir + "/" + sbeImgFile + " -ifs attrdump sbeAttr.bin > "+\
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

def ppeState( sbeObjDir, target, node, proc, file_path ):
    if(target == 'FILE'):
        print "File path: ", file_path
        fileHandle = open(file_path)
        l_cnt = 0
        print '********************************************************************'
        print 'Reg Number  Reg Value    '
        while(l_cnt < os.path.getsize(file_path)):
            str1 = binascii.hexlify(fileHandle.read(2))
            str3 = binascii.hexlify(fileHandle.read(4))
            print str(str1).ljust(11),str(str3).ljust(20)
            l_cnt = l_cnt + 6;

        print '********************************************************************'
        fileHandle.close()
    else:
        cmd1 = ("p9_ppe_state_wrap.exe -verbose -sbe -snapshot" +\
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

def sbeLocalRegister( sbeObjDir, target, node, proc, file_path ):
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
        cmd1 = ("p9_sbe_localreg_dump_wrap.exe -quiet -sbe -snapshot" +\
                " -n" + str(node) + " -p" + str(proc))
        print "cmd1:", cmd1
        rc = os.system( cmd1 )
        if ( rc ):
            print "ERROR running %s: %d " % ( cmd1, rc )
            return 1

def sbeState( sbeObjDir, target, node, proc, file_path ):
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
        cmd1 = ("p9_pibms_reg_dump_wrap.exe -quiet" +\
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
    print "usage: testarg.py [-h] [-l {trace,attr,ppestate,sbestate,sbestatus,sbelocalregister}]\n\
\t\t\t\t[-t {AWAN,HW,FILE}] [-n NODE] [-p PROC] [-f FILE_PATH]\n\
\n\
SBE Dump Parser\n\
\n\
optional arguments:\n\
  -h, --help            show this help message and exitn\n\
  -l {trace,attr,ppestate,sbestate,sbestatus,sbelocalregister}, --level {trace,attr,ppestate,sbestate,sbestatus,sbelocalregister}\n\
                        Parser level\n\
  -t {AWAN,HW,FILE}, --target {AWAN,HW,FILE}\n\
                        Target type\n\
  -n NODE, --node NODE  Node Number\n\
  -p PROC, --proc PROC  Proc Number\n\
  -d DDLEVEL, --ddlevel DD Specific image identifier\n\
  -f FILE_PATH, --file_path FILE_PATH Path of the file if Target is FILE"

def main( argv ):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "l:t:n:p:d:f:h", ['level=', 'target=', 'node=', 'proc=', 'ddlevel=', 'file_path=', 'help'])
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

    # Parse the command line arguments
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            exit(1)
        elif opt in ('-l', '--level'):
            if arg in ('trace', 'forced-trace','attr','ppestate','sbestate','sbestatus','sbelocalregister'):
                level = arg
            else:
                print "level should be one of {trace,attr,ppestate,sbestate,sbestatus,sbelocalregister}"
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
        assert os.path.exists(arg), "Did not find the file at, "+str(arg)

    sbeObjDir = SBE_TOOLS_PATH;
    print "sbeObjDir", sbeObjDir
    fillSymTable(sbeObjDir, target, ddsuffix)
    if ( level == 'all' ):
        print "Parsing everything"
        collectTrace( sbeObjDir, target, node, proc, ddsuffix, file_path )
        collectAttr(  sbeObjDir, target, node, proc, ddsuffix, file_path )
        sbeStatus( target, node, proc )
        ppeState( sbeObjDir, target, node, proc, file_path )
        sbeState( sbeObjDir, target, node, proc, file_path )
        sbeLocalRegister( sbeObjDir, target, node, proc, file_path )
    elif ( level == 'trace' ):
        collectTrace( sbeObjDir, target, node, proc, ddsuffix, file_path )
    elif ( level == 'forced-trace' ):
        forcedCollectTrace( sbeObjDir, target, node, proc, ddsuffix, file_path )
    elif ( level == 'attr' ):
        collectAttr( sbeObjDir, target, node, proc, ddsuffix, file_path )
    elif ( level == 'ppestate' ):
        ppeState( sbeObjDir, target, node, proc, file_path )
    elif ( level == 'sbestate' ):
        sbeState( sbeObjDir, target, node, proc, file_path )
    elif ( level == 'sbestatus' ):
        sbeStatus( target, node, proc )
    elif ( level == 'sbelocalregister' ):
        sbeLocalRegister( sbeObjDir, target, node, proc, file_path )

    if(target != 'FILE'):
        # On cronus, set the FIFO mode to previous state
        cmdFifoLastState = "setconfig USE_SBE_FIFO " + cmdFifoLastState
        rc = os.system( cmdFifoLastState )
        if ( rc ):
           print "ERROR running %s: %d " % ( cmdFifoLastState, rc )
           return 1

if __name__ == "__main__":
    main( sys.argv )

