# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/hwtestsuiteverificationimg/hw_verificationtestsuite.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2024
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

import pyecmd
import sys
import subprocess
import ecmd
import time
import json
import os
import shutil
import time

# ECMD SETUP
sys.path.insert(0, '/afs/apd.pok.ibm.com/projects/eclipz/lab/p10/gsiexe/pervasive_gen2/src')

from util import (
    EcmdArgumentParser, boolarg, add_chip_path,
    ArgumentError, HardwareError, FileFormatError,
    DataFormatError, ResourceError
)


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
    print (f"Removing : {path}")
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

def execute_Command(command, capture_output=False):
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

import os

def executeCommand(command, print_trace_on_console=False):
    '''
    Executes a shell command.

    Args:
        command (str): The command to execute.
        print_trace_on_console (bool): If True, print the command's output to the console.

    Returns:
        str: The output of the command.

    Raises:
        Exception: If the command returns a non-zero exit status.
    '''
    try:
        # Open a pipe to the command
        stream = os.popen(command)
        output = stream.read()
        exit_status = stream.close()

        # Check for errors
        if exit_status is not None:
            print(f"Command failed with exit status {exit_status >> 8}: {command}")
            raise Exception(f"Command failed: {command}")

        # Optionally print the output
        if print_trace_on_console:
            print(output)

        return output

    except Exception as e:
        print(f"An error occurred while executing the command: {command}")
        print(f"Error: {str(e)}")
        raise


def checkData(data: list, subdata: str):
    if subdata in data:
        return True
    else:
        return False


def keyPresent(dictionary, key):
    """
    Check if a key is present in a dictionary.

    Args:
        dictionary (dict): The dictionary to check.
        key: The key to look for.

    Returns:
        bool: True if the key is present, False otherwise.
    """
    return key in dictionary


def string_to_hex_list(s):
    data = bytes.fromhex(s.strip().lower())
    return list(data)



def getcfam(i_target: 'pyecmd.Target', i_addr: int) -> (int, int):
    """
    Get CFAM data for given address

    Args:
        i_target: Ecmd system target
        i_addr: cfam address

    Return:
        (rc, data):
            rc: return code
            data: getcfam output data
    """
    address=0
    o_data = 0
    if (type(i_addr) == int):
        address = i_addr
    else:
        print (f"Invalid data passed to getcfam, Addr: {hex(i_addr)}")
        print ("Invalid data passed to getcfam")
        return 0,1

    data = ecmd.ecmdDataBuffer()
    rc = ecmd.getCfamRegister(i_target, address, data)

    if ( rc == 0 ):
        if data.getByteLength() == 8:
            o_data = data.getDoubleWord(0)
        else:
            o_data = data.getWord(0)
    else:
        print(f"getcfam : getCfamRegister() returned error, RC : {rc}")

    return (rc, o_data)



def getscom(i_target: 'pyecmd.Target', i_addr: int) -> (int, int):
    """
    Get SCOM data for given address

    Args:
        i_target: Ecmd system target
        i_addr: cfam address

    Return:
        (rc, data):
            rc: return code
            data: scom output data
    """
    address=0
    o_data = 0
    if (type(i_addr) == int):
        address = i_addr
    else:
        print (f"Invalid data passed : {hex(i_addr)}")
        return (0,1)

    data = ecmd.ecmdDataBuffer()
    rc = ecmd.getScom(i_target, address, data)

    if ( rc == 0 ):
        if data.getByteLength() == 8:
            o_data = data.getDoubleWord(0)
        else:
            o_data = data.getWord(0)
    else:
        print(f"getscom : getScom() returned error, RC : {rc}")

    return (rc, o_data)


