#  @file pkgpefiles.mk
#
#  @brief mk for including gpe object files
#
#  @page ChangeLogs Change Logs
#  @section pkgpefiles.mk
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

GPE-C-SOURCES = gpe_init.c gpe_irq_init.c
GPE-S-SOURCES = gpe_scom_handler.S

GPE-TIMER-C-SOURCES = 
GPE-TIMER-S-SOURCES = 

GPE-THREAD-C-SOURCES = 
GPE-THREAD-S-SOURCES = 

GPE-ASYNC-C-SOURCES =
GPE-ASYNC-S-SOURCES =

GPE_OBJECTS += $(GPE-C-SOURCES:.c=.o) $(GPE-S-SOURCES:.S=.o)

