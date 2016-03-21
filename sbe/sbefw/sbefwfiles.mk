SBEFW-CPP-SOURCES = sbemain.C
SBEFW-CPP-SOURCES += sbeirq.C
SBEFW-CPP-SOURCES += sbecmdreceiver.C
SBEFW-CPP-SOURCES += sbecmdprocessor.C
SBEFW-CPP-SOURCES += sbecmdparser.C
SBEFW-CPP-SOURCES += sbecmdscomaccess.C
SBEFW-CPP-SOURCES += sbecmdiplcontrol.C
SBEFW-CPP-SOURCES += pool.C
SBEFW-CPP-SOURCES += sbecmdgeneric.C
SBEFW-CPP-SOURCES += sbeFifoMsgUtils.C
SBEFW-CPP-SOURCES += sbecmdmemaccess.C
SBEFW-CPP-SOURCES += sbeHostUtils.C
SBEFW-CPP-SOURCES += sbecmdcntrldmt.C
SBEFW-CPP-SOURCES += sbecmdsram.C
SBEFW-CPP-SOURCES += sberegaccess.C
SBEFW-CPP-SOURCES += sbecmdcntlinst.C
SBEFW-CPP-SOURCES += sbecmdregaccess.C

SBEFW-C-SOURCES =
SBEFW-S-SOURCES =

SBEFW_OBJECTS = $(SBEFW-C-SOURCES:.c=.o) $(SBEFW-CPP-SOURCES:.C=.o) $(SBEFW-S-SOURCES:.S=.o)
