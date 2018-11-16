#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbe-size/section-sizes.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2018
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

from elftools.elf.elffile import ELFFile
from os import path
from itertools import groupby
from sys import argv, stderr
from argparse import ArgumentParser
from collections import defaultdict

def load_mapfile(fname):
    sections = defaultdict(lambda: set())
    with open(fname) as f:
        for line in f:
            if "memory map" in line:
                break

        for line in f:
            if line[0:2] != " .":
                continue

            parts = line.split()
            if len(parts) == 1:
                parts += next(f).split()

            name, addr, size, object = parts
            if "(" in object:
                dirname, fname = path.split(object)
                fname = fname[fname.index("(")+1:-1]
                object = path.join(dirname, fname)

            sections[path.abspath(object)].add(name)

    return sections

parser = ArgumentParser(description="Collect summary of section sizes")
parser.add_argument("-m", "--mapfile", help="Count only sections that are used according to MAPFILE")
parser.add_argument("object", nargs="+", help="List of object files to count")
args = parser.parse_args()

section_filter = None if not args.mapfile else load_mapfile(args.mapfile)

for fname in sorted(args.object):
    fname = path.abspath(fname)
    pname = "/".join(fname.split("/")[-2:])
    with open(fname, "rb") as f:
        data = 0
        code = 0
        elf = ELFFile(f)

        for section in elf.iter_sections():
            if section_filter and section.name not in section_filter[fname]:
                continue

            for start in (".text", ".base"):
                if section.name.startswith(start):
                    code += section.header.sh_size
                    continue

            for start in (".rodata", ".data", ".sdata", ".sbss", ".bss"):
                if section.name.startswith(start):
                    data += section.header.sh_size
                    continue
        stack = 0
        if code:
            try:
                with open(path.splitext(fname)[0] + ".su") as f:
                    stack = sum(int(line.split("\t")[1]) for line in f)
            except IOError:
                pass

        if code or data or stack:
            print(pname, code, data, stack)
