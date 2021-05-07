#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/dumpParser/dump_utils.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021
# [+] International Business Machines Corp.
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

#Python Provided
import sys
import os
import datetime

#Supporting modules from CTE path
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)),os.path.expandvars("$CTEPATH")+ "/tools/crondump/rel/pymod"))
import out

#Dump Pre-req bits to updates for command status(This as per sbeCollectDump.H)
preReqCommandStatus = {0x80 : "FUNCTIONAL",
                       0x40 : "NON_FUNCTIONAL",
                       0x20 : "CLK_ON_STATE",
                       0x10 : "CORE_STOP_STATE",
                      }
def formatTime(timePassed, fraction=True):
    """
    Handles time formatting in common function
    """
    # The time comes out as 0:00:45.3482..
    # We find the break from the full seconds to the fractional seconds
    timeString = str(datetime.timedelta(seconds=timePassed))
    decIdx = timeString.find(".")

    if (fraction):
        # The second half of this is a bit of a mess
        # Convert the decimal string to a float, then round it to two places, then turn it back into a string
        # It has to be a formatted string conversion, a simple str() would turn .10 into .1.  Then remove the "0."
        return timeString[0:decIdx] + ("%.2f" % round(float(timeString[decIdx:]), 2))[1:]
    else:
        return timeString[0:decIdx]

def fancyPrint(text, specialCharacter, printLevel, length = 170):

    # Keep length less than the number of characters the screen size can
    # accmodate in a single line -- max = 180

    #TODO: Can be done with setattr by passing attribute as function param
    if(printLevel == "print"):
        out.print(text.center(length, specialCharacter))
    elif(printLevel == "info"):
        out.info(text.center(length, specialCharacter))
    elif(printLevel == "warn"):
        out.warn(text.center(length, specialCharacter))
    elif(printLevel == "debug"):
        out.debug(text.center(length, specialCharacter))
    elif(printLevel == "critical"):
        out.critical(text.center(length, specialCharacter))
    elif(printLevel == "error"):
        out.error(text.center(length, specialCharacter))
    else:
        out.critical("Unknown print level passed")
        sys.exit(1)

