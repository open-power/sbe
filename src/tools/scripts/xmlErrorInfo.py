# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/scripts/xmlErrorInfo.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021,2024
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

import os, sys
import xml.etree.ElementTree as ET
import glob
import argparse

class CRFData():
    def __init__(self, crfId, targetType, target, target_index):
        self.crfId = crfId
        self.targetType = targetType
        self.target_index = target_index
        self.target = target

class Missing_CollectRegisterFFDC(Exception):
    def __init__(self, value):
        self.value = value

    # __str__ is to print() the value
    def __str__(self):
        return(repr(self.value))

def _printHelp(errorMessage):
    print(errorMessage)
    print("Usage:\n\n")
    print("{} - file name".format(__file__))
    print("ODir - Output directory where our c files will be stored")
    print("XmlDir - XML direcxtory path that holds error info xml files")
    exit(1)

# utility function to check if list is empty or not
def _exists(elementList):
    return len(elementList)!=0

def _createDir(dirname):
    try:
        os.mkdir(dirname)
    except:
        print("The directory path doesn't exist. Check the path and re-run")

def parse_xml(xml, tankRegisterHash, rcTankHash):
    tree = ET.parse(xml)
    root = tree.getroot()
    hwperrors = root.findall("hwpError")
    global crfIdset
    # map rc id and collect register ffdc id only if collect register ffdc is present
    for err in hwperrors:
        # check for sbe error
        if(len(err.findall("sbeError"))!= 0):
            # find all collectregisterffdcs
            collectRegFFDCs = err.findall("collectRegisterFfdc")
            if(len(collectRegFFDCs)!=0):
                # find rc only if collect reg ffdc is present
                rc = err.findall("rc")[0].text
                ffdc_list = [ffdc.text for ffdc in err.findall("ffdc")]
                target_index = None
                # find ids for each collectregffdc
                for crfdata in collectRegFFDCs:
                    crfId = crfdata.findall("id")[0].text
                    crfTargetType = crfdata.findall("targetType")[0].text
                    crfTarget = crfdata.findall("target")[0].text
                    if(len(ffdc_list)!=0 and crfTarget in ffdc_list):
                        target_index = ffdc_list.index(crfTarget)
                        print("\t\tTARGET index {} rc {} target {}".format(target_index, rc, crfTarget))
                    elif(len(ffdc_list)!=0):
                        print("\t\tNO TARGET FOR RC {} in {} target {} ".format(rc, xml, crfTarget))
                    else:
                        print("NO FFDC FOR rc {} in {} target {}".format(rc, xml, crfTarget))
                    data = CRFData(crfId, crfTargetType, crfTarget, target_index)
                    if(rc in rcTankHash.keys()):
                        rcTankHash[rc].add(data)
                    else:
                        # make serial number map
                        # create a new entry for rc and tid and tid to it
                        rcTankHash[rc] =set()
                        rcTankHash[rc].add(data)
                    crfIdset.add(crfId)

def parse_xml_regffdc(xml, tankRegisterHash, rcTankHash):
    # map register ffdc id and registers inside it
    global crfIdset
    tree = ET.parse(xml)
    root = tree.getroot()
    regFFDCs = root.findall("registerFfdc")
    for registerFFDC in regFFDCs:
        registerFFDCid = registerFFDC.findall("id")[0].text
        scomRegisters = [reg.text for reg in registerFFDC.findall("scomRegister")]
        registers = set(scomRegisters)
        try:
            if registerFFDCid in crfIdset:
                if registerFFDCid not in tankRegisterHash.keys():
                    tankRegisterHash[registerFFDCid] = set()
                    # have a mapping of a serial number along with this
                    tankRegisterHash[registerFFDCid] = registers
                else:
                    tankRegisterHash[registerFFDCid].update(set(registers))
            else:
                raise(Missing_CollectRegisterFFDC(registerFFDCid))
        except Missing_CollectRegisterFFDC as resourceMissing:
            global missingResource
            if(missingResource==False):
                missingResource = True

