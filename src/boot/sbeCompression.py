#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/boot/sbeCompression.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2019
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
import binascii
import fileinput
import getopt
import struct
import operator
err = False

def compress(inputFile, compressedFile):

    try:
      f = open(inputFile, "rb")
    except IOError as e :
      print "I/O error File for File to be compressed."
      sys.exit(1)

    try:
      fW = open(compressedFile, "wb")
    except IOError as e :
      print "I/O error File for compressed file."
      sys.exit(1)

    if os.stat(inputFile).st_size < 4 :
      print "File is less than four bytes."
      sys.exit(1)

    instDict = dict()
    for i in range(0, os.stat(inputFile).st_size / 4 ):

       fourByt = f.read(4)

       if fourByt in instDict:

          iCount = instDict[fourByt]
          instDict[fourByt] = iCount + 1;

       else :

          iCount = 1
          instDict[fourByt] = iCount

    sortedList =  sorted(instDict.iteritems(), key=operator.itemgetter(1), reverse = True)

    sortedList[256:] = []
    instList = []
    iCount = 0
    for k, v in sortedList:
      instList.append(k)

    for x in instList:
       fW.write(x)

    fileSize = os.stat(inputFile).st_size
    fW.write(struct.pack(">Q",fileSize))

    f.seek(0, 0)
    strBits = ""
    count = 0

    #Create a bitmap for each four bytes of binary.
    for i in range(0, os.stat(inputFile).st_size / 4 ):

       fourByt = f.read(4)
       if fourByt in instList:
          strBits += '1'

       else :
          strBits += '0'

       if ((len(strBits) == 32) or (i == (os.stat(inputFile).st_size / 4) - 1)):
          value = int(strBits, 2)
          fW.write(struct.pack('>I', value))
          strBits = ""
          count = count + 1

    value = 0
    #To make the bit map eight byte alligned in compressed image.
    if ((count % 2) == 0):
      padCount = count
    else:
      padCount = count + 1

    for i in range(count, padCount):
      fW.write(struct.pack('>I', value))

    f.seek(0, 0)

    for i in range(0, os.stat(inputFile).st_size / 4 ):

       fourByt = f.read(4)

       if fourByt in instList:
          ind = instList.index(fourByt)
          fW.write(struct.pack('>B', ind))

       else:
          fW.write(fourByt)

    f.close()
    fW.close()

def usage():
    print "usage: sbeCompression.py [-h] [-l <path>] [-i <image>]"
    print "SBE Compression Parser"
    print "Arguments:"
    print "-h, --help\t\tshow this help message and exit"
    print "-l, --imageLoc\t\tSeeprom Binary Location"
    print "-i, --image\t\tSeeprom Binary"
    print "-p, --p9_xip_tool\t\tp9_xip_tool path"
    return 1

def main( argv ):

    try:
        opts, args = getopt.getopt(argv[1:], "l:i:p:h", ['imageLoc=', 'image=', 'p9_xip_tool=', 'help'])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        exit(1)

    imagePath = ''
    image = ''
    p9_xip_tool = ''

    for opt, arg in opts:
        if opt in ('-l', '--imageLoc'):
            imagePath = arg
        elif opt in ('-i', '--image'):
            image = arg
        elif opt in ('-p', '--p9_xip_tool'):    
            p9_xip_tool = arg
        else:
            usage()
            exit(1)

    #Make a copy of SEEPROM binary.
    cmd1 = "cp " + imagePath + "/" + image + " " + imagePath +  "/" + image + ".orig"
    rc = os.system(cmd1)
    if rc:
      print "Unable to make copy of seeprom binary"
      sys.exit(1)

    #Extract base from SEEPROM binary.
    cmd2 = p9_xip_tool + " " + imagePath + "/" + image + " extract .base " + imagePath + "/" + image + ".base"
    rc = os.system(cmd2)
    if rc:
      print "Unable to extract the base from seeprom binary"
      sys.exit(1)

    #Compress the base section
    compress(imagePath + "/" + image + ".base", imagePath + "/" + image + ".base.compressed")

    #Delete the base section from SEEPEOM binary.
    cmd3 = p9_xip_tool + " " + imagePath + "/" + image + " delete .base"
    rc = os.system(cmd3)
    if rc:
      print "Unable to delete base section from seeprom binary"
      sys.exit(1)

    #Append the base section from SEEPEOM binary.
    cmd4 = p9_xip_tool + " " + imagePath +  "/" + image + " append .base " + imagePath + "/" + image + ".base.compressed"
    rc = os.system(cmd4)
    if rc:
      print "Unable to append the base section"
      sys.exit(1)

if __name__ == "__main__":
    main( sys.argv )

