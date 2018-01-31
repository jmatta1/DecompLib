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
#ifndef DECOMPLIB_DECOMPOSITION_H
#define DECOMPLIB_DECOMPOSITION_H
#include"DataVector.h"
#include"DecompVector.h"
#include"RespMatrix.h"

/*!
 * \brief checkConvergence tests if two parameter sets vary more than convThresh
 * \param oldVals The array of parameters at the beginning of the current iteration
 * \param newVals The array of parameters at the end of the current iteration
 * \param minThresh The value that causes a parameter to be ignored if both the
 * old and new val sets have a parameter whose value is below minThresh it is not
 * considered for determining convergence
 * \param convThresh The maximum fractional change allowed before a pair of
 * parameters are considered un-converged
 */
template<typename ParamType>
bool checkConvergence(ParamType* oldVals, ParamType* newVals, int numRespFuncs,
                      ParamType minThresh, ParamType convThresh)
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
 * \brief Performs a decomposition of data using respMatrix and the initial guess in paramSet, it then outputs the result in paramSet
 * \param data A vector with the spectrum to be decomposed
 * \param respMatrix The response matrix to use to decompose the data spectrum
 * \param paramSet The DecompVector which contains the initial guess and will contain the final decomposition parameters
 * \param minThresh The value below which parameters are not tested for determining convergence defaults to 1.0e-6
 * \param convThresh maximum fractional change between iterations allowed before parameters are considered unconverged defaults to 0.005
 * \return The number of iterations, or negative values for errors, -1 if a row
 *  or column of the response matrix contains all zeros, -2 if the initial parameter
 *  set has one or more values that are less than or equal to zero, -3 if the
 *  data vector contains one or more negative numbers or it may all be zeros, and
 *  -4 for the case that a value in a denominator, from a product of response
 *  matrix column and decomposition vector
 *
 * This function uses the algorithm in "J. Tain, D. Cano-Ott, Algorithms for the
 * analysis of beta-decay total absorption spectra, NIMA 571 (3) (2007) 728-738"
 */
template<typename ParamType>
long long performDecomposition(DataVector<ParamType>& data, RespMatrix<ParamType>& respMatrix,
                               DecompVector<ParamType>& paramSet, ParamType minThresh = 1.0e-6,
                               ParamType convThresh = 0.005)
{
    // generate the two sets of decomp values that get swapped as iteration occurs
    int numRespFunc = respMatrix.getNumRespFuncs();
    int numDataBins = respMatrix.getRespFuncsLens();
    ParamType* oldVals = new ParamType[numRespFunc];
    ParamType* newVals = new ParamType[numRespFunc];
    for(int i=0; i<numRespFunc; ++i)
    {
        newVals[i] = 1.0e-32;
        oldVals[i] = paramSet.getElement(i);
    }
    //check to make sure the inputs are safe
    if(!respMatrix.isSafe())
    {
        return -1;// a row or column of the response matrix contains all zeros
    }
    if(!paramSet.isSafe())
    {
        return -2;//initial guess may have one or more values that are less than or equal to zero
    }
    if(!data.isSafe())
    {
        return -3;//data vector may be all zeros, or contain one or more negative number
    }
    
    const ParamType* respMat = respMatrix.getMatrixPtr();
    // get the transpose of the response matrix
    const ParamType* respMatTr = respMatrix.getTransposePtr();
    // sum the columns of the response matrix and get a ptr to them
    respMatrix.calculateSummedRows();
    const ParamType* respSum = respMatrix.getSummedRowPtr();
    // create the array of multipliers
    ParamType* mults = new ParamType[numDataBins];
    // create array of the product of mults and the resp mat
    ParamType* products = new ParamType[numRespFunc];
    
    // create the convergence variable
    bool hasConverged = false;
    // loop until convergence
    long long i=0;
    for(; !hasConverged; ++i)
    {
        //calculate multipliers
        for(int j=0; j<numDataBins; ++j)
        {
            ParamType denom = 0.0;
            int offset = j*numRespFunc;
            for(int k=0; k<numRespFunc; ++k)
            {
                denom += oldVals[k]*respMatTr[offset+k];
            }
            if(denom == 0.0) return -4ULL;
            mults[j] = data.getElement(j)/denom;
        }
        //calculate vector resp function multipliers
        for(int j=0; j<numRespFunc; ++j)
        {
            products[j] = 0.0;
            int offset = j*numDataBins;
            for(int k=0; k<numDataBins; ++k)
            {
                products[j] += respMat[offset+k]*mults[k];
            }
        }
        //calculate the new values
        for(int j=0; j<numRespFunc; ++j)
        {
            newVals[j] = (products[j]*oldVals[j])/respSum[j];
        }
        //check convergence
        hasConverged = checkConvergence(oldVals, newVals, numRespFunc, minThresh, convThresh);
        //swap new values into old values and vice versa
        ParamType* temp = oldVals;
        oldVals = newVals;
        newVals = temp;
    }
    for(int i=0; i<numRespFunc; ++i)
    {
        paramSet.setElement(i, oldVals[i]);
    }
    delete[] oldVals;
    delete[] newVals;
    delete[] mults;
    delete[] products;
    return i;
}

#endif //DECOMPLIB_DECOMPOSITION_H
