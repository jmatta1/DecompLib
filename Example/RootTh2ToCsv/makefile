#third level makefile that builds the RootTh2ToCsv utility
ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -march=native -mtune=native

decompose: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convert2d

.PHONY:cleanall
cleanall:
	-rm convert2d
