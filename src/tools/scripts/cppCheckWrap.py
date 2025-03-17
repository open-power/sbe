#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/scripts/cppCheckWrap.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2025
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

#-------------------------
#  Imports
#-------------------------
import subprocess
import os, sys
import xml.etree.ElementTree as ET
import csv
import argparse
import json
import multiprocessing
import time

# cppcheck is used from below path
CPPCHECK_PATH = "/afs/rchland.ibm.com/usr7/phyptool/phyptools/outside_tools/cppcheck/cppcheck-2.16.0_x86_64/cppcheck"

'''
Load list of directories to be ignore from a JSON config file,
Include all the directories from json and suppression from JSON.
@param json config file
'''
def load_json_config(listOfDirConfigFile):
    if not listOfDirConfigFile:
        sys.exit(0)

    try:
        with open(listOfDirConfigFile, "r") as jsonConfig:
            dirList = json.load(jsonConfig)
            ignoreDir = dirList.get("ignoreDirs", [])
            includeHeader = dirList.get("includeHeaders", [])
            suppressionList = dirList.get("suppressionLists", [])
            macroList = dirList.get("preprocessorMacroList", [])
            return ignoreDir, includeHeader, suppressionList, macroList
    except Exception as e:
        print(f"Unexpected error while parsing json: {e}")
        sys.exit(0)

"""
Runs cppcheck on the given source directory, filter output by severity.
@param srcDir   : Root source directory to analyze.
@param ignDirs  : Directories to be ignored ( support multiple directory).
@param outDir   : Directory to save the output.
@param includeHeaders: Include all header those cppcheck does not find
@param suppressionList: suppress error id those not needful
@param preprocessorMacroLists: Add all the macro those needed
"""
def cppcheck(srcDir, outDir, ignDirs, includeHeaders, suppressionList, preprocessorMacroLists):
    try:
        # Ensure output directory exists
        os.makedirs(outDir, exist_ok=True)

        cppcheckXmlOut = os.path.join(outDir,"cppcheck_output.xml")
        cppcheckCSVOut = os.path.join(outDir,"cppcheck_output.csv")

        # Create build directory to reuse
        cppCheckBuildDir = os.path.join(outDir,"cppcheckbuild_dir")
        os.makedirs(cppCheckBuildDir, exist_ok=True)

        # Get the Number of CPU core
        numberOfCpuCore = multiprocessing.cpu_count()
        print(" CPU CORE :",numberOfCpuCore)

        # Constructing  the cppcheck command to run
        cmd = [CPPCHECK_PATH,
            "--enable=warning,performance,missingInclude",
            f"--output-file={cppcheckXmlOut}", # xml file to save output from cppcheck
            "--quiet", # Do not show progress reports.
            "--check-level=exhaustive", # Performs the most detailed analysis
            "--xml", # Saving output in xml format
            "--xml-version=2", # xml version
            f"--cppcheck-build-dir={cppCheckBuildDir}", # faster analysis; Cppcheck will reuse the results if the hash for a file is unchanged.
            "--inline-suppr",
            "--error-exitcode=1", # If errors are found, integer 1 is returned instead of the default '0'.
            "-j", # Parallel processing
            str(numberOfCpuCore), # Parallel execution based on CPU core
            srcDir
            ]

        # Add --suppress options
        for suppression in suppressionList:
            cmd.extend(["--suppress=" + suppression])

        # Add ignore directories if any
        for ignDir in ignDirs:
            cmd.append(f"-i{ignDir}")

        # Include Header file
        for incHeader in includeHeaders:
            cmd.extend(["-I", incHeader])

        # Add pre-processor macro if any
        for incMacro in preprocessorMacroLists:
            cmd.extend(["-D", incMacro])

        try:
            # Run cppcheck and capture output
            output = subprocess.run(cmd, capture_output=True, text=True, check=False)
            if output.stderr:
                print(f" cppcheck detected errors! Failing the scritp {output.stderr} ")
                sys.exit(0)
        except FileNotFoundError:
            print(f"Error: cppcheck is not found in PATH..{output.stderr}")
            sys.exit(0)
        except PermissionError:
            print(f"Error: Permission denied while accessing files..{output.stderr}")
            sys.exit(0)
        except Exception as e:
            print(f"Unexpected error: {e}")
            sys.exit(0)

        # Read cppcheck output from XML file
        if not os.path.exists(cppcheckXmlOut):
            print("Error: Output is not generated by cppcheck.")
            sys.exit(0)

        tree = ET.parse(cppcheckXmlOut)
        root = tree.getroot()

        # Categorize issues by severity
        severity_files = {"error": [], "warning": [], "style": [],"performance": [], "information": [],"portability": [],}

        # Open CSV file for writing
        with open(cppcheckCSVOut, "w", newline="", encoding="utf-8") as csvfile:

            csv_writer = csv.writer(csvfile)
            # Write header row
            csv_writer.writerow(["File", "Line", "Column", "Id", "Severity", "Message", "Verbose"])

            # Process and categorize issues as per output format
            for error in root.findall("errors/error"):
                id = error.get("id", "N/A")
                severity = error.get("severity", "N/A")
                message = error.get("msg", "N/A")
                verbose = error.get("verbose", "N/A")

                # Extract file, line, and column from child nodes
                location = error.find("location")
                if location is not None:
                    file = location.get("file", "N/A")
                    print(f"Checking current running file: {file}")
                    line = location.get("line", "N/A")
                    column = location.get("column", "N/A")

                    # output format as below
                    output_format = f"{file}:{line}:{column}: {id} {severity} {message} {verbose}\n"

                    if severity in severity_files:
                        severity_files[severity].append(output_format)

                # Write to CSV
                csv_writer.writerow([file, line, column, id, severity, message, verbose])

        # Write severity reports to separate files
        for severity, issues in severity_files.items():
            if issues:
                with open(os.path.join(outDir, f"{severity}.txt"), "w", encoding="utf-8") as f:
                    f.writelines(issues)

        print(f"Cppcheck analysis completed. Reports saved in {outDir}")

    except Exception as e:
            print(f"Error: Cppcheck analysis Failed: {e}")
            sys.exit(0)


