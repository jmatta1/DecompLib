#ifndef DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMIO_H
#define DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMIO_H

#include"DecompLib/DataVector.h"
#include"DecompLib/DecompVector.h"
#include"DecompLib/RespMatrix.h"

//reads a 1-d spectrum binary file at a given path and returns it as a data vector
DataVector<double>* readDataVector(const std::string& fileName);
//reads a 2-d matrix binary file at a given path and returns it as a response matrix
RespMatrix<double>* readResponseMatrix(const std::string& fileName);
//takes a file path and decomp vector and writes a 1-d spectrum file from the decomp vector
void writeDecompData(const std::string& fileName, const DecompVector<double>& dv);
//takes a file path and spectrum and writes a 1-d spectrum file from the spectrum data
void writeSpectumData(const std::string& fileName, const DataVector<double>& dv);

#endif //DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMIO_H
