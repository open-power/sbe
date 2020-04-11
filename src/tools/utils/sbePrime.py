#!/usr/bin/python
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbePrime.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
'''
###########################################################
#    @file    sbePrime.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#    @brief   Main Module to support developer compilation
#             and patching.
#
#    Created on March 03, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     022/03/16     Initial create
###########################################################
'''

#-------------------------
#  Imports
#-------------------------
import getopt
import os, sys, glob
import shutil
import stat # for File permission op

# Libraries/utility funcs and user define const
import sbeCmvcConstants as errorcode
import sbeCmvcUtility as utilcode
import sbePatchUtility as utilpatch

#-------------------------
# Main Function
#-------------------------
def main():

    #------------------------------------------
    # Usage tool option
    #------------------------------------------
    def usage():
        print " \n"
        print "  :: Command line USAGE options for Copying SBE FW files for compilation :: \n"
        print "  sbeDistribute.py -s <Sandbox Name> -i <file1,file2...>"

        print " \n"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print "   |       By default NO argument is needed as an input .                            |"
        print "   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        print " \n  ***** Options Supported  *****"
        print " \t  -p,--patch     = [ Optional ] Patch Simics related files on Sandbox "
        print " \t  -s,--sb        = [ Optional ] Sandbox base name"
        print " \t                                By default it picks up the ppe Repo test_sb sandbox"
        print " \t                                but if you have created a sandbox of your own manually "
        print " \t                                and you want to compile, use this option then only"
        print " \t  -i,--files     = [ Optional ] Firmware Files coma ',' separated input file1,file2"
        print " \t                                Only the pre-define listed files bellow:"
        print " \t                                sbe_sp_intf.H,simics.tar,"
        print " \t                                sbe_seeprom_DD2.bin"
        print " \t  -r,--rc_file   = [ Optional ] The RC file for the sandbox (with absolute path)"
        print " \t  -n,--no_build  = [ Optional ] Flag to determine if sbei component should be compiled"
        print " \t  -m,--simicsPath    = [ Optional ] Path to create the simics folder"
        print " \t  -b,--hbStandalone  = [ Optional ] Flag to determine if sb prime is for sbe or HBstnadalone"
        print " \t  -h,--help      = Help"
        print "  ------------------------------------------------------------------------------------"

    #------------------------------------------
    # Exit from this Main
    #------------------------------------------
    def exit_main(rc):
        if rc == errorcode.HELP_EXIT:
           print "  [ HELP DOCUMENTATION ]\n"
           sys.exit(0)

        if rc == errorcode.ERROR_BUILD_FAILED:
            print "  Compilation Failed .. Error "

        if rc == errorcode.SUCCESS_DEV_EXIT:
           print "  [ Files Copy done! ]\n"
           sys.exit(0)

        if rc:
           print "\n  [ ERROR - MAIN ] Exiting with error code = ", rc
           sys.exit(rc)
        else:
           print "\n  Fips Sandbox compilation and simics patching completed [ OK ] "
           sys.exit(0)

    #------------------------------------------
    # Local var place name holder's
    #------------------------------------------
    sim_patch    = "None"
    sandbox_name = "None"
    path_name    = "None" # PPE Repo
    file_name    = "None"
    rc_file      = "None"
    build        = "1"
    ddlevel      = "None"
    hbStandalone = "0"
    simicsPath   = "simics"

    #----------------------------
    # Read command line args
    #----------------------------
    opts, args = getopt.getopt(sys.argv[1:],"p:s:i:hr:nl:m:b:",['patch=', 'sb=', 'files=', 'help', 'rc_file=', 'no_build', 'level=', 'simicsPath=', 'hbStandalone='])
    for opt, arg in opts:
       if opt in ('-h', '--help'):
           usage()
           exit_main(errorcode.HELP_EXIT)
       elif opt in ('-p', '--patch'):
           sim_patch = arg
       elif opt in ('-s', '--sb'):
           sandbox_name = arg
       elif opt in ('-i', '--files'):
           file_name = arg
       elif opt in ('-r', '--rc_file'):
           rc_file = arg
       elif opt in ('--no_build'):
           build = "0"
       elif opt in ('-l', '--level'):
           ddlevel = arg
       elif opt in ('-m', '--simicsPath'):
           simicsPath = arg
       elif opt in ('-b', '--hbStandalone'):
           hbStandalone = arg
       else:
           usage()
           exit_main(errorcode.ERROR_EXIT)

    #------------------------------------------------------
    # Make sure that it has passed atleast one arg with it
    #------------------------------------------------------
    if len(sys.argv)<1:
        usage()
        exit_main(errorcode.ERROR_SYS_EXIT)

    #---------------------------------------------
    # Callling the Func defs in order
    #---------------------------------------------

    #------------------------------
    # 1) User input params/ Check ENV
    #------------------------------
    print "\n [ Checking SBE user ENV Pre-req ] "
    # Get it from ENV
    if path_name == "None":
        # Get the PPE path
        l_ppe_path = utilcode.utilppeSbENV("SBEROOT")
        if l_ppe_path == "None":
            print "\n  Couldn't find PPE repo info from ENV currently set... "
            print "  [ ERROR ] PPE Repo ENV Setting Path  : %s " % l_ppe_path
            exit_main(errorcode.ERROR_SETTING)
        else:
            print "  PPE Repo path Setting\t :  %s "% l_ppe_path
            path_name = l_ppe_path

    if build == "1": 
        #----------------------------------------
        # 2) Simics setup
        #----------------------------------------
        rc = os.system("sh $SBEROOT/src/test/framework/populate-sandbox " + simicsPath + " " + hbStandalone)
        print "  Simics setup returned rc :",rc
        if rc != 0:
            exit_main(errorcode.ERROR_BUILD_FAILED)


    # Clean exit
    exit_main(errorcode.SUCCESS_EXIT)


if __name__=="__main__":
    main()

