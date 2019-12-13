#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/updateBuildTag.py $
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
import subprocess
import socket
from datetime import datetime

# Update the build tag/time/data/user/host in the
# sbe_seeprom_DD1.bin/the sbe_seeprom_DD2.bin binary image
def updateBuildTag(argv):
    try:
        xip_tool = argv[1]
        image_dir = argv[2]
        seeprom_name = argv[3]
    except:
        print "Missing Xip Tool Path/Image Directory/Seeprom Binary Name"
        exit(-1)

    # Commandline cmds getting formed here
    cmd_basic      = xip_tool+" "+image_dir+"/"+seeprom_name+".bin"
    cmd_build_tag  = cmd_basic+" set build_tag "
    cmd_build_date = cmd_basic+" set build_date "
    cmd_build_time = cmd_basic+" set build_time "
    cmd_build_user = cmd_basic+" set build_user "
    cmd_build_host = cmd_basic+" set build_host "

    tag_exist_cmd = "git log | grep \"Release notes for sbe\"| awk '{ print $NF }'"
    proc = subprocess.Popen(tag_exist_cmd, shell=True, stdout=subprocess.PIPE)
    build_tag = proc.stdout.read()
    if ( build_tag ):
        cmd_build_tag = cmd_build_tag+" "+build_tag

    else:
        commit_id_cmd = "git log |grep commit -m1 |awk '{ print $NF }' |awk '{print substr ($0, 0, 17)}'"
        proc = subprocess.Popen(commit_id_cmd, shell=True, stdout=subprocess.PIPE)
        commit_id = proc.stdout.read()
        cmd_build_tag = cmd_build_tag+" "+commit_id

    proc = subprocess.Popen(cmd_build_tag, shell=True, stdout=subprocess.PIPE)
    cmd_build_date = cmd_build_date+" "+datetime.now().strftime('%Y%m%d')
    proc = subprocess.Popen(cmd_build_date, shell=True, stdout=subprocess.PIPE)
    cmd_build_time = cmd_build_time+" "+datetime.now().strftime('%H%M')
    proc = subprocess.Popen(cmd_build_time, shell=True, stdout=subprocess.PIPE)
    cmd_temp = "id -un"
    proc = subprocess.Popen(cmd_temp, shell=True, stdout=subprocess.PIPE)
    cmd_build_user = cmd_build_user+" "+proc.stdout.read() 
    proc = subprocess.Popen(cmd_build_user, shell=True, stdout=subprocess.PIPE)
    cmd_build_host = cmd_build_host+" "+socket.gethostname()
    proc = subprocess.Popen(cmd_build_host, shell=True, stdout=subprocess.PIPE)

updateBuildTag(sys.argv)

