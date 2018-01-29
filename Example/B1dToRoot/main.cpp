#include<TFile.h>
#include<TH1.h>
#include<iostream>
#include<fstream>

double getLoBound();
double getHiBound();

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"A simple utility to write a specified binary file into a ROOT TH1"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputBinaryFile> <outputRootFile> <outputTH1Name>"
                 <<std::endl;
        return 1;
    }
    std::cout<<"  From Binary File: "<<argv[1]<<std::endl;
    std::ifstream out;
    out.open(argv[1], std::ios_base::binary);
    int histSize;
    out.read(reinterpret_cast<char*>(&histSize), sizeof(int));
    int byteSize = (sizeof(double)*histSize);
    char* inArray = new char[byteSize];
    int index = 0;
    out.read(inArray, byteSize);
    out.close();
    std::cout<<"     To ROOT File: "<<argv[2]<<std::endl;
    TFile* file = new TFile(argv[2], "UPDATE");
    std::cout<<"Writing TH1 Named: "<<argv[3]<<std::endl;
    double low = getLoBound();
    double high = getHiBound();
    TH1D* hist = new TH1D(argv[3], argv[3], histSize, low, high);
    for(int i=1; i<=histSize; ++i)
    {
        hist->SetBinContent(i, *(reinterpret_cast<double*>(inArray+index)));
        index += sizeof(double);
    }
    hist->Write();
    file->Flush();
    delete[] inArray;
    delete hist;
    delete file;
    std::cout<<"Done"<<std::endl;
}

double getLoBound()
{
    std::cout<<"What is the low edge energy of the first bin in the spectrum?"<<std::endl;
    double en = -1.0;
    std::cin>>en;
    while(en < 0.0)
    {
        std::cout<<"Value must be positive"<<std::endl;
        std::cout<<"What is the low edge energy of the first bin in the spectrum?"<<std::endl;
        std::cin>>en;
    }
    return en;
}

double getHiBound()
{
    std::cout<<"What is the high edge energy of the last bin in the spectrum?"<<std::endl;
    double en = -1.0;
    std::cin>>en;
    while(en < 0.0)
    {
        std::cout<<"Value must be positive"<<std::endl;
        std::cout<<"What is the high edge energy of the last bin in the spectrum?"<<std::endl;
        std::cin>>en;
    }
    return en;
}
