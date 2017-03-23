# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/sbeOpDistribute.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017
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

CHIPID = 'p9n'
p9n_EC = ['10']#, '20']
DD_level = {'10':'DD1', '20':'DD2'}

def usage():
    print "usage:sbeOpDistribute.py [--root_dir] <root path> [--staging_dir] <staging path> [--img_dir] <images path>"

def run_system_cmd(cmd):
    print 'Cmd:<'+cmd+'>'
#    if(os.system(cmd)):
#        print "ERROR running cmd:<"+cmd+">"
#        exit(1)

def main(argv):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "", ['root_dir=', 'staging_dir=', 'img_dir=', 'host_dir=', 'help'])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        exit(1)

    # Default values
    root_dir = ''
    staging_dir = ''
    img_dir = ''
    host_dir = ''

    if len(opts)<3:
        usage()
        exit(1)

    # Parse the command line arguments
    for opt, arg in opts:
        if opt == '--help':
            usage()
            exit(1)
        elif opt == '--root_dir':
            root_dir = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)
        elif opt == '--staging_dir':
            staging_dir = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)
        elif opt == '--img_dir':
            img_dir = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)
        elif opt == '--host_dir':
            host_dir = str(arg)
            assert os.path.exists(arg), "Did not find the file at,"+str(arg)

    SEEPROM_IMAGE           = 'sbe_seeprom.bin'
    SEEPROM_HDR_BIN         = 'sbe_seeprom.hdr.bin'
    SBE_BINARIES_PATH       = staging_dir+'/sbe_binaries'
    HB_BUILD_IMAGES         = staging_dir+'/hostboot_build_images'
    SBE_OUT_BINARY          = staging_dir+'/hostboot_binaries/nimbus_sbe.img'
    HW_REF_IMAGE            = staging_dir+'/hostboot_binaries/'+CHIPID+'.ref_image.bin'
    IPL_BUILD_TOOL          = img_dir+'/p9_ipl_build'
    BUILD_PREFIX_HEADER     = 'buildPrefixHeader.pl'
    ECC_TOOL                = host_dir+'/usr/bin/ecc'

    # Create binaries folder
    run_system_cmd('mkdir -p '+SBE_BINARIES_PATH)
    for ecLevel in p9n_EC:
        # Copy sbe raw binary to binaries folder
        run_system_cmd('cp '+img_dir+'/'+'sbe_seeprom_'+DD_level[ecLevel]+'.bin'+' '+SBE_BINARIES_PATH+'/'+CHIPID+'_'+ecLevel+'.'+SEEPROM_IMAGE)
        # Add HW ref image
        run_system_cmd(IPL_BUILD_TOOL+' '+SBE_BINARIES_PATH+'/'+CHIPID+'_'+ecLevel+'.'+SEEPROM_IMAGE+' '+HW_REF_IMAGE+' 0x'+ecLevel)
        # Add build header
        run_system_cmd(BUILD_PREFIX_HEADER+' --inputFile '+SBE_BINARIES_PATH+'/'+CHIPID+'_'+ecLevel+'.'+SEEPROM_IMAGE+' --outputFile '+SBE_BINARIES_PATH+'/'+CHIPID+'_'+ecLevel+'.'+SEEPROM_HDR_BIN)
        # buildSbePart.pl
        run_system_cmd(HB_BUILD_IMAGES+'/buildSbePart.pl --sbeOutBin '+SBE_OUT_BINARY+'  --ecImg_'+ecLevel+' '+SBE_BINARIES_PATH+'/'+CHIPID+'_'+ecLevel+'.'+SEEPROM_HDR_BIN)
        run_system_cmd('dd if='+SBE_OUT_BINARY+' of='+SBE_OUT_BINARY+'.256K ibs=256K conv=sync')
        run_system_cmd(ECC_TOOL+' --inject '+SBE_OUT_BINARY+'.256K --output '+SBE_OUT_BINARY+'.ecc --p8')
if __name__ == "__main__":
    main(sys.argv)
