#  @file pkppe42files.mk
#
#  @brief mk for including ppe42 object files
#
#  @page ChangeLogs Change Logs
#  @section pkppe42files.mk
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
# Include Files
##########################################################################

			

##########################################################################
# Object Files
##########################################################################
PKTRACE-C-SOURCES = pk_trace_core.c pk_trace_big.c pk_trace_binary.c
				   
PKTRACE-S-SOURCES = 

PKTRACE-TIMER-C-SOURCES = 
PKTRACE-TIMER-S-SOURCES = 

PKTRACE-THREAD-C-SOURCES += 
PKTRACE-THREAD-S-SOURCES += 


PKTRACE_OBJECTS += $(PKTRACE-C-SOURCES:.c=.o) $(PKTRACE-S-SOURCES:.S=.o)



