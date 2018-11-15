# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testFastArray.py $
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False

CONTROL_FAST_ARRAY_SETUP_TESTDATA = [0, 0, 0, 0x05,
                                     0, 0, 0xA6, 0x01,
                                     0, 0x02, 0x20, 0x01,    #PERV, 0x20 - Core chiplet, setup
                                     0x12, 0x34, 0x56, 0x78, #data[0-31]
                                     0x9a, 0xbc, 0xde, 0xf0] #data[32-63]
CONTROL_FAST_ARRAY_CATCHUP_TESTDATA= [0, 0, 0, 0x05,
                                     0, 0, 0xA6, 0x01,
                                     0, 0x02, 0x20, 0x02,    #PERV, 0x20 - Core chiplet, catchup
                                     0x00, 0x00, 0x00, 0x00, #data[0-31]
                                     0x00, 0x00, 0x00, 0x01] #data[32-63]
CONTROL_FAST_ARRAY_CLEANUP_TESTDATA = [0, 0, 0, 0x05,
                                       0, 0, 0xA6, 0x01,
                                       0, 0x02, 0x20, 0x03,    #PERV, 0x20 - Core chiplet, cleanup
                                       0x12, 0x34, 0x56, 0x78, #dont care
                                       0x9a, 0xbc, 0xde, 0xf0] #dont care

CONTROL_FAST_ARRAY_VALID = [0xC0, 0xDE, 0xA6, 0x01,
                             0, 0, 0, 0,
                             0, 0, 0, 0x03]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    print ("\nStarting control fastarray test")
    print ("\nTest case: Setup")
    testUtil.writeUsFifo( CONTROL_FAST_ARRAY_SETUP_TESTDATA)
    testUtil.writeEot( )
    testUtil.readDsFifo( CONTROL_FAST_ARRAY_VALID)
    testUtil.readEot( )
    print ("\nTest case: Catchup")
    testUtil.writeUsFifo(CONTROL_FAST_ARRAY_CATCHUP_TESTDATA)
    testUtil.writeEot( )
    testUtil.readDsFifo( CONTROL_FAST_ARRAY_VALID)
    testUtil.readEot( )
    print ("\nTest case: Cleanup")
    testUtil.writeUsFifo( CONTROL_FAST_ARRAY_CLEANUP_TESTDATA)
    testUtil.writeEot( )
    testUtil.readDsFifo( CONTROL_FAST_ARRAY_VALID)
    testUtil.readEot( )

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