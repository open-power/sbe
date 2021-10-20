# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/ecdsaAlgoTest/x86TestCase/testEcdsaNistVectors.py $
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
    public_key = []
    for i in range(0, size):
        ch = random.choice(HEXCHARS)
        t = struct.pack('B',ch )
        filePointer.write(t)
        public_key += t
    return public_key

def createBinaryFromTestCases(i, case, filename):
    print(i)
    if(i == 0):
        mode = "wb"
    else:
        mode = "ab"
    with open(path.join(CURRENT_DIR, filename), mode) as filePointer:
        message_hash = bytearray.fromhex(case[0])
        filePointer.write(message_hash)
        pub_key = bytearray.fromhex(case[1])
        filePointer.write(pub_key)
        signature =bytearray.fromhex(case[2])
        filePointer.write(signature)


def runTest():
    # make a call to this executable with binary created
    return subprocess.call(CURRENT_DIR+"/"+"ecdsa" )

def writeReport(status, public_key, signature, message_hash, i):
    if(i==0):
        mode = "w"
    else:
        mode = "a"
    with open(REPORT_FILE, mode) as filePointer:

        filePointer.write("public key :\n\t{}\n\n".format(public_key))
        filePointer.write("signature :\n\t{}\n\n".format(signature))
        filePointer.write("message_hash :\n\t{}\n\n".format(message_hash))
        filePointer.write("test status :\n\t{}\n\n\n".format(status))
        filePointer.write("\t ---------------*************************---------------- \n\n\n")

# 0th - message hash
# 1st - public key
# 2nd - signature

NIST1 = [
"65f83408092261bda599389df03382c5be01a81fe00a36f3f4bb6541263f801627c440e50809712b0cace7c217e6e5051af81de9bfec3204dcd63c4f9a741047",

"0061387fd6b95914e885f912edfbb5fb274655027f216c4091ca83e19336740fd81aedfe047f51b42bdf68161121013e0d55b117a14e4303f926c8debb77a7fdaad100e7d0c75c38626e895ca21526b9f9fdf84dcecb93f2b233390550d2b1463b7ee3f58df7346435ff0434199583c97c665a97f12f706f2357da4b40288def888e59e6",

"004de826ea704ad10bc0f7538af8a3843f284f55c8b946af9235af5af74f2b76e099e4bc72fd79d28a380f8d4b4c919ac290d248c37983ba05aea42e2dd79fdd33e80087488c859a96fea266ea13bf6d114c429b163be97a57559086edb64aed4a18594b46fb9efc7fd25d8b2de8f09ca0587f54bd287299f47b2ff124aac566e8ee3b43"

    ]


NIST2 = [
"a6200971c6a289e2fcb80f78ec08a5079ea2675efd68bcab479552aa5bcb8edf3c993c79d7cebcc23c20e5af41723052b871134cc71d5c57206182a7068cc39b",

"004d5c8afee038984d2ea96681ec0dccb6b52dfa4ee2e2a77a23c8cf43ef19905a34d6f5d8c5cf0981ed804d89d175b17d1a63522ceb1e785c0f5a1d2f3d15e513520014368b8e746807b2b68f3615cd78d761a464ddd7918fc8df51d225962fdf1e3dc243e265100ff0ec133359e332e44dd49afd8e5f38fe86133573432d33c02fa0a3",

"01a3c4a6386c4fb614fba2cb9e74201e1aaa0001aa931a2a939c92e04b8344535a20f53c6e3c69c75c2e5d2fe3549ed27e6713cb0f4a9a94f6189eb33bff7d453fce016a997f81aa0bea2e1469c8c1dab7df02a8b2086ba482c43af04f2174831f2b1761658795adfbdd44190a9b06fe10e578987369f3a2eced147cff89d8c2818f7471"

]


NIST3 = [
"46ff533622cc90321a3aeb077ec4db4fbf372c7a9db48b59de7c5d59e6314110676ba5491bd20d0f02774eef96fc2e88ca99857d21ef255184c93fb1ff4f01d3",

"00c2d540a7557f4530de35bbd94da8a6defbff783f54a65292f8f76341c996cea38795805a1b97174a9147a8644282e0d7040a6f83423ef2a0453248156393a1782e0119f746c5df8cec24e4849ac1870d0d8594c799d2ceb6c3bdf891dfbd2242e7ea24d6aec3166214734acc4cbf4da8f71e2429c5c187b2b3a048527c861f58a9b97f",

"010ed3ab6d07a15dc3376494501c27ce5f78c8a2b30cc809d3f9c3bf1aef437e590ef66abae4e49065ead1af5f752ec145acfa98329f17bca9991a199579c41f9229008c3457fe1f93d635bb52df9218bf3b49a7a345b8a8a988ac0a254340546752cddf02e6ce47eee58ea398fdc9130e55a4c09f5ae548c715f5bcd539f07a34034d78"

]

