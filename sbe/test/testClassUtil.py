#!/usr/bin/python
'''
#############################################################
#    @file    testClassUtil.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#    @brief   Framework utility fucntions for Host SBE 
#             interface on simics
#
#    Created on March 29, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     29/03/16     Initial create
#############################################################
'''

import testClass as testClass

'''
Add your personalize functions here for execution but ensure it returns
either SUCCESS or FAILURE as an end result for generalization purpose.
'''

##########################################################################
# Function : classUtilFuncSample
#
# @param   i_paramArray   : user supplied input array parameters
#
# @brief   Function to do a task and returns SUCCCES or FAILURE
#
##########################################################################
def classUtilFuncSample(i_paramArray):
    for input in i_paramArray:
        print "  classUtilFuncSample : parm: ",input
    return testClass.SUCCESS 
