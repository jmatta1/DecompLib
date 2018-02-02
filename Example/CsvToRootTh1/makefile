#third level makefile that builds the CsvToRootTh1 utility
ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -march=native -mtune=native

convert1dCsv: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convert1dCsv

.PHONY:cleanall
cleanall:
	-rm convert1dCsv