NIST4 = [
"6b514f8d85145e30ced23b4b22c85d79ed2bfcfed5b6b2b03f7c730f1981d46d4dadd6699c28627d41c8684bac305b59eb1d9c966de184ae3d7470a801c99fd4",

"0160d7ea2e128ab3fabd1a3ad5455cb45e2f977c2354a1345d4ae0c7ce4e492fb9ff958eddc2aa61735e5c1971fa6c99beda0f424a20c3ce969380aaa52ef5f5daa8014e4c83f90d196945fb4fe1e41913488aa53e24c1d2142d35a1eed69fed784c0ef44d71bc21afe0a0065b3b87069217a5abab4355cf8f4ceae5657cd4b9c8008f1f",

"01e2bf98d1186d7bd3509f517c220de51c9200981e9b344b9fb0d36f34d969026c80311e7e73bb13789a99e0d59e82ebe0e9595d9747204c5f5550c30d934aa30c05012fed45cc874dc3ed3a11dd70f7d5c61451fbea497dd63e226e10364e0718d3722c27c7b4e5027051d54b8f2a57fc58bc070a55b1a5877b0f388d768837ef2e9cec"

]

NIST5 =[
"53c86e0b08b28e22131324f6bfad52984879ab09363d6b6c051aac78bf3568be3faeade6a2dda57dece4527abaa148326d3adbd2d725374bdac9ccb8ac39e51e",

"01ceee0be3293d8c0fc3e38a78df55e85e6b4bbce0b9995251f0ac55234140f82ae0a434b2bb41dc0aa5ecf950d4628f82c7f4f67651b804d55d844a02c1da6606f701f775eb6b3c5e43fc754052d1f7fc5b99137afc15d231a0199a702fc065c917e628a54e038cbfebe05c90988b65183b368a2061e5b5c1b025bbf2b748fae00ba297",

"014dfa43046302b81fd9a34a454dea25ccb594ace8df4f9d98556ca5076bcd44b2a9775dfaca50282b2c8988868e5a31d9eb08e794016996942088d43ad3379eb9a10120be63bd97691f6258b5e78817f2dd6bf5a7bf79d01b8b1c3382860c4b00f89894c72f93a69f3119cb74c90b03e9ede27bd298b357b9616a7282d176f3899aaa24"

]

NIST6 = [
"a9e9a9cb1febc380a22c03bacd18f8c46761180badd2e58b94703bd82d5987c52baec418388bc3f1e6831a130c400b3c865c51b73514f5b0a9026d9e8da2e342",
"014cab9759d4487987b8a00afd16d7199585b730fb0bfe63796272dde9135e7cb9e27cec51207c876d9214214b8c76f82e7363f5086902a577e1c50b4fbf35ce996601a83f0caa01ca2166e1206292342f47f358009e8b891d3cb817aec290e0cf2f47e7fc637e39dca03949391839684f76b94d34e5abc7bb750cb44486cce525eb0093",

"011a1323f6132d85482d9b0f73be838d8f9e78647934f2570fededca7c234cc46aa1b97da5ac1b27b714f7a171dc4209cbb0d90e4f793c4c192dc039c31310d6d99b00386a5a0fc55d36ca7231a9537fee6b9e51c2255363d9c9e7cb7185669b302660e23133eb21eb56d305d36e69a79f5b6fa25b46ec61b7f699e1e9e927fb0bceca06"

]

