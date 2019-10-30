# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testIstepAuto.py $
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
import copy
from sim_commands import *
import imp

err = False

def collectFFDC():
    simics.SIM_run_command('sbe-trace 0')
    simics.SIM_run_command('sbe-stack 0')
    simics.SIM_run_command('sbe-regffdc 0')
    simics.SIM_run_command('backplane0.dcm0.chip0.pib_cmp.sbe_ppe->ppe_state')
    simics.SIM_run_command('backplane0.dcm0.chip0.cfam_cmp.sbe_fifo->upstream_hw_fifo')
    simics.SIM_run_command('backplane0.dcm0.chip0.cfam_cmp.sbe_fifo->downstream_hw_fifo')

SBE_TOOLS_PATH = simenv.sbe_scripts_path
testUtil = imp.load_source("testUtil", SBE_TOOLS_PATH + "/testUtil.py")
EXPDATA = [0xc0,0xde,0xa1,0x01,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x03];
gIstepArray = {
        2:[2, 15],#istep 2.2 to 2.15
        3:[1, 23],#istep 3.1 to 3.23
        4:[1, 20],#istep 4.1 to 4.20
        5:[1, 3], #istep 5.1 to 5.3
        96:[1, 8],#istep 96.1 to 96.8
        97:[1, 7], #istep 97.1 to 97.7
        98:[1, 1] #istep 98.1 to 98.1
               }
# MAIN Test Run Starts Here...
#-------------------------------------------------
def sbe_istep_func( inum1, inum2, node=0):
    # Convert float number to string, which would help extracting
    # decimal and integral part separately
    # Interpretation:
    # (Single iStep)    inum1=2 inum2=2     => startMajor = 2; startMinor = 2;
    #                                          endMajor = 0; endMinor = 0;
    # (Range of iSteps) inum1=2.3 inum2=3.7 => startMajor = 2; startMinor = 3;
    #                                          endMajor = 3; endMinor = 7;
    istr1 = str(inum1)
    istr2 = str(inum2)
    startMajor = 0
    startMinor = 0
    endMajor = 0
    endMinor = 0
    startMajor = int(istr1.split('.')[0])
    # Check for single istep condition, where inum1=x.0 and inum2=x.0
    # If not extract the start and end isteps range
    if(int(istr1.split('.')[1]) != 0):
        startMinor = int(istr1.split('.')[1])
    if(int(istr2.split('.')[1]) != 0):
        endMajor = int(istr2.split('.')[0])
        endMinor = int(istr2.split('.')[1])
    else:
        startMinor = int(istr2.split('.')[0])

    # Make sure array is a deep copy,
    # as we are modifying and setting up the local array
    # based on ranges requested
    lIstepArray = copy.deepcopy(gIstepArray)
    lIstepArray[startMajor][0] = startMinor
    if endMajor != 0:
        lIstepArray[endMajor][1] = endMinor
    else:
        endMajor = startMajor
        lIstepArray[startMajor][1] = lIstepArray[startMajor][0]
    for major in range(startMajor, endMajor+1):
        for minor in range(lIstepArray[major][0], lIstepArray[major][1] + 1):
            print "Running:"+str(major)+"."+str(minor)

            try:
                TESTDATA = [0,0,0,3,
                             0,0,0xA1,0x01,
                            0,major,0,minor ]
                testUtil.runCycles( 10000000 )
                testUtil.writeUsFifo( TESTDATA, node)
                testUtil.writeEot( node)
                testUtil.runCycles( 10000 )
                testUtil.readDsFifo( EXPDATA, node)
                testUtil.runCycles( 10000 )
                testUtil.readEot( node )

            except:
                print ("\nTest completed with error(s). Raise error")
                collectFFDC()
                # TODO via RTC 142706
                # Currently simics commands created using hooks always return
                # success. Need to check from simics command a way to return
                # Calling non existant command to return failure
                run_command("Command Failed");
                raise
    print ("\nTest completed with no errors")
        #sys.exit(0);



