#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/buildSBEImage.py $
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
##############################################################
#    @file     buildSBEImage.py
#    @brief    Main Module to support building SBE image
#
#    @author   George Keishing
#
#    @date     Nov 23, 2015
#
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     11/12/15     Initial create
##############################################################

#-------------------------
#  Imports 
#-------------------------
import os, sys, glob
import getopt
import os.path
import subprocess
import time
import datetime
import threading
from os.path import expanduser    # for getting $HOME PATH
# Local imports
import sbeUtility as sbeUtility
import sbeConstants as sbeConstants
#-------------------------

#-------------------------
# Global Var for reference
#-------------------------

#-------------------------
# Main Functions
#-------------------------
def main():

    #-----------------------
    # Start time tick tock
    #-----------------------
    start_time = datetime.datetime.now().replace(microsecond=0)

    #------------------------------------------
    # Usage tool option
    #------------------------------------------
    def usage():
        print "  ++++++++++++++++++++++++++++++++++++++++++++++++"
        print "  +++  Refer README file for more information  +++"
        print "  ++++++++++++++++++++++++++++++++++++++++++++++++"
        print "  -------------------------------------------------------------------------------------------------------------"
        print "  :: Command line USAGE options for building FW SBE image :: \n"
        print "  buildSBEImage.py  -m <model_name> -c < Commit log id> -i <abc,xyz> -p < PPE build path> -b <Auto|Manual|Force> -e <ENV exports>"

        print " \n"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print "   | NOTE: Please use VALID input string since validation in code is a tideous task. |"
        print "   |       By default user MUST pass a VALID Model ID.                               |"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print " \n  ***** Options Supported  *****"
        print " \t  -m,--model        = Target Model to be used to build the image"
        print " \t  -c,--commit       = Base commit id of the patch to be checkout"
        print " \t  -i,--cherry-pick  = refs id of the patch to be cherry pick"
        print " \t  -p,--path         = Path where user wants the image to be built"
        print " \t  -b,--build        = Default is Auto, user can pass Manual/Force"
        print " \t                      Auto   : CI build and test"
        print " \t                      Manual : Only compiles No Simics run"
        print " \t                      Force  : Rebuild Repo"
        print " \t  -e,--env          = User ENV build exports"
        print " \n  ***** Allowed options example  *****"

        print "   [ Only Model as an input ]"
        print "   buildSBEImage.py -m n10_e9027_tp026_soa_sc_u285_01 "
        print " \n"
        print "   [ Model and PPE commit id as an input ]"
        print "   buildSBEImage.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a"
        print " \n"
        print "   [ Model and PPE commit id and cherry pick refs as an input ]"
        print "   [ ** Coma ',' separated cherry pick patch refs **]"
        print "   buildSBEImage.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a -i refs/changes/57/21157/3, refs/changes/35/22535/2"
        print " \n"
        print "   [ Model, PPE commit id and PPE image build path as an input ] "
        print "   buildSBEImage.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a -p <user path>"
        print " \n"
        print "   [ Choosing to compile ONLY option - By Default it run in Auto mode and trigger CI build and simics run test ]"
        print "   buildSBEImage.py -m n10_e9027_tp026_soa_sc_u285_01 -b Manual"
        print " \n"
        print "   [ Choosing to do fresh Force build ]"
        print "   buildSBEImage.py -m n10_e9027_tp026_soa_sc_u285_01 -b Force"
        print " \n"
        print "   [ Facility to -ONLY- Build PPE with ENV passed and run simcis without EKB models ]"
        print "   buildSBEImage.py -m <model> -c b85fe14bd0c86ff223ac15e261ead4877949084c -i refs/changes/57/21157/3,refs/changes/35/22535/2 -e '__FAPI_DELAY_SIM__=1'"
        print "  -------------------------------------------------------------------------------------------------------------"
        exit_main(sbeConstants.HELP_EXIT)
        
    #------------------------------------------
    # Exit from this Main
    #------------------------------------------
    def exit_main(rc):
        if rc == sbeConstants.HELP_EXIT:
           print ""
           sys.exit(0)

        #------------------------------------- 
        # Calculate the time different and log 
        #------------------------------------- 
        end_time = datetime.datetime.now().replace(microsecond=0) 
        print "\n *********************************************************"
        print "  Build Time End :  ", end_time 
        print "  [ Total Time taken for build :  %s ] " % ( end_time - start_time )
        print "\n *********************************************************"

        if rc:
           print "\n  [ ERROR - MAIN ] Exiting with error code = ", rc
           sys.exit(rc)
        else:
           print "  Build PPE image build process completed [ OK ] "
           sys.exit(0)

    #------------------------------------------
    # Local var place name holder's
    #------------------------------------------
    model_name  = "None"
    commit_name = "None"
    patch_refs  = "None"
    build_type  = "Force"
    build_path  = "None"
    env_patch   = "None"

    # UNCOMMENT WHEN IN PRODUCTION
    #l_Log= expanduser("~") + '/GIT_REPO_SB/logs/console_out.log'
    #print "   Check log : %s" % l_Log
    #orig_stdout = sys.stdout 
    #f = file(l_Log, 'w') 
    #sys.stdout = f

    #----------------------------
    # Read command line args
    #----------------------------
    opts, args = getopt.getopt(sys.argv[1:],"m:c:i:p:b:e:h",['model=', 'commit=', 'cherry-pick=', 'path=', 'build=', 'env=', 'help'])
    for opt, arg in opts:
       if opt in ('-h', '--help'):
           usage() 
           exit_main(sbeConstants.ERROR_SYS_ERROR_EXIT)
       elif opt in ('-m', '--model'):
           model_name = arg
       elif opt in ('-c', '--commit'):
           commit_name = arg
       elif opt in ('-i', '--cherry-pick'):
           patch_refs = arg
       elif opt in ('-p', '--path'):
           build_path = arg
       elif opt in ('-e', '--env'):
           env_patch = arg
       elif opt in ('-b', '--build'):
           build_type = arg
       else:
           usage()
           exit_main(sbeConstants.ERROR_SYS_ERROR_EXIT)

    #----------------------------------
    # Preping the data for Image build
    #----------------------------------
    print " \n"
    print "  ******************************************************"
    print "  ****** Preping workspace for PPE image Build *********"
    print "  ******************************************************"

    #------------------------------------------------------
    # Make sure that it has passed atleast one arg with it 
    #------------------------------------------------------
    if len(sys.argv)<2:
        usage()
        exit_main(sbeConstants.ERROR_SYS_ERROR_EXIT)
    else:
        if build_path == "None":
            #---------------
            # Default paths
            #---------------
            l_home_ekb_path= expanduser("~") + "/" +sbeConstants.GIT_EKB_REPO_SB
            l_home_ppe_path= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
            l_buildActionHooks  = expanduser("~") + '/GIT_REPO_SB/logs/buildActionHooks' + '_' + model_name
            l_RepoSuccess       = expanduser("~") + '/GIT_REPO_SB/logs/sandboxSkip'
            l_action_file       = expanduser("~") + '/GIT_REPO_SB/logs/PATCH_ACTION_TEMPLATE' + '_' + model_name
            l_log_dir           = expanduser("~") + '/GIT_REPO_SB/logs/'
        else:
            #----------------------
            # User Specified paths
            #---------------------
            l_home_ekb_path= build_path + "/" +sbeConstants.GIT_EKB_REPO_SB
            l_home_ppe_path= build_path + "/" +sbeConstants.GIT_PPE_REPO_SB
            l_buildActionHooks  = build_path + '/GIT_REPO_SB/logs/buildActionHooks' + '_' + model_name
            l_RepoSuccess       = build_path + '/GIT_REPO_SB/logs/sandboxSkip'
            l_action_file       = build_path + '/GIT_REPO_SB/logs/PATCH_ACTION_TEMPLATE' + '_' + model_name
            l_log_dir           = build_path + '/GIT_REPO_SB/logs/'

    #------------------------------------------------------------- 
    # Local functions
    #------------------------------------------------------------- 

    #------------------------------------------------------------- 
    # Default main func
    #------------------------------------------------------------- 
    def main(): 
        print ""

    # This is for future logging purpose.. keep it or re-implement
    #logging.basicConfig(filename='test.log', level=logging.INFO) 
    # create logger
    #logger = logging.getLogger('buildSBEImage_log') 
    #logger.setLevel(logging.DEBUG)

    # create console handler and set level to debug 
    #ch = logging.StreamHandler() 
    #ch.setLevel(logging.DEBUG) 

    # create formatter 
    #formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s') 

    # add formatter to ch 
    #ch.setFormatter(formatter) 

    # add ch to logger 
    #logger.addHandler(ch)

    #------------------------------------------------------------- 
    # Check user inputs and display
    #------------------------------------------------------------- 
    def input_setting(): 
        print "  ---------------------------------------------------------------------------------"
        print "           [ Display User Inputs ]" 
        print "  ---------------------------------------------------------------------------------"
        print "  [ Build Time Start ]        \t =", start_time
        if not model_name == "None":
            print "  [ Model ID ]  model name\t = %s"%(model_name) 
        else:
            print "  [ Model ID ]  This can't be empty if EKB model is needed "
            print "                Proceeding to build -ONLY- PPE image"
            #exit_main(0)

        print "  [ Optional ]  commit ID\t = %s"%(commit_name)
        print "  [ Optional ]  Patch ref\t = %s"%(patch_refs)
        print "  [ Optional ]  Build path\t = %s"%(build_path)
        print "  [ Optional ]  Build Type\t = %s"%(build_type)
        print "  [ Optional ]  ENV PATCH\t = %s"%(env_patch)
        print "  ---------------------------------------------------------------------------------"
        print "  ---------------------------------------------------------------------------------"
        print "  [ ** These Files are generated on Failure/incomplete Build process ** ]"
        print "  ---------------------------------------------------------------------------------"
        print "  -> Patch Action File =",l_action_file
        print "  -> buildActionHooks  =",l_buildActionHooks
        print "  -> Sandbox checkpoint=",l_RepoSuccess
        print "  ---------------------------------------------------------------------------------"
        print "  ---------------------------------------------------------------------------------"
        print "  [ Repo sandbox paths ] "
        print "  ---------------------------------------------------------------------------------"
        print "  -> EKB HOME path =", l_home_ekb_path
        print "  -> PPE HOME path =", l_home_ppe_path
        print "  ---------------------------------------------------------------------------------"

    #------------------------------------------------------------- 
    # Check pre-req setup requirements
    #------------------------------------------------------------- 
    def user_setting():
        print "\n  [ Checking Pre-requisite settings ]"
        token_rc=sbeUtility.utilCheckTokens()
        if token_rc:
            print "  One of the access AWD/Austin Token NOT found [ FAILED ]"
            print "  Please klog -c to awd and austn to get the access tokens"
            exit_main(token_rc)
        else:
            print "\n   Token access grant found [ OK ]\n"

        setup_rc=sbeUtility.utilCheckSetup()
        if setup_rc:
            print "  Gerrit Setup Access for hw.gerrit Not found [ FAILED ]"
            print "  * Check the README section CONFIGURATION "
            exit_main(setup_rc)
        else:
            print "  Gerrit setup found [ OK ]"

    #------------------------------------------------------------- 
    # Clean up existing setup for fresh build from master
    #------------------------------------------------------------- 
    def cleanup_build(): 
        if not os.path.exists(l_RepoSuccess):
            print "\n  [ Cleaning up working Sandboxes/Repo ]" 
            sbeUtility.utilCleanup(build_path)
            print "  Clean up sandbox Repo [ OK ]\n"
        else:
            print "  Looks like there is failure post Repo create earlier"

    #------------------------------------------------------------- 
    # Build default sandboxes for PPE
    #------------------------------------------------------------- 
    def sandbox_build_ppe(): 
        ret_ppe_code=sbeUtility.utilCreateSandbox_ppe(build_path)
        if ret_ppe_code:
            print "  Creating ppe sandbox [ FAILED ] rc =%s" % ret_ppe_code
            exit_main(ret_ppe_code)
        else:
            print "  Creating ppe sandbox [ OK ]"

    #------------------------------------------------------------- 
    # Build default sandboxes for EKB 
    #------------------------------------------------------------- 
    def sandbox_build_ekb(): 
        ret_ekb_code=sbeUtility.utilCreateSandbox_ekb(build_path)
        if ret_ekb_code:
            print "  Creating ekb sandbox [ FAILED ] rc= %s" % ret_ekb_code
            exit_main(ret_ekb_code)
        else:
            print "  Creating ekb sandbox [ OK ]"

        if build_path == "None":
            print "  Using Default path for EKB Repo"
        else:
            print "  Creating Sandbox using user input path"

        # Sanbox create is good.. Ignore re create if it failed in this run
        # This ensure that we can't keep creating repo each run for same model
        if not os.path.exists(l_RepoSuccess): 
            open(l_RepoSuccess, 'w').close()

    #-------------------------------------------------------------
    # Is the Saved off Hook file same as the user input
    #-------------------------------------------------------------
    def ValidateHookFile():
        print "\n  [ Validate Action hooks Signature vs inputs ] "
        ret_val_code = sbeUtility.utilValidateHookFile(l_buildActionHooks, model_name, commit_name, patch_refs, build_path, build_type, env_patch)
        if ret_val_code == sbeConstants.VALIDATE_EXIT:
            print "  * Saved off action files signature doesn't match the user input"
            return sbeConstants.VALIDATE_EXIT
        else:
            print "  * Signature Match 1x1 with Action File saved off earlier"
            return sbeConstants.SUCCESS_EXIT

    #-------------------------------------------------------------
    # By-pass the main init process if the build failed latter
    #-------------------------------------------------------------
    def bypass_main():
        print "\n  [  Check if by-pass is allowed for this run ]"
        ret_bypass_code = sbeUtility.utilBypassCheck(l_buildActionHooks)
        if ret_bypass_code == sbeConstants.BYPASS_EXIT:
            # Wait don't be in hurry, it may also mean this model
            # id is different so check if it has patch action

            print "\n  [ Checking if this Model has PATCH action Pending ]"
            ret_continue_code = ThisModelPatchExist(model_name)
            if ret_continue_code == sbeConstants.NO_PENDING_MODEL_EXIT:
                print "  There is No Failure noticed in earlier build for this Model "
                return sbeConstants.NO_PENDING_MODEL_EXIT
            elif ret_continue_code == sbeConstants.NEW_MODEL_EXIT:
                print "  --> By-pass action pending.. but not for this model"
                print "  This is a new Model name user input, treating as new build"
                return sbeConstants.NEW_MODEL_EXIT
            else:
                # Make sure the signature matches before loading it
                ret_val_code = ValidateHookFile()
                if ret_val_code == sbeConstants.VALIDATE_EXIT:
                    print "  * Signature - NOT- matching to continue .. "
                    return sbeConstants.NEW_MODEL_EXIT
                else:
                    # Load the action file
                    print "  --> By-pass action pending.. Loading"
                    with open(l_buildActionHooks) as fp:
                        for line in fp:
                            # convert the single line entry to command
                            l_cmd = line.strip()
                            print "\n  Executing : %s" % l_cmd
                            rc = os.system( l_cmd )
                            if rc:
                                # Fix the things manually and compile
                                exit_main(rc)
                            else:
                                # getta out Exit from here ONLY
                                exit_main(sbeConstants.SUCCESS_EXIT)
        else:
            print"  No by-pass action pending.. Continuing"

    #-------------------------------------------------------------
    # validate PPE Model
    #-------------------------------------------------------------
    def ValidatePPEModel():
        # Call in PPE Model check code
        # Find out from where the main script triggered,
        # yank out the last word and replace with the hook script
        print "\n  [ Validating PPE Model base ]" 
        l_string=__file__
        l_string=l_string.rsplit('/', 1)[0]
        l_string=l_string +'/ppeModel.py'

        ppe_cmd = "python " + l_string + " -m " + model_name + " -c " + commit_name + " -i " + patch_refs + " -p " + build_path + " -b " + build_type + " -e " + env_patch
        print "  Executing : ", ppe_cmd

        rc_ppe_model = os.system(ppe_cmd)
        if not rc_ppe_model == sbeConstants.SUCCESS_EXIT:
            print "  * PPE build conflict.. Resolve Manually or Review Patches"
            #print "  * Switch build to Manual to skip strict checking - ONLY- if needed"
            exit_main(rc_ppe_model)
        else:
            return sbeConstants.SUCCESS_EXIT

    #------------------------------------------------------------- 
    # Workon the EKB and then hook to the env.bash to load ENV
    #------------------------------------------------------------- 
    def LoadEnv():
        print "  --------------------------" 
        print "  [ Loading EKB workon ENV ]" 
        print "  --------------------------" 
        ret_env_load=sbeUtility.utilLoadEnv(model_name , commit_name, patch_refs, build_type, build_path,env_patch)
        if ret_env_load == sbeConstants.ERROR_LOAD_ENV:
            print "  EKB ENV load [ FAILED ] rc= %s" % ret_env_load
            exit_main(ret_env_load)
        elif ret_env_load == sbeConstants.ERROR_MODEL_NOT_EXIST:
            # Remove the action file since its a basic failure
            # But check if its for sure ? since bash returns by default this RC
            rc_ekb_code = sbeUtility.utilEkbModelExist(model_name, l_home_ekb_path )
            if rc_ekb_code == sbeConstants.EKB_MODEL_NOT_EXIST:
                print "  EKB Model [%s] Not Found [ FAILED ] rc= %s" % (model_name,rc_ekb_code)
                os.remove(l_buildActionHooks)
            else:
                print "  EKB Model [%s] Build [ FAILED ] rc= %s" % (model_name,ret_env_load)

            exit_main(ret_env_load)
        else:
            print "  EKB ENV load and Exited [ OK ] " 

    #-------------------------------------------------------------
    # is Model patch action of this user input exist ?
    #-------------------------------------------------------------
    def ThisModelPatchExist(model_name):
        # It is possible that the user just ran Model xxx and failed, 
        # and still wants to try another Model yyy

        # if the patch action exist, then continue else 
        # allow to do build and jump to corresponding checkpoint
        if os.path.exists(l_action_file):
            if os.path.exists(l_buildActionHooks):
                return sbeConstants.SUCCESS_EXIT
            else:
                return sbeConstants.NEW_MODEL_EXIT
        else:
            # This is an indication this is new
            return sbeConstants.NO_PENDING_MODEL_EXIT

    #-------------------------------------------------------------
    # Cleanup on exit
    #-------------------------------------------------------------
    def cleanupOnExit(): 
        if os.path.exists(l_RepoSuccess):
            print "  Removing Sandbox create log = %s" % l_RepoSuccess
            os.remove(l_RepoSuccess)

        # PPE base file 
        l_base_file = l_log_dir +"/base"
        if os.path.exists(l_base_file):
            print "  Removing PPE base file= %s" % l_base_file
            os.remove(l_base_file)

        # Use glob method to remove files starting with same name
        if build_path == "None":
            l_Hooks  = expanduser("~") + '/GIT_REPO_SB/logs/buildActionHooks*'
        else:
            l_Hooks  = build_path + '/GIT_REPO_SB/logs/buildActionHooks*'

        for filename in glob.glob(l_Hooks):
            # Don't remember anything if its a clean exit
            print "  Removing Hooks file = %s" % filename
            os.remove(filename)

    #------------------------------------- 
    # MAIN : Call the sequence func here
    #------------------------------------- 

    #----------------
    # Pre-req cheks
    #----------------
    input_setting()
    user_setting()

    #-------------------------
    # Force fresh new build
    #-------------------------
    if build_type == "Force":
        print "\n  [ Deafult :  Forced to rebuild Repo and sandboxes for clean build ]" 
        cleanupOnExit()
        # Job done , now fall back to Auto mode
        build_type = "Auto"

    #-------------------------
    # Manual new build
    #-------------------------
    if build_type == "Manual":
        print "\n  [ Manual Repo and sandboxes for clean build ]" 
        cleanupOnExit()
        # User knows what they are doing this for
        build_type = "Manual"

    #----------------
    # By-pass cheks
    #----------------
    # is this the first test run ?
    ret_log_code = sbeUtility.utilFilesInDirExist(l_log_dir)
    if ret_log_code == sbeConstants.FILES_EXIST: 
        ret_bypass = bypass_main() 
        # if new model and not the previous run 
        # clean start fresh 
        if ret_bypass == sbeConstants.NEW_MODEL_EXIT: 
            cleanupOnExit()
        elif ret_bypass == sbeConstants.NO_PENDING_MODEL_EXIT:
            cleanupOnExit()
    else:
        print "\n  [ Clean Build test started ]"

    #---------------------------
    # Build ENV HACK
    #---------------------------
    if not env_patch == "None": 
        # This will compile and run CI internally
        if model_name == "None":
            cleanup_build()
            print "\n  [ PPE ENV : Setting up PPE working Sandboxes/Repo ]" 
            sandbox_build_ppe()

            ret_code_hack = ValidatePPEModel()
            if ret_code_hack == sbeConstants.SUCCESS_EXIT:
                if build_type == "Auto":
                    print "  Build and CI successfully completed [ OK ] "
                else:
                    print "  Build successfully completed [ OK ] "
                    sbeUtility.utilshowBuildPPEObj(build_path)
            else:
                print "  Build -OR- CI [ FAILED ] rc = ",ret_code_hack
            # Don't go beyond this.. just exit
            exit_main(0)

    #---------------------------
    #  if Model name is empty
    #---------------------------
    if model_name == "None":
        cleanup_build()
        print "\n  [  PPE Model : Setting up PPE working Sandboxes/Repo ]"
        sandbox_build_ppe()
        # This will compile and run CI internally
        ret_code_ppe = ValidatePPEModel()
        if ret_code_ppe == sbeConstants.SUCCESS_EXIT:
            if build_type == "Auto":
                print "  Build and CI successfully completed [ OK ] "
            else:
                print "  Build successfully completed [ OK ] "
            sbeUtility.utilshowBuildPPEObj(build_path)
        else:
            print "  Build -OR- CI [ FAILED ] rc = ",ret_code_ppe
        # Don't go beyond this.. just exit
        exit_main(0)

    #---------------------------
    # Pre-req sandbox stagging
    #---------------------------
    if not os.path.exists(l_RepoSuccess):
        cleanup_build()
        print "\n  [ Setting up working Sandboxes/Repo ]" 
        sandbox_build_ppe()

        # skip EKB if Model name is empty
        if not model_name == "None":
            sandbox_build_ekb()
    else:
        print "  * Skipping Repo create [ Previous run hasn't completed ]"

    #---------------------------
    # Pre-req PPE model check
    #---------------------------
    ValidatePPEModel()

    #---------------------------
    # if Model name is empty
    #---------------------------
    if model_name == "None":
        print "  PPE build process completed [ OK ]"
        cleanupOnExit()
        # Since we are not going to get code from EKB
        # Exit from here
        exit_main(0)

    #---------------------------
    # Load EKB and hook
    #---------------------------
    LoadEnv()

    #---------------------------
    # Clean up on exit
    #---------------------------
    cleanupOnExit()

    # Close the file
    # UNCOMMENT WHEN IN PRODUCTION
    #sys.stdout = orig_stdout 
    #f.close()

    exit_main(0)

    #------------------------------------- 
    # End of the main.. exit
    #------------------------------------- 

if __name__=="__main__":
    main()
