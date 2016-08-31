# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testSram.py $
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
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 1

PUTSRAM_OCC_CNTLDATA =  [0,0,0,0x20,
                         0,0,0xa4,0x04, #magic
                         0,0,0,0x01,
                         0xe7,0xf0,0x00,0x00, #addr
                         0,0,0x01,0x00]  # length

PUTSRAM_OCC_TESTDATA =  [0xab,0xcd,0xef,0x12,
                         0xba,0xdc,0xfe,0x21,
                         0x34,0x56,0x78,0x9a,
                         0x43,0x65,0x87,0xa9,
                         0xab,0xcd,0xef,0x12,
                         0xba,0xdc,0xfe,0x21,
                         0x34,0x56,0x78,0x9a,
                         0x43,0x65,0x87,0xa9]

PUTSRAM_OCC_EXP_CNTLDATA  =  [0,0,0x01,0x00,
                              0xc0,0xde,0xa4,0x04,
                              0x0,0x0,0x0,0x0,
                              0x00,0x0,0x0,0x03]

GETSRAM_OCC_CNTLDATA =  [0,0,0,0x5,
                         0,0,0xa4,0x03,
                         0,0,0,0x01,
                         0xe7,0xf0,0x00,0x00,  #address
                         0x00,0x00,0x01,0x00]  # length of data

GETSRAM_OCC_EXP_TESTDATA =  [0xab,0xcd,0xef,0x12,  #data
                             0xba,0xdc,0xfe,0x21,
                             0x34,0x56,0x78,0x9a,
                             0x43,0x65,0x87,0xa9,
                             0xab,0xcd,0xef,0x12,
                             0xba,0xdc,0xfe,0x21,
                             0x34,0x56,0x78,0x9a,
                             0x43,0x65,0x87,0xa9]

GETSRAM_OCC_EXP_CNTLDATA =   [0x00,0x00,0x01,0x00,  # length
                              0xc0,0xde,0xa4,0x03,
                              0x0,0x0,0x0,0x0,
                              0x00,0x0,0x0,0x03];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # Put Occ Sram test - Linear - Can be tested over Normal
    # Debug mode
    testUtil.writeUsFifo( PUTSRAM_OCC_CNTLDATA )
    # Write 32 bytes of data 8 times => 32*8 = 256 = 0x100
    i_cnt = 0
    while i_cnt < 8:
        testUtil.writeUsFifo( PUTSRAM_OCC_TESTDATA )
        i_cnt = i_cnt+1

    testUtil.writeEot( )

    # Read the expected data for put sram
    testUtil.readDsFifo( PUTSRAM_OCC_EXP_CNTLDATA )
    testUtil.readEot( )

    # Get Sram Linear
    testUtil.writeUsFifo( GETSRAM_OCC_CNTLDATA )
    testUtil.writeEot( )

    # Read the Expected Data for get Sram
    i_cnt = 0
    while i_cnt < 8:
        testUtil.readDsFifo( GETSRAM_OCC_EXP_TESTDATA )
        i_cnt = i_cnt+1

    testUtil.readDsFifo( GETSRAM_OCC_EXP_CNTLDATA )
    testUtil.readEot( )

    # Put Occ Sram test - Circular - Can be enabled once we get
    # valid address range to read the circular data
    #testUtil.writeUsFifo( PUTSRAM_OCC_TESTDATA_1 )
    #testUtil.writeEot( )
    #testUtil.readDsFifo( PUTSRAM_OCC_EXPDATA_1 )
    #testUtil.readEot( )
    #testUtil.writeUsFifo( GETSRAM_OCC_TESTDATA_1 )
    #testUtil.writeEot( )
    #testUtil.readDsFifo( GETSRAM_OCC_EXPDATA_1 )
    #testUtil.readEot( )


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

