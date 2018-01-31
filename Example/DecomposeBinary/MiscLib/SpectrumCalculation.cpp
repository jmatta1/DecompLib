#include"SpectrumCalculation.h"

DataVector<double>* calculateSumSpectrum(const DecompVector<double>& dv, const RespMatrix<double>& respMat)
{
    int numRespFunc = respMat.getNumRespFuncs();
    int numDataBins = respMat.getRespFuncsLens();
    //use kahan summation
    //allocate arrays
    double* sums = new double[numDataBins];
    double* comps = new double[numDataBins];
    //initialize arrays
    for(int i=0; i<numDataBins; ++i)
    {
        sums[i] = 0.0;
        comps[i] = 0.0;
    }
    //loop through the response functions and apply kahan summation to each bin
    for(int i=0; i<numRespFunc; ++i)
    {
        for(int j=0; j<numDataBins; ++j)
        {
            double tempy = ((respMat.getElement(i, j)*dv.getElement(i)) - comps[j]);
            double tempt = sums[j] + tempy;
            comps[j] = ((tempt - sums[j]) - tempy);
            sums[j] = tempt;
        }
    }
    //allocate the output
    DataVector<double>* output = new DataVector<double>(numDataBins);
    for(int i=0; i<numDataBins; ++i)
    {
        output->setElement(i, sums[i]);
    }
    //cleanup and return
    delete[] comps;
    delete[] sums;
    return output;
}

DataVector<double>* calculateResidSpectrum(const DataVector<double>& input, const DataVector<double>& sum)
{
    int numDataBins = input.getLength();
    DataVector<double>* output = new DataVector<double>(numDataBins);
    for(int i=0; i<numDataBins; ++i)
    {
        output->setElement(i, (input.getElement(i) - sum.getElement(i)));
    }
    return output;
}
