ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -g -march=native -mtune=native -fstrict-aliasing
#CPP_FLAGS= -g -march=native -mtune=native

decompose: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convert1d

.PHONY:clean
clean:
	rm convert1d
