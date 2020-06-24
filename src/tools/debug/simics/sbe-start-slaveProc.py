# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/simics/sbe-start-slaveProc.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2018,2020
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False

import testScomUtil
from sim_commands import *

simicsProcObj = simics.SIM_run_command("get-component-list -all proc_p10")
for proc in simicsProcObj:
    if(proc !=  "backplane0.dcm[0].chip[0]"):
        print(proc)
        cmd_write = proc + ".cfam_cmp.lbus_map.write 0x2804 0x90000000 4"
        print(cmd_write)
        quiet_run_command( cmd_write, output_modes.regular )
        testUtil.runCycles( 1000000000 )
        cmd_write = proc + ".cfam_cmp.lbus_map.write 0x2820 0x00080000 4"
        print(cmd_write)
        quiet_run_command( cmd_write, output_modes.regular )
        testUtil.runCycles( 1000000000 )

