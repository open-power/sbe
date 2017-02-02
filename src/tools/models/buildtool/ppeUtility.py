#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/ppeUtility.py $
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
#    @file     ppeUtility.py
#    @brief    Utilility Module to support model for PPE
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
import os
import sys
import time
import shutil
import os.path
import subprocess
from subprocess import Popen, PIPE
from os.path import expanduser    # for getting $HOME PATH
# Local imports
import sbeConstants as sbeConstants

##########################################################################
# Function : utilppeModelCreate
#
# @param   i_model_name   : user supplied command line input model name
# @param   i_build_path   : user supplied command line input buil Repo path
# @param   i_commit_name  : user supplied command line input commit id
#
# @brief   Check if this Model existing in PPE and create
#
##########################################################################
def utilppeModelCreate(i_model_name, i_build_path, i_commit_name):

    # Use default if user hasn't provided one
    if i_build_path == "None":
        l_model_ppe_path= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB + "/models/" + i_model_name
    else:
        l_model_ppe_path= i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB + "/models/" + i_model_name

    print "  Create Model Name      : %s " % (i_model_name)
    print "  Create Commit tag Name : %s " % (i_commit_name)
    print "  Create PPE Model Path  : %s " % (l_model_ppe_path)

    if not os.path.isdir(l_model_ppe_path):
        print "  User provided Model doesn't exist in PPE Repo"
        print "  Creating model in PPE Repo"
        os.makedirs(l_model_ppe_path)
        # Create an empty base file under the model
        l_base_path = l_model_ppe_path + "/base"
        print "  Creating base file : " , l_base_path
        file = open(l_base_path, 'a')
        file.write(i_commit_name)
        file.close()

        # create models/<name>/include and include/.empty
        l_model_ppe_include =l_model_ppe_path + "/include/"
        os.makedirs(l_model_ppe_include)
        utilCreateFile(l_model_ppe_include,".empty")

##########################################################################
# Function : utilppeModelExist
#
# @param   i_model_name   : user supplied command line input model name
# @param   i_build_path   : user supplied command line input buil Repo path
# @param   i_commit_name  : user supplied command line input commit id
#
# @brief   Check if this Model existing in PPE, Skip if Commit is "None"
#
##########################################################################
def utilppeModelExist(i_model_name, i_build_path, i_commit_name):

    # Use default if user hasn't provided one
    if i_build_path == "None":
        l_model_ppe_path= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB + "/src/tools/models/" + i_model_name
    else:
        l_model_ppe_path= i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB + "/src/tools/models/" + i_model_name

    print "  Model Name      : %s " % (i_model_name)
    print "  Commit tag Name : %s " % (i_commit_name)
    print "  PPE Model Path  : %s " % (l_model_ppe_path)

    if os.path.isdir(l_model_ppe_path):
        #print "  Model exist in PPE Repo "
        return sbeConstants.SUCCESS_EXIT

    if not i_commit_name == "None":
        #print "  Skipping Model check in PPE since it's a user explicit request"
        return sbeConstants.PPE_MODEL_SKIP_EXIT

    return sbeConstants.PPE_MODEL_NOT_EXIST

##########################################################################
# Function : utilCreateFile
#
# @param   i_build_path   : user supplied command line input buil Repo path
# @param   i_file_name    : user supplied input file names
#
# @brief   Create the file/ID file under model directoty
#
##########################################################################
def utilCreateFile(i_build_path, i_file_name):
    l_file_path= i_build_path + i_file_name
    print "  Creating file under Model dir : ", l_file_path
    open(l_file_path, 'a').close()

##########################################################################
# Function : utilCheckOutCommit
#
# @param   i_model_name   : user supplied command line input model name
# @param   i_build_path   : user supplied command line input buil Repo path
# @param   i_commit_name  : user supplied command line input commit id
#
# @brief   Checkout the commit id user passed or in the model base
#
##########################################################################
def utilCheckOutCommit(i_model_name, i_build_path, i_commit_name):
    #print "  Model Name      : %s " % (i_model_name)
    #print "  Commit tag Name : %s " % (i_commit_name)
    #print "  PPE Model Path  : %s " % (i_build_path)

    if i_build_path == "None":
        l_model_ppe_base= expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB + "/src/tools/models/" + i_model_name +"/base"
    else:
        l_model_ppe_base=i_build_path + "/" +sbeConstants.GIT_PPE_REPO_SB + "/src/tools/models/" + i_model_name +"/base"

    # Check if the base exisit
    if not os.path.exists(l_model_ppe_base):
        #print "  Model doesn't exist .. skipping base check-out"
        return sbeConstants.SUCCESS_EXIT
    #else:
        # DEBUG - Saved off a copy of the base file to HOME
        # Since once we checkout the base , the model wouldn't be viewable
        #shutil.copy(l_model_ppe_base,l_model_copy_base)

    # Load base model file and extract the commit base id
    #print "  Opening base file : ",l_model_ppe_base
    #with open(l_model_ppe_base) as fp:
    #    for line in fp:
    #        if "base:" in line.rstrip():
    #            #l_base_git_id = line.split("base:")
    #            l_base_git_id = line[5:].rstrip()
    #            print "  Base commit id : ", l_base_git_id

    # Git checkout base
    #l_gitCmd='git checkout ' + l_base_git_id + ' -b test_build'

    l_gitCmd='git checkout ' + i_commit_name + ' -b test_build'
    print "  Executing : ", l_gitCmd
    #p_out = subprocess.Popen(l_gitCmd, stdout=subprocess.PIPE, shell=True)
    rc = os.system(l_gitCmd)

    # Return error if there is
    if rc :
        print "  [ ERROR - PPE UTIL ] Base checkout failed  rc = ",rc
        # Intercept and send our own rc
        return 1

    # Just for showing which branch we are in
    l_gitbranch='git branch'
    p_checkout_out = subprocess.Popen(l_gitbranch, stdout=subprocess.PIPE, shell=True)
    for line in p_checkout_out.stdout:
        line = line.strip()
        print "   " ,line

    # Allow time to switch to new branch
    time.sleep(3)

    return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilCherryPick
#
# @param   i_refs_name  : user supplied command line input refs id
#
# @brief   Cherry pick the ref patch ids
#
##########################################################################
def utilCherryPick(i_refs_name):
    print "  * Cherry Pick ref id : ", i_refs_name

    # Fetch
    l_fetch = "git fetch ssh://gerrit-server/hw/ppe " + i_refs_name

    #l_gitCmd="git cherry-pick -x  " + i_refs_name
    l_gitCmd= l_fetch + " && " + "git cherry-pick FETCH_HEAD"
    print "  Executing : ", l_gitCmd
    #p_out = subprocess.Popen(l_gitCmd, stdout=subprocess.PIPE, shell=True)
    rc = os.system(l_gitCmd)

    # Reset the HEAD if cherry pick failed
    if rc:
        print "  * Patch failed .. Cleaning up Patches Applied : git reset HEAD --hard "
        os.system("git reset HEAD --hard")
        return sbeConstants.PPE_CHERRY_ERROR_EXIT
    else:
        return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilScanModelCommits
#
# @param   i_model_name   : user supplied command line input model name
# @param   i_build_path   : user supplied command line input buil Repo path
# @param   i_commit_name  : user supplied command line input commit id
#
# @brief   Prepare a list of commits to be cherry pick from the PPE model
#
##########################################################################
def utilScanModelCommits(i_model_name, i_build_path):
    print "  Listing out which commits needs to be Cherry pick "