def checkFwMeasurementRegisterData(i_target, i_measurementRegData):
    """
    # Measurement scom reg 0x10018 - 0x1001B each reg has 64 Bit
    fwMeasurementValue      = value["fw_measurement"]
    """
    print ("checkFwMeasurementRegisterData ====>")
    OTPROM_MEASUREMENT_REG8  = 0x10018
    OTPROM_MEASUREMENT_REG9  = 0x10019
    OTPROM_MEASUREMENT_REG10 = 0x1001A
    OTPROM_MEASUREMENT_REG11 = 0x1001B


    regList = [OTPROM_MEASUREMENT_REG8, OTPROM_MEASUREMENT_REG9,OTPROM_MEASUREMENT_REG10, OTPROM_MEASUREMENT_REG11]

    meaStrData = ""
    for reg in regList:
        (rc, data) = getscom(i_target, reg)
        if rc == 0:
            meaStrData += f"{data:016x}"
            print (f"FW Measurement reg {reg:08x} - {data:016x}, {meaStrData}")

    expMeasurementRegData = i_measurementRegData[:64].lower()
    meaStrData = meaStrData.lower()

    print (f"expected meas: {expMeasurementRegData}, type: {type(expMeasurementRegData)}, len: {len(expMeasurementRegData)}")
    print (f"Actual   meas: {meaStrData}, type: {type(meaStrData)}, len: {len(meaStrData)}")

    status = False
    if meaStrData in expMeasurementRegData:
        print (f"FW Measurement data match")
        status = True
    else:
        print (f"Fw Measurement data mismatch: exp: {expMeasurementRegData}, actual: {meaStrData}")

    return (status, expMeasurementRegData, meaStrData) # return (status, expectedValue, ActualValue)


def checkHbblMeasurementRegisterData(i_target, i_measurementRegData):
    """
    # Measurement scom reg 0x1001C - 0x1001F each reg has 64 Bit
    hbblMeasurementValue    = value["hbbl_measurement"]
    """

    print ("checkHbblMeasurementRegisterData ====>")
    OTPROM_MEASUREMENT_REG12 = 0x1001C
    OTPROM_MEASUREMENT_REG13 = 0x1001D
    OTPROM_MEASUREMENT_REG14 = 0x1001E
    OTPROM_MEASUREMENT_REG15 = 0x1001F

    regList = [OTPROM_MEASUREMENT_REG12, OTPROM_MEASUREMENT_REG13,OTPROM_MEASUREMENT_REG14, OTPROM_MEASUREMENT_REG15]

    meaStrData = ""
    for reg in regList:
        (rc, data) = getscom(i_target, reg)
        if rc == 0:
            meaStrData += f"{data:016x}"
            print (f"HBBL Measurement reg {reg:08x} - {data:016x}, {meaStrData}")

    expMeasurementRegData = i_measurementRegData[:64].lower()
    meaStrData = meaStrData.lower()

    print (f"expected meas: {expMeasurementRegData}, type: {type(expMeasurementRegData)}, len: {len(expMeasurementRegData)}")
    print (f"Actual   meas: {meaStrData}, type: {type(meaStrData)}, len: {len(meaStrData)}")

    status = False
    if meaStrData in expMeasurementRegData:
        print (f"HBBL Measurement data match")
        status = True
    else:
        print (f"HBBL Measurement data mismatch: exp: {expMeasurementRegData}, actual: {meaStrData}")

    return (status, expMeasurementRegData, meaStrData) # return (status, expectedValue, ActualValue)


def setImagFlashDefaultEcmdConfig(i_target, imagePath):
    """
    Set the default Ecmd configuration for flashing seeprom for Primary proc

    Args:
        target: ecmd target
    """

    configData = {
        "SPI_ACCESS_MODE"       : "dd",
        "HW_IMAGE_PATH"         : imagePath,
        "GLOBAL_DEBUG"          :  "6.U6.HU.BU.P",
        'USE_SEEPROM_FASTLOAD'  : 'on',
        'USE_SBE_FIFO'          : 'off'
    }

    data = ecmd.ecmdConfigData()
    for key, value in configData.items():
        data.cdString = value
        rc = ecmd.ecmdSetConfigurationComplex(i_target, key, data)
        if (rc != 0):
            print (f"ecmd set config failed. {key}, {rc}")
            raise Exception("ecmd set config failed")


