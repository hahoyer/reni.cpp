#pragma once
#include "..\HWLib\DumpableObject.h"
#include "..\HWLib\DumpToString.h"

using namespace HWLib;

namespace Reni{
    class Size: public DumpableObject{
        using baseType = DumpableObject;
        using thisType = Size;
    public:
        int const value;
        Size(int value): value(value){}
        DefaultAssignmentOperator;
        Size const operator+(Size const&other)const{return value + other.value;};
        Size const operator*(int other)const{ return value *other; };
        bool const operator==(Size const&other)const{ return value == other.value; };
        bool const operator<(Size const&other)const{return value < other.value;};
        bool const operator<=(Size const&other)const{return value <= other.value;};

        p(String, format){return String::Convert(value);}

    private:
        p_function(Array<String>,DumpData) override{return{ HWLib::Dump(value) };};
        p_function(String, DumpShort) override{return HWLib::Dump(value);};
    };

    template<typename T>
    inline Size const BitCount(){return Size(sizeof(T)* 8);};
}

namespace HWLib{
    template<>
    class Constants<Reni::Size>{
    public:
        static Reni::Size const NotValid;
    };

    template<>
    class Constants<Reni::Size const>{
    public:
        static Reni::Size const NotValid;
    };
}
