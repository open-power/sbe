#  @file pkppefiles.mk
#
#  @brief mk for including ppe object files
#
#  @page ChangeLogs Change Logs
#  @section pkppefiles.mk
#  @verbatim
#
#
# Change Log ******************************************************************
# Flag     Defect/Feature  User        Date         Description
# ------   --------------  ----------  ------------ -----------
#
# @endverbatim
#
##########################################################################
# Object Files
##########################################################################

PPE-C-SOURCES = ppe_init.c
PPE-S-SOURCES =

PPE-TIMER-C-SOURCES = 
PPE-TIMER-S-SOURCES = 

PPE-THREAD-C-SOURCES = 
PPE-THREAD-S-SOURCES = 

PPE-ASYNC-C-SOURCES =
PPE-ASYNC-S-SOURCES =

PPE_OBJECTS += $(PPE-C-SOURCES:.c=.o) $(PPE-S-SOURCES:.S=.o)

