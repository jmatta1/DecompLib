/*!*****************************************************************************
********************************************************************************
**
** @file
** @brief Contains the definition of the DataVector template class
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
#ifndef DECOMPLIB_DATAVECTOR_H
#define DECOMPLIB_DATAVECTOR_H
#include<iostream>
/*!
 * @class DataVector
 * @brief A representation of the vector of data that is the object of the decomposition
 *
 * @ingroup IO
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 */
template<typename ParamType>
class DataVector
{
public:
    /*!
     * \brief Constructor
     * \param length the number of values in the vector
     * \param printBadSafety Whether or not to print locations of errors in safety checks
     */
    DataVector(int length, bool printBadSafety=true) :
        printErrors(printBadSafety), size(length), vec(new ParamType[length]){}

    ~DataVector(){delete[] vec;}

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
     * \brief Gives access to the underlying vector pointer
     * \return The underlying vector pointer
     *
     * \remark This function should only be used within decomp library
     */
    ParamType* getRawDataPtr(){return vec;}

    /*!
     * \brief Checks if the data vector is safe for usage in the decomposition
     * \return True if safe, False if unsafe
     *
     * Checking the vector's safety involves testing that every element is greater
     * than or equal to zero and that the sum of all the elements is greater than
     * zero (i.e. the vector is not a zero vector)
     */
    bool isSafe();

private:
    bool printErrors; ///<Stores if errors should be printed during safety checks
    int size; ///<The number of cells in the input data vector
    ParamType* vec; ///<The data vector itself
};

template<typename ParamType>
bool DataVector<ParamType>::isSafe()
{
    //initially assume the input spectrum is safe
    bool output = true;
    //since the input spectrum is allowed to have some zeros, check to make sure that
    //the *entire* vector is not zero using kahan summation (while also checking
    //to be certain that there are no negative values)
    ParamType sum = 0.0;
    ParamType comp = 0.0;
    for(int i=0; i<size; ++i)
    {
        if(vec[i] < static_cast<ParamType>(0.0))
        {
            if(printErrors) std::cout<<"Bin number "<<i<<" in input spectrum is negative"<<std::endl;
            output = false;
            
        }
        ParamType compInput = (vec[i] - comp);
        ParamType tempSum = (sum + compInput);
        comp = ((tempSum - sum) - compInput);
        sum = tempSum;
    }
    if(sum <= static_cast<ParamType>(0.0))//we have a zero or negative vector
    {
        if(printErrors) std::cout<<"Sum of all bins in input spectrum was zero or negative"<<std::endl;
        output = false;
    }
    
    return output;
}

#endif //DECOMPLIB_DATAVECTOR_H
