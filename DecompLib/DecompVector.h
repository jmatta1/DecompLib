/*!*****************************************************************************
********************************************************************************
**
** @file
** @brief Contains the definition of the DecompVector template class
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
#ifndef DECOMPLIB_DECOMPVECTOR_H
#define DECOMPLIB_DECOMPVECTOR_H
#include<cassert>
#include<iostream>
#include"DataVector.h"

/*!
 * @class DecompVector
 * @brief A representation of the vector of decomposition parameters that is passed into the decomposition as an initial guess and passed out as the final answer
 *
 * @ingroup IO
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 */
template<typename ParamType>
class DecompVector
{
public:
    /*!
     * \brief Constructor
     * \param length the number of values in the vector
     * \param printBadSafety Whether or not to print locations of errors in safety checks
     */
    DecompVector(int length, bool printBadSafety=true) :
        printErrors(printBadSafety), size(length), vec(new ParamType[length]){}

    ~DecompVector(){delete[] vec;}

    /*!
     * \brief Places a given value into a specific index in the vector
     * \param index The index into the vector
     * \param value The value to place at index
     * 
     * \remark if index is negative or greater than the length of the vector, behavior is undefined
     */
    void setElement(int index, ParamType value){vec[index] = value;}

    /*!
     * \brief Returns the value of the vector at index
     * \param index The index into the vector
     * \return the value of the vector at index
     * 
     * \remark if index is negative or greater than the length of the vector, behavior is undefined
     */
    ParamType getElement(int index) const{return vec[index];}

    /*!
     * \brief Gets the size of the vector
     * \return The number of cells in the vector
     */
    int getLength() const{return size;}

    /*!
     * \brief Initializes this vector with a constant parameter
     * \param val The value that every element of the vector should contain
     */
    void initWithConstant(const ParamType& val);

    /*!
     * \brief Initializes this vector by subsampling a DataVector
     * \param val The data vector that this decomp vector should be initialized from,
     * the length of this vector should be greater than or equal to the length of this vector
     * \param factor A factor to multiply the values from the data vector by
     * \param offset An offset to add to the values from the data vector
     */
    void initWithDataVector(const DataVector<ParamType>& val, const ParamType& factor, const ParamType& offset = 0.0);

    /*!
     * \brief Gives access to the underlying vector pointer
     * \return The underlying vector pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getRawDataPtr(){return vec;}

    /*!
     * \brief Checks if the decomp vector guess is safe for usage in the decomposition
     * \return True if safe, False if unsafe
     *
     * Checking the vector's safety involves testing that every element is
     * greater than zero
     */
    bool isSafe();

private:
    bool printErrors; ///<Stores if errors should be printed during safety checks
    int size; ///<The number of cells in the input data vector
    ParamType* vec; ///<The data vector itself
    static const double TinyValue = 1.0e-10; ///<Tiny value to use where the input spectrum is zero / too small in initialization
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
            //safety net to handle cases where the input spectrum is zero
            if(vec[i] < Detail::TinyValue)
            {
                vec[i] = Detail::TinyValue;
            }
        }
        else
        {
            vec[i] = ((factor*val.getElement(valSize-1))+offset);
            //safety net to handle cases where the input spectrum is zero
            if(vec[i] < Detail::TinyValue)
            {
                vec[i] = Detail::TinyValue;
            }
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
        if(vec[i] <= static_cast<ParamType>(0.0))
        {
            if(printErrors) std::cout<<"The decomposition vector has a zero or negative value"<<std::endl;
            output = false; // here we have a negative or zero value
        }
    }
    
    return output;
}

#endif //DECOMPLIB_DECOMPVECTOR_H
