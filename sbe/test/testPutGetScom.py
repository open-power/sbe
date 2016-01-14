import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

# @TODO via RTC : 141905
#       Modify the test sequence in such a way that
#       the test does not leave the Register value altered.

PUTSCOM_TESTDATA = [0,0,0,6,
                    0,0,0xA2,0x02,
                    0,0,0x0,0x00,
                    0,0x05,0x00,0x3E, #scratch reg 7 (32-bit)
                    0xde,0xca,0xff,0xee,
                    0x00,0x00,0x00,0x00 ]

PUTSCOM_EXPDATA = [0xc0,0xde,0xa2,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

GETSCOM_TESTDATA = [0,0,0,4,
                    0,0,0xA2,0x01,
                    0,0,0x0,0x00,
                    0,0x05,0x0,0x3E]

GETSCOM_EXPDATA = [0xde,0xca,0xff,0xee,
                   0x00,0x00,0x00,0x00,
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
    testUtil.writeUsFifo( GETSCOM_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETSCOM_EXPDATA )
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

