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
#ifndef DECOMPLIB_DECOMPVECTOR_H
#define DECOMPLIB_DECOMPVECTOR_H
#include<cassert>
#include"DataVector.h"
#include<iostream>

/*!
 * @class DecompVector
 * @brief A representation of the vector of decomposition parameters that is passed into the decomposition as an initial guess and passed out as the final answer
 * @author James Till Matta
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 * @tparam SimdAlign The alignment in bytes needed for any SIMD intrinsics that will be used. Values not in the set 16, 32, 64 will result in default allocation.
 */
template<typename ParamType>
class DecompVector
{
public:
    /*!
     * \brief DecompVector Constructor
     * \param length, the number of values in the vector
     */
    DecompVector(int length) : size(length), vec(new ParamType[length]){}
    ~DecompVector(){delete[] vec;}

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
    ParamType getElement(int index) const{return vec[index];}

    /*!
     * \brief getLength gets the size of the vector
     * \return The number of cells in the vector
     */
    int getLength() const{return size;}

    /*!
     * \brief initWithConstant initializes this vector with a constant parameter
     * \param val The value that every element of the vector should contain
     */
    void initWithConstant(const ParamType& val);

    /*!
     * \brief initWithDataVector initializes this vector by compressing a DataVector
     * \param val The data vector that this decomp vector should be initialized from,
     * the length of this vector should be greater than or equal to the length of this vector
     * \param factor A factor to multiply the values from the data vector by
     * \param offset An offset to add to the values from the data vector
     */
    void initWithDataVector(const DataVector<ParamType>& val, const ParamType& factor, const ParamType& offset = 0.0);

    /*!
     * \brief getRawDataPtr gives access to the underlying vector pointer
     * \return The underlying vector pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getRawDataPtr(){return vec;}

    /*!
     * \brief isSafe checks if the decomp vector guess is safe for usage in the decomposition
     * \return True if safe, False if unsafe
     *
     * Checking the vector's safety involves testing that every element is
     * greater than zero
     */
    bool isSafe();

private:
    int size; ///<The number of cells in the input data vector
    ParamType* vec; ///<The data vector itself
};

template<typename ParamType>
void DecompVector<ParamType>::initWithDataVector(const DataVector<ParamType>& val, const ParamType& factor, const ParamType& offset)
{
    int valSize = val.getLength();
    assert(valSize >= size);
    ParamType stepSize = static_cast<ParamType>(valSize) / static_cast<ParamType>(size);
    for(int i=0; i<size; ++i)
    {
        int index = static_cast<int>(i*stepSize);
        if(index < valSize)
        {
            vec[i] = ((factor*val.getElement(index))+offset);
            if(vec[i] < static_cast<ParamType>(0.0)) std::cout<<"nvv: "<<val.getElement(index)<<", "<<vec[i]<<std::endl;
        }
        else
        {
            vec[i] = ((factor*val.getElement(valSize-1))+offset);
            if(vec[i] < static_cast<ParamType>(0.0)) std::cout<<" negative vector value"<<std::endl;
        }
        
    }
}

template<typename ParamType>
void DecompVector<ParamType>::initWithConstant(const ParamType& val)
{
    for(int i=0; i<size; ++i)
    {
        vec[i] = val;
    }
}

template<typename ParamType>
bool DecompVector<ParamType>::isSafe()
{
    bool output = true;
    for(int i=0; i<size; ++i)
    {
        //std::cout<<i<<", "<<vec[i]<<std::endl;
        if(vec[i] < static_cast<ParamType>(0.0)) output = false; // here we have a negative value so we have failed
    }
    
    //if we made it to here then all the parameters are greater than zero
    return output;
}

#endif //DECOMPLIB_DECOMPVECTOR_H
