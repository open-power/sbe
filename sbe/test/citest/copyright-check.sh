#!/bin/sh
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/test/citest/copyright-check.sh $
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

#
#   Front end to addCopyright - script to check for copyright block during
#   Gerrit checkin.
#

export WORKSPACE_DIR=`pwd`
export ADDCOPYRIGHT=${WORKSPACE_DIR}/sbe/tools/hooks/addCopyright

##  run git show to get a list of checked in files
CHECKINFILES=`git show --pretty=format: --name-only -n1 | tr '\n' ' '`
##  use git log to determine the year of the commit.
##     Since commits have their copyright updated at the time they are
##     committed, a commit might have a copyright date in its prolog of
##     last year.  Set the DATE_OVERRIDE variable to the 'validate' to allow
##     slightly-old prologs (ie. ones corresponding to the date in the msg).
export DATE_OVERRIDE=`git log -n1 --date=short | grep "Date" | sed "s/Date: *//" | sed "s/-.*//"`

echo "========================================================================"

echo "  Checking Copyright blocks for checked-in files:"
echo "  $CHECKINFILES"
echo
$ADDCOPYRIGHT validate  $CHECKINFILES --copyright-check

if [ $? -eq 0 ]; then
    echo "Copyright Check passed OK, $?"
    exit 0
else
    echo "ERROR: $?"
    exit 1
fi
