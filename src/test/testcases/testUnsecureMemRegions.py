# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testUnsecureMemRegions.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2018
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
import os
import struct
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
import testMemUtil as testMemProcUtil
err = False

MEM_WINDOWS_BASE_ADDRESS = 0x08000000 + 0x2000

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main():
    # Close the HB dump region
    testMemProcUtil.setUnsecureMemRegion(0x08000000, 32*1024*1024, 0x0120, 0)

    # Test case 1: open RO mem region - success
    testcase = 1
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS, 1024, 0x0111, 0)
    # Test case 2: open RW mem region - success
    # This region is kept open for mem testcases - will be closed by
    # testUnsecureMemRegionsClose.py
    testcase = 2
    testMemProcUtil.setUnsecureMemRegion(0x08000000, 0x2000, 0x0112, 0)
    # Test case 3: close RO mem region - success
    testcase = 3
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS, 1024, 0x0120, 0)

    # Test case 4: open 9th window - failure
    testcase = 4
    # 2
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS, 0x10, 0x0111, 0)
    # 3
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x10, 0x10, 0x0111, 0)
    # 4
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x20, 0x10, 0x0111, 0)
    # 5
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x30, 0x10, 0x0111, 0)
    # 6
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x10, 0x0111, 0)
    # 7
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x50, 0x10, 0x0111, 0)
    # 8
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x10, 0x0111, 0)
    # 9 - failed - response expected -
    # SBE_PRI_USER_ERROR = 0x03
    # SBE_SEC_MAXIMUM_MEM_REGION_EXCEEDED = 0x16
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x70, 0x10, 0x0111, 0x00030016)
    # Test case 5: open existing window - failure
    testcase = 5
    # response expected
    # SBE_PRI_USER_ERROR = 0x03
    # SBE_SEC_MEM_REGION_AMEND_ATTEMPTED = 0x17
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x10, 0x0111, 0x00030017)
    # Test case 6: close non-existing window - failure
    testcase = 6
    # response expected
    # SBE_PRI_USER_ERROR = 0x03
    # SBE_SEC_MEM_REGION_NOT_FOUND = 0x15
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x70, 0x10, 0x0120, 0x00030015)
    # Test case 7: remove a window in between - adding it again should pass
    testcase = 7
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x20, 0x10, 0x0120, 0)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x20, 0x10, 0x0111, 0)
    # Test case 8: remove all and open again to validate removal
    testcase = 8
    # 1
    testMemProcUtil.setUnsecureMemRegion(0x08000000, 0x2000, 0x0120, 0)
    # Open Window with start address not exsting, but size spanning across
    # existing window
    # failed - response expected -
    # SBE_PRI_USER_ERROR = 0x03
    # SBE_SEC_MEM_REGION_AMEND_ATTEMPTED = 0x17
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS-0x10, 0x20, 0x0111, 0x00030017)
    # 2
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS, 0x10, 0x0120, 0)
    # 3
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x10, 0x10, 0x0120, 0)
    # 4
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x20, 0x10, 0x0120, 0)
    # 5
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x30, 0x10, 0x0120, 0)
    # 6
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x10, 0x0120, 0)
    # 7
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x50, 0x10, 0x0120, 0)
    # 8
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x10, 0x0120, 0)
    # 1
    testMemProcUtil.setUnsecureMemRegion(0x08000000, 0x2000, 0x0112, 0)
    # 2
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS, 0x10, 0x0111, 0)
    testcase = "8: new window subsuming exsiting windows"
    # failed - response expected -
    # SBE_PRI_USER_ERROR = 0x03
    # SBE_SEC_MEM_REGION_AMEND_ATTEMPTED = 0x17
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS-0x10, 0x30, 0x0111, 0x00030017)
    testcase = "8"
    # 3
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x10, 0x10, 0x0111, 0)
    # 4
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x20, 0x10, 0x0111, 0)
    # 5
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x30, 0x10, 0x0111, 0)
    # 6
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x10, 0x0111, 0)
    # 7
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x50, 0x10, 0x0111, 0)
    # 8
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x10, 0x0111, 0)
    # 9 - failed - response expected -
    # SBE_PRI_USER_ERROR = 0x03
    # SBE_SEC_MAXIMUM_MEM_REGION_EXCEEDED = 0x16
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x70, 0x10, 0x0111, 0x00030016)

    # Cleanup - close all the windows open except the one for mem testcases
    # 2
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS, 0x10, 0x0120, 0)
    # 3
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x10, 0x10, 0x0120, 0)
    # 4
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x20, 0x10, 0x0120, 0)
    # 5
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x30, 0x10, 0x0120, 0)
    # 6
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x10, 0x0120, 0)
    # 7
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x50, 0x10, 0x0120, 0)
    # 8
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x10, 0x0120, 0)

    # Test case 9: access memory in unopened window - ADU
    testcase = 9
    testMemProcUtil.getmem_failure(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x40, 0xA5, 0x00050014)
    # Test case 10: access memory in unopened window - PBA
    testcase = 10
    testMemProcUtil.getmem_failure(MEM_WINDOWS_BASE_ADDRESS+0x80, 0x80, 0x02, 0x00050014)

    # Test case 11: access memory spanning across two windows
    testcase = 11
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x40, 0x0111, 0)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x80, 0x40, 0x0111, 0)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0xC0, 0x40, 0x0111, 0)
    # start and end on edges
    testMemProcUtil.getmem(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x80, 0xA5)
    # start in between and end on edge
    testMemProcUtil.getmem(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x60, 0xA5)
    # start on edge and end in between
    testMemProcUtil.getmem(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x60, 0xA5)
    # start on edge and end in between - 3rd window
    testMemProcUtil.getmem(MEM_WINDOWS_BASE_ADDRESS+0x40, 0xA0, 0xA5)

    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x40, 0x0120, 0)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x80, 0x40, 0x0120, 0)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0xC0, 0x40, 0x0120, 0)

    # Test case 12: read access in read-write window - ADU
    testcase = 12
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x80, 128, 0x0112, 0)
    testMemProcUtil.getmem(MEM_WINDOWS_BASE_ADDRESS+0x80, 128, 0xA5)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x80, 128, 0x0120, 0)
    # Test case 13: read access in read-write window - PBA
    testcase = 13
    testMemProcUtil.getmem(0x08000080, 128, 0x02)

    # Test case 14: write access in read-only window - ADU
    testcase = 14
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x80, 128, 0x0111, 0)
    data = os.urandom(8)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem_failure(MEM_WINDOWS_BASE_ADDRESS+0x80, data, 0xA5, 0x00050014)
    # Test case 15: write access in read-only window - PBA
    testcase = 15
    data = os.urandom(128)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem_failure(MEM_WINDOWS_BASE_ADDRESS+0x80, data, 0x02, 0x00050014)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x80, 128, 0x0120, 0)
    # Test case 16: access memory for which a partial window is open
    testcase = 16
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x30, 0x0111, 0)
    # start in between and end outside
    testMemProcUtil.getmem_failure(MEM_WINDOWS_BASE_ADDRESS+0x60, 0x40, 0xA5, 0x00050014)
    # start on edge and end outside
    testMemProcUtil.getmem_failure(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x40, 0xA5, 0x00050014)
    # start and end within
    testMemProcUtil.getmem(MEM_WINDOWS_BASE_ADDRESS+0x50, 0x10, 0xA5)
    testMemProcUtil.setUnsecureMemRegion(MEM_WINDOWS_BASE_ADDRESS+0x40, 0x30, 0x0120, 0)
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
