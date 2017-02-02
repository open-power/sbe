#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/ppeModel.py $
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
#    @file     ppeModel.py
#    @brief    Model tool to creat and validate the PPE model
#
#    @author   George Keishing
#
#    @date     Dec 07, 2015
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
import ppeUtility as ppeUtility
import sbeUtility as sbeUtility
import sbeGitUtility as sbeGitUtility
import sbeConstants as sbeConstants

#-------------------------
# Global Var for reference
#-------------------------
timestr = time.strftime("%Y%m%d-%H%M%S")
g_ppeLog = expanduser("~") + '/GIT_REPO_SB/logs/ppeModel_out_' + timestr

#-------------------------
# Main entry
#-------------------------
def main():

    #------------------------
    # This main exit func
    #------------------------
    def exit_main(rc):
        if rc:
           print "  \n  [ ERROR - PPE MODEL ] Exiting with error code = %d "% rc
           sys.exit(rc) # get outta from here with rc
        else:
           print "  PPE Model and Processing [ OK ]\n "
           sys.exit(0) # get outta from here normal

    #------------------------------------------
    # Usage tool option
    #------------------------------------------
    def usage():
        print "  ++++++++++++++++++++++++++++++++++++++++++++++++"
        print "  +++  Refer README file for more information  +++"
        print "  ++++++++++++++++++++++++++++++++++++++++++++++++"
        print "  -------------------------------------------------------------------------------------------------------------"
        print "  :: Command line USAGE options for building FW SBE image :: \n"
        print "  ppeModel.py  -m <model_name> -c < Commit log id> -i <patch refs> -p < PPE build path> -s <None|Create>"

        print " \n"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print "   | NOTE: Please use VALID input string since validation in code is a tideous task. |"
        print "   |       By default user MUST pass a VALID Model ID.                               |"
        print "   |       This script alone doesn't work properly.                                  |"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print " \n  ***** Options Supported  *****"
        print " \t  -m,--model          = Target Model to be used to build the image"
        print " \t  -c,--commit         = commit id of the patch to be checkout"
        print " \t  -i,--cherry-pick   = patch refs to be applied"
        print " \t  -p,--path           = Path where user wants the image to be built"
        print " \t  -s,--stagged        = Default is None, user can pass Create"
        print " \n  ***** Allowed options example  *****"

        print "   [ Only Model as an input ]"
        print "   ppeModel.py -m n10_e9027_tp026_soa_sc_u285_01 "
        print " \n"
        print "   [ Model and PPE commit id as an input ]"
        print "   ppeModel.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a"
        print " \n"
        print "  -------------------------------------------------------------------------------------------------------------"
        print "   [  Creating PPE model base directory ]"
        print "   [  Creates models/n10_e9027_tp026_soa_sc_u285_01/ directory and base file with the commit ]"
        print "   ppeModel.py -m n10_e9027_tp026_soa_sc_u285_01 -c c1980f5a4de0bac8922260ed827fe37a124b2b0a -s create"
        print "  -------------------------------------------------------------------------------------------------------------"
        exit_main(sbeConstants.HELP_EXIT)

    #-----------------------------------
    # Local var input name place holder
    #-----------------------------------
    model_name  ="None"
    commit_name ="None"
    patch_refs  ="None"
    build_path  ="None"
    create_ppe_model  ="None"
    build_type  ="Auto"
    env_patch   ="None"

    # Local for base checkout from PPE model
    l_base_commit ="None"
    l_ignore_base =False

    #-----------------------------------
    # Read command line args
    #-----------------------------------
    opts, args = getopt.getopt(sys.argv[1:],"m:c:i:p:s:b:e:h",['model=', 'commit=', 'cherry-pick=', 'path=', 'stagged=', 'build=',  'env', 'help'])
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
       elif opt in ('-p', '--path'):
           build_path = arg
       elif opt in ('-s', '--stagged'):
           create_ppe_model = arg
       elif opt in ('-b', '--build'):
           build_type = arg
       elif opt in ('-e', '--env'):
           env_patch = arg
       else:
           usage()
           exit_main(1)

    #-----------------------------------
    # Preping the data for Image build
    #-----------------------------------
    print "  \n"
    print "  ******************************************************"
    print "  *********  ENV Loaded for PPE Model Build  ***********"
    print "  ******************************************************"

    #-------------------------------------------------------------
    # Build default sandboxes for EKB and PPE
    #-------------------------------------------------------------
    def sandbox_build():
        print "\n  [ Setting up working Sandboxes/Repo ]"
        if build_path == "None":
            print "  Using Default PPE git Repo"
        else:
            print "  Setting up the PPE repor at = ", build_path

        ret_ppe_code=sbeUtility.utilCreateSandbox_ppe(build_path)
        if ret_ppe_code:
            print "  Creating ppe sandbox [ FAILED ] rc =%s" % ret_ppe_code
            exit_main(ret_ppe_code)
        else:
            print "  Creating ppe sandbox [ OK ]"

    #-------------------------------------------------------------
    # Build default sandboxes for EKB and PPE
    #-------------------------------------------------------------
    def change_ppe_dir():
        if build_path == "None":
            l_home_path_ppe= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
        else:
            l_home_path_ppe= build_path + "/" +sbeConstants.GIT_PPE_REPO_SB

        os.chdir(l_home_path_ppe)

    #-------------------------------------
    # Create Model
    #-------------------------------------
    def ppeModelCreate():
        # Create Model
        rc_create = ppeUtility.utilppeModelCreate(model_name, build_path, commit_name)
        if rc_create == sbeConstants.SUCCESS_EXIT:
            exit_main(0)
        else:
            exit_main(rc_create)

    #-------------------------------------
    # Staged PPE model
    #-------------------------------------
    def ppeModelExist():
        # Model name exist ?
        rc_exist = ppeUtility.utilppeModelExist(model_name, build_path, commit_name)
        if rc_exist == sbeConstants.SUCCESS_EXIT:
            print "  PPE Model directory exist [ OK ]"
            print " \n"
            print "  ** Proceeding with the PPE Model base commits"
        elif rc_exist == sbeConstants.PPE_MODEL_SKIP_EXIT:
            print "  * Skipping Model check in PPE since it's a user explicit request"
            return sbeConstants.PPE_MODEL_SKIP_EXIT
        else:
            print "  PPE Model directory doesn't exist [ FAILED ] rc = ",rc_exist
            if build_type == "Auto":
                print "  * Continuing with PPE master checkout to build"
                return sbeConstants.PPE_MODEL_SKIP_EXIT
                #exit_main(rc_exist)
            else:
                print "  * Allowing to continue since it's a Manual Build run"
                # Do no more of other task in this Model, return to MAIN
                exit_main(sbeConstants.SUCCESS_EXIT)

    #-------------------------------------
    # Checkout base commit test_model
    #-------------------------------------
    def ppeUserbaseCheckout():
        print "\n"
        print "   -------------------------------"
        print "  [ Checking out user base commit ] "
        print "   -------------------------------"
        l_gitCmd='git checkout ' + commit_name + ' -b test_build'
        print "  Executing : ", l_gitCmd
        rc = os.system(l_gitCmd)
        if rc:
            return sbeConstants.PPE_BASE_ERROR_EXIT
        else:
            return sbeConstants.SUCCESS_EXIT

    #-------------------------------------
    # Checkout base commit test_model
    #-------------------------------------
    def ppebaseCheckout():
        # Check out the base commit
        print "\n"
        print "   -------------------------------"
        print "   [ PPE Model base Check-out ]"
        print "   -------------------------------"
        rc_base = ppeUtility.utilCheckOutCommit(model_name, build_path, l_base_commit)
        if rc_base == sbeConstants.SUCCESS_EXIT:
            print "  Base Commit checkout [ OK ]"
            print "\n"
        else:
            print "  Base Commit checkout [ FAILED ] rc = ",rc_base
            exit_main(rc_base)

    #-------------------------------------
    # Cherry pick the user supplied refs
    #-------------------------------------
    def ppeUserCherryPick():
        print "\n  [ PPE : Applying User Patches on PPE Repo ] "
        print "  ++++++++++++++++++++++++++++++++++++++"
        print " Patch list : ", patch_refs.split(",")
        print "  ++++++++++++++++++++++++++++++++++++++"

        # Get from remote and then cherry pick
        for refs_id in patch_refs.split(","):
            print "\n  [ Applying Patch : %s ]\n " %refs_id
            rc_cherry_code = ppeUtility.utilCherryPick(refs_id)
            if rc_cherry_code == sbeConstants.PPE_CHERRY_ERROR_EXIT:
                print "\n  *** Cherry-pick of %s failed  *** " % refs_id
                print " Aborting Build process.... [ ABORT ] rc = %s" % rc_cherry_code
                print " [ * Please recheck the patch refs data provided ]"
                exit_main(1)
            else:
                print "\n    --> Cherry picked [ OK ] for the Patch ",refs_id

        print "\n "
        print "   ------------------------------------------------------- "
        print "   ** All patches Cherry picked and applied successfully **"
        print "   ------------------------------------------------------- "

    #-------------------------------------
    # Cherry pick the commits in the PPE model
    #-------------------------------------
    def ppeFileCherryPick():
        l_patch_found = False
        # Check out commits under model
        if build_path == "None":
            l_model_build_path= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB + "/src/tools/models/" + model_name
        else:
            l_model_build_path= build_path + "/" +sbeConstants.GIT_PPE_REPO_SB + "/src/tools/models/" + model_name

        # Loop through the dir files
        if os.path.exists(l_model_build_path):
            print "\n"
            print "   ++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            print "   [ PPE Model Cherry pick if any ... applying patches ]"
            print "   ++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            #list=os.listdir(l_model_build_path)
            # Open base file
            l_model_ppe_base = l_model_build_path +"/base"
            with open(l_model_ppe_base) as fp:
                for line in fp:
                    # If new line or empty
                    if not line.strip(): continue
                    # Checkout the base here
                    if "base:" in line.rstrip():
                        print "  ", line.rstrip()
                        l_base_commit = line[5:].rstrip()
                        print "\n"
                        print "    --------------------------"
                        print "   [ PPE Model base Check-out ]"
                        print "    --------------------------"
                        rc_base = ppeUtility.utilCheckOutCommit(model_name, build_path, l_base_commit)
                        if rc_base == sbeConstants.SUCCESS_EXIT:
                            print "  Base Commit checkout [ OK ]"
                            print "\n"
                        else:
                            print "  Base Commit checkout [ FAILED ] rc = ",rc_base
                            exit_main(rc_base)


                    # Loop through the rest
                    if not "cp:" in line.strip():  continue

                    if "cp:" in line:
                        # Patch found
                        l_patch_found = True

                    print "  Reading Line : ", line.strip()
                    #patch_id=patch_id.split("cp:")
                    patch_id=line[3:].rstrip()
                    print "  Cherry-Pick : ", patch_id
                    rc_cherry_code = ppeUtility.utilCherryPick(patch_id)
                    if rc_cherry_code == sbeConstants.PPE_CHERRY_ERROR_EXIT:
                        # Reset the what had been done so far
                        # os.system("git reset HEAD --hard")
                        print "  ** Cherry-pick of %s failed " % patch_id
                        exit_main(1)

            print " \n"
            print "   ++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            print "   ** All PPE Patches applied successfully ** "
            print "   ++++++++++++++++++++++++++++++++++++++++++++++++++++++"
            print " \n"
        else:
            print "  Model doesn't exist in PPE.. Continue with Master branch "

        # If patch found ?
        if l_patch_found == False:
            print "  * Patches Not Found .."

    #-------------------------------------------------------------
    #  From commit to change id extract
    #-------------------------------------------------------------
    def git_comitId_to_changeId():
        print "\n  [ commit -- to --> Change Id ]"
        l_changeId=sbeGitUtility.utilGetChangeID(commit_name)

        if l_changeId == sbeConstants.ERROR_PREP_CHANGE_FAILED:
            print "  [ ERROR ] Change log id info not found - FAILED "
            exit_main(2)
        elif l_changeId == sbeConstants.ERROR_PREP_GIT_FAILED:
            print "  [ ERROR ] git command - FAILED "
            exit_main(2)
        else:
            #print " ", l_changeId.strip('Change-Id: ')
            print " ", l_changeId
            print "  * Found change log id info [ OK ] "

    #-------------------------------------------------------------
    #  From change id to commit id extract
    #-------------------------------------------------------------
    def git_changeId_to_commitid():
        print "\n  [ Change Id -- to --> Commit Id ]"
        #l_comId=sbeGitUtility.utilGetCommitID(l_changeId.strip('Change-Id: '))
        l_comId=sbeGitUtility.utilGetCommitID(commit_name)
        if l_comId == sbeConstants.ERROR_PREP_COM_FAILED:
            print "  [ ERROR ] commit log id info not found - FAILED "
            exit_main(2)
        elif l_comId == sbeConstants.ERROR_PREP_GIT_FAILED:
            print "  [ ERROR ] git command - FAILED "
            exit_main(2)
        else:
            #print " ", l_comId.strip('commit ')
            print " ", l_comId
            print "  * Found commit log id info [ OK ] "

    #----------------------------
    # Do stuff here
    #----------------------------
    #sandbox_build()

    #--------------------------------------
    # HACK for ENV build  and model EMPTY
    #--------------------------------------
    if model_name == "None":
        change_ppe_dir()
        if not commit_name == "None":
            ret_base_code = ppeUserbaseCheckout()
            if ret_base_code == sbeConstants.PPE_BASE_ERROR_EXIT:
                print "  * Base commit checkout failed !!! rc =", sbeConstants.PPE_BASE_ERROR_EXIT
                exit_main(sbeConstants.PPE_BASE_ERROR_EXIT)

        if not patch_refs == "None":
            ppeUserCherryPick()

        print "  !!!! ENV loaded for build run for developement !!!! "
        ret_code_hack = sbeUtility.utilBuildPPE(build_type, build_path, model_name, env_patch)
        if not ret_code_hack == sbeConstants.SUCCESS_EXIT:
            exit_main(ret_code_hack)
        else:
            exit_main(0)

    #----------------------------
    #  if Model name is EMPTY
    #----------------------------
    #if model_name == "None":
    #    change_ppe_dir()
    #    if not commit_name == "None":
    #        ppeUserbaseCheckout()
    #    if not patch_refs == "None":
    #        ppeUserCherryPick()
    #    print "  !!!! PPE SIMICS TEST run build !!!! "
    #    ret_code_hack = sbeUtility.utilBuildPPE(build_type, build_path, model_name, env_patch)
    #    if not ret_code_hack == sbeConstants.SUCCESS_EXIT:
    #        exit_main(ret_code_hack)
    #    else:
    #        exit_main(0)

    #----------------------------------
    # For creating PPE model - Offline
    #----------------------------------
    if not create_ppe_model == "None":
        ppeModelCreate()

    #----------------------------
    # PPE model check and prep
    #----------------------------
    # User has not provided the patch refs, PPE model check
    if patch_refs == "None":
        rc_ppe_code = ppeModelExist()
        if rc_ppe_code == sbeConstants.SUCCESS_EXIT:
            print "   * Using base file entries as source of comits"
        elif rc_ppe_code == sbeConstants.PPE_MODEL_SKIP_EXIT:
            change_ppe_dir()
            os.system("git branch")

    change_ppe_dir()

    #-------------------------------------
    # If comit is not passed, look up the
    # PPE model and continue
    #-------------------------------------
    if commit_name == "None":
        #ppebaseCheckout()
        if patch_refs == "None":
           ppeFileCherryPick()
        else:
            ppeUserCherryPick()
    else:
        ret_base_code = ppeUserbaseCheckout()
        if ret_base_code == sbeConstants.PPE_BASE_ERROR_EXIT:
            print "  * Base commit checkout failed !!! rc =", sbeConstants.PPE_BASE_ERROR_EXIT
            exit_main(sbeConstants.PPE_BASE_ERROR_EXIT)

        # Fixes for ONLY master commit and not patches
        if not patch_refs == "None":
            ppeUserCherryPick()

    #-------------------------------------
    # Exit gracefully and return the control
    # to the main script
    #-------------------------------------
    exit_main(0)

    #-------------------------------------
    #  End of this main .. exiting
    #-------------------------------------

if __name__=="__main__":
    main()
