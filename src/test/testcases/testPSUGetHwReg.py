# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUGetHwReg.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2020
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


# /**
#  * @brief structure for HWReg Chip-op (0xA503)
#  */
# typedef struct
# {
#     uint32_t targetType:16;
#     uint32_t reserved:8;
#     uint32_t targetInstance:8;
#     uint32_t hiAddr;
#     uint32_t lowAddr;
# }sbeGetHWRegReqMsg_t;

# target type consts
EQ_TARGET_TYPE = 0x02
OCMB_TARGET_TYPE = 0x04

# status consts
GOOD_STATUS = 0x0
INVALID_TYPE = 0x00020004
FAPI2_FAIL = 0x00FE000A


def getHwReg(targetType, targetInstance, hiAddr, lowAddr, exp_status, exp_value):
    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation
    '''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data -
#------------------------------------------------------------------------------------------------------------------------------
    '''
    mbox1data = getdoubleword((targetType << 48) + (targetInstance << 32) + hiAddr)
    mbox2data = getdoubleword((lowAddr << 32))

    sbe_test_data = (
        #-----------------------------------------------------------------------------------------------------
        #   OP      Reg                            ValueToWrite         size    Test Expected Data       Description
        #-----------------------------------------------------------------------------------------------------
        #                                          Get HW Reg Request
        ["write", reg.REG_MBOX0,                   "0000030000F0D503",   8,     "None",                 "Writing to MBOX0 address"],
        #                                          targetType, targetInstance, hiAddr
        ["write", reg.REG_MBOX1,                   mbox1data,            8,     "None",                 "Writing to MBOX1 address"],
        #                                          lowAddr
        ["write", reg.REG_MBOX2,                   mbox2data,            8,     "None",                 "Writing to MBOX2 address"],
        #
        ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000",    8,     "None",                 "Update SBE Doorbell register to interrupt SBE"],
        )

        #-----------------------------------------------------------------------
        # Do not modify - Used to simulate interrupt on Ringing Doorbell on Host
        #-----------------------------------------------------------------------
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
    ["read", reg.REG_MBOX4,             "0",             8,     getdoubleword((exp_status << 32)+0x00F0D503), "Reading Host MBOX4 data to Validate"],
    )
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg           ValueToWrite        size      Test Expected Data                            Description
    #----------------------------------------------------------------------------------------------------------------
    host_data_test_data = (
    ["read", reg.REG_MBOX5,             "0",             8,     getdoubleword(exp_value),                     "Reading Host MBOX5 data to Validate"],
    )
    # HOST->SBE data set execution - Less length setup
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_status_test_data )

    if exp_status == GOOD_STATUS:
        regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_data_test_data )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 100000000 )


    print("\n  Execute SBE Test PSUGetHwReg - negative testcase - invalid target type\n")
    getHwReg(EQ_TARGET_TYPE, 0, 0x00000000, 0x00000000, INVALID_TYPE, 0)
    print("\n  Execute SBE Test PSUGetHwReg - negative testcase - invalid scom address \n")
    getHwReg(OCMB_TARGET_TYPE, 0, 0xFFFFFFFF, 0xFFFFFFFF, FAPI2_FAIL, 0)

    for i in range(2):
      print("\n  Execute SBE Test PSUGetHwReg - positive testcase - request OCMB{0} address 0x000000000020B08C\n".format(i))
      getHwReg(OCMB_TARGET_TYPE, i, 0x00000000, 0x0020B08C, GOOD_STATUS, 0x0000000000000000)
      print("\n  Execute SBE Test PSUGetHwReg - positive testcase - request OCMB{0} address 0x000000000020B08C\n".format(i))
      getHwReg(OCMB_TARGET_TYPE, i, 0x00000000, 0x00002134, GOOD_STATUS, 0x00000000100600D2)


if __name__ == "__main__":
    try:
        main()
    except:
        print("\nTest Suite completed with error(s)")
        testUtil.collectFFDC()
        raise()

    print("\nTest Suite completed with no errors")
