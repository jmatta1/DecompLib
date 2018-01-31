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
#include"UserIo.h"
#include<iostream>

int getStartingValType()
{
    int initValSetting = 0;
    bool validAnswer=false;
    while(!validAnswer)
    {
        std::cout<<"Intitial Value Setting?"<<std::endl;
        std::cout<<"  0 - Scaled Spectrum\n  1 - Constant"<<std::endl;
        std::cin>>initValSetting;
        if((initValSetting > -1) && (initValSetting < 2))
        {
            validAnswer = true;
        }
        else
        {
            std::cout<<"Invalid Choice, Try Again"<<std::endl;
        }
    }
    return initValSetting;
}

double getScalingFactor()
{
    double scaleFactor = 0.0;
    bool validAnswer=false;
    while(!validAnswer)
    {
        
        std::cout<<"Spectrum Scaling Factor?"<<std::endl;
        std::cin>>scaleFactor;
        if(scaleFactor > 0.0)
        {
            validAnswer = true;
        }
        else
        {
            std::cout<<"The scaling factor must be greater than zero. Try Again"<<std::endl;
        }
    }
    return scaleFactor;
}

double getStartingConstant()
{
    double constantVal = 0.0;
    bool validAnswer=false;
    while(!validAnswer)
    {
        
        std::cout<<"Constant Value?"<<std::endl;
        std::cin>>constantVal;
        if(constantVal > 0.0)
        {
            validAnswer = true;
        }
        else
        {
            std::cout<<"The constant must be greater than zero. Try Again"<<std::endl;
        }
    }
    return constantVal;
}

double getMinThreshold()
{
    double thresh = 0.0;
    bool validAnswer=false;
    while(!validAnswer)
    {
        
        std::cout<<"Convergence Consideration Threshold?"<<std::endl;
        std::cin>>thresh;
        if(thresh > 0.0)
        {
            validAnswer = true;
        }
        else
        {
            std::cout<<"The threshold must be greater than zero. Try Again"<<std::endl;
        }
    }
    return thresh;
}

double getConvergence()
{
    double conv = 0.0;
    bool validAnswer=false;
    while(!validAnswer)
    {
        
        std::cout<<"Convergence Maximum Fractional Change?"<<std::endl;
        std::cin>>conv;
        if(conv > 0.0)
        {
            validAnswer = true;
        }
        else
        {
            std::cout<<"The maximum fractional change for convergence must be greater than zero. Try Again"<<std::endl;
        }
    }
    return conv;
}

void get2dAxisBounds(TwoDMetaData& meta)
{
    std::cout<<"The X-axis of the response matrix has "<<meta.numXbins<<" bins and\n";
    std::cout<<"    the low edge of the first bin is: "<<meta.firstXbinLoEdge<<"\n"
             <<"    and the high edge of the last bin is: "<<meta.finalXbinHiEdge<<std::endl;
    double perBin = (meta.finalXbinHiEdge - meta.firstXbinLoEdge)/static_cast<double>(meta.numXbins);
    meta.startXbin = Detail::getAxisLoBound("X", meta.numXbins, meta.firstXbinLoEdge, perBin);
    meta.stopXbin  = Detail::getAxisHiBound("X", meta.numXbins, meta.finalXbinHiEdge, perBin);
    std::cout<<"The Y-axis of the response matrix has "<<meta.numYbins<<" bins\n";
    std::cout<<"    the low edge of the first bin is: "<<meta.firstYbinLoEdge<<"\n"
             <<"    and the high edge of the last bin is: "<<meta.finalYbinHiEdge<<std::endl;
    perBin = (meta.finalYbinHiEdge - meta.firstYbinLoEdge)/static_cast<double>(meta.numYbins);
    meta.startYbin = Detail::getAxisLoBound("Y", meta.numYbins, meta.firstYbinLoEdge, perBin);
    meta.stopYbin  = Detail::getAxisHiBound("Y", meta.numYbins, meta.firstYbinLoEdge, perBin);
}

void get1dAxisBounds(OneDMetaData& meta)
{
    std::cout<<"The X-axis of the spectrum has "<<meta.numBins<<" bins and\n";
    std::cout<<"    the low edge of the first bin is: "<<meta.firstBinLoEdge<<"\n"
             <<"    and the high edge of the last bin is: "<<meta.finalBinHiEdge<<std::endl;
    double perBin = (meta.finalBinHiEdge - meta.firstBinLoEdge)/static_cast<double>(meta.numBins);
    meta.startBin = Detail::getAxisLoBound("X", meta.numBins, meta.firstBinLoEdge, perBin);
    meta.stopBin  = Detail::getAxisHiBound("X", meta.numBins, meta.finalBinHiEdge, perBin);
}

namespace Detail
{

int getAxisLoBound(const std::string& axis, int numBins, double loEdge, double perBin)
{
    bool goodValue = false;
    int loBin = -1;
    char ans = 't';
    do
    {
        std::cout<<"Please give the first bin number of the "<<axis<<"-axis to use."<<std::endl;
        std::cin>>loBin;
        if((loBin < 0) || (loBin >= numBins))
        {
            std::cout<<"Error - The value must be an integer in the range [0,"<<numBins<<")"<<std::endl;
            continue;
        }
        std::cout<<"This sets the first used bin of the "<<axis<<"-axis to have:\n  Low Edge:"
                 <<(loEdge + (loBin*perBin))<<"\n  Center:"<<(loEdge + (perBin/2.0) + (loBin*perBin))<<std::endl;
        std::cout<<"Is this satisfactory (Y/N)?"<<std::endl;
        std::cin>>ans;
        if((ans == 'Y') || (ans =='y'))
        {
            goodValue = true;
        }
    }
    while(!goodValue);
    return loBin;
}

int getAxisHiBound(const std::string& axis, int numBins, double hiEdge, double perBin)
{
    bool goodValue = false;
    int hiBin = -1;
    char ans = 't';
    do
    {
        std::cout<<"Please give the last bin number of the "<<axis<<"-axis to use."<<std::endl;
        std::cin>>hiBin;
        if((hiBin < 0) || (hiBin >= numBins))
        {
            std::cout<<"Error - The value must be an integer in the range [0,"<<numBins<<")"<<std::endl;
            continue;
        }
        std::cout<<"This sets the last used bin of the "<<axis<<"-axis to have:\n  High Edge:"
                 <<(hiEdge - ((numBins - 1 - hiBin)*perBin))<<"\n  Center:"<<(hiEdge - (perBin/2.0) - ((numBins - 1 - hiBin)*perBin))<<std::endl;
        std::cout<<"Is this satisfactory (Y/N)?"<<std::endl;
        std::cin>>ans;
        if((ans == 'Y') || (ans =='y'))
        {
            goodValue = true;
        }
    }
    while(!goodValue);
    return hiBin;
}

}
