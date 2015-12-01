#  @file libcommonfiles.mk
#
#  @brief mk for including library common object files
#
##########################################################################
# Object Files
##########################################################################

LIB-CPP-SOURCES += p9_common_poweronoff.C
LIB-CPP-SOURCES += p9_common_pro_epi_log.C

LIB-C-SOURCES   +=
LIB-S-SOURCES   +=

LIB_OBJECTS     += $(LIB-CPP-SOURCES:.C=.o)
LIB_OBJECTS     += $(LIB-C-SOURCES:.c=.o)
LIB_OBJECTS     += $(LIB-S-SOURCES:.S=.o)

