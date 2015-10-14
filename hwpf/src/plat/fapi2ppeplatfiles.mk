#  @file fapi2ppefiles.mk
#
#  @brief mk for including fapi2 object files
#
#  @page ChangeLogs Change Logs
#  @section fapi2ppeplatfiles.mk
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


FAPI2PLAT-CPP-SOURCES +=  target.C
FAPI2PLAT-CPP-SOURCES +=  plat_utils.C

FAPI2PLAT-S-SOURCES =

FAPI2PLATLIB_OBJECTS += $(FAPI2PLAT-CPP-SOURCES:.C=.o) 
FAPI2PLATLIB_OBJECTS += $(FAPI2PLAT-S-SOURCES:.S=.o)

