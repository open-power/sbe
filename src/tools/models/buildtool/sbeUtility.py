#!usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/sbeUtility.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017
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
###########################################################
#    @file  sbeUtility.py
#    @brief Utilility Module to support building SBE image
#
#    @author   George Keishing
#
#    @date  Nov 23, 2015
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     11/12/15     Initial create
###########################################################

#-------------------------
# Imports
#-------------------------
import os, sys
import time
import os.path
import subprocess
import shutil
from subprocess import Popen, PIPE
from os.path import expanduser    # for getting $HOME PATH
# Local imports
import sbeConstants as sbeConstants

# Global vars
# Action file to write if found guilty during build
g_action_file = expanduser("~") + '/GIT_REPO_SB/logs/buildActionHooks'

# log path
timestr = time.strftime("%Y%m%d-%H%M%S")
g_ppeLog = expanduser("~") + '/GIT_REPO_SB/logs/buildPPE_out_' + timestr

##########################################################################
# Function : utilCheckTokens
#
# @param   None
#
# @brief   Check if the user work ENV has git setup or not
#
##########################################################################
def utilCheckTokens():
    awd_token = False
    austin_token = False
    token_cmd='tokens'
    token_out = subprocess.Popen(token_cmd, shell=True, stdout=subprocess.PIPE)
    for line in token_out.stdout:
        line = line.strip()
        if not line: continue
        if '@awd.austin.ibm.com' in line:
            print "  ", line
            awd_token = True
        if '@austin.ibm.com' in line:
            print "  ", line
            austin_token = True

    # Check if the AWD and Austin token exist
    if (awd_token != False ) and ( austin_token != False):
        return sbeConstants.SUCCESS_EXIT
    else:
        # should not reach here
        return sbeConstants.ERROR_PREP_TOKEN_SETUP_FAILED

##########################################################################
# Function : utilCheckSetup
#
# @param   None
#
# @brief   Check if the user work ENV has git setup or not
#
##########################################################################
def utilCheckSetup():
    ENV_SSH = expanduser("~")  + '/.ssh/config'
    #print ENV_SSH
    ssh_file = open(ENV_SSH)
    scanlines=ssh_file.readlines()
    for line in scanlines:
        if 'hw.gerrit' in line:
        #if 'gerrit-server' in line:
            print "  ", line
            ssh_file.close()
            return sbeConstants.SUCCESS_EXIT

    ssh_file.close()
    # should not reach here
    return sbeConstants.ERROR_PREP_GIT_SETUP_FAILED

##########################################################################
# Function : utilCleanup
#
# @param   i_build_path   : user supplied command line input path
#
# @brief   cleanup sandbox
#
##########################################################################
def utilCleanup(i_build_path):
    # default
    rc = sbeConstants.SUCCESS_EXIT

    if i_build_path == "None":
        l_home_ekb_path= expanduser("~") + "/" +sbeConstants.GIT_EKB_REPO_SB
        l_home_ppe_path= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB

        # Print what is to be cleaned
        print "  EKB path = %s" % l_home_ekb_path
        print "  PPE path = %s" % l_home_ppe_path

        if os.path.exists(l_home_ekb_path):
            shutil.rmtree(l_home_ekb_path)

        if os.path.exists(l_home_ppe_path):
            shutil.rmtree(l_home_ppe_path)

    # Make sure the same path doesn't have old stale copy
    if i_build_path == "None":
       print ""
    else:
       l_user_ppe_home = i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB
       l_user_ekb_home = i_build_path + "/" +sbeConstants.GIT_EKB_REPO_SB
       print "  User PPE path = %s" % l_user_ppe_home
       print "  User EKB path = %s" % l_user_ekb_home
       if os.path.exists(l_user_ppe_home):
            shutil.rmtree(l_user_ppe_home)
       if os.path.exists(l_user_ekb_home):
            shutil.rmtree(l_user_ekb_home)

    # Create the directory logs for logging output
    if i_build_path == "None":
        l_ppeLog= expanduser("~") + '/GIT_REPO_SB/logs/'
    else:
        l_ppeLog= i_build_path + '/GIT_REPO_SB/logs/'

    if os.path.exists(l_ppeLog):
        l_ppeLog=  l_ppeLog + 'buildActionHooks'
        # If Action file exist
        if os.path.exists(l_ppeLog):
            print "  Removing Action Build Hooks = %s" % l_ppeLog
            #shutil.rmtree(l_ppeLog)
            # Remove only the Action file
            rc = os.remove(l_ppeLog)
    else:
        # Create the logs/ dir if not exisit
        rc = os.makedirs(l_ppeLog)

    # Rc codes
    if rc:
        return rc
    else:
        return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilCreateSandbox_ekb
