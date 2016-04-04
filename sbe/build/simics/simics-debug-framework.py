import os
import os.path
import subprocess
import re
import random
import sys
import imp
testIstepAuto = imp.load_source("testIstepAuto", os.environ['SBE_TOOLS_PATH'] + "/testIstepAuto.py")
err = False

syms = {};

def check_sbe_tools_path ():
  global SBE_TOOLS_PATH
  SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];

def register_sbe_debug_framework_tools():
    check_sbe_tools_path ()
    fillSymTable()
    # Create command hook.
    new_command("sbe-istep",testIstepAuto.sbe_istep_func,
                 args = [arg(int_t, "major"), arg(int_t, "minor")],
                 alias = "istep",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for istep ",
                 doc = "")
    new_command("sbe-trace", collectTrace,
                 args = [],
                 alias = "strace",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for istep ",
                 doc = "")
    print "SBE Debug Framework: Registered tool:", "sbe-istep"
    print "SBE Debug Framework: Registered tool:", "sbe-trace"


def fillSymTable():
#    symFile = os.environ['SBE_IMG_OUT_LOC'] +  "/sbe.syms"
    symFile = SBE_TOOLS_PATH + "/sbe.syms"
#    symFile = os.environ['sb'] + "/../obj/ppc/sbei/sbfw/simics/sbe.syms"
    f = open( symFile, 'r')
    for line in f:
        words = line.split()
        if( len(words) == 3):
            syms[words[2]] = words[0]

def collectTrace():
  cmd1 = "pipe \"p9Proc0.sbe.mibo_space.x 0x"  + syms['g_pk_trace_buf'] + " 0x2028\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> ppetrace.bin\""
  cmd2 = "shell \"" + SBE_TOOLS_PATH + "/ppe2fsp ppetrace.bin sbetrace.bin \""
  cmd3 = "shell \"" + SBE_TOOLS_PATH + "/fsp-trace -s " + SBE_TOOLS_PATH + "/trexStringFile sbetrace.bin > tracMERG \""
  cmd4 = "shell \"" + "cat tracMERG \""

  ( rc, out )  =   quiet_run_command( cmd1, output_modes.regular )
  if ( rc ):
    print "simics ERROR running %s: %d "%( cmd1, rc )

  SIM_run_alone( run_command, cmd2 )
  SIM_run_alone( run_command, cmd3 )
  SIM_run_alone( run_command, cmd4 )


# Run the registration automatically whenever this script is loaded.
register_sbe_debug_framework_tools()

