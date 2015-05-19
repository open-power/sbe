import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

TESTDATA = [0,0,0,3,
            0,0,0xA1,0x01,
            0,0x02,0x00,0x1]

EXPDATA = [0xc0,0xde,0xa1,0x01,
           0x0,0x2,0x0,0xa,
           0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( EXPDATA )
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

