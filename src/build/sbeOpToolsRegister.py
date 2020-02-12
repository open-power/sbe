#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/sbeOpToolsRegister.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
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

import os
import sys
import getopt

def exportFiles(sbe_simdata_dir, img_dir):

    #Create the sbe tools dir.
    os.system('mkdir -p ' + sbe_simdata_dir)

    #Copy sbe op binaries to sbe_simdata_dir
    os.system('cp '+sbe_simdata_dir+'/../sbe_binaries/*.ecc'+' '+sbe_simdata_dir+'/')

    #Copy the binaries to the sbe_simdata_dir
    os.system('cp '+img_dir+'/*.bin'+' '+sbe_simdata_dir+'/')

    #Copy sbe syms file to the sbe_simdata_dir
    os.system('cp '+img_dir+'/*.syms'+' '+sbe_simdata_dir+'/')

    #Copy the dis file to the sbe_simdata_dir
    os.system('cp '+img_dir+'/*.dis'+' '+sbe_simdata_dir+'/')

    #Copy the String file to the sbe_simdata_dir
    os.system('cp '+img_dir+'/*StringFile*'+' '+sbe_simdata_dir+'/')

    #Copy the ppe2fsp file to the sbe_simdata_dir
    os.system('cp '+img_dir+'/../obj/power/genfiles/ppe2fsp'+' '+sbe_simdata_dir+'/')

    #Copy the ppe2fsp file to the sbe_simdata_dir
    os.system('cp /opt/mcp/shared/fr_FLD9-DEV-20190320/opt/fsp/usr/bin/fsp-trace ' +sbe_simdata_dir+'/')

    #Copy the simic scripts to the sbe_simdata_dir
    os.system('cp '+img_dir+'/../src/tools/debug/simics-debug-framework_rainier.py'+' '+sbe_simdata_dir+'/')
    os.system('cp '+img_dir+'/../src/tools/debug/sbe-debug.py'+' '+sbe_simdata_dir+'/')
    os.system('cp '+img_dir+'/../src/tools/debug/simics/sbe_startup.simics'+' '+sbe_simdata_dir+'/')
    os.system('cp '+img_dir+'/../src/test/testcases/testP10_standalone.simics'+' '+sbe_simdata_dir+'/')
    os.system('cp '+img_dir+'/../src/test/testcases/testIstepAuto.py'+' '+sbe_simdata_dir+'/')
    os.system('cp '+img_dir+'/../src/test/testcases/testUtil.py'+' '+sbe_simdata_dir+'/')

    #Copy the security files to sbe_simdata_dir
    os.system('cp '+img_dir+'/../src/build/security/securityRegListGen.py'+' '+sbe_simdata_dir+'/')
    os.system('cp '+img_dir+'/../src/import/chips/p9/security/p9_security_white_black_list.csv'+' '+sbe_simdata_dir+'/')

