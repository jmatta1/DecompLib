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
#ifndef DECOMPLIB_DATAVECTOR_H
#define DECOMPLIB_DATAVECTOR_H
#include"TmpUtility.h"

/*!
 * @class DataVector
 * @brief A representation of the vector of data that is the object of the decomposition
 * @author James Till Matta
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 * @tparam SimdAlign The alignment in bytes needed for any SIMD intrinsics that will be used. Values not in the set 16, 32, 64 will result in default allocation.
 */
template<typename ParamType, int SimdAlign = 16>
class DataVector
{
public:
    const int Alignment = SimdAlign; ///< A constant for other classes to check their SIMD Alignment against

    /*!
     * \brief DataVector Constructor
     * \param length, the number of values in the vector
     */
    DataVector(int length) : size(length), vec(nullptr){vec = Detail::AllocSelect<ParamType, SimdAlign>(size);}
    ~DataVector(){Detail::ReleaseArraySelect<ParamType, SimdAlign>(vec);}

    /*!
     * \brief setElement places a given value into a specific index in the vector
     * \param index The index into the vector
     * \param value The value to place at index
     * 
     * \remark if index is negative or greater than the length of the vector, behavior is undefined
     */
    void setElement(int index, ParamType value){vec[index] = value;}

    /*!
     * \brief getElement returns the value of the vector at index
     * \param index The index into the vector
     * \return the value of the vector at index
     * 
     * \remark if index is negative or greater than the length of the vector, behavior is undefined
     */
    ParamType getElement(int index){return vec[index];}

    /*!
     * \brief getLength gets the size of the vector
     * \return The number of cells in the vector
     */
    int getLength(){return size;}

    /*!
     * \brief getRawDataPtr gives access to the underlying vector pointer
     * \return The underlying vector pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getRawDataPtr(){return vec;}

    /*!
     * \brief isSafe checks if the data vector is safe for usage in the decomposition
     * \return True if safe, False if unsafe
     *
     * Checking the vector's safety involves testing that every element is greater
     * than or equal to zero and that the sum of all the elements is greater than
     * zero (i.e. the vector is not a zero vector)
     */
    bool isSafe();

private:
    int size; ///<The number of cells in the input data vector
    ParamType* vec; ///<The data vector itself
}

template<typename ParamType, int SimdAlign>
bool DataVector<ParamType, SimdAlign>::isSafe()
{
    //use kahan summation to check that the vector is non-zero
    ParamType sum = 0.0;
    ParamType comp = 0.0;
    for(int i=0; i<size; ++i)
    {
        if(vec[i] < static_cast<ParamType>(0.0)) return false; // here we have a negative value so we have failed
        ParamType compInput = (vec[i] - comp);
        ParamType tempSum = (sum + compInput);
        comp = ((tempSum - sum) - y);
        sum = tempSum;
    }
    if(sum <= static_cast<ParamType>(0.0)) return false; //if we made it here then we have a zero vector
    
    //if we made it to here then we have passed all safety checks
    return true;
}

#endif //DECOMPLIB_DATAVECTOR_H
