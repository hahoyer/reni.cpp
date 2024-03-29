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

        HW_DO_PLACEMENT_ASSIGN;
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

        HW_PR_GET(String, format){ return String::Convert(value); }
        static thisType const Address;
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{return{ HWLib::Dump(value) };};
        HW_PR_DECL_GETTER(string, DumpShort) override{return HWLib::Dump(value);};

    };

    template<typename T>
    inline Size BitCount() {return Size(sizeof(T)* 8);};
}

