#third level makefile that builds the RootTh1ToCsv utility
ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -march=native -mtune=native

convert1d: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convert1d

.PHONY:cleanall
cleanall:
	-rm convert1d
