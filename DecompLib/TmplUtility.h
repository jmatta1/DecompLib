#ifndef DECOMPLIB_TMPLUTILITY_H
#define DECOMPLIB_TMPLUTILITY_H

#include<cstdlib>//needed for aligned allocation

namespace Detail
{
/*!
 * @class AllocSelect
 * @brief A class to select the appropriate allocation scheme based on SIMD alignment
 * @author James Till Matta
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 * @tparam SimdAlign The alignment in bytes needed for any SIMD intrinsics that will be used. Values not in the set 16, 32, 64 will result in default allocation (using new[]).
 */
template<typename ParamType, int SimdAlign>
struct AllocSelect
{
    ParamType* allocate(int numCells)
    {
        return (new ParamType[numCells]);
    }
};

template<typename ParamType>
struct AllocSelect<ParamType, 16>
{
    const int AlignmentLength = 16;
    ParamType* allocate(int numCells)
    {
        size_t allocSize = (sizeof(ParamType)*numCells);
        if(allocSize%AlignmentLength) //if allocSize is not an integral multiple of AlignmentLength
        {
            allocSize = (((allocSize/AlignmentLength)+1)*AlignmentLength);
        }
        return reinterpret_cast<ParamType*>(aligned_alloc(Utility::AlignmentLength, allocSize));
    }
};

template<typename ParamType>
struct AllocSelect<ParamType, 32>
{
    const int AlignmentLength = 32;
    ParamType* allocate(int numCells)
    {
        size_t allocSize = (sizeof(ParamType)*numCells);
        if(allocSize%AlignmentLength) //if allocSize is not an integral multiple of AlignmentLength
        {
            allocSize = (((allocSize/AlignmentLength)+1)*AlignmentLength);
        }
        return reinterpret_cast<ParamType*>(aligned_alloc(Utility::AlignmentLength, allocSize));
    }
};

template<typename ParamType>
struct AllocSelect<ParamType, 64>
{
    const int AlignmentLength = 64;
    ParamType* allocate(int numCells)
    {
        size_t allocSize = (sizeof(ParamType)*numCells);
        if(allocSize%AlignmentLength) //if allocSize is not an integral multiple of AlignmentLength
        {
            allocSize = (((allocSize/AlignmentLength)+1)*AlignmentLength);
        }
        return reinterpret_cast<ParamType*>(aligned_alloc(Utility::AlignmentLength, allocSize));
    }
};


/*!
 * @class ReleaseArraySelect
 * @brief A class to select the appropriate deallocation scheme based on SIMD alignment
 * @author James Till Matta
 * 
 * @tparam ParamType The floating point type for which this calculation will be carried out
 * @tparam SimdAlign The alignment in bytes needed for any SIMD intrinsics that will be used. Values not in the set 16, 32, 64 will result in default allocation (using delete[]).
 */
template<typename ParamType, int SimdAlign>
struct ReleaseArraySelect
{
    void release(ParamType* ptr)
    {
        delete[] ptr;
    }
};

template<typename ParamType>
struct ReleaseArraySelect<ParamType, 16>
{
    void release(ParamType* ptr)
    {
        free(ptr);
    }
};

template<typename ParamType>
struct ReleaseArraySelect<ParamType, 32>
{
    void release(ParamType* ptr)
    {
        free(ptr);
    }
};

template<typename ParamType>
struct ReleaseArraySelect<ParamType, 64>
{
    void release(ParamType* ptr)
    {
        free(ptr);
    }
};
}

#endif //DECOMPLIB_TMPLUTILITY_H
