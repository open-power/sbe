#!/usr/bin/env python3
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/hwtestsuiteverificationimg/imagegenerator.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2024,2025
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

import json
import os
import subprocess
import shutil
import argparse

'''
V3 secure header name
'''
header_name = ['hw', 'prefix', 'fw']


'''
V3 secure header fields name
'''
sh_field_name = [
    'magic_number', 'container_version', 'container_size', 'reserved', 'hw_ecdsa_pub_A', 'hw_mldsa_pub_D',
    'header_version', 'hash_algo', 'sign_algo', 'flags', 'fw_key_count', 'size_protected_keys_payload', 'hash_protected_keys_payload', 'ecid', 'hw_sig_A', 'hw_sig_D', 'fw_ecdsa_pub_key_P', 'fw_mldsa_pub_key_S',
    'component_id', 'fw_secure_version', 'size_protected_payload', 'size_unprotected_payload', 'hash_protected_payload', 'fw_sig_P', 'fw_sig_S'
]


def loadJsonData(filename):
    """
    Open JSON file and load

    Args:
        filename: json file name

    Return:
        json
    """
    try:
        with open(filename, 'r') as file:
            data = json.load(file)
            return data
    except FileNotFoundError:
        print(f"File '{filename}' not found.")

    except json.JSONDecodeError as e:
        print(f"Error decoding JSON: {e}")
    return None


def mkdir(path):
    """
    Create Directories

    Args:
        path: directories to create

    return:
        None
    """
    try:
        os.makedirs(path, exist_ok=True)
        print("Directory created successfully!")
    except FileExistsError:
        print("Directory already exists.")

def rm(path):
    """
    remove directories
    """
    if os.path.isfile(path) or os.path.islink(path):
        os.remove(path)  # remove the file
    elif os.path.isdir(path):
        shutil.rmtree(path)  # remove dir and all contains
    else:
        raise ValueError("file {} is not a file or dir.".format(path))


def copyFile(source, destination, overwrite=True):
    """
    Copy a file from one location to another.

    Args:
        sourceh: The path to the source file.
        destination: The path to the destination file.
        overwrite: Whether to overwrite the destination file if it already exists.
    """
    shutil.copy(source, destination)

def getFilenameFromPath(path):
    """Gets the filename from a given path.

    Args:
    path: The full path to the file.

    Returns:
    The filename without the path.
    """

    return os.path.basename(path)


class ParseError(Exception):
    '''
    A new type of Exception that can be thrown for parsing related error.
    '''
    pass

def executeCommand(command, capture_output=False):
    """
    Execute a command on the console.

    Args:
        command (str): The command to execute.
        capture_output (bool): Whether to capture and return the output.

    Returns:
        str: The command's output if capture_output is True, else None.

    Raises:
        ParseError: If the command execution fails.
    """
    try:
        # Execute the command
        process = subprocess.Popen(
            command,
            shell=True,
            stdout=subprocess.PIPE if capture_output else None,
            stderr=subprocess.STDOUT
        )

        # Capture the output
        output, _ = process.communicate()

        if process.returncode != 0:
            print(f"Command failed: {command}")
            print("Error:", output)
            raise ParseError("execute_command failed")

        if capture_output:
            return output.decode()

    except Exception as e:
        print("Error occurred:", str(e))
        raise  # Re-raise the exception for caller to handle


def checkData(data: list, subdata: str):
    if subdata in data:
        return True
    else:
        return False


