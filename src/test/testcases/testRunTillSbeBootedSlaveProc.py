# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testRunTillSbeBootedSlaveProc.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2018,2020
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False

import testScomUtil

simicsObj = simics.SIM_run_command("get-component-list -all proc_p10")

from sim_commands import *

lbus = SIM_get_object(simicsObj[1] + ".cfam_cmp.lbus_map")

while True:
    try:
        if testUtil.read(lbus, 0x2824, 4)[0] & 0x80 :
            print "SBE is booted, continue"
            break
        else:
            print "SBE is still not booted"

    except:
        print "."
    testUtil.runCycles( 10000000 )
