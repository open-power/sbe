#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testExecutorStopTimer.py $
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
# SBE side test data - 512000us timer
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_startTimer = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010100F0D401", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "000000000007D000", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data - Stop Timer.
#------------------------------------------------------------------------------------------------------------------------------
'''
sbe_test_StopTimer = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010200F0D401", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )
'''
#---------------------
# Host side test data - SUCCESS
#---------------------
'''
host_test_Timer_Cmd_success = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data        Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"0000000000F0D401", "Reading Host MBOX4 data to Validate"],
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

timer_polling_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data           Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR, 		"0", 		8, 	"0002000000000000", "Reading Host Doorbell for Interrupt"],
    )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 10000000 );

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print "\n  Execute SBE Test set1  [ PutCntrlTimer ] ...\n"

    '''
    Test Case 1
    '''
    print "\n  Test Start timer\n"
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_startTimer )

    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_Timer_Cmd_success )

    print "\n  Test Stop timer\n"
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_StopTimer )

    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    isTimerFired = True;
    #SBE->HOST data set execution
    # As we have stopped the timer, timer should not fire
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_Timer_Cmd_success )
    try:
        #Poll on HOST DoorBell Register for interrupt
        print "\n  Poll on Host side for Timer INTR  ...\n"
        regObj.pollingOn( testPSUUtil.simSbeObj, timer_polling_data, 20 )
    except:
        isTimerFired = False

    if isTimerFired:
        print "\n  Problem. Timer not cancelled\n"
        raise Exception('Timer Not cancelled ');

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


