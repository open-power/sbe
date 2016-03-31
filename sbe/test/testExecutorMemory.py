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
    main()

