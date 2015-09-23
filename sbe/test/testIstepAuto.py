import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False

EXPDATA = [0xc0,0xde,0xa1,0x01,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def sbe_istep_func( major, minor ):
    TESTDATA = [0,0,0,3,
                0,0,0xA1,0x01,
                0,major,0,minor ]
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( EXPDATA )
    testUtil.readEot( )
    if err:
        print ("\nTest completed with error(s)")
        #sys.exit(1)
    else:
        print ("\nTest completed with no errors")
        #sys.exit(0);

