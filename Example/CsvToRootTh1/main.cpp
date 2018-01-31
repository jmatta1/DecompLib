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
#include<fstream>
#include<string>
#include<sstream>
#include<tuple>
#include<TFile.h>
#include<TH1.h>

//reads a csv spectrum into a TH1D and returns a pointer to that histogram and the associated file
TH1D* readCsvSpectrum(const std::string& fileName, const std::string& histName);
//reads lines from the input stream until it finds a line that does not have '#' at index 0
//if the next line is not a comment this is functionally identical to getline
bool readAndIgnoreCommentLines(std::istream& input, std::string& str);
//reads an axis information line and breaks it into the number of bins and the edges
std::tuple<int, double, double> parseAxisInfoLine(const std::string& line);
//reads a 1d data line and breaks it into the X bin number, and value
std::tuple<int, double> parse1dDataLine(const std::string& line);


int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout<<"A simple utility to write a 1D spectrum in csv format into a specified ROOT TH1"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputCsvFile> <outputRootFile> <outputTh1Name>"
                 <<std::endl;
        return 1;
    }
    //now create the root file and histogram
    std::cout<<"            Creating ROOT File: "<<argv[2]<<std::endl;
    TFile* file = new TFile(argv[2], "UPDATE");
    TH1D* hist = readCsvSpectrum(argv[1], argv[3]);
    hist->Write();
    file->Flush();
    std::cout<<"Finished"<<std::endl;
    delete hist;
    delete file;
}

TH1D* readCsvSpectrum(const std::string& fileName, const std::string& histName)
{
    //open the file and allocate the meta data system
    std::cout<<"            Opening Input File: "<<fileName<<std::endl;
    std::ifstream input;
    input.open(fileName.c_str());
    std::string line;
    //the first non comment line of a 1d csv is X axis information
    readAndIgnoreCommentLines(input, line);
    int numBins;
    double firstBinLoEdge, finalBinHiEdge;
    std::tie(numBins, firstBinLoEdge, finalBinHiEdge) = parseAxisInfoLine(line);
    //now create the histogram to these specifications
    std::cout<<"Creating and Filling Histogram: "<<histName<<std::endl;
    TH1D* hist = new TH1D(histName.c_str(), histName.c_str(), numBins, firstBinLoEdge, finalBinHiEdge);
    //now read all the bin locations and values, only importing into the data vector what is needed
    bool lastLineGood = true;
    int binNum;
    double value;
    while(lastLineGood)
    {
        lastLineGood = readAndIgnoreCommentLines(input, line);
        if(line.size() <= 3) // skip lines that are too short to contain anything
        {
            continue;
        }
        else if(!lastLineGood)
        {
            break;
        }
        std::tie(binNum, value) = parse1dDataLine(line);
        hist->SetBinContent(binNum+1, value);
    }
    //now return the matrix and its meta data
    return hist;
}

bool readAndIgnoreCommentLines(std::istream& input, std::string& str)
{
    do
    {
        std::getline(input, str);
    }
    while(str[0] == '#');
    return !(input.fail());
}

std::tuple<int, double, double> parseAxisInfoLine(const std::string& line)
{
    //set up the converter string stream and the searching indices
    std::istringstream converter;
    int startIndex = 0;
    int searchIndex = 0;
    //extract the first value the number of bins in the axis
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    int numBins = 0;
    converter >> numBins;
    converter.clear();
    //extract the second value, the lo edge of the first bin
    startIndex = (searchIndex + 1);
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    double loEdge = 0.0;
    converter >> loEdge;
    converter.clear();
    //extract the third value, the hi edge of the last bin
    startIndex = (searchIndex + 1);
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    double hiEdge = 0.0;
    converter >> hiEdge;
    return std::make_tuple(numBins, loEdge, hiEdge);
}

std::tuple<int, double> parse1dDataLine(const std::string& line)
{
    //set up the converter string stream and the searching indices
    std::istringstream converter;
    int startIndex = 0;
    int searchIndex = 0;
    //extract the first value the x-axis bin number
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    int xBin = 0;
    converter >> xBin;
    converter.clear();
    //extract the third value, the bin content
    startIndex = (searchIndex + 1);
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    double value = 0.0;
    converter >> value;
    return std::make_tuple(xBin, value);
}