def flashImg(i_target, i_imagePath):
    """
    Flash image to P10 proc using crorecoversbe

    Args:
        i_target: ecmd target
        i_imagePath: seeprom image path to be flash
    """

    print ("--------")

    # Renaming the given image name to sbe_seeprom_DD1.bin with is using in crorecoversbe
    img             = getFilenameFromPath(i_imagePath)
    imgFileIndex    = i_imagePath.find(img)
    seepromImgPath  = i_imagePath[:imgFileIndex] + "temp1"
    mkdir(seepromImgPath)
    seepromImg      = seepromImgPath + "/sbe_seeprom_DD1.bin"

    print (f"i_imagePath: {i_imagePath}, seepromImgPath: {seepromImg}")
    copyFile(i_imagePath, seepromImg)

    # Default configuration for flash image
    configData = {
        "SPI_ACCESS_MODE"       : "dd",
        "HW_IMAGE_PATH"         : seepromImgPath,
        "GLOBAL_DEBUG"          : "6.U6.HU.BU.P",
        'USE_SEEPROM_FASTLOAD'  : 'on',
        'USE_SBE_FIFO'          : 'off'
    }
    print(configData)

    data = ecmd.ecmdConfigData()
    for key, value in configData.items():
        rc = ecmd.ecmdGetConfigurationComplex(target, key, data)
        data.cdString = value
        rc = ecmd.ecmdSetConfigurationComplex(i_target, key, data)
        if (rc != 0):
            print (f"ecmd set config failed. {key} - {value}, {rc}")
            raise Exception("ecmd set config failed")

    # Run the crorecoversbe for flash seeprom image
    cmd = f"crorecoversbe -p{target.pos} -slow"
    print (cmd)
    # executeCommand(cmd, True)
    os.system(cmd)
    print("")

    # Set USE_SBE_FIF to "on"
    data = ecmd.ecmdConfigData()
    rc = ecmd.ecmdGetConfigurationComplex(target, key, data)
    print(f"getconfig - USE_SBE_FIFO: {data.cdString}")
    data.cdString = "on"
    rc = ecmd.ecmdSetConfigurationComplex(i_target, "USE_SBE_FIFO", data)
    print(f"setconfig - USE_SBE_FIFO: {data.cdString}")

    # rm (seepromImg)


def spiFlashImg(i_target, i_image):
    """
    Flash image to P10 proc using PUTSPI

    Args:
        i_target: ecmd target
        i_imagePath: seeprom image path to be flash
    """

    print ("--------")

    # Default configuration for flash image
    configData = {
        "SPI_ACCESS_MODE"       : "dd",
        "GLOBAL_DEBUG"          : "6.U6.HU.BU.P",
        'USE_SEEPROM_FASTLOAD'  : 'on',
        'USE_SBE_FIFO'          : 'off'
    }
    print(configData)

    data = ecmd.ecmdConfigData()
    for key, value in configData.items():
        rc = ecmd.ecmdGetConfigurationComplex(target, key, data)
        data.cdString = value
        rc = ecmd.ecmdSetConfigurationComplex(i_target, key, data)
        if (rc != 0):
            print (f"ecmd set config failed. {key} - {value}, {rc}")
            raise Exception("ecmd set config failed")

    os.system("setconfig USE_SBE_FIFO off")

    # Run the putspi for flash seeprom image
    cmd = f"/labnfs/cronus/p10/exe/dev/ecmd15/p10_dev_x86_64.exe putspi pu 0 1 0 524288 -p{target.pos} -f {i_image}"

    # Execute putspi cmd with retry in case error
    retry = 3
    while retry > 0:
        print ("cmd: ", cmd)
        output = executeCommand(cmd)
        print (f"\n {output} \n\n")

        if "ERROR:" in output or "errorMessage" in output.lower():
            time.sleep(2)
            retry -= 1
        else:
            break

    if retry == 0:
        print ("Error: putspi cmd failure")
        raise Exception ("putsp cmd failure")

    # Set USE_SBE_FIF to "on"
    data = ecmd.ecmdConfigData()
    rc = ecmd.ecmdGetConfigurationComplex(target, "USE_SBE_FIFO", data)
    print(f"getconfig - USE_SBE_FIFO: {data.cdString}")
    data.cdString = "on"
    rc = ecmd.ecmdSetConfigurationComplex(i_target, "USE_SBE_FIFO", data)
    print(f"setconfig - USE_SBE_FIFO: {data.cdString}")


