#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/security/securityRegListGen.py $
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
import getopt
import sys
import os
import csv

# Exit codes
SUCCESS       = 0
INVALID_USAGE = 1
PRINT_AND_EXIT= 2

SECURITY_LIST = "p9_security_white_black_list.csv"
DEBUG         = False
VERBOSE       = False

GEN_FILE      = "sbeSecurityGen.H"

# csv tags
TAG_BASE_ADDR     = 'Base Address'
TAG_CHIPLET       = 'Chiplet'
TAG_CHIPLET_RANGE = 'Chiplet Id - range'
TAG_VERSION       = 'Version'
TAG_TYPE          = 'Type'
TAG_BIT_MASK      = 'Bit Mask'

TAG_NAME_WHITELIST = 'write_whitelist'
TAG_NAME_GREYLIST = 'write_greylist'
TAG_NAME_BLACKLIST = 'read_blacklist'

def usage():
    print "usage: p9_security_list_gen.py [-h] [-f <security_list_path>] [-0 <output directory] [i] [-d] [-v]\n\
arguments:\n\
-h, --help               show this help message and exit\n\
-f, --file               path to the security list csv file\n\
-o, --output             output directory\n\
-w, --whitelist          print whitelist read from csv\n\
-b, --blacklist          print blacklist read from csv\n\
-g, --greylist           print greylist read from csv\n\
-i, --info               get version info of the security list\n\
-d, --debug              enable debug traces\n\
-v, --verbose            enable verbose traces"

def exit(error, msg = ''):
    if(error == SUCCESS):
        return 0
    elif(error == INVALID_USAGE):
        print msg
        usage()
    elif(error == PRINT_AND_EXIT):
        print msg
    else:
        if(DEBUG):
            print "unknown error:exiting"
    sys.exit(1)

def remove_duplicates(xlist):
    xlist = list(set(xlist))
    return xlist

def remove_zeroes(list):
    out_list = []
    for a in list:
        if not a == 0x00:
            out_list += [a]
    return out_list

def gen_file(whitelist_tables, blacklist_tables, greyList):
    global GEN_FILE

    header = ("#ifndef __SBE_SECURITY_GEN_H\n"+
              "#define __SBE_SECURITY_GEN_H\n\n"+
              "#include \"sbeSecurity.H\"\n\n"+
              "#include \"securityAlgo.H\"\n\n"+
              "using namespace SBE_SECURITY;\n\n")

    tables = (('WHITELIST', 'whitelist', whitelist_tables),
              ('BLACKLIST', 'blacklist', blacklist_tables))
    body = ''
    # table 1 range and running count type
    table1_range_type = "uint8_t"
    table1_index_type = "uint8_t"
    # table 2 value and running count type
    table2_value_type = "uint8_t"
    table2_index_type = "uint16_t"
    # table 3 value type
    table3_value_type = "uint16_t"
    for namespace, tablename, table in tables:
        body += ("""
namespace """+namespace+"""
{
    /*
    table 1:
       keys   = 2byte - (start , end) of
                the ranges in bit 0-7 of the 32-bit address
       values = running count of the paths to table2

    for example - if ranges are 0x20-0x37, 0x01-0x01, 0x10-0x17 and has
                  1, 2 and 3 paths respectively to table 2
    then table 1 will have {0x20,0x37} = 01, {0x01,0x01} = 3, {0x10,0x17} = 7

    1 byte for running count - we are good with uint8_t till the
    total paths are less than 256
    */
    _t1_t _t1[]  = {
    // length of the table = """+s_list_len(table[0])+"""
"""+s_table1_gen(tablename, table[0])+"""
                                                          };

    /*
    table 2
       keys   = unique 1 byte numbers having same prefix for each
                range in table 1 key - bit 8-15 from a 32-bit address
       values = running count of paths from each of the keys

    for example - if element a has 1 path, b has 0 and c has 3 paths
    then table 1 will have a = 1, b = 1, c = 4

    1 byte for key
    2 byte for number of paths
    We are good with uint16_t,
    till the number of paths to table 3 from each key is less than 65536
    */
    _t2_t _t2[] = {
    // length of the table = """+s_list_len(table[1])+"""
"""+s_table2_gen(tablename, table[1])+"""
                                      };
    /*
    table 3
       values = 2 byte value bit 16-31 of the 32-bit address
    */
    _t3_t _t3[] = {
    // length of the table = """+s_list_len(table[2])+"""
"""+s_table3_gen(tablename, table[2])+"""
                      };
    _t1_table_t t1 =
            {sizeof(_t1)/sizeof(_t1_t),
             0xFF000000,
             _t1};
    _t2_table_t t2 =
            {sizeof(_t2)/sizeof(_t2_t),
             0x00FF0000,
             _t2};
    _t3_table_t t3 =
            {sizeof(_t3)/sizeof(_t3_t),
             0x0000FFFF,
             _t3};

    bool isPresent(uint32_t i_addr)
    {
        return _is_present(t1, t2, t3, i_addr);
    }
}""")

    greylist_addr_type = "uint32_t"
    greylist_mask_type = "uint64_t"
    body += ("""
namespace GREYLIST
{
    /*
    table 1:
       Address   = 4 byte
       Mask      = 8 byte
    */
    _gl_t1_t _t1[] = {
"""+s_greylist_table_gen(greyList)+"""
                                      };

    _gl_t1_table_t t1 =
            {sizeof(_t1)/sizeof(_gl_t1_t),
             0xFFFFFFFF,
             _t1};

    bool isPresent(uint32_t i_addr, uint64_t i_mask)
    {
        return _is_present(t1, i_addr, i_mask);
    }
}""")
    footer = "\n#endif //__SBE_SECURITY_GEN_H"

    with open(GEN_FILE, 'w') as o_f:
        o_f.write(header)
        o_f.write(body)
        o_f.write(footer)

