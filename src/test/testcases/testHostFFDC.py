# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testHostFFDC.py $
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testPSUUtil
import testRegistry as reg
import testUtil
import testMemUtil

i_fifoType = 0
#-------------------------------
# This  is a Test Expected Data
#-------------------------------
'''
This data are the values or strings that needs to be validated for the test.
'''
'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data -
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_data = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Set FFDC chip-op
    ["write", reg.REG_MBOX0, "0000010000F0D704",     8,     "None",         "Writing to MBOX0 address"],
    #                        FFDC Size, Pass CMD Size
    ["write", reg.REG_MBOX1, "0000200000000100",     8,     "None",         "Writing to MBOX1 address"],
    #                           FFDC Addr
    ["write", reg.REG_MBOX2, "0000000008000000",     8,     "None",             "Writing to MBOX2 address"],
    #                           Pass Cmd Addr
    ["write", reg.REG_MBOX3, "0000000008000000",     8,     "None",             "Writing to MBOX3 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000",    8,     "None",         "Update SBE Doorbell register to interrupt SBE"],
    )
'''
#---------------------
# Host side test data - SUCCESS
#---------------------
'''
host_test_data_success = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data        Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.REG_MBOX4,         "0",         8,     "0000000000F0D704", "Reading Host MBOX4 data to Validate"],
    )

'''
#-----------------------------------------------------------------------
# Do not modify - Used to simulate interrupt on Ringing Doorbell on Host
#-----------------------------------------------------------------------
'''
host_polling_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data           Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR,       "0",        8, "8000000000000000", "Reading Host Doorbell for Interrupt Bit0"],
    )

host_pass_through_polling_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data           Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR,       "0",        8, "0800000000000000", "Reading Host Doorbell for Interrupt Bit4"],
    )

'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data - Target - Pervasive(Core), Chiplet Id - 32, Ring ID - ec_func(224), mode - 0x0020(RING_MODE_HEADER_CHECK)
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_invalid_ring = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D301", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0002002000FF0020", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
host_test_data_failure = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data        Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"00FE000A00F0D301", "Reading Host MBOX4 data to Validate"],
    )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 10000000 );

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print("\n  Execute SBE Test - Set FFDC Address\n")

    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )

    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )

    testUtil.runCycles( 10000000 )

    # Invalid ring - 248
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_invalid_ring )

    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_failure )

    #dump ffdc to a file
    readData = testMemUtil.getmem(0x08000000, 0x2000, 0x02)
    hostDumpFile = open("hostDumpFFDC.bin", 'wb')
    hostDumpFile.write(bytearray(readData))
    hostDumpFile.close()

    # extract HWP ffdc
    readData = testUtil.extractHWPFFDC(i_fifoType, True, readData)

if __name__ == "__main__":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )
