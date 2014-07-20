#pragma once

#include "Properties.h"

#undef min
#undef max
//#undef NULL

namespace HWLib
{
    template< class T > struct remove_const         { using type = T; };
    template< class T > struct remove_const<const T> { using type = T; };

    template< class T > struct remove_ref    { using type = T; };
    template< class T > struct remove_ref<T&> { using type = T; };

    template<typename TDestination, typename T> inline TDestination convert(T const&data)
    {
        auto result = TDestination(data);
        if (T(result) == data)
            return result;
        throw "Conversion overflow";
    };

    template <typename T0, typename T1> bool const operator!=(T0 const&x, T1 const&y){ return !(x == y); };

    template <class T> inline T min(T x, T y) { return (x < y) ? x : y; };
    template <class T> inline T max(T x, T y) { return (x < y) ? y : x; };
    template <class T> inline T sign(T x) { return x < 0 ? -1 : x > 0 ? 1 : 0; };
    int const NotANumber = 0x80000000l;

    /* Alignment: get the next number that is not less and a mutilple of 2^AlignBits
    examples:
    align(0,3) --> 0
    align(1,3) --> 8
    align(2,3) --> 8
    ...
    align(8,3) --> 8
    align(9,3) --> 16
    */
    template<class T> inline T align(T x, int AlignBits){ return (((x - 1) >> AlignBits) + 1) << AlignBits; };
}
