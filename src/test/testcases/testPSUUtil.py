#!/usr/bin/python2
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPSUUtil.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2019
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
'''
#############################################################
#    @file    testClass.py
#    @author: George Keishing <gkeishin@in.ibm.com>
#    @brief   Framework class Host SBE interface on simics
#
#    Created on March 29, 2016
#    ----------------------------------------------------
#    @version  Developer      Date       Description
#    ----------------------------------------------------
#      1.0     gkeishin     29/03/16     Initial create
#############################################################
'''

#-------------------------
# Imports packages
#-------------------------
import time
import conf
import testUtil
import testPSUUserUtil
from sim_commands import *

#-------------------------
# Macros constants
#-------------------------
SUCCESS = 1
FAILURE = 0

#-------------------------
# SIM OBJs
#-------------------------
'''
This is a simulator obj mapped. Refer simics folks if new objects are needed.
'''
if testUtil.getMachineName() == "axone":
    simSbeObj  = conf.backplane0.proc0.pib_cmp.sbe_scom_space
else:
    simSbeObj  = conf.p9Proc0.sbe.scom_space
simMemObj  = conf.system_cmp0.phys_mem

'''
This is a base MBOX registry address from 0..7
'''
# Register MBOX 0..3 SBE side address in order
REGDATA_SBE = [
          0x000D0050,
          0x000D0051,
          0x000D0052,
          0x000D0053,
          ]

# Register MBOX 4..7 host side address in order
REGDATA_HOST = [
          0x000D0054,
          0x000D0055,
          0x000D0056,
          0x000D0057,
          ]

# Supporting Class objects
'''
Base function members definitions for set,get,read, write and others needed.
Keep it simple and modular so that it can be extended as a base class.
'''
#------------------
# Registry class
#------------------
class registry(object):
    #------------------------------
    # Set the reg data
    #------------------------------
    def setRegData(self, addr, value, size):
        self.regAddr = addr
        self.regVal  = value
        self.regSize = size

    #------------------------------
    # Read Reg value set or updated
    #------------------------------
    def getRegData(self):
        print "  Addr  : ",hex(self.regAddr)
        print "  Value : ",self.regVal
        print "  Size  : ",self.regSize

    #------------------------------
    # Write to a Registry
    #------------------------------
    def writeToReg(self, objType):
        address = self.regAddr
        value   = self.stringToByte(self.regVal)
        size    = self.regSize
#        print "  WData  : 0x%s -> Byte Data %s"% (self.regVal,value)
#        print "  Addr   :", hex(address)
#        print "  Size   : %s Bytes"% size

        self.__write(objType,address,value,size)
        return

    #------------------------------
    # Write to Registry 0..3 using
    # test data directly.
    #------------------------------
    def writeTestData(self, data):
        simObj = SIM_get_interface(simSbeObj, "memory_space")
        entryCount = len(data)
        size = 8
        for i in range (entryCount):
            value = stringToByte(data[i])
#            print "\n   Writting ", hex(REGDATA_SBE[i])
#            print "   %x %x %x %x %x %x %x %x" % (value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7])
            simObj.write(None, REGDATA_SBE[regIndex],
                        (value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]),
                        size)
        return

    #------------------------------
    # Write using SIM object
    # 4/8 Bytes data
    #------------------------------
    def __write(self, Targetobj, address, value, size):
        simObj = SIM_get_interface(Targetobj, "memory_space")
        if int(size) == 4:
            simObj.write(None, address,
                        (value[0],value[1],value[2],value[3]),
                        size)
        elif int(size) == 8:
            simObj.write(None, address,
                        (value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]),
                        size)
        print "  SIM obj: Write %s bytes [ OK ] " % size
        return

    #---------------------------
    # Read from a Registry
    #---------------------------
    def readFromReg(self, objType):
        address = self.regAddr
        size    = self.regSize
        value   = self.regVal
#        if int(value) !=0:
#            print "  RData  :", value
#        print "  Addr   :", hex(address)
#        print "  Size   : %s Bytes"% size

        value = self.__read(objType,address,size)
        return value

    #---------------------------
    # Read from a memomry
    # Max Sim interface can read 8
    # byte data at a given time
    #---------------------------
    def readFromMemory(self, objType, magicNum):
        # Start addr + 8 bytes
        address = self.regAddr
        size    = self.regSize    # Max it can read is 8 Bytes
        value   = self.regVal     # Max lentgth it should read

        MaxAddr  = address + value   # This is the addres range it could read
#        print "  MaxAddr Range:",hex(MaxAddr)
        OffsetAddr = address
#        print "  OffsetAddr:",hex(OffsetAddr)

#        print "  Memory Entries to be read : %d" % (value/8)
#        print "  Match Magic Number : ", magicNum

        while ( OffsetAddr <= MaxAddr):
            sim_data = self.__read(objType,OffsetAddr,size)
#            print " ", hex(OffsetAddr),self.joinListDataToHex(sim_data).upper()
            OffsetAddr += 8

            if self.validateTestMemOp(sim_data,magicNum) == True:
