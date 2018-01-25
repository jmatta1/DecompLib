#include<TFile.h>
#include<TH2.h>
#include<iostream>
#include<fstream>

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"A simple utility to write a specified ROOT TH2 into a binary file"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputRootFile> <inputTH2Name> <outputFileName>"
                 <<std::endl;
        return 1;
    }
    std::cout<<"      From ROOT File: "<<argv[1]<<std::endl;
    TFile* file = new TFile(argv[1]);
    std::cout<<"Retrieving TH2 Named: "<<argv[2]<<std::endl;
    TH1* hist = (TH2*)file->Get(argv[2]);
    std::cout<<"Outputting 2D Hist to Binary File: "<<argv[3]<<std::endl;
    int histSizeX = hist->GetNbinsX();
    int histSizeY = hist->GetNbinsY();
    char* outArray = new char[sizeof(int)*2+sizeof(double)*histSizeX*histSizeY];
    *(reinterpret_cast<int*>(outArray)) = histSizeX;
    int index = sizeof(int);
    *(reinterpret_cast<int*>(outArray+index)) = histSizeY;
    for(int i=1; i<=histSizeY; ++i)
    {
        for(int j=1; j<=histSizeX; ++j)
        {
            *(reinterpret_cast<double*>(outArray+index)) = hist->GetBinContent(j, i);
            index += sizeof(double);
        }
    }
    std::ofstream out;
    out.open(argv[3],std::ios_base::binary);
    out.write(outArray, index);
    out.close();
    delete[] outArray;
    delete hist;
    delete file;
    std::cout<<"Done"<<std::endl;
}
