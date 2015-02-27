#  @file pkstdfiles.mk
#
#  @brief mk for including std object files
#
#  @page ChangeLogs Change Logs
#  @section pkstdfiles.mk
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

STD-C-SOURCES = std_init.c std_irq_init.c
STD-S-SOURCES =

STD-TIMER-C-SOURCES = 
STD-TIMER-S-SOURCES = 

STD-THREAD-C-SOURCES = 
STD-THREAD-S-SOURCES = 

STD-ASYNC-C-SOURCES =
STD-ASYNC-S-SOURCES =

STD_OBJECTS += $(STD-C-SOURCES:.c=.o) $(STD-S-SOURCES:.S=.o)

