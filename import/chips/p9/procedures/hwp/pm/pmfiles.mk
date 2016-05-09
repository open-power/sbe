#  @file pmfiles.mk
#
#  @brief mk for including pm object files
#
##########################################################################
# Object Files
##########################################################################

PM-CPP-SOURCES +=p9_pm_ocb_indir_access.C
PM-CPP-SOURCES +=p9_pm_ocb_indir_setup_circular.C
PM-CPP-SOURCES +=p9_pm_ocb_indir_setup_linear.C
PM-CPP-SOURCES +=p9_pm_ocb_init.C
PM-CPP-SOURCES +=p9_pm_utils.C
PM-CPP-SOURCES +=p9_block_wakeup_intr.C
PM-C-SOURCES =
PM-S-SOURCES =

PM_OBJECTS += $(PM-CPP-SOURCES:.C=.o)
PM_OBJECTS += $(PM-C-SOURCES:.c=.o)
PM_OBJECTS += $(PM-S-SOURCES:.S=.o)
