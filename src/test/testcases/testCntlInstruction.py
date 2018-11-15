# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testCntlInstruction.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2019
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
import sys
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 1

#Invalid Input
INST_INVALID_TESTDATA = [0,0,0,3,
                         0,0,0xa7,0x01,
                         0,1,0x20,0xee]


INST_INVALID_EXPDATA_ERR = [0xc0,0xde,0xa7,0x01,
                            0x00,0x02,0x00,0x0A,
                            0x00,0x00,0x00,0x03]

# STOP Ins
# core 0 thread 0 STOP WARN FLAG as true
INST_STOP_0_0_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x01]

# core 0 thread 1 STOP WARN FLAG as true
INST_STOP_0_1_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x11]

# core 0 thread 2 STOP WARN FLAG as true
INST_STOP_0_2_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x21]

# core 0 thread 3 STOP with WARN FLAG as true
INST_STOP_0_3_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x31]

# core 0 thread 0 STOP WARN FLAG as false
INST_STOP_0_0_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x01]

# core 0 thread 1 STOP WARN FLAG as false
INST_STOP_0_1_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x11]

# core 0 thread 2 STOP WARN FLAG as false
INST_STOP_0_2_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x21]

# core 0 thread 3 STOP WARN FLAG as false
INST_STOP_0_3_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x31]

# Stop All thread in Core0 with warn flag true
INST_STOP0_ALL_TESTDATA_WITH_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,1,0x20,0xf1]

# Stop All thread in Core0 with warn flag false
INST_STOP0_ALL_TESTDATA_WITHOUT_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,0,0x20,0xf1]


# START Ins
# core 0 thread 0 START WARN FLAG as true
INST_START_0_0_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x00]

# core 0 thread 1 START WARN FLAG as true
INST_START_0_1_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x10]

# core 0 thread 2 START WARN FLAG as true
INST_START_0_2_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x20]

# core 0 thread 3 START with WARN FLAG as true
INST_START_0_3_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,1,0x20,0x30]

# core 0 thread 0 START WARN FLAG as false
INST_START_0_0_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x00]

# core 0 thread 1 START WARN FLAG as false
INST_START_0_1_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x10]

# core 0 thread 2 START WARN FLAG as false
INST_START_0_2_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x20]

# core 0 thread 3 START WARN FLAG as false
INST_START_0_3_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                               0,0,0xa7,0x01,
                                               0,0,0x20,0x30]

# Start All thread in Core0 with warn flag true
INST_START0_ALL_TESTDATA_WITH_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,1,0x20,0xf0]

# Start All thread in Core0 with warn flag false
INST_START0_ALL_TESTDATA_WITHOUT_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,0,0x20,0xf0]

# STEP Ins
# core 0 thread 0 STEP WARN FLAG as true
INST_STEP_0_0_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x02]

# core 0 thread 1 STEP WARN FLAG as true
INST_STEP_0_1_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x12]

# core 0 thread 2 STEP WARN FLAG as true
INST_STEP_0_2_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x22]

# core 0 thread 3 STEP with WARN FLAG as true
INST_STEP_0_3_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x32]

# core 0 thread 0 STEP WARN FLAG as false
INST_STEP_0_0_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x02]

# core 0 thread 1 STEP WARN FLAG as false
INST_STEP_0_1_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x12]

# core 0 thread 2 STEP WARN FLAG as false
INST_STEP_0_2_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x22]

# core 0 thread 3 STEP WARN FLAG as false
INST_STEP_0_3_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x32]

# Step All thread in Core0 with warn flag true
INST_STEP0_ALL_TESTDATA_WITH_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,1,0x20,0xf2]

# Step All thread in Core0 with warn flag false
INST_STEP0_ALL_TESTDATA_WITHOUT_WARN_FLG =   [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,0,0x20,0xf2]

# SRESET Ins
# core 0 thread 0 SRESET WARN FLAG as true
INST_SRESET_0_0_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x03]

# core 0 thread 1 SRESET WARN FLAG as true
INST_SRESET_0_1_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x13]

