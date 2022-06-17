# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUSyncTargetState.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2022
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
import testPSUUtil
import testRegistry as reg
import testUtil


def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return hex_string


def syncTarget(coreFuncState):
    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation
    '''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data -
#------------------------------------------------------------------------------------------------------------------------------
    '''

    coreState = getdoubleword((coreFuncState << 32))
    sbe_test_data = (
        #-----------------------------------------------------------------------------------------------------
        #   OP      Reg                            ValueToWrite         size    Test Expected Data       Description
        #-----------------------------------------------------------------------------------------------------
        #                                          Sync Target State Request
        ["write", reg.REG_MBOX0,                   "0000030000F0D103",   8,     "None",                 "Writing to MBOX0 address"],
        #                                          Core Functional State
        ["write", reg.REG_MBOX1,                   coreState,            8,     "None",                 "Writing to MBOX1 address"],
        #
        ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000",    8,     "None",                 "Update SBE Doorbell register to interrupt SBE"],
        )

    '''
#-----------------------------------------------------------------------
# Do not modify - Used to simulate interrupt on Ringing Doorbell on Host
#-----------------------------------------------------------------------
    '''

    host_polling_data = (
        #----------------------------------------------------------------------------------------------------------------
        # OP     Reg                                  ValueToWrite           size       Test Expected Data   Description
        #----------------------------------------------------------------------------------------------------------------
        ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR,               "0",            8,      "8000000000000000", "Reading Host Doorbell for Interrupt"],
        )

    '''
#---------------------
# Host side test data
#---------------------
    '''

    host_status_test_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg           ValueToWrite        size      Test Expected Data                            Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.REG_MBOX4,        "0",             8,     "0000000000F0D103", "Reading Host MBOX4 data to Validate"],
    )

    # HOST->SBE data set execution - Less length setup
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )

    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_status_test_data )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 100000000 )
    print("\n  Execute SBE Test PSUSyncTargetState - positive testcase \n")
    syncTarget(0x34311312)

if __name__ == "__main__":
    try:
        main()
    except:
        print("\nTest Suite completed with error(s)")
        testUtil.collectFFDC()
        raise()

    print("\nTest Suite completed with no errors")
