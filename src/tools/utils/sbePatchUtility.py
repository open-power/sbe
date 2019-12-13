#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbePatchUtility.py $
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
#    @brief Utilility Module to Patching files for SBE simics
#
#    Created on March 23, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     23/03/16     Initial create
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
import sbeCmvcUtility as utilcode

##########################################################################
# Function : utilPatchSimics
#
# @param   i_sandbox_path : Sandbox full path
#
# @param   i_sandbox_root : Sandbox RC root path
#
# @brief   Patch pre-req patches for simics
#
##########################################################################
def utilPatchSimics(i_sandbox_path, i_sandbox_root):
    print "\n  ... Patching simics files "

    sb_name=os.path.basename(i_sandbox_path)

    # Write the hooks for sim setup
    l_sim_file = utilShell_hooks(i_sandbox_path)

    if not l_sim_file:
        return errorcode.ERROR_HOOKING_FILE

    l_sim_cmd = "workon -m ppc  " + sb_name + " -c " + l_sim_file + " -rc " + i_sandbox_root +"/sbesandboxrc"
    print " ", l_sim_cmd

    os.system(l_sim_cmd)

    return errorcode.SUCCESS_EXIT

##########################################################################
# Function : utilExecuteShell
#
# @param   i_ppe_root     : Root folder for PPE. This script must be
#                         from PPE repo.
#
# @param   i_sandbox_path : fips Sandbox path
#
# @param   i_shell_file   : User defined shell script name
#
# @brief   Apply the simics patches pre define in shell script.
#
##########################################################################
def utilExecuteShell(i_ppe_root, i_sandbox_path, i_shell_file):
    print "\n   ... Executing shell : ",i_shell_file

    # Sanbox name
    if i_sandbox_path != "None":
        sb_name=os.path.basename(i_sandbox_path)

    # Find the file and execute
    l_path_name = i_ppe_root + '/src'
    l_shell_path=utilcode.utilFindFile(i_shell_file, l_path_name)
    print "  [ %s ]"%l_shell_path

    if i_sandbox_path != "None":
        # Load the shell onto the Sandbox env and execute
        l_shell_exec = "workon -m ppc  " + sb_name + " -c " + l_shell_path + " -rc " + i_sandbox_path.replace(sb_name,"") +"/sbesandboxrc"
    else:
        # Execute a stand alone script
        l_shell_exec = l_shell_path

    #rc = subprocess.call([l_shell_exec])
    rc = os.system(l_shell_exec)
    if rc :
        return rc

    return errorcode.SUCCESS_EXIT

##########################################################################
# Function : utilShell_hooks
#
# @param   i_sandbox_path : Sandbox full path
#
# @brief   find the ENV string set in the env
#
##########################################################################
def utilShell_hooks(i_sandbox_path):
    # Find the simics machine from ENV
    l_machine = os.environ['MACHINE'].rstrip('\n')
    print "  Machine : ",l_machine
    l_cmd_exec = 'start_simics -no_start -machine ' + l_machine + ' -batch_mode '

    # Write the compile shell hook on the fips sandbox location
    hook_file=i_sandbox_path + '/src/simsb'
    f = open(hook_file,'w')

    # simsb: This hook schell script will look like this
    #
    # #!/bin/sh
    # start_simics -no_start -machine NIMBUS -batch_mode

    f.write('#!/bin/sh \n')
    f.write('\n')
    f.write(l_cmd_exec)
    f.close()

    # Change the file permission for execute
    perm_st = os.stat(hook_file)
    os.chmod(hook_file, perm_st.st_mode | stat.S_IEXEC)

    return hook_file # path of the shell file

