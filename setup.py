# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: setup.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021,2022
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
import os.path
import os

"""
 Setuptools is an open source package.
 Documentation on setuptools can be found on the web.

"""
from setuptools import setup

package_directories = {
    # User Data packages.
    # Component package names must be in the form of: `udparsers.bxxxx`. Where
    # 'xxxx' is the 4 digit component ID (lowercase).

    "udparsers.o3500":   "modules/udparsers/o3500/",
}

setup(
    name          = "sbe-pel-parser",
    version       = os.getenv('PELTOOL_VERSION', '0.1'),
    packages      = package_directories.keys(),
    package_dir   = package_directories,
)
