# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPsuHostPassThrough.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017
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
sys.path.append("targets/p9_nimbus/sbeTest" )
import testPSUUtil
import testRegistry as reg
import testUtil
import testMemUtil

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
    #                        FFDC Size, Pass CMD Size
    ["write", reg.REG_MBOX0, "0000010000F0D704", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                        FFDC Size, Pass CMD Size
    ["write", reg.REG_MBOX1, "0000010000000100", 	 8, 	"None", 		"Writing to MBOX1 address"],
    #                           FFDC Addr
    ["write", reg.REG_MBOX2, "1234567898765432", 	 8, 	"None", 			"Writing to MBOX2 address"],
    #                           Pass Cmd Addr
    ["write", reg.REG_MBOX3, "0000000008000000", 	 8, 	"None", 			"Writing to MBOX3 address"],
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
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"0000000000F0D704", "Reading Host MBOX4 data to Validate"],
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
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR, 		"0", 		8, "8000000000000000", "Reading Host Doorbell for Interrupt Bit0"],
    )

host_pass_through_polling_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg                                 ValueToWrite        size    Test Expected Data           Description
    #----------------------------------------------------------------------------------------------------------------
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR, 		"0", 		8, "0800000000000000", "Reading Host Doorbell for Interrupt Bit4"],
    )


#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 10000000 );

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print("\n  Execute SBE Test - Set Pass through Address\n")

    '''
    Test Case 1
    '''
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )

    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )

    testUtil.runCycles( 10000000 )
    # Put mem PBA - Passthrough
    data = os.urandom(128*2)
    data = [ord(c) for c in data]
    # WO FMODE WO LCO PASSTHROUGH 
    testMemUtil.putmem(0x00000000, data, 0x102)
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_pass_through_polling_data, 5 )
    
    readData = testMemUtil.getmem(0x00000000, 128*2, 0x102)
    if(data == readData):
        print ("Success - Write-Read PBA - With Pass through Mode")
    else:
        print(data)
        print(readData)
        raise Exception('data mistmach')
    # Send an invalid size, it should fail
    testMemUtil.getmem_failure(0x00000000, 128*4, 0x102, 0x0002000a)
    print ("Success - Wrote an invalid size which failed")

if __name__ == "__main__":
    main()
    if err:
    	print ( "\nTest Suite completed with error(s)" )
    	#sys.exit(1)
    else:
    	print ( "\nTest Suite completed with no errors" )
	#sys.exit(0);


