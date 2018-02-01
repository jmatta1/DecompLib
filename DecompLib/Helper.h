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

#endif //DECOMPLIB_HELPER_H
