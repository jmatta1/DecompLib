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
#include<TH2.h>

//reads a csv spectrum into a TH1D and returns a pointer to that histogram and the associated file
TH2F* readCsvSpectrum(const std::string& fileName, const std::string& histName, float cutoff);
//reads lines from the input stream until it finds a line that does not have '#' at index 0
//if the next line is not a comment this is functionally identical to getline
bool readAndIgnoreCommentLines(std::istream& input, std::string& str);
//reads an axis information line and breaks it into the number of bins and the edges
std::tuple<int, double, double> parseAxisInfoLine(const std::string& line);
//reads a 2d data line and breaks it into the X bin number, Y bin number, and bin value
std::tuple<int, int, float> parse2dDataLine(const std::string& line);


int main(int argc, char* argv[])
{
    if(argc != 4 && argc != 5)
    {
        std::cout<<"A simple utility to write a 1D spectrum in csv format into a specified ROOT TH1"<<std::endl;
        std::cout<<"  Usage:\n    "<<argv[0]
                 <<" <inputCsvFile> <outputRootFile> <outputTh1Name> [minCutoff]\n\n"
                 <<"minCutoff is an optional parameter that specifies a value that\n"
                 <<"below which zero is entered into the histogram instead of the value\n"
                 <<"It defaults to 1.0e-10, and can be turned off entirely by passing\n"
                 <<"a zero or negative value"
                 <<std::endl;
        return 1;
    }
    float cutOff = 1.0e-10;
    if(argc == 5)
    {
        std::istringstream converter;
        converter.str(argv[4]);
        converter>>cutOff;
    }
    //now create the root file and histogram
    std::cout<<"  Creating / Opening ROOT File: "<<argv[2]<<std::endl;
    TFile* file = new TFile(argv[2], "UPDATE");
    TH2F* hist = readCsvSpectrum(argv[1], argv[3], cutOff);
    if(hist == nullptr)
    {
        std::cout<<"Input Csv file does not exist"<<std::endl;
        delete file;
        return 1;
    }
    hist->Write();
    file->Flush();
    std::cout<<"Finished"<<std::endl;
    delete hist;
    delete file;
    return 0;
}

TH2F* readCsvSpectrum(const std::string& fileName, const std::string& histName, float cutOff)
{
    //open the file and allocate the meta data system
    std::cout<<"            Opening Input File: "<<fileName<<std::endl;
    std::ifstream input;
    input.open(fileName.c_str());
    if(!(input.good()))
    {
        return nullptr;
    }
    std::string line;
    //the first non comment line of a 2d csv is X axis information
    readAndIgnoreCommentLines(input, line);
    int numXbins;
    double firstXbinLoEdge, finalXbinHiEdge;
    std::tie(numXbins, firstXbinLoEdge, finalXbinHiEdge) = parseAxisInfoLine(line);
    //the second non comment line of a 2d csv is Y axis information
    readAndIgnoreCommentLines(input, line);
    int numYbins;
    double firstYbinLoEdge, finalYbinHiEdge;
    std::tie(numYbins, firstYbinLoEdge, finalYbinHiEdge) = parseAxisInfoLine(line);
    //now create the histogram to these specifications
    std::cout<<"Creating and Filling Histogram: "<<histName<<std::endl;
    TH2F* hist = new TH2F(histName.c_str(), histName.c_str(), numXbins, firstXbinLoEdge, finalXbinHiEdge, numYbins, firstYbinLoEdge, finalYbinHiEdge);
    //now read all the bin locations and values
    bool lastLineGood = true;
    int xBinNum, yBinNum;
    float value;
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
        std::tie(xBinNum, yBinNum, value) = parse2dDataLine(line);
        if(value < cutOff)
        {
            hist->SetBinContent(xBinNum+1, yBinNum+1, 0.0);
        }
        else
        {
            hist->SetBinContent(xBinNum+1, yBinNum+1, value);
        }
    }
    //now make ROOT actually calculate the statistics for the histogram correctly (since I am not using fill)
    hist->ResetStats();
    //now return the 2D histogram
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

std::tuple<int, int, float> parse2dDataLine(const std::string& line)
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
    //extract the second value the y-axis bin number
    startIndex = (searchIndex + 1);
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    int yBin = 0;
    converter >> yBin;
    converter.clear();
    //extract the third value, the bin content
    startIndex = (searchIndex + 1);
    searchIndex = line.find_first_of(',', startIndex);
    converter.str(line.substr(startIndex, (searchIndex-startIndex)));
    float value = 0.0;
    converter >> value;
    return std::make_tuple(xBin, yBin, value);
}
