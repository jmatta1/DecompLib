#include"SpectrumIo.h"
#include<iostream>
#include<sstream>
#include"SimpleStructs.h"
#include"UserIo.h"

std::tuple<RespMatrix<double>*, TwoDMetaData*> readResponseMatrix(const std::string& fileName)
{
    //open the file and allocate the meta data system
    std::ifstream input;
    input.open(fileName.c_str());
    TwoDMetaData* meta = new TwoDMetaData();
    std::string line;
    //the first non comment line of a 2d csv is X axis information
    Detail::readAndIgnoreCommentLines(input, line);
    std::tie(meta->numXbins, meta->firstXbinLoEdge, meta->finalXbinHiEdge) = Detail::parseAxisInfoLine(line);
    //the second non comment line of a 2d csv is Y axis information
    Detail::readAndIgnoreCommentLines(input, line);
    std::tie(meta->numYbins, meta->firstYbinLoEdge, meta->finalYbinHiEdge) = Detail::parseAxisInfoLine(line);
    //all subsequent non comment lines of a 2d csv are bin locations and values so now ask the user what bins to use
    get2dAxisBounds(*meta);
    //now create the response matrix
    int numFuncs = (meta->stopYbin - meta->startYbin + 1);
    int numRespBins = (meta->stopXbin - meta->startXbin + 1);
    RespMatrix<double>* rMat = new RespMatrix<double>(numFuncs, numRespBins);
    //now read all the bin locations and values, only importing into the resp matrix what is needed
    bool lastLineGood = true;
    int respBinNum;
    int funcNum;
    double value;
    while(lastLineGood)
    {
        lastLineGood = Detail::readAndIgnoreCommentLines(input, line);
        if(!lastLineGood && (line.size() > 6))
        {
            lastLineGood = false;
            break;
        }
        std::tie(respBinNum, funcNum, value) = Detail::parse2dDataLine(line);
        if((funcNum <= meta->stopYbin) && (funcNum >= meta->startYbin) &&
           (respBinNum <= meta->stopXbin) && (respBinNum >= meta->startXbin))
        {
            rMat->setElement((funcNum - meta->startYbin), (respBinNum - meta->startXbin), value);
        }
    }
    //now return the matrix and its meta data
    return std::make_tuple(rMat, meta);
}

std::tuple<DataVector<double>*, OneDMetaData*> readDataVector(const std::string& fileName)
{
    //open the file and allocate the meta data system
    std::ifstream input;
    input.open(fileName.c_str());
    OneDMetaData* meta = new OneDMetaData();
    std::string line;
    //the first non comment line of a 1d csv is X axis information
    Detail::readAndIgnoreCommentLines(input, line);
    std::tie(meta->numBins, meta->firstBinLoEdge, meta->finalBinHiEdge) = Detail::parseAxisInfoLine(line);
    //all subsequent non comment lines of a 1d csv are bin locations and values so now ask the user what bins to use
    get1dAxisBounds(*meta);
    //now create the data vector
    int numBins = (meta->stopBin - meta->startBin + 1);
    DataVector<double>* data = new DataVector<double>(numBins);
    //now read all the bin locations and values, only importing into the data vector what is needed
    bool lastLineGood = true;
    int binNum;
    double value;
    while(lastLineGood)
    {
        lastLineGood = Detail::readAndIgnoreCommentLines(input, line);
        if(!lastLineGood && (line.size() > 3))
        {
            lastLineGood = false;
            break;
        }
        std::tie(binNum, value) = Detail::parse1dDataLine(line);
        if((binNum <= meta->stopBin) && (binNum >= meta->startBin))
        {
            data->setElement(binNum, value);
        }
    }
    //now return the matrix and its meta data
    return std::make_tuple(data, meta);
}

void writeDecompData(const std::string& fileName, const DecompVector<double>& dv, const OneDMetaData& meta)
{
    //create the fstream and open it
    std::ofstream out;
    out.open(fileName.c_str());
    //write the normal comments
    out<<"#Lines starting with a '#' in the first column are ignored\n";
    out<<"#X-axis: nbins, first bin low edge, last bin high edge\n";
    //write the axis parameters
    out<<meta.numBins<<", "<<meta.firstBinLoEdge<<", "<<meta.finalBinHiEdge<<"\n";
    //write the data
    out<<"# bin number, value\n";
    int numBins = dv.getLength();
    for(int i=0; i<numBins; ++i)
    {
        out<<(i+meta.startBin)<<", "<<dv.getElement(i)<<"\n";
    }
    //clean up
    out.close();
}

void writeSpectumData(const std::string& fileName, const DataVector<double>& dv, const OneDMetaData& meta)
{
    //create the fstream and open it
    std::ofstream out;
    out.open(fileName.c_str());
    //write the normal comments
    out<<"#Lines starting with a '#' in the first column are ignored\n";
    out<<"#X-axis: nbins, first bin low edge, last bin high edge\n";
    //write the axis parameters
    out<<meta.numBins<<", "<<meta.firstBinLoEdge<<", "<<meta.finalBinHiEdge<<"\n";
    //write the data
    out<<"# bin number, value\n";
    int numBins = dv.getLength();
    for(int i=0; i<numBins; ++i)
    {
        out<<(i+meta.startBin)<<", "<<dv.getElement(i)<<"\n";
    }
    //clean up
    out.close();
}


namespace Detail
{

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

std::tuple<int, int, double> parse2dDataLine(const std::string& line)
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
    double value = 0.0;
    converter >> value;
    return std::make_tuple(xBin, yBin, value);
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

}
