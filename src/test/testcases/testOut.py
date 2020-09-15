# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testOut.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG

############################################################
# Imports - Imports - Imports - Imports - Imports - Imports
############################################################
from __future__ import print_function
# Import builtins so we can call the actual python print function
# for output to the screen
# Without that, we get stuck in a loop of calling the module print
import __builtin__ as builtins
import os
import datetime
import logging
import ctypes
import io
import tempfile
import sys
import signal
from enum import Enum

############################################################
# Variables - Variables - Variables - Variables - Variables
############################################################
# These match the default logging levels, with the addition of one level
# That is the "BASE" level which is used to ensure the basic output to the screen
class levels(Enum):
    DEBUG = 10
    INFO = 20
    WARN = 30
    ERROR = 40
    BASE = 45
    CRITICAL = 50

    # Function to help with printing and comparision
    def __repr__(self):
        return str(self)

    def __str__(self):
        return self.name

    def __int__(self):
        return self.value

    def __lt__(self, other):
        return self.value < other.value

    def __le__(self, other):
        return self.value <= other.value

    def __gt__(self, other):
        return self.value > other.value

    def __ge__(self, other):
        return self.value >= other.value

    # Need to make choices in argparse work
    def argparse(s):
        try:
            return levels[s]
        except KeyError:
            return s

class VarBox:
    pass

__m = VarBox()
__m.consoleLevel = levels.BASE
__m.logLevel = levels.INFO
__m.indent = 0
__m.prefix = ""
__m.postfix = ""
# The consuming program can optionally setup to log
# By default, it is disabled
__m.log = None
__m.console = None

############################################################
# Function - Functions - Functions - Functions - Functions
############################################################

###########################
# output functions
###########################
def debug(message, logOnly=False, doLevel=True):
    """
    Function to generate/print/log debug messages
    """
    return __output(levels.DEBUG, message, logOnly, doLevel)

def info(message, logOnly=False, doLevel=True):
    """
    Function to generate/print/log normal output printing
    """
    return __output(levels.INFO, message, logOnly, doLevel)

def warning(message, logOnly=False, doLevel=True):
    """
    Function to generate/print/log warning messages
    """
    return __output(levels.WARN, message, logOnly, doLevel)

def error(message, logOnly=False, doLevel=True):
    """
    Function to generate/print/log error messages
    """
    return __output(levels.ERROR, message, logOnly, doLevel)

def print(message, logOnly=False):
    """
    Function to generate/print/log standard messages that always go out
    """
    # print level messages never get a level prefix on the front
    return __output(levels.BASE, message, logOnly, doLevel=False)

def critical(message, logOnly=False, doLevel=True):
    """
    Function to generate/print/log critical messages
    """
    return __output(levels.CRITICAL, message, logOnly, doLevel)

def __output(level, message, logOnly, doLevel):
    """
    Common function that underpins the debug/info/etc.. functions
    """
    logOutput = ""
    consoleOutput = ""
    # Handle multi-line messages that are output in one statement
    # This ensures everything gets leveled, prefixed and indented properly
    for line in message.split("\n"):
        # Let up the line we are going to log
        formatLine = __m.prefix + (' ' * __m.indent) + line + __m.postfix + "\n"
        logOutput += formatLine
        # Setup the line we are going to put ot the console
        # If the level is to be on the message, redo the formatLine
        if (doLevel):
            formatLine = __m.prefix + (' ' * __m.indent) + level.name + ": " + line + __m.postfix + "\n"
        consoleOutput += formatLine
    logOutput = logOutput[:-1] # Eat the extra new line we add above
    consoleOutput = consoleOutput[:-1] # Eat the extra new line we add above

    if (__m.log):
        __m.log.log(level.value, logOutput)

    if (__m.console):
        __m.console.log(level.value, consoleOutput)

    if (not logOnly):
        if (__m.consoleLevel <= level):
            builtins.print(consoleOutput)

###########################
# setup/support functions
###########################
def setIndent(num):
    """
    Sets the output indent on all printed lines
    """
    __m.indent = num

def getIndent():
    """
    Gets the output indent on all printed lines
    """
    return __m.indent

def setPrefix(prefix):
    """
    Set the prefix to put onto any printed lines
    Useful for programs scraping output looking for specific pattern
    """
    __m.prefix = prefix

def setPostfix(postfix):
    """
    Set the postfix to put onto any printed lines
    Useful for programs scraping output looking for specific pattern
    """
    __m.postfix = postfix

def date():
    """
    Return the current date/time
    """
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

def getConsoleLevel():
    """
    Return the level we are currently at
    """
    return __m.consoleLevel

def setConsoleLevel(level):
    """
    Set the output level to the console
    If not called, will use the default defined in VarBox
    """
    __m.consoleLevel = level

def getLogLevel():
    """
    Return the level we are currently at
    """
    return __m.logLevel

def setLogLevel(level):
    """
    Set the output level to the log
    If not called, will use the default defined in VarBox
    """
    __m.logLevel = level