'''
Functions related to address translation
'''
EQ_CHIPLET_OFFSET   = 0x10
EQ_TARGET_COUNT     = 6
CORE_CHIPLET_OFFSET = 0x20
CORE_TARGET_COUNT   = 24
CORES_PER_EX        = 2

def get_chiplet(addr):
    chiplet = (addr & 0xFF000000) >> 24
    return chiplet
def set_chiplet(addr, chiplet):
    rAddr = ((addr & 0x00FFFFFF) | (chiplet << 24)) & 0xFFFFFFFF
    return rAddr
def get_ring(addr):
    ring = (addr & 0x00003C00) >> 10
    return ring
def set_ring(addr, ring):
    rAddr = ((addr & 0xFFFFC3FF) | (ring << 10)) & 0xFFFFFFFF
    return rAddr
def get_satId(addr):
    satId = (addr & 0x000003C0) >> 6
    return satId
def set_satId(addr, satId):
    rAddr = ((addr & 0xFFFFFC3F) | (satId << 6)) & 0xFFFFFFFF
    return rAddr

def get_effective_address(chiplet, list):
    base_chiplet = ((list[0] & 0xFF000000) >> 24)
    base_chiplet_type = ''
    if((base_chiplet >= EQ_CHIPLET_OFFSET) and
        (base_chiplet < (EQ_CHIPLET_OFFSET+EQ_TARGET_COUNT))):
        base_chiplet_type = 'EQ'
    elif((base_chiplet >= CORE_CHIPLET_OFFSET) and
        (base_chiplet < (CORE_CHIPLET_OFFSET+CORE_TARGET_COUNT))):
        base_chiplet_type = 'CORE'

    effective_addr_list = []
    for addr in list:
        if(chiplet == 'EX'):
            chiplet_id = get_chiplet(addr)
            instance = (chiplet_id - CORE_CHIPLET_OFFSET) / CORES_PER_EX
            if(base_chiplet_type == 'EQ'):
                # Chiplet num is already embedded;
                # Add both even and odd EX under each EQ
                # effective ring
                ring = get_ring(addr)
                ring = (ring - (ring % 2)) + 0
                effective_addr_list += [set_ring(addr, ring)]
                ring = (ring - (ring % 2)) + 1
                effective_addr_list += [set_ring(addr, ring)]
            elif(base_chiplet_type == 'CORE'):
                # get EX associated with each core,
                # duplicates would be removed later
                chiplet_id = (CORE_CHIPLET_OFFSET + (chiplet_id % 2) +
                                                            (instance * 2))
                effective_addr_list += [set_chiplet(addr, chiplet_id)]
            else:
                exit(PRINT_AND_EXIT, "Invalid chiplet id for EX chiplet type")
