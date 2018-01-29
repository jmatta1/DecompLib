#include"SpectrumIo.h"
#include<fstream>
#include<iostream>

RespMatrix<double>* readResponseMatrix(const std::string& fileName)
{
    //open the file and prepare a readout buffer
    std::ifstream input;
    input.open(fileName.c_str(), std::ios_base::binary);
    char buffer[sizeof(double)];
    //read the number of bins in each response function
    input.read(buffer, sizeof(int));
    int numBins = reinterpret_cast<int*>(buffer)[0];
    //read the number of response functions in the matrix
    input.read(buffer, sizeof(int));
    int numResp = reinterpret_cast<int*>(buffer)[0];
    //allocate the response matrix
    RespMatrix<double>* temp = new RespMatrix<double>(numResp, numBins);
    //read the response matrix in
    for(int i=0; i<numResp; ++i)
    {
        for(int j=0; j<numBins; ++j)
        {
            input.read(buffer, sizeof(double));
            temp->setElement(i, j, reinterpret_cast<double*>(buffer)[0]);
        }
    }
    //close the file and return
    input.close();
    return temp;
}

DataVector<double>* readDataVector(const std::string& fileName)
{
    //open the file and prepare a readout buffer
    std::ifstream input;
    input.open(fileName.c_str(), std::ios_base::binary);
    char buffer[sizeof(double)];
    //read the number of bins in the histogram
    input.read(buffer, sizeof(int));
    int numBins = reinterpret_cast<int*>(buffer)[0];
    //allocate the data vector
    DataVector<double>* temp = new DataVector<double>(numBins);
    //read the spectrum in
    for(int i=0; i<numBins; ++i)
    {
        input.read(buffer, sizeof(double));
        temp->setElement(i, reinterpret_cast<double*>(buffer)[0]);
    }
    //close the file and return
    input.close();
    return temp;
}

void writeDecompData(const std::string& fileName, const DecompVector<double>& dv)
{
    //create the fstream and open it
    std::ofstream output;
    output.open(fileName.c_str(), std::ios_base::binary);
    //make a simply buffer for output
    char buffer[sizeof(double)];
    //write the number of bins in the histogram
    int numBins = dv.getLength();
    reinterpret_cast<int*>(buffer)[0] = numBins;
    output.write(buffer, sizeof(int));
    //write the bin contents
    for(int i=0; i<numBins; ++i)
    {
        reinterpret_cast<double*>(buffer)[0] = dv.getElement(i);
        output.write(buffer, sizeof(double));
    }
    output.close();
}

void writeSpectumData(const std::string& fileName, const DataVector<double>& dv)
{
    //create the fstream and open it
    std::ofstream output;
    output.open(fileName.c_str(), std::ios_base::binary);
    //make a simply buffer for output
    char buffer[sizeof(double)];
    //write the number of bins in the histogram
    int numBins = dv.getLength();
    reinterpret_cast<int*>(buffer)[0] = numBins;
    output.write(buffer, sizeof(int));
    //write the bin contents
    for(int i=0; i<numBins; ++i)
    {
        reinterpret_cast<double*>(buffer)[0] = dv.getElement(i);
        output.write(buffer, sizeof(double));
    }
    output.close();
}

