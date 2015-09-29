import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

LOOP_COUNT = 1

GETMEM_TESTDATA =  [0,0,0,0x6,
                    0,0,0xA4,0x01,
                    0,0,0x0,0x02,
                    0,0,0,0,
                    0x08,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x80]  # length of data

GETMEM_EXPDATA =   [0x00,0x00,0x00,0x80,  # length of data
                    0xc0,0xde,0xa4,0x01,
                    0x0,0x0,0x0,0x0,
                    0x00,0x0,0x0,0x03];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # GetMem test
    testUtil.writeUsFifo( GETMEM_TESTDATA )
    testUtil.writeEot( )
    # GetMem chipOp would send the read data first,
    # thus, would attempt to read the expected length of data first
    loop = 1
    while ( loop <= LOOP_COUNT ):
        testUtil.readDsEntry ( 32 )  ## 32 entries ~ 128B PBA granule
        loop += 1
    testUtil.readDsFifo( GETMEM_EXPDATA )
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

