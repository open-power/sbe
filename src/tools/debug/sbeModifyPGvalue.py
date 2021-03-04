#!/usr/bin/env python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/sbeModifyPGvalue.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020,2021
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
import sys
import binascii
import fileinput
import getopt
import struct
import operator

err = False

SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];

if 'BLD' in os.environ:
    BLD = os.environ['BLD'];
    #Add sbeModifyPGvalue.py path in PATH for Denali system.
    SBE_SCRIPTS_PATH_DENALI=os.environ['BLD']+"/obj/ppc/sbei/sbfw/simics/"
    # preference is SBE_TOOLS_PATH, current woring directory and then system path
    PATH = SBE_TOOLS_PATH+os.pathsep+os.getcwd()+os.pathsep+os.environ['PATH']+os.pathsep+SBE_SCRIPTS_PATH_DENALI
else:
    #Add sbeModifyPGvalue.py path in PATH for Rainier system.
    SBE_SCRIPTS_PATH_RAINIER=os.getcwd()+"/sbe_sim_data/"
    # preference is SBE_TOOLS_PATH, current woring directory and then system path
    PATH = SBE_TOOLS_PATH+os.pathsep+os.getcwd()+os.pathsep+os.environ['PATH']+os.pathsep+SBE_SCRIPTS_PATH_RAINIER

def getFilePath(filename):
    for dir in PATH.split(os.pathsep):
        retPath = os.path.join(dir, filename)
        if(os.path.exists(retPath)):
            return os.path.abspath(retPath)
    print("ERROR: file " + filename +" not found")
    return 1

def swap32(x):
    return (((x << 24) & 0xFF000000) |
            ((x <<  8) & 0x00FF0000) |
            ((x >>  8) & 0x0000FF00) |
            ((x >> 24) & 0x000000FF))

def get_mvpd_eq(inputImage, eqNumber):
    # This method update the PG MVPD EQ bytes of a provided Seeprom Image##
    cmd1 = getFilePath("ipl_image_tool") + " " + inputImage + " getv ATTR_PG_MVPD " + str(eqNumber)
    rc = subprocess.Popen(cmd1, shell=True, stdout=subprocess.PIPE)
    eqPgData = rc.stdout.read()
    return eqPgData

def put_mvpd_eq(inputImage, eqNumber, eqPgData):

    # This method update the PG MVPD EQ bytes of a provided Seeprom Image##
    pgValueStr = get_mvpd_eq(inputImage, eqNumber)
    pgValue = int(pgValueStr, 16)
    # Verify Activation and Deactivation for core state
    putData = 0
    if (eqPgData  == int("0x78000", 16)):
      putData = eqPgData | pgValue
    else:
      putData = eqPgData & pgValue
    cmd1 = getFilePath("ipl_image_tool") + " " + inputImage + " setv ATTR_PG_MVPD " + str(eqNumber) + " " + str(hex(putData))
    subprocess.Popen(cmd1, shell=True, stdout=subprocess.PIPE)

def gard_all_core_mvpd_eq(inputImage):

    gard_all_core = int("0x78000",16)
    # Num of EQ pervasive chiplets per chip
    for eqNo in range(8):
      put_mvpd_eq(inputImage, eqNo+32, gard_all_core)

def update_mvpd_eq(inputImage, eqNum, coreNum):

    # This method update the PG MVPD EQ bytes of a provided Seeprom Image##
    coreMask = ["0xFFFBFFFF", "0xFFFDFFFF", "0xFFFEFFFF", "0xFFFF7FFF"]
    make = coreMask[coreNum]
    putCoreMask = int(make, 16)
    put_mvpd_eq(inputImage, eqNum+32, putCoreMask)

