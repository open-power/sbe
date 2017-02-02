#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/sbeGitUtility.py $
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
#    @file     sbeGitUtility.py
#    @brief    Git Utilility Module to support building SBE image
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
# Function : utilGitShowCommitFiles
#
# @param   i_commitId   : user supplied command line input commit id
# @param   i_ppe_path   : user supplied command line input path
# 
# @brief   Check the files associated with this commit id
#
##########################################################################
def utilGitShowCommitFiles(i_commitId, i_ppe_path):

    if i_commitId == "None":
        return sbeConstants.SUCCESS_EXIT

    if i_ppe_path == "None":
        l_home_ppe_path = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
    else:
        l_home_ppe_path = i_ppe_path + "/" +sbeConstants.GIT_PPE_REPO_SB

    os.chdir(l_home_ppe_path)
    print "   --> ", l_home_ppe_path

    # Quickly check if this commit id is valid or not
    is_valid = "git cat-file -t " + i_commitId
    p_valid = subprocess.Popen(is_valid, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, shell=True)
    for line in p_valid.stdout:
            line = line.strip()
            if line == "commit":
                print "  * This Commit Validated "
            else:
                #There is an error - command has exited with non-zero code
                return sbeConstants.ERROR_GIT_CHANGEID_FAILED

    # Git command to list only the files associatede with this commit
    l_gitCmd='git show --pretty="format:" --name-only  ' + i_commitId
    print "  Executing : ", l_gitCmd

    p_out = subprocess.Popen(l_gitCmd, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, shell=True)
    for line in p_out.stdout:
        line = line.strip()
        print "   " ,line

    return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilGitShowCommitFiles
#
# @param   i_commitId   : user supplied command line input commit id
# @param   i_ppe_path   : user supplied command line input path
#
# @brief   Check the files associated with this commit id
#
##########################################################################
def utilGitCommitCheckout(i_commitId, i_ppe_path):

    if i_commitId == "None":
        return sbeConstants.SUCCESS_EXIT

    if i_ppe_path == "None":
        l_home_ppe_path = expanduser("~") + "/" +sbeConstants.GIT_PPE_REPO_SB
    else:
        l_home_ppe_path = i_ppe_path + "/" +sbeConstants.GIT_PPE_REPO_SB

    os.chdir(l_home_ppe_path)
    print "   --> ", l_home_ppe_path

    # Git command to list only the files associatede with this commit
    l_gitCmd='git checkout ' + i_commitId + ' -b test_build'
    print "  Executing : ", l_gitCmd
    p_out = subprocess.Popen(l_gitCmd, stdout=subprocess.PIPE, shell=True)

    # Allow time to sink in the checkout
    time.sleep(10)

    l_gitbranch='git branch'
    p_checkout_out = subprocess.Popen(l_gitbranch, stdout=subprocess.PIPE, shell=True)
    for line in p_checkout_out.stdout:
        line = line.strip()
        print "   " ,line

    # Allow time to switch to new branch
    time.sleep(3)

    return sbeConstants.SUCCESS_EXIT

##########################################################################
# Function : utilGetChangeID
#
# @param   i_num   : user supplied command line input commit id
#
# @brief   Get the git log id based on the  changed ID
#
##########################################################################
def utilGetChangeID(i_num):
    print "  commit",i_num
    l_found = False
    #data = []

    # execute the command
    l_cmd = 'git log ' + i_num

    p = subprocess.Popen(l_cmd, stdout=subprocess.PIPE, shell=True)
    #(output, err) = p.communicate()

    while p.poll() is None:
        line = p.stdout.readline()
        line = line.strip()
        if not line: continue

        if i_num in line:
            #print "Commit id marker found"
            l_found=True
            continue

        if 'Change-Id:' in line:
            if l_found:
                #print " ", line
                #data.append(line)
                break

    # Should n't come out without data
    if l_found:
        return line
    else:
        return sbeConstants.ERROR_PREP_CHANGE_FAILED

##########################################################################
# Function : utilGetCommitID
#
# @param   i_num   : user supplied command line input commit id
#
# @brief   Get the git log id based on the  changed ID
#
##########################################################################
def utilGetCommitID(i_num):
    print "  Change-Id: ",i_num
    l_found = False
    #data = []

    # execute the command
    l_cmd = 'git log'
    p = subprocess.Popen(l_cmd, stdout=subprocess.PIPE, shell=True)

    while p.poll() is None:
        line = p.stdout.readline()
        line = line.strip()
        if not line:
            continue

        # Saved off the previous entry which is commit id
        if line.startswith('commit'):
           l_saved_commit_id=line

        if line.startswith('Change-Id:'):
            # Check if the Id is found
            if i_num in line:
                l_found=True
                #print " ", l_saved_commit_id
                break
        else:
            continue

    # Should n't come out without data
    if l_found:
        return l_saved_commit_id
    else:
        return sbeConstants.ERROR_PREP_COM_FAILED


