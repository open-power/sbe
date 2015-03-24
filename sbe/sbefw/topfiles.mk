TOP-CPP-SOURCES = sbemain.C  sbeirq.C sbecmdreceiver.C sbecmdprocessor.C sbecmdparser.C sbecmdscomaccess.C sbecmdiplcontrol.C sbefifo.C
TOP-C-SOURCES =
TOP-S-SOURCES =

TOP_OBJECTS = $(TOP-C-SOURCES:.c=.o) $(TOP-CPP-SOURCES:.C=.o) $(TOP-S-SOURCES:.S=.o)
