# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/simics-debug-framework_rainier.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2021
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
from __future__ import print_function
import os
import os.path
import subprocess
import re
import random
import sys
import imp
import struct

SBE_TOOLS_PATH = simenv.sbe_scripts_path
print("SBE_TOOLS_PATH = " +  SBE_TOOLS_PATH)
# "Export SBE_TOOL_PATH" as OS environment variable.
os.environ['SBE_TOOLS_PATH'] = SBE_TOOLS_PATH

SBE_SEEPROM_IMG = simenv.sbe_seeprom_img
print("SBE_SEEPROM_IMG = " +  SBE_SEEPROM_IMG)

NUM_CORES = simenv.num_cores_per_chip
print("Functional Num of Core = " +  str(NUM_CORES)) 

testIstepAuto = imp.load_source("testIstepAuto", SBE_TOOLS_PATH + "/testIstepAuto.py")
sbeDebug = imp.load_source("sbeDebug", SBE_TOOLS_PATH + "/sbe-debug.py")
sbeUpdateAttrPG = imp.load_source("sbeUpdateAttrPG", SBE_TOOLS_PATH + "/sbeModifyPGvalue.py")
err = False
simicsObj = simics.SIM_run_command("get-master-procs")
simicsObjForPrimarySeeprom = simics.SIM_run_command("get-seeprom 0 0 0")
simicsObjForBackupSeeprom = simics.SIM_run_command("get-seeprom 0 0 1")

syms = {};

def get_dd_level(procNr = 0, nodeNr = 0):
    return "DD1" 


def register_sbe_debug_framework_tools():
    fillSymTable()
    # Create command hook.
    new_command("sbe-istep",istep_func,
                 args = [arg(float_t, "Major/start istep"), arg(float_t, "Minor/end istep"), arg(int_t, "procNr", "?", 0), arg(int_t, "arg", "?", 0)],
                 alias = "istep",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for istep ",
                 doc = "")
    new_command("sbe-trace", collectTrace,
                args = [arg(int_t, "procNr"), arg(int_t, "arg", "?", 0)],
                alias = "strace",
                type = ["sbe-commands"],
                short = "Runs the debug framework for trace ",
                doc = "")
    new_command("sbe-stack", collectStackUsage,
                args = [arg(int_t, "procNr"), arg(int_t, "arg", "?", 0)],
                alias = "sstack",
                type = ["sbe-commands"],
                short = "Runs the debug framework for stack usage ",
                doc = "")
    new_command("sbe-ddlevel", get_dd_level,
                args = [arg(int_t, "procNr"), arg(int_t, "arg", "?", 0)],
                alias = "sddlevel",
                type = ["sbe-commands"],
                short = "Runs the debug framework for dd level ",
                doc = "")
    new_command("sbe-attrdump", collectAttr,
                args = [arg(int_t, "procNr"), arg(int_t, "arg", "?", 0)],
                alias = "sattrdump",
                type = ["sbe-commands"],
                short = "Runs the debug framework for runtime attribute dump ",
                doc = "")
    new_command("sbe-regffdc", collectRegFfdc,
                args = [arg(int_t, "procNr"), arg(int_t, "arg", "?", 0)],
                alias = "sregffdc",
                type = ["sbe-commands"],
                short = "Runs the debug framework for register ffdc ",
                doc = "")
    new_command("sbe-updatepg",updateAttrPG,
                args = [arg(str_t, "eccFile"), arg(int_t, "coreNr", "?", 2)],
                alias = "supdatepg",
                type = ["sbe-commands"],
                short = "Runs the debug framework for update EQ PG Attr ",
                doc = "")

    print("SBE Debug Framework: Registered tool:", "sbe-istep")
    print("SBE Debug Framework: Registered tool:", "sbe-trace")
    print("SBE Debug Framework: Registered tool:", "sbe-stack")
    print("SBE Debug Framework: Registered tool:", "sbe-ddlevel")
    print("SBE Debug Framework: Registered tool:", "sbe-attrdump")
    print("SBE Debug Framework: Registered tool:", "sbe-regffdc")
    print("SBE Debug Framework: Registered tool:", "sbe-updatepg")
    updateAttrPG(SBE_SEEPROM_IMG, NUM_CORES)

def fillSymTable():
    symFile = SBE_TOOLS_PATH + "/sbe_"+get_dd_level()+".syms"
    f = open( symFile, 'r')
    for line in f:
        words = line.split()
        if( len( words ) == 4 ):
            syms[words[3]] = [words[0], words[1]]

# Print least available stack of each thread in SBE during a Run.
#
# Logic is - during init, ppe kernel fills the stack memory with '0xEFCDAB03'.
# So while traversing the stack, starting from lowest memory to the top,
# the first memory address where the pattern('0xEFCDAB03') is broken,
# will be the deepest stack usage point of tht thread during the run
def collectStackUsage ( procNr, nodeNr=0 ):
  threads = ('sbeSyncCommandProcessor_stack',
             'sbeCommandReceiver_stack',
             'sbe_Kernel_NCInt_stack',
             'sbeAsyncCommandProcessor_stack')
  print("==================================Stack usage===================================")
  # Dump stack memory to binary files
  for thread in threads:
    cmd = "pipe \"" + simicsObj[procNr] + ".pib_cmp.sbe_mibo.x 0x" + syms[thread][0] + " 0x"+syms[thread][1]+"\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> "+thread+"\""
    print("simics running %s: "%( cmd))
    ( rc, out )  =   quiet_run_command( cmd, output_modes.regular )
    if ( rc ):
        print("simics ERROR running %s: %d "%( cmd, rc ))

  print("Thread".ljust(40)+"Least Available[bytes]".ljust(30)+"Max usage[%]")
  for thread in threads:
    with open(thread, "rb") as f:
        word = struct.unpack('I', f.read(4))[0]
        leastAvailable = 0
        while (1):
            if (word == int("0xEFCDAB03", 16)):
                leastAvailable += 4
                word = struct.unpack('I', f.read(4))[0]
            else:
                break
        print(str("["+thread+"]").ljust(40) + str(leastAvailable).ljust(30) + str("%.2f" % (100 * (1 - (leastAvailable/float(int("0x"+syms[thread][1],16)))))))

