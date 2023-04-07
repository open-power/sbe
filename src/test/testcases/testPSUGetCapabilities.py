# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUGetCapabilities.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2023
# [+] International Business Machines Corp.
# [+] Marty E. Plummer
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
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testPSUUtil
import testRegistry as reg
import testUtil
import testMemUtil

def getsingleword(dataInInt):
    hex_string = '0'*(8-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BBBB',hex_string.decode('hex')))

def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return hex_string

#-------------------------------
# This  is a Test Expected Data
#-------------------------------
'''
Capabilities structure
'''
capMsg = (getsingleword(0xC000003F) +
          getsingleword(0xC8000000) +
          getsingleword(0xD1000001) +
          getsingleword(0xD1800000) +
          getsingleword(0xD2000000) +
          getsingleword(0xD2800000) +
          getsingleword(0xD3000001) +
          getsingleword(0xD3800000) +
          getsingleword(0xD4000001) +
          getsingleword(0xD4800000) +
          getsingleword(0xD5000000) +
          getsingleword(0xD5800000) +
          getsingleword(0xD6000001) +
          getsingleword(0xD6800000) +
          getsingleword(0xD70000FE) +
          getsingleword(0xD7800000))

def getCapabilities(addr, size, exp_status):
    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation
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
        #                        Get Capabilities CMD
        ["write", reg.REG_MBOX0, "0000010000F0D702", 	 8, 	"None", 		"Writing to MBOX0 address"],
        #                        Size
        ["write", reg.REG_MBOX1, getdoubleword(size), 	 8, 	"None", 		"Writing to MBOX1 address"],
        #                        Addr
        ["write", reg.REG_MBOX2, getdoubleword(addr), 	 8, 	"None", 			"Writing to MBOX2 address"],
        ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
        )

        #-----------------------------------------------------------------------
        # Do not modify - Used to simulate interrupt on Ringing Doorbell on Host
        #-----------------------------------------------------------------------
    host_polling_data = (
        #----------------------------------------------------------------------------------------------------------------
        #   OP     Reg                                 ValueToWrite        size    Test Expected Data           Description
        #----------------------------------------------------------------------------------------------------------------
        ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR, 		"0", 		8, 	"8000000000000000", "Reading Host Doorbell for Interrupt"],
        )
    length = 0
    if exp_status == 0:
        length = 92
    '''
#---------------------
# Host side test data
#---------------------
    '''
    host_test_data = (
        #----------------------------------------------------------------------------------------------------------------
        #   OP     Reg                                 ValueToWrite        size    Test Expected Data        Description
        #----------------------------------------------------------------------------------------------------------------
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	getdoubleword((exp_status << 32)+0x00F0D702), "Reading Host MBOX4 data to Validate"],
    ["read", reg.REG_MBOX5, 		"0", 	 	 8, 	getdoubleword(length), "Reading Host MBOX4 data to Validate"],
    )
    # HOST->SBE data set execution - Less length setup
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 100000000 )

    print("\n  Execute SBE Test - negative testcase - less size\n")
    getCapabilities(0x08000000, 30, 0x00020019)
    print("\n  Execute SBE Test - negative testcase - not multiple of PBA\n")
    getCapabilities(0x08000000, 129, 0x00020019)

    print("\n  Execute SBE Test - positive testcase \n")
    getCapabilities(0x08000000, 128, 0)

    testUtil.runCycles( 100000000 );
    # read capabilities memory
    readData = testMemUtil.getmem(0x08000000, 128, 0x02)
    # ignore first 20 bytes which is a dynamic data
    readData = readData[28:]
    # get only valid data
    readData = readData[:len(capMsg)]
    if(capMsg == readData):
        print ("Success - PSU get capabilities")
    else:
        print(capMsg)
        print(readData)
        raise Exception('data mistmach')

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
