import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

PUTSCOM_TESTDATA = [0,0,0,6,
                    0,0,0xA2,0x02,
                    0,0,0x0,0x00,
                    0,0x02,0x00,0x14,
                    0xff,0xff,0xff,0xff,
                    0xff,0xff,0xff,0xff ]

PUTSCOM_EXPDATA = [0xc0,0xde,0xa2,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];


PUTSCOMUMASK_TESTDATA = [0,0,0,8,
            0,0,0xA2,0x04,
            0,0,0x0,0x00,
            0,0x02,0x00,0x14,
            0xde,0xca,0xff,0xee,
            0xaa,0xca,0xff,0xee,
            0xff,0x00,0xff,0x00,
            0x00,0xff,0x00,0xff]


PUTSCOMUMASK_EXPDATA = [0xc0,0xde,0xa2,0x04,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

GETSCOMUMASK_TESTDATA = [0,0,0,4,
                   0,0,0xA2,0x01,
                   0,0,0x0,0x00,
                   0,0x02,0x0,0x14]

GETSCOMUMASK_EXPDATA = [0xde, 0xff, 0xff, 0xff,
                   0xff, 0xca, 0xff, 0xee,
                   0xc0,0xde,0xa2,0x01,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    testUtil.writeUsFifo( PUTSCOM_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTSCOM_EXPDATA )
    testUtil.readEot( )

    testUtil.writeUsFifo( PUTSCOMUMASK_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTSCOMUMASK_EXPDATA )
    testUtil.readEot( )

    testUtil.writeUsFifo( GETSCOMUMASK_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETSCOMUMASK_EXPDATA )
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

