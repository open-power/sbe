# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testQuiesce.py $
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
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil
import testMemUtil
err = False

TESTDATA = [0,0,0,2,
            0,0,0xA8,0x06 ]

EXPDATA = [0xc0,0xde,0xa8,0x06,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x3];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( EXPDATA )
    testUtil.readEot( )

    # fail get mem in quiesce state
    testMemUtil.getmem_failure(0x08000000, 128*2, 0x02, 0x00010008, False)

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

