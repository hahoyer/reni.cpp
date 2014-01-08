#pragma once

template <typename T> T& ref(T*x) { assert(x); return *x; };
template <typename T> void deleteex(T*&x) { assert(x); delete x; x = NULL; };
template <typename T> void cdeleteex(T*&x) { if (x) delete x; x = NULL; };

template <typename T0, typename T1> bool const operator!=(T0 const&x, T1 const&y){ return !(x == y); };

#undef min
#undef max
template <class t> inline t min(t x, t y) { return (x < y) ? x : y; };
template <class t> inline t max(t x, t y) { return (x < y) ? y : x; };

template<typename t0, typename t1> inline t0 convert(t1 x)
{
    auto xx = t0(x);
    if (t1(xx) != x)
        throw String("Conversion overflow");
    return xx;
};

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
