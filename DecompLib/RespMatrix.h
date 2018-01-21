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
#ifndef DECOMPLIB_RESPMATRIX_H
#define DECOMPLIB_RESPMATRIX_H
#include"TmpUtility.h"
#include"DataVector.h"
#include"DecompVector.h"

/*!
 * @class RespMatrix
 * @brief A representation of the matrix response functions
 * @author James Till Matta
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 * @tparam SimdAlign The alignment in bytes needed for any SIMD intrinsics that will be used. Values not in the set 16, 32, 64 will result in default allocation.
 */
template<typename ParamType, int SimdAlign = 16>
class RespMatrix
{
public:
    const int Alignment = SimdAlign; ///< A constant for other classes to check their SIMD Alignment against

    /*!
     * \brief DecompVector Constructor
     * \param numFunctions The number of response functions
     * \param numRespCells The number of cells per response function
     */
    RespMatrix(int numFunctions, int numRespCells) : numFuncTrue(numFunctions),
    respLenTrue(numRespCells), {vec = Detail::AllocSelect<ParamType, SimdAlign>(size);}
    ~RespMatrix(){Detail::ReleaseArraySelect<ParamType, SimdAlign>(vec);}


private:
    int findRowAlignment(int numFunctions){}

    int numFuncTrue; ///<The number of response functions in the matrix
    int numFunc; ///<The number of response functions in the matrix (after tweaking for alignment purposes)
    int respLenTrue; ///<The number of elements in each response function
    int respLen; ///<The number of elements in each response function (after tweaking for alignment purposes)
    ParamType* matrix; ///<The response matrix
    ParamType* transpose; ///<The transpose of the response matrix
}


#endif //DECOMPLIB_RESPMATRIX_H
