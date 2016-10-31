# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutMem.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 4

PUTMEM_TEST_HDR_WO_FMODE_WO_LCO =  [0,0,0,0x86,
                                    0,0,0xA4,0x02,
                                    0,0,0x0,0x02,
                                    0,0,0,0,
                                    0x08,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x80]

PUTMEM_TEST_HDR_W_FMODE_WO_LCO =  [0,0,0,0x86,
                                   0,0,0xA4,0x02,
                                   0,0,0x0,0x22,
                                   0,0,0,0,
                                   0x08,0x00,0x00,0x00,
                                   0x00,0x00,0x00,0x80]

PUTMEM_TEST_HDR_W_FMODE_W_LCO =  [0,0,0,0x86,
                                  0,0,0xA4,0x02,
                                  0x20,0,0x0,0x62,
                                  0,0,0,0,
                                  0x08,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x80]

PUTMEM_TEST_DATA = [0xAB,0xCD,0xEF,0x01,
                    0xAB,0xCD,0xEF,0x02,
                    0xAB,0xCD,0xEF,0x03,
                    0xAB,0xCD,0xEF,0x04,
                    0xAB,0xCD,0xEF,0x05,
                    0xAB,0xCD,0xEF,0x06,
                    0xAB,0xCD,0xEF,0x07,
                    0xAB,0xCD,0xEF,0x08]

PUTMEM_EXPDATA =  [0x00,0x00,0x00,0x80,
                   0xc0,0xde,0xa4,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03]


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
# First Case without Fast Mode without LCO
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( PUTMEM_TEST_HDR_WO_FMODE_WO_LCO )

    loop = 1
    while (loop <= LOOP_COUNT):
        testUtil.writeUsFifo( PUTMEM_TEST_DATA )
        loop += 1
    testUtil.writeEot( )

    testUtil.readDsFifo( PUTMEM_EXPDATA )
    testUtil.readEot( )

# Second Case with Fast Mode without LCO
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( PUTMEM_TEST_HDR_W_FMODE_WO_LCO )

    loop = 1
    while (loop <= LOOP_COUNT):
        testUtil.writeUsFifo( PUTMEM_TEST_DATA )
        loop += 1
    testUtil.writeEot( )

    testUtil.readDsFifo( PUTMEM_EXPDATA )
    testUtil.readEot( )

# Third Case with Fast Mode with LCO
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( PUTMEM_TEST_HDR_W_FMODE_W_LCO )

    loop = 1
    while (loop <= LOOP_COUNT):
        testUtil.writeUsFifo( PUTMEM_TEST_DATA )
        loop += 1
    testUtil.writeEot( )

    testUtil.readDsFifo( PUTMEM_EXPDATA )
    testUtil.readEot( )

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
main()

if err:
    print ("\nTest Suite completed with error(s)")
    #sys.exit(1)
else:
    print ("\nTest Suite completed with no errors")
    #sys.exit(0);

