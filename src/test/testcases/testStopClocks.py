# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testStopClocks.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False
#from testWrite import *
STOPCLOCK_PROC_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x0,0x0,0x00]      # target type/chiplet id

STOPCLOCK_EQ_TESTDATA =  [0,0,0,0x3,
                          0,0,0xA9,0x03,
                          0x0,0x2,0x0,0x20]      # target type/chiplet id

STOPCLOCK_ALL_EQ_TESTDATA =  [0,0,0,0x3,
                              0,0,0xA9,0x03,
                              0x0,0x2,0x0,0xFF]      # target type/chiplet id

STOPCLOCK_CORE_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x3,0x0,0x0]      # target type/chiplet Instance

STOPCLOCK_ALL_CORE_TESTDATA =  [0,0,0,0x3,
                                0,0,0xA9,0x03,
                                0x0,0x3,0x0,0xFF]      # target type/chiplet id

STOPCLOCK_INVALIDTARGET_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x09,0x00,0x20]      # target type/chiplet id

STOPCLOCK_PASS_EXPDATA =   [0xc0,0xde,0xa9,0x03,
                            0x0,0x0,0x0,0x0,
                            0x0,0x0,0x0,0x03];

STOPCLOCK_FAIL_EXPDATA =   [0xc0,0xde,0xa9,0x03,
                            0x0,0x02,0x0,0x04,
                            0x0,0x0,0x0,0x03];

STOPCLOCK_PERV_TESTDATA =  [0,0,0,0x3,
                            0,0,0xA9,0x03,
                            0x0,0x1,0x0,0xFF]      # target type/chiplet id



# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    testUtil.writeUsFifo( STOPCLOCK_PROC_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for Proc Level Stop Clock completed!!" )

    testUtil.writeUsFifo( STOPCLOCK_PERV_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for PERV Level Stop Clock completed!!" )


    testUtil.writeUsFifo( STOPCLOCK_EQ_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for specific EQ Stop Clock completed!!" )


    testUtil.writeUsFifo( STOPCLOCK_ALL_EQ_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for All EQ Stop Clock completed!!" )

    testUtil.writeUsFifo( STOPCLOCK_CORE_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for Specific Core Stop Clock completed!!" )

    testUtil.writeUsFifo( STOPCLOCK_ALL_CORE_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_PASS_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for All Core Stop Clock completed!!" )

    testUtil.writeUsFifo( STOPCLOCK_INVALIDTARGET_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOPCLOCK_FAIL_EXPDATA )
    testUtil.runCycles( 10000000 )
    testUtil.readEot( )
    print ( "Test for invalid target for Stop Clock completed!!" )

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
try:
    main()
except:
    print ( "\nTest Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

print ( "\nTest Suite completed with no errors" )

