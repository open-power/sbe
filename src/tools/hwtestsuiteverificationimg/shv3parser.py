#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/securitytestsuite/shv3parser.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2024
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

import argparse
import sys
import ctypes
from ctypes import (
    c_uint32, c_uint64, c_uint8, c_uint16,
    BigEndianStructure, sizeof, Union
)

# Secure header size
v1_secure_header_size = 1288
v3_secure_header_size = 15360

'''
v3 secure header name
'''
header_name = ['hw', 'prefix', 'fw']


'''
v3 secure header fields name
'''
sh_field_name = [
    'magic_number', 'container_version', 'container_size', 'reserved', 'hw_ecdsa_pub_A', 'hw_mldsa_pub_D',
    'header_version', 'hash_algo', 'sign_algo', 'flags', 'fw_key_count', 'size_protected_keys_payload', 'hash_protected_keys_payload', 'ecid', 'hw_sig_A', 'hw_sig_D', 'fw_ecdsa_pub_key_P', 'fw_mldsa_pub_key_S',
    'component_id', 'fw_secure_version', 'size_protected_payload', 'size_unprotected_payload', 'hash_protected_payload', 'fw_sig_P', 'fw_sig_S'
]


class HwHeader_t(BigEndianStructure):
    """
    v3 Secure header Hardware header structure
    """
    _pack_ = 1
    _fields_ = [
        ("magic_number"         , c_uint32), # 4 bytes
        ("container_version"    , c_uint16), # 2 Bytes
        ("container_size"       , c_uint64), # 8 Bytes
        ("reserved"             , c_uint8 * 6),   # 6 Bytes
        ("hw_ecdsa_pub_A"       , c_uint8 * 132), # 132 Bytes
        ("hw_mldsa_pub_D"       , c_uint8 * 2592) # 2592 Bytes
    ]

    def __int__(self):
        # Convert the entire structure to an integer
        return int.from_bytes(bytes(self), byteorder='big')

    def __getitem__(self, key):
        for field_name, field_type in self._fields_:
            if field_name == key:
                return getattr(self, field_name)
        raise KeyError(f"Field '{key}' not found in Hw1Header_t structure")

    def __setitem__(self, key, value):
        for field_name, field_type in self._fields_:
            if field_name == key:
                setattr(self, field_name, value)
                return
        raise KeyError(f"Field '{key}' not found in Hw1Header_t structure")


class PrefixHeader_t(BigEndianStructure):
    """
    v3 Secure header Prefix header structure
    """
    _pack_ = 1
    _fields_ = [
        ("header_version"    , c_uint16),
        ("hash_algo"         , c_uint8 ),
        ("sign_algo"         , c_uint8 ),
        ("reserved"          , c_uint64),
        ("flags"             , c_uint32),
        ("fw_key_count"      , c_uint8 ),
        ("size_protected_keys_payload", c_uint64),
        ("hash_protected_keys_payload", c_uint8 * 64),
        ("ecid"              , c_uint8 * 16),
        ("reserved1"         , c_uint8 * 3),
        ("hw_sig_A"          , c_uint8 * 132),
        ("hw_sig_D"          , c_uint8 * 4627),
        ("fw_ecdsa_pub_key_P", c_uint8 * 132),
        ("fw_mldsa_pub_key_S", c_uint8 * 2592)
    ]

    def __int__(self):
        # Convert the entire structure to an integer
        return int.from_bytes(bytes(self), byteorder='big')

    def __getitem__(self, key):
        for field_name, field_type in self._fields_:
            if field_name == key:
                return getattr(self, field_name)
        raise KeyError(f"Field '{key}' not found in Hw1Header_t structure")

    def __setitem__(self, key, value):
        for field_name, field_type in self._fields_:
            if field_name == key:
                setattr(self, field_name, value)
                return
        raise KeyError(f"Field '{key}' not found in Hw1Header_t structure")


class FwHeader_t(BigEndianStructure):
    """
    v3 Secure header Firmware header structure
    """
    _pack_ = 1
    _fields_ = [
        ("header_version"    , c_uint16 ),
        ("hash_algo"         , c_uint8  ),
        ("unused"            , c_uint8  ),
        ("reserved"          , c_uint64 ),
        ("component_id"      , c_uint64 ),
        ("flags"             , c_uint32 ),
        ("fw_secure_version" , c_uint8  ),
        ("size_protected_payload"    , c_uint64 ),
        ("size_unprotected_payload"  , c_uint64 ),
        ("hash_protected_payload"    , c_uint8 * 64),
        ("ecid"              , c_uint8 * 16 ),
        ("reserved1"         , c_uint8 * 7  ),
        ("fw_sig_P"          , c_uint8 * 132),
        ("fw_sig_S"          , c_uint8 * 4627)
    ]

    def __int__(self):
        # Convert the entire structure to an integer
        return int.from_bytes(bytes(self), byteorder='big')

    def __getitem__(self, key):
        for field_name, field_type in self._fields_:
            if field_name == key:
                return getattr(self, field_name)
        raise KeyError(f"Field '{key}' not found in Hw1Header_t structure")

    def __setitem__(self, key, value):
        for field_name, field_type in self._fields_:
            if field_name == key:
                setattr(self, field_name, value)
                return
        raise KeyError(f"Field '{key}' not found in Hw1Header_t structure")