# core 0 thread 2 SRESET WARN FLAG as true
INST_SRESET_0_2_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x23]

# core 0 thread 3 SRESET with WARN FLAG as true
INST_SRESET_0_3_TESTDATA_WITH_WARN_FLG  =  [0,0,0,0x03,
                                          0,0,0xa7,0x01,
                                          0,1,0x20,0x33]

# core 0 thread 0 SRESET WARN FLAG as false
INST_SRESET_0_0_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x03]

# core 0 thread 1 SRESET WARN FLAG as false
INST_SRESET_0_1_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x13]

# core 0 thread 2 SRESET WARN FLAG as false
INST_SRESET_0_2_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x23]

# core 0 thread 3 SRESET WARN FLAG as false
INST_SRESET_0_3_TESTDATA_WITHOUT_WARN_FLG  =  [0,0,0,0x03,
                                             0,0,0xa7,0x01,
                                             0,0,0x20,0x33]

# Sreset All thread in Core0 with warn flag true
INST_SRESET0_ALL_TESTDATA_WITH_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,1,0x20,0xf3]

# Sreset All thread in Core0 with warn flag false
INST_SRESET0_ALL_TESTDATA_WITHOUT_WARN_FLG =   [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,0,0x20,0xf3]


INST_EXPDATA  =     [0xc0,0xde,0xa7,0x01,
                     0x0,0x0,0x0,0x0,
                     0x00,0x0,0x0,0x03]

INST_EXPDATA_ERR = [0xc0,0xde,0xa7,0x01,
                    0x00,0xFE,0x00,0x0A,
                    0x00,0x00,0x00,0x03]

STOP_INST_EXPDATA_ERR_WTH_FFDC = [0xc0,0xde,0xa7,0x01,
                                  0x00,0xFE,0x00,0x0A]

START_INST_EXPDATA_ERR_WTH_FFDC = [0xc0,0xde,0xa7,0x01,
                                  0x00,0xFE,0x00,0x0A]

STEP_INST_EXPDATA_ERR_WTH_FFDC = [0xc0,0xde,0xa7,0x01,
                                  0x00,0xFE,0x00,0x0A]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    #Try an invalid data case
    testUtil.writeUsFifo( INST_INVALID_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_INVALID_EXPDATA_ERR )
    testUtil.readEot( )

    # Control Instruction Message - Stop
    testUtil.writeUsFifo( INST_STOP_0_0_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_1_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_2_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_3_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_0_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_1_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_2_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP_0_3_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )

    #stop all thread in core0
    testUtil.writeUsFifo( INST_STOP0_ALL_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STOP0_ALL_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STOP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )

    # Control Instruction Message - Start
    testUtil.writeUsFifo( INST_START_0_0_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START_0_1_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START_0_2_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START_0_3_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )

    testUtil.writeUsFifo( INST_START_0_0_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( START_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START_0_1_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( START_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START_0_2_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( START_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START_0_3_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( START_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )

    #start all thread in core0
    testUtil.writeUsFifo( INST_START0_ALL_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_START0_ALL_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( START_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )

    # Control Instruction Message - Step
    testUtil.writeUsFifo( INST_STEP_0_0_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_1_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_2_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_3_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_0_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STEP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_1_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STEP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_2_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STEP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP_0_3_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STEP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )

    #step all thread in core0
    testUtil.writeUsFifo( INST_STEP0_ALL_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_STEP0_ALL_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( STEP_INST_EXPDATA_ERR_WTH_FFDC )
    testUtil.extractHWPFFDC( )
    #flush out distance
    testUtil.readDsEntryReturnVal()
    testUtil.readEot( )

    # Control Instruction Message - Sreset
    testUtil.writeUsFifo( INST_SRESET_0_0_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_1_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_2_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_3_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_0_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_1_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_2_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET_0_3_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )

    #step all thread in core0
    testUtil.writeUsFifo( INST_SRESET0_ALL_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( INST_SRESET0_ALL_TESTDATA_WITHOUT_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )


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