import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False

PUTREG_TESTDATA =  [0,0,0,9,
                    0,0,0xA5,0x02,
                    0x00,0x20,0x01,0x02, # two spr registers
                    0,0,0x0,0x08,
                    0,0,0x0,0x0,
                    0,0,0x0,0x1,
                    0,0,0x0,0x09,
                    0,0,0x0,0x0,
                    0,0,0x0,0x2 ]

PUTREG_EXPDATA =  [0xc0,0xde,0xa5,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

GETREG_TESTDATA = [0,0,0,5,
                    0,0,0xA5,0x01,
                    0x00,0x20,0x01,0x02, # two spr registers
                    0,0,0x0,0x08,
                    0,0,0x0,0x09 ]

GETREG_EXPDATA = [0x00,0x00,0x00,0x00,
                   0x00,0x00,0x00,0x01,
                   0x00,0x00,0x00,0x00,
                   0x00,0x00,0x00,0x02,
                   0xc0,0xde,0xa5,0x01,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( PUTREG_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTREG_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( GETREG_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETREG_EXPDATA )
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