# generate files for xmls
def writeStart(rcTankHash):
    global crfIdset
    print("    LOG : Writing file {} to {} ".format(os.path.basename(xmlErrorInfo), outputDir))
    with open(xmlErrorInfo, 'w') as filePointer:
        filePointer.write("/* This file is generated from a python script - \n")
        filePointer.write(" {} */ \n\n".format(sys.argv[0]))
        filePointer.write("#include <stdint.h>\n")
        filePointer.write("#include <vector>\n")
        filePointer.write("#include <xmlErrorInfo.H>\n")
        filePointer.write('#include "plat_hw_access.H"\n')
        filePointer.write('#include "assert.h"\n')
        filePointer.write('#include "sbeFFDC.H"\n')
        filePointer.write('#include "hwp_error_info.H"\n\n')
    print("    LOG : Writing file {} to {} ".format(os.path.basename(xmlErrorInfoHeader), outputDir))
    with open(xmlErrorInfoHeader, 'w') as filePointer:
        filePointer.write("/* This file is generated from a python script - \n")
        filePointer.write(" {} */\n\n".format(sys.argv[0]))
        filePointer.write("#ifndef __SBEFW_RC_TANK_HEADER_H\n")
        filePointer.write("#define __SBEFW_RC_TANK_HEADER_H\n")
        filePointer.write("#include <stdint.h>\n")
        filePointer.write("#include <vector>\n")
        #with open(xmlErrorInfoHeader, 'w') as filePointer:
        filePointer.write("\tenum Regs:uint8_t { \n")
        for reg in crfIdset:
            filePointer.write("\t\t{}, \n".format(reg))
        filePointer.write(" \t};\n\n")
        filePointer.write("// creating struct to hold ids and target\n")
        filePointer.write("struct crfData{\n")
        filePointer.write("\t\t Regs crfId;\n")
        filePointer.write("\t\t uint8_t target_index;\n\t};\n\n")
        filePointer.write("    void rcToCollectRegId(\n")
        filePointer.write("                         const uint32_t rcId,\n")
        filePointer.write("                         std::vector<crfData>& crfDataVec ,\n")
        filePointer.write("                         uint32_t & numTanks);\n\n\n")
        filePointer.write("    void regIdToRegisters(\n")
        filePointer.write("                         Regs regId,\n")
        filePointer.write("                         std::vector<uint32_t>& registerAddress ,\n")
        filePointer.write("                         uint32_t & numTanks);\n")
        filePointer.write("#endif \n\n")

def populateFiles(tankRegisterHash, rcTankHash, regNameValueDic):
    with open(xmlErrorInfo, 'a') as filePointer:
        # write tankToRegisters function
        filePointer.write("\n    void regIdToRegisters(\n")
        filePointer.write("                         Regs regId,\n")
        filePointer.write("                         std::vector<uint32_t>& registerAddress ,\n")
        filePointer.write("                         uint32_t & numTanks)\n")
        filePointer.write("    {\n")
        filePointer.write("        SBE_DEBUG(\"regIdToRegisters. REG ID: 0x%x\", regId);\n")
        filePointer.write("        numTanks = 0;\n\n")
        filePointer.write("        switch (regId)\n")
        filePointer.write("          {\n\n")
        for tankId, regs in tankRegisterHash.items():
            if tankId in crfIdset:
                filePointer.write("          case {}:\n".format(tankId))
                for reg in regs:
                    regName = reg.split("::")[-1]
                    if(regName in regNameValueDic):
                        regValue = regNameValueDic[regName]
                        filePointer.write("                registerAddress.push_back(0x{});\n".format(regValue))
                    else:
                        # to debug and see why these values are not populated
                        filePointer.write("                registerAddress.push_back({});\n".format(reg))
                        print("Missing register address for {}".format(reg))
                        exit(1)
                filePointer.write("                break;\n")
        filePointer.write("            default:\n")
        filePointer.write("                /*either hw data is not present\n                    or xml parsing is not able to parse an rc make it an info trace*/\n")
        filePointer.write("                SBE_DEBUG(\"Error xmlErrorInfo.C . regIdToRegisters() : Invalid Reg ID 0x%x\", ")
        filePointer.write("regId);\n")
        filePointer.write("                return;\n")
        filePointer.write("        }\n")
        filePointer.write("numTanks = registerAddress.size() * sizeof(uint32_t);\n")
        filePointer.write("    }\n")

        # write rcToRegistersTank function
        filePointer.write("\n    void rcToCollectRegId(\n")
        filePointer.write("                         const uint32_t rcId,\n")
        filePointer.write("                         std::vector<crfData>& crfDataVec ,\n")
        filePointer.write("                         uint32_t & numTanks)\n")
        filePointer.write("    {\n")
        filePointer.write("        SBE_DEBUG(\"rcToCollectRegId. RC ID: 0x%x\", rcId);\n")
        filePointer.write("        numTanks = 0;\n\n")
        filePointer.write("        switch (rcId)\n")
        filePointer.write("          {\n\n")
        for rc, crfDataList in rcTankHash.items():
            filePointer.write("            case {}:\n".format(rc))
            for crfData in crfDataList:
                if(crfData.target_index ==None):
                    target_index = -1
                    print("crfid {} rc {} has failed, target index not found ".format(crfData.crfid, rc, crfData.target))
                    missingTargetInstanceIndex = True
                else:
                    target_index = crfData.target_index
                    filePointer.write("                crfDataVec.push_back(crfData{"+ crfData.crfId+', '+ str(target_index) + "});\n")
            filePointer.write("                break;\n")
        filePointer.write("            default:\n")
        filePointer.write("                /*either hw data is not present\n                   or xml parsing is not able to parse an rc make it an info trace*/\n")
        filePointer.write("                SBE_DEBUG(\"Error xmlErrorInfo.C . regIdToRegisters() : Invalid Reg ID 0x%x\", ")
        filePointer.write("rcId);\n")
        filePointer.write("                return;\n")
        filePointer.write("        }\n")
        filePointer.write("numTanks = crfDataVec.size() * sizeof(crfData);\n")
        filePointer.write("    }\n")

