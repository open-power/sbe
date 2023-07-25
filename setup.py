# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: setup.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021,2023
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
from setuptools.command.build_py import build_py
"""
 Setuptools is an open source package.
 Documentation on setuptools can be found on the web.

 SBE Usages:

 Command to run from SBE PPE repo:
   python3 setup.py bdist_wheel

   dist/sbe_pel_parser-0.1-py3-none-any.whl will be produced.

   If desired, export PELTOOL_VERSION can be defined
   to customize the version before running setup.py
     export PELTOOL_VERSION=6.7.1006

 To install the SBE wheel:
   pip3 install --user sbe_pel_parser-0.1-py3-none-any.whl

 If necessary, setup the environment before starting:
   scl enable rh-python36 "bash"

"""
from setuptools import setup

package_directories = {
    # User Data packages.
    # Component package names must be in the form of: `udparsers.bxxxx`. Where
    # 'xxxx' is the 4 digit component ID (lowercase).

    "udparsers.o3500":   "modules/udparsers/o3500/",
}

# custom_data_files is a list of tuples
custom_data_files = [( 'sbe_data', ['images/sbeStringFile_DD1'])]

def check_environment_files():
    """
    Check the environment for the needed files

    SBE setup.py is invoked in two contexts:
    1 - op-build, where the images file exists, post build
    2 - OpenBMC, where the images file does NOT exist
        OpenBMC clones a clean SBE PPE repo (source only)

    setup.py will fail if data_files do not exist,
    so if we encounter a missing file, clear the
    expectation and only populate the wheel with
    the usual python source files.
    """
    for i in custom_data_files:
        for x in i[1]:
            if not os.path.isfile(x):
                custom_data_files.clear()
                return

class BuildCommand(build_py):
    """
    Subclass the build_py command

    This allows the capability to add custom build
    steps.
    """
    def run(self):
        # First run the regular build_py
        build_py.run(self)
        # Now run the custom step we need
        check_environment_files()

setup(
    name          = "sbe-pel-parser",
    cmdclass      = {'build_py': BuildCommand},
    version       = os.getenv('PELTOOL_VERSION', '0.1'),
    packages      = package_directories.keys(),
    data_files    = custom_data_files,
    package_dir   = package_directories,
)
