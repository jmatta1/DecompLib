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
#ifndef DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMCALCULATION_H
#define DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMCALCULATION_H

#include"DecompLib/DataVector.h"
#include"DecompLib/DecompVector.h"
#include"DecompLib/RespMatrix.h"

//function that takes the decomposition result and the response matrix and finds the spectrum that would be observed
DataVector<double>* calculateSumSpectrum(const DecompVector<double>& dv, const RespMatrix<double>& respMat);
//function that takes the sum spectrum and the input spectrum and calculates the residuals
DataVector<double>* calculateResidSpectrum(const DataVector<double>& input, const DataVector<double>& sum);

#endif //DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SPECTRUMCALCULATION_H
