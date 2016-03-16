import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False

GETMEMADU_TESTDATA_ECC_ITAG =  [0,0,0,0x6,
                                0,0,0xA4,0x01,
                                0,0,0x0,0xBD, #CoreChipletId/EccByte/Flags -> CacheInhibit/FastMode/Tag/Ecc/AutoIncr/Adu/Proc
                                0,0,0,0,              # Addr Upper 32 bit
                                0x08,0x00,0x00,0x00,  # Addr Lower 32 bit
                                0x00,0x00,0x00,0x40]  # length of data

GETMEMADU_EXPDATA_ECC_ITAG =   [0x00,0x00,0x00,0x50,  # length of data
                                0xc0,0xde,0xa4,0x01,
                                0x0,0x0,0x0,0x0,
                                0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # GetMemAdu with Ecc with Itag test
    testUtil.writeUsFifo( GETMEMADU_TESTDATA_ECC_ITAG )
    testUtil.writeEot( )

    testUtil.readDsEntry ( 20 )
    testUtil.readDsFifo( GETMEMADU_EXPDATA_ECC_ITAG )
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

