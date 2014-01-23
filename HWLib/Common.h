#pragma once

#include "Properties.h"

#undef min
#undef max
//#undef NULL

namespace HWLib
{
    template< class T > struct remove_const          { typedef T type; };
    template< class T > struct remove_const<const T> { typedef T type; };

    const class
    {
    public:
        template<typename T>
        operator T const*const ()const{ return 0; };
        operator std::nullptr_t ()const{ return nullptr; };
        operator std::nullptr_t(){ return nullptr; };
    } null;

    const class
    {
    public:
        template<typename T>
        operator T const ()const{ return T(); };
    } empty;

    template<typename TDestination, typename T> inline TDestination convert(T const&data)
    {
        auto result = TDestination(data);
        if (T(result) == data)
            return result;
        throw "Conversion overflow";
    };

    template <typename T0, typename T1> bool const operator!=(T0 const&x, T1 const&y){ return !(x == y); };

    template <class t> inline t min(t x, t y) { return (x < y) ? x : y; };
    template <class t> inline t max(t x, t y) { return (x < y) ? y : x; };

    int const NoANumber = 0x80000000l;

    /* Alignment: get the next number that is not less and a mutilple of 2^AlignBits
    examples:
    align(0,3) --> 0
    align(1,3) --> 8
    align(2,3) --> 8
    ...
    align(8,3) --> 8
    align(9,3) --> 16
    */
    template<class t> inline t align(t x, int AlignBits){ return (((x - 1) >> AlignBits) + 1) << AlignBits; };
}
