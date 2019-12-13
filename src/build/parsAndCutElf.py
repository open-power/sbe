#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/parsAndCutElf.py $
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
import sys

# Workaround to cut SBE image from elf image.
def parserElf(argv):
    try:
        ddlevel = argv[1]
        outdir  = argv[2]
        img     = argv[3]
    except:
        print "Missing argument : arg[0] ddlevel; arg[1] output directory; arg[2] img (seeprom/pibmem)"
        exit(-1)
    SBE_OUT = outdir+"/sbe_"+img+"_"+ddlevel+".out"
    SBE_BIN = outdir+"/sbe_"+img+"_"+ddlevel+".bin"
    cmd = "readelf -S "+SBE_OUT
    firstSection = ".header"
    cmd1 = "nm "+SBE_OUT+" | grep  _sbe_image_size"
    output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    i = 0;
    for line in output.stdout:
        line = line.strip()

        if not line: continue

        if( (line.find(firstSection) != -1) ):
            tokens = line.split();
            startSize = int( tokens[5], 16 )
            print startSize
            break;

   # Get the location of sbe end
    output = subprocess.Popen(cmd1, shell=True, stdout=subprocess.PIPE)
    for line in output.stdout:
        line = line.strip()
        tokens = line.split();
        endSize = int( tokens[0], 16 );
        break;

    print endSize;
    if( (startSize == 0) or (endSize == 0)):
        exit(-1)

    # cut the image
    cmd1 = "dd skip=" + str(startSize) + " count=" + str(endSize) + " if="+SBE_OUT+" of="+SBE_BIN+" bs=1"
    rc = os.system(cmd1)
    if ( rc ):
       print "ERROR running %s: %d "%( cmd1, rc )
       exit(-1)

parserElf(sys.argv)
