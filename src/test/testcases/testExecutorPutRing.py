#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testExecutorPutRing.py $
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
# SBE side test data - Target - Pervasive(Core), Chiplet Id - 32, Ring ID - ec_func(224), mode - 0x0020(RING_MODE_HEADER_CHECK)
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_data1 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D301", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0002002000E00020", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data - Target - Pervasive(Perv), Chiplet Id - 1, Ring ID - perv_fure(00), mode - 0x0020(RING_MODE_HEADER_CHECK)
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_data2 = (
    #--------------------------------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #--------------------------------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D301", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0002000100000020", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
#---------------------
# SBE side test data - Target - PROC CHIP, Chiplet Id - x, Ring ID - ob0_fure(118), mode - 0x0020(RING_MODE_HEADER_CHECK)
#---------------------
'''
sbe_test_data3 = (
    #--------------------------------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #--------------------------------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D301", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0000000600760020", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data - Target - EX, Chiplet Id - 32, Ring ID - ex_l3_fure(176), mode - 0x0020(RING_MODE_HEADER_CHECK)
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_data4 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D301", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0001002000B00020", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data - Target - Invalid target 0x10, Chiplet Id - 32, Ring ID - ex_l3_refr_repr(248), mode - 0x0020(RING_MODE_HEADER_CHECK)
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_data5 = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D301", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0010002000F80020", 	 8, 	"None", 			"Writing to MBOX1 address"],
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
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"0000000000F0D301", "Reading Host MBOX4 data to Validate"],
    )
'''
#---------------------
# Host side test data - FAILURE
#---------------------
'''
host_test_data_failure5 = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data        Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"0002000400F0D301", "Reading Host MBOX4 data to Validate"],
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
    testUtil.runCycles( 10000000 );

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print "\n  Execute SBE Test set1  [ Put Ring ] ...\n"

    '''
    Test Case 1
    '''
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data1 )

    print "\n  Poll on Host side for INTR  ...\n"
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )

#   Commenting out test cases for perv and proc chiplets, as there is no
#   way to stop cloks for these chiplets from the test framework
#    print "\n  Execute SBE Test set2  [ Put Ring ] ...\n"
#    '''
#    Test Case 2
#    '''
#    # HOST->SBE data set execution
#    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data2 )
#
#    print "\n  Poll on Host side for INTR  ...\n"
#    #Poll on HOST DoorBell Register for interrupt
#    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
#
#    #SBE->HOST data set execution
#    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
#
#    print "\n  Execute SBE Test set3 [ Put Ring ] ...\n"
#    '''
#    Test Case 3
#    '''
#    # HOST->SBE data set execution
#    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data3 )
#
#    print "\n  Poll on Host side for INTR  ...\n"
#    #Poll on HOST DoorBell Register for interrupt
#    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
#
#    #SBE->HOST data set execution
#    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
#
    print "\n  Execute SBE Test set4  [ Put Ring ] ...\n"
    '''
    Test Case 4
    '''
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data4 )

    print "\n  Poll on Host side for INTR  ...\n"
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )

    print "\n  Execute SBE Test set5  [ Put Ring ] ...\n"
    '''
    Test Case 5
    '''
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data5 )

    print "\n  Poll on Host side for INTR  ...\n"
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_failure5 )

if __name__ == "__main__":
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
            print ( "\nTest Suite completed with error(s)" )
            #sys.exit(1)
        else:
            print ( "\nTest Suite completed with no errors" )
        #sys.exit(0);


