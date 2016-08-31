# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testIstepAuto.py $
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
from sim_commands import *
import imp
err = False
testUtil = imp.load_source("testUtil", os.environ['SBE_TOOLS_PATH'] + "/testUtil.py")
EXPDATA = [0xc0,0xde,0xa1,0x01,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x03];
# MAIN Test Run Starts Here...
#-------------------------------------------------
def sbe_istep_func( major, minor ):
    try:
        TESTDATA = [0,0,0,3,
                     0,0,0xA1,0x01,
                    0,major,0,minor ]
        testUtil.runCycles( 10000000 )
        testUtil.writeUsFifo( TESTDATA )
        testUtil.writeEot( )
        testUtil.readDsFifo( EXPDATA )
        testUtil.readEot( )
    except:
        print ("\nTest completed with error(s). Raise error")
        # TODO via RTC 142706
        # Currently simics commands created using hooks always return
        # success. Need to check from simics command a way to return
        # Calling non existant command to return failure
        run_command("Command Failed");
        raise
    print ("\nTest completed with no errors")
        #sys.exit(0);