def setupLogging(filenameLog, filenameConsole):
    """
    Setup the logging infrastructure
    """
    # Load in custom levels
    for level in levels:
        logging.addLevelName(level.value, level.name)

    # Setup main log file
    if (filenameLog.endswith(".bz2")):
        logHandler = logging.StreamHandler(bz2.open(filenameLog, mode='wt', encoding='utf-8'))
    else:
        logHandler = logging.FileHandler(filenameLog, mode='w')
    logHandler.setFormatter(logging.Formatter("%(asctime)s %(levelname)-5s| %(message)s","%Y%m%d-%H%M%S"))

    __m.log = logging.getLogger("log")
    __m.log.addHandler(logHandler)
    __m.log.setLevel(__m.logLevel.value)

    # Setup console log file
    if (filenameConsole.endswith(".bz2")):
        consoleHandler = logging.StreamHandler(bz2.open(filenameConsole, mode='wt', encoding='utf-8'))
    else:
        consoleHandler = logging.FileHandler(filenameConsole, mode='w')
    consoleHandler.setFormatter(logging.Formatter("%(message)s"))

    __m.console = logging.getLogger("console")
    __m.console.addHandler(consoleHandler)
    __m.console.setLevel(__m.consoleLevel.value)

def shutdown():
    """
    Called at program exit to ensure anything in process gets flushed and closed
    """
    # Go through the log/console handlers and close them out
    # Especially important when writing directly to compressed logs.
    # They'll show as corrupted if not closed properly
    handlers = __m.log.handlers[:]
    for handler in handlers:
        handler.close()
        __m.log.removeHandler(handler)
    handlers = __m.console.handlers[:]
    for handler in handlers:
        handler.close()
        __m.console.removeHandler(handler)

###########################
# capture functions
###########################

# A program needs to be able to capture the stdout from function calls
# in order to control what goes to the screen and ensure everything is
# properly logged.  That is what this stdout_redirector function does
# The concept/code comes from https://eli.thegreenplace.net/2015/redirecting-all-kinds-of-stdout-in-python/
#
# In the default implementation, if a ctrl-c is raised in the middle of this, we won't go
# through the __exit__ call, which leaves stdout in a weird state
# Output may not come out, we get errors about writing to closed files, etc..
# To solve that problem we install our own signal handler for the length of the run
# It just captures the ctrl-c happened and continues on until __exit__
# Then, as the last thing in __exit__ it calls the original handler with the signal it caught
# This ensures stdout gets set back properly!
class stdout_redirector():
    def __init__(self, stream):
        self.stream = stream
        # Variables we need shared within the class
        self.original_stdout_fd = None
        self.saved_stdout_fd = None
        self.tfile = None
        # Variables for signal handling
        self.received_signal = False
        self.original_handler = None
        self.libc = ctypes.CDLL(None)
        self.c_stdout = ctypes.c_void_p.in_dll(self.libc, 'stdout')

    def __enter__(self):
        # Must be first: Install the signal handler
        self.original_handler = signal.signal(signal.SIGINT, self._handler)

        # The original fd stdout points to. Usually 1 on POSIX systems.
        self.original_stdout_fd = sys.stdout.fileno()

        # Save a copy of the original stdout fd in saved_stdout_fd
        self.saved_stdout_fd = os.dup(self.original_stdout_fd)

        # Create a temporary file and redirect stdout to it
        self.tfile = tempfile.TemporaryFile(mode='w+b')
        self._redirect_stdout(self.tfile.fileno())

    def __exit__(self, type, value, traceback):
        self._redirect_stdout(self.saved_stdout_fd)
        # Copy contents of temporary file to the given stream
        self.tfile.flush()
        self.tfile.seek(0, os.SEEK_SET)
        self.stream.write(self.tfile.read())
        self.tfile.close()
        os.close(self.saved_stdout_fd)

        # Must be last: Restore the orginal signal handler and call it if needed
        signal.signal(signal.SIGINT, self.original_handler)
        if (self.received_signal):
            self.original_handler(*self.received_signal)

    def _redirect_stdout(self, to_fd):
        """ Redirect stdout to the given file descriptor """
        # Flush the C-level buffer stdout
        self.libc.fflush(self.c_stdout)
        # Flush and close sys.stdout - also closes the file descriptor (fd)
        sys.stdout.close()
        # Make original_stdout_fd point to the same file as to_fd
        os.dup2(to_fd, self.original_stdout_fd)
        # Create a new sys.stdout that points to the redirected fd
        sys.stdout = io.TextIOWrapper(os.fdopen(self.original_stdout_fd, 'wb'), encoding='utf-8', line_buffering=True)
        #sys.stdout = codecs.getreader("utf-8")(os.fdopen(self.original_stdout_fd, 'wb'))

    def _handler(self, sig, frame):
        # Save the signal so we can put it out after we get everything cleaned up in __exit__
        self.received_signal = (sig, frame)
        # Debug tracing - since you can't print to the screen, increment size of file you can monitor
        #os.system("echo 1 >> /tmp/signal")

def capture(function, *args, **kwargs):
    """
    Wraps the passed in function to collect any output to the screen
    Returns the rc from the function, along with the captured output
    """
    rc = 0
    msgs = list()
    f = io.BytesIO()

    # Run our command capture the output
    # Do it within an exception handler to bolster the output capturing
    # Without the exception handler here, any of the captured output is lost
    # due to the fact it bails immediately and the msgs aren't processed
    # A future enhancement might be to rethrow the exception after output
    # is handled.  Not needed at this time though.
    try:
        with stdout_redirector(f):
            rc = function(*args, **kwargs)
    except BaseException as e:
        msgs.append("Exception - %s - %s" % (function, format(e)))

    # Process the output captured and put it out
    for line in "{0}".format(f.getvalue().decode('utf-8')).splitlines():
        msgs.append(line)

    return (rc, msgs)
