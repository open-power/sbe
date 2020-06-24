# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testHResetIpl.py $
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
from __future__ import print_function
import sys
import os
import struct
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
import testScomUtil
import time
from sim_commands import *

simicsObj = simics.SIM_run_command("get-master-procs")

err = False
i_fifoType = 0

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    # To trigger HReset using 50008 bit 13
    cmd_read = simicsObj[0] + ".pib_cmp.sbe_mibo.read 0xc0002040 8"
    ( readValue, out ) = quiet_run_command( cmd_read, output_modes.regular )
    print("\n 0xC0002040 LFR Local Register Value Before HReset: " + str(hex(readValue >> 32)))
    
    print("Trigger HReset ")
    cmd_write = simicsObj[0] + ".cfam_cmp.lbus_map.write 0x2820 0x00040000 -b 4"
    quiet_run_command( cmd_write, output_modes.regular )
    testUtil.runCycles( 1000000000 )

    # Verify HReset is done Reset 
    max_sleep = 0
    while (1):
        ( rValue, out ) = quiet_run_command( cmd_read, output_modes.regular )
        isDone = ( rValue >> 32 ) & 0x00004000
        print("\n HReset is On Going ...... isHResetDone: " + str(hex(rValue >> 32)))
        if (isDone == 0):
            time.sleep(1)
        else:
            print("PASS HReset IPL and 0xC0002040 LFR Local Register Value: " + str(hex(rValue >> 32)))
            break;
        max_sleep = max_sleep + 1
        if ( max_sleep == 10 ):
            print("FAIL HReset IPL and isHResetDone: " + str(hex(rValue >> 32)))
            raise Exception('HReset IPL Failed. 0xC0002040 Register value = ' + str(hex(rValue >> 32)))
            break;

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
try:
    main()
except:
    print( "\nTest Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

print( "\nTest Suite completed with no errors" )
