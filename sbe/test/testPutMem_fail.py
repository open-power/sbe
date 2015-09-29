import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 4

PUTMEM_TEST_HDR =  [0,0,0x00,0x86,
                    0,0,0xA4,0x02,
                    0,0,0x0,0x02,
                    0,0,0,0,
                    0x08,0x00,0x00,0x04,  # Un-aligned PBA Address
                    0x00,0x00,0x00,0x80]

PUTMEM_TEST_DATA = [0xAB,0xCD,0xEF,0x01,
                    0xAB,0xCD,0xEF,0x02,
                    0xAB,0xCD,0xEF,0x03,
                    0xAB,0xCD,0xEF,0x04,
                    0xAB,0xCD,0xEF,0x05,
                    0xAB,0xCD,0xEF,0x06,
                    0xAB,0xCD,0xEF,0x07,
                    0xAB,0xCD,0xEF,0x08]

PUTMEM_EXPDATA =  [0x00,0x00,0x00,0x00,
                   0xc0,0xde,0xa4,0x02,
                   0x00,0xfe,0x00,0x0a,
                   0xff,0xdc,0x00,0x03,
                   0x00,0x00,0x00,0x00,
                   0x00,0xf8,0x82,0x19,
                   0x00,0x00,0x00,0x06]


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    testUtil.writeUsFifo( PUTMEM_TEST_HDR )

    loop = 1
    while (loop <= LOOP_COUNT):
        testUtil.writeUsFifo( PUTMEM_TEST_DATA )
        loop += 1
    testUtil.writeEot( )

    testUtil.readDsFifo( PUTMEM_EXPDATA )
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

