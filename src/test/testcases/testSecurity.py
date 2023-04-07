# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testSecurity.py $
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
import os
import sys
import struct
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil
import testScomUtil

#SECURITY_FILE = "../../src/import/chips/p9/security/p9_security_white_black_list.csv"
#SECURITY_SCRIPT = "../../src/build/security/securityRegListGen.py"

if testUtil.getMachineName() == "axone":
    SECURITY_FILE = "targets/p9_axone/sbeTest/p9_security_white_black_list.csv"
    SECURITY_SCRIPT = "targets/p9_axone/sbeTest/securityRegListGen.py"
else:
    SECURITY_FILE = "../../src/import/chips/p9/security/p9_security_white_black_list.csv"
    SECURITY_SCRIPT = "../../src/build/security/securityRegListGen.py"

def getlist(cmd):
    os.system("python "+SECURITY_SCRIPT+" "+cmd+" -f "+SECURITY_FILE+" > temp")
    list = []
    with open('temp', 'r') as f:
        a = f.read()
        list = a.split('[')[-1].split(']')[0].split(", ")
    list = [eval(x) for x in list]
    return list

def getmap(cmd):
    map = ()
    os.system("python "+SECURITY_SCRIPT+" "+cmd+" -f "+SECURITY_FILE+" > temp")
    with open('temp', 'r') as f:
        a = f.read()
        map = eval(a.split('[')[-1].split(']')[0])
    return map

# Takes around 27hrs  to complete on tmux
BRUTE_FORCE_TEST = False
BRUTE_FORCE_LAST_ADDR = 0x000FFFFF
# Recommended to disable debug in brute force test
DEBUG = False

# Number of random addresses for negative testcase
NUM_RANDOM_ADDR = 100000

WHITELISTED_REG_FOR_WRITE_TEST = "0x05013419"
BLACKLISTED_REG_FOR_READ_TEST  = "0x07012316"

def main():
    whitelist = getlist('-w')
    whitelist_table1 = getmap('-W 1')
    whitelist_table2 = getmap('-W 2')
    whitelist_table3 = getlist('-W 3')
    blacklist = getlist('-b')
    blacklist_table1 = getmap('-B 1')
    blacklist_table2 = getmap('-B 2')
    blacklist_table3 = getlist('-B 3')

    try:
        if(BRUTE_FORCE_TEST == True):
            test_brute_force('whitelist',
                             whitelist,
                             whitelist_table1,
                             whitelist_table2,
                             whitelist_table3)
        else:
            test_normal('whitelist',
                        whitelist,
                        whitelist_table1,
                        whitelist_table2,
                        whitelist_table3)
            print ("generated whitelist validation passed")
            test_normal('blacklist',
                        blacklist,
                        blacklist_table1,
                        blacklist_table2,
                        blacklist_table3)
            print ("generated blacklist validation passed")

        # getscom success
        testScomUtil.getscom(0x0204001A)
        print ("getscom success testcase - passed")
        # getscom failure
        testScomUtil.getscom(eval(BLACKLISTED_REG_FOR_READ_TEST), [0x00, 0x05, 0x00, 0x23])
        print ("getscom failure testcase - passed")
        # putscom success
        testScomUtil.putscom(eval(WHITELISTED_REG_FOR_WRITE_TEST), testScomUtil.getscom(eval(WHITELISTED_REG_FOR_WRITE_TEST)))
        print ("putscom success testcase - passed")
        # putscom failure
        while(True):
            random_addr = struct.unpack('>L', os.urandom(4))[0]
            if random_addr not in [eval(a) for a in whitelist]:
                if not ((random_addr & 0x80000000) or (random_addr & 0x00F00000)):
                    testScomUtil.putscom(random_addr, 0, [0x00, 0x05, 0x00, 0x23])
                    break
        print ("putscom failure testcase - passed")
        # modify scom success
        dataWritten = testScomUtil.getscom(0x00040006)
        testScomUtil.modifyScom(0x01, 0x00040006, 0x0)
        dataRead = testScomUtil.getscom(0x00040006)
        if(dataRead != dataWritten):
            raise Exception('modify scom failed %x != %x' % (dataRead, dataWritten))
        print ("modify scom success testcase - passed")
        # putscom under mask success
        dataWritten = testScomUtil.getscom(0x00040006)
        testScomUtil.putScomUnderMask(0x00040006, dataWritten, 0xFFFFFFFFFFFFFFFF)
        dataRead = testScomUtil.getscom(0x00040006)
        if(dataRead != dataWritten):
            raise Exception('PutScom under mask failed %x != %x' % (dataRead, dataWritten))
        print ("putscom under mask success testcase - passed")

        # Greylist test cases
        dataWritten = testScomUtil.getscom(0x0901080B)
        # Do putScomUnderMask with wrong mask
        testScomUtil.putScomUnderMask(0x0901080B, dataWritten, 0xF0FFFFFFFFFFFFFF, [0x00, 0x05, 0x00, 0x23])
        # Do putScom on grey list register
        testScomUtil.putscom(0x0901080B, 0, [0x00, 0x05, 0x00, 0x23])
        # Do putScomUnderMask with exact mask
        testScomUtil.putScomUnderMask(0x0901080B, dataWritten, 0xFF0FFFFFFF0FFFFF)
        # Do putScomUnderMask with superset mask
        testScomUtil.putScomUnderMask(0x0901080B, dataWritten, 0xFF00FFFFFF0FFFFF)
        print ("Greylist testcases - passed")

        # indirect scom test
        dataWritten = testScomUtil.getscom(0x8000000D06010C3F)
        dataiActWritten = (dataWritten & 0x00000000FFFFFFFF) | (0xDECAFFEE00000000)
        testScomUtil.putscom(0x8000000D06010C3F, dataiActWritten)
        dataRead = testScomUtil.getscom(0x8000000D06010C3F)
        if(dataRead != dataWritten):
            raise Exception('indirect scom test failed %x != %x' % (dataRead, dataWritten))
        print ("Indirect scom success testcase - passed")

    except Exception, error:
        raise Exception(error)

