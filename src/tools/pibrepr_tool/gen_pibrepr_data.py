# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/pibrepr_tool/gen_pibrepr_data.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2018,2020
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

bin_file = open("pibrepr.bin", "wb")

############################################################
#############       Format of the Data       ###############
## 1st 1B of Command - (1) Putscom (0) Getscom
## 2nd 1B for end row indication
## 3rd & 4th Bytes are the pad bytes - 2B Pad
## 5th,6th,7th,8th Byte are the address bytes - 4B Addr
## Next 8Bytes are the data - 8B Data
## -------------------------------------------------------##
## Total ring length of the repr data is 1263bits, which is
## 1199 bits actual chain length + 64bits header.
## perv_pib_repr 0x01031006 707 N N Y Y PERVPIB OFF
############################################################
string = '''
           01 00 00 00 01 03 00 05 02 00 00 00 00 00 02 00
           01 00 00 00 01 03 E0 40 A5 A5 A5 A5 A5 A5 A5 A5
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 63 60
           01 00 00 00 01 03 E0 40 20 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 40 00 00 00 00 00 00 00 00
           01 00 00 00 01 03 E0 2F 00 00 00 00 00 00 00 00
           00 00 00 00 01 03 80 00 A5 A5 A5 A5 A5 A5 A5 A5
           01 01 00 00 01 03 00 05 00 00 00 00 00 00 00 00'''
string = string.split()
string = [int(x,16) for x in string]
string = bytearray(string)
bin_file.write(string)
bin_file.close()
