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
        Size operator+(Size const& other) const {return value + other.value;};
        Size operator-(Size const& other) const { return value - other.value; };
        Size operator*(size_t other) const { return value * other; };
        bool operator==(Size const& other) const { return value == other.value; };
        bool operator<(Size const& other) const {return value < other.value;};
        bool operator<=(Size const& other) const {return value <= other.value;};
        bool operator>(Size const& other) const { return value > other.value; };
        void operator+=(Size const&other){ *this = *this + other; };
        void operator-=(Size const&other){ *this = *this - other; };

        Size Align(size_t alignBits) const;

        p(String, format){ return String::Convert(value); }
        static thisType const Address;
    private:
        p_function(Array<string>,DumpData) override{return{ HWLib::Dump(value) };};
        p_function(string, DumpShort) override{return HWLib::Dump(value);};

    };

    template<typename T>
    inline Size BitCount() {return Size(sizeof(T)* 8);};
}

