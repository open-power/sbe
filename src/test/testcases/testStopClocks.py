# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testStopClocks.py $
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
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil
err = False
#from testWrite import *

STOPCLOCK_CORE_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x02,0x00,0x20]      # target type/chiplet id
STOPCLOCK_PROC_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x0,0x0,0x00]      # target type/chiplet id

STOPCLOCK_ALL_CORE_TESTDATA =  [0,0,0,0x3,
                                0,0,0xA9,0x03,
                                0x0,0x5,0x0,0xFF]      # target type/chiplet id

STOPCLOCK_PASS_EXPDATA =   [0xc0,0xde,0xa9,0x03,
                            0x0,0x0,0x0,0x0,
                            0x0,0x0,0x0,0x03];

STOPCLOCK_EQ_TESTDATA =  [0,0,0,0x3,
                          0,0,0xA9,0x03,
                          0x0,0x2,0x0,0x10]      # target type/chiplet id

STOPCLOCK_EX0_TESTDATA =  [0,0,0,0x3,
                          0,0,0xA9,0x03,
                          0x0,0x1,0x0,0x24]      # target type/chiplet id

STOPCLOCK_EX1_TESTDATA =  [0,0,0,0x3,
                          0,0,0xA9,0x03,
                          0x0,0x1,0x0,0x26]      # target type/chiplet id

STOPCLOCK_ALL_EQ_TESTDATA =  [0,0,0,0x3,
                              0,0,0xA9,0x03,
                              0x0,0x4,0x0,0xFF]      # target type/chiplet id

STOPCLOCK_INVALIDTARGET_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x09,0x00,0x20]      # target type/chiplet id
STOPCLOCK_FAIL_EXPDATA =   [0xc0,0xde,0xa9,0x03,
                            0x0,0x02,0x0,0x04,
                            0x0,0x0,0x0,0x03];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    testUtil.writeUsFifo( STOPCLOCK_CORE_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )

#    testUtil.writeUsFifo( STOPCLOCK_ALL_CORE_TESTDATA )
#    testUtil.writeEot( )
#    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
#    testUtil.runCycles( 10000000 )
#    testUtil.readEot( )
#
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( STOPCLOCK_EQ_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )

    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( STOPCLOCK_EX0_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )

    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( STOPCLOCK_EX1_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
#
#    testUtil.runCycles( 10000000 )
#    testUtil.writeUsFifo( STOPCLOCK_ALL_EQ_TESTDATA )
#    testUtil.writeEot( )
#    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
#    testUtil.runCycles( 10000000 )
#    testUtil.readEot( )

    testUtil.writeUsFifo( STOPCLOCK_PROC_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )

    testUtil.writeUsFifo( STOPCLOCK_INVALIDTARGET_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_FAIL_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
if testUtil.getMachineName() == "axone":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )
else:
    main()

    if err:
        print ("\nTest Suite completed with error(s)")
        #sys.exit(1)
    else:
        print ("\nTest Suite completed with no errors")
        #sys.exit(0);

