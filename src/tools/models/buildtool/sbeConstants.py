# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/models/buildtool/sbeConstants.py $
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
'''
###########################################################
#    Created on Nov 23, 2015
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     11/12/15     Initial create
###########################################################
'''

##
# General Build constants
#
# @file    sbeImageConstants.py
# @author: George Keishing <gkeishin@in.ibm.com>
#

'''
Generic Failure RC code
'''
SUCCESS_EXIT   = 0
SHELL_EXIT     = 1
HELP_EXIT      = 10
NEW_MODEL_EXIT = 11
NO_PENDING_MODEL_EXIT = 12
VALIDATE_EXIT  = 13
FILES_EXIST    = 14
DIR_EMPTY      = 15

BYPASS_EXIT  = 20
PATCH_IGNORE_EXIT = 21
PPE_MODEL_NOT_EXIST = 22
PPE_MODEL_SKIP_EXIT = 23
PPE_BASE_ERROR_EXIT = 24
PPE_CHERRY_ERROR_EXIT = 25

EKB_MODEL_NOT_EXIST = 40

ERROR_SYS_ERROR_EXIT = 100

ERROR_PREP_GIT_FAILED         = 1000
ERROR_PREP_TOKEN_SETUP_FAILED = 1001
ERROR_PREP_GIT_SETUP_FAILED   = 1002

ERROR_PREP_COM_FAILED     = 2000
ERROR_PREP_CHANGE_FAILED  = 2001
ERROR_GIT_CHANGEID_FAILED = 2002

ERROR_LOAD_ENV = 5000
ERROR_MODEL_NOT_EXIST = 256  # hacked RC code exit from bash

# Build failures Manual or CI
BUILD_CI_FAILED      = 10000
BUILD_MANUAL_FAILED  = 10001


'''
git command ekb and ppe
'''

GIT_EKB_SETUP_CMD='git init;\
                   git remote add gerrit ssh://hw.gerrit/hw/ekb;\
                   git fetch gerrit;\
                   scp -p -P 29418 hw.gerrit:hooks/commit-msg .git/hooks;\
                   git checkout -b master gerrit/master'

GIT_PPE_SETUP_CMD='git init;\
                   git remote add gerrit ssh://hw.gerrit/hw/ppe;\
                   git fetch gerrit;\
                   scp -p -P 29418 hw.gerrit:hooks/commit-msg .git/hooks;\
                   git checkout -b master gerrit/master'

'''
git base repo home dir default
'''

GIT_EKB_REPO_SB='GIT_REPO_SB/ekb_sb'
GIT_PPE_REPO_SB='GIT_REPO_SB/ppe_sb'