class v3Secure_t(BigEndianStructure):
    """
    v3 Secure header structure
    """
    _pack_ = 1
    _fields_ = [
        ("hw"     , HwHeader_t),
        ("prefix" , PrefixHeader_t),
        ("fw"     , FwHeader_t)
    ]

    def __int__(self):
        # Convert the entire structure to an integer
        return int.from_bytes(bytes(self), byteorder='big')

    def __getitem__(self, key):
        # Check if the key is a top-level field
        if key in [f[0] for f in self._fields_]:
            field_value = getattr(self, key)
            if isinstance(field_value, BigEndianStructure):
                return field_value
            return field_value

        # If it's not a top-level field, try to access as nested structure field
        for field_name, field_type in self._fields_:
            if isinstance(getattr(self, field_name), BigEndianStructure):
                nested_structure = getattr(self, field_name)
                if key in [f[0] for f in nested_structure._fields_]:
                    return nested_structure[key]

        raise KeyError(f"Field '{key}' not found in vHeader_t structure")

    def __setitem__(self, key, value):
        if key in [f[0] for f in self._fields_]:
            field_value = getattr(self, key)
            if isinstance(field_value, BigEndianStructure):
                # Ensure value is an instance of the same type
                if not isinstance(value, type(field_value)):
                    raise ValueError(f"Value must be of type {type(field_value).__name__}")
                setattr(self, key, value)
            else:
                setattr(self, key, value)
            return

        # If it's not a top-level field, try to set as nested structure field
        for field_name, field_type in self._fields_:
            if isinstance(getattr(self, field_name), BigEndianStructure):
                nested_structure = getattr(self, field_name)
                if key in [f[0] for f in nested_structure._fields_]:
                    nested_structure[key] = value
                    return

        raise KeyError(f"Field '{key}' not found in vHeader_t structure")



class v3SecureHeader_t(Union):
    """
    Union that holds either HwHeader_t, Hw1Header_t, or raw data
    """
    _fields_ = [
        ("vheader", v3Secure_t),                # The structured view
        ("data", c_uint8 * sizeof(v3Secure_t))  # The raw byte view
    ]

    @property
    def hw(self):
        return self.vheader.hw

    @property
    def prefix(self):
        return self.vheader.prefix

    @property
    def fw(self):
        return self.vheader.fw


def to_bytes(value):
    """
    Convert interger to bytes

    Args:
        value: interger value

    Return:
        bytes value
    """
    try:
        byte_length = (value.bit_length() + 7) // 8  # Calculate the number of bytes needed
        byte_order = 'big'  # Big-endian (most significant byte first)

        return value.to_bytes(byte_length, byte_order)
    except:
        return bytes(value)


def get_nested_struct_field_metadata(parent_structure, parent_field_name, nested_field_name):
    """
    /**
    * @brief Get metadata of a nested field within a parent structure.
    *
    * This function retrieves the type, offset, and size of a field from a nested structure
    * that is part of a parent structure. The offset is computed relative to the parent structure.
    *
    * @param parent_structure The parent structure class (e.g., `vHeader_t`).
    * @param parent_field_name The name of the field in the parent structure that holds the nested structure (e.g., `"hw"`).
    * @param nested_field_name The name of the field within the nested structure whose metadata is being retrieved (e.g., `"b1"`).
    *
    * @return A formatted tuple absolute offset and size of teh field in the form of:
    *         (absolute offset, size), where:
    *         - OFFSET is the byte offset of the nested field relative to the start of the parent structure.
    *         - SIZE is the size of the field in bytes.
    *
    * Example usage:
    * @code
    * print(get_nested_field_metadata(vHeader_t, "hw", "b1"))
    * @endcode
    *
    * @note This function assumes that the fields exist in the parent and nested structures.
    */
    """
    # Get the parent field type and offset
    parent_field_type = dict(parent_structure._fields_)[parent_field_name]
    parent_field_offset = parent_structure.__dict__[parent_field_name].offset

    # Get the nested field type, offset, and size
    nested_field_type = dict(parent_field_type._fields_)[nested_field_name]
    nested_field_offset = parent_field_type.__dict__[nested_field_name].offset
    nested_field_size = sizeof(nested_field_type)

    # Compute the absolute offset relative to the parent structure
    absolute_offset = parent_field_offset + nested_field_offset

    return (absolute_offset, nested_field_size)

def from_bytes(byteData, byteorder='big'):
    """
    Convert bytes to integer

    Args:
        byteData: bytes data
        byteorder: byte order ('big' or 'little')

    Return:
        integer value
    """
    try:
        if (len(byteData) % 2) or (len(byteData) > 8):
            return byteData
        else:
            return int.from_bytes(byteData, byteorder)
    except Exception as e:
        return byteData


