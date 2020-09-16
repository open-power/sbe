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

#Get the latest backing build from fips based on the time
BACKING_BUILD_PATH=/esw/fips1010/Builds
OUT_FILES=( $(ls -lr ${BACKING_BUILD_PATH}/*.1010) )
#If file found use latest backing build else use default
if [[ ${OUT_FILES[8]} == *".1010"* ]]; then
      export BACKING_BUILD=${OUT_FILES[8]}/ #latest
else
      export BACKING_BUILD=${BACKING_BUILD_PATH}/built #Default
fi

# Set CTE path if not already set
if [ -z "${CTEPATH}" ]; then
    export CTEPATH=/afs/apd.pok.ibm.com/projects/cte
fi

# set path for Python 3
if [ -f /opt/xsite/cte/tools/python/bin/python3 ]; then
    export PATH_PYTHON3=/opt/xsite/cte/tools/python/bin/python3
else
    export PATH_PYTHON3=$CTEPATH/tools/ecmd/rel/x86_64/python3
fi

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
