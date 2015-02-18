#  @file pkcmefiles.mk
#
#  @brief mk for including cme object files
#
#  @page ChangeLogs Change Logs
#  @section pkcmefiles.mk
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

CME-C-SOURCES = cme_init.c
CME-S-SOURCES =

CME-TIMER-C-SOURCES = 
CME-TIMER-S-SOURCES = 

CME-THREAD-C-SOURCES = 
CME-THREAD-S-SOURCES = 

CME-ASYNC-C-SOURCES =
CME-ASYNC-S-SOURCES =

CME_OBJECTS += $(CME-C-SOURCES:.c=.o) $(CME-S-SOURCES:.S=.o)

