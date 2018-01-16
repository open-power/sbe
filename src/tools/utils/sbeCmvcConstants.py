# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbeCmvcConstants.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2018
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
###########################################################
#    @file    sbeCmvcConstants.py
#    @author  George Keishing <gkeishin@in.ibm.com>
#             Sangeetha TS    <sangeet2@in.ibm.com>
#    @brief   Constants support tool operation
#
#    Created on March 03, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     03/03/16     Initial create
###########################################################
'''


'''
Generic Failure RC code
'''
SUCCESS_EXIT   = 0
SHELL_EXIT     = 1 # Shell Exited with error
ERROR_EXIT     = 2
HELP_EXIT      = 3
SUCCESS_DEV_EXIT   = 4
ERROR_SYS_EXIT = 100


# This funcs specific error codes
ERROR_SETTING         = 10 # No CMV ENV set in the .bashrc or a generic error for Sandbox ENV not set
ERROR_CMVC_LOGIN      = 11 # No Cmvc login access session not established
ERROR_CMVC_CHECKOUT   = 12 # Failed file checkout
ERROR_CMVC_CHECKIN    = 13 # Failed file check in
ERROR_CMVC_FIX_RECORD = 14 # Failed fixing complete record
ERROR_CMVC_FILE_COPY  = 15 # Copying file failed
ERROR_TRACK_STATE     = 16 # Track not in required state
ERROR_CI_TRIGGER      = 17 # CI not started successfully

SUCCESS_CMVC_CHECKOUT   = 50
SUCCESS_CMVC_CHECKIN    = 51
SUCCESS_CMVC_FIX_RECORD = 52
SUCCESS_TRACK_STATE     = 53
SUCCESS_CI_TRIGGER      = 54

# This is a default files list to Check-in
CMVC_FILE_LIST  ="sbe_sp_intf.H,simics.tar,sbe_seeprom.bin,sbe_seeprom_DD2.bin,releaseNotes.html"
CMVC_FILE_UNDO_LIST  ="src/sbei/sbfw/sbe_sp_intf.H src/sbei/sbfw/simics.tar src/sbei/sbfw/img/sbe_seeprom.bin src/sbefw/sbe_seeprom_DD2.bin src/sbei/sbfw/releaseNotes.html"

CMVC_DIR_CREATE ="sandbox_"


# Hashing specfic
SUCCESS_HASH_CHECK = 100
ERROR_HASH_CHECK   = 101


# For Development
ERROR_SANDBOX_EXIST = 200 # Sandbox doesnt exist
ERROR_FILE_INPUT    = 201 # User have entered no file
ERROR_BUILD_FAILED  = 202 # Compilation failed
ERROR_HOOKING_FILE  = 203 # Error while building shell hooks
FILE_LOOKUP_LIST  ="src/sbei/sbfw/sbe_sp_intf.H,src/sbei/sbfw/simics.tar,src/sbei/sbfw/img/sbe_seeprom.bin,src/sbei/sbfw/img/sbe_seeprom_DD2.bin,src/sbei/sbfw/releaseNotes.html"