#                        print "  Test validated .. [ OK ]"
                        return SUCCESS

        return FAILURE # Failed validation

    #------------------------------
    # Read using SIM Object
    #------------------------------
    def __read(self, Targetobj, address, size):
        simObj = SIM_get_interface(Targetobj, "memory_space")
        value = simObj.read(None, address, size, 0x0)
        #print "  SIM obj: Read %s bytes [ OK ] " % size
        return value

    #--------------------------------
    # Prepare the byte data from the
    # string and return the list set
    #-------------------------------
    def stringToByte(self,value):
        '''
         The sim interface doesnt take the values as it is ..
         it takes as byte arrays
         Ex: "0000030100F0D101"
             '\x00\x00\x03\x01\x00\xf0\xd1\x01'
             [0, 0, 3, 1, 0, 240, 209, 1]
        '''
        # Convert it to a hex string
        hex_val= value.decode("hex")
        # Prepare the conversion to a list of byte values
        value=map(ord, hex_val)
        return value

    #---------------------------------------
    # Joing the list set data to hex data
    # Reverse of the stringToByte logic
    #---------------------------------------
    def joinListDataToHex(self, data):
        # simics>  (0, 0, 3, 1, 0, 240, 209, 1)
        # Join this data into hex string 0xf0d101
        bit_shift=56
        hex_val = 0x0
        for val in data:
            hex_val |= int(val) << bit_shift
            bit_shift -=8
        return hex(hex_val)

    #----------------------------------------------------
    # Execute the read or write operation in loop as per
    # Test data set pre-defined
    #----------------------------------------------------
    def ExecuteTestOp(self, testOp, test_bucket, raiseException=True):
        '''
           3 prong steps : set data, read/write data, validate
        '''
        #--------------------------------------------
        for l_params in test_bucket:
        #--------------------------------------------
#            print "  Desc   : %s "  % l_params[5]
#            print "  Op     : %s "  % l_params[0]
            if "func" == l_params[0]:
                print "  Func    : %s "  % l_params[1]
            if l_params[4] != "None":
                print "  Expect : %s "  % l_params[4]
            if "func" == l_params[0]:
                print "  Function Params :",l_params[2]
            else:
                                  # addr,      value,      size
                self.setRegData(l_params[1],l_params[2],l_params[3])

            # ---------------------------------------------
            # Check the Op and perform the action
            # read/write
            # ---------------------------------------------
            if "read" == l_params[0]:
                sim_data = self.readFromReg(testOp)

                # Validate the test data
                '''
                  This field in the test entry holds the data
                  that needs validation against sim data.
                '''
                if l_params[4] != "None":
                    if self.validateTestOp(sim_data,l_params[4]) == True:
                        print "  Test validated .. [ OK ]"
                    else:
                        print "  ++++++++++++++++++++++++++++++++++++++++++"
                        print "  simics Data  : ", sim_data
                        print "  simics Hex   : ", self.joinListDataToHex(sim_data).upper()
                        if(raiseException == True):
                            raise Exception('Data mistmach');
                        return FAILURE # Failed validation
#                else:
#                    print "  ++++++++++++++++++++++++++++++++++++++++++"
            elif "write" == l_params[0]:
                self.writeToReg(testOp)
            elif "memRead" == l_params[0]:
                                   # (Sim obj)    (Validate)
                return self.readFromMemory(testOp, l_params[4])
            elif "func" == l_params[0]:
                                    # Func name   Params
                rc = self.loadFunc( l_params[1], l_params[2] )
                return rc
            else:
                print "\n Invalid Test Data"
                if(raiseException == True):
                    raise Exception('Invalid Test Data');
                return FAILURE # Unknown entry op

            print "\n"
        return SUCCESS

    #----------------------------------------------------
    # Validate simulator data against test data
    #----------------------------------------------------
    def validateTestOp(self, sim_data, test_data):
        print "  Test Expects :  0x%s " % test_data
        print "  Expect bytes : ", self.stringToByte(test_data)
        if self.compareList(self.stringToByte(test_data), sim_data, "None") == True:
           print "  Test ... [ OK ] "
           print "  ++++++++++++++++++++++++++++++++++++++++++"
           return SUCCESS
        else:
           print "  Test Failed... !!!"
           print "  ++++++++++++++++++++++++++++++++++++++++++"
           return FAILURE

    #----------------------------------------------------
    # Validate simulator data against test data
    #----------------------------------------------------
    def validateTestMemOp(self, sim_data, test_data):
        if self.compareList(self.stringToByte(test_data), sim_data,"memRead") == True:
           return SUCCESS
        return # Return nothing to check next memory entry


    #----------------------------------------------------
    # Compare the result vs expected list data
    # byte by byte
    #----------------------------------------------------
    def compareList(self, expList, resList, opType):
        for i in range(0,8):
            if int(expList[i]) == int(resList[i]):
                #print "  %s : %s  " % (expList[i],resList[i])
                continue
            else:
                if opType != "memRead":
                    print "  Error \t  %s : %s  [ Mismatch ]" % (expList[i],resList[i])
                    return False # mismatch
                return # Return nothing for Next Mem byte read
        return True

    #----------------------------------------------------
    # A basic loop poll mechanism
    #----------------------------------------------------
    def pollingOn(self, simObj, test_data, retries=20):
        for l_param in test_data:
            while True:
                testUtil.runCycles( 1000000);
                test_d = (l_param,)
                rc = self.ExecuteTestOp(simObj, test_d, False)
                if rc == SUCCESS:
                    break
                elif retries <= 0:
                    print "  Retrials exhausted... Exiting polling"
                    raise Exception('Polling Failed for - ' + l_param[5]);
                    break
                else:
                    retries = retries - 1

        # TODO: cleanup this handling
        test_data = (["write", test_data[0][1], "0000000000000000" ,8, None, "Reading Host Doorbell for Interrupt"],)
        self.ExecuteTestOp(simObj, test_data)
        return SUCCESS

    #----------------------------------------------------
    # Load the function and execute
    #----------------------------------------------------
    def loadFunc(self, func_name, i_pArray ):
        rc = testPSUUserUtil.__getattribute__(func_name)(i_pArray)
        return rc # Either success or failure from func


