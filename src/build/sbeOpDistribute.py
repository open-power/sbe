#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/sbeOpDistribute.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2019
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
import os
import sys
import getopt

def usage():
    print "usage:sbeOpDistribute.py [--sbe_binary_dir] <sbe binary path> [--img_dir] <images path>"

def run_system_cmd(cmd):
    print 'Cmd:<'+cmd+'>'
    if(os.system(cmd)):
        print "ERROR running cmd:<"+cmd+">"
        exit(1)

def main(argv):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "", ['sbe_binary_dir=', 'img_dir=', 'buildSbePart=', 'hw_ref_image=', 'sbe_binary_filename=', 'scratch_dir=', 'install', 'help'])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        exit(1)

    # Default values
    mode = "MAKE"
    sbe_binary_dir = ''
    img_dir = ''
    buildSbePart = ''
    hw_ref_image = ''
    sbe_binary_filename = ''
    scratch_dir = ''

    sbe_out_name = ''

    if len(opts)<2:
        usage()
        exit(1)

    # Parse the command line arguments
    for opt, arg in opts:
        if opt == '--help':
            usage()
            exit(1)
        elif opt == '--install':
            mode = "INSTALL"
        elif opt == '--sbe_binary_dir':
            sbe_binary_dir = str(arg)
        elif opt == '--img_dir':
            img_dir = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)
        elif opt == '--buildSbePart':
            buildSbePart = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)
        elif opt == '--hw_ref_image':
            hw_ref_image = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)
        elif opt == '--sbe_binary_filename':
            sbe_binary_filename = str(arg)
            sbe_out_name = sbe_binary_filename.split(".ecc")[0]
        elif opt == '--scratch_dir':
            scratch_dir = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)

    SEEPROM_IMAGE           = 'sbe_seeprom.bin'
    SEEPROM_HDR_BIN         = 'sbe_seeprom.hdr.bin'

    #Default chipId for Nimbus chips as this script can be called without the
    #input SBE binary name.
    CHIPID = 'p9n'
    eclist = {'21':'DD2', '22':'DD2', '23':'DD2'}

    #Update if the script is called for Axone chips
    if (sbe_binary_filename == "axone_sbe.img.ecc"):
      CHIPID = 'p9a'
      eclist = {'10':'AXONE'}

    if (mode == "MAKE"):
        # Create binaries folder
        run_system_cmd('mkdir -p '+sbe_binary_dir)
        for ecLevel, ddLevel in eclist.items():
            # Copy sbe raw binary to binaries folder
            run_system_cmd('cp '+img_dir+'/'+'sbe_seeprom_'+ddLevel+'.bin'+' '+sbe_binary_dir+'/'+CHIPID+'_'+ecLevel+'.'+SEEPROM_IMAGE)
    elif (mode == "INSTALL"):
        ec_build_sbe_cmd = ''
        for ecLevel, ddLevel in eclist.items():
            basename = CHIPID+'_'+ecLevel+'.sbe_seeprom'
            ec_build_sbe_cmd += ' --ecImg_'+ecLevel+' '+scratch_dir+'/'+basename+'.hdr.bin'
            # Copy sbe raw binary to scratch folder
            run_system_cmd('cp '+sbe_binary_dir+'/'+basename+'.bin'+' '+scratch_dir+'/'+basename+'.bin')
            # Add HW ref image
            run_system_cmd('p9_ipl_build '+scratch_dir+'/'+basename+'.bin '+hw_ref_image+' 0x'+ecLevel)
            #add pnor header
            run_system_cmd("env echo -en VERSION\\\\0 > "+scratch_dir+"/"+basename+".sha.bin")
            run_system_cmd("sha512sum "+scratch_dir+"/"+basename+".bin | awk '{print $1}' | xxd -pr -r >> "+scratch_dir+"/"+basename+".sha.bin")
            run_system_cmd("dd if="+scratch_dir+"/"+basename+".sha.bin of="+scratch_dir+"/"+basename+".hdr.bin ibs=4k conv=sync")
            run_system_cmd("cat "+scratch_dir+"/"+basename+".bin >> "+scratch_dir+"/"+basename+".hdr.bin")

        # buildSbePart.pl
        run_system_cmd(buildSbePart+" --sbeOutBin "+scratch_dir+"/"+sbe_out_name+ec_build_sbe_cmd)
        run_system_cmd("ecc --inject "+scratch_dir+"/"+sbe_out_name+" --output "+scratch_dir+"/"+sbe_out_name+".ecc --p8")
if __name__ == "__main__":
    main(sys.argv)
