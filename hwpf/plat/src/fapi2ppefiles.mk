#  @file fapi2ppefiles.mk
#
#  @brief mk for including fapi2 object files
#
#  @page ChangeLogs Change Logs
#  @section fapi2ppefiles.mk
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

FAPI2-C-SOURCES =  fapi2PlatAttributeService.C      
FAPI2-S-SOURCES =

FAPI2LIB_OBJECTS += $(FAPI2-C-SOURCES:.C=.o) $(FAPI2-S-SOURCES:.S=.o)