def usage():
    print("usage: sbeModifyPGvalue.py [-h] [-i <seeprom ecc image>] [-c list of core numbers from 0 to 31")
    print("Example : sbeModifyPGvalue.py -i sbe_seeprom_p10.bin.ecc -c \" 1 2 3 4 5 6 7 \" ")
    print("Listed cores are activated via ATTR_PG_MVPD and rest of the code are not functional state")
    print("Arguments:")
    print("-h, --help\t\tshow this help message and exit")
    print("-i, --image\t\tSeeprom ECC Binary")
    print("-c, --core_list\t\tActive core list")
    return 1

def to_int32(val):
    val &= ((1<<32)-1)
    if val & (1<<31): val -= (1<<32)
    print("hex value: " + hex(val))
    return val

def updateAttrPg( image, coresnum):

    #Make a copy of SEEPROM binary.
    cmd1 = "cp " + image + " " + image + ".orig"
    rc = os.system(cmd1)
    if rc:
      print("Unable to make copy of ecc seeprom binary")
      sys.exit(1)

    cmd2 = getFilePath("ecc") + " --p8 --remove " + image + " --output "  + image + "updat.bin"

    rc = os.system(cmd2)
    if rc:
      print("Unable to remove ECC from seeprom binary")
      sys.exit(1)
    image_ecc = image
    image = image + "updat.bin"
    #Extract report from SEEPROM binary.
    cmd3 = getFilePath("ipl_image_tool") + " " + image + " report > " + image + ".Report"
    rc = os.system(cmd3)
    if rc:
      print("Unable to extract the report from seeprom binary")
      sys.exit(1)

    #Modify the code ATTR MVPD values
    # DeActive all core functional state
    gard_all_core_mvpd_eq(image)
    # Enable given core into funtional state
    core_str_list = "0 1 2 3"
    if (coresnum == 2) :
      core_str_list = "0 1"
    elif (coresnum == 4 ):
      core_str_list = "0 1 2 3"
    elif (coresnum == 8 ):
      core_str_list = "0 1 2 3 4 5 6 7"
    elif (coresnum == 16 ):
      core_str_list = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15"
    else:
      core_str_list = "0 1 2 3"

    cores = core_str_list.split()
    for core in cores:
      coreId = int(core) % 4 # Num of cores per EQ chiplet
      eqId = int(core) / 4 # Num of cores per EQ chiplet
      print("Updated ATTR_PG_MVPD data for Core No: " + str(core) + " coreId: " + str(coreId) + " eqId: " + str(eqId))
      update_mvpd_eq(image, eqId, coreId )

    #Delete the fixed section from SEEPEOM binary.
    cmd4 = getFilePath("ipl_image_tool") + " " + image + " report > " + image + ".updatedReport"
    rc = os.system(cmd4)
    if rc:
      print("Unable to Delete the report from seeprom binary")
      sys.exit(1)

    #Make a copy of SEEPROM binary.
    cmd7 = "cp " + image + " sbe_seeprom_DD1.bin"
    rc = os.system(cmd7)
    if rc:
      print("Unable to make  seeprom binary")
      sys.exit(1)

    cmd6 = getFilePath("ecc") + " --p8 image --inject " + image + " --output "  + image_ecc

    rc = os.system(cmd6)
    if rc:
      print("Unable to append the ECC to seeprom binary")
      sys.exit(1)

def main( argv ):
    try:
        opts, args = getopt.getopt(argv[1:], "i:c:h", ['image=','core_list=', 'help'])
    except getopt.GetoptError as err:
        print(str(err))
        usage()
        exit(1)

    # Default values
    image = ''
    cores = []
    verify = 0

    for opt, arg in opts:
        if opt in ('-i', '--image'):
            image = arg
            verify = 1
        elif opt in ('-c', '--core_list'):
            cores = arg.split()
            verify = 1
            for eachNo in cores:
              if int(eachNo) > 31 :
                print("core list should be 0 to 31. In avalid core no: " + str(eachNo))
                exit(1)
        else:
            usage()
            exit(1)

    if verify == 0 :
      usage()
      exit(1)
    updateAttrPg(image,cores)

if __name__ == "__main__":
    main( sys.argv )
