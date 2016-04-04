import sys
from sim_commands import *
import imp
err = False
testUtil = imp.load_source("testUtil", os.environ['SBE_TOOLS_PATH'] + "/testUtil.py")
EXPDATA = [0xc0,0xde,0xa1,0x01,
           0x0,0x0,0x0,0x0,
           0x00,0x0,0x0,0x03];
# MAIN Test Run Starts Here...
#-------------------------------------------------
def sbe_istep_func( major, minor ):
    try:
        TESTDATA = [0,0,0,3,
                     0,0,0xA1,0x01,
                    0,major,0,minor ]
        testUtil.runCycles( 10000000 )
        testUtil.writeUsFifo( TESTDATA )
        testUtil.writeEot( )
        testUtil.readDsFifo( EXPDATA )
        testUtil.readEot( )
    except:
        print ("\nTest completed with error(s). Raise error")
        # TODO via RTC 142706
        # Currently simics commands created using hooks always return
        # success. Need to check from simics command a way to return
        # Calling non existant command to return failure
        run_command("Command Failed");
        raise
    print ("\nTest completed with no errors")
        #sys.exit(0);