# MCS and PHB targets are not being used as of now
#        elif(chiplet == 'MCS' or chiplet == 'MI'):
#            chiplet_id = get_chiplet(addr)
#            instance = chiplet_id - MCS_TARGET_OFFSET
#            # Chiplet num is already embedded
#            # set satellite id
#            satId = (2 * (instance % 2))
#            effective_addr_list += [set_satId(addr, satId)]
#        elif(chiplet == 'PHB'):
#            chiplet_id = get_chiplet(addr)
#            instance = chiplet_id - PHB_TARGET_OFFSET
#            satId = get_satId(addr)
#            if(base_chiplet_type == 'N2'):
#                if(instance == 0):
#                    addr = set_ring(addr, 0x3)
#                    satId = 1 if (satId < 4) else 4
#                else:
#                    add = set_ring((0x3 + (instance / 3) + 1) & 0xF)
#                    satId = ((1 if (satId < 4) else 4) +
#                             (0 if (instance % 2) else 1) +
#                             (2 * (instance / 5)))
#            else:
#                if(instance == 0):
#                    satId = 1 if (satId < 4) else 4
#                else:
#                    satId = ((1 if (satId < 4) else 4) +
#                    (0 if (instance % 2) else 1) +
#                    (2 * (instance / 5)))
#            effective_addr_list += [set_satId(addr, satId)]
        else:
            effective_addr_list += [addr]
    return effective_addr_list

def s_list_hex(prefix, list, num_chars):
    fmt_string = '0x%0'+str(num_chars)+'x'
    return prefix+str([fmt_string % ele for ele in list])

def s_list_len(list):
    return str(len(list))

