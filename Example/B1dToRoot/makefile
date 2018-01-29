ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 -g -march=native -mtune=native -fstrict-aliasing
#CPP_FLAGS= -g -march=native -mtune=native

convert1dToRoot: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convert1dToRoot

.PHONY:clean
clean:
	rm convert1dToRoot
