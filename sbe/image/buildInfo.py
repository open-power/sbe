#! /usr/bin/python

# This script will create header file sbe_build_info.H which will have
# buld information required by SBE code.
import os
buildInfoFileName = "sbe_build_info.H"

def buildInfo():
    header = \
"#ifndef SBE_BUILD_INFO_H  \n\
#define SBE_BUILD_INFO_H  \n\n"

    footer = "\n#endif  // SBE_BUILD_INFO_H"
    commitId = "0x" + os.popen("git rev-parse --short=8 HEAD").read().rstrip()

    f = open( buildInfoFileName, 'w')

    f.write(header)
    f.write("//Define SBE Commit ID \n")
    f.write("#define SBE_COMMIT_ID " + commitId + "\n")
    f.write(footer)
    f.close()

# Call buildInfo
buildInfo()
