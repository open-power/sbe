# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testGetMem_expdata.py $
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

GETMEM_TESTDATA =  [0,0,0,0x6,
                    0,0,0xA4,0x01,
                    0,0,0x0,0x02,
                    0,0,0,0,
                    0x08,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x80]  # length of data

GETMEM_EXP_RESPHDR =  [0x00,0x00,0x00,0x80,  # length of data
                       0xc0,0xde,0xa4,0x01,
                       0x0,0x0,0x0,0x0,
                       0x00,0x0,0x0,0x03]

GETMEM_EXP_RESPDATA = [0xAB,0xCD,0xEF,0x01,
                       0xAB,0xCD,0xEF,0x02,
                       0xAB,0xCD,0xEF,0x03,
                       0xAB,0xCD,0xEF,0x04,
                       0xAB,0xCD,0xEF,0x05,
                       0xAB,0xCD,0xEF,0x06,
                       0xAB,0xCD,0xEF,0x07,
                       0xAB,0xCD,0xEF,0x08]


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # GetMem test
    testUtil.writeUsFifo( GETMEM_TESTDATA )
    testUtil.writeEot( )
    # GetMem chipOp would send the read data first,
    # thus, would attempt to read the expected length of data first
    loop = 1
    while ( loop <= LOOP_COUNT ):
        testUtil.readDsFifo ( GETMEM_EXP_RESPDATA )
        loop += 1
    testUtil.readDsFifo( GETMEM_EXP_RESPHDR )
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

