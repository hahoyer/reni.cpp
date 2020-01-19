#pragma once
#include <set>


namespace HWLib{
    template<class T>
    class MemBox{
        T value;
    public:
        MemBox(){ memset(&value, 0, sizeof(value)); }
        T const& operator*()const { return value; };
        T const* operator->()const { return &value; };
        T & operator*(){ return value; };
        T * operator->(){ return &value; };
        operator T* (){ return &value; };
        operator T const* ()const{ return &value; };
    };
}
