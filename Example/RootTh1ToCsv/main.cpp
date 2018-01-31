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
#include<TFile.h>
#include<TH1.h>
#include<iostream>
#include<fstream>

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"A simple utility to write a specified ROOT TH1 into a csv file"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputRootFile> <inputTH1Name> <outputFileName>"
                 <<std::endl;
        return 1;
    }
    std::cout<<"      From ROOT File: "<<argv[1]<<std::endl;
    TFile* file = new TFile(argv[1]);
    std::cout<<"Retrieving TH1 Named: "<<argv[2]<<std::endl;
    TH1* hist = (TH1*)file->Get(argv[2]);
    if(hist == nullptr)
    {
        std::cout<<"ERROR - Histogram does not exist in ROOT file"<<std::endl;
        return 1;
    }
    std::cout<<"Outputting Hist to Csv File: "<<argv[3]<<std::endl;
    int histSize = hist->GetNbinsX();
    std::ofstream out(argv[3]);
    double loBinEdge = hist->GetXaxis()->GetBinLowEdge(1);
    double hiBinEdge = hist->GetXaxis()->GetBinLowEdge(histSize + 1);
    out<<"#Lines starting with a '#' in the first column are ignored\n";
    out<<"#X-axis: nbins, first bin low edge, last bin high edge\n";
    out<<histSize<<", "<<loBinEdge<<", "<<hiBinEdge<<"\n";
    out<<"# bin number, value\n";
    for(int i=1; i<=histSize; ++i)
    {
        out<<(i-1)<<", "<<hist->GetBinContent(i)<<"\n";
    }
    out.close();
    delete hist;
    delete file;
    std::cout<<"Done"<<std::endl;
}