#-------------------------
# Main Function
#-------------------------
def main():
    #----------------------------
    # Read command line args
    #----------------------------
    parser = argparse.ArgumentParser(description=" Wrapper tool to run cppcheck")
    parser.add_argument("-s", "--source", required=True, help="Path of the source directory")
    parser.add_argument("-d", "--destination", required=True, help="Path of the destination directory where we save the output")
    parser.add_argument("-c", "--configfile", help="JSON config file specifying directories to be ignore")
    args = parser.parse_args()

    # Printing path of input directory
    print(" \n source path:",args.source)
    print(" \n destination path:",args.destination)

    # Check if the source directory or destination directory exists or not
    if not os.path.exists(args.source) or not os.path.isdir(args.destination):
        print(f"[ERROR:] Either source '{args.source}' or destination '{args.destination}' Directory does not exist")
        sys.exit(0)

    # Load ignore directories from JSON config file
    ignoreDir = []

    # Load include directories from JSON config file
    includeHeaderDir = []
    suppressionList = []
    if args.configfile is not None:
        ignoreDir, includeHeaderDir, suppressionList, preprocessorMacroList = load_json_config(args.configfile)
        if ignoreDir:
            for dirs in ignoreDir:
                print(f" ignore dir path: {dirs}")
        if includeHeaderDir:
            for incs in includeHeaderDir:
                print(f" include header path: {incs}")
    else:
        print(" \n ignore dir path:",ignoreDir)
        print(" \n include header path:",includeHeaderDir)

    # Run cppcheck ..
    cppcheck(args.source, args.destination, ignoreDir, includeHeaderDir, suppressionList, preprocessorMacroList)

if __name__=="__main__":

    localStartTime = time.asctime( time.localtime(time.time()) )
    print ("\nTest start time: ", localStartTime)
    main()
    localCompleteTime = time.asctime( time.localtime(time.time()) )
    print ("\nTest completed time: ", localCompleteTime)