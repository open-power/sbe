#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbeCmvcUtility.py $
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
###########################################################
#    @file  sbeCmvcUtility.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#             Sangeetha TS    <sangeet2@in.ibm.com>
#    @brief Utilility Module to support CMVC operation
#
#    Created on March 03, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     03/03/16     Initial create
###########################################################
'''

#-------------------------
# Imports
#-------------------------
import os, sys
import time
import os.path
import subprocess
import shutil
import hashlib
from subprocess import Popen, PIPE
from os.path import expanduser    # for getting $HOME PATH
import stat # for File permission op

# Libraries/utility funcs and user define const
import sbeCmvcConstants as errorcode


##########################################################################
# Function : utilCmvcChangeDir
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @brief   Create a directory
#
##########################################################################
def utilCmvcChangeDir(i_cmvcnum):
    l_home_path= expanduser("~") + "/" + errorcode.CMVC_DIR_CREATE + i_cmvcnum

    print "  Sandbox path\t: ",l_home_path
    cmd='mkdir -p  ' + l_home_path
    os.system(cmd)
    os.chdir(l_home_path)

##########################################################################
# Function :utilCmvcRepoPath
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @param   i_pathname : SBE repo Path location
#
# @param   i_filename : Files to be copied
#
# @brief   find the files in repo
#
##########################################################################
def utilCmvcRepoPath(i_pathname, i_cmvcnum, i_filename):
    l_home_path= expanduser("~") + "/" + errorcode.CMVC_DIR_CREATE + i_cmvcnum
    # Strip the last string from the file path input

    print "  Sandbox path\t: ",l_home_path
    print "  SBE Repo path\t: ",i_pathname

    if i_filename == "None":
        i_filename = errorcode.CMVC_FILE_LIST

    for l_filename in i_filename.split(","):

        # Find the files and copy
        l_sb_path   = utilFindFile(l_filename,l_home_path)
        l_repo_path = utilFindFile(l_filename,i_pathname)

        if l_sb_path is None :
           print "  ERROR: Checked out sandbox does not contain " + l_filename
           return errorcode.ERROR_CMVC_FILE_COPY
        if l_repo_path is None :
           print "  ERROR: File Not Found in SBE repo " + l_filename
           return errorcode.ERROR_CMVC_FILE_COPY

        cp_cmd = 'cp ' + l_repo_path + '  ' + l_sb_path
        rc=os.system(cp_cmd )
        if rc:
            return errorcode.ERROR_CMVC_FILE_COPY
        else:
            # validate the copied files via hashing
            l_src  = l_repo_path
            l_dest = l_sb_path
            hash_err = utilCheckFileHash(l_src,l_dest)
            if hash_err == errorcode.ERROR_HASH_CHECK:
                return hash_err

    # Returned success
    return errorcode.SUCCESS_EXIT

##########################################################################
# Function :utilFindFile
#
# @param   i_filename : File Name
#
# @param   i_path     : Directory to search in
#
# @brief   Finds a given file and returns the absoulte path
#
##########################################################################
def utilFindFile(i_filename, i_path):
    # preferred paths for files
    ppaths = [utilFind_ENV_string("SANDBOXROOT").rstrip('\n')+'/src', utilFind_ENV_string("SANDBOXROOT").rstrip('\n')+'/images']
    # all the occurances of the file in the repo
    filePaths = []
    for root, dirs, files in os.walk(i_path):
        if i_filename in files:
            filePaths.append(os.path.join(root, i_filename))
    for ppath in ppaths:
        for file in filePaths:
            if ppath in file:
                return file
    # if no preferred path found, just return the first found path
    return filePaths[0]

##########################################################################
# Function :utilFindFilePPE
#
# @param   i_filename : File Name
#
# @param   i_path     : Directory to search in
#
# @brief   Finds a given file ins PPE repo and returns the absoulte path
#
##########################################################################
def utilFindFilePPE(i_filename, i_path, i_sandbox_name):
    for root, dirs, files in os.walk(i_path):
        if i_filename in files:
            # Ignore the test sandbox files in the PPE
            if not i_sandbox_name in root:
                return os.path.join(root, i_filename)
            # Consider the path, if the repository is contanied in the sandbox
            else:
                if i_sandbox_name in i_path:
                    return os.path.join(root, i_filename)

##########################################################################
# Function :utilCmvcCheckout
#
# @param   i_filename : File Name
#
# @param   i_release  : Fips FW Release to checkout (ex: fips910 )
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @brief   Check out a given file
#
##########################################################################
def utilCmvcCheckout(i_filename, i_release, i_cmvcnum):
    print "  File Name\t: ",i_filename
    print "  Release\t: ",i_release

    # The file simics.tar,releaseNotes.html is not unique so provide
    # the relative path
    if i_filename == "simics.tar":
       i_filename = 'src/sbei/sbfw/simics.tar'
    elif i_filename == "releaseNotes.html":
       i_filename = 'src/sbei/sbfw/releaseNotes.html'

    print "  CMVC #\t: ",i_cmvcnum[1:]
    l_home_path= expanduser("~") + "/" + errorcode.CMVC_DIR_CREATE + i_cmvcnum

    cmd='File -checkout ' + i_filename + '  -release '+ i_release + '  -relative ' + l_home_path
    if i_cmvcnum[:1] == "D":
        cmd += '  -defect ' + i_cmvcnum[1:]
    else:
        cmd += '  -feature ' + i_cmvcnum[1:]

    print "  Executing\t: ", cmd
    rc = os.system(cmd)
    if rc:
        # rc 256 File not found in CMVC
        if rc == 256:
            print "  * File was not found or Error operation in CMVC"
        return errorcode.ERROR_CMVC_CHECKOUT
    else:
        return errorcode.SUCCESS_CMVC_CHECKOUT


##########################################################################
# Function :utilCmvcCheckin
#
# @param   i_filename : Relative Path of the File
#
# @param   i_release  : Fips FW Release to checkout (ex: fips910 )
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @brief   Check in a given file
#
##########################################################################
def utilCmvcCheckin(i_filename, i_release, i_cmvcnum):
    print "  File Name\t: ",i_filename
    print "  Release\t: ",i_release
    print "  CMVC #\t: ",i_cmvcnum[1:]

    l_home_path= expanduser("~") + "/" + errorcode.CMVC_DIR_CREATE + i_cmvcnum
    l_base_path = utilFindFile(i_filename,l_home_path)
    # This will give the ablsolute path, strip it from src
    for l_var in l_base_path.split("/src"):
        if i_filename in l_var:
            l_str = 'src' + l_var

    cmd='File -checkin ' + l_str + '  -release '+ i_release + '  -relative ' + l_home_path
    if i_cmvcnum[:1] == "D":
        cmd += '  -defect ' + i_cmvcnum[1:]
    else:
        cmd += '  -feature ' + i_cmvcnum[1:]

    print "  Executing\t: ", cmd
    rc = os.system(cmd)
    if rc:
        return errorcode.ERROR_CMVC_CHECKIN
    else:
        return errorcode.SUCCESS_CMVC_CHECKIN


##########################################################################
# Function :utilCmvcFixComplete
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @param   i_release  : Fips Release string
#
# @brief   Fix the record to complete
#
##########################################################################
def utilCmvcFixComplete(i_cmvcnum, i_release):
    print "  CMVC #\t: ",i_cmvcnum
    l_cmvcnum =i_cmvcnum[1:]

    if i_cmvcnum[:1] == "D":
        cmd='Fix -complete ' + ' -defect ' + l_cmvcnum + '  -r ' + i_release + ' -component esw_sbei'
    else:
        cmd='Fix -complete ' + ' -feature ' + l_cmvcnum + '  -r ' + i_release + ' -component esw_sbei'

    print "  Executing\t: ", cmd
    rc = os.system(cmd)
    if rc:
        return errorcode.ERROR_CMVC_FIX_RECORD
    else:
        return errorcode.SUCCESS_CMVC_FIX_RECORD

##########################################################################
# Function :utilCheckTrackState
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @param   i_release  : Fips Release string
#
# @brief   Check if the track is in fix state or not
#
##########################################################################
def utilCheckTrackState(i_cmvcnum, i_release):
    print "  CMVC #\t: ",i_cmvcnum
    print "  Release\t: ",i_release

    l_cmvcnum =i_cmvcnum[1:]
    cmd = 'Track -view -release ' + i_release
    if i_cmvcnum[:1] == "D":
        cmd += ' -defect ' + l_cmvcnum
    else:
        cmd += ' -feature ' + l_cmvcnum
    cmd += ' | grep state '

    print "  Executing\t: ", cmd
    ex_cmd = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    for line in ex_cmd.stdout :
        if not line: continue
        line = line.strip()
        if "fix" in line :
            return errorcode.SUCCESS_TRACK_STATE
    return errorcode.ERROR_TRACK_STATE

##########################################################################
# Function :utilCheckFileHash
#
# @param   i_src  : Source location of the file to be copy from
#
# @param   i_dest : Destination location of the file to be copied to
#
# @brief   Make sure after copying files, the hash matches
#
##########################################################################
def utilCheckFileHash(i_src, i_dest):
    print "  Source\t: ",i_src
    print "  Destination\t: ",i_dest

    sha_orig = hashlib.sha256()
    sha_orig.update(file(i_src).read())
    orig_hash=sha_orig.hexdigest()
    print "  * Orig Hash\t [ %s : %s ] "% (os.path.basename(i_src),orig_hash)

    sha_copy = hashlib.sha256()
    sha_copy.update(file(i_dest).read())
    copied_hash=sha_copy.hexdigest()
    print "  * Copied Hash\t [ %s : %s ] "% (os.path.basename(i_dest),copied_hash)

    if orig_hash == copied_hash:
        print "  -> Hash Match .. Continue\n"
        return errorcode.SUCCESS_HASH_CHECK
    else:
        print " Mismatch Hash.. Abort"
        return errorcode.ERROR_HASH_CHECK

##########################################################################
# Function :utilRollBack
#
# @param   i_action   : Undo Checkout or checkin
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @param   i_release  : Fips FW Release to checkout (ex: fips910 )
#
# @brief   Brute force undo of all files
#
##########################################################################
def utilRollBack(i_action, i_cmvcnum, i_release):
    print "\n  * Action request\t: ",i_action
    if i_action == "checkin":
        print "  * Undoing All default File list"
        if i_cmvcnum[:1] == "D":
            cmd = 'File -undo ' + errorcode.CMVC_FILE_UNDO_LIST + ' -release ' + i_release + '  -defect ' + i_cmvcnum[1:] + ' >/dev/null 2>&1'
        else:
            cmd = 'File -undo ' + errorcode.CMVC_FILE_UNDO_LIST + ' -release ' + i_release + '  -feature ' + i_cmvcnum[1:] + ' >/dev/null 2>&1'
    else:
        print "  * Unlocking All default Files list"
        if i_cmvcnum[:1] == "D":
            cmd = 'File -unlock ' + errorcode.CMVC_FILE_UNDO_LIST + ' -release ' + i_release + ' >/dev/null 2>&1'
        else:
            cmd = 'File -unlock ' + errorcode.CMVC_FILE_UNDO_LIST + ' -release ' + i_release + ' >/dev/null 2>&1'

    #print "  Executing : ", cmd
    rc = os.system(cmd)
    # CMVC throws this rc even if its successfull
    if rc != 1024 and rc !=0:
        print "\n  Error [ %s ] in Undoing/Unlocking Files.. Please check manually"%rc
    else:
        print "\n  Roll Back Successfull.. Please Revisit your inputs [ OK ] "


##########################################################################
# Function :utilTriggerJenkins
#
# @param   i_cmvcnum  : CMVC defect or Feature number
#
# @param   i_release  : Fips FW Release to checkout (ex: fips910 )
#
# @param   i_bvt      : Fips BVT xml file
#
# @brief   Trigger Jenkins CI job
#
##########################################################################
def utilTriggerJenkins(i_cmvcnum,i_release,i_bvt):
    print "  Jenkins job for %s" % i_cmvcnum
    #PATH : /afs/austin.ibm.com/projects/esw/bin/fsp-CI-jenkins
    cmd = "fsp-CI-jenkins -r " + i_release + " -t " + i_cmvcnum[1:] + " --test_on_hardware=y"
    if not i_bvt == "None" :
        cmd += " -b " + i_bvt
    rc = os.system(cmd)
    if rc :
       return errorcode.ERROR_CI_TRIGGER
    else :
       return errorcode.SUCCESS_CI_TRIGGER


##########################################################################
# Function :utilCopyFileToSandbox
#
# @param   i_pathname    : SBE repo Path location
#
# @param   i_sandboxname : Sandbox repo Path location
#
# @param   i_filename    : Files to be copied
#
# @brief   find the files in repo and copy to the sandbox
#
##########################################################################
def utilCopyFileToSandbox(i_pathname,i_sandboxname,i_filename):

    print "\n  *** For Development *** \n"
    print "  Sandbox path\t: ",i_sandboxname
    print "  SBE Repo path\t: ",i_pathname
    print "\n"

    if i_filename == "None":
        i_filename = errorcode.CMVC_FILE_LIST
        #print "  No User supplied Files to copy"
        #return errorcode.ERROR_FILE_INPUT

    for l_filename in i_filename.split(","):
        # Find the files and copy
        l_sandbox_name = os.path.basename(i_sandboxname)
        l_repo_path = utilFindFilePPE(l_filename,i_pathname,l_sandbox_name)
        if l_repo_path is None :
            print "  File [ %s ] not found in Repo..\t Check your repo and retry "%l_filename
            return errorcode.ERROR_CMVC_FILE_COPY
        else:
            for files in errorcode.FILE_LOOKUP_LIST.split(","):
                if l_filename in files:
                    break

            # Append this src path and to the sanbox base and copy the file
            sb_cp_path = i_sandboxname + '/' + files

            # Check if this path exist in the sandbox , if not create and copy
            if os.path.exists(sb_cp_path) == False:
                cmd='mkdir -p  ' + os.path.dirname(sb_cp_path)
                #print "  Creating dir %s "%cmd
                os.system(cmd)

            # Copy the file
            copy_cmd = 'cp -rf ' + l_repo_path + '  ' + sb_cp_path
            rc = os.system(copy_cmd)
            if rc:
                print "  RC code :",rc
                print "  ERROR : Copying file : ",copy_cmd
                return errorcode.ERROR_CMVC_FILE_COPY
            else:
                print "  Copied file : ",copy_cmd

    return errorcode.SUCCESS_DEV_EXIT


##########################################################################
# Function :utilppeSbENV
#
# @param   i_env_key : ENV paramter
#
# @brief   find the PPE Repo path from ENV and returns the path string
#
##########################################################################
def utilppeSbENV(i_env_key):
    #-----------------------------------------------------------
    # SBEROOT=/gsa/ausgsa/projects/i/indiateam04/gkeishin/PPE_CHANGES
    #-----------------------------------------------------------
    l_found_ppe_conf = False
    env_ppe_path="None"
    for key in os.environ.keys():
        if i_env_key in key:
            #print "\t %s : %s" % (key,os.environ[key])
            env_ppe_path = os.environ[key]
            l_found_ppe_conf = True

    if l_found_ppe_conf == False:
       print "  ---------------------------------------------------------"
       print "  | [ ERROR SETTING ] : The PPE Repository ENV is not set |"
       print "  | Please do ./sb workon to set/load the PPE repo ENV    |"
       print "  ---------------------------------------------------------"

    return env_ppe_path

##########################################################################
# Function : utilFind_sb_base
#
# @param   i_sb_name : Sandbox name
#
# @brief   find the sandbox base path
#
##########################################################################
def utilFind_sb_base(i_sb_name):
    out_str= "None"

    # workon -m ppc sbeisb -c 'env | grep SANDBOXBASE'
    find_sb_base = 'workon -m ppc  ' + i_sb_name + " -c 'env | grep SANDBOXBASE ' | grep SANDBOXBASE"

    # SANDBOXBASE=/gsa/ausgsa/projects/i/indiateam04/gkeishin/sbeisb
    out_str = os.popen(find_sb_base).read()

    if not out_str:
        return "None"
    else:
        return out_str.strip('SANDBOXBASE=')

##########################################################################
# Function : utilFind_sb_rc
#
# @param   i_sb_name : Sandbox RC path
#
# @brief   find the sandbox RC path
#
##########################################################################
def utilFind_sb_rc(i_sb_name):
    out_str= "None"

    # workon -m ppc sbeisb -c 'env | grep SANDBOXRC'
    find_sb_rc = 'workon -m ppc  ' + i_sb_name + " -c 'env | grep SANDBOXRC ' | grep SANDBOXRC"
    # SANDBOXRC=/gsa/ausgsa/projects/i/indiateam04/gkeishin/.sandboxrc
    out_str = os.popen(find_sb_rc).read()

    if not out_str:
        return "None"
    else:
        return os.path.dirname(out_str.strip('SANDBOXRC='))

##########################################################################
# Function : utilFind_ENV_string
#
# @param   i_env_name : ENV string
#
# @brief   find the ENV string set in the env
#
##########################################################################
def utilFind_ENV_string(i_env_name):
    out_str= "None"

    find_env_name = "env | grep " + i_env_name

    out_str = os.popen(find_env_name).read()

    if not out_str:
        return "None"
    else:
        # Strip string
        l_strip=i_env_name +"="
        return out_str.strip(l_strip)

##########################################################################
# Function : utilWriteShell_hooks
#
# @param   i_sandbox_path : Sandbox full path
#
# @brief   find the ENV string set in the env
#
##########################################################################
def utilWriteShell_hooks(i_sandbox_path):
    # Write the compile shell hook on the fips sandbox location
    hook_file=i_sandbox_path + '/src/compilesb'
    f = open(hook_file,'w')

    # compilesb: This hook schell script will look like this
    #
    # #!/bin/sh
    # cd /gsa/ausgsa/projects/i/indiateam04/gkeishin/sbeisb/src/sbei/sbfw/
    # mk -a
    # mk install_all

    f.write('#!/bin/sh \n')
    cd_cmd= 'cd ' + i_sandbox_path + '/src/sbei/sbfw/'
    f.write(cd_cmd)
    f.write('\n')
    f.write('mk -a || exit -1 \n')
    f.write('mk install_all || exit -1\n')
    f.close()

    # Change the file permission for execute
    perm_st = os.stat(hook_file)
    os.chmod(hook_file, perm_st.st_mode | stat.S_IEXEC)

    return hook_file # path of the shell file