def checkChipReachRuntime(i_target):
    """
    Check the given target chip reach runtime

    Args:
        i_target: Ecmd system target

    Return:
        True: System reach runtime
        False: System not reach runtime
    """

    (rc, data) = getcfam(i_target, 0x2809)
    if rc != 0:
        print (f"checkChipReachRuntime - Get CFAM failed for 0x2809, Rc: {rc}")
        raise Exception ("checkChipReachRuntime - Get CFAM failed for 0x2809")

    """
    Messaging register structure
    struct
    {
        uint64_t iv_sbeBooted : 1;
        uint64_t iv_asyncFFDC : 1;
        uint64_t iv_s1Supported : 1; // If set implies PPE supports S1 interrupt
        uint64_t iv_reserved1 : 1;
        uint64_t iv_prevState : 4;
        uint64_t iv_currState : 4;
        uint64_t iv_majorStep : 8; // Max major is 97
        uint64_t iv_minorStep : 6; // Max minor is 34
        uint64_t iv_reserved2 : 2; // Unused
        uint64_t iv_progressCode  : 4;
    };
    """
    print ("Msg register: ", hex(data))
    progressCode = data & 0x0000000F
    sbeBooted    = (data & 0x80000000) >> 31

    print (f"Msg register: {data:08x}, progressCode: {progressCode:02x}, sbeBooted: {sbeBooted:01x}")
    if progressCode == 0xF and sbeBooted == 1:
        return True
    else:
        return False


def waitTillSbeBoot(i_target):
    time_out = 120 #2 mins timeout
    count = 0
    while (checkChipReachRuntime(i_target) == False) and (count < time_out):
        print (f"SBE booting... {checkChipReachRuntime(i_target)}")
        time.sleep(5)
        count += 1

    if (count >= time_out):
        raise Exception("waitTillSbeBoot: Timeout...")

    print ("SBE Booted")