def get_tables(id, list):
    # -----------------------------------------------------------------#
    # Step 1:             [register list]                              #
    #                           ||                                     #
    #                           \/                                     #
    #         {base_address1 : [chiplet range1, chiplet range2]}       #
    #..................................................................#
    # Eg: [01ABCDEF, 02ABCDEF, 04ABCDEF, 05ABCDEF, 10UVWXYZ, 11UVWXYZ] #
    #                           ||                                     #
    #                           \/                                     #
    #         {ABCDEF : [0102, 0405], UVWXYZ : [1011]}                 #
    # -----------------------------------------------------------------#
    table_base_to_range = {}
    # get a list of unique base addressess
    base_addr = [ele & 0x00FFFFFF for ele in list]
    base_addr = remove_duplicates(base_addr)

    # for each base address, find the chiplet ranges
    table1_range_keys = []
    for base_ele in base_addr:
        # get a list of chiplet ids for the base address
        ele_list = []
        for list_ele in list:
            if((list_ele & 0x00FFFFFF) == base_ele):
                ele_list += [(list_ele & 0xFF000000) >> 24]
        ele_list = remove_duplicates(ele_list)
        ele_list.sort()
        # prepare a list of unique ranges
        range_val = [ele_list[0]<<8 | ele_list[0]]
        for ele_list_ele in ele_list:
            # each consecutive number found expands the existing range
            # and the non consecutive number adds a new range
            if(ele_list_ele == (range_val[-1]&0xFF)+1):
                range_val[-1] = (range_val[-1] & 0xFF00) | ele_list_ele
            elif(ele_list_ele != (range_val[-1]&0xFF00 >> 8)):
                range_val += [ele_list_ele<<8 | ele_list_ele]
        table_base_to_range[base_ele] = range_val

    # -----------------------------------------------------------------#
    # Step 2: {base_address1 : [chiplet range1, chiplet range2]}       #
    #                           ||                                     #
    #                           \/                                     #
    #         {chiplet range1 : [base_address1, base_address2]}        #
    #..................................................................#
    # Eg:     {ABCDEF : [0102, 0405], UVWXYZ : [0405, 1020]}           #
    #                           ||                                     #
    #                           \/                                     #
    #     {0102 : [ABCDEF], 0405 : [ABCDEF, UVWXYZ], 1020 : [UVWXYZ]}  #
    # -----------------------------------------------------------------#
    table_range_to_base = {}
    # get unique ranges
    for key, values in table_base_to_range.items():
        for val in values:
            if(val not in table1_range_keys):
                table1_range_keys += [val]
    # for each range find the base addressess
    for range_ele in table1_range_keys:
        table_range_to_base[range_ele] = []
        for key, values in table_base_to_range.items():
            if(range_ele in values):
                table_range_to_base[range_ele] += [key]
        table_range_to_base[range_ele] = remove_duplicates(table_range_to_base[range_ele])
        table_range_to_base[range_ele].sort()

    # -----------------------------------------------------------------#
    # Step 3: {chiplet range1 : [base_address1, base_address2]}        #
    #                           ||                                     #
    #                           \/                                     #
    #         {chiplet range1 : {key1 : [base_addr1, base_Addr2]}}     #
    #..................................................................#
    # Eg: {0102 : [ABCDEF], 0405 : [ABCDEF, UVWXYZ], 1020 : [UVWXYZ]}  #
    #                           ||                                     #
    #                           \/                                     #
    #               {0102 : {AB : [CDEF]},                             #
    #                0405 : {AB : [CDEF], UV : [WXYZ]},                #
    #                1020 : {UV : [WXYZ]}}                             #
    # -----------------------------------------------------------------#
    table_range_to_key_to_base = []
    for key, values in table_range_to_base.items():
        # prepare a list of table 2 keys for each range
        temp_keys = [(val & 0x00FF0000)>> 16 for val in values]
        temp_keys = remove_duplicates(temp_keys)
        temp_keys.sort()
        l = []
        for temp_key in temp_keys:
            # for each key associated with range get a list of base addressess
            a = []
            for val in values:
                if(((val & 0x00FF0000)>>16) == temp_key):
                    a += [val & 0x0000FFFF]
            l.append((temp_key, a))
        table_range_to_key_to_base.append((key, l))

    # -----------------------------------------------------------------#
    # Step 4: {chiplet range1 : {key1 : [base_addr1, base_Addr2]}}     #
    #                           ||                                     #
    #                           \/                                     #
    #                  table1, table2, table3                          #
    #..................................................................#
    # Eg:           {0102 : {AB : [CD, EF]},                           #
    #                0405 : {AB : [CD, EF], UV : [WX, YZ]},            #
    #                1020 : {UV : [WX, YZ]}}                           #
    #                           ||                                     #
    #                           \/                                     #
    #         table1: {0102 : 1, 0405 : 3, 1020 : 4}                   #
    #         table2: {AB : 2, AB : 4, UV : 6, UV : 8}                 #
    #         table3: {CD, EF, CD, EF, WX, YZ, WX, YZ}                 #
    # -----------------------------------------------------------------#
    """
    table 1:
       keys   = 2byte - (start , end) of
                the ranges in bit 0-7 of the 32-bit address
       values = running count of the paths to table2

    for example - if ranges are 0x20-0x37, 0x01-0x01, 0x10-0x17 and has
                  1, 2 and 3 paths respectively to table 2
    then table 1 will have {0x20,0x37} = 01, {0x01,0x01} = 3, {0x10,0x17} = 7

    1 byte for running count - we are good with uint8_t till the
    total paths are less than 256
    """
    table1 = []

    """
    table 2
       keys   = unique 1 byte numbers having same prefix for each
                range in table 1 key - bit 8-15 from a 32-bit address
       values = running count of paths from each of the keys

    for example - if element a has 1 path, b has 0 and c has 3 paths
    then table 1 will have a = 1, b = 1, c = 4

    1 byte for key
    1 byte for number of paths
    We are good with uint8_t,
    till the number of paths to table 3 from each key is less than 256
    """
    table2 = []

    """
    table 3
       values = 2 byte value bit 16-31 of the 32-bit address
    """
    table3 = []

    running_count_table1 = 0
    running_count_table2 = 0
    for key_m, tree in table_range_to_key_to_base:
        running_count_table1 += len(tree)
        table1.append((key_m, running_count_table1))
        for key, values in tree:
            temp_keys = values
            temp_keys = remove_duplicates(temp_keys)
            temp_keys.sort()
            running_count_table2 += len(temp_keys)
            table2.append((key, running_count_table2))
            # table 3 values will just be the base addressess
            # for each range and key combination in order
            table3 += temp_keys
    if(VERBOSE):
        print id+" table3 keys len ["+s_list_len(table3)+"]"

    if(VERBOSE):
        print id,"table1:", ['0x%04x:0x%02x' % ele for ele in table1]
        print id,"table2:", ['0x%02x:0x%02x' % ele for ele in table2]
        print id,"table3:", ['0x%04x' % ele for ele in table3]
    if(DEBUG):
        print id,"table1 len ["+s_list_len(table1)+"]"
        print id,"table2 len ["+s_list_len(table2)+"]"
        print id+" table3 len ["+s_list_len(table3)+"]"

    return (table1, table2, table3)

