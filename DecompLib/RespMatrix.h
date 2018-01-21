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
template<typename ParamType>
class RespMatrix
{
public:
    /*!
     * \brief DecompVector Constructor
     * \param numFunctions The number of response functions
     * \param numRespCells The number of cells per response function
     */
    RespMatrix(int numFunctions, int numRespCells) : numFunc(numFunctions),
        respLen(numRespCells), matrix(new ParamType[numFunc*respLen]),
        matrix(new ParamType[numFunc*respLen]){}
    ~RespMatrix(){delete[] matrix; delete[] transpose;}
    
    /*!
     * \brief setElement sets the element in the matrix and its transpose
     * \param funcNum The index of the response function whose value is being set
     * \param respInd The index within the response function that is being set
     * \param val The value to set the cell to
     */
    void setElement(int funcNum, int respInd, ParamType val)
    {
        matrix[funcNum*respLen+respInd] = val;
        transpose[respInd*numFunc+funcNum] = val;
    }

    /*!
     * \brief getMatrixPtr gives access to the underlying matrix pointer
     * \return The underlying matrix pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getMatrixPtr(){return matrix;}
    
    /*!
     * \brief getTransposePtr gives access to the underlying matrix transpose pointer
     * \return The underlying matrix transpose pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getTransposePtr(){return transpose;}
private:
    int numFunc; ///<The number of response functions in the matrix
    int respLen; ///<The number of elements in each response function
    ParamType* matrix; ///<The response matrix
    ParamType* transpose; ///<The transpose of the response matrix
}


#endif //DECOMPLIB_RESPMATRIX_H