class imgTool:
    def __init__(self, imgTool: str, image: str, outputDir: str) -> None:
        self.tool = imgTool
        self.img = image
        self.sbhFwSectionFile = outputDir + "/sbeFw.bin"
        self.sbhHbblSectionFile = outputDir + "/sbeHbbl.bin"

    def getSbhFwSection(self) -> "outfile":
        cmd = f"{self.tool} {self.img} extract .sbh_firmware {self.sbhFwSectionFile}"
        executeCommand(cmd)
        return self.sbhFwSectionFile

    def getSbhHbblSection(self) ->  "outfile":
        cmd = f"{self.tool} {self.img} extract .sbh_hbbl {self.sbhHbblSectionFile}"
        executeCommand(cmd)
        return self.sbhHbblSectionFile

    def deleteSbhFwSection(self) -> None:
        cmd = f"{self.tool} {self.img} delete .sbh_firmware"
        executeCommand(cmd)

    def deleteSbhHbblSection(self) -> None:
        cmd = f"{self.tool} {self.img} delete .sbh_hbbl"
        executeCommand(cmd)

    def appendSbhFwSection(self, fwSection: str) -> None:
        cmd = f"{self.tool} {self.img} append .sbh_firmware {fwSection}"
        executeCommand(cmd)

    def appendSbhHbblSection(self, hbblSection: str) -> None:
        cmd = f"{self.tool} {self.img} append .sbh_hbbl {hbblSection}"
        executeCommand(cmd)

    def deletehbbl(self) -> None:
        cmd = f"{self.tool} {self.img} delete .hbbl"
        executeCommand(cmd)

class shv3Parser:
    """
    Secure header v3 parser
    """
    def __init__(self, shV3ParserTool: str, shBin: str) -> None:
        """
        Contructor

        Args:
            shV3ParserTool: secure header Version 2 parser tool
            shBin: Secure header binary
        """
        self.tool = shV3ParserTool
        self.bin = shBin

    def corrupt(self, header, field, output) -> None:
        """
        Secure header V3 image corrupt
        """
        cmd = f"python3 {self.tool} -i {self.bin} corrupt {header} {field} -o {output}"
        executeCommand(cmd)

    def get(self, header, field) -> None:
        """
        Secure header V3 image corrupt
        """
        cmd = f"python3 {self.tool} -i {self.bin} get {header} {field}"
        data = executeCommand(cmd, True)
        hexstring = data.strip().lower()
        bytedata = bytes.fromhex(hexstring)
        return bytedata


