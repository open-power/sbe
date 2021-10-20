# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/ecdsaAlgoTest/x86TestCase/testEcdsaRandomVectors.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021
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
from os import getcwd, path
import subprocess
import random
import struct
import sys

#This variable defines how many random test vectors needs to be generated and run against the
#ECDSA algo. Preferably 1 Million.
testCaseCount = 10

# get current working directory
CURRENT_DIR = getcwd()
print(CURRENT_DIR)
REPORT_FILE = sys.argv[1]
#HEXCHARS = [str(num) for num in range(0,10)]
#HEXCHARS.extend( [chr(num) for num in range(65,71)]   )

HEXCHARS = [n for n in range(0,255)]

random_hexChar = random.choice(HEXCHARS)

public_key = ''
signature = ''
message_hash = ''

def generateSignature(filePointer, size = 132 ):
    # we need to generate 132 Bytes -> 264 hexchars
    signature = []
    for i in range(0, size):
        ch = random.choice(HEXCHARS)
        t = struct.pack('B',ch )
        filePointer.write(t)
        signature += t
    return signature

def generateMessageHash(filePointer, size = 64):
    message_hash = []
    for i in range(0, size):
       ch = random.choice(HEXCHARS)
       t = struct.pack('B',ch )
       filePointer.write(t)
       message_hash += t
    return message_hash


def generatePublicKey(filePointer, size = 132 ):
    public_key = "00bb1e087896a09e307274068de7ca8a02a09c55438f50f4de291e63379f736cb7c27a1ef277b2781f97d3bd64a5783cde710056ec6a9b5627d4830908ff53cfb36100b860944176473722512c05f860f1f025bb4654819716ed10fc693830fcfed2269e346283f3a781915c7bc7ddb3b34f114c4fb284bbc4243a57e75201a60cf90622"
    public_key = bytearray.fromhex(public_key)
    #for i in range(0, size):
    #    ch = random.choice(HEXCHARS)
    #    t = struct.pack('B',ch )
    #    filePointer.write(t)
    #    public_key += t
    return public_key

def createBinary(filename):
    with open(path.join(CURRENT_DIR, filename), "wb") as filePointer:
        global public_key
        public_key = generatePublicKey(filePointer)
        global signature
        signature = generateSignature(filePointer)
        global message_hash
        message_hash = generateMessageHash(filePointer)

def runTest():
    # make a call to this executable with binary created
    return subprocess.call(CURRENT_DIR+"/"+"ecdsa" )

def writeReport(status, public_key, signature, message_hash, i):
    if(i==0):
        mode = "w"
    else:
        mode ="a"
        with open(REPORT_FILE, mode) as filePointer:

            filePointer.write("public key :\n\t{}\n\n".format([i.to_bytes(1,"little") for i in public_key]))
            filePointer.write("signature :\n\t{}\n\n".format([i.to_bytes(1,"little") for i in signature]))
            filePointer.write("message_hash :\n\t{}\n\n".format([i.to_bytes(1,"little") for i in message_hash]))
            filePointer.write("test status :\n\t{}\n\n\n".format(status))
            filePointer.write("\t ---------------*************************---------------- \n\n\n")


i = 0
passCount = 0
failCount = 0
didntExecute = 0
binaryName = "random_cases.bin"
while(i<testCaseCount):
    createBinary(binaryName)
    flag = runTest()
    if(flag == 2):
        status = "FAILED TEST CASE"
        print(status)
        failCount+=1
    elif( flag == 1):
        status = "PASSED TEST CASE"
        print(status)
        passCount+=1
    else:
        print(flag)
        status = "SOME ERROR IN EXECUTION"
        print(status)
        notExecuted+=1
    print("PASSED: {}\nFAILED: {}\nDIDNT RUN:{}".format(passCount, failCount, didntExecute))
    writeReport(status, public_key, signature, message_hash, i)
    i+=1

print("PASSED: {}\nFAILED: {}\nDIDNT RUN:{}".format(passCount, failCount, didntExecute))
with open(REPORT_FILE, "a") as filePointer:
    filePointer.write("PASSED: {}\nFAILED: {}\nDIDNT RUN:{}".format(passCount, failCount, didntExecute))

