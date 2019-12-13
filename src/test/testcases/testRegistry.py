#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testRegistry.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2017
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
'''
#############################################################
#    @file    testClass.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#    @brief   Framework class Host SBE interface on simics
#
#    Created on March 29, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     29/03/16     Initial create
#############################################################
'''

# Test OP keywords for reference
'''
  - read    : Read from a Registry
  - write   : write to a Registry
  - memRead : Read from a memory address block
'''

# Registry address for direct usage
REG_MBOX0 = 0x000D0050
REG_MBOX1 = 0x000D0051
REG_MBOX2 = 0x000D0052
REG_MBOX3 = 0x000D0053
REG_MBOX4 = 0x000D0054
REG_MBOX5 = 0x000D0055
REG_MBOX6 = 0x000D0056
REG_MBOX7 = 0x000D0057

# PSU doorbell regs
PSU_SBE_DOORBELL_REG         = 0x000D0060
PSU_SBE_DOORBELL_REG_WO_AND  = 0x000D0061
PSU_SBE_DOORBELL_REG_WO_OR   = 0x000D0062

PSU_HOST_DOORBELL_REG        = 0x000D0063
PSU_HOST_DOORBELL_REG_WO_AND = 0x000D0064
PSU_HOST_DOORBELL_REG_WO_OR  = 0x000D0065


# Memory space address
'''
simics> system_cmp0.phys_mem.map
           Base  Object                  Fn           Offset       Length
-------------------------------------------------------------------------
0x0000008000000  p9Proc0.l3_cache_ram     0              0x0     0xa00000
    width 8192 bytes
0x6030000000000  p9Proc0.lpcm             0  0x6030000000000   0xffffffff
    width 4 bytes
0x603fc00000000  proc_p9chip0.mc_freeze   0              0x0  0x400000000
    target -> proc_p9chip0.xscom_memspc, width 8 bytes

'''
MEM_ADDR = 0x0000008000000 