def s_table1_gen(id, table):
    # write table 1 string
    str_table1 = ""
    for i,(key, value) in enumerate(table):
        str_table1 += '{0x%02x, 0x%02x, 0x%02x}, ' % (((key & 0xFF00) >> 8),
                                                        (key & 0x00FF),
                                                        value)
        if(0 == ((i+1) % 4)):
            str_table1 = str_table1[:-1]
            str_table1 += '\n'
    str_table1 = str_table1[:-1]
    if(VERBOSE):
        print id+" generated table1"
        print str_table1
    return str_table1

def s_table2_gen(id, table):
    # write table 2 string
    str_table2 = ""
    for i,(key, value) in enumerate(table):
        str_table2 += '{0x%02x, 0x%02x}, ' % (key, value)
        if(0 == ((i+1) % 4)):
            str_table2 = str_table2[:-1]
            str_table2 += '\n'
    str_table2 = str_table2[:-1]
    if(VERBOSE):
        print id+" generated table2"
        print str_table2
    return str_table2

def s_table3_gen(id, table):
    # write table 3 string
    str_table3 = ""
    for i,value in enumerate(table):
        str_table3 += '0x%04x, ' % (value)
        if(0 == ((i+1) % 8)):
            str_table3 = str_table3[:-1]
            str_table3 += '\n'
    str_table3 = str_table3[:-1]
    if(VERBOSE):
        print id+" generated table3"
        print str_table3
    return str_table3

def s_greylist_table_gen( greyList):
    # write greylist string
    str_table = ""
    for ele in greyList:
        str_table += '{0x%08x, 0x%016xull}, ' % (ele[0], ele[1])
    str_table = str_table[:-1]
    if(VERBOSE):
        print " greylist table"
        print str_table
    return str_table