#
# @param   None
#
# @brief   Create ekb sandbox
#
##########################################################################
def utilCreateSandbox_ekb(i_build_path):

    if i_build_path == "None":
        l_home_path_ekb= expanduser("~") + "/" +sbeConstants.GIT_EKB_REPO_SB
    else:
        l_home_path_ekb= i_build_path + "/" +sbeConstants.GIT_EKB_REPO_SB

    print "  ekb Sandbox Path = %s" % l_home_path_ekb
    if not os.path.isdir(l_home_path_ekb):
       os.makedirs(l_home_path_ekb)

    os.chdir(l_home_path_ekb)
    ekb = Popen(sbeConstants.GIT_EKB_SETUP_CMD , shell=True, stdout=PIPE, stderr=PIPE)
    out, err = ekb.communicate()
    #print out.rstrip(), err.rstrip()
    # Due to git re-init this warning errors may be thrown .. ignore
    if ekb.returncode == 128:
        return sbeConstants.SUCCESS_EXIT
    else:
        return ekb.returncode

##########################################################################
# Function : utilCreateSandbox_ppe
#
# @param   i_build_path   : user supplied command line input path
#
# @brief   Create ppe sandbox
#
##########################################################################
def utilCreateSandbox_ppe(i_build_path):

    if i_build_path == "None":
        l_home_path_ppe= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
    else:
        l_home_path_ppe= i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB

    print "  ppe Sandbox Path = %s" % l_home_path_ppe
    if not os.path.isdir(l_home_path_ppe):
           os.makedirs(l_home_path_ppe)

    os.chdir(l_home_path_ppe)
    ppe = Popen(sbeConstants.GIT_PPE_SETUP_CMD , shell=True, stdout=PIPE, stderr=PIPE)
    out_ekb, err_ekb = ppe.communicate()
    #print out_ekb.rstrip(), err_ekb.rstrip()

    # Due to git re-init this warning errors may be thrown .. ignore
    if ppe.returncode == 128:
        return sbeConstants.SUCCESS_EXIT
    else:
        return ppe.returncode

