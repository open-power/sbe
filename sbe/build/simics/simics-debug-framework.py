import os
import os.path
import subprocess
import re
import random
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testIstepAuto
err = False

syms = {};

def check_sbe_tools_path ():
  global SBE_TOOLS_PATH
  print "checking for SBE_TOOLS_PATH .."
  SB_SRC_PATH = os.environ['sb']
  SB_BB_PATH = os.environ['bb']
  SBE_TOOLS_REL_PATH = "/obj/ppc/sbei/sbfw/simics"
  TOOLS_IN_SB = os.environ['sb'] + "/.." + SBE_TOOLS_REL_PATH
  TOOLS_IN_BB = os.environ['bb'] + SBE_TOOLS_REL_PATH

  if os.path.exists(TOOLS_IN_SB):
    SBE_TOOLS_PATH = TOOLS_IN_SB 
    print "Found SBE_TOOLS_PATH at: ", SBE_TOOLS_PATH
  elif os.path.exists(TOOLS_IN_BB):
    SBE_TOOLS_PATH = TOOLS_IN_BB 
    print "Found SBE_TOOLS_PATH at: ", SBE_TOOLS_PATH
  else:
    print "C"
    print "Error: Could not find SBE_TOOLS_PATH!"
    SBE_TOOLS_PATH = ""

# MAGIC_INSTRUCTION hap handler
# arg contains the integer parameter n passed to MAGIC_INSTRUCTION(n)
# See src/include/arch/ppc.H for the definitions of the magic args.
# Hostboot magic args should range 7000..7999.
def magic_instruction_callback_sb(user_arg, cpu, arg):
    print "In magic_instruction_callback."
    print arg
    if arg == 8008:
        cpu.r3 = random.randint(1, 0xffffffffffffffffL)

    if arg == 8006:   # MAGIC_SHUTDOWN
        # KernelMisc::shutdown()
        print "KernelMisc::shutdown() called."
        # Could break/stop/pause the simics run, but presently
        # shutdown() is called four times. --Monte Jan 2012
        # SIM_break_simulation( "Shutdown. Simulation stopped." )


# @fn register_hb_debug_framework_tools
# @brief Create a simics command wrapper for each debug tool module.
def register_sbe_debug_framework_tools():
    check_sbe_tools_path ()
    fillSymTable()
    # Create command hook.
    new_command("sbe-istep", testIstepAuto.sbe_istep_func,
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


# Register the magic instruction hap handler (a callback).
SIM_hap_add_callback_range( "Core_Magic_Instruction", magic_instruction_callback_sb, None, 8000, 8190 )



# Run the registration automatically whenever this script is loaded.
register_sbe_debug_framework_tools()