def main(argv):

    try:
        opts, args = getopt.getopt(sys.argv[1:],
                "f:o:wbgidvhW:B:",
                                   ['file=', 'output=', 'whitelist', 'blacklist', 'greylist', 'info', 'debug', 'verbose', 'help', 'wt=', 'bt='])
    except getopt.GetoptError as err:
        exit(INVALID_USAGE, str(err))

    # Parse the command line arguments
    global DEBUG, VERBOSE, SECURITY_LIST, GEN_FILE

    print_info = None
    wt = -1
    bt = -1
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            exit(INVALID_USAGE)
        elif opt in ('-d', '--debug'):
            DEBUG = True
        elif opt in ('-v', '--verbose'):
            DEBUG = True
            VERBOSE = True
        elif opt in ('-i', '--info'):
            print_info = "version"
        elif opt in ('-w', '--whitelist'):
            print_info = "whitelist"
        elif opt in ('-b', '--blacklist'):
            print_info = "blacklist"
        elif opt in ('-g', '--greylist'):
            print_info = "greylist"
        elif opt in ('-f', '--file'):
            assert os.path.exists(arg), "file doesn't exist at:"+str(arg)
            SECURITY_LIST = str(arg)
        elif opt in ('-W', '--wt'):
            wt = int(arg)
            print_info = "whitelist_table"
        elif opt in ('-B', '--bt'):
            print_info = "blacklist_table"
            bt = int(arg)
        elif opt in ('-o', '--output'):
            assert os.path.exists(arg), "directory doesn't exist at:"+str(arg)
            GEN_FILE = str(arg)+"/"+GEN_FILE

    if(DEBUG):
        print "file ["+str(SECURITY_LIST)+"]"
        print "output ["+str(GEN_FILE)+"]"

    # Read the security list file
    version   = 'unknown'
    whitelist = []
    blacklist = []
    greylist = []
    with open(SECURITY_LIST, 'rbU') as f:
        reader = csv.DictReader(f)
        for idx, row in enumerate(reader):
            try:
                if(version.strip().lower() == 'unknown'):
                    version = row[TAG_VERSION]
                base_addr = row[TAG_BASE_ADDR].strip().lower().split('0x')[-1]
                # Append 0s for numbers represented by less than 8 chars
                base_addr = '0'*(8-len(base_addr))+base_addr
                # Extract the least 32 bit number for base address
                base_addr = base_addr[len(base_addr)-8:]
                base_addr = int(base_addr, 16)
                if(VERBOSE):
                    print "base["+'0x%08x' % base_addr + "]"
                bit_mask = row[TAG_BIT_MASK].strip()
                if not bit_mask:
                    bit_mask = 0
                else:
                    bit_mask = int( bit_mask.lower().split('0x')[-1], 16)

                chiplet_range = row[TAG_CHIPLET_RANGE].split('-')
                # Empty range field considered as error
                if(chiplet_range[0] == ''):
                    exit(PRINT_AND_EXIT, "Missing chiplet id range")
                if(chiplet_range[0].strip().lower() != '0x00'):
                    if(chiplet_range[0].strip().lower() != '0x%02x' % (get_chiplet(base_addr))):
                        print "base_addr",hex(base_addr)
                        print "get_chiplet(base_addr)",hex(get_chiplet(base_addr))
                        print "chiplet_range[0]", chiplet_range[0]
                        exit(PRINT_AND_EXIT, "Base address is not consistent")
                    base_addr = base_addr & 0x00FFFFFF
                chiplet_range = [int(ele, 16) for ele in chiplet_range]
                # Expand base address with ranges
                expanded_range = []
                for ele in range(chiplet_range[0], chiplet_range[-1]+1):
                    expanded_range += [ele*(2**24)]
                expanded_line = [(base_addr + ele) for ele in expanded_range]
                expanded_line = get_effective_address(row[TAG_CHIPLET], expanded_line)
                if(VERBOSE):
                    print s_list_hex("range:", expanded_range, 8)
                if(row[TAG_TYPE].strip().lower() == TAG_NAME_GREYLIST):
                   if(( bit_mask == 0 ) or ( bit_mask == 0xffffffffffffffff)):
                        exit(PRINT_AND_EXIT, "Wrong mask for Greylist")
                   greylist_line = expanded_line
                   if(VERBOSE):
                        print s_list_hex("greylist_line:", greylist_line, 8)
                        print "mask:", bit_mask
                   for ele in greylist_line:
                        greylist.append((ele, bit_mask))
                elif(row[TAG_TYPE].strip().lower() == TAG_NAME_WHITELIST):
                    whitelist_line = expanded_line
                    if(VERBOSE):
                        print s_list_hex("whitelist_line:", whitelist_line, 8)
                    whitelist += whitelist_line
                elif(row[TAG_TYPE].strip().lower() == TAG_NAME_BLACKLIST):
                    blacklist_line = expanded_line
                    if(VERBOSE):
                        print s_list_hex("blacklist_line:", blacklist_line, 8)
                    blacklist += blacklist_line

            except:
                print "Error in line ["+str(idx+2)+"]"
                exit(PRINT_AND_EXIT, sys.exc_info()[0])

    whitelist = remove_duplicates(whitelist)
    whitelist = remove_zeroes(whitelist)
    whitelist.sort()
    blacklist = remove_duplicates(blacklist)
    blacklist = remove_zeroes(blacklist)
    blacklist.sort()
    greylist = remove_duplicates(greylist)
    greylist.sort()

    if(print_info == 'version'):
        exit(PRINT_AND_EXIT, "security list version ["+version+"]")
    if(print_info == 'whitelist'):
        exit(PRINT_AND_EXIT, s_list_hex("whitelist:", whitelist, 8))
    if(print_info == 'blacklist'):
        exit(PRINT_AND_EXIT, s_list_hex("blacklist:", blacklist, 8))
    if(print_info == 'greylist'):
        exit(PRINT_AND_EXIT, greylist)

    if(VERBOSE):
        print s_list_hex("whitelist:", whitelist, 8)
        print s_list_hex("blacklist:", blacklist, 8)
    if(DEBUG):
        print "security list version ["+version+"]"
        print "Whitelist len ["+s_list_len(whitelist)+"]"
        print "Blacklist len ["+s_list_len(blacklist)+"]"
        print "Greylist len ["+s_list_len(greylist)+"]"

    whitelist_tables = get_tables("Whitelist", whitelist)
    blacklist_tables = get_tables("Blacklist", blacklist)

    if(print_info == 'whitelist_table'):
        exit(PRINT_AND_EXIT, "whitelist_table["+str(wt-1)+"]" + str(whitelist_tables[wt-1]))
    if(print_info == 'blacklist_table'):
        exit(PRINT_AND_EXIT, "blacklist_table["+str(bt-1)+"]" + str(blacklist_tables[bt-1]))

    # Generate output file
    gen_file(whitelist_tables, blacklist_tables, greylist)

    exit(SUCCESS)

if __name__ == "__main__":
    main(sys.argv)
