import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False

LOOP_COUNT = 1

PUTMEMADU_CNTLDATA =  [0,0,0,0,
                       0,0,0xA4,0x02,
                       0,0x07,0x0,0xBD, #CoreChipletId/EccByteTrue/Flags -> EccOverride/CacheInhibit/FastMode/Tag/EccOverride/AutoIncr/Adu/Proc
                       0,0,0,0,              # Addr Upper 32 bit
                       0x08,0x00,0x00,0x00,  # Addr Lower 32 bit
                       0x00,0x00,0x00,0x08]  # length of data

PUTMEMADU_TESTDATA   =  [0xab,0xcd,0xef,0x12,
                         0xba,0xdc,0xfe,0x21]

PUTMEMADU_EXPDATA =   [0x00,0x00,0x00,0x0a,  # length of data
                       0xc0,0xde,0xa4,0x02,
                       0x0,0x0,0x0,0x0,
                       0x00,0x0,0x0,0x03]



GETMEMADU_TESTDATA =  [0,0,0,0x6,
                       0,0,0xA4,0x01,
                       0,0,0x0,0xBD, #CoreChipletId/EccByte/Flags -> CacheInhibit/FastMode/Tag/Ecc/AutoIncr/Adu/Proc
                       0,0,0,0,              # Addr Upper 32 bit
                       0x08,0x00,0x00,0x00,  # Addr Lower 32 bit
                       0x00,0x00,0x00,0x08]  # length of data

GETMEMADU_EXPDATA =   [0xab,0xcd,0xef,0x12,  #data
                       0xba,0xdc,0xfe,0x21,
                       0x01,0x07,0,0,        #First Byte is iTag / Second Byte is ECC
                       0x00,0x00,0x00,0x0a,  # length of data
                       0xc0,0xde,0xa4,0x01,
                       0x0,0x0,0x0,0x0,
                       0x00,0x0,0x0,0x03];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    #PutMemAdu Test
    testUtil.writeUsFifo( PUTMEMADU_CNTLDATA )
    testUtil.writeUsFifo( PUTMEMADU_TESTDATA )
    testUtil.writeEot( )

    testUtil.readDsFifo( PUTMEMADU_EXPDATA )
    testUtil.readEot( )

    # GetMemAdu test
    testUtil.writeUsFifo( GETMEMADU_TESTDATA )
    testUtil.writeEot( )

    testUtil.readDsFifo( GETMEMADU_EXPDATA )
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

