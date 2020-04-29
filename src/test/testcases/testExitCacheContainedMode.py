#!/usr/bin/env python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testExitCacheContainedMode.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2019,2020
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
import os
import struct
sys.path.append("targets/p10_standalone/sbeTest" )
import testPSUUtil
import testRegistry as reg
import testUtil
import testMemUtil

pib_space = simics.SIM_get_object('system_cmp0.phys_mem')
def write_pibmem_direct(offset, data):
   """ Write PIBMEM Data """
   global golden_data
   iface = simics.SIM_get_interface(pib_space, "memory_space")
   iface.write(None, offset, data,0)

#Update the XSCOM address and data into the memory
def write_xscom_address_data( ):
    data = ( 0x00,0x06,0x03,0xfc,0x60,0x08,0x60,0x50 )
    write_pibmem_direct(0xF046e600,data);
    data = (0x80,0x00,0x00,0x00,0x0f,0x41,0xfd,0xfc)
    write_pibmem_direct(0xF046e608,data);
    data = ( 0x00,0x06,0x03,0xfc,0x60,0x08,0x60,0x58 )
    write_pibmem_direct(0xF046e610 ,data);
    data = ( 0x80,0x00,0x00,0x00,0x0f,0x45,0xfd,0xfc )
    write_pibmem_direct(0xF046e618 , data);
    data = ( 0x00,0x06,0x03,0xfc,0x60,0x08,0x60,0x88 )
    write_pibmem_direct(0xF046e620 ,data);
    data = ( 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00)
    write_pibmem_direct(0xF046e628 , data);
    data = ( 0x00,0x06,0x03,0xfc,0x60,0x08,0x60,0x98 )
    write_pibmem_direct(0xF046e630 ,data);
    data = ( 0x00,0x00,0x00,0x00, 0x00,0x00,0x07,0x80)
    write_pibmem_direct(0xF046e638 , data);
    data = (0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x50)
    write_pibmem_direct(0xF046e640,data);
    data = (0x82,0x00,0x1f,0xfc,0x00,0x00,0x00,0x00 )
    write_pibmem_direct(0xF046e648,data);
    data = (0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x00 )
    write_pibmem_direct(0xF046e650,data);
    data = (0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 )
    write_pibmem_direct(0xF046e658,data);
    data = (0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x18)
    write_pibmem_direct(0xF046e660,data);
    data = ( 0x4c,0x60,0x00,0x00,0x00,0x00,0x00,0x00)
    write_pibmem_direct(0xF046e668,data);
    data = (0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x30 )
    write_pibmem_direct(0xF046e670,data);
    data = (0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 )
    write_pibmem_direct(0xF046e678,data);
    data = (0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x38 )
    write_pibmem_direct(0xF046e680,data);
    data = (0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00 )
    write_pibmem_direct(0xF046e688,data);
    data = (0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x88 )
    write_pibmem_direct(0xF046e690,data);
    data = (0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00 )
    write_pibmem_direct(0xF046e698,data);
    data = ( 0x00,0x06,0x03,0xfc,0x18,0x08,0x40,0x80)
    write_pibmem_direct(0xF046e6A0,data);
    data = ( 0xa0,0x03,0x20,0x00,0x00,0xf0,0x7f,0xff)
    write_pibmem_direct(0xF046e6A8,data);

'''
#------------------------------------------------------------------------------------------------------------------------------
# SBE side test data - Address 0x846E600 and 11 entries
#------------------------------------------------------------------------------------------------------------------------------
'''
sbeExitCacheContainedMode = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                               ValueToWrite         size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write", reg.REG_MBOX0, "0000010000F0D102", 	 8, 	"None", 		"Writing to MBOX0 address"],
    ["write", reg.REG_MBOX1, "0000000B0000001F", 	 8, 	"None", 			"Writing to MBOX1 address"],
    ["write", reg.REG_MBOX2, "00000000F046E600",   8,  "None",    "Writing to MBOX2 address"],
    ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 	 8, 	"None", 		"Update SBE Doorbell register to interrupt SBE"],
    )

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
    
    #Update the XSCOM address and data into the memory   
    write_xscom_address_data();

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    '''
    Test Case 1
    '''
    print "\n  Trigger Exit Cache Contained ChipOp\n"
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbeExitCacheContainedMode )

    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data,1000 )


if __name__ == "__main__":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )


