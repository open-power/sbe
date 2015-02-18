#  @file pkkernelfiles.mk
#
#  @brief mk for including architecture independent pk object files
#
#  @page ChangeLogs Change Logs
#  @section pkkernelfiles.mk
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
# Include
##########################################################################


##########################################################################
# Object Files 
##########################################################################
PK-C-SOURCES = pk_core.c pk_init.c pk_stack_init.c

PK-TIMER-C-SOURCES += pk_timer_core.c pk_timer_init.c

PK-THREAD-C-SOURCES += pk_thread_init.c pk_thread_core.c \
	 pk_semaphore_init.c pk_semaphore_core.c

PK_OBJECTS += $(PK-C-SOURCES:.c=.o)