def testSuite(ppe, args):
    """
    Test suite function

    Args:
        ppe: ppe target
        args: argurement parser class
    """
    target = ppe.target

    # to get token
    os.system("~/Documents/tokenSupport/get_token")

    # Print Register value
    (rc, data) = getcfam(target, 0x2809)
    print (f"Messaging Register : cfam - 2809 : {hex(data)}")

    (rc, data) = getcfam(target, 0x2801)
    print (f"Register : cfam - 2801 : {hex(data)}")

    (rc, data) = getcfam(target, 0x2801)
    print (f"Register : cfam - 2982 : {hex(data)}")

    if checkChipReachRuntime(target) == False:
        waitTillSbeBoot(target)

    print ("System Booted")

    json_data = loadJsonData(args.json)

    imagePath = args.imgpath
    testStartIndex = 0

    tidStartAttrPresent  = getattr(args, "testid")
    if tidStartAttrPresent != None:
        testStartIndex = int(args.testid)
        print (f"testStartIndex: {testStartIndex}")

    start_main_time = time.time()
    token_count = 0

    # Parsing the json
    for key, value in json_data.items():
        if testStartIndex > int(key):
            print (f"Skipping test id {key}, as per user input")
            continue

        # Run testsuite for only automation key value
        if value["Test type"] == "Automation":
            start_sub_time = time.time()
            print ("\n" * 4)
            print ("-"* 80)

            # check token
            token_count += 1
            if token_count > 4:
                # to get token
                os.system("~/Documents/tokenSupport/get_token")
                token_count = 0

            tid                     = int(key, 10)
            section                 = value["section"]
            desc                    = value["desc"]
            image                   = imagePath + "/" + value["image"]
            expReachRuntime         = True if value["PPE_state"] == "RUNTIME" else False

            ecc_image = None
            if "ecc_image" in value:
                ecc_image = imagePath + "/" + value["ecc_image"]

            hbblMeasFound = False
            # Measurement scom reg 0x10018 - 0x1001B each reg has 64 Bit
            if "fw_measurement" in value:
                fwMeasurementValue      = value["fw_measurement"]

            # Measurement scom reg 0x1001C - 0x1001F each reg has 64 Bit
            if "hbbl_measurement" in value:
                hbblMeasFound = True
                hbblMeasurementValue    = value["hbbl_measurement"]

            msgRegAddr   = int( value["data"]["verify"][0]["address"], 16)
            msgRegValue  = value["data"]["verify"][0]["value"]
            msgRegister = (msgRegAddr, msgRegValue) # CFAM = 2809

            statusRegAddr   = int(value["data"]["verify"][1]["address"], 16)
            statusRegValue  = value["data"]["verify"][1]["value"]
            statusRegister  = (statusRegAddr, statusRegValue) # CFAM = 2982

            # Skip hbbl for skip hbbl argument pass
            if getattr(args, "skiphbbl") != None:
                print (".hbbl Section found breaking out, Not execute HBBL test case")
                break

            print (f"Flashing Test ID {tid} image, Description: {desc}")

            if ecc_image != None:
                # Flash test image by spi
                spiFlashImg(target, ecc_image)
            else:
                # Flash test image by crorecoversbe
                flashImg(target, image)

            # Sleep for 2 sec after flash
            time.sleep(2)

            # Running istep
            rc = ecmd.iStepsByNameRange('purge_state_file', 'sbe_start')
            print ('Istep Rc: ', rc)

            time.sleep(10)

            if checkChipReachRuntime(target) == False:
                waitTillSbeBoot(target)

            statusRegDataMatch = False
            statusRegExpData = None
            statusRegActualData = None
            statusRegResult = {}
            if statusRegister[1] > 0:
                (rc, data) = getcfam(target, 0x2982)
                if rc == 0:
                    print (f"Status Register({hex(0x2982)}):  {data:08X}")
                    statusRegActualData = f"{data:08x}"
                    if data != statusRegister[1]:
                        print (f"{'#' * 80}\nExpected Status register data: {hex(statusRegister[1])}, Actual Data: {hex(data)}\n")
                    else:
                        statusRegDataMatch = True

            statusRegResult["result"] = "Pass" if statusRegDataMatch == True else "Fail"
            statusRegResult["expected"] = f"{statusRegister[1]:08X}"
            statusRegResult["actual"] = f"{data:08X}"


            fwMeasResult    = None
            hbblMeasResult  = None
            fwExpValue      = None
            fwActualValue   = None
            hbblExpValue    = None
            hbblActualValue = None
            # Check the SBE reach runtime
            if expReachRuntime == True:
                if checkChipReachRuntime(target) == True:
                    (fwMeasResult, fwExpValue, fwActualValue)   = checkFwMeasurementRegisterData(target, fwMeasurementValue)
                    if (hbblMeasFound == True):
                        (hbblMeasResult, hbblExpValue, hbblActualValue) = checkHbblMeasurementRegisterData(target, hbblMeasurementValue)

            json_data[key]["Result"] = {
                "Sys state"     : "Runtime" if checkChipReachRuntime(target) == True else "Verification",

                "StatusRegResult" : statusRegResult,

                "fwMeasResult"  : {
                    "Result" : "PASS" if fwMeasResult == True else "FAIL",
                    "Expected" : fwExpValue,
                    "Actual" : fwActualValue
                },

                "hbblMeasResult": {
                    "Result" : "PASS" if hbblMeasResult == True else "FAIL",
                    "Expected" : hbblExpValue,
                    "Actual" : hbblActualValue
                },

                "FinalResult" : "Pass" if (statusRegDataMatch == True) and (fwMeasResult == True) and (hbblMeasResult == True) else "Fail"
            }

            print (json_data[key]["Result"])

            end_sub_time = time.time()
            elapsed_sub_time = end_sub_time - start_sub_time
            print ("\n\n")
            print(f"Time taken for testcase id {key} is {elapsed_sub_time} seconds")
            print ("-"* 80)

            if os.path.exists(args.output) == True:
                rm (args.output)

            with open(args.output, "w") as outfile:
                json.dump(json_data, outfile, indent=4)

    if os.path.exists(args.output) == True:
        rm (args.output)

    with open(args.output, "w") as outfile:
        json.dump(json_data, outfile, indent=4)

    end_main_time = time.time()
    elapsed_main_time = end_main_time - start_main_time
    print ("\n\n")
    print(f"Time taken for full testcase is {elapsed_main_time} seconds")

    print("")
    print("----------------- All test case succesfully executed -----------------")



