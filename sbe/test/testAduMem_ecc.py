import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False

LOOP_COUNT = 1

GETMEMADU_TESTDATA_ECC =  [0,0,0,0x6,
                           0,0,0xA4,0x01,
                           0,0,0x0,0xAD, #CoreChipletId/EccByte/Flags - CacheInhibit/FastMode/NoTag/Ecc/AutoIncr/Adu/Proc
                           0,0,0,0,              # Addr Upper 32 bit
                           0x08,0x00,0x00,0x00,  # Addr Lower 32 bit
                           0x00,0x00,0x00,0x20]  # length of data

GETMEMADU_EXPDATA_ECC =   [0x00,0x00,0x00,0x24,  # length of data
                           0xc0,0xde,0xa4,0x01,
                           0x0,0x0,0x0,0x0,
                           0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # GetMemAdu with Ecc
    testUtil.writeUsFifo( GETMEMADU_TESTDATA_ECC)
    testUtil.writeEot( )

    testUtil.readDsEntry ( 9 )
    testUtil.readDsFifo( GETMEMADU_EXPDATA_ECC)
    testUtil.runCycles( 10000000 )
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

