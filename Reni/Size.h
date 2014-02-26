#pragma once
#include "..\HWLib\DumpableObject.h"
#include "..\HWLib\DumpToString.h"

using namespace HWLib;

namespace Reni
{
    class Size: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Size;
        int const value;
    public:
        Size(int value): value(value){}

        bool const operator==(Size const&other)const{ return value == other.value; };
        bool const operator<(Size const&other)const{ return value < other.value; };
        bool const operator<=(Size const&other)const{ return value <= other.value; };
        p(String, format){ return String::Convert(value); }
    private:
        override_p_function(Array<String>, DumpData){ return{ HWLib::Dump(value) }; };
    };


    template<typename T>
    inline Size const BitCount(){ return Size(sizeof(T)* 8); };

}
