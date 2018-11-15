# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testIstepInvalid.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2019
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

TESTDATA = [0,0,0,3,
            0,0,0xA1,0x01,
            0,0x02,0x00,0x1]

EXPDATA = [0xc0,0xde,0xa1,0x01,
           0x00,0x02,0x00,0x0A,
           0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    testUtil.runCycles( 10000000 )
    testUtil.readDsFifo( EXPDATA )
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