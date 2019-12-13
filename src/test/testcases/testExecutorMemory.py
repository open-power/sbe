#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testExecutorMemory.py $
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
'''
#############################################################
#    @file    testExecutor.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#    @brief   Framework to test Host SBE interface on simics
#
#    Created on March 29, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     29/03/16     Initial create
#############################################################
'''

import testClass as testObj
import testRegistry as reg
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil

#-------------------------------
# This  is a Test Expected Data
#-------------------------------
'''
This data are the values or strings that needs to be validated for the test.
'''
SBE_TEST_EXPECT_DEFAULT  = "None"

HOST_TEST_EXPECT_MAGIC = "00000000DEADBEEF"

sbe_test_data = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg              Mem Length (bytes)    size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    #["memRead",  reg.MEM_ADDR,     0xA00000,            8,    HOST_TEST_EXPECT_MAGIC,   "Reading data from the address"],
    ["memRead",  reg.MEM_ADDR,     0x50,            8,    HOST_TEST_EXPECT_MAGIC,   "Reading data from the address"],
    )

#-------------------------
# Main Function
#-------------------------
def main():

    # Intialize the class obj instances
    print "\n  Initializing Registry instances ...."
    regObj = testObj.registry() # Registry obj def for operation

    print "\n  Execute SBE Test set  [ Indirect Commands ] ...\n"
                                   # Sim obj Target    Test set 
    rc_test = regObj.ExecuteTestOp(testObj.simMemObj,sbe_test_data)
    if rc_test != testObj.SUCCESS:
        print "  SBE Test data set .. [ FAILED ] .."
    else:
        print "  SBE Test data set .. [ SUCCESS ] "
    print "\n"

if __name__=="__main__":
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

