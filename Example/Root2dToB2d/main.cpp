#include<TFile.h>
#include<TH2.h>
#include<iostream>
#include<fstream>
int getStartingRow();
int getStoppingRow();
int getStartingColumn();
int getStoppingColumn();
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
    int startY = getStartingRow();
    int stopY = getStoppingRow();
    int startX = getStartingColumn();
    int stopX = getStoppingColumn();
    int histSizeX = (stopX-startX+1);
    int histSizeY = (stopY-startY+1);
    char* outArray = new char[sizeof(int)*2+sizeof(double)*histSizeX*histSizeY];
    *(reinterpret_cast<int*>(outArray)) = histSizeX;
    int index = sizeof(int);
    *(reinterpret_cast<int*>(outArray+index)) = histSizeY;
    index += sizeof(int);
    for(int i=startY; i<=stopY; ++i)
    {
        for(int j=startX; j<=stopX; ++j)
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
int getStartingRow()
{
    std::cout<<"\nGive the starting row of the input histogram: "<<std::endl;
    int val = -1;
    std::cin>>val;
    while(val<0)
    {
        std::cout<<"  Negative values are not allowed"<<std::endl;
        std::cout<<"  Give the starting row of the output: "<<std::endl;
        std::cin>>val;
    }
    return val;
}
int getStoppingRow()
{
    std::cout<<"\nGive the stopping row of the input histogram: "<<std::endl;
    int val = -1;
    std::cin>>val;
    while(val<0)
    {
        std::cout<<"  Negative values are not allowed"<<std::endl;
        std::cout<<"  Give the stopping row of the input histogram: "<<std::endl;
        std::cin>>val;
    }
    return val;
}
int getStartingColumn()
{
    std::cout<<"\nGive the starting column of the input histogram: "<<std::endl;
    int val = -1;
    std::cin>>val;
    while(val<0)
    {
        std::cout<<"  Negative values are not allowed"<<std::endl;
        std::cout<<"  Give the starting row of the input histogram: "<<std::endl;
        std::cin>>val;
    }
    return val;
}
int getStoppingColumn()
{
    std::cout<<"\nGive the stopping column of the input histogram: "<<std::endl;
    int val = -1;
    std::cin>>val;
    while(val<0)
    {
        std::cout<<"  Negative values are not allowed"<<std::endl;
        std::cout<<"  Give the stopping row of the input histogram: "<<std::endl;
        std::cin>>val;
    }
    return val;
}
