# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/tools/src/ringspinner/engd.py $
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
from ringspinner.engd_loader import query_spy


def get_rings_and_spys(spynames, spydef, scandef):
    if not isinstance(spynames, list):
        spynames = [spynames]
    rings = {}
    for spy in [Spy(x, spydef, scandef) for x in spynames]:
        ring = spy.ring
        if ring not in rings:
            rings[ring] = Ring(ring, spy.ringlen)
        rings[ring].add_spy(spy)
    return rings


class Ring:
    def __init__(self, ring, ringlen):
        self.__ring = ring
        self.__ringlen = ringlen
        self.__spybits = []
        self.__spynames = []

    def add_spy(self, spy):
        if spy not in self.__spynames:
            self.__spynames.append(spy.spyname)
        self.__spybits = sorted(self.spybits + spy.spybits,
                                key=lambda x: x.rbits[1])

    @property
    def ring(self):
        return self.__ring

    @property
    def ringlen(self):
        return self.__ringlen

    @property
    def spybits(self):
        return self.__spybits

    @property
    def spynames(self):
        return self.__spynames


class Spybit:
    def __init__(self, spyname):
        self.__spyname = spyname
        self.__invmask = 0
        self.__is_bkwd = False
        self.__sbits = None
        self.__rbits = None
        self.__nbits = 0

        self.__last_appended_sbit = None
        self.__last_appended_rbit = None

    def __get_spybit_invmask(self, spy_invmask):
        bslice = slice(self.__sbits[0], self.__sbits[1]+1)
        return spy_invmask[bslice]

    def try_expand(self, sbit, rbit, ring, spy_invmask):
        """
        Try to append a bit to this spybit.
        """
        if self.nbits == 0:
            self.__sbits, self.__rbits = (sbit, sbit), (rbit, rbit)
            appended = True
        elif (sbit == self.__last_appended_sbit + 1
              and abs(rbit - self.__last_appended_rbit) == 1):
            self.__sbits = (self.sbits[0], sbit)
            appended = True

            if rbit < self.__last_appended_rbit:
                self.__rbits = (rbit, self.rbits[1])
                self.__is_bkwd = True
            else:
                self.__rbits = (self.rbits[0], rbit)
        else:
            appended = False

        if appended:
            self.__nbits = self.__nbits + 1
            self.__invmask = self.__get_spybit_invmask(spy_invmask)
            self.__last_appended_sbit = sbit
            self.__last_appended_rbit = rbit
        return appended

    @property
    def nbits(self):
        return self.__nbits

    @property
    def spyname(self):
        return self.__spyname

    @property
    def invmask(self):
        return self.__invmask

    @property
    def is_bkwd(self):
        return self.__is_bkwd

    @property
    def sbits(self):
        return self.__sbits

    @property
    def rbits(self):
        return self.__rbits


class Spy:
    def __init__(self, spy, spydef, scandef):
        self.__spyname = spy
        self.__spybits = []

        spyquery = query_spy(spy, spydef, scandef)

        ring, invmask = spyquery.ring, spyquery.invmask
        for latch in spyquery.latches:
                rbit = latch.rbit # Bit position in the ring
                sbit = latch.sbit # Bit position in the spy

                if (len(self.__spybits) == 0 or
                    not self.__spybits[-1].try_expand(sbit, rbit, ring, invmask)):
                    spybit = Spybit(spy)
                    spybit.try_expand(sbit, rbit, ring, invmask)
                    self.__spybits.append(spybit)

        self.__ring = ring
        self.__spylen = len(spyquery.latches)
        self.__ringlen = spyquery.ringlen

    @property
    def ring(self):
        return self.__ring

    @property
    def ringlen(self):
        return self.__ringlen

    @property
    def spylen(self):
        return self.__spylen

    @property
    def spyname(self):
        return self.__spyname

    @property
    def spybits(self):
        return self.__spybits
