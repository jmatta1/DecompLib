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
#include<iostream>
#include<string>
#include<sstream>
#include<tuple>
#include"DecompLib/Decomposition.h"
#include"MiscLib/UserIo.h"
#include"MiscLib/SpectrumIo.h"
#include"MiscLib/SpectrumCalculation.h"
#include"MiscLib/SimpleStructs.h"

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"Spectrum Decomposer"<<std::endl;
        std::cout<<"  A program to decompose an input spectrum using a response matrix."<<std::endl;
        std::cout<<"    The response matrix and input spectrum must be in the csv\n"
                 <<"    format produced by the programs in RootTh1ToCsv and RootTh2ToCsv"<<std::endl;
        std::cout<<"  The outputs will be in the same format and can be converted to root\n"
                 <<"    format with the program in Csv1dToRoot"<<std::endl;
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
        std::cout<<"    Spectrum Scaled By A Factor Of "<<startVal<<"\n"<<std::endl;
    }
    else
    {
        std::cout<<"    Constant Value Of "<<startVal<<"\n"<<std::endl;
    }
    //read the raw data
    DataVector<double>* inputSpectrum = nullptr;
    OneDMetaData* spectrumMeta = nullptr;
    std::tie(inputSpectrum, spectrumMeta) = readDataVector(std::string(argv[1]));
    RespMatrix<double>* responseMatrix = nullptr;
    TwoDMetaData* respMatMeta = nullptr;
    std::tie(responseMatrix, respMatMeta) = readResponseMatrix(std::string(argv[2]));
    //make the decomp vector
    DecompVector<double> decompProc(responseMatrix->getNumRespFuncs());
    //initialize the decomp vector as appropriate
    if(initValSetting == 0)
    {decompProc.initWithDataVector(*inputSpectrum, startVal, 0.0);}
    else
    {decompProc.initWithConstant(startVal);}
    //store a copy of the decomposition vector as the original values will be overwritten with the final values
    DecompVector<double> decompOrig(decompProc);
    
    //decompose the input spectrum
    std::cout<<"Starting Decomposition"<<std::endl;
    long long retVal = performDecomposition(*inputSpectrum, *responseMatrix,
                                            decompProc, minThresh, convCrit);
    //tell the user about success or return (since they already know about failure)
    if(retVal < 0) return 1;
    std::cout << "Succesful decomposition took: "<<retVal<<" iterations"<<std::endl;
    //if there was success then write out the various important spectra
    //create the decomp vector meta data
    OneDMetaData decompMeta;
    decompMeta.numBins = respMatMeta->numYbins;
    decompMeta.startBin = respMatMeta->startYbin;
    decompMeta.stopBin = respMatMeta->stopYbin;
    decompMeta.firstBinLoEdge = respMatMeta->firstYbinLoEdge;
    decompMeta.finalBinHiEdge = respMatMeta->finalYbinHiEdge;
    //write out the starting point
    std::ostringstream namer;
    namer << argv[3] << "_init.csv";
    std::string name = namer.str();
    std::cout<<"Writing the initial decomposition values to: "<<name<<std::endl;
    writeDecompData(name, decompOrig, decompMeta);
    namer.str("");
    namer.clear();
    //write out the final decomposition
    namer << argv[3] << "_final.csv";
    name = namer.str();
    std::cout<<"  Writing the final decomposition values to: "<<name<<std::endl;
    writeDecompData(name, decompProc, decompMeta);
    namer.str("");
    namer.clear();
    //calculate and write the sum spectrum
    DataVector<double>* sumSpectrum = calculateSumSpectrum(decompProc, *responseMatrix);
    namer << argv[3] << "_sum.csv";
    name = namer.str();
    std::cout<<"             Writing the fitted spectrum to: "<<name<<std::endl;
    writeSpectumData(name, *sumSpectrum, *spectrumMeta);
    namer.str("");
    namer.clear();
    //calculate and write the residual spectrum
    DataVector<double>* residSpectrum = calculateResidSpectrum(*inputSpectrum, *sumSpectrum);
    namer << argv[3] << "_resid.csv";
    name = namer.str();
    std::cout<<"      Writing the fit residuals spectrum to: "<<name<<std::endl;
    writeSpectumData(name, *residSpectrum, *spectrumMeta);
    
    delete residSpectrum;
    delete sumSpectrum;
    delete respMatMeta;
    delete responseMatrix;
    delete spectrumMeta;
    delete inputSpectrum;
    std::cout<<"Finished"<<std::endl;
    return 0;
}

