# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testGetCapabilities.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2017
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
import testUtil
err = False

TESTDATA = [0,0,0,2,
            0,0,0xA8,0x02 ]

EXPDATA1 = [0x0,0x0,0x0,0x0,
           0x0,0x0,0x0,0x0,
           0xa1,0x0,0x0,0x03, # istep/suspendio
           0x0,0x0,0x0,0x0,
           0xa2,0x0,0x0,0x0f, #getscom/putscom/modifyscom/putscomundermask
           0x0,0x0,0x0,0x0,
           0xa3,0x0,0x0,0x3,  #getring/putring
           0x00,0x0,0x0,0x0];

EXPDATA2 = [0xa4,0x0,0x0,0x0f, #GetMemPba/PutMemPba/GetSramOcc/PutSramOcc
           0x0,0x0,0x0,0x0,
           0xa5,0x0,0x0,0x03, #GetReg/PutReg
           0x0,0x0,0x0,0x0,
           0x0,0x0,0x0,0x0,
           0x0,0x0,0x0,0x0,
           0xa7,0x0,0x0,0x1, #  control Instruction
           0x00,0x0,0x0,0x0];

EXPDATA3 = [0xa8,0x0,0x0,0x13, #getcapability/getSbeFFDC/quiesce
           0x0,0x0,0x0,0x0,
           0xc0,0xde,0xa8,0x02,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x3];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    ( rc, out )  =   quiet_run_command( "sbe-ddlevel 0", output_modes.regular )
    if(rc == "DD1"):
        print "Not running Get Capabilities on DD1"
        return
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    # Ignore first 7 enteries ( major number, minor number
    # and fw version & tag) as they will keep on changing
    testUtil.readDsEntry( 7 )
    testUtil.readDsFifo( EXPDATA1 )
    testUtil.readDsFifo( EXPDATA2 )
    testUtil.readDsFifo( EXPDATA3 )
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

