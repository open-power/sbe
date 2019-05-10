# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testRunTillSbeBooted.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2018,2019
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
from __future__ import print_function
import os
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil
err = False

import testScomUtil

from sim_commands import *

if testUtil.getMachineName() == "axone":
    lbus = conf.backplane0.proc0.cfam_cmp.lbus_map
else:
    lbus = conf.p9Proc0.proc_lbus_map

while True:
    try:
        if testUtil.read(lbus, 0x2824, 4)[0] & 0x80 :
            print("SBE is booted, continue")
            break
        else:
            print("SBE is still not booted")

    except:
        print(".")
    testUtil.runCycles( 10000000 )
