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
@param convThresh maximum fractional change between iterations allowed before parameters are considered unconverged defaults to 0.005

@return The number of iterations, or negative values for errors
Error Value | Description
:----------:|------
-1          | A row or column of the response matrix contains all zeros
-2          | One of the initial decomposition parameters is less than or equal to zero
-3          | The data vector is either all zeros or contains one or more negative numbers
-4          | The sum of one of the rows of the response matrix was zero
-5          | The sum of the element-wise product of a response matrix column and the decomposition parameters was zero

This function uses the expectation maximization algorithm in "J. Tain, D. Cano-Ott, Algorithms for the analysis of beta-decay total absorption spectra, NIMA 571 (3) (2007) 728-738"
The iterative formula presented in Equation 20 of that paper can be seen below (with the paper's indexing errors corrected).
@f[
f_{\mu}^{(s+1)} = \frac{1}{\sum\limits_{j}R_{\mu{}j}} \sum\limits_{i}\frac{f_{\mu{}}^{(s)} R_{\mu{}i}d_i}{\sum\limits_{\alpha}R_{\alpha{}i}f_{\alpha}^{(s)}}
@f]
Where @f$f_{\mu}^{(s+1)}@f$ contains the decomposition weights / spectrum for iteration @f$(s+1)@f$, @f$R_{\mu{}\nu{}}@f$ contains the response matrix, and @f$d_{i}@f$ contains the initial spectrum to be decomposed.

The formula above is slightly rearranged to yield the algorithm used in the function below.
 */
template<typename ParamType>
long long performDecomposition(DataVector<ParamType>& data, RespMatrix<ParamType>& respMatrix, DecompVector<ParamType>& paramSet,
                               ParamType minThresh = 1e-6, ParamType convThresh = 0.005)
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
        return -1ULL;// a row or column of the response matrix contains all zeros
    }
    if(!paramSet.isSafe())
    {
        return -2ULL;//initial guess may have one or more values that are less than or equal to zero
    }
    if(!data.isSafe())
    {
        return -3ULL;//data vector may be all zeros, or contain one or more negative number
    }
    
    const ParamType* respMat = respMatrix.getMatrixPtr();
    // get the transpose of the response matrix
    const ParamType* respMatTr = respMatrix.getTransposePtr();
    // sum the rows of the response matrix and get a ptr to them
    respMatrix.calculateSummedRows();
    const ParamType* respSum = respMatrix.getSummedRowPtr();
    // test the safety of the summed response rows
    for(int j=0; j<numRespFunc; ++j)
    {
        if(respSum[j] == 0.0)
        {
            return -4ULL;//Response function row sums has a zero, which should not be possible with the other checks
        }
    }
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
            if(denom == 0.0) return -5ULL;
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
