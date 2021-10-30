#pragma once
#include "../HWLib/Optional.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Util{
    class Size: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Size;
    public:
        size_t const value;

        Size(size_t value) : value(value)
        {
            SetDumpString();
        }

        DefaultAssignmentOperator;
        Size const operator+(Size const&other)const{return value + other.value;};
        Size const operator-(Size const&other)const{ return value - other.value; };
        Size const operator*(size_t other)const{ return value * other; };
        bool const operator==(Size const&other)const{ return value == other.value; };
        bool const operator<(Size const&other)const{return value < other.value;};
        bool const operator<=(Size const&other)const{return value <= other.value;};
        bool const operator>(Size const&other)const{ return value > other.value; };
        void operator+=(Size const&other){ *this = *this + other; };
        void operator-=(Size const&other){ *this = *this - other; };

        Size const Align(size_t alignBits) const;

        p(String, format){ return String::Convert(value); }
        static thisType const Address;
    private:
        p_function(Array<String>,DumpData) override{return{ HWLib::Dump(value) };};
        p_function(String, DumpShort) override{return HWLib::Dump(value);};

    };

    template<typename T>
    inline Size const BitCount(){return Size(sizeof(T)* 8);};
}

