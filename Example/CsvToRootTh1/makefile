#third level makefile that builds the CsvToRootTh1 utility
ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -march=native -mtune=native

convertCsv: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convertCsv

.PHONY:cleanall
cleanall:
	-rm convertCsv
