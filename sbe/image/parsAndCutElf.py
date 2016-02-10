#!/usr/bin/python
import os
import subprocess
import re
import random
import sys

# Workaround to cut SBE image from elf image.
def parserElf():
    cmd = "readelf -S ../obj/sbe_seeprom.out"
    firstSection = ".header"
    cmd1 = "nm ../obj/sbe_seeprom.out | grep  _sbe_image_size"
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
    cmd1 = "dd skip=" + str(startSize) + " count=" + str(endSize) + " if=../obj/sbe_seeprom.out of=../obj/sbe_seeprom.bin bs=1"
    rc = os.system(cmd1)
    if ( rc ):
       print "ERROR running %s: %d "%( cmd1, rc )
       exit(-1)

parserElf()

