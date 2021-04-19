#!/usr/bin/env python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPsuMemConfig.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2021
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
import testPSUUtil
import testRegistry as reg
import testUtil

pib_space = simics.SIM_get_object('system_cmp0.phys_mem')
def write_pibmem_direct(offset, data):
    """ Write PIBMEM Data """
    global golden_data
    iface = simics.SIM_get_interface(pib_space, "memory_space")
    iface.write(None, offset, data,0)

def write_mem_config_to_pibmem( ):
    PMIC_DEV_ADDR = [0xCE, 0x9E, 0xB4, 0xBE]
    GI2C_DEV_ADDR = [0x20, 0x2E, 0x70, 0x7E]
    DIMM_I2C_PORT = [
            0x00, 0x0A, 0x0B, 0x09, 0x0C, 0x0D, 0x08, 0x07,
            0x0F, 0x02, 0x01, 0x0E, 0x03, 0x06, 0x05, 0x04
    ]

    data = [0x00, 0x01,                     # sbe_psu_config_version
            0x00, 0x03]                     # sbe_psu_config_types_supported

    data.extend([0x00, 0x40])               # pmic_config_max_number
    for i in range(64):
        i2c_data = [
            DIMM_I2C_PORT[i % 16] + 1,      # +1 to make a difference with default value
            0x06,                           # engine
            PMIC_DEV_ADDR[i % 4] + 1,
            (0 if (i % 12 == 1) else 1),    # mark some target non-functional
            (0 if (i % 12 == 5) else 1),    # mark some target non-present
            0, 0, 0                         # reserved
        ]
        data.extend(i2c_data)

    data.extend([0x00, 0x40])               # generic_config_max_number
    for i in range(64):
        i2c_data = [
            DIMM_I2C_PORT[i % 16] + 1,      # +1 to make a difference with default value
            0x06,                           # engine
            GI2C_DEV_ADDR[i % 4] + 1,
            (0 if (i % 12 == 4) else 1),    # mark some target non-functional
            (0 if (i % 12 == 9) else 1),    # mark some target non-present
            0, 0, 0                         # reserved
        ]
        data.extend(i2c_data)

    data.extend([0x00, 0x10])               # ocmb_config_max_number
    for i in range(16):
        i2c_data = [
            DIMM_I2C_PORT[i] + 1,           # +1 to make a difference with default value
            0x06,                           # engine
            0x41,                           # default addres + 1
            (0 if (i == 4) else 1),         # mark some target non-functional
            (0 if (i == 9) else 1),         # mark some target non-present
            0, 0, 0                         # reserved
        ]
        data.extend(i2c_data)

    i = 0
    while (i < len(data)):
        data_tuple = tuple(data[i:i+8])
        write_pibmem_direct(0xE046E600 + i, data_tuple)
        i += 8


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
    ["write", reg.REG_MBOX0, "0000000000F0D70B", 	 8, 	"None", 		"Writing to MBOX0 address"],
    #                           FFDC Addr
    ["write", reg.REG_MBOX2, "00000000E046E600", 	 8, 	"None", 			"Writing data address to MBOX2"],
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
    ["read", reg.REG_MBOX4, 		"0", 	 	 8, 	"0000000000F0D70B", "Reading Host MBOX4 data to Validate"],
    )

#-------------------------
# Main Function
#-------------------------
def main():
    # Run Simics initially
    testUtil.runCycles( 10000000 );
    
    write_mem_config_to_pibmem()

    # Intialize the class obj instances
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print("\n  Execute SBE Test - Update OCMB Target\n")

    '''
    Test Case 1
    '''
    # HOST->SBE data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, sbe_test_data )

    testUtil.runCycles( 10000000 );

    #SBE->HOST data set execution
    #regObj.ExecuteTestOp( testPSUUtil.simSbeObj, host_test_data_success )

if __name__ == "__main__":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )
