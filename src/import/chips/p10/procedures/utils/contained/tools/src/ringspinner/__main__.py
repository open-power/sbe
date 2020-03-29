# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/tools/src/ringspinner/__main__.py $
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
import argparse
import pathlib
import sys
import os
import yaml

from ringspinner import engd
from ringspinner import engd_loader
from ringspinner import prcd


def get_header_guard(filename):
    return str(filename.parts[-1]).upper().replace('.', '_')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('cfg', help='YAML input file',
                        type=lambda x: pathlib.Path(x).resolve(strict=True))
    parser.add_argument('-s', help='Show function cache stats',
                        action='store_true', default=False)
    parser.add_argument('-o', help='Output file basename without extension',
                        metavar='output file basename', required=True,
                        type=lambda x: pathlib.Path(x+'.H').resolve())
    args = parser.parse_args()

    with open(args.cfg) as f:
        cfg = yaml.load(f)

    with open(args.o, 'w') as fp:
        fp.write('// *INDENT-OFF*\n')
        fp.write(f'#ifndef {get_header_guard(args.o)}\n')
        fp.write(f'#define {get_header_guard(args.o)}')

        for namespace in cfg:
            engd_path = os.path.expandvars(cfg[namespace]['engd_path'])
            engd_path = pathlib.Path(engd_path).resolve(strict=True)

            for fname, func in cfg[namespace]['functions'].items():
                rings = engd.get_rings_and_spys(func['spy_names'],
                                                engd_path/'p10.spydef',
                                                engd_path/'p10.scandef')

                for _, ring in rings.items():
                    prcd.gen_procedure(ring, ring.ring + '_' + fname, fp=fp,
                                       namespace=namespace)

        fp.write(f'\n\n#endif//{get_header_guard(args.o)}\n')
        fp.write('// *INDENT-ON*\n')

    if args.s:
        print('\n\nFunction cache (memoization) stats')
        print('----------------------------------')
        print(f'engd_loader.query_spy: {engd_loader.query_spy.cache_info()}')
        print('----------------------------------')


if __name__ == '__main__':
    main()
