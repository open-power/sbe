# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: env.bash $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2020
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
ROOTDIR=.
export SBEROOT=`pwd`

if [ -e ./projectrc ]; then
    source ./projectrc
fi

if [ -e ./customrc ]; then
    source ./customrc
fi

export PATH=${PATH}:${SBEROOT}/images/

if [ -n "${SANDBOXROOT}" ]; then
    if [ -n "${SANDBOXNAME}" ]; then
        export SANDBOXBASE="${SANDBOXROOT}/${SANDBOXNAME}"
    fi
fi

export DEFAULT_MACHINE=nimbus

# The following ENV variables are required by the pre/post commit hooks
export PROJECT_NAME=sbe
export TOOLSDIR=$SBEROOT/src/tools/hooks
export HOOKSDIR=$SBEROOT/.git/hooks
export IMPORT_REL_PATH=import
export IMPORT_DIR=$SBEROOT/$IMPORT_REL_PATH
export LICENSE=$SBEROOT/LICENSE_PROLOG # Copyright license file for PPE
export PATH=${PATH}:$TOOLSDIR # Update PATH

# Fetch the gerrit server; Required by the pre/post commit hooks
if [ -e $HOME/.ssh/config ]; then
if [ -e $TOOLSDIR/gerrit-hostname ]; then
    echo "Searching for Gerrit Host..."
    eval $($TOOLSDIR/gerrit-hostname)
fi
fi

# Copy the git hooks to .git/hooks folder
if [ -e .git/hooks ]; then
if [ -e $TOOLSDIR/setupgithooks.sh ]; then
    $TOOLSDIR/setupgithooks.sh
fi
fi
