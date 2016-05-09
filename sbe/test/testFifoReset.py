import sys
sys.path.append("targets/p9_nimbus/sbeTest")
import testUtil

err = False

# Test data that only contains the command header
TESTDATA = [0, 0, 0, 3,
            0, 0, 0xA1, 0x01]

# Complete test data
TESTDATA_FULL = [0, 0, 0, 3,
                 0, 0, 0xA1, 0x01,
                 0, 0x02, 0x00, 0x01]

# Get capabilities command. This will ensure the DS FIFO gets full
TESTDATA_2 = [0, 0, 0, 2,
              0, 0, 0xA8, 0x02]

def main():
    try:
        testUtil.runCycles(10000000)
        # Send a partial chip-op
        testUtil.writeUsFifo(TESTDATA)
        testUtil.resetFifo()
        # Make sure both the upstream and downstrem FIFOs are clear after the reset
        testUtil.waitTillUsFifoEmpty()
        testUtil.waitTillDsFifoEmpty()
        # Now send a complete chip-op on the upstream FIFO
        testUtil.writeUsFifo(TESTDATA_FULL)
        testUtil.writeEot()
        testUtil.resetFifo()
        # Make sure both the upstream and downstrem FIFOs are clear after the reset
        testUtil.waitTillUsFifoEmpty()
        testUtil.waitTillDsFifoEmpty()
        # Now send a get capabilities chip-op, so that in response, the DS FIFO
        # gets full before we do a reset
        testUtil.writeUsFifo(TESTDATA_2)
        testUtil.writeEot()
        testUtil.resetFifo()
        # Make sure both the upstream and downstrem FIFOs are clear after the reset
        testUtil.waitTillUsFifoEmpty()
        testUtil.waitTillDsFifoEmpty()
    except:
        print("\nTest completed with error(s), Raise error")
        raise
    print("\nTest completed with no errors")

main()

if err:
    print ("\nTest Suite completed with error(s)")
    #sys.exit(1)
else:
    print ("\nTest Suite completed with no errors")
    #sys.exit(0);