def imageGeneration(args):

    testcaseJson = f"{args.output}/testcase.json"

    print ("hasattr(args.ecc) :", getattr(args, "ecc"))

    imagePath = args.output
    tempPath = f"./{imagePath}/temp"

    # Create Image directories
    mkdir (imagePath)
    mkdir (tempPath)

    cmd = f"python3 {args.testxls2json} -f {args.excel} -l {args.lookup} -o {testcaseJson}"
    print (f"Executing Excel to json converter script")
    print (f"Cmd: {cmd}")
    executeCommand(cmd)

    iplImgTool      = args.ipltool
    shV3ParserTool  = args.shparsertool
    v3BinPath       = args.seeprombin
    eccAttrPresent  = getattr(args, "ecc")
    if eccAttrPresent != None:
        ecc = args.ecc


    json_data = loadJsonData(testcaseJson)

    copyFile(v3BinPath, tempPath)

    tmpSeepromImage = tempPath + "/" + getFilenameFromPath(v3BinPath)

    # image tool
    imgtool = imgTool(iplImgTool, tmpSeepromImage, tempPath)
    #Fetch SBH FW section
    tempSbhFwBin = imgtool.getSbhFwSection();
    #Fetch SBH HBBL section
    tempSbhHbblBin = imgtool.getSbhHbblSection()

    # Define Class for secyre boot header corruption
    sbhCorruptFw = shv3Parser(shV3ParserTool, tempSbhFwBin)
    sbhCorruptHbbl = shv3Parser(shV3ParserTool, tempSbhHbblBin)

    for key, value in json_data.items():
        if value["Test type"] == "Automation":
            header = value["header"]
            field  = value["fieldname"]
            section = value["section"]

            print (f"Image generating for tid{key} --> {section} | {header} | {field}")

            if (checkData(header_name, header) == True) and (checkData(sh_field_name, field) == True):

                # Seeprom Image name
                seepromImageName = imagePath + "/sbe_seeprom_tid" + key + ".bin"
                sbhhbblcorupt = imagePath + "/HBBL_V3.header_tid" + key + ".bin"

                # Copy Seeprom for create corrupt seeprom image
                tempSeeprom = tempPath + "/tempseeprom_bin.bin"
                copyFile(v3BinPath, tempSeeprom)
                # image tool
                tmpImgtool = imgTool(iplImgTool, tempSeeprom, tempPath)

                # Temporary secure boot header corrupt binary name
                tempSbhCorruptBin = tempPath + "/sbh_corrupt_" + header + field + ".bin"


                # TODO have to remove
                if (section == ".sbh_hbbl") and (args.skipHbbl == "True"):
                    print ("Skipping .hbbl Section found, Not execute HBBL test case")
                    break

                # Corrupt field in header
                if section == ".sbh_firmware":
                    sbhCorruptFw.corrupt(header, field, tempSbhCorruptBin)
                    tmpImgtool.deleteSbhFwSection()
                    tmpImgtool.deleteSbhHbblSection()
                    tmpImgtool.appendSbhFwSection(tempSbhCorruptBin)

                elif section == ".sbh_hbbl":
                    sbhCorruptHbbl.corrupt(header, field, tempSbhCorruptBin)
                    tmpImgtool.deleteSbhHbblSection()
                    cmd = "tail -c +1289 " + tempSbhCorruptBin + " > " + sbhhbblcorupt
                    os.system(cmd)
                else:
                    print ("Error: Invalid Section")
                    continue

                tmpImgtool.deletehbbl()

                fwMeasurementData   = sbhCorruptFw.get("fw", "hash_protected_payload")

                hbblMeasurementData = None
                if args.skipHbbl == "False":
                    hbblMeasurementData = sbhCorruptHbbl.get("fw", "hash_protected_payload")

                copyFile (tempSeeprom, seepromImageName)
                rm (tempSeeprom)
                rm (tempSbhCorruptBin)

                eccSeepromImageName = None
                if eccAttrPresent != None:
                    eccSeepromImageName = seepromImageName + ".ecc"
                    executeCommand(f"{ecc} --inject {seepromImageName} --output {eccSeepromImageName} --p8")

                # add image name to json
                json_data[key]["image"] = getFilenameFromPath(seepromImageName)
                if eccSeepromImageName != None:
                    json_data[key]["ecc_image"] = getFilenameFromPath(eccSeepromImageName)

                if not "fw_measurement" in value:
                    meas = ''.join(format(x, '02x') for x in fwMeasurementData)
                    json_data[key]["fw_measurement"] = meas[0:64]
                elif value["fw_measurement"] == "0.0":
                    json_data[key]["fw_measurement"] = f'{"0" * 64}'

                if args.skipHbbl == "False" and not "hbbl_measurement" in value:
                    meas = ''.join(format(x, '02x') for x in hbblMeasurementData)
                    json_data[key]["hbbl_measurement"] = meas[0:64]
                elif value["hbbl_measurement"] == "0.0":
                    json_data[key]["hbbl_measurement"] = f'{"0" * 64}'
                print (f"Image generated successful for tid{key}")


    # remove temporary path
    # rm (tempPath)
    rm (testcaseJson)

    with open(testcaseJson, "w") as outfile:
        json.dump(json_data, outfile, indent=4)


parser = argparse.ArgumentParser(description="Image generator")
parser.add_argument('-e', '--excel',         required=True, help='Excel file')
parser.add_argument('-l', '--lookup',       required=True, help='Look up file which have the mapping of Error and status code')
parser.add_argument('-t', '--testxls2json',     required=True, help='Excel to json parse tool')
parser.add_argument('-s', '--shparsertool', required=True, help='Secure header V3 parser tool')
parser.add_argument('-i', '--ipltool',      required=True, help='ipl image tool')
parser.add_argument('-b', '--seeprombin',   required=True, help='Seeprom Binary image')
parser.add_argument('--ecc',                help='Enable the ECC for all images')
parser.add_argument("-o", "--output",       default='./imageGenerator', help="Output path")
parser.add_argument("--skipHbbl",     default="False", help="Skip HBBL")

parser.set_defaults ( func=imageGeneration )
args = parser.parse_args()

args.func(args)
print ("Image generation - successful")