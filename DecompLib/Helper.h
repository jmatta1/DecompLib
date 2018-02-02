/*!*****************************************************************************
********************************************************************************
**
** @file
** @brief Contains the template functions that perform small tasks for the decomposition
**
** @copyright Copyright (C) 2018 James Till Matta
**
** @author James Till Matta
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** 
********************************************************************************
*******************************************************************************/
#ifndef DECOMPLIB_HELPER_H
#define DECOMPLIB_HELPER_H
/*!
 * @ingroup Calculation
 * @brief Tests if two parameter sets vary more than convThresh
 * @tparam ParamType The floating point type contained in the arrays of values to check for convergence
 * @param oldVals The array of parameters at the beginning of the current iteration
 * @param newVals The array of parameters at the end of the current iteration
 * @param numRespFuncs The number of response functions for the decomposition / the length of oldVals and newVals
 * @param minThresh The value that causes a parameter to be ignored if both the
 * old and new val sets have a parameter whose value is below minThresh it is not
 * considered for determining convergence
 * @param convThresh The maximum fractional change allowed before a pair of
 * parameters are considered un-converged
 *
 */
template<typename ParamType>
bool testConvergence(ParamType* oldVals, ParamType* newVals, int numRespFuncs, ParamType minThresh, ParamType convThresh)
{
    for(int i=0; i<numRespFuncs; ++i)
    {
        if(newVals[i] < minThresh && oldVals[i] < minThresh)
        {
            continue;
        }
        ParamType frac = ((newVals[i]-oldVals[i])/((oldVals[i]+newVals[i])/2.0));
        if (frac < 0.0) frac = -frac;
        if(frac > convThresh)
        {
            return false;
        }
    }
    return true;
}

/*!
@ingroup Calculation

@brief Tests the inputs that would go into performDecomposition for safety

@tparam ParamType The floating point type that is to be used for the calculations
 
@param data The DataVector with the spectrum to be decomposed
@param respMatrix The RespMatrix which holds the set of response functions to use to decompose the data spectrum
@param paramSet The DecompVector which contains the initial guess and will contain the final decomposition parameters
@param minThresh The value below which parameters are not tested for determining convergence defaults to 1.0e-6
@param convThresh The maximum fractional change between iterations allowed before parameters are considered unconverged defaults to 0.005
@param printBadSafety Print what failed in a safety check

@return Negative values for errors, zero otherwise
Error Value | Description
:----------:|------
-1          | A row or column of the response matrix contains all zeros
-2          | One of the initial decomposition parameters is less than or equal to zero
-3          | The data vector is either all zeros or contains one or more negative numbers
-4          | The number of bins in the input spectrum is not the same as the number of bins in a response function
-5          | The number of response functions is not the same as the number of bins in the DecompVector
-6          | The minimum value to be tested for convergence is less than or equal to zero
-7          | The maximum change allowed to be considered converged is less than or equal to zero
-8          | The number of bins in the input spectrum does not exceed the number of response functions
*/
template<typename ParamType>
int testSafety(DataVector<ParamType>& data, RespMatrix<ParamType>& respMatrix,
               DecompVector<ParamType>& paramSet, ParamType minThresh,
               ParamType convThresh, bool printBadSafety=true)
{
    if(!respMatrix.isSafe())
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"A row or column of the response matrix contains all zeros."<<std::endl;
        }
        return -1;
    }
    if(!paramSet.isSafe())
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"Initial guess may have one or more values that are less than or equal to zero."<<std::endl;
        }
        return -2;
    }
    if(!data.isSafe())
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"Input spectrum may be all zeros, or contain one or more negative number."<<std::endl;
        }
        return -3;
    }
    if(data.getLength() != respMatrix.getRespFuncsLens())
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"Number of bins in input spectrum does not match number of bins per response function"<<std::endl;
        }
        return -4;
    }
    if(paramSet.getLength() != respMatrix.getNumRespFuncs())
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"Number of response functions in the matrix does not match the number of bins in the DecompVector"<<std::endl;
        }
        return -5;
    }
    if(!(minThresh > 0.0))
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"Minimum value to be tested for convergence is less than or equal to zero"<<std::endl;
        }
        return -6;
    }
    if(!(convThresh > 0.0))
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"Fractional change for convergence is less than or equal to zero"<<std::endl;
        }
        return -7;
    }
    if(data.getLength() <= paramSet.getLength())
    {
        if(printBadSafety)
        {
            std::cout<<"ERROR - Decomposition not performed"<<std::endl;
            std::cout<<"There must be at least 1 more data bin than there are response functions"<<std::endl;
        }
        return -8;
    }
    
    return 0;
}

#endif //DECOMPLIB_HELPER_H
