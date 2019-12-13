#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUUserUtil.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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

import testPSUUtil

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
    return testPSUUtil.SUCCESS 
