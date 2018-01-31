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
#include<TH2.h>
#include<iostream>
#include<fstream>

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"A simple utility to write a specified ROOT TH2 into a csv file"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputRootFile> <inputTH2Name> <outputFileName>"
                 <<std::endl;
        return 1;
    }
    std::cout<<"      From ROOT File: "<<argv[1]<<std::endl;
    TFile* file = new TFile(argv[1]);
    std::cout<<"Retrieving TH2 Named: "<<argv[2]<<std::endl;
    TH2* hist = (TH2*)file->Get(argv[2]);
    if(hist == nullptr)
    {
        std::cout<<"ERROR - Histogram does not exist in ROOT file"<<std::endl;
        return 1;
    }
    std::cout<<"Outputting 2D Hist to csv File: "<<argv[3]<<std::endl;
    int histSizeX = hist->GetXaxis()->GetNbins();
    double loEdgeX = hist->GetXaxis()->GetBinLowEdge(1);
    double hiEdgeX = hist->GetXaxis()->GetBinLowEdge(histSizeX+1);
    int histSizeY = hist->GetYaxis()->GetNbins();
    double loEdgeY = hist->GetYaxis()->GetBinLowEdge(1);
    double hiEdgeY = hist->GetYaxis()->GetBinLowEdge(histSizeY+1);
    std::ofstream out(argv[3]);
    out<<"#Lines starting with a '#' in the first column are ignored\n";
    out<<"#X-axis: nbins, first bin low edge, last bin high edge\n";
    out<<histSizeX<<", "<<loEdgeX<<", "<<hiEdgeX<<"\n";
    out<<"#Y-axis: nbins, first bin low edge, last bin high edge\n";
    out<<histSizeY<<", "<<loEdgeY<<", "<<hiEdgeY<<"\n";
    out<<"#x-axis bin number, y-axis bin number, value\n";
    for(int i=1; i<=histSizeY; ++i)
    {
        for(int j=1; j<=histSizeX; ++j)
        {
            out<<(j-1)<<", "<<(i-1)<<", "<<hist->GetBinContent(j, i)<<"\n";
        }
    }
    out.close();
    delete hist;
    delete file;
    std::cout<<"Done"<<std::endl;
}