def _printDetails():
    print("__________________________________________________________________________")
    print("list of tank and regiters hash")
    print("__________________________________________________________________________")
    for key,val in tankRegisterHash.items():
        print("tank id : {}\nregisters {} \n\n".format(key, val))
    print("__________________________________________________________________________")
    print("register tank hash")
    print("__________________________________________________________________________")
    for key,val in rcTankHash.items():
        print("tank id : {}\nregisters {} \n\n".format(key, val))

def genFiles(path, xmls, regNameValDic, tankRegisterHash, rcTankHash):
    if(len(xmls)==0):
        _printHelp("No xml files found in {}".format(xmlPath))
        exit(1)
    for xml in xmls:
        parse_xml(path+xml, tankRegisterHash, rcTankHash)
    for xml in xmls:
        parse_xml_regffdc(xml, tankRegisterHash, rcTankHash)
    global missingResource
    global crfIdset
    if(len(set(crfIdset)- set(tankRegisterHash.keys()))>0):
        print("MISSING CollectregisterFFDCIds in register ffdc id")
        print(set(crfIdset)- set(tankRegisterHash.keys()))
        exit(1)
    writeStart(rcTankHash)
    populateFiles(tankRegisterHash, rcTankHash, regNameValDic)

def regNameRegValue(includeDirec):
    headerFiles =  [os.path.join(includeDirec,fileName) for fileName in os.listdir(includeDirec) if(not os.path.isdir(fileName) and  fileName.endswith(".H"))]
    lines = []
    for headerFile in headerFiles:
        with open(headerFile, 'r') as filePointer:
            lines.extend(line.strip('\n') for line in filePointer.readlines())
    for line in lines:
        if line.startswith("static const") and ("ull;") in line:
            name = line.strip().split("=")[0].strip().split()[-1]
            val = line.strip().split("=")[1].strip().split("ull;")[0][2:]
            regNameValDic[name] =  val

################################## Main ###############################################
if __name__=="__main__":

    parser = argparse.ArgumentParser(description="Parse Error xml files to geneate C and H files")
    parser.add_argument("--output_directory", help="Output directory for generated C and H files")
    parser.add_argument("--list_xmls", nargs="+", help="list of xml files to be parsed")

    outputDir = parser.parse_args().output_directory
    xmls = parser.parse_args().list_xmls
    xmlErrorInfo = outputDir+'/'+"xmlErrorInfo.C"
    xmlErrorInfoHeader = outputDir+'/'+"xmlErrorInfo.H"
    rcTankHash = {} # map for rc and tank
    tankRegisterHash = {} # map of tank and registers
    tankSerialNum = {} # tank name and serial id
    crfIdset = set()
    SBE_SRC_BUILD_DIR = os.getcwd()
    SBE_SRC_DIR = SBE_SRC_BUILD_DIR+'/..'
    regNameValDic = {}
    pathIncl = os.path.join(SBE_SRC_DIR,"import/chips/p10/common/include/")
    regNameRegValue(pathIncl)
    missingResource = False
    if( os.path.isfile(xmls[0]) and  xmls[0].endswith(".xml")):
        path =''
        genFiles(path, xmls, regNameValDic, tankRegisterHash, rcTankHash)

    elif( os.path.isdir(sys.argv[2])):
        path =sys.argv[2]
        xmls = [fileName for fileName in glob.glob(path+'*.xml') ]
        genFiles(path, xmls, regNameValDic, tankRegisterHash, rcTankHash)

    if(len(sys.argv)>3 and sys.argv[-1]=='d'):
        printDetails(tankRegisterHash, rcTankHash)
