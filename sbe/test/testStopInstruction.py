import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 1


# Stop All thread in Core0 with warn flag true
INST_STOP0_ALL_TESTDATA_WITH_WARN_FLG =      [0,0,0,0x03,
                                              0,0,0xa7,0x01,
                                              0,1,0x20,0xf1]

INST_EXPDATA  =     [0xc0,0xde,0xa7,0x01,
                     0x0,0x0,0x0,0x0,
                     0x00,0x0,0x0,0x03]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    #stop all thread in core0
    testUtil.writeUsFifo( INST_STOP0_ALL_TESTDATA_WITH_WARN_FLG )
    testUtil.writeEot( )
    testUtil.readDsFifo( INST_EXPDATA )
    testUtil.readEot( )

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
main()

if err:
    print ("\nTest Suite completed with error(s)")
    #sys.exit(1)
else:
    print ("\nTest Suite completed with no errors")
    #sys.exit(0);

