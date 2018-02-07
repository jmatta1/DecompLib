#third level makefile that builds the CsvToRootTh1 utility
ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -march=native -mtune=native

convert2dCsv: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convert2dCsv

.PHONY:cleanall
cleanall:
	-rm convert2dCsv
