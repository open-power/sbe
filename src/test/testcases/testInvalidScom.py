#!/usr/bin/env python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testInvalidScom.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
import sys
import os
sys.path.append("targets/p10_standalone/sbeTest" )
import testScomUtil
import testUtil

i_fifoType_fifo1 = 0
i_fifoType_fifo2 = 1
HWPffdc = True
expStatus = [0x00,0xFE,0x00,0x11]
invalid_addr = 0x4010C2F

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # getscom fail
    testScomUtil.getscom(invalid_addr, i_fifoType_fifo1, expStatus, HWPffdc)
    testScomUtil.getscom(invalid_addr, i_fifoType_fifo2, expStatus, HWPffdc)

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
try:
    main()
except:
    print ( "\nTest Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

print ( "\nTest Suite completed with no errors" )

#insert into Fifo
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0x4 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0xA201 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0x0 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0x4010C2F size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2408 value=0x1 size=4 -b

#readout of Fifo
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.read address = 0x2440 size = 4 -b
