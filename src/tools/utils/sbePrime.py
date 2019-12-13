#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbePrime.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2018
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

    #----------------------------
    # Read command line args
    #----------------------------
    opts, args = getopt.getopt(sys.argv[1:],"p:s:i:hr:nl:",['patch=', 'sb=', 'files=', 'help', 'rc_file=', 'no_build', 'level='])
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

    #-----------------------------------
    # 2) Get the Sanbox and repo paths
    #-----------------------------------
    # Get the base path of the fips sandbox
    if sandbox_name == "None":
        # Find the sanbox name and base from ENV
        # User must have done workon fips sandbox to work
        sandbox_path = utilcode.utilFind_ENV_string("SANDBOXBASE").rstrip('\n')
    else:
        sandbox_path = utilcode.utilFind_sb_base(sandbox_name).rstrip('\n')
    print "  Fips Sandbox path\t : ",sandbox_path

    #-----------------------------------
    # 3) Get the Sanbox root path
    #-----------------------------------
    if sandbox_name == "None":
        sandbox_root = utilcode.utilFind_ENV_string("SANDBOXROOT").rstrip('\n')
    else:
#        sandbox_root = utilcode.utilFind_ENV_string("SANDBOXRC").rstrip('\n')
        sandbox_root = utilcode.utilFind_sb_rc(sandbox_name).rstrip('\n')

    if sandbox_root == "None":
        print "  ** [ ERROR ] Something Fishy about the ENV set -OR- Option used.. Please check manually ** "
        usage()
        exit_main(errorcode.ERROR_SETTING)
    else:
        print "  Sandbox root path\t : ",sandbox_root

    #---------------------------------------------
    # sim setup if user initiates
    #---------------------------------------------
    if sim_patch != "None":
        #---------------------------------------------
        # Create sandbox for simics
        #---------------------------------------------
        rc_sb = utilpatch.utilExecuteShell(path_name,"None","sandbox-create")
        if rc_sb == errorcode.SUCCESS_EXIT:
            print "  Sandbox Created.. [ OK ] \n"
        else:
            print "  Sandbox Create.. [ ERROR ]",rc_sb
            exit_main(rc_sb)

        #----------------------------------------
        # Patch up the simics patches files
        #----------------------------------------

        print "\n  *** Update Simics patches onto Sandbox *** \n "
        # Pre sim setup
        rc_shell = utilpatch.utilExecuteShell(path_name,sandbox_path,"workarounds.presimsetup")
        if rc_shell == errorcode.SUCCESS_EXIT:
            print "  presimsetup [ OK ]\n"
        else:
            print "  presimsetup [ ERROR ] : ",rc_shell
            exit_main(rc_shell)

        # Patch the simics files
        rc_sim = utilpatch.utilPatchSimics(sandbox_path,sandbox_root)
        if rc_sim != errorcode.SUCCESS_EXIT:
            exit_main(rc_sim)
        else:
            print "  Patch the simics files on Sandbox [ OK ] \n"

        # Post sim setup
        rc_shell = utilpatch.utilExecuteShell(path_name,sandbox_path,"workarounds.postsimsetup")
        if rc_shell == errorcode.SUCCESS_EXIT:
            print "  postsimsetup [ OK ]\n"
            # Clean exit Get out from here
            exit_main(errorcode.SUCCESS_EXIT)
        else:
            print "  postsimsetup [ ERROR ] : ",rc_shell
            exit_main(rc_shell)

    # Files to copy for sbe prime
    if ddlevel == "None":
        PRIME_FILE_LIST  ="sbe_sp_intf.H,simics.tar,sbe_seeprom_DD2.bin"
    else:
        PRIME_FILE_LIST  ="sbe_sp_intf.H,simics.tar,sbe_seeprom_"+ddlevel+".bin"

    #----------------------------------------
    # 4) Copy the files from repo to sandbox
    #----------------------------------------
    # Find the files and copy to the sanbox dir
    # Just take a quick check if the Sandbox exist or not

    if sim_patch == "None":
        file_name = PRIME_FILE_LIST
    if sandbox_path != "None":
        if os.path.isdir(sandbox_path) == True:
           rc_copy = utilcode.utilCopyFileToSandbox(path_name,sandbox_path,file_name)
           if rc_copy == errorcode.SUCCESS_DEV_EXIT:
               print "  Files Copied to Fips Sandbox : [ OK ]"
           else:
               exit_main(rc_copy)
        else:
            print "  Sandbox : %s [ Either doesn't exist or do workon to fips sb to load the ENV.. ]" % os.path.basename(sandbox_path)
            print "               - OR - "
            print "  [ Optional ] You can specify your sandbox name as input as well"
            print "               -s <fips_sandbox Name >"
            usage()
            exit_main(errorcode.ERROR_SANDBOX_EXIST)
    else:
        print "  Please Check your fips Sandbox and retry"
        exit_main(errorcode.ERROR_SANDBOX_EXIST)

    sb_name=os.path.basename(sandbox_path)
    print "\n  Sandbox :",sb_name

    if build == "1":
        #----------------------------------------
        # 5) Wite the hook file into shell file
        #----------------------------------------
        # Write the compile shell hook on the fips sandbox location
        hook_file = utilcode.utilWriteShell_hooks(sandbox_path)

        #----------------------------------------
        # 6) Compile the code
        #----------------------------------------
        # Use the hook script to compile the code
        if sandbox_name == "None":
            compile_cmd="workon -m ppc  " + sb_name + " -c " + hook_file + " -rc " + sandbox_root +"/sbesandboxrc"
        else:
            if rc_file == "None":
                compile_cmd="workon -m ppc  " + sb_name + " -c " + hook_file + " -rc " + sandbox_root +"/.sandboxrc"
            else:
                print " getting rc file from user \n"
                compile_cmd="workon -m ppc  " + sb_name + " -c " + hook_file + " -rc " + rc_file
        print "\n  [ COMPILE ] Executing :%s \n"%compile_cmd
        rc = os.system(compile_cmd)

        print "  Compilation returned rc :",rc
        if rc != 0:
            exit_main(errorcode.ERROR_BUILD_FAILED)

    # Clean exit
    exit_main(errorcode.SUCCESS_EXIT)


if __name__=="__main__":
    main()

