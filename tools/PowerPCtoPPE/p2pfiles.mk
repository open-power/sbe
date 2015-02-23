#  @file p2pfiles.mk
#
#  @brief mk for including P2P support library object files
#

##########################################################################
# Object Files
##########################################################################
P2P-S-SOURCES = ppe42_mulhw.S ppe42_mulhwu.S ppe42_mullw.S \
                ppe42_divw.S ppe42_divwu.S

P2P_OBJECTS = $(P2P-S-SOURCES:.S=.o)


