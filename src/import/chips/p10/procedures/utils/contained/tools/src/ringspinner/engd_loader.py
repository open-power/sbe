# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/tools/src/ringspinner/engd_loader.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
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
import re
import mmap
import functools
from collections import namedtuple
from bitstring import BitArray


SpyLatch = namedtuple('_SpyLatch', ['sbit', 'rbit', 'latch'])
SpyQuery = namedtuple('_SpyQuery', ['ring', 'ringlen', 'latches', 'invmask'])


@functools.lru_cache(maxsize=32)
def query_spy(spy, spydef, scandef):
    ring, latches = __query_spy_latches(spy, spydef)
    latch = latches[0].latch.split('.LATC.L2')[0]
    invmask, ringlen = __query_spy_invmask_ringlen(latch, ring, scandef)
    return SpyQuery(ring, ringlen, latches, invmask)


def __query_spy_latches(spy, spydef):
    with open(spydef) as f, mmap.mmap(f.fileno(), 0, prot=mmap.PROT_READ) as mm:
        # Start by finding beginning of the spy definition, also yields ring
        SPYDEF_RE_0_STR = f"(?:(?:edial)|(?:alias)|(?:idial))\s+{spy}\s+{{\n"\
                        "\s+clock-off\s+{\n"\
                        "\s+ring\s+([a-z0-9_]+)\s+{\n"
        SPYDEF_RE_0_STR = SPYDEF_RE_0_STR.encode("utf-8")
        SPYDEF_RE_0 = re.compile(SPYDEF_RE_0_STR, re.MULTILINE)

        m0 = SPYDEF_RE_0.search(mm)
        if m0 is None or m0.lastindex != 1:
            err = f"'{spy}' start not found in '{spydef}'"
            raise RuntimeError(err)

        ring = m0.group(1)
        ring = ring.decode()

        # Now find the end of the spy definition, the latches are between these
        # two match positions in the file (this is a bit fragile due to spaces)
        SPYDEF_RE_1_STR = b"    }\n"\
                          b"  }\n"
        SPYDEF_RE_1 = re.compile(SPYDEF_RE_1_STR, re.MULTILINE)

        m1 = SPYDEF_RE_1.search(mm, m0.end())
        if m1 is None:
            err = f"'{spy}' end not found in '{spydef}'"
            raise RuntimeError(err)

        # Finally gather each latch line by line
        SPYDEF_RE_2_STR = b"\s+1\s+([0-9]+)\s+[0-9]+\s+0\s+([A-Z0-9_.]+)\(([0-9]+)\)"
        SPYDEF_RE_2 = re.compile(SPYDEF_RE_2_STR, re.IGNORECASE)
        latch_lines = mm[m0.end():m1.start()]
        latch_lines = latch_lines.split(b"\n")[:-1]

        latches = []
        for l in latch_lines:
            m2 = SPYDEF_RE_2.search(l)
            if m2 is None or m2.lastindex != 3:
                err = f"no bits found for '{spy}' in '{spydef}'"
                raise RuntimeError(err)

            rbit, latch, sbit = (int(m2.group(1), 10), m2.group(2),
                                 int(m2.group(3), 10))
            latches.append(SpyLatch(sbit, rbit, latch.decode("utf-8")))
    return ring, latches


def __query_spy_invmask_ringlen(latch, ring, scandef):
    with open(scandef) as f, mmap.mmap(f.fileno(), 0, prot=mmap.PROT_READ) as mm:
        SCANDEF_RE_STR = "BEGIN Scanring\n"\
                        f"Name = {ring}\n"\
                        "Address = .*?\n"\
                        "ClockDomain = .*?\n"\
                        "State = .*?\n"\
                        "Checkable = .*?\n"\
                        "Broadside = .*?\n"\
                        "ScanMode = .*?\n"\
                        "Length = ([0-9]+)\n"
        SCANDEF_RE_STR = SCANDEF_RE_STR.encode("utf-8")
        SCANDEF_RE = re.compile(SCANDEF_RE_STR, re.MULTILINE)

        m0 = SCANDEF_RE.search(mm)
        if m0 is None or m0.lastindex != 1:
            err = f"'{ring}' start not found in '{scandef}'"
            raise RuntimeError(err)

        ringlen = m0.group(1)
        ringlen = int(ringlen.decode(), 10)

        SCANDEF_RE_1_STR = f"^\s+1\s+[0-9]+\s+[0-9]+\s+([0-1])\s+{latch}[A-Z0-9_.]+\(([0-9]+)\)$"
        SCANDEF_RE_1_STR = SCANDEF_RE_1_STR.encode("utf-8")
        SCANDEF_RE_1 = re.compile(SCANDEF_RE_1_STR, re.MULTILINE | re.IGNORECASE)

        invbits = [(int(x[1], 10), int(x[0], 2))
                   for x in SCANDEF_RE_1.findall(mm, m0.end())]
        invmask = BitArray(length=len(invbits))
        for i, b in invbits:
            invmask.set(b, i)
    return invmask, ringlen
