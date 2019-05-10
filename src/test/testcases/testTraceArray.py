# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testTraceArray.py $
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
from __future__ import print_function
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False

        #(Target Type, Chiplet Id, Trace bus)
TRACE_IDS = (
             (0x00, 0x00, 0x01), #Proc, xx, PROC_TB_PIB
             (0x00, 0x00, 0x3C), #Proc, xx, PROC_TB_IOO
             (0x00, 0x00, 0x3E), #Proc, xx, PROC_TB_MCA1
             (0x01, 0x20, 0x4B), #EX, 0x20, PROC_TB_L20
#             (0x02, 0x20, 0x50) #PERV, 0x20(Core), PROC_TB_CORE1
            )

CONTROL_TRACE_ARRAY_VALID = [0, 0, 0, 0, #Number of Words
                             0xC0, 0xDE, 0xA6, 0x02,
                             0, 0, 0, 0,
                             0, 0, 0, 0x03]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    print ("\nStarting control tracearray test")
    # Stop all the trace bus
    for traceId in TRACE_IDS:
        print ("Stop : "+str(traceId[2]))
        CONTROL_TRACE_ARRAY_STOP_TESTDATA = [0, 0, 0, 0x04,
                                        0, 0, 0xA6, 0x02,
                                        0, traceId[0], 0, traceId[1], #TARGET_PROC_CHIP, chiplet xx
                                        0, traceId[2], 0, 0x14] #stop & ignore mux
        testUtil.writeUsFifo( CONTROL_TRACE_ARRAY_STOP_TESTDATA)
        testUtil.writeEot( )
        testUtil.readDsFifo( CONTROL_TRACE_ARRAY_VALID)
        testUtil.readEot( )
    # dump traces from all the trace bus
    for traceId in TRACE_IDS:
        print ("Collect dump : "+str(traceId[2]))
        CONTROL_TRACE_ARRAY_COLLECT_DUMP_TESTDATA = [0, 0, 0, 0x04,
                                        0, 0, 0xA6, 0x02,
                                        0, traceId[0], 0, traceId[1], #TARGET_PROC_CHIP, chiplet xx
                                        0, traceId[2], 0, 0x18] #PROC_TB_PIB , stop & ignore mux
        testUtil.writeUsFifo(CONTROL_TRACE_ARRAY_COLLECT_DUMP_TESTDATA)
        testUtil.writeEot( )
        testUtil.readDsEntry(128 * 4) # Flush tracearray buffer - 128 rows of 4words
        CONTROL_TRACE_ARRAY_VALID_DUMP = [0, 0, 0x02, 0x00,#Number of Words - 0x200 - 128*4
                                         0xC0, 0xDE, 0xA6, 0x2,
                                         0, 0, 0, 0, #Primary and secondary status
                                         0, 0, 0, 0x03]
        testUtil.readDsFifo(CONTROL_TRACE_ARRAY_VALID_DUMP)
        testUtil.readEot( )
    # Reset and restart all the trace bus
    for traceId in TRACE_IDS:
        print ("Reset and restart : "+str(traceId[2]))
        CONTROL_TRACE_ARRAY_RESET_RESTART_TESTDATA = [0, 0, 0, 0x04,
                                                      0, 0, 0xA6, 0x02,
                                                      0, traceId[0], 0, traceId[1], #TARGET_PROC_CHIP, chiplet xx
                                                      0, traceId[2], 0, 0x13] #reset, restart & ignore mux
        testUtil.writeUsFifo(CONTROL_TRACE_ARRAY_RESET_RESTART_TESTDATA)
        testUtil.writeEot( )
        testUtil.readDsFifo( CONTROL_TRACE_ARRAY_VALID)
        testUtil.readEot( )


#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
if testUtil.getMachineName() == "axone":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )
else:
    main()

    if err:
        print ("\nTest Suite completed with error(s)")
        #sys.exit(1)
    else:
        print ("\nTest Suite completed with no errors")
        #sys.exit(0);
