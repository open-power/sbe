#! /usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/buildInfo.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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

# This script will create header file sbe_build_info.H which will have
# buld information required by SBE code.
import os
buildInfoFileName = "sbe_build_info.H"

def buildInfo():
    header = \
"#ifndef SBE_BUILD_INFO_H  \n\
#define SBE_BUILD_INFO_H  \n\n"

    footer = "\n#endif  // SBE_BUILD_INFO_H"
    commitId = "0x" + os.popen("git rev-parse --short=8 HEAD").read().rstrip()

    f = open( buildInfoFileName, 'w')

    f.write(header)
    f.write("//Define SBE Commit ID \n")
    f.write("#define SBE_COMMIT_ID " + commitId + "\n")
    f.write(footer)
    f.close()

# Call buildInfo
buildInfo()