NIST7 = [
"7e324819033de8f2bffded5472853c3e68f4872ed25db79636249aecc24242cc3ca229ce7bd6d74eac8ba32f779e7002095f5d452d0bf24b30e1ce2eb56bb413",

"009da1536154b46e3169265ccba2b4da9b4b06a7462a067c6909f6c0dd8e19a7bc2ac1a47763ec4be06c1bec57d28c55ee936cb19588cc1398fe4ea3bd07e6676b7f014150cdf25da0925926422e1fd4dcfcffb05bdf8682c54d67a9bd438d21de5af43a15d979b320a847683b6d12ac1383a7183095e9da491c3b4a7c28874625e70f87",

"000dbf787ce07c453c6c6a67b0bf6850c8d6ca693a3e9818d7453487844c9048a7a2e48ff982b64eb9712461b26b5127c4dc57f9a6ad1e15d8cd56d4fd6da718642900c6f1c7774caf198fc189beb7e21ca92ceccc3f9875f0e2d07dc1d15bcc8f210b6dd376bf65bb6a454bf563d7f563c1041d62d6078828a57538b25ba54723170665"

]


NIST8 = [
"4541f9a04b289cd3b13d31d2f513d9243b7e8c3a0cbd3e0c790892235a4d4569ef8aef62444ecc64608509e6ad082bf7cd060d172550faa158b2fd396aa1e37b",

"000bbd4e8a016b0c254e754f68f0f4ed081320d529ecdc7899cfb5a67dd04bc85b3aa6891a3ed2c9861ae76c3847d81780c23ad84153ea2042d7fd5d517a26ff3ce400645953afc3c1b3b74fdf503e7d3f982d7ee17611d60f8eb42a4bddbec2b67db1f09b54440c30b44e8071d404658285cb571462001218fc8c5e5b98b9fae28272e6",

"014bf63bdbc014aa352544bd1e83ede484807ed760619fa6bc38c4f8640840195e1f2f149b29903ca4b6934404fb1f7de5e39b1ea04dba42819c75dbef6a93ebe269005d1bcf2295240ce4415042306abd494b4bda7cf36f2ee2931518d2454faa01c606be120b057062f2f3a174cb09c14f57ab6ef41cb3802140da22074d0e46f908d4"

]

NIST9 = [
"7ec0906f9fbe0e001460852c0b6111b1cd01c9306c0c57a5e746d43f48f50ebb111551d04a90255b22690d79ea60e58bed88220d485daaf9b6431740bb499e39",

"00157d80bd426f6c3cee903c24b73faa02e758607c3e102d6e643b7269c299684fdaba1acddb83ee686a60acca53cddb2fe976149205c8b8ab6ad1458bc00993cc43016e33cbed05721b284dacc8c8fbe2d118c347fc2e2670e691d5d53daf6ef2dfec464a5fbf46f8efce81ac226915e11d43c11c8229fca2327815e1f8da5fe95021fc",

"007e315d8d958b8ce27eaf4f3782294341d2a46fb1457a60eb9fe93a9ae86f3764716c4f5f124bd6b114781ed59c3f24e18aa35c903211b2f2039d85862932987d6801bcc1d211ebc120a97d465b603a1bb1e470109e0a55d2f1b5c597803931bd6d7718f010d7d289b31533e9fcef3d141974e5955bc7f0ee342b9cad05e29a3dded30e"

]

NIST10 = [
"7230642b79eed2fd50f19f79f943d67d6ef609ec06c9adbb4b0a62126926080ecd474922d1af6c01f4c354affde016b284b13dbb3122555dea2a2e6ca2a357dc",

"007002872c200e16d57e8e53f7bce6e9a7832c387f6f9c29c6b75526262c57bc2b56d63e9558c5761c1d62708357f586d3aab41c6a7ca3bf6c32d9c3ca40f9a2796a01fe3e52472ef224fb38d5a0a14875b52c2f50b82b99eea98d826c77e6a9ccf798de5ffa92a0d65965f740c702a3027be66b9c844f1b2e96c134eb3fdf3edddcf11c",

"00d732ba8b3e9c9e0a495249e152e5bee69d94e9ff012d001b140d4b5d082aa9df77e10b65f115a594a50114722db42fa5fbe457c5bd05e7ac7ee510aa68fe7b1e7f0134ac5e1ee339727df80c35ff5b2891596dd14d6cfd137bafd50ab98e2c1ab4008a0bd03552618d217912a9ec502a902f2353e757c3b5776309f7f2cfebf913e9cd"

]



NIST_ = [NIST1, NIST2, NIST3, NIST4, NIST5, NIST6, NIST7, NIST8, NIST9, NIST10]


i = 0
passCount = 0
failCount = 0
didntExecute = 0
binaryName = "NIST_test_vectors.bin"
for tc in NIST_:
    createBinaryFromTestCases(i, tc, binaryName)
    i+=1
flag = runTest()

