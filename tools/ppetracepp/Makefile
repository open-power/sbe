all: ppetracepp ppe2fsp

ppetracepp: ppetracepp.C
	g++ -m32 -O3 -w -g -I./ ppetracepp.C -o ppetracepp
#	g++ -O3 -w -x c++ -fPIC -g -I./ ppetracepp.C -o ppetracepp

ppe2fsp: ppe2fsp.c ppe2fsp_cmd.c
	gcc -m32 -w -g -I./ -I../../pk/trace ppe2fsp.c ppe2fsp_cmd.c -o ppe2fsp

clean:
	rm ppetracepp ppe2fsp

