#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/Debug/sbe-debug.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
import argparse
import sys
err = False

syms = {};

def fillSymTable(sbeObjDir):
    symFile = sbeObjDir + "/sbe.syms"
    f = open( symFile, 'r' )
    for line in f:
        words = line.split()
        if( len( words ) == 4 ):
            syms[words[3]] = [words[0], words[1]]

def collectTrace( hwpBinDir, sbeObjDir, target, proc ):
    cmd1 = ("."+hwpBinDir+"/p9_sbe_pibMemDump_wrap.exe " + \
                          syms['g_pk_trace_buf'][0] +\
                          " " + syms['g_pk_trace_buf'][1] + " " + target)
    cmd2 = "." + "/ppe2fsp dumpPibMem sbetrace.bin "
    cmd3 = ("." + "/fsp-trace -s " + sbeObjDir +\
                         "/trexStringFile sbetrace.bin > "+\
                         "sbe_"+str(proc)+"_tracMERG")
    cmd4 = "mv dumpPibMem dumpPibMem_trace"
    print "\ncollecting trace with commands -\n"
    print "cmd1:", cmd1
    rc = os.system( cmd1 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd1, rc )
       return 1

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

def collectAttr( hwpBinDir, sbeObjDir, target, proc ):
    cmd1 = ("."+hwpBinDir+"/p9_sbe_pibMemDump_wrap.exe " +\
                         syms['G_sbe_attrs'][0] + " " + \
                         syms['G_sbe_attrs'][1] + " " + target)
    cmd2 = "mv dumpPibMem sbeAttr.bin"
    cmd3 = ("."+ sbeObjDir + "/p9_xip_tool " +\
                 sbeObjDir + "/sbe_seeprom.bin -ifs attrdump sbeAttr.bin > "+\
                 "sbe_"+str(proc)+"_attrs")
    print "\ncollecting attributes with commands -\n"
    print "cmd1:", cmd1
    rc = os.system( cmd1 )
    if ( rc ):
       print "ERROR running %s: %d " % ( cmd1, rc )
       return 1

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

def main( argv ):
    parser = argparse.ArgumentParser( description = "SBE Dump Parser" )

    parser.add_argument( '-hwpBinDir', type=str, default = os.getcwd(), \
                                help = 'Path of p9_sbe_pibMemDump_wrap.exe')
    parser.add_argument( '-sbeObjDir', type=str, default = os.getcwd(), \
                                help = 'Path of sbe.syms file')
    parser.add_argument( '-l', '--level', choices = ['all', 'trace', 'attr'],\
                                default='all', help = 'Parser level' )
    parser.add_argument( '-t', '--target', choices = ['AWAN', 'HW'], \
                                required = 'true', help = 'Target type' )
    parser.add_argument( '-p', '--proc', type=int , default = 0, \
                                help = 'Proc Number' )

    args = parser.parse_args()

    if ( args.target == 'AWAN' ):
        target = "1"
    elif ( args.target == 'HW' ):
        target = "0"

    fillSymTable(args.sbeObjDir)
    if ( args.level == 'all' ):
        print "Parsing everything"
        collectTrace( args.hwpBinDir, args.sbeObjDir, target, args.proc )
        collectAttr( args.hwpBinDir, args.sbeObjDir, target, args.proc )
    elif ( args.level == 'trace' ):
        collectTrace( args.hwpBinDir, args.sbeObjDir, target, args.proc )
    elif ( args.level == 'attr' ):
        collectAttr( args.hwpBinDir, args.sbeObjDir, target, args.proc )

if __name__ == "__main__":
    main( sys.argv )
