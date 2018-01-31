#*******************************************************************************
#*******************************************************************************
#*
#* @copyright Copyright (C) 2018 James Till Matta
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#* 
#*******************************************************************************
#*******************************************************************************
DECOMP_LIB_LOCATION_ABS=/home/jmatta1/code/DecompLib  # change this to the path in your system
DECOMP_LIB_LOCATION_REL=../..
CPP_FLAGS=-O3 -g --std=c++11 -march=native -mtune=native
DECOMP_LIB_FLAG=-I${DECOMP_LIB_LOCATION_REL}

decompose: main.o MiscLib/UserIo.o MiscLib/SpectrumIo.o MiscLib/SpectrumCalculation.o
	g++ ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $^ -o $@

main.o: main.cpp MiscLib/UserIo.h MiscLib/SpectrumIo.h MiscLib/SpectrumCalculation.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

MiscLib/UserIo.o: MiscLib/UserIo.cpp MiscLib/UserIo.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

MiscLib/SpectrumIo.o: MiscLib/SpectrumIo.cpp MiscLib/SpectrumIo.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

MiscLib/SpectrumCalculation.o: MiscLib/SpectrumCalculation.cpp MiscLib/SpectrumCalculation.h
	g++ -c ${CPP_FLAGS} ${DECOMP_LIB_FLAG} $< -o $@

.PHONY:clean
clean:
	-rm *.o MiscLib/*.o

.PHONY:cleanall
cleanall:clean
	-rm decompose
