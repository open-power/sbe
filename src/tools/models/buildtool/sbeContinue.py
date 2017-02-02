#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/sbeContinue.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017
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
###########################################################
#    @file     sbeContinue.py
#    @brief    Module to support the main building SBE imag script.
#              This is a hook from the env.bash from ekb to allow
#              the compilation process to continue.
#
#    @author   George Keishing
#
#    @date     Nov 23, 2015
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     11/12/15     Initial create
###########################################################

#-------------------------
# Imports
#-------------------------
import os, sys
import getopt
import time
import subprocess
import threading
from os.path import expanduser    # for getting $HOME PATH
# Local Imports
import sbeUtility as sbeUtility
import sbeGitUtility as sbeGitUtility
import sbeConstants as sbeConstants

#-------------------------
# Global Var for reference
#-------------------------
timestr = time.strftime("%Y%m%d-%H%M%S")
#g_ppeLog = expanduser("~") + '/GIT_REPO_SB/logs/buildPPE_out_' + timestr

#-------------------------
# Main entry
#-------------------------
def main():

    #------------------------------------------
    # Usage tool option
    #------------------------------------------
    def usage():
        print "  ++++++++++++++++++++++++++++++++++++++++++++++++"
        print "  +++  Refer README file for more information  +++"
        print "  ++++++++++++++++++++++++++++++++++++++++++++++++"
        print "  -------------------------------------------------------------------------------------------------------------"
        print "  :: Command line USAGE options for building FW SBE image :: \n"
        print "  sbeContinue.py  -m <model_name> -c < Commit log id> -p < PPE build path> -b <Auto|Manual>"

        print " \n"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print "   | NOTE: Please use VALID input string since validation in code is a tideous task. |"
        print "   |       By default user MUST pass a VALID Model ID.                               |"
        print "   |       -- This is a clone copy args of MAIN to hook into scripts. --             |"
        print "   |       -- However this script won't work by it's own by nature.   --             |"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print " \n  ***** Options Supported  *****"
        print " \t  -m,--model  = Target Model to be used to build the image"
        print " \t  -c,--commit = commit id of the patch to be checkout"
        print " \t  -p,--path   = Path where user wants the image to be built"
        print " \t  -b,--build  = Default is Auto, user can pass Manual"
        print " \n  ***** Allowed options example  *****"

        print "   [ Only Model as an input ]"
        print "   sbeContinue.py -m n10_e9027_tp026_soa_sc_u285_01 "
        print " \n"
        print "   [ Model and PPE commit id as an input ]"
        print "   sbeContinue.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a"
        print " \n"
        print "   [ Model, PPE commit id and PPE image build path as an input ] "
        print "   sbeContinue.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a -p <user path>"
        print " \n"
        print "   [ Choosing to compile ONLY option - By Default it run in Auto mode and trigger CI build and simics run test ]"
        print "   sbeContinue.py -m n10_e9027_tp026_soa_sc_u285_01 -b Manual"
        print "  -------------------------------------------------------------------------------------------------------------"
        exit_main(sbeConstants.HELP_EXIT)

    #------------------------
    # This is main exit func
    #------------------------
    def exit_main(rc):
        if rc:
           print "  \n  [ ERROR - CONTINUE ] Exiting with error code = ", rc
           sys.exit(rc) # get outta from here with rc
        else:
           print "  Build successful [ OK ]\n "
           sys.exit(0) # get outta from here normal

    #-------------------------------------
    # Local var input name place holder
    #-------------------------------------
    model_name  ="None"
    commit_name ="None"
    patch_refs  ="None"
    build_type  ="Auto"
    build_path  ="None"
    env_patch   ="None"

    # UNCOMMENT WHEN IN PRODUCTION
    #l_Log= expanduser("~") + '/GIT_REPO_SB/logs/console_out.log'
    #print "   Check log : %s" % l_Log
    #orig_stdout = sys.stdout
    #f = file(l_Log, 'a+')
    #sys.stdout = f

    #---------------------------
    # Read command line args
    #---------------------------
    opts, args = getopt.getopt(sys.argv[1:],"m:c:i:b:p:e:h",['model=', 'commit=', 'cherry_pick=', 'build=', 'path=', 'env=', 'help'])
    for opt, arg in opts:
       if opt in ('-h', '--help'):
           usage()
           exit_main(1)
       elif opt in ('-m', '--model'):
           model_name = arg
       elif opt in ('-c', '--commit'):
           commit_name = arg
       elif opt in ('-i', '--cherry-pick'):
           patch_refs = arg
       elif opt in ('-b', '--build'):
           build_type = arg
       elif opt in ('-p', '--path'):
           build_path = arg
       elif opt in ('-e', '--env'):
           patch_env = arg
       else:
           usage()
           exit_main(1)

    #---------------------------
    # Path to action file
    #---------------------------
    # This file is for Manual hack per build if extra method is needed.
    # Basically this file is an EMPTY file and the tool reads in next
    # run and executes the commands in the files.
    if build_path == "None":
        l_ppeLog = expanduser("~") + '/GIT_REPO_SB/logs/buildPPE_out_' + timestr
        l_action_file = expanduser("~") + '/GIT_REPO_SB/logs/PATCH_ACTION_TEMPLATE' + '_' + model_name
        l_buildActionHooks = expanduser("~") + '/GIT_REPO_SB/logs/buildActionHooks' + '_' + model_name
    else:
        l_ppeLog = build_path + '/GIT_REPO_SB/logs/buildPPE_out_' + timestr
        l_action_file = build_path + '/GIT_REPO_SB/logs/PATCH_ACTION_TEMPLATE' + '_' + model_name
        l_buildActionHooks = build_path + '/GIT_REPO_SB/logs/buildActionHooks' + '_' + model_name

    #----------------------------------
    # Preping the data for Image build
    #----------------------------------
    print "  \n"
    print "  ******************************************************"
    print "  ********* ENV Loaded for compilation Build ***********"
    print "  ******************************************************"

    #---------------------------------------------------
    # Check if Action file pending from last Test run ?
    #---------------------------------------------------
    def patch_action():
        # Model name Action exist ?
        if os.path.isfile(l_action_file):
            print "\n  [ Checking if PATCH action exist ]"
            # Check if the file is empty or not
            if os.stat(l_action_file).st_size == 0:
                print "   %s is EMPTY.. Add instruction" % l_action_file
                return sbeConstants.PATCH_IGNORE_EXIT
            else:
                print "   Executing the line entries as commands in the file"

                # Load the action file and read per line as command and execute
                with open(l_action_file) as fp:
                    for line in fp:
                        if not line: continue
                        else:
                           l_cmd = line.strip()
                           print "\n   ++++++++++++++++++++++++++++++++++++++++++++++++++"
                           print "     Executing command : %s" % l_cmd
                           print "   ++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                           rc = os.system( l_cmd )
                           if rc:
                               # Fix the things manually and compile
                               print "\n  Error Executing : %s" % l_cmd
                               exit_main(rc)
                           else:
                               continue # next line
            print "   ++++++++++++++++++++++++++++++++++++++++++++++++++"
            return sbeConstants.SUCCESS_EXIT

    #-------------------------------------
    # Stagged and Copy model files
    #-------------------------------------
    def PPE_ModelCopy():
        print "\n  [ Stagging Model file's from EKB to PPE ]"
        ret_code=sbeUtility.utilCopyModel(build_path)
        if ret_code:
            print "  Copying model files [ FAILED ] rc = %s " % ret_code
            exit_main(ret_code)
        else:
            print "  Copying model files [ OK ]"

    #-------------------------------------
    # Show a git Status display
    #-------------------------------------
    def gitPPE_Status():
        print "\n  [ PPE Git Status log ]"
        sbeUtility.utilGitStatus(build_path)

    #------------------------------------------
    # Show the git commit filesa passed by user
    #------------------------------------------
    def gitPPE_Files():
        print "\n  [ PPE Git show files for commit = %s ]" % commit_name
        git_show_ret=sbeGitUtility.utilGitShowCommitFiles(commit_name, build_path)
        if git_show_ret:
            print "  This commit ID passed is either not Valid or active [ FAILED ] rc = %s" % git_show_ret
            # Remove the action file since its a basic failure
            os.remove(l_buildActionHooks)
            exit_main(git_show_ret)
        else:
            if commit_name == "None":
                print ""
            else:
                print "\n  This commit ID is active and valid [ OK ]"
    #-------------------------------------
    # Chckout PPE from user input commit
    #-------------------------------------
    def gitPPE_Checkout():
        print "\n  [ PPE Git checkout for commit = %s ]" % commit_name
        git_checkout_ret=sbeGitUtility.utilGitCommitCheckout(commit_name, build_path)
        if git_checkout_ret:
            print "  Checkout of commit [ FAILED ] rc = %s" % git_checkout_ret
            exit_main(git_checkout_ret)
        else:
            if commit_name == "None":
                if build_path == "None":
                    l_home_build_path = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
                else:
                    l_home_build_path = build_path + "/" +sbeConstants.GIT_PPE_REPO_SB

                os.chdir(l_home_build_path)
                print "   --> ", l_home_build_path
                print "  Working on branch checkout \n"
                os.system("git branch")
            else:
                print "\n  Checkout commit to test_build [ OK ]"

    #-------------------------------------
    # Compile PPE code
    #-------------------------------------
    def Trigger_buildPPE():
        print "\n  [ PPE Repo Build triggered ]"
        if build_type == "Auto":
            print "\n  *** Build and trigger CI in progress... ***"
        else:
            print "\n  *** Sandbox Build in progress... ***"

        build_ret=sbeUtility.utilBuildPPE(build_type, build_path, model_name,patch_env)
        if build_ret == sbeConstants.BUILD_MANUAL_FAILED:
            print "  Sandbox Build Manual Mode [ FAILED ] rc = %s " % build_ret
            # Create a template file PATCH_ACTION_TEMPLATE so that user can manually
            # add commands or instructions for execution
            WriteActionFile()
            print "  --> Edit Manually : %s" % l_action_file
            exit_main(build_ret)
        elif build_ret == sbeConstants.BUILD_CI_FAILED:
            print "  CI Triggered build or simics test [ FAILED ] rc = %s " % build_ret
            # Create a template file PATCH_ACTION_TEMPLATE so that user can manually
            # add commands or instructions for execution
            WriteActionFile()
            print "  --> Edit Manually : %s" % l_action_file
            exit_main(build_ret)
        else:
            # per model per log
            l_ppeLog_open = l_ppeLog + "_" + model_name +".log"
            if build_type == "Auto":
                # Look up the log and verify
                with open(l_ppeLog_open) as fp:
                   for line in fp:
                       if not "Finished tests on" in line:
                           continue
                       else:
                           print "  --> %s" % line

                print "  Build and CI run [ OK ]"
            else:
                print "  Sandbox PPE build compiled [ OK ]"

            # Remove the action file since its left over
            os.remove(l_buildActionHooks)

    #-------------------------------------
    # Show the PPE obj dir listing build
    #-------------------------------------
    def show_buildPPEObj():
        # Just show the ppe/obj/ dir listing
        print "\n  [ Listing PPE obj build directory files ]"
        if build_path == "None":
            l_home_build_path = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB  + "/obj"
        else:
            l_home_build_path = build_path + "/" +sbeConstants.GIT_PPE_REPO_SB + "/obj"

        l_listCmd = "ls -lart " + l_home_build_path
        print "  PPE obj : ", l_home_build_path
        os.system(l_listCmd)

    #-------------------------------------
    # Compile PPE code
    #-------------------------------------
    def WriteActionFile():
        print "\n  [ Writting Action File for build ]"
        sbeUtility.utilWriteActionFile(l_action_file, model_name)

    # Do stuff here in sequence


    #-------------------------------------
    #  Call in sequentially for build
    #-------------------------------------
    patch_ret_code = patch_action()
    if patch_ret_code == sbeConstants.PATCH_IGNORE_EXIT:
        print "  --> Continuing with EMPTY patch instruction"
        print "  [ !!! Manually add steps in the file & re-run the tool !!! ]"

    #------------------------
    # Stagging files
    #------------------------
    gitPPE_Files()
    #gitPPE_Checkout()

    #------------------------
    # Copy Model files needed
    #------------------------
    PPE_ModelCopy()
    gitPPE_Status()

    #------------------------
    # Trigger PPE build
    #------------------------
    Trigger_buildPPE()
    show_buildPPEObj()

    # Close the file
    # UNCOMMENT WHEN IN PRODUCTION
    #sys.stdout = orig_stdout
    #f.close()

    #----------------------------------
    # Exit gracefully and return the
    # control to the main script
    #----------------------------------
    exit_main(0)

    #-------------------------------------
    #  End of this main .. exiting
    #-------------------------------------

if __name__=="__main__":
    main()
