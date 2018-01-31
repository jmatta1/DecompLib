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
ROOT_FLAGS=`root-config --cflags --libs`
CPP_FLAGS=-O3 --std=c++11 -march=native -mtune=native

decompose: main.cpp
	g++ ${CPP_FLAGS} ${ROOT_FLAGS} main.cpp -o convertCsv

.PHONY:clean
clean:
	rm convertCsv