def getHeader(args):
    """
    Get header data from given header and field

    Args:
        args: argparser argument
    """
    # Loading input image
    with open(args.image, 'rb') as f:
        data = f.read()
        v3StartOffset = 0

        # Check the file contain v3 along with V1
        # First 1288 bytes of V1
        # later 15360 bytes of v3
        if args.fileHasV1 == "True":
            # skip the sh v1 data to access the v3
            v3StartOffset = v1_secure_header_size
            v3sh_bytes = data[v3StartOffset: ]

        # Fetching the required arg data
        header = args.header
        field  = args.field

        v3sh = v3SecureHeader_t.from_buffer(bytearray(v3sh_bytes))
        parse_output = to_bytes( v3sh.vheader[header][field] )

        print ("".join("{:02x}".format(num) for num in list(parse_output)))

        if getattr(args, "output") != None:
            with open(args.output, 'wb') as wf:
                wf.write(parse_output)


def setHeader(args):
    # Loading input image
    with open(args.image, 'rb') as f:
        data = f.read()
        v3StartOffset = 0

        # Check the file contain v3 along with V1
        # First 1288 bytes of V1
        # later 15360 bytes of v3
        if args.fileHasV1 == "True":
            # skip the sh v1 data to access the v3
            v3StartOffset = v1_secure_header_size
            data_v3 = data[v3StartOffset: ]

        with open(args.binary, 'rb') as fb:

            bindata = fb.read()
            # Fetching the required arg data
            header = args.header
            field  = args.field

            v3sh = v3SecureHeader_t.from_buffer(bytearray(data_v3))
            tmpData = to_bytes( v3sh.vheader[header][field] )
            if len(tmpData) == len(bindata):
                v3sh.vheader[header][field] = from_bytes(bindata)

                structData = bytes(v3sh.data)
                if len(data_v3) >= len(structData):
                    data_v3[:len(structData)] = structData

                    v1_v3_data = data[:v3StartOffset] + data_v3

                    with open(args.output, 'wb') as wf:
                        wf.write(v1_v3_data)
                else:
                    print (f"ERROR: Input binary len mismatch, expected len: {len(tmpData)}, actual len: {len(fb)}")
            else:
                print (f"ERROR: Input binary len mismatch, expected len: {len(tmpData)}, actual len: {len(fb)}")


def corruptHeader(args):
    # Loading lookup table and loading to json
    with open(args.image, 'rb') as f:

        data = bytearray(f.read())
        v3StartOffset = 0

        # Check the file contain v3 along with V1
        # First 1288 bytes of V1
        # later 15360 bytes of v3
        if args.fileHasV1 == "True":
            # skip the sh v1 data to access the v3
            v3StartOffset = v1_secure_header_size

        # Fetching the required arg data
        header = args.header
        field  = args.field

        # Get the Offset value to corrupt
        offset    = 0
        numOfByte = 1

        if hasattr(args, "offset"):
            offset = int(args.offset, 10)

        if hasattr(args, "num"):
            numOfByte = int(args.num, 10)

        for i in range(numOfByte):
            (field_abs_offset, size) = get_nested_struct_field_metadata(v3Secure_t, header, field)
            cal_offset = v3StartOffset + field_abs_offset + offset + i
            d = data[cal_offset]
            # print (f"corruptHeader - cal_offset : {cal_offset} data: {hex(d)} Inverted data: {hex(~d & 0xFF)}")
            data[cal_offset] = ~d & 0xFF

        data = bytes(data)
        with open(args.output, 'wb') as wf:
            # print ("corruptHeader - Writing output file")
            wf.write(data)


def main():
    parser = argparse.ArgumentParser(description="Tool for parsing secure header version 2 parser")
    parser.add_argument("-i", "--image",     required=True,  help="Binary image")
    parser.add_argument("-s", "--fileHasV1", default="True", help="Specify Image consist v3 along with V1")

    subparsers = parser.add_subparsers(title="Action", dest="action")
    # Get subparser
    action_parser = subparsers.add_parser("get", help="Get the given header")
    action_parser.add_argument("header", choices=header_name, help="header name")
    action_parser.add_argument('field', choices=sh_field_name, help="Choice the fields name")
    action_parser.add_argument("-o", "--output", help="Output Image")
    action_parser.set_defaults ( func=getHeader )

    action_parser = subparsers.add_parser("set", help="Set the given header")
    action_parser.add_argument("header", choices=header_name, help="header name")
    action_parser.add_argument('field',  choices=sh_field_name, help="Choice the fields name")
    action_parser.add_argument("-b", "--binary", required=True, help="Input image")
    action_parser.set_defaults ( func=setHeader )

    action_parser = subparsers.add_parser("corrupt",            help="Corrupt the given header field")
    action_parser.add_argument("header", choices=header_name,   help="Select the header")
    action_parser.add_argument('field',  choices=sh_field_name, help="Select the field name")
    action_parser.add_argument("-c", "--offset", default = "0", help="Provide the byte position")
    action_parser.add_argument("-n", "--num",    default = "1", help="Specify the number of bytes to corrupt")
    action_parser.add_argument("-o", "--output", required=True, default="./corrupt_img.bin", help="Specify the path where the output image will be saved")
    action_parser.set_defaults ( func=corruptHeader )

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()