# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testFifoReset.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
sys.path.append("targets/p9_nimbus/sbeTest")
import testUtil

err = False

# Test data that only contains the command header
TESTDATA = [0, 0, 0, 3,
            0, 0, 0xA1, 0x01]

# Complete test data
TESTDATA_FULL = [0, 0, 0, 3,
                 0, 0, 0xA1, 0x01,
                 0, 0x02, 0x00, 0x01]

# Get capabilities command. This will ensure the DS FIFO gets full
TESTDATA_2 = [0, 0, 0, 2,
              0, 0, 0xA8, 0x02]

def main():
    try:
        testUtil.runCycles(10000000)
        # Send a partial chip-op
        testUtil.writeUsFifo(TESTDATA)
        testUtil.resetFifo()
        # Make sure both the upstream and downstrem FIFOs are clear after the reset
        testUtil.waitTillUsFifoEmpty()
        testUtil.waitTillDsFifoEmpty()
        # Now send a complete chip-op on the upstream FIFO
        testUtil.writeUsFifo(TESTDATA_FULL)
        testUtil.writeEot()
        testUtil.resetFifo()
        # Make sure both the upstream and downstrem FIFOs are clear after the reset
        testUtil.waitTillUsFifoEmpty()
        testUtil.waitTillDsFifoEmpty()
        # Now send a get capabilities chip-op, so that in response, the DS FIFO
        # gets full before we do a reset
        testUtil.writeUsFifo(TESTDATA_2)
        testUtil.writeEot()
        testUtil.resetFifo()
        # Make sure both the upstream and downstrem FIFOs are clear after the reset
        testUtil.waitTillUsFifoEmpty()
        testUtil.waitTillDsFifoEmpty()
    except:
        print("\nTest completed with error(s), Raise error")
        raise
    print("\nTest completed with no errors")

main()

if err:
    print ("\nTest Suite completed with error(s)")
    #sys.exit(1)
else:
    print ("\nTest Suite completed with no errors")
    #sys.exit(0);

