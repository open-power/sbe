import stest
import time
import conf
from sim_commands import *

#err = False
lbus = conf.p9Proc0.proc_lbus_map
stest.untrap_log(logtype="error")
stest.untrap_log(logtype="info")
def writeUsFifo( data):
    """Main test Loop"""
    print "Starting test. Data =", data
    loopCount = len(data)/4;
    for i in range (loopCount):
        idx = i * 4;
        write(lbus, 0x2400, (data[idx], data[idx+1], data[idx+2], data[idx+3]) )

def readDsFifo(data):
    """Main test Loop"""
    print "Starting test. Data =", data
    loopCount = len(data)/4;
    for i in range (loopCount):
        idx = i * 4;
        stest.expect_equal(readEntry(lbus, 0x2440, 4), (data[idx], data[idx+1], data[idx+2], data[idx+3]))

def writeEot():
    write(lbus, 0x2408, (0, 0, 0, 1) )

def write(obj, address, value ):
    """ Write to memory space """
    iface = SIM_get_interface(obj, "memory_space")
    iface.write(None, address, value, 0x0)

def readEot():
    """ Read from memory space """
    status = read(lbus, 0x2444, 4)
    print "readEot status[3]",  status[3]
    stest.expect_equal( (status[3] & 0x80), 0x80 );
    read(lbus, 0x2440, 4)

# This function will only read the entry but will not compare it
# with anything. This can be used to flush out enteries.
def readDsEntry(entryCount):
    for i in range (entryCount):
        readEntry(lbus, 0x2440, 4)

def readEntry(obj, address, size):

    """ Read from memory space """
    loop = 1;
    count = 0;
    value = (0,0,0,0)
    while( loop ):
        status = read(lbus, 0x2444, 4)

        if( status[1] & 0x0F):
            # read entry
            value = read(lbus, address, size)
            loop = 0
        else:
            count = count + 1
            runCycles(200000)
            # This will cause  test to fail
            stest.expect_true( count < 10 , "No data in FIFO")

    return value

def read(obj, address, size):
    """ Read from memory space """
    iface = SIM_get_interface(obj, "memory_space")
    value = iface.read(None, address, size, 0x0)
    print "address:", address
    print "value:", value
    return value

def runCycles( cycles ):
    if (not SIM_simics_is_running()):
        syscmd   =   "run-cycles %d"%(cycles)
        ( rc, out )  =   quiet_run_command( syscmd, output_modes.regular )
        if ( rc ):
            print "simics ERROR running %s: %d "%( syscmd, rc )

