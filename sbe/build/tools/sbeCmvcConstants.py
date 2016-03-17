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
SHELL_EXIT     = 1
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
CMVC_FILE_LIST  ="sbe_sp_intf.H,simics.tar,sbe_pibmem.bin,sbe_seeprom.bin"
CMVC_FILE_UNDO_LIST  ="src/sbei/sbfw/sbe_sp_intf.H src/sbei/sbfw/simics.tar src/sbei/sbfw/img/sbe_pibmem.bin src/sbei/sbfw/img/sbe_seeprom.bin"

#CMVC_FILE_LIST  ="src/sbei/sbfw/sbe_sp_intf.H,src/sbei/sbfw/simics.tar,src/sbei/sbfw/img/sbe_main.bin,src/sbei/sbfw/img/sbe_pibmem.bin,src/sbei/sbfw/img/sbe_seeprom.bin"
CMVC_DIR_CREATE ="sandbox_"


# Hashing specfic
SUCCESS_HASH_CHECK = 100
ERROR_HASH_CHECK   = 101


# For Development 
ERROR_SANDBOX_EXIST = 200 # Sandbox doesnt exist
ERROR_FILE_INPUT    = 201 # User have entered no file
ERROR_BUILD_FAILED  = 202 # Compilation failed
FILE_LOOKUP_LIST  ="src/sbei/sbfw/sbe_sp_intf.H,src/sbei/sbfw/simics.tar,src/sbei/sbfw/img/sbe_pibmem.bin,src/sbei/sbfw/img/sbe_seeprom.bin"
