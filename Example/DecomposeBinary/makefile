DECOMP_LIB_LOCATION_ABS=/home/jmatta1/code/DecompLib  # change this to the path in your system
DECOMP_LIB_LOCATION_REL=../..
CPP_FLAGS=-O3 -g --std=c++11 -march=native -mtune=native -fstrict-aliasing
DECOMP_LIB_FLAG=-I${DECOMP_LIB_LOCATION_REL}

decompose: main.o UserIo.o SpectrumIo.o SpectrumCalculation.o
	g++ ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $^ -o $@

main.o: main.cpp UserIo.h SpectrumIo.h SpectrumCalculation.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

UserIo.o: UserIo.cpp UserIo.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

SpectrumIo.o: SpectrumIo.cpp SpectrumIo.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

SpectrumCalculation.o: SpectrumCalculation.cpp SpectrumCalculation.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

.PHONY:clean
clean:
	-rm *.o

.PHONY:cleanall
cleanall:clean
	-rm decompose
