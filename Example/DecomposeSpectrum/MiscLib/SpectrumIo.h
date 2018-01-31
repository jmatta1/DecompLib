/*!*****************************************************************************
********************************************************************************
**
** @copyright Copyright (C) 2018 James Till Matta
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** 
********************************************************************************
*******************************************************************************/
#ifndef DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMIO_H
#define DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMIO_H

#include<tuple>
#include<string>

#include"DecompLib/DataVector.h"
#include"DecompLib/DecompVector.h"
#include"DecompLib/RespMatrix.h"

#include"SimpleStructs.h"

//reads a 2-d matrix binary file at a given path and returns it as a response matrix
std::tuple<RespMatrix<double>*, TwoDMetaData*> readResponseMatrix(const std::string& fileName);
//reads a 1-d spectrum binary file at a given path and returns it as a data vector
std::tuple<DataVector<double>*, OneDMetaData*> readDataVector(const std::string& fileName);
//takes a file path and decomp vector and writes a 1-d spectrum file from the decomp vector
void writeDecompData(const std::string& fileName, const DecompVector<double>& dv, const OneDMetaData& meta);
//takes a file path and spectrum and writes a 1-d spectrum file from the spectrum data
void writeSpectumData(const std::string& fileName, const DataVector<double>& dv, const OneDMetaData& meta);


namespace Detail
{
//reads lines from the input stream until it finds a line that does not have '#' at index 0
//if the next line is not a comment this is functionally identical to getline
bool readAndIgnoreCommentLines(std::istream& input, std::string& str);
//reads an axis information line and breaks it into the number of bins and the edges
std::tuple<int, double, double> parseAxisInfoLine(const std::string& line);
//reads a 2d data line and breaks it into the X bin number, Y bin number, and value
std::tuple<int, int, double> parse2dDataLine(const std::string& line);
//reads a 1d data line and breaks it into the X bin number, and value
std::tuple<int, double> parse1dDataLine(const std::string& line);
}

#endif //DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMIO_H
