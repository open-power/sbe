# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testInvalidScom.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
import struct
import testScomUtil

i_fifoType = 0
# getscom fail
testScomUtil.getscom(0x6c090, i_fifoType)

#insert into Fifo
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0x4 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0xA201 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0x6c090 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2400 value=0x0 size=4 -b
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.write address=0x2408 value=0x1 size=4 -b

#readout of Fifo
#backplane0.dcm[0].chip[0].cfam_cmp.lbus_map.read address = 0x2440 size = 4 -b
