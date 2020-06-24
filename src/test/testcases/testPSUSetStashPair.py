# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUSetStashPair.py $
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
sbe_test_data1 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000001", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "AAAAAAAAAAAAAAAA", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data2 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000002", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "BBBBBBBBBBBBBBBB", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data3 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000003", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "CCCCCCCCCCCCCCCC", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data4 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000004", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "DDDDDDDDDDDDDDDD", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data5 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000005", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "EEEEEEEEEEEEEEEE", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data6 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000006", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "FFFFFFFFFFFFFFFF", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data7 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000007", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "ABABABABABABABAB", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
'''
sbe_test_data8 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #                        Stash CMD
    ["write", reg.REG_MBOX0, "0000010000F0D707", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        Stash CMD key
    ["write", reg.REG_MBOX1, "0000000000000008", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                        Stash Addr
    ["write", reg.REG_MBOX2, "CDCDCDCDCDCDCDCD", 	 8, 	"None", 			"Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
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
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"0000000000F0D707", "Reading Host MBOX4 data to Validate"],
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
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR, 		"0", 		8, 	"8000000000000000", "Reading Host Doorbell for Interrupt"],
    )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 100000000 );
    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data1 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data2 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data3 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data4 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data5 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data6 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data7 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

    print("\n  Execute SBE Test \n")
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data8 )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    testUtil.runCycles( 10000000 );

if __name__ == "__main__":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )


