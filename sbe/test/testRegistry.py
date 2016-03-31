#!/usr/bin/python
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
REG_MBOX0 = 0x00680500
REG_MBOX1 = 0x00680510
REG_MBOX2 = 0x00680520
REG_MBOX3 = 0x00680530
REG_MBOX4 = 0x00680540
REG_MBOX5 = 0x00680550
REG_MBOX6 = 0x00680560
REG_MBOX7 = 0x00680570

# PSU doorbell regs
PSU_SBE_DOORBELL_REG         = 0x00680600
PSU_SBE_DOORBELL_REG_WO_AND  = 0x00680610
PSU_SBE_DOORBELL_REG_WO_OR   = 0x00680620

PSU_HOST_DOORBELL_REG        = 0x00680630
PSU_HOST_DOORBELL_REG_WO_AND = 0x00680640
PSU_HOST_DOORBELL_REG_WO_OR  = 0x00680650


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
