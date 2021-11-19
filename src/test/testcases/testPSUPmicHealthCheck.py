# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUPmicHealthCheck.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2021
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
sys.path.append("targets/p10_standalone/sbeTest")
import testMemUtil
import testPSUUtil
import testRegistry as reg
import testUtil

def read_pba(address, size):
    pba_data = testMemUtil.getmem(address, size, 0x0)
    return pba_data

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
    ["write", reg.REG_MBOX0, "0000010000F0D70C",         8,     "None",                 "Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0004000000000000",         8,     "None",                 "Writing to MBOX1 address"],
    ["write", reg.REG_MBOX2, "00000000E046E600",         8,     "None",                 "Writing data address to MBOX2"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000",    8, "None",         "Update SBE Doorbell register to interrupt SBE"],
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
    ["read", reg.REG_MBOX4,                            "0",              8,     "0000000000F0D70C", "Reading Host MBOX4 data to Validate"],
    ["read", reg.REG_MBOX5,                            "0",              8,     "00000000000000E0", "Reading Host MBOX5 data to Validate"],
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
    ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR,           "0",              8,     "8000000000000000", "Reading Host Doorbell for Interrupt"],
    )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 10000000 );
    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation
    '''
    Test Case 1
    '''
    # HOST->SBE data set execution
    print("HOST->SBE data set execution")
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )
    print("Poll on Host side for INTR  ...\n")
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    print("SBE->HOST data set execution")
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )
    expected_data = [0x02, 0x04]
    expected_data.extend([0x00 for i in range(0xDE)])
    #compare data that we recieved
    readData0 = read_pba(0xE046E600, 128*2)[0:0xE0]
    print(readData0)
    if(readData0 == expected_data):
        print("Passed")
    else:
        print("Failed")
        print(expected_data)
        print(readData0)

if __name__ == "__main__":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )

