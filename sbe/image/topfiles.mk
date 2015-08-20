TOP-C-SOURCES = base_ppe_main.c sbe_loader.c
TOP-CPP-SOURCES =
TOP-S-SOURCES = base_ppe_header.S

TOP-FIXED-HEADERS += $(IMAGE_SRCDIR)/proc_sbe_fixed_perv.H
TOP-FIXED-HEADERS += $(IMAGE_SRCDIR)/proc_sbe_fixed_proc_chip.H
TOP-FIXED-HEADERS += $(IMAGE_SRCDIR)/proc_sbe_fixed_core.H
TOP-FIXED-HEADERS += $(IMAGE_SRCDIR)/proc_sbe_fixed_ex.H
TOP-FIXED-HEADERS += $(IMAGE_SRCDIR)/proc_sbe_fixed_eq.H

BASE-LOADER-C-SOURCES = base_loader.c

TOP_OBJECTS = $(TOP-C-SOURCES:.c=.o) $(TOP-CPP-SOURCES:.C=.o) $(TOP-S-SOURCES:.S=.o)
BASE_LOADER_OBJECTS = $(BASE-LOADER-C-SOURCES:.c=.o)