def test_normal(id, list, table1, table2, table3):
    # Positive testcase
    num_list = [eval(a) for a in list]
    for addr in num_list:
        if(False == is_present(addr,
                               table1,
                               table2,
                               table3)):
            print id, list
            print ('table1:', table1)
            print ('table2:', table2)
            print ('table3:', table3)
            print ("Failed Addr", hex(addr))
            raise Exception(id+' positive testcase')
    # negative testcase - number of random addresses checked is configurable
    i = 0
    while(i < NUM_RANDOM_ADDR):
        i = i+1
        random_addr = struct.unpack('>L', os.urandom(4))[0]
        if(random_addr in num_list):
            i = i-1
        else:
            if(True == is_present(random_addr, table1, table2, table3)):
                print id, list
                print ('table1:', table1)
                print ('table2:', table2)
                print ('table3:', table3)
                print ("Failed Addr", hex(random_addr))
                raise Exception(id+' negative testcase')

def test_brute_force(id, list, table1, table2, table3):
    num_list = [eval(a) for a in list]
    addr = 0
    last_addr = BRUTE_FORCE_LAST_ADDR
    while(addr <= last_addr):
        if(addr not in num_list):
            sys.stdout.write('\r')
            sys.stdout.write("Progress: addr[0x%08x] last_addr[0x%08x]" % (addr, last_addr) )
            sys.stdout.flush()
            if(True == is_present(addr, table1, table2, table3)):
                print id, list
                print ('table1:', table1)
                print ('table2:', table2)
                print ('table3:', table3)
                print ("Failed Addr", hex(addr))
                raise Exception(id+' brute force testcase')
        addr = addr + 1

def is_present(addr, table1, table2, table3):
    t1_addr = (addr & 0xFF000000) >> 24;
    if(DEBUG):
        print ("find", t1_addr, "in table1")
    for id1, (chiplet_range, count) in enumerate(table1):
        start = (chiplet_range & 0xFF00) >> 8
        end = chiplet_range & 0x00FF
        if( t1_addr >= start and t1_addr <= end ):
            if(id1-1 < 0):
                first_t2 = 0
            else:
                first_t2 = table1[id1-1][1]
            last_t2 = count-1
            t2_addr = (addr & 0x00FF0000) >> 16;
            if(DEBUG):
                print ("find", t2_addr, "in table2", first_t2, last_t2)
            (found, id2) = binary_search(t2_addr, table2, first_t2, last_t2, True)
            if(found):
                if(id2-1 < 0):
                    first_t3 = 0
                else:
                    first_t3 = table2[id2-1][1]
                last_t3 = table2[id2][1]-1
                t3_addr = (addr & 0x0000FFFF)
                if(DEBUG):
                    print ("find",t3_addr,"in table3", first_t3, last_t3)
                (found, id3) = binary_search(t3_addr, table3, first_t3, last_t3)
                if(found):
                    if(DEBUG):
                        print ("found")
                    return True
    return False

def binary_search(item, list, first, last, isMap = False):
    found = False
    found_index = 0

    while(first<=last and not found):
        midpoint = (first+last)//2
        if(isMap):
            ele = list[midpoint][0]
        else:
            ele = list[midpoint]

        if(item == ele):
            found = True
            found_index = midpoint
        else:
            if(item < ele):
                last = midpoint-1
            else:
                first = midpoint+1

    return (found, found_index)

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
if __name__=="__main__":
    if testUtil.getMachineName() == "axone":
        try:
            main()
        except:
            print ( "\nTest Suite completed with error(s)" )
            testUtil.collectFFDC()
            raise()

        print ( "\nTest Suite completed with no errors" )
    else:
        main()

        if err:
           print ("\nTest Suite completed with error(s)")
        else:
           print ("\nTest Suite completed with no errors")