if __name__ == '__main__':
    parser = EcmdArgumentParser("Hardware - verification code secure header V3 test suite on Hardware")

    parser.add_argument("target", metavar="chip[.unit]", help="Target containing desired PPE")
    parser.add_argument("--ppe", help="PPE to talk to, defaults to first PPE matching the target", default=None)
    parser.add_argument("--sim", help="Use simulation shortcuts (yes/no/true/false/1/0), default: detect based on eCmd target", type=boolarg, default=None)

    parser.add_argument("-j", "--json",    help="testcase json file (.json)")
    parser.add_argument("-i", "--imgpath", help="Image path / Image directory")
    parser.add_argument("--testid",        help="continue from given test id number")
    parser.add_argument("--skiphbbl",      help="skip hbbl testcase")
    parser.add_argument("-o", "--output",     help="Output JSON")

    parser.set_defaults(func=testSuite)

    args = parser.parse_args()

    try:
        with pyecmd.Ecmd(args=args.ecmd_args, cro="ver2"):
            if args.sim is None:
                args.sim = pyecmd.queryDllInfo().dllEnv == pyecmd.ECMD_DLL_ENV_SIM

            targets = pyecmd.loopTargets(args.target, pyecmd.ECMD_SELECTED_TARGETS_LOOP)
            if not targets:
                raise ArgumentError("No targets found")

            add_chip_path(targets[0])
            from ppes import PPES

            for factory in PPES:
                if args.target in factory.target_types and (args.ppe is None or args.ppe.lower() == factory.shortname):
                    args.ppe = factory.shortname
                    break
            else:
                raise ArgumentError("No matching PPE found for target")

            for target in targets:
                ppe = factory(target, sim_mode=args.sim, write_reset_file=getattr(args, "reset", False))
                args.func(ppe, args)

    except pyecmd.EcmdError as e:
        print("eCMD error: " + str(e))
        sys.exit(1)
    except ArgumentError as e:
        print("Invalid argument: " + str(e))
        sys.exit(1)
    except TimeoutError as e:
        print("Timeout: " + str(e))
        sys.exit(1)
    except HardwareError as e:
        print("Hardware error: " + str(e))
        sys.exit(1)
    except FileFormatError as e:
        print("File format error: " + str(e))
        sys.exit(1)
    except DataFormatError as e:
        print("Data format error: " + str(e))
        sys.exit(1)
    except ResourceError as e:
        print("Resource error: " + str(e))
        sys.exit(1)
