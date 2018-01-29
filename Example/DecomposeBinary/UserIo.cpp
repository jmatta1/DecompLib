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

