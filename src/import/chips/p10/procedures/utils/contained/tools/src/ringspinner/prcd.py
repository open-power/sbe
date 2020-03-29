# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/tools/src/ringspinner/prcd.py $
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
import sys
from bitstring import BitArray
from collections import namedtuple

from ringspinner.prcd_templates import *


def spy2var(spyname):
    return "i_" + spyname.replace(".", "_").lower()


def get_ringbits_in_group(spybit, group):
    """
    Return tuple representing the closed interval bitrange of how bits in
    'spybit' map to the ringbits in 'group'.
    """
    return (spybit.rbits[0] - group.rbits[0],
            63 - (group.rbits[1] - spybit.rbits[1]))


def gen_expr_rotate(ringpos, numbits, exprs):
    exprs.append(ROTATE.render(numbits=numbits))
    return ringpos + numbits


def gen_expr_bitops(spybit, group, spyexprs):
    sbits = spybit.sbits
    rbits = get_ringbits_in_group(spybit, group)
    svar = spy2var(spybit.spyname)
    mask = BitArray(64)
    mask.set(1, range(sbits[0], sbits[1]+1))
    nbits = spybit.nbits

    spyexprs.append(f'// {spybit.spyname}[{sbits[0]}:{sbits[1]}]')

    if spybit.is_bkwd:
        spyexprs.append('{')
        # Right-align spybit and apply inversion mask
        expr = f'    uint64_t x = (({svar} & 0x{mask.hex}) >> {63 - sbits[1]})'
        expr+= f' ^ 0b{spybit.invmask.bin}'
        spyexprs.append(expr)
        for i in range(0, nbits):
            sbpos = nbits-1 - i
            rbpos = [x for x in range(rbits[0], rbits[1]+1)][::-1][i] # FIXME yikes
            mask = BitArray(64)
            mask.set(1, 63 - sbpos)
            # Reverse each bit: ((( data & mask ) >> sbpos) << (63 - rbpos))
            if i == 0:
                expr = f'    tmp |= (((x & 0x{mask.hex}ull) >> {sbpos}) << {63 - rbpos})'
            else:
                expr += f' | (((x & 0x{mask.hex}ull) >> {sbpos}) << {63 - rbpos})'

        spyexprs.append(expr)
        spyexprs.append('}')
    else:
        # Right-align spybit and apply inversion mask
        expr = f'tmp |= ((({svar} & 0x{mask.hex}) >> {63 - sbits[1]})'
        expr+= f' ^ 0b{spybit.invmask.bin})'
        expr+= f' << {63 - rbits[1]}'
        spyexprs.append(expr)


def gen_expr_scanin(ringpos, group, exprs):
    mask = BitArray(64)
    mask.invert()
    spyexprs = []

    for spybit in group.spybits:
        rbits = get_ringbits_in_group(spybit, group)
        # Not all bits in the group are covered by spybits. Mask all such bits
        # from the scan64 buffer data to prevent overwriting other latches in
        # the ring.
        mask.set(0, range(rbits[0], rbits[1]+1))
        gen_expr_bitops(spybit, group, spyexprs)

    exprs.append(f'tmp &= 0x{mask.hex}ull')
    for expr in spyexprs:
        exprs.append(expr)
    exprs.append(SCANIN.render(data='tmp', numbits=0))

    return ringpos


def gen_insns(groups, ringlen):
    ringpos = 0
    insns = []
    for group in groups:
        exprs = []

        # Rotate ring to get the full 64b group into the scan64 buffer
        if ringpos != group.rbits[0]:
            numbits = group.rbits[0] - ringpos
            ringpos = gen_expr_rotate(ringpos, numbits, exprs)

        ringpos = gen_expr_scanin(ringpos, group, exprs)

        insns.append({'exprs' : exprs})

    if ringpos < ringlen:
        numbits = ringlen - ringpos
        ringpos = gen_expr_rotate(ringpos, numbits, insns[-1]['exprs'])
    return insns


def get_groups(spybits):
    Group = namedtuple('_Group', ['spybits', 'rbits'])
    groups = []

    for spybit in spybits:
        rbits = spybit.rbits
        # Start a new group
        if len(groups) == 0 or rbits[1] > groups[-1].rbits[1]:
            groups.append(Group(spybits=[spybit],
                                rbits=(rbits[0], rbits[0]+63)))
        # Expand an existing group
        else:
            spybits = groups[-1].spybits
            spybits.append(spybit)
            groups[-1] = Group(spybits=spybits, rbits=groups[-1].rbits)
    return groups


def gen_procedure(ring, fname, namespace=None, fp=sys.stdout):
    """Generate a FAPI2 procedure to set spys in a ring.

    Overview
    --------
    Each ring has a number of associated spys. Each spy has a number of spybits
    which correspond to a range of bits (or a single bit) in the ring. The
    spybits are not guaranteed to be sequential in the ring itself. A maximum
    of 64b can be scanned into the ring per SCOM. To optimize, first gather as
    many spybits as possible which fit into 64b 'groups' of the ring. This
    limits the total number of necessary SCOMs.

    Algorithm
    ---------
    foreach spybit in ring:
        groups := group spybits which fit into 64 sequential bits in the ring

        foreach group:
            rotate ring to starting position of the group
            bitops to place spybits from input data into 64b buffer
            scan buffer into ring
    """
    groups = get_groups(ring.spybits)
    insns = gen_insns(groups, ring.ringlen)
    spys = [spy2var(x) for x in ring.spynames]
    procedure = PROCEDURE.render(fname=fname, insns=insns, spyvars=spys,
                                 cpp_namespace=namespace)
    fp.write('\n\n')
    fp.write(procedure)
