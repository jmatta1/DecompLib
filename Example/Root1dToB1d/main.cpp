#include<TFile.h>
#include<TH1.h>
#include<iostream>
#include<fstream>

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"A simple utility to write a specified ROOT TH1 into a binary file"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputRootFile> <inputTH1Name> <outputFileName>"
                 <<std::endl;
        return 1;
    }
    std::cout<<"      From ROOT File: "<<argv[1]<<std::endl;
    TFile* file = new TFile(argv[1]);
    std::cout<<"Retrieving TH1 Named: "<<argv[2]<<std::endl;
    TH1* hist = (TH1*)file->Get(argv[2]);
    std::cout<<"Outputting Hist to Binary File: "<<argv[3]<<std::endl;
    int histSize = hist->GetNbinsX();
    char* outArray = new char[sizeof(int)+sizeof(double)*histSize];
    *(reinterpret_cast<int*>(outArray)) = histSize;
    int index = sizeof(int);
    for(int i=1; i<=histSize; ++i)
    {
        *(reinterpret_cast<double*>(outArray+index)) = hist->GetBinContent(i);
        index += sizeof(double);
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
