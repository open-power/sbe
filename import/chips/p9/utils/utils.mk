# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: chips/p9/utils/utils.mk $
#
# IBM CONFIDENTIAL
#
# EKB Project
#
# COPYRIGHT 2016
# [+] International Business Machines Corp.
#
#
# The source code for this program is not published or otherwise
# divested of its trade secrets, irrespective of what has been
# deposited with the U.S. Copyright Office.
#
# IBM_PROLOG_END_TAG
#  @file utils.mk
#
#  @brief mk for including Utils object files
#
##########################################################################
# Object Files
##########################################################################

UTILS-CPP-SOURCES = p9_putRingUtils.C

UTILS_OBJECTS = $(UTILS-CPP-SOURCES:.C=.o)
