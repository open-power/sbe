#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/utils/sbe-size/section-deltas.py $
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

from sys import argv, stderr
import numpy as np

def read_file(fname):
    result = dict()
    with open(fname) as f:
        for line in f:
            parts = line.split()
            result[parts[0]] = np.array(parts[1:], dtype=int)
    return result

reference = read_file(argv[1])
current = read_file(argv[2])
result = dict()
width = len(next(iter(reference.values())))

keys = set(reference.keys())
keys.update(current.keys())
for name in sorted(keys):
    delta = current.get(name, np.zeros(width, dtype=int)) - reference.get(name, np.zeros(width, dtype=int))
    if delta.any():
        print(name, *delta)