def updateAttrPG( eccFile, coreNr, procNr=0, nodeNr=0 ):
  cmd1 = "shell \"mkdir -p " + os.getcwd() + "/sbe_updated_seeprom_image \""
  quiet_run_command( cmd1, output_modes.regular )

  cmd2 = "shell \"cp " + eccFile + " " + os.getcwd() + "/sbe_updated_seeprom_image/ \""
  quiet_run_command( cmd2, output_modes.regular )

  filename = os.getcwd() + "/sbe_updated_seeprom_image/p10_10.sbe_seeprom.bin.ecc"
  sbeUpdateAttrPG.updateAttrPg(filename, coreNr)

  sfilename = "\"" + filename + "\""
  cmd3 = simicsObjForPrimarySeeprom + "->files = [[" + sfilename + ", \"ro\", 0, 0 ]]"
  ( rc, out ) = quiet_run_command( cmd3, output_modes.regular )
  if ( rc ):
    print("simics ERROR running %s: %d "%( cmd3, rc ))

  cmd4 = simicsObjForBackupSeeprom + "->files = [[" + sfilename + ", \"ro\", 0, 0 ]]"
  ( rc, out ) = quiet_run_command( cmd4, output_modes.regular )
  if ( rc ):
    print("simics ERROR running %s: %d "%( cmd4, rc ))

def collectAttr( procNr, nodeNr=0 ):
  cmd= "pipe \"" + simicsObj[procNr] + ".pib_cmp.sbe_mibo.x " + '0xFFFE8000' + " "+hex(96*1024)+"\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> DumpFullPIBMEM\""
  print("simics running %s: "%( cmd))
  ( rc, out )  =   quiet_run_command( cmd, output_modes.regular )
  if ( rc ):
    print("simics ERROR running %s: %d "%( cmd, rc ))
  ddlevel = get_dd_level(procNr, nodeNr)
  sbeDebug.ddsuffix = ddlevel
  sbeDebug.target = 'FILE'
  sbeDebug.file_path = 'DumpFullPIBMEM'
  sbeDebug.fillSymTable()
  sbeDebug.collectAttr()

def collectRegFfdc( procNr, nodeNr=0 ):
  cmd = "pipe \"" + simicsObj[procNr] + ".pib_cmp.sbe_mibo.x " + '0xFFFE8000' + " "+hex(96*1024)+"\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> DumpFullPIBMEM\""
  print("simics running %s: "%( cmd))
  ( rc, out )  =   quiet_run_command( cmd, output_modes.regular )
  if ( rc ):
    print("simics ERROR running %s: %d "%( cmd, rc ))
  ddlevel = get_dd_level(procNr, nodeNr)
  sbeDebug.ddsuffix = ddlevel
  sbeDebug.target = 'FILE'
  sbeDebug.file_path = 'DumpFullPIBMEM'
  sbeDebug.fillSymTable()
  sbeDebug.ppeStateFfdc()

def istep_func ( majorIstep, minorIstep, procNr=0, nodeNr=0):
  testIstepAuto.sbe_istep_func(majorIstep, minorIstep, procNr, nodeNr)

def collectTrace ( procNr, nodeNr=0 ):
  fileName = "sbe_" + repr(procNr) + "_tracMERG"
  cmd1 = "pipe \"" + simicsObj[procNr] + ".pib_cmp.sbe_mibo.x 0x" + syms['g_pk_trace_buf'][0] + " 0x2028\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> ppetrace.bin\""
  cmd2 = "shell \"" + SBE_TOOLS_PATH + "/ppe2fsp ppetrace.bin sbetrace.bin \""
  cmd3 = "shell \"" + SBE_TOOLS_PATH + "/fsp-trace -s " + SBE_TOOLS_PATH + "/sbeStringFile_"+get_dd_level(procNr, nodeNr)+" sbetrace.bin >" +  fileName + "\""
  cmd4 = "shell \"" + "cat " + fileName + "\""

  print("simics running %s: "%( cmd1))
  ( rc, out )  =   quiet_run_command( cmd1, output_modes.regular )
  if ( rc ):
    print("simics ERROR running %s: %d "%( cmd1, rc ))

  run_command ( cmd2 )
  run_command ( cmd3 )
  run_command ( cmd4 )

# MAGIC_INSTRUCTION hap handler
# arg contains the integer parameter n passed to MAGIC_INSTRUCTION(n)
# See src/include/arch/ppc.H for the definitions of the magic args.
# SBE  magic args should range 8000..8190.
def sbe_magic_instruction_callback(user_arg, cpu, inst_num):
    # Check if its for ppe processor
    if(SIM_get_class_name(SIM_object_class(cpu)) != "ppe"):
        return;

    if inst_num == 8000: #MAGIC_SIMICS_CHECK
        iface = SIM_get_interface(cpu, "int_register")
        iface.write(iface.get_number("r3"), 1)
        print("SBE::isSimicsRunning = true")

# Run the registration automatically whenever this script is loaded.
register_sbe_debug_framework_tools()

# Register the magic instruction hap handler (a callback).
# Currently registering a range does not work on simics for sbe
SIM_hap_add_callback( "Core_Magic_Instruction", sbe_magic_instruction_callback, None )

