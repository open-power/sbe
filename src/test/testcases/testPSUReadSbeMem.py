# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUReadSbeMem.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2023
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
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testPSUUtil
import testRegistry as reg
import testUtil
import testMemUtil

#-------------------------------
# This  is a Test Expected Data
#-------------------------------
def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return hex_string

def readSeeprom(offset, size, destAddr, primStatus, secStatus):
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
        ["write", reg.REG_MBOX0, "0000010000F0D703", 	 8, 	"None", 		"Writing to MBOX0 address"],
        #                        seeprom offset,  Size
        ["write", reg.REG_MBOX1, getdoubleword((offset<<32)+size), 	 8, 	"None", 		"Writing to MBOX1 address"],
        #                           response Addr
        ["write", reg.REG_MBOX2, getdoubleword(destAddr), 	 8, 	"None", 			"Writing to MBOX2 address"],
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
        ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	getdoubleword((primStatus<<48)+(secStatus<<32)+0xF0D703), "Reading Host MBOX4 data to Validate"],
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
    # Run Simics initially
    testUtil.runCycles( 10000000 );

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )

    print ("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5 )

    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 10000000 );

    print ("\n  Execute SBE Test - Read SBE Mem\n")

    '''
    Test Case 1
    '''
    readSeeprom(0, 128, 0x08000000, 0, 0)
    print ("SUCCESS: read seeprom valid")

    # Read data from cache and verify its contents
    # seeprom header
    seepprmHdr = 'XIP SEPM'
    #read from cache
    readData = testMemUtil.getmem(0x08000000, 0x80, 0x02)

    for byte in range(len(seepprmHdr)):
        if( ord(seepprmHdr[byte]) != readData[byte ]):
             print ("Data mismtach at: ",  byte )
             print (" expected: ", ord(seepprmHdr[byte]))
             print (" Actual: ", readData[byte])
             raise Exception('data mistmach');

    '''
    Test Case 2
    '''
    readSeeprom(0x38CA0, 0x180, 0x8973780, 0, 0)
    print ("SUCCESS: read seeprom HB testcase")

    '''
    Test Case 3
    '''
    readSeeprom(0x0, 0x40, 0x08000000, 0x03, 0x19)
    print ("SUCCESS: read seeprom size not aligned")

    '''
    Test Case 4
    '''
    readSeeprom(0x3fe80, 0x180, 0x08000000, 0x03, 0x19)
    print ("SUCCESS: read seeprom size exceeded")

    '''
    Test Case 5
    '''
    readSeeprom(0x7, 0x40, 0x08000000, 0x03, 0x19)
    print ("SUCCESS: read seeprom offset not aligned")

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


