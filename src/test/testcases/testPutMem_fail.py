# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutMem_fail.py $
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
from __future__ import print_function
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 4

PUTMEM_TEST_HDR =  [0,0,0x00,0x86,
                    0,0,0xA4,0x02,
                    0,0,0x0,0x02,
                    0,0,0,0,
                    0x08,0x00,0x00,0x04,  # Un-aligned PBA Address
                    0x00,0x00,0x00,0x80]

PUTMEM_TEST_DATA = [0xAB,0xCD,0xEF,0x01,
                    0xAB,0xCD,0xEF,0x02,
                    0xAB,0xCD,0xEF,0x03,
                    0xAB,0xCD,0xEF,0x04,
                    0xAB,0xCD,0xEF,0x05,
                    0xAB,0xCD,0xEF,0x06,
                    0xAB,0xCD,0xEF,0x07,
                    0xAB,0xCD,0xEF,0x08]

PUTMEM_EXPDATA =  [0x00,0x00,0x00,0x00,
                   0xc0,0xde,0xa4,0x02,
                   0x00,0xfe,0x00,0x0a,
                   0xff,0xdc,0x00,0x03,
                   0x00,0x00,0x00,0x00,
                   0x00,0xf8,0x82,0x19,
                   0x00,0x00,0x00,0x06]


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    testUtil.writeUsFifo( PUTMEM_TEST_HDR )

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

