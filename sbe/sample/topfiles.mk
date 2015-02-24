TOP-C-SOURCES = pk_trace_wrap.c pk_scom.c
TOP-CPP-SOURCES = sample_main.C
TOP-S-SOURCES =


TOP_OBJECTS = $(TOP-C-SOURCES:.c=.o) $(TOP-CPP-SOURCES:.C=.o) $(TOP-S-SOURCES:.S=.o)
