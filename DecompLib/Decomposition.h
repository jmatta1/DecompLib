/*!*****************************************************************************
********************************************************************************
**
** @file
** @brief Contains the template functions that perform the decomposition
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
#ifndef DECOMPLIB_DECOMPOSITION_H
#define DECOMPLIB_DECOMPOSITION_H
#include<iostream>
#include<utility>
#include"DataVector.h"
#include"DecompVector.h"
#include"RespMatrix.h"
#include"Helper.h"

/*!
@ingroup Calculation

@brief Performs a decomposition of data using respMatrix and the initial guess in paramSet, it then outputs the result in paramSet

@tparam ParamType The floating point type that is to be used for the calculations
 
@param data The DataVector with the spectrum to be decomposed
@param respMatrix The RespMatrix which holds the set of response functions to use to decompose the data spectrum
@param paramSet The DecompVector which contains the initial guess and will contain the final decomposition parameters
@param minThresh The value below which parameters are not tested for determining convergence defaults to 1.0e-6
@param convThresh The maximum fractional change between iterations allowed before parameters are considered unconverged defaults to 0.005
@param printBadSafety Print what failed in a safety check

@return The number of iterations, or negative values for errors
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

This function uses the expectation maximization algorithm in "J. Tain, D. Cano-Ott, Algorithms for the analysis of beta-decay total absorption spectra, NIMA 571 (3) (2007) 728-738"
The iterative formula presented in Equation 20 of that paper can be seen below (with the paper's indexing errors corrected).
@f[
f_{\mu}^{(s+1)} = \frac{1}{\sum\limits_{j}R_{\mu{}j}} \sum\limits_{i}\frac{f_{\mu{}}^{(s)} R_{\mu{}i}d_i}{\sum\limits_{\alpha}R_{\alpha{}i}f_{\alpha}^{(s)}}
@f]
Where @f$f_{\mu}^{(s+1)}@f$ contains the decomposition weights / spectrum for iteration @f$(s+1)@f$, @f$R_{\mu{}\nu{}}@f$ contains the response matrix, and @f$d_{i}@f$ contains the initial spectrum to be decomposed.

The formula above is slightly rearranged to yield the algorithm used in the function below.
*/
template<typename ParamType>
long long performDecomposition(DataVector<ParamType>& data, RespMatrix<ParamType>& respMatrix,
                               DecompVector<ParamType>& paramSet, ParamType minThresh = 1e-6,
                               ParamType convThresh = 0.005, bool printBadSafety=true)
{
    //before we allocate anything check to make sure the inputs are safe to procede
    long long testResult = testSafety(data, respMatrix, paramSet, minThresh, convThresh, printBadSafety);
    if(testResult != 0) return testResult;
    //now that all the safety checks are done that can be done, proceed with allocation and the like
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
    
    const ParamType* respMat = respMatrix.getMatrixPtr();
    // get the transpose of the response matrix
    const ParamType* respMatTr = respMatrix.getTransposePtr();
    // sum the rows of the response matrix and get a ptr to them
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
        hasConverged = testConvergence(oldVals, newVals, numRespFunc, minThresh, convThresh);
        //swap new values into old values and vice versa
        std::swap(oldVals, newVals);
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