##########################################################################
# Function : utilLoadEnv
#
# @param   i_model      : user supplied command line input model id
# @param   i_commit     : user supplied command line input commit id
# @param   i_build_type : user supplied command line input build type
#                         Default : Auto ( Build and trigger CI)
#                         Manual  : Compilation ONLY
# @param   i_build_path : user supplied command line input path
# @param   i_env_patch  : user supplied command line input export patch
#
# @brief   Load ekb work on ENV . Write Hooks to env.bash
#
##########################################################################
def utilLoadEnv(i_model, i_commit, i_patch, i_build_type, i_build_path, i_env_patch):

     if i_build_path == "None":
         l_home_path_ekb= expanduser("~") + "/" +sbeConstants.GIT_EKB_REPO_SB
         l_action_file = g_action_file + "_" + i_model
     else:
         l_home_path_ekb= i_build_path + "/" +sbeConstants.GIT_EKB_REPO_SB
         l_action_file = i_build_path + '/GIT_REPO_SB/logs/buildActionHooks' + "_" + i_model

     os.chdir(l_home_path_ekb)

     # Now once we load the bash env here the control goes away from the main
     # until returned, now append the command needed and spun of another script
     # sbeContinue hook method. The only way to mix bash and python right now.
     ENV_BASH_PATH=l_home_path_ekb +'/env.bash'

     # Probably safe to make a copy of it
     l_cmd ='cp ' + ENV_BASH_PATH +'  ' + l_home_path_ekb+'/env.bash_backup'
     os.system( l_cmd )

     # Now open the file and fiddle without breaking it
     file = open(ENV_BASH_PATH,'a')

     print "  Executing : git reset --hard : Remove untracked files from the current directory\n"
     file.write("git reset --hard\n")
     print "  Executing : ./ekb model fetch\n"
     file.write("./ekb model fetch\n")

     # Give a sync time to fetch the model
     file.write("sleep 5 \n")

     # Check if this Model nameed directory exist or not
     l_ekb_model_exist=l_home_path_ekb + '/models/' + i_model

     model_exist='[ -d ' + l_ekb_model_exist + ' ] && echo "  ' + l_ekb_model_exist + '  exists" || exit 1 '
     file.write(model_exist)
     file.write("\n")

     cmd_string="./ekb model checkout --model="+i_model
     print "  Executing : ./ekb model checkout --model= %s\n" % i_model
     file.write(cmd_string)
     file.write("\n")
     file.write("sleep 10 \n")
     file.write("\n")

     # Find out from where the main script triggered,
     # yank out the last word and replace with the hook script
     l_string=__file__
     l_string=l_string.rsplit('/', 1)[0]
     l_string= l_string +'/sbeContinue.py'
     hooks_cmd="python " + l_string + " -m " + i_model + " -c " + i_commit + " -i " + i_patch + " -b " + i_build_type + " -p " + i_build_path + " -e " + i_env_patch + " || exit 1"
     print "  hooks appended to env.bash : ", hooks_cmd

     # We want to write to refer when commit or compilation breaks
     # +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     print "  Writing Action file to %s" % l_action_file
     a_file = open(l_action_file,'w')
     a_file.write(hooks_cmd)
     a_file.close()
     # +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

     file.write(hooks_cmd)
     file.write("\n")

     # make the script exit
     file.write("\n")
     file.write("exit")
     file.close()

     # Work on ekb and load the env.bash
     rc = os.system("./ekb workon")

     # Update PATH

     #ROOTDIR/tools:ROOTDIR/tools/test
     #/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin
     #os.environ['PATH'] = ROOTDIR +'/tools' ':' + ROOTDIR+ '/tools/test'\
     #                     + ':/usr/lib64/qt-3.3/bin:/usr/local/bin:/bin:/usr/bin:'\
     #                     + '/usr/local/sbin:/usr/sbin:/sbin:/opt/ibm/cmvc/bin:'\
     #                     + '/afs/austin.ibm.com/projects/esw/bin:/usr/ode/bin:'\
     #                     + '/usr/lpp/cmvc/bin'
     #os.system("echo $PATH")

     # ROOTDIR/output/lib/:/tools/ecmd/ver-14-1/x86_64/lib
     #os.environ['LD_LIBRARY_PATH'] = ROOTDIR +'/output/lib/' + ':' + '/tools/ecmd/ver-14-1/x86_64/lib'
     #os.system("echo $LD_LIBRARY_PATH")


     # Read the env file and execute it in loop to simulate loading the env

     #print "  DEBUG Exit code from env.bash EKB === %d" % rc
     if rc:
         return rc
     else:
         return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilCopyModel
#
# @param   i_build_path   : user supplied command line input path
#
# @brief   Copy EKB Model files to PPE import
#
##########################################################################
def utilCopyModel(i_build_path):
    # EKB and PPE path
    if i_build_path == "None":
        l_ekbPath = expanduser("~") + "/" +sbeConstants.GIT_EKB_REPO_SB +'/'
        l_ppePath = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'
        l_ppeModelLog = expanduser("~") + '/GIT_REPO_SB/logs/buildModel_out.log'
    else:
        l_ekbPath = i_build_path + "/" +sbeConstants.GIT_EKB_REPO_SB +'/'
        l_ppePath = i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'
        l_ppeModelLog = i_build_path + '/GIT_REPO_SB/logs/buildModel_out.log'

    l_ppeImportPath =  l_ppePath + "src/import/"

    # Good practice to keep a count how many files did we copied
    file_count = 0

    l_cmdStr = "find " + l_ppeImportPath + "chips" + " -type f -follow -print"
    cmd = subprocess.Popen(l_cmdStr, shell=True, stdout=subprocess.PIPE)
    for line in cmd.stdout:
        line = line.strip()
        line = line.replace(l_ppeImportPath,"");
        srcPath = l_ekbPath + line;
        destPath = l_ppeImportPath + line;
        #print "Copying %s --> %s\n" % (srcPath,destPath)
        if( os.path.isfile(srcPath.strip()) ):
            file_count+=1 # increment
            cmd = "cp " + srcPath + " " + destPath;
            rc = os.system( cmd )
            if ( rc ):
                 print "  [ ERROR ] Copying file %s: %d "%( cmd, rc )
                 return rc

    print "  [ %s ] Files copied from EKB to PPE" % file_count
    return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilGitStatus
#
# @param   i_build_path   : user supplied command line input path
#
# @brief   Get the git changes status to confirm the copied files
#
##########################################################################
def utilGitStatus(i_build_path):
    if i_build_path == "None":
        l_ppePath= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'
    else:
        l_ppePath= i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'

    os.chdir(l_ppePath)

    # execute the command
    l_cmd = 'git status'
    git_cmd = subprocess.Popen(l_cmd, stdout=subprocess.PIPE, shell=True)
    for line in git_cmd.stdout:
        line = line.strip()
        if 'modified:' in line:
            print line

    # fail or pass doesnt matter, this just for info
    return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilCompileManual
#
# @param   i_ppeLog     : Default logging path
# @param   i_build_path : user supplied command line input path
# @param   i_env        : user supplied ENV patch string flag
#
# @brief    Compile step by step
#
##########################################################################
def utilCompileManual(i_ppeLog, i_build_path, i_env):

    if i_build_path == "None":
        l_home_path_ppe = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
    else:
        l_home_path_ppe = i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'

    # Go to sbe/image/ folder and compile
    l_sbe_img =l_home_path_ppe
    os.chdir(l_sbe_img)

    print "  * Changing directory to : ", l_sbe_img

    #execute make all
    if i_env:
        l_make=i_env + "make all > " + i_ppeLog + "  2>&1"
    else:
        l_make="make all > " + i_ppeLog + "  2>&1"

    print "  * Executing : ", l_make
    print "  * Code Compilation in progress..."
    rc = os.system("source ./env.bash && bash -c '"+l_make+" && exit'")

    # Check if the shell returns error
    if rc:
        # intercept the rc and override with user define err
        return sbeConstants.BUILD_MANUAL_FAILED
    else:
        return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilBuildPPE
#
# @param   i_build_type : user supplied command line input build type
#                         Default : Auto ( Build and trigger CI)
#                         Manual  : Compilation ONLY
# @param   i_build_path : user supplied command line input path
# @param   i_model_name : user supplied command line input model name
# @param   i_env_patch  : user specific work around ENV commands
#
# @brief    Trigger build for PPE git repo
#
##########################################################################
def utilBuildPPE(i_build_type, i_build_path, i_model_name , i_env_patch):
    if i_build_path == "None":
        l_ppePath= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'
        l_ppeLog = g_ppeLog + "_" + i_model_name +".log"
    else:
        l_ppePath = i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB +'/'
        l_ppeLog = i_build_path + '/GIT_REPO_SB/logs/buildPPE_out_' + timestr + "_" + i_model_name +".log"

    # Change directory to PPE Repo path
    os.chdir(l_ppePath)

    # execute the command
    print "   --> in %s" % l_ppePath
    print "\n"
    print "  --------------------------------------------"
    print "  ** In PPE Repo : which Current GIT branch ? "
    print "  --------------------------------------------"
    os.system("git branch")
    print "\n"

    # New path with model
    print "  [ Logging build output : %s ]\n" % l_ppeLog


    env_load_cmd =""
    print "  * If any ENV work around ? ", i_env_patch.split(",")
    for env_id in i_env_patch.split(","):
        if env_id:
             env_load_cmd += "export " + env_id + " ; "

     # Hack running user ENV for simics build
    if not i_env_patch == "None":
        if i_build_type == "Auto":
            l_cmd = env_load_cmd + "./src/test/framework/build-script"
        else:
            l_cmd = env_load_cmd + " "
    else:
        if i_build_type == "Auto":
            l_cmd = "./src/test/framework/build-script"
        else:
            l_cmd = ""

    # Compile build strategy Auto CI / Manual
    if i_build_type == "Auto":
        print "  * Executing Build script :", l_cmd
        print "\n   *** Please Wait : Build and NIMBUS simics Test in progress ***"
        rc = os.system("source ./env.bash && bash -c '%s > %s 2>&1 && exit'" % (l_cmd,l_ppeLog))
        print "  [  ** CI build and Test Result  ** ]"
    else:
        print "  * Executing Build : make all"
        rc = utilCompileManual(l_ppeLog, i_build_path, l_cmd)

    # Return the rc codes for failure/success
    if rc == sbeConstants.BUILD_MANUAL_FAILED:
        return rc
    elif rc:
        return sbeConstants.BUILD_CI_FAILED
    else:
        return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilValidateHookFile
#
# @param   i_hook_path  : Foot print log of last saved off hooks
# @param   i_model      : user supplied command line input model id
# @param   i_commit     : user supplied command line input commit id
# @param   i_patch      : user supplied command line input patch refs
# @param   i_build_path : user supplied command line input path
# @param   i_build_type : user supplied command line input build type
#                         Default : Auto ( Build and trigger CI)
#                         Manual  : Compilation ONLY
#
# @brief   Validate the input signature vs action file
#
##########################################################################
def utilValidateHookFile(i_hook_path, i_model, i_commit, i_patch, i_build_path, i_build_type, i_env_patch):
    # Open the file and walk through
    # python <PATH>/sbeContinue.py -m n10_e9025_tp024_soa_sc_u289_01 -c None -b manual -p None

    # Local vars for check match
    l_model  = " -m " + i_model
    l_commit = " -c " + i_commit
    l_patch  = " -i " + i_patch
    l_path   = " -p " + i_build_path
    l_build  = " -b " + i_build_type
    l_env  = " -e " + i_env_patch

    act_file = open(i_hook_path,'r')
    scanlines=act_file.readlines()
    for line in scanlines:
        if l_model in line:
            if l_commit in line:
              if l_patch in line:
                if l_path in line:
                    if l_build in line:
                       if l_env in line:
                           # All parameters are exactly matching
                           act_file.close()
                           return sbeConstants.SUCCESS_EXIT

    act_file.close()

    return sbeConstants.VALIDATE_EXIT

##########################################################################
# Function : utilBypassCheck
#
# @param   i_build_path : Foot print log of last failing build comand
#
# @brief   Trigger the build by passing the main script
#
##########################################################################
def utilBypassCheck(i_build_path):
    # Check if this file exist
    if os.path.exists(i_build_path):
        print "  [  Build action file exist ]"
        print "  [ FileName ] : %s " % i_build_path
        return sbeConstants.BYPASS_EXIT
    else:
        return sbeConstants.SUCCESS_EXIT


##########################################################################
# Function : utilWriteActionFile
#
# @param   i_action_file : Create template action file for user to add
#                          command to execute as part of build
# @param   i_model_name  : User input model string
#
# @brief   Trigger the build by passing the main script plus patch action
#          file needed rules
#
##########################################################################
def utilWriteActionFile(i_action_file, i_model_name):
    # Check if this file exist
    # logs/PATCH_ACTION_TEMPLATE_n10_e9027_tp026_soa_sc_u285_01
    print "  Patch Action : ", i_action_file
    if not os.path.exists(i_action_file):
        open(i_action_file, 'a').close()

    return sbeConstants.SUCCESS_EXIT


##########################################################################
# Function : utilVerifyPPEmodel
#
# @param   i_model_name  : User input model string
#
# @brief   Verify of the model name exist or not in PPE Repo
#
##########################################################################
def utilVerifyPPEmodel(i_model_name):
    print " Verify PPE Model existence "

##########################################################################
# Function : utilEkbModelExist
#
# @param   i_model_name   : user supplied command line input model name
# @param   i_ekb_path     : user supplied command line input EKB Repo
#
# @brief   Check if this Model existing in PPE, Skip if Commit is "None"
#
##########################################################################
def utilEkbModelExist(i_model_name, i_ekb_path ):

    # Use default
    #print "  Model Name      : %s " % (i_model_name)

    l_model_ekb_path = i_ekb_path + "/models/" + i_model_name
    #print "  EKB Model Path  : %s " % (l_model_ekb_path)

    if os.path.isdir(l_model_ekb_path):
        return sbeConstants.SUCCESS_EXIT
    else:
        return sbeConstants.EKB_MODEL_NOT_EXIST


##########################################################################
# Function : utilFilesInDirExist
#
# @param   i_folder_name   : Path to the dir absolute path
#
# @brief   Check if there is files in a folder or not
#
##########################################################################
def utilFilesInDirExist(i_folder_path ):
    for dirpath, dirnames, files in os.walk(i_folder_path):
        if files:
            #print dirpath, 'has files'
            return sbeConstants.FILES_EXIST
        if not files:
            #print dirpath, 'is empty'
            return sbeConstants.DIR_EMPTY

##########################################################################
# Function : utilshow_buildPPEObj
#
# @param   i_build_path   : user supplied command line input path
#
# @brief   List the ppe/images/ after compilation for proof
#
##########################################################################
def utilshowBuildPPEObj(i_build_path):
        # Just show the ppe/sbe/obj/ dir listing
        print "\n  [ Listing PPE obj build directory files ]"
        if i_build_path == "None":
            l_home_build_path = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB  + "/images"
        else:
            l_home_build_path = i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB + "/images"

        l_listCmd = "ls -lart " + l_home_build_path
        print "  PPE obj : ", l_home_build_path
        os.system(l_listCmd)
