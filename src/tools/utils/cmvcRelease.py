# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/cmvcRelease.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2023
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
import subprocess
import os
import sys

REPO            = "ssh://hw.gerrit/hw/ppe"
REMOTE          = "ppe"
BRANCH          = {"master":"master", "fips910":"release-fips910"}
RELEASE         = {"master":"911", "fips910":"910"}
REL_TOOL        = "./src/tools/utils/gitRelease.pl"
CMVC_USER       = "shakeeb"
CMVC_PASSWORD   = "xxxxxx"

#def create_cmvc_track(branch):
#    os.system('rm -rf release_temp')
#    f = open('release_temp', 'w+')
#
#    # cmvc feature for release
#    cmd = ['Feature','-open', '-remarks',' \"SBE release '+build_name+'\"', '-component', 'esw_sbei']
#    result = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
#    cmvc_feature = result.split()[-1].split('.')[0]
#    print ("cmvc track ["+cmvc_feature+']')
#    os.system('Feature -assign '+cmvc_feature+' -owner '+CMVC_USER)
#    os.system('Feature -accept '+cmvc_feature)
#    os.system('Track -create -feature '+cmvc_feature+' -release fips911')
#
#    f.write(cmvc_feature)
#    f.close()

def step_1(branch):
    # checkout master branch
    os.system('git fetch '+REMOTE)
    os.system('git checkout ' + REMOTE + "/" + BRANCH[branch])

    # clean stale files
    os.system('git clean src/ -d -f -x')
    # stash any changes
    os.system('git stash')
    # fetch all tags
    os.system('git fetch '+REPO+' --tags')

    # workon
    os.system('./sb workon')

def step_2(branch, cmvc_feature):
    # get last release tag
    cmd = ['git', 'for-each-ref', '--sort=taggerdate', '--format','\'%(refname) %(taggerdate)\'','refs/tags']
    result = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()

    resultarray = result[0].split('\n')
    filterarray = []
    for ele in resultarray:
        if "."+RELEASE[branch] in ele:
            filterarray += [ele]
    last_release = filterarray[-1].split()[0].split('/')[-1]

    print ("last release ["+last_release+"]")

    # build-name generation
    cmd = [REL_TOOL,'build-name', '-release', RELEASE[branch]]
    build_name = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0].split('\n')[0]
    print ("build name ["+build_name+"]")

    # latest commit
    cmd = ['git','log', '-1', '--pretty=format:%H']
    commit = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0].split('\n')[0]
    print ("commit ["+commit+"]")

    # define release level
    cmd = REL_TOOL+' define --level '+str(build_name)+' --base '+str(commit)+' --released '+str(last_release)
    os.system(cmd)
    print cmd
    # release
    cmd = REL_TOOL+' release --level '+build_name
    os.system(cmd)
    print cmd

    # make SBE
    cmd = 'make clean'
    print cmd
    os.system(cmd)
    cmd = 'make install'
    print cmd
    os.system(cmd)

    # trigger CI
    cmd = './sb cmvc -f '+str(cmvc_feature)+' -r ' + 'fips'+RELEASE[branch]
    os.system(cmd)
    print cmd

# after ci pass
def step_3(branch, track):
    # integrate track
    cmd = 'Track -integrate -feature '+track+' -release fips'+RELEASE[branch]
    os.system(cmd)
    print cmd

    # get last release tag
    cmd = ['git', 'for-each-ref', '--sort=taggerdate', '--format','\'%(refname) %(taggerdate)\'','refs/tags']
    result = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()

    resultarray = result[0].split('\n')
    filterarray = []
    for ele in resultarray:
        if "."+RELEASE[branch] in ele:
            filterarray += [ele]
    last_release = filterarray[-1].split()[0].split('/')[-1]

    print ("last release ["+last_release+"]")
    cmd = 'git push '+REMOTE+' '+last_release
    os.system(cmd)
    print cmd


# cmvc log in
os.system('cmvclog -in '+CMVC_USER+' -noprompt '+CMVC_PASSWORD)

if(sys.argv[1] == '1'):
    step_1(sys.argv[2])
elif(sys.argv[1] == '2'):
    step_2(sys.argv[2], sys.argv[3])
elif(sys.argv[1] == '3'):
    step_3(sys.argv[2], sys.argv[3])
