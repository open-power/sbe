# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testEnterMpipl.py $
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
from sim_commands import *

simicsObj = simics.SIM_run_command("get-master-procs")

err = False

TESTDATA = [0,0,0,2,
            0,0,0xA9,0x01 ]

EXPDATA = [0xc0,0xde,0xa9,0x01,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x3];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):

    cmd_read = "backplane0.dcm[0].chip[0].pib_cmp.sbe_mibo.read 0xc0002040 8"
    ( readValue, out ) = quiet_run_command( cmd_read, output_modes.regular )
    print ("\n 0xC0002040 LFR Local Register Value Before MpiplReset: " + str(hex(readValue >> 32)))

    # To check for MPIPL status, Read MPIPL is done or not
    #testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    testUtil.runCycles( 10000000 )

    # Verify MPIPL is done Reset
    max_sleep = 0
    while (1):
        ( rValue, out ) = quiet_run_command( cmd_read, output_modes.regular )
        isMpiplDone = ( rValue >> 32 ) & 0x00002000  # Check MpIpl done bit 18 from LFR
        print ("\nMPIPL System Reset is On Going ... isResetDone: " + str(hex(rValue >> 32)))
        if (isMpiplDone == 0):
            time.sleep(1)
        else:
            print ("PASS MPIPL Reset and 0xC0002040 LFR Local Register Value: " + str(hex(rValue >> 32)))
            break;
        max_sleep = max_sleep + 1
        if ( max_sleep == 10 ):
            print ("\nFAIL MPIPL System Reset. isResetDone: " + str(hex(rValue >> 32)))
            raise Exception('MPIPL System Reset Failed. 0xC0002040 Register value = ' + str(hex(rValue >> 32)))
            break;
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
