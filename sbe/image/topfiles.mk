TOP-C-SOURCES = base_ppe_demo.c
TOP-CPP-SOURCES = base_main.C
TOP-S-SOURCES = base_ppe_header.S


TOP_OBJECTS = $(TOP-C-SOURCES:.c=.o) $(TOP-CPP-SOURCES:.C=.o) $(TOP-S-SOURCES:.S=.o)
