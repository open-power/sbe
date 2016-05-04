#  @file initfiles.mk
#
#  @brief mk for including initfile object files
#
##########################################################################
# Object Files
##########################################################################

INITFILES-CPP-SOURCES += p9_ncu_scom.C
INITFILES-CPP-SOURCES += p9_l2_scom.C
INITFILES-CPP-SOURCES += p9_l3_scom.C

INITFILES-C-SOURCES   +=
INITFILES-S-SOURCES   +=

INITFILES_OBJECTS     += $(INITFILES-CPP-SOURCES:.C=.o)
INITFILES_OBJECTS     += $(INITFILES-C-SOURCES:.c=.o)
INITFILES_OBJECTS     += $(INITFILES-S-SOURCES:.S=.o)

