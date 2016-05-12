#!/usr/bin/python
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
    regObj = testObj.registry() # Registry obj def for operation

    print "\n  Execute SBE Test set  [ PSU ] ...\n"
                                   # Sim obj Target    Test set 
    rc_test = regObj.ExecuteTestOp(testObj.simSbeObj,sbe_test_data)
    if rc_test != testObj.SUCCESS:
        print "  SBE Test data set .. [ Failed ] .."
    else:
        print "  SBE Test data set .. [ OK ] "
        print "\n  Poll on Host side for INTR  ...\n"
                                   # Sim obj Target    Test set     Max timedout
        rc_intr = regObj.pollingOn(testObj.simSbeObj,host_test_data,20)
        if rc_intr == testObj.SUCCESS:
            print "  Interrupt Event Recieved .. Success !!"
        else:
            print "  Interrupt not Recieved.. Exiting .."

    print "\n"

if __name__=="__main__":
    main()

