#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/CommitSbeImageToCMVC.py $
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
#    @file    CommitSbeImageToCMVC.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#             Sangeetha TS    <sangeet2@in.ibm.com>
#    @brief   Main Module to support CMVC operation
#
#    Created on March 03, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     03/03/16     Initial create
###########################################################
'''

#-------------------------
#  Imports
#-------------------------
import getopt
import os, sys, glob
import shutil

# Libraries/utility funcs and user define const
import sbeCmvcConstants as errorcode
import sbeCmvcUtility as utilcode

#-------------------------
# Main Function
#-------------------------
def main():

    #------------------------------------------
    # Usage tool option
    #------------------------------------------
    def usage():
        print "  ---------------------------------------------------------------------------------------------------"
        print "  :: Command line USAGE options for Uploading FW SBE image to CMVC :: \n"
        print "  CommitSbeImageToCMVC.py -d <cmvc defect/feature id> -r <fips release> -p <SBE repo Path> -i <file1,file2.file3>"

        print " \n"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print "   |       By default user MUST pass CMVC/Release/Path input.                        |"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print " \n  ***** Options Supported  *****"
        print " \t  -d,--defect       = Defect CMVC number"
        print " \t  -f,--feature      = Feature CMVC number"
        print " \t  -r,--release      = FW fips release string EX: fips910"
        print " \t  -p,--path         = Absolute path of the SBE repo"
        print " \t  -i,--input        = [ Optional ] List of image or file to upload"
        print " \t  -b,--bvt          = BVT xml file for CI"
        print " \t  -h,--help         = Help"
        print "  ------------------------------------------------------------------------------------"

    #------------------------------------------
    # Exit from this Main
    #------------------------------------------
    def exit_main(rc):
        if rc == errorcode.HELP_EXIT:
           print "  [ HELP DOCUMENTATION ]\n"
           sys.exit(0)

        if rc:
           print "\n  [ ERROR - MAIN ] Exiting with error code = ", rc
           sys.exit(rc)
        else:
           print "\n  SBE Image Upload to CMVC completed [ OK ] "
           sys.exit(0)

    #------------------------------------------
    # Local var place name holder's
    #------------------------------------------
    defect_num   = "None"
    feature_num  = "None"
    release_name = "None"
    path_name    = "None"
    file_name    = "None"
    bvt          = "None"

    #----------------------------
    # Read command line args
    #----------------------------
    opts, args = getopt.getopt(sys.argv[1:],"d:f:r:p:i:b:h",['defect=', 'feature=', 'release=', 'path=', 'input=', 'bvt=', 'help'])
    for opt, arg in opts:
       if opt in ('-h', '--help'):
           usage()
           exit_main(errorcode.HELP_EXIT)
       elif opt in ('-d', '--defect'):
           defect_num = arg
       elif opt in ('-f', '--feature'):
           feature_num = arg
       elif opt in ('-r', '--release'):
           release_name = arg
       elif opt in ('-p', '--path'):
           path_name = arg
       elif opt in ('-i', '--input'):
           file_name = arg
       elif opt in ('-b', '--bvt'):
           bvt = arg
       else:
           usage()
           exit_main(errorcode.ERROR_EXIT)

    #----------------------------------
    # Preping the data for Image Upload
    #----------------------------------
    print " \n"
    print "  ******************************************************"
    print "  ******  Stagging PPE image Files Upload to CMVC ******"
    print "  ******************************************************"

    #------------------------------------------------------
    # Make sure that it has passed atleast one arg with it
    #------------------------------------------------------
    if len(sys.argv)<2:
        usage()
        exit_main(errorcode.ERROR_SYS_EXIT)

    #-------------------------------------------------------------
    # Check user inputs and display
    #-------------------------------------------------------------
    def input_setting():
        print "  ---------------------------------------------------------------------------------"
        print "           [ Display User Inputs ]"
        print "  ---------------------------------------------------------------------------------"
        # Look elsewhere for the name instead of assigning to it locally.
        # This is now gloabal var
        global g_cmvc_num
        if ( defect_num != "None" ) or ( feature_num != "None" ):
           if not defect_num == "None":
               g_cmvc_num = 'D'+defect_num  #D1234
               print " [ CMVC Defect ]  \t\t#Number\t = %s"%(g_cmvc_num)
           else:
               g_cmvc_num = 'F'+feature_num  # F1234
               print " [ CMVC Feature ]  \t\t#Number\t = %s"%(g_cmvc_num)
        else:
           print " [ CMVC Defect/Feature ]  Neeed CMVC number. This can't be empty "
           exit_main(errorcode.ERROR_EXIT)

        if not release_name == "None":
           print " [ Fips Release Name ]  \t#String\t = %s"%(release_name)
        else:
           print " [ Fips release ]  Neeed fips release string. This can't be empty "
           exit_main(errorcode.ERROR_EXIT)

        if not path_name == "None":
           print " [ Build Repo Path ]  \t\t#String\t = %s"%(path_name)
        else:
           print " [ User Build Repo Path ]  \t\t#String\t = %s"%(path_name)

        # Optional, by default looks up predefined files
        if not file_name == "None":
           print " [ Files for Check-in - User List ]"
           for files in file_name.split(","):
               print " \t\t\t\t#",files
        else:
           print " [ Files for Check-in - Default List ]"
           for files in errorcode.CMVC_FILE_LIST.split(","):
               print " \t\t\t\t# ",files

        print "  ---------------------------------------------------------------------------------"

    #-------------------------------------------------------------
    # CMVC ENV check
    #-------------------------------------------------------------
    def UserCmvcENV():
        # Assumed the CMVC cofig is there in the user bash ENV
        # In .bashrc the CMVX ENV would look like this
        # CMVC specific example
        #-----------------------------------------------------------
        #export CMVC_FAMILY=aix@auscmvc1.austin.ibm.com@2035
        #export CMVC_BECOME=gkeishin
        #export CMVC_AUTH_METHOD=PWD
        #-----------------------------------------------------------

        l_found_cmvc_conf = False
        for key in os.environ.keys():
            if "CMVC" in key:
                print "\t %s : %s" % (key,os.environ[key])
                l_found_cmvc_conf = True

        if l_found_cmvc_conf == False:
           print "\n [ ERROR SETTING ] :  The CMVC specific ENV is not set"
           print " Please add the following CMVC details in ~/.bashrc"
           print " ------------------------------------------------------"
           print " export CMVC_FAMILY=aix@<yourcmvcdomian>@<portnumber>"
           print " export CMVC_BECOME=<your cmvc id>"
           print " export CMVC_AUTH_METHOD=PWD"
           print " ------------------------------------------------------"
           return errorcode.ERROR_SETTING

        return errorcode.SUCCESS_EXIT

    # Testing CMVC login session.. probe
    def CheckCmvcAccess():
        cmd='File -view src/sbei/sbfw/img/sbe_seeprom_DD2.bin -family aix -release fips910 >/dev/null 2>&1'
        rc = os.system(cmd)
        if rc:
            return errorcode.ERROR_CMVC_LOGIN

    #---------------------------------------------
    # Callling the Func defs in order
    #---------------------------------------------

    #------------------------------
    # 1) User input params/ Check ENV
    #------------------------------
    input_setting()

    print "\n [ Checking PPE ENV Pre-req ] "
    # Check if User has passed the path, else get it from ENV
    if path_name == "None":
        # Get the PPE path
        l_ppe_path = utilcode.utilppeSbENV("SBEROOT")
        if l_ppe_path == "None":
            print "  PPE Repo ENV Setting Path  : [ ERROR CODE: %s ] " % l_ppe_path
            exit_main(errorcode.ERROR_SETTING)
        else:
            print "  PPE Repo path Setting  : [ %s ]"% l_ppe_path
            path_name = l_ppe_path

    print "\n [ Checking CMVC user ENV Pre-req ] "
    rc_code = UserCmvcENV()
    if rc_code == errorcode.SUCCESS_EXIT :
        print "  CMVC Setting  : [ OK ] "
    else:
        print "  CMVC Setting  : [ ERORR CODE: %s ]"% rc_code

    #------------------------------
    # 2) Check the CMVC login access
    #------------------------------
    print "\n [ Checking CMVC user Login Session access ] "
    rc_cmvc = CheckCmvcAccess()
    if rc_cmvc == errorcode.ERROR_CMVC_LOGIN:
        print "  CMVC Login Access  : [ ERORR CODE: %s ]"% rc_cmvc
        print "\t  No cmvc login was found in this session."
        print "\t  Issue the cmvclog command to establish a login and re-run."
        print "\t  Command : cmvclog -in <login user id>"
        exit_main(rc_cmvc)
    else:
        print "  CMVC Session Login  : [ OK ] "

    # Call API/Utility funcs def here
    #------------------------------
    # 3) Check track status
    #------------------------------
    print "\n [ Checking CMVC track state ] "
    l_trackFix = utilcode.utilCheckTrackState(g_cmvc_num,release_name)
    if l_trackFix == errorcode.SUCCESS_TRACK_STATE :
        print "  Track in fix state. Suitable to continue."
    else :
        print "  Track not in fix state. Aborting activity."
        return errorcode.ERROR_TRACK_STATE

    #------------------------------
    # 4) Checkout the code
    #------------------------------
    print "\n  [ Creating Sandbox ]"
    origDir = os.getcwd()
    utilcode.utilCmvcChangeDir(g_cmvc_num)
    sbDir = os.getcwd()

    print "\n [ Checkout Files from CMVC ] "
    print " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    if file_name == "None":
        file_name = errorcode.CMVC_FILE_LIST
    for l_filename in file_name.split(","):
        rc_checkout = utilcode.utilCmvcCheckout(l_filename,release_name,g_cmvc_num)
        if rc_checkout == errorcode.ERROR_CMVC_CHECKOUT:
            print "  [ CMVC File Checkout Failed ] [Error code : %s]\t:%s"%(rc_checkout,l_filename)
            # Undo checkout.. dont check errors just do it
            utilcode.utilRollBack("checkout",g_cmvc_num,release_name)
            # Return to initial directory of operation
            os.chdir(origDir)
            shutil.rmtree(sbDir)
            exit_main(rc_checkout)
        else:
            print "  CMVC File Checkout  [ OK ]"
    print " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    #------------------------------
    # 4) Copy the binaries and file
    #------------------------------
    # Find the files from the repo and copy to the Checkout dir
    print "\n [ Find files and Overide the checkout file ] "
    print " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    rc_copy = utilcode.utilCmvcRepoPath(path_name,g_cmvc_num,file_name)
    if rc_copy == errorcode.ERROR_CMVC_FILE_COPY:
        print "  [ File copy Failed ] [ Error code : %s]"%rc_copy
        # Return to initial directory of operation
        os.chdir(origDir)
        shutil.rmtree(sbDir)
        exit_main(rc_copy)
    else:
        print "  Files Copied Successfully  : [ OK ] "

    print " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    #---------------------
    # 6) Checkin the files
    #---------------------
    print "\n [ Check-in Files from CMVC ] "
    print " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    if file_name == "None":
        file_name = errorcode.CMVC_FILE_LIST
    for files in file_name.split(","):
        rc_checkin = utilcode.utilCmvcCheckin(files,release_name,g_cmvc_num)
        if rc_checkin == errorcode.ERROR_CMVC_CHECKIN:
            print "  [ CMVC File Checkin Failed ] [Error code : %s]\t:%s"%(rc_checkin,files)
            # Undo checkin.. dont check errors just do it
            utilcode.utilRollBack("checkin",g_cmvc_num,release_name)
            # Return to initial directory of operation
            os.chdir(origDir)
            shutil.rmtree(sbDir)
            exit_main(rc_checkin)
        else:
            print "  CMVC File Checkin  [ OK ]"
    print " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

    #---------------------------------
    # 7) Fix record the defect/release
    #---------------------------------
    print "\n [ Fix the Record to complete in CMVC ] "
    utilcode.utilCmvcFixComplete(g_cmvc_num, release_name)

    #---------------------------------
    # 8) Trigger Jenkins CI Job
    #---------------------------------
    print "\n [ Trigering Jenkins job ] "
    rc_ci = utilcode.utilTriggerJenkins(g_cmvc_num, release_name, bvt)
    if rc_ci == errorcode.ERROR_CI_TRIGGER :
       print "  [ CI Trigger Failed ] [Error code : %s]\t"%(rc_ci)
       # Return to initial directory of operation
       os.chdir(origDir)
       shutil.rmtree(sbDir)
       exit_main(rc_ci)
    else :
       print "  CI Trigger [ OK ]"

    #-----------------------------------------
    # Return to initial directory of operation
    #-----------------------------------------
    os.chdir(origDir)
    shutil.rmtree(sbDir)

    # Clean exit
    print "\n [ Manually Integrate on CMVC post CI ] "
    exit_main(errorcode.SUCCESS_EXIT)


if __name__=="__main__":
    main()

