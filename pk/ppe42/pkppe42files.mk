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
PPE42-C-SOURCES = ppe42_core.c \
				   ppe42_init.c \
				   ppe42_irq_core.c\
				   ppe42_gcc.c\
				   
PPE42-S-SOURCES =  ppe42_boot.S \
					ppe42_exceptions.S\
					div64.S\
					ppe42_timebase.S

PPE42-TIMER-C-SOURCES = 
PPE42-TIMER-S-SOURCES = 

PPE42-THREAD-C-SOURCES += 
PPE42-THREAD-S-SOURCES += ppe42_thread_init.S


PPE42_OBJECTS += $(PPE42-C-SOURCES:.c=.o) $(PPE42-S-SOURCES:.S=.o)



