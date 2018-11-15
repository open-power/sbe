# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testStartInstruction.py $
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
#from testWrite import *

LOOP_COUNT = 1


# Start All thread in Core0 with warn flag true
INST_START0_ALL_TESTDATA_WITH_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,1,0x20,0xf0]

INST_EXPDATA  =     [0xc0,0xde,0xa7,0x01,
                     0x0,0x0,0x0,0x0,
                     0x00,0x0,0x0,0x03]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    #stop all thread in core0
    testUtil.writeUsFifo( INST_START0_ALL_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
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

