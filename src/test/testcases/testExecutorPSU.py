#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testExecutorPSU.py $
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

import testPSUUtil
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

HOST_TEST_EXPECT_DEFAULT = "None"
HOST_TEST_EXPECT_MBOX04  = "0000000000F0D101"

'''
The test data is designed to accomodate as many as new entries a test needs
and can also increase the field in it to add new action associated with it.
'''
#---------------------
# SBE side test data
#---------------------
'''
Every test data entry itself represent an action associated with it's data.
The data is validated as it executes.

The Test Expected Data if "None" signifies that this test entry is not to be
validated else it would validated against the expected value in the field.
On success returns macro SUCCESS else FAILURE

Refer Documentation for the data used here directly.
'''

sbe_test_data = (
    #-----------------------------------------------------------------------------------------------------
    #   OP      Reg                           Value           size    Test Expected Data       Description
    #-----------------------------------------------------------------------------------------------------
    ["write",  reg.REG_MBOX0,                  "0000030100F0D101", 8, SBE_TEST_EXPECT_DEFAULT,    "Writing to MBOX0 address"],
    ["write",  reg.REG_MBOX1,                  "0000000000001000", 8, SBE_TEST_EXPECT_DEFAULT,    "Writing to MBOX1 address"],
    ["write",  reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 8, SBE_TEST_EXPECT_DEFAULT,    "Update SBE Doorbell register to interrupt SBE"],
    )

#---------------------
# Host side test data
#---------------------
'''
This Host data indicates that this will validate the SBE test set execution
if the overall test is a success or failure.

It can have as many entries which are needed to be validated.
'''
host_test_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     Reg          Value           size    Test Expected Data           Description
    #----------------------------------------------------------------------------------------------------------------
    ["read",  reg.REG_MBOX4, "0000000000000000",  8,   HOST_TEST_EXPECT_MBOX04,     "Reading Host MBOX4 data to Validate"],
    )

'''
User can define a function which does some task and returns SUCCESS or FAILURE.
one can simply call that function like any OP in the test data and still work.

Define those function in testClassUtil.py context for this to work.
'''

SAMPLE_TEST_EXPECT_FUNC = "None"
PARM_DATA = [1, 2, 3, 4] # sample 4 input paramters
sample_test_data = (
    #----------------------------------------------------------------------------------------------------------------
    #   OP     function Name          Parameters  NA     Test Expected Data           Description
    #----------------------------------------------------------------------------------------------------------------
    ["func",  "classUtilFuncSample",  PARM_DATA,   0,   SAMPLE_TEST_EXPECT_FUNC,     "Load func and do task"],
    )

#-------------------------
# Main Function
#-------------------------
def main():

    # Intialize the class obj instances
    print "\n  Initializing Registry instances ...."
    regObj = testPSUUtil.registry() # Registry obj def for operation

    print "\n  Execute SBE Test set  [ PSU ] ...\n"
                                   # Sim obj Target    Test set     Raise Exception
    rc_test = regObj.ExecuteTestOp(testPSUUtil.simSbeObj,sbe_test_data, True)
    if rc_test != testPSUUtil.SUCCESS:
        print "  SBE Test data set .. [ Failed ] .."
    else:
        print "  SBE Test data set .. [ OK ] "
        print "\n  Poll on Host side for INTR  ...\n"
                                   # Sim obj Target    Test set     Max timedout
        rc_intr = regObj.pollingOn(testPSUUtil.simSbeObj,host_test_data,20)
        if rc_intr == testPSUUtil.SUCCESS:
            print "  Interrupt Event Recieved .. Success !!"
        else:
            print "  Interrupt not Recieved.. Exiting .."

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

