#include<iostream>
#include<string>
#include<sstream>
#include"UserIo.h"
#include"SpectrumIo.h"
#include"SpectrumCalculation.h"
#include"DecompLib/Decomposition.h"

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"Spectrum Decomposer"<<std::endl;
        std::cout<<"  A program to decompose an input spectrum using a response matrix."<<std::endl;
        std::cout<<"    The response matrix and input spectrum must be in the simple binary\n"
                 <<"    format produced by the programs in Root1dToB1d and Rood2dToB2d"<<std::endl;
        std::cout<<"  The outputs will be in the same format and can be converted to root\n"
                 <<"    format with the program in B1dToRoot"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <spectrumFile> <responseMatrixFile> <outputFileNameBase>"
                 <<std::endl;
        return 1;
    }
    int initValSetting = getStartingValType();
    double startVal = ((initValSetting==0)?getScalingFactor():getStartingConstant());
    double minThresh = getMinThreshold();
    double convCrit = getConvergence();
    
    std::cout<<"\n\nPerforming Decomposition With: \n";
    std::cout<<"  Convergence Check Minimum Threshold: "<<minThresh<<std::endl;
    std::cout<<"  Convergence Maximum Change: "<<convCrit<<std::endl;
    std::cout<<"  Initial Values Set To:\n";
    if(initValSetting == 0)
    {
        std::cout<<"    Spectrum Scaled By: "<<startVal<<std::endl;
    }
    else
    {
        std::cout<<"    Constant Value Set To: "<<startVal<<std::endl;
    }
    
    DataVector<double>* inputSpectrum = readDataVector(std::string(argv[1]));
    RespMatrix<double>* responseMatrix = readResponseMatrix(std::string(argv[2]));
    DecompVector<double> decompOrig(responseMatrix->getNumRespFuncs());
    DecompVector<double> decompProc(responseMatrix->getNumRespFuncs());
    if(initValSetting == 0)
    {
        decompOrig.initWithDataVector(*inputSpectrum, startVal, 0.0);
        decompProc.initWithDataVector(*inputSpectrum, startVal, 0.0);
    }
    else
    {
        decompOrig.initWithConstant(startVal);
        decompProc.initWithConstant(startVal);
    }
    
    //decompose
    long long retVal = performDecomposition(*inputSpectrum, *responseMatrix,
                                            decompProc, minThresh, convCrit);
    //tell the user about success or failure
    switch(retVal)
    {
    case -1:
        std::cout<<"A row or column of the response matrix contains all zeros" << std::endl;
        return 1;
    case -2:
        std::cout << "Initial guess may have one or more values that are less than or equal to zero" << std::endl;
        return 1;
    case -3:
        std::cout << "Data vector may be all zeros, or contain one or more negative number" << std::endl;
        return 1;
    default:
        std::cout << "Succesful decomposition took: "<<retVal<<" iterations"<<std::endl;
        break;
    }
    //if there was success then write out the various important spectra
    //write out the starting point
    std::ostringstream namer;
    namer << argv[3] << "_init.b1d";
    writeDecompData(namer.str(), decompOrig);
    namer.str("");
    namer.clear();
    //write out the final decomposition
    namer << argv[3] << "_final.b1d";
    writeDecompData(namer.str(), decompProc);
    namer.str("");
    namer.clear();
    //calculate and write the sum spectrum
    DataVector<double>* sumSpectrum = calculateSumSpectrum(decompProc, *responseMatrix);
    namer << argv[3] << "_sum.b1d";
    writeSpectumData(namer.str(), *sumSpectrum);
    namer.str("");
    namer.clear();
    //calculate and write the residual spectrum
    DataVector<double>* residSpectrum = calculateResidSpectrum(*inputSpectrum, *sumSpectrum);
    namer << argv[3] << "_resid.b1d";
    writeSpectumData(namer.str(), *residSpectrum);
    
    delete residSpectrum;
    delete sumSpectrum;
    delete responseMatrix;
    delete inputSpectrum;
    return 0;
}

