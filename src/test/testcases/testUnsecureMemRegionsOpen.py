# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testUnsecureMemRegionsOpen.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017
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
import os
import struct
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
import testMemUtil as testMemProcUtil
err = False

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main():
    testUtil.runCycles( 10000000 )

    # Test case 1: open RO mem region - success
    testMemProcUtil.setUnsecureMemRegion(0x07000000, 1024, 0x0111, 0)
    print ("Success - setUnsecureMemRegion - open RO")
    # Test case 2: close RW mem region - success
    # This region is kept open for mem testcases - will be closed by
    # testUnsecureMemRegionsClose.py
    testMemProcUtil.setUnsecureMemRegion(0x08000000, 1024, 0x0112, 0)
    print ("Success - setUnsecureMemRegion - open RW")
    # Test case 3: close RO mem region - success
    testMemProcUtil.setUnsecureMemRegion(0x07000000, 1024, 0x0120, 0)
    print ("Success - setUnsecureMemRegion - close RO")

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
