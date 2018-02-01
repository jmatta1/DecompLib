/*!*****************************************************************************
********************************************************************************
**
** @file
** @brief Contains the definition of the RespMatrix template class
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
#ifndef DECOMPLIB_RESPMATRIX_H
#define DECOMPLIB_RESPMATRIX_H
#include<cassert>
#include<iostream>
#include"DataVector.h"
#include"DecompVector.h"

/*!
 * @class RespMatrix
 * @brief A representation of the matrix response functions
 * 
 * @ingroup IO
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 */
template<typename ParamType>
class RespMatrix
{
public:
    /*!
     * \brief Constructor
     * \param numFunctions The number of response functions
     * \param numRespCells The number of cells per response function
     */
    RespMatrix(int numFunctions, int numRespCells) : numFunc(numFunctions),
        respLen(numRespCells), matrix(new ParamType[numFunc*respLen]),
        transpose(new ParamType[numFunc*respLen]), summedRows(new ParamType[numFunc]){assert(numFunctions>0);assert(numRespCells>0);}
    ~RespMatrix(){delete[] matrix; delete[] transpose; delete[] summedRows;}
    
    /*!
     * \brief Sets the element in the matrix and its transpose
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
     * \brief Returns the value of the matrix at the specified cell
     * \param funcNum The index of the response function whose value is being set
     * \param respInd The index within the response function that is being set
     * \return The value of the matrix in the specified location
     */
    ParamType getElement(int funcNum, int respInd) const
    {
        return matrix[funcNum*respLen+respInd];
    }
    
     /*!
     * \brief Returns the value of the transpose at the specified cell
     * \param funcNum The index of the response function whose value is being set
     * \param respInd The index within the response function that is being set
     * \return The value of the matrix in the specified location
     * 
     * This is the same as get element but allows row major traversal even with
     * iterating on function number faster than response index
     */
    ParamType getElementTranspose(int funcNum, int respInd) const
    {
        return transpose[respInd*respLen+funcNum];
    }

    /*!
     * \brief Returns the number of response functions in the matrix
     * \return The number of response functions in the matrix
     */
    int getNumRespFuncs() const{return numFunc;}
    
    /*!
     * \brief Returns the length of each response function in the matrix
     * \return The length of the response functions in the matrix
     */
    int getRespFuncsLens() const{return respLen;}

    /*!
     * \brief Gives access to the underlying matrix pointer
     * \return The underlying matrix pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getMatrixPtr(){return matrix;}
    
    /*!
     * \brief Gives access to the underlying matrix transpose pointer
     * \return The underlying matrix transpose pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getTransposePtr(){return transpose;}

    /*!
     * \brief Gives access to the underlying pointer to the vector from summing the rows of the resp matrix
     * \return The underlying summed row vector pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getSummedRowPtr(){return summedRows;}

    /*!
     * \brief Calculates the summed rows vector used in the decomposition calculation
     */
    void calculateSummedRows();

    /*!
     * \brief Checks if the response matrix is safe for usage in the decomposition
     * \return True if safe, False if unsafe
     *
     * This makes sure that each of the rows and each of the columns sums to
     * greater than zero and contains no negative values
     */
    bool isSafe();

private:
    int numFunc; ///<The number of response functions in the matrix
    int respLen; ///<The number of elements in each response function
    ParamType* matrix; ///<The response matrix
    ParamType* transpose; ///<The transpose of the response matrix
    ParamType* summedRows; ///<The vector that results when each row of the matrix is summed independently
};

template<typename ParamType>
void RespMatrix<ParamType>::calculateSummedRows()
{
    for(int i=0; i<numFunc; ++i)
    {
        int offset = i*respLen;
        int edge = (offset+respLen);
        summedRows[i] = 0.0;
        ParamType comp = 0.0;
        for(int j=offset; j<edge; ++j)
        {
            ParamType compInput = (matrix[j] - comp);
            ParamType tempSum = (summedRows[i] + compInput);
            comp = ((tempSum - summedRows[i]) - compInput);
            summedRows[i] += matrix[j];
        }
    }
}

template<typename ParamType>
bool RespMatrix<ParamType>::isSafe()
{
    bool containsZeroRowOrCol = false;
    for(int i=0; i<numFunc; ++i)
    {
        bool zeroRow = true;
        int offset = (i*respLen);
        for(int j=0; j<respLen; ++j)
        {
            if(matrix[offset+j] > 0.0)
            {
                zeroRow = false;
            }
        }
        if(zeroRow)
        {
            containsZeroRowOrCol = true;
            std::cout<<"Row #"<<i<<" of the response matrix contains all zeros or negative values"<<std::endl;
        }
    }
    for(int i=0; i<respLen; ++i)
    {
        bool zeroCol = true;
        int offset = (i*numFunc);
        for(int j=0; j<numFunc; ++j)
        {
            if(transpose[offset+j] > 0.0)
            {
                zeroCol = false;
            }
        }
        if(zeroCol)
        {
            containsZeroRowOrCol = true;
            std::cout<<"Column #"<<i<<" of the response matrix contains all zeros or negative values"<<std::endl;
        }
    }
    return !containsZeroRowOrCol;
}

#endif //DECOMPLIB_RESPMATRIX_H
